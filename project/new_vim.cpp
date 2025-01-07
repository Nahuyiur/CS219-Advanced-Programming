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
    MiniVim(const string& filename): filename(filename), cursor_x(0), cursor_y(0), insert_mode_active(false), command_mode_active(false) {}
    
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
    int screen_width, screen_height;
    bool insert_mode_active;
    bool command_mode_active;
    string command_buffer;
    string copied_line;
        // 撤销与重做的栈
    stack<pair<string, pair<int, string>>> undo_stack; // {操作类型, {行号, 操作文本}}
    stack<pair<string, pair<int, string>>> redo_stack;

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

    void draw() {
        // 主编辑区
        clear();
        int line_number_width = 5;  // 行号宽度，确保行号对齐
        for (size_t i = 0; i < lines.size() && i < screen_height - 2; ++i) {
            // 显示行号
            stringstream ss;
            ss << setw(line_number_width) << right << (i + 1) << " | "; // 行号 + 分隔符
            mvprintw(i, 0, "%s%s", ss.str().c_str(), lines[i].c_str());
        }
        move(cursor_y, cursor_x + line_number_width + 3); // 光标位置需要偏移行号宽度

        // 光标高亮
        attron(A_STANDOUT);
        if (cursor_x < lines[cursor_y].length()) {
            mvprintw(cursor_y, cursor_x + line_number_width + 3, "%c", lines[cursor_y][cursor_x]);
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
        // 检查命令格式是否正确 s/old/new/g
        size_t first_slash = command.find('/');  // 找到第一个 '/'
        size_t second_slash = command.find('/', first_slash + 1);  // 找到第二个 '/'
        size_t third_slash = command.find('/', second_slash + 1);  // 找到第三个 '/'

        if (first_slash == string::npos || second_slash == string::npos) {
            return;  // 如果格式错误，直接返回
        }

        // 提取 old 和 new 字符串
        string old_text = command.substr(first_slash + 1, second_slash - first_slash - 1);
        string new_text = (third_slash != string::npos) ? command.substr(second_slash + 1, third_slash - second_slash - 1)
                                                        : command.substr(second_slash + 1);

        bool global = (third_slash != string::npos && command.substr(third_slash + 1) == "g");

        // 替换当前行中的内容
        string& current_line = lines[cursor_y];
        size_t pos = 0;

        while ((pos = current_line.find(old_text, pos)) != string::npos) {
            current_line.replace(pos, old_text.length(), new_text);
            if (!global) break;  // 如果不是全局替换，只替换第一个匹配项
            pos += new_text.length();
        }

        draw();  // 替换后刷新屏幕显示
    }

    bool is_number(const string& str) {
        return !str.empty() && all_of(str.begin(), str.end(), ::isdigit);
    }       


    void normal_mode(int ch) {
        switch (ch) {
            case 'h':
            case 4:
                if (cursor_x > 0) --cursor_x;
                break;
            case 'j':
            case 2:
                if (cursor_y < lines.size() - 1) ++cursor_y;
                if (cursor_x > lines[cursor_y].size()) cursor_x = lines[cursor_y].size();
                break;
            case 'k':
            case 3:
                if (cursor_y > 0) --cursor_y;
                if (cursor_x > lines[cursor_y].size()) cursor_x = lines[cursor_y].size();
                break;
            case 'l':
            case 5:
                if (cursor_x < lines[cursor_y].size()) ++cursor_x;
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
                break;
            case '$':
                cursor_x = lines[cursor_y].length();
                break;
            case 'g':
                if (getch() == 'g') {
                    cursor_y = 0;
                    cursor_x = 0;
                }
                break;
            case 'G':
                cursor_y = lines.size() - 1;
                cursor_x = 0;
                break;
            case 'd':
                if (getch() == 'd' && cursor_y < lines.size()) {
                    // 记录删除操作到撤销栈，存储删除行的内容和行号
                    string deleted_line = lines[cursor_y];
                    undo_stack.push({"delete", {cursor_y, deleted_line}});
                    lines.erase(lines.begin() + cursor_y);
                    if (lines.empty()) lines.push_back("");
                    if (cursor_y >= lines.size()) {
                        --cursor_y;
                        cursor_x = min(cursor_x, (int)lines[cursor_y].size());
                    }
                }
                break;

            case 'y': // 复制当前行
                if (getch() == 'y') {
                    copied_line = lines[cursor_y];
                }
                break;
            case 'p': // 粘贴复制的行
                if (!copied_line.empty()) {
                    // 记录粘贴操作到撤销栈，存储粘贴的内容和插入位置
                    undo_stack.push({"paste", {cursor_y, copied_line}});
                    lines.insert(lines.begin() + cursor_y + 1, copied_line);
                    ++cursor_y;
                }
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
            case 27:  // ESC 键退出插入模式
                insert_mode_active = false;
                break;
            case 4:  // 左方向键
                if (cursor_x > 0) --cursor_x;
                break;
            case 5:  // 右方向键
                if (cursor_x < lines[cursor_y].size()) ++cursor_x;
                break;
            case 3:  // 上方向键
                if (cursor_y > 0) {
                    --cursor_y;
                    if (cursor_x > lines[cursor_y].size()) cursor_x = lines[cursor_y].size();
                }
                break;
            case 2:  // 下方向键
                if (cursor_y < lines.size() - 1) {
                    ++cursor_y;
                    if (cursor_x > lines[cursor_y].size()) cursor_x = lines[cursor_y].size();
                }
                break;
            case 10:  // Enter 键创建新行
                {
                    string new_line = lines[cursor_y].substr(cursor_x);
                    lines[cursor_y] = lines[cursor_y].substr(0, cursor_x);
                    lines.insert(lines.begin() + cursor_y + 1, new_line);
                    ++cursor_y;
                    cursor_x = 0;
                }
                break;
            case 7:
            case KEY_BACKSPACE:  // Backspace 删除
                if (cursor_x > 0) {
                    lines[cursor_y].erase(cursor_x - 1, 1);
                    --cursor_x;
                } else if (cursor_y > 0) {
                    cursor_x = lines[cursor_y - 1].length();
                    lines[cursor_y - 1] += lines[cursor_y];
                    lines.erase(lines.begin() + cursor_y);
                    --cursor_y;
                }
                break;
            default:  // 其他字符插入
                lines[cursor_y].insert(cursor_x, 1, ch);
                ++cursor_x;
                break;
        }
    }


    void command_mode(int ch) {
        if (ch == 27) {  // ESC 键退出命令模式
            command_mode_active = false;
            command_buffer.clear();
            return;
        }
        if (ch == 10) {  // 回车键执行命令
            if (command_buffer == "q") {
                endwin();
                exit(0);
            } else if (command_buffer == "w") {
                saveFile();
            } else if (command_buffer == "wq") {
                saveFile();
                endwin();
                exit(0);
            } else if (command_buffer.rfind("s/", 0) == 0) {  // 搜索替换命令
                handle_search_replace(command_buffer);
            }else if(is_number(command_buffer)){
                int target_line = stoi(command_buffer);  // 转换为整数
                if (target_line >= 1 && target_line <= lines.size()) {
                    cursor_y = target_line - 1;  // 跳转到目标行
                    cursor_x = min(cursor_x, (int)lines[cursor_y].size());  // 确保光标不超出行长度
                }
            }
            command_buffer.clear();
            command_mode_active = false;
            return;
        }
        if (ch == 7 || ch == KEY_BACKSPACE) {  // Backspace 键删除字符
            if (!command_buffer.empty()) {
                command_buffer.pop_back();
            }
            return;
        }
        command_buffer += ch;
    }
    // 撤销操作
    void undo() {
        if (!undo_stack.empty()) {
            auto operation = undo_stack.top();
            undo_stack.pop();

            // 将撤销的操作存入 redo 栈
            redo_stack.push(operation);

            if (operation.first == "delete") {
                // 恢复删除的行
                lines.insert(lines.begin() + operation.second.first, operation.second.second);
            } else if (operation.first == "paste") {
                // 恢复粘贴的行
                lines.erase(lines.begin() + operation.second.first);
            }
        }
    }

    // 重做操作
    void redo() {
        if (!redo_stack.empty()) {
            auto operation = redo_stack.top();
            redo_stack.pop();

            // 将重做的操作存入 undo 栈
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
