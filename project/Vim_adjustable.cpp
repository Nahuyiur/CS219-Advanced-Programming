#include <ncurses.h>
#include <string>
#include <vector>
#include <fstream>
#include <iomanip>
#include <sstream>
#include <algorithm>

using namespace std;

class MiniVim {
public:
    MiniVim(const string& filename): filename(filename), cursor_x(0), cursor_y(0), top_line(0), left_column(0), insert_mode_active(false), command_mode_active(false) {} // [MODIFIED] 添加 top_line 和 left_column

    ~MiniVim() { endwin(); }

    void run() {
        while (true) {
            draw();
            char ch = getch();
            if (ch == 'q' && !command_mode_active && !insert_mode_active) break; // 临时退出键
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
        loadFile();
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
    string filename;
    vector<string> lines;
    int cursor_x = 0, cursor_y = 0;
    int top_line = 0, left_column = 0; // [ADDED] 当前显示的第一行和最左列
    int screen_width, screen_height;
    bool insert_mode_active;
    bool command_mode_active;
    string command_buffer;
    string copied_line;

    void loadFile() {
        ifstream file(filename);
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

    void adjust_window() { // [ADDED] 滚动窗格逻辑
        // 垂直滚动
        if (cursor_y < top_line) {
            top_line = cursor_y; // 如果光标在当前窗口上方，则向上滚动
        } else if (cursor_y >= top_line + screen_height - 2) {
            top_line = cursor_y - (screen_height - 3); // 如果光标在当前窗口下方，则向下滚动
        }

        // 水平滚动
        if (cursor_x < left_column) {
            left_column = cursor_x; // 如果光标在当前窗口左侧，则向左滚动
        } else if (cursor_x >= left_column + screen_width - 10) { // [MODIFIED] 10 留出行号和分隔符的宽度
            left_column = cursor_x - (screen_width - 11); // 滚动窗口，确保光标可见
        }
    }

    void draw() {
        // 主编辑区
        clear();
        int line_number_width = 5;  // 行号宽度，确保行号对齐
        for (int i = top_line; i < lines.size() && i < top_line + screen_height - 2; ++i) { // [MODIFIED] 根据 top_line 调整显示范围
            stringstream ss;
            ss << setw(line_number_width) << right << (i + 1) << " | "; // 行号 + 分隔符

            // 截取需要显示的部分
            string visible_text = lines[i].substr(left_column, screen_width - line_number_width - 3); // [MODIFIED] 根据 left_column 截取
            mvprintw(i - top_line, 0, "%s%s", ss.str().c_str(), visible_text.c_str()); // [MODIFIED] 调整显示位置
        }
        move(cursor_y - top_line, cursor_x - left_column + line_number_width + 3); // [MODIFIED] 调整光标位置

        // 光标高亮
        attron(A_STANDOUT);
        if (cursor_x < lines[cursor_y].length()) {
            mvprintw(cursor_y - top_line, cursor_x - left_column + line_number_width + 3, "%c", lines[cursor_y][cursor_x]); // [MODIFIED] 调整光标位置
        }
        attroff(A_STANDOUT);

        // 底部状态栏
        attron(A_REVERSE);
        mvprintw(screen_height - 2, 0, " MODE: %s ", insert_mode_active ? "INSERT" : (command_mode_active ? "COMMAND" : "NORMAL"));
        attroff(A_REVERSE);

        // 命令输入窗格
        attron(A_REVERSE);
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
            return;  // 如果格式错误，直接返回
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
                adjust_window(); // [MODIFIED]
                break;
            case 'j':
            case 2:
                if (cursor_y < lines.size() - 1) ++cursor_y;
                adjust_window(); // [MODIFIED]
                break;
            case 'k':
            case 3:
                if (cursor_y > 0) --cursor_y;
                adjust_window(); // [MODIFIED]
                break;
            case 'l':
            case 5:
                if (cursor_x < lines[cursor_y].size()) ++cursor_x;
                adjust_window(); // [MODIFIED]
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
                adjust_window(); // [MODIFIED]
                break;
            case '$':
                cursor_x = lines[cursor_y].length();
                adjust_window(); // [MODIFIED]
                break;
            case 'g':
                if (getch() == 'g') {
                    cursor_y = 0;
                    adjust_window(); // [MODIFIED]
                }
                break;
            case 'G':
                cursor_y = lines.size() - 1;
                adjust_window(); // [MODIFIED]
                break;
            case 'd': 
                if (getch() == 'd' && cursor_y < lines.size()) {
                    lines.erase(lines.begin() + cursor_y);
                    if (lines.empty()) {
                        lines.push_back("");
                    }
                    if (cursor_y >= lines.size()) {
                        --cursor_y;
                        cursor_x = min(cursor_x, (int)lines[cursor_y].size());
                    }
                }
                adjust_window(); // [MODIFIED]
                break;
            case 'y': 
                if (getch() == 'y') {
                    copied_line = lines[cursor_y];
                }
                break;
            case 'p': 
                if (!copied_line.empty()) {
                    lines.insert(lines.begin() + cursor_y + 1, copied_line);
                    ++cursor_y;
                }
                adjust_window(); // [MODIFIED]
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
                adjust_window(); // [MODIFIED]
                break;
            case 5:
                if (cursor_x < lines[cursor_y].size()) ++cursor_x;
                adjust_window(); // [MODIFIED]
                break;
            case 3:
                if (cursor_y > 0) --cursor_y;
                adjust_window(); // [MODIFIED]
                break;
            case 2:
                if (cursor_y < lines.size() - 1) ++cursor_y;
                adjust_window(); // [MODIFIED]
                break;
            case 10:
                {
                    string new_line = lines[cursor_y].substr(cursor_x);
                    lines[cursor_y] = lines[cursor_y].substr(0, cursor_x);
                    lines.insert(lines.begin() + cursor_y + 1, new_line);
                    ++cursor_y;
                    cursor_x = 0;
                    adjust_window(); // [MODIFIED]
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
                adjust_window(); // [MODIFIED]
                break;
            default:
                lines[cursor_y].insert(cursor_x, 1, ch);
                ++cursor_x;
                adjust_window(); // [MODIFIED]
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
                    adjust_window(); // [MODIFIED]
                    cursor_x = min(cursor_x, (int)lines[cursor_y].size());
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
