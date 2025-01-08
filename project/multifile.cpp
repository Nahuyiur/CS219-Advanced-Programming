#include <ncurses.h>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>
#include <stack>
using namespace std;

class MiniVim {
public:
    MiniVim(const string& filename): cursor_x(0), cursor_y(0), top_line(0), left_column(0), insert_mode_active(false), command_mode_active(false) {
        file_history.push_back(filename);
        current_file_index = 0;
        loadFile();
    }

    ~MiniVim() { endwin(); }

    void run() {
        while (true) {
            draw();
            char ch = getch();
            if (command_mode_active) {
                command_mode(ch);
            } else if (insert_mode_active) {
                insert_mode(ch);
            } else {
                normal_mode(ch);
            }
        }
    }
    
    void init() {
        initscr();
        keypad(stdscr, TRUE);
        noecho();
        cbreak();
        raw();
        curs_set(TRUE);
        getmaxyx(stdscr, screen_height, screen_width);
        refresh();
    }

private:
    vector<string> file_history; // 文件历史列表
    size_t current_file_index;   // 当前文件的索引
    string filename;             // 当前文件名
    vector<string> lines;        // 当前文件内容
    int cursor_x = 0, cursor_y = 0;
    int top_line = 0, left_column = 0;
    int screen_width, screen_height;
    bool insert_mode_active;
    bool command_mode_active;
    string command_buffer;
    string copied_line;
    stack<pair<string, pair<int, string>>> undo_stack; // {操作类型, {行号, 操作文本}}
    stack<pair<string, pair<int, string>>> redo_stack;

    void loadFile() {
        filename = file_history[current_file_index];
        ifstream file(filename);
        lines.clear();
        if (file.is_open()) {
            string line;
            while (getline(file, line)) {
                lines.push_back(line);
            }
            file.close();
        }
        if (lines.empty()) lines.push_back("");
    }

    void saveFile() {
        ofstream file(filename);
        if (file.is_open()) {
            for (const string& line : lines) {
                file << line << endl;
            }
            file.close();
        }
    }

    void adjust_window() { 
        // 垂直滚动
        if (cursor_y < top_line) {
            top_line = cursor_y;
        } else if (cursor_y >= top_line + screen_height - 2) {
            top_line = cursor_y - (screen_height - 3);
        }

        // 水平滚动
        if (cursor_x < left_column) {
            left_column = cursor_x;
        } else if (cursor_x >= left_column + screen_width - 10) {
            left_column = cursor_x - (screen_width - 11);
        }
    }

    void draw() {
        clear();
        int line_number_width = 5;

        for (int i = top_line; i < lines.size() && i < top_line + screen_height - 2; ++i) {
            stringstream ss;
            ss << setw(line_number_width) << right << (i + 1) << " | ";

            string visible_text;
            if (left_column < lines[i].length()) {
                visible_text = lines[i].substr(left_column, screen_width - line_number_width - 3);
            } else {
                visible_text = "";
            }
            mvprintw(i - top_line, 0, "%s%s", ss.str().c_str(), visible_text.c_str());
        }

        // 确保光标位置
        if (cursor_y >= lines.size()) cursor_y = lines.size() - 1;
        cursor_x = min(cursor_x, (int)lines[cursor_y].length() - 1);
        cursor_x = max(cursor_x, 0);

        move(cursor_y - top_line, cursor_x - left_column + line_number_width + 3);

        // 高亮光标位置
        attron(A_STANDOUT);
        if (cursor_x >= lines[cursor_y].length()) {
            mvprintw(cursor_y - top_line, cursor_x - left_column + line_number_width + 3, " ");
        } else {
            mvprintw(cursor_y - top_line, cursor_x - left_column + line_number_width + 3, "%c", lines[cursor_y][cursor_x]);
        }
        attroff(A_STANDOUT);

        // 状态栏和命令显示
        attron(A_REVERSE);
       mvprintw(screen_height - 2, 0, " MODE: %s | FILE: %s ", 
         insert_mode_active ? "INSERT" : (command_mode_active ? "COMMAND" : "NORMAL"),
         filename.c_str());
        clrtoeol();
        mvprintw(screen_height - 1, 0, ": %s", command_buffer.c_str());
        clrtoeol();
        attroff(A_REVERSE);

        refresh();
    }

    void handle_search_replace(const string& command) {
        size_t first_slash = command.find('/');
        size_t second_slash = command.find('/', first_slash + 1);
        size_t third_slash = command.find('/', second_slash + 1);

        if (first_slash == string::npos || second_slash == string::npos) {
            return;
        }

        string old_text = command.substr(first_slash + 1, second_slash - first_slash - 1);
        string new_text = (third_slash != string::npos) ? command.substr(second_slash + 1, third_slash - second_slash - 1)
                                                        : command.substr(second_slash + 1);

        bool global = (third_slash != string::npos && command.substr(third_slash + 1) == "g");

        string& current_line = lines[cursor_y];
        size_t pos = 0;

        while ((pos = current_line.find(old_text, pos)) != string::npos) {
            current_line.replace(pos, old_text.length(), new_text);
            if (!global) break;
            pos += new_text.length();
        }

        // 更新 cursor_x 到替换文本的最后
        cursor_x = min(cursor_x, (int)lines[cursor_y].length() - 1);
        adjust_window();
        draw();
    }

    bool is_number(const string& str) {
        return !str.empty() && all_of(str.begin(), str.end(), ::isdigit);
    }

    void normal_mode(int ch) {
        switch (ch) {
            case 'h':
            case 4:
                if (cursor_x > 0) --cursor_x;
                adjust_window();
                break;
            case 'j':
            case 2:
                if (cursor_y < lines.size() - 1) ++cursor_y;
                adjust_window();
                break;
            case 'k':
            case 3:
                if (cursor_y > 0) --cursor_y;
                adjust_window();
                break;
            case 'l':
            case 5:
                if (cursor_x < lines[cursor_y].size()) ++cursor_x;
                adjust_window();
                break;
            case 'i':
                insert_mode_active = true;
                break;
            case ':':
                command_mode_active = true;
                command_buffer.clear();
                break;
            case '0':
                cursor_x = 0;
                adjust_window();
                break;
            case '$':
                if (!lines[cursor_y].empty()) {
                    cursor_x = lines[cursor_y].length() - 1;
                } else {
                    cursor_x = 0;
                }
                adjust_window();
                break;
            case 'g':
                if (getch() == 'g') {
                    cursor_y = 0;
                    adjust_window();
                }
                break;
            case 'G':
                cursor_y = lines.size() - 1;
                adjust_window();
                break;
            case 'd': 
                if (getch() == 'd' && cursor_y < lines.size()) {
                    string deleted_line = lines[cursor_y];
                    undo_stack.push({"delete", {cursor_y, deleted_line}});
                    lines.erase(lines.begin() + cursor_y);
                    if (lines.empty()) {
                        lines.push_back("");
                    }
                    if (cursor_y >= lines.size()) {
                        --cursor_y;
                        cursor_x = min(cursor_x, (int)lines[cursor_y].size());
                    }
                }
                adjust_window();
                break;
            case 'y': 
                if (getch() == 'y') {
                    copied_line = lines[cursor_y];
                }
                break;
            case 'p': 
                if (!copied_line.empty()) {
                    undo_stack.push({"paste", {cursor_y, copied_line}});
                    lines.insert(lines.begin() + cursor_y + 1, copied_line);
                    ++cursor_y;
                }
                adjust_window();
                break;
            case 'u': // 撤销
                undo();
                break;
            case 18: // Ctrl+r - 重做
                redo();
                break;
            default:
                break;
        }
    }

    void insert_mode(int ch) {
        switch (ch) {
            case 27:
                insert_mode_active = false;
                break;
            case 4:
                if (cursor_x > 0) --cursor_x;
                adjust_window();
                break;
            case 5:
                if (cursor_x < lines[cursor_y].length()) ++cursor_x;
                adjust_window();
                break;
            case 3:
                if (cursor_y > 0) --cursor_y;
                adjust_window();
                break;
            case 2:
                if (cursor_y < lines.size() - 1) ++cursor_y;
                adjust_window();
                break;
            case 10:
                {
                    string new_line = lines[cursor_y].substr(cursor_x);
                    lines[cursor_y] = lines[cursor_y].substr(0, cursor_x);
                    lines.insert(lines.begin() + cursor_y + 1, new_line);
                    ++cursor_y;
                    cursor_x = 0;
                    adjust_window();
                }
                break;
            case 7:
            case KEY_BACKSPACE:
                if (cursor_x > 0) {
                    lines[cursor_y].erase(cursor_x - 1, 1);
                    --cursor_x;
                } else if (cursor_y > 0) {
                    cursor_x = lines[cursor_y - 1].length();
                    lines[cursor_y - 1] += lines[cursor_y];
                    lines.erase(lines.begin() + cursor_y);
                    --cursor_y;
                }
                adjust_window();
                break;
            default:
                if (cursor_x > lines[cursor_y].length()) {
                    lines[cursor_y].append(cursor_x - lines[cursor_y].length(), ' ');
                }
                lines[cursor_y].insert(cursor_x, 1, ch);
                ++cursor_x;
                adjust_window();
                break;
        }
    }

    void command_mode(int ch) {
        if (ch == 27) {
            command_mode_active = false;
            command_buffer.clear();
            return;
        }
        if (ch == 10) {
            if (command_buffer == "q") {
                endwin();
                exit(0);
            } else if (command_buffer == "w") {
                saveFile();
            } else if (command_buffer == "wq") {
                saveFile();
                endwin();
                exit(0);
            } else if (command_buffer.rfind("s/", 0) == 0) {
                handle_search_replace(command_buffer);
            } else if (is_number(command_buffer)) {
                int target_line = stoi(command_buffer);
                if (target_line >= 1 && target_line <= lines.size()) {
                    cursor_y = target_line - 1;
                    adjust_window();
                    cursor_x = min(cursor_x, (int)lines[cursor_y].size());
                }
            } else if (command_buffer.rfind("e ", 0) == 0) {
                string new_filename = command_buffer.substr(2);
                file_history.push_back(new_filename);
                current_file_index = file_history.size() - 1;
                loadFile();
                cursor_x = 0;
                cursor_y = 0;
                top_line = 0;
                left_column = 0;
            } else if (command_buffer == "N") {
                if (current_file_index > 0) {
                    --current_file_index;
                    loadFile();
                    cursor_x = 0;
                    cursor_y = 0;
                    top_line = 0;
                    left_column = 0;
                }
            } else if (command_buffer == "n") {
                if (current_file_index < file_history.size() - 1) {
                    ++current_file_index;
                    loadFile();
                    cursor_x = 0;
                    cursor_y = 0;
                    top_line = 0;
                    left_column = 0;
                }
            } else if (command_buffer == "ls") {
                clear();
                for (size_t i = 0; i < file_history.size(); ++i) {
                    mvprintw(i, 0, "%zu: %s", i + 1, file_history[i].c_str());
                }
                refresh();
                getch(); // 等待用户按任意键
            } else if (command_buffer.rfind("b ", 0) == 0) {
                string buffer_number_str = command_buffer.substr(2);
                if (is_number(buffer_number_str)) {
                    size_t buffer_number = stoi(buffer_number_str) - 1;
                    if (buffer_number < file_history.size()) {
                        current_file_index = buffer_number;
                        loadFile();
                        cursor_x = 0;
                        cursor_y = 0;
                        top_line = 0;
                        left_column = 0;
                    }
                }
            }
            command_buffer.clear();
            command_mode_active = false;
            return;
        }
        if (ch == 7 || ch == KEY_BACKSPACE) {
            if (!command_buffer.empty()) {
                command_buffer.pop_back();
            }
            return;
        }
        command_buffer += ch;
    }

    void undo() {
        if (!undo_stack.empty()) {
            auto operation = undo_stack.top();
            undo_stack.pop();

            redo_stack.push(operation);

            if (operation.first == "delete") {
                lines.insert(lines.begin() + operation.second.first, operation.second.second);
            } else if (operation.first == "paste") {
                lines.erase(lines.begin() + operation.second.first);
            }
        }
    }

    void redo() {
        if (!redo_stack.empty()) {
            auto operation = redo_stack.top();
            redo_stack.pop();

            undo_stack.push(operation);

            if (operation.first == "delete") {
                lines.erase(lines.begin() + operation.second.first);
            } else if (operation.first == "paste") {
                lines.insert(lines.begin() + operation.second.first, operation.second.second);
            }
        }
    }
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    MiniVim editor(argv[1]);
    editor.init();
    editor.run();

    return 0;
}