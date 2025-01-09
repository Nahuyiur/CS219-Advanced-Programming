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
    // 构造函数，初始化MiniVim对象
    MiniVim(const vector<string>& filenames)//可以一次性接受多个文件名
    : cursor_x(0), cursor_y(0), top_line(0), left_column(0), insert_mode_active(false), command_mode_active(false) {
        // 将所有文件名添加到文件历史
        file_history = filenames;
        current_file_index = 0;  // 默认加载第一个文件
        loadFile();               // 加载第一个文件
    }

    // 析构函数，结束ncurses模式
    ~MiniVim() { endwin(); }

    // 主循环，处理用户输入和界面更新
    void run() {
        while (true) {
            draw();  // 绘制界面
            char ch = getch();  // 获取用户输入
            if (command_mode_active) {
                command_mode(ch);  // 处理命令模式输入
            } else if (insert_mode_active) {
                insert_mode(ch);  // 处理插入模式输入
            } else {
                normal_mode(ch);  // 处理普通模式输入
            }
        }
    }
    
    // 初始化ncurses环境
    void init() {
        initscr();  // 初始化屏幕
        keypad(stdscr, TRUE);  // 启用键盘功能键
        noecho();  // 关闭输入回显
        cbreak();  // 禁用行缓冲
        raw();  // 禁用Ctrl+C等信号
        curs_set(TRUE);  // 显示光标
        getmaxyx(stdscr, screen_height, screen_width);  // 获取屏幕尺寸
        refresh();  // 刷新屏幕
    }

private:
    vector<string> file_history; // 文件历史列表
    size_t current_file_index;   // 当前文件的索引
    string filename;             // 当前文件名
    vector<string> lines;        // 当前文件内容
    int cursor_x = 0, cursor_y = 0;  // 光标位置
    int top_line = 0, left_column = 0;  // 窗口滚动位置
    int screen_width, screen_height;  // 屏幕尺寸
    bool insert_mode_active;  // 插入模式是否激活
    bool command_mode_active;  // 命令模式是否激活
    string command_buffer;  // 命令缓冲区
    string copied_line;  // 复制的行内容
    stack<pair<string, pair<int, string>>> undo_stack; // 撤销栈 {操作类型, {行号, 操作文本}}
    stack<pair<string, pair<int, string>>> redo_stack; // 重做栈

    // 加载当前文件
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

    // 保存当前文件
    void saveFile() {
        ofstream file(filename);
        if (file.is_open()) {
            for (const string& line : lines) {
                file << line << endl;
            }
            file.close();
        }
    }

    // 调整窗口滚动位置以适应光标
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

    // 绘制界面
    void draw() {
        clear();  // 清屏
        int line_number_width = 5;  // 行号宽度

        // 绘制文件内容
        for (int i = top_line; i < lines.size() && i < top_line + screen_height - 2; ++i) {
            stringstream ss;
            ss << setw(line_number_width) << right << (i + 1) << " | ";  // 行号

            string visible_text;
            if (left_column < lines[i].length()) {
                visible_text = lines[i].substr(left_column, screen_width - line_number_width - 3);  // 可见文本
            } else {
                visible_text = "";
            }
            mvprintw(i - top_line, 0, "%s%s", ss.str().c_str(), visible_text.c_str());  // 打印行号和文本
        }

        // 确保光标位置在有效范围内
        if (cursor_y >= lines.size()) cursor_y = lines.size() - 1;
        cursor_x = min(cursor_x, (int)lines[cursor_y].length() - 1);
        cursor_x = max(cursor_x, 0);

        // 移动光标到正确位置
        move(cursor_y - top_line, cursor_x - left_column + line_number_width + 3);

        // 高亮光标位置
        attron(A_STANDOUT);
        if (cursor_x >= lines[cursor_y].length()) {
            mvprintw(cursor_y - top_line, cursor_x - left_column + line_number_width + 3, " ");
        } else {
            mvprintw(cursor_y - top_line, cursor_x - left_column + line_number_width + 3, "%c", lines[cursor_y][cursor_x]);
        }
        attroff(A_STANDOUT);

        // 绘制状态栏和命令显示
        attron(A_REVERSE);
        mvprintw(screen_height - 2, 0, " MODE: %s | FILE: %s ", 
            insert_mode_active ? "INSERT" : (command_mode_active ? "COMMAND" : "NORMAL"),
            filename.c_str());
        clrtoeol();
        mvprintw(screen_height - 1, 0, ": %s", command_buffer.c_str());
        clrtoeol();
        attroff(A_REVERSE);

        refresh();  // 刷新屏幕
    }

    // 处理搜索和替换命令
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

        // 替换文本
        while ((pos = current_line.find(old_text, pos)) != string::npos) {
            current_line.replace(pos, old_text.length(), new_text);
            if (!global) break;
            pos += new_text.length();
        }

        // 更新光标位置
        cursor_x = min(cursor_x, (int)lines[cursor_y].length() - 1);
        adjust_window();
        draw();
    }

    // 检查字符串是否为数字
    bool is_number(const string& str) {
        return !str.empty() && all_of(str.begin(), str.end(), ::isdigit);
    }

    // 处理普通模式输入
    void normal_mode(int ch) {
        switch (ch) {
            case 'h':
            case 4:
                if (cursor_x > 0) --cursor_x;  // 左移光标
                adjust_window();
                break;
            case 'j':
            case 2:
                if (cursor_y < lines.size() - 1) ++cursor_y;  // 下移光标
                adjust_window();
                break;
            case 'k':
            case 3:
                if (cursor_y > 0) --cursor_y;  // 上移光标
                adjust_window();
                break;
            case 'l':
            case 5:
                if (cursor_x < lines[cursor_y].size()) ++cursor_x;  // 右移光标
                adjust_window();
                break;
            case 'i':
                insert_mode_active = true;  // 进入插入模式
                break;
            case ':':
                command_mode_active = true;  // 进入命令模式
                command_buffer.clear();
                break;
            case '0':
                cursor_x = 0;  // 移动到行首
                adjust_window();
                break;
            case '$':
                if (!lines[cursor_y].empty()) {
                    cursor_x = lines[cursor_y].length() - 1;  // 移动到行尾
                } else {
                    cursor_x = 0;
                }
                adjust_window();
                break;
            case 'g':
                if (getch() == 'g') {
                    cursor_y = 0;  // 移动到文件开头
                    adjust_window();
                }
                break;
            case 'G':
                cursor_y = lines.size() - 1;  // 移动到文件末尾
                adjust_window();
                break;
            case 'd': 
                if (getch() == 'd' && cursor_y < lines.size()) {
                    string deleted_line = lines[cursor_y];  // 删除当前行
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
                    copied_line = lines[cursor_y];  // 复制当前行
                }
                break;
            case 'p': 
                if (!copied_line.empty()) {
                    undo_stack.push({"paste", {cursor_y, copied_line}});
                    lines.insert(lines.begin() + cursor_y + 1, copied_line);  // 粘贴复制的行
                    ++cursor_y;
                }
                adjust_window();
                break;
            case 'u': // 撤销操作
                undo();
                break;
            case 18: // Ctrl+r - 重做操作
                redo();
                break;
            default:
                break;
        }
    }

    // 处理插入模式输入
    void insert_mode(int ch) {
        switch (ch) {
            case 27:
                insert_mode_active = false;  // 退出插入模式
                break;
            case 4:
                if (cursor_x > 0) --cursor_x;  // 左移光标
                adjust_window();
                break;
            case 5:
                if (cursor_x < lines[cursor_y].length()) ++cursor_x;  // 右移光标
                adjust_window();
                break;
            case 3:
                if (cursor_y > 0) --cursor_y;  // 上移光标
                adjust_window();
                break;
            case 2:
                if (cursor_y < lines.size() - 1) ++cursor_y;  // 下移光标
                adjust_window();
                break;
            case 10:
                {
                    string new_line = lines[cursor_y].substr(cursor_x);  // 插入新行
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
                    lines[cursor_y].erase(cursor_x - 1, 1);  // 删除字符
                    --cursor_x;
                } else if (cursor_y > 0) {
                    cursor_x = lines[cursor_y - 1].length();
                    lines[cursor_y - 1] += lines[cursor_y];  // 合并行
                    lines.erase(lines.begin() + cursor_y);
                    --cursor_y;
                }
                adjust_window();
                break;
            default:
                if (cursor_x > lines[cursor_y].length()) {
                    lines[cursor_y].append(cursor_x - lines[cursor_y].length(), ' ');  // 插入空格
                }
                lines[cursor_y].insert(cursor_x, 1, ch);  // 插入字符
                ++cursor_x;
                adjust_window();
                break;
        }
    }

    // 处理命令模式输入
    void command_mode(int ch) {
        if (ch == 27) {
            command_mode_active = false;  // 退出命令模式
            command_buffer.clear();
            return;
        }
        if (ch == 10) {
            if (command_buffer == "q") {
                endwin();
                exit(0);  // 退出程序
            } else if (command_buffer == "w") {
                saveFile();  // 保存文件
            } else if (command_buffer == "wq") {
                saveFile();  // 保存并退出
                endwin();
                exit(0);
            } else if (command_buffer.rfind("s/", 0) == 0) {
                handle_search_replace(command_buffer);  // 处理搜索替换
            } else if (is_number(command_buffer)) {
                int target_line = stoi(command_buffer);
                if (target_line >= 1 && target_line <= lines.size()) {
                    cursor_y = target_line - 1;  // 跳转到指定行
                    adjust_window();
                    cursor_x = min(cursor_x, (int)lines[cursor_y].size());
                }
            } else if (command_buffer.rfind("e ", 0) == 0) {
                string new_filename = command_buffer.substr(2);
                file_history.push_back(new_filename);  // 打开新文件
                current_file_index = file_history.size() - 1;
                loadFile();
                cursor_x = 0;
                cursor_y = 0;
                top_line = 0;
                left_column = 0;
            } else if (command_buffer == "N") {
                if (current_file_index > 0) {
                    --current_file_index;  // 切换到上一个文件
                    loadFile();
                    cursor_x = 0;
                    cursor_y = 0;
                    top_line = 0;
                    left_column = 0;
                }
            } else if (command_buffer == "n") {
                if (current_file_index < file_history.size() - 1) {
                    ++current_file_index;  // 切换到下一个文件
                    loadFile();
                    cursor_x = 0;
                    cursor_y = 0;
                    top_line = 0;
                    left_column = 0;
                }
            } else if (command_buffer == "ls") {
                clear();
                for (size_t i = 0; i < file_history.size(); ++i) {
                    mvprintw(i, 0, "%zu: %s", i + 1, file_history[i].c_str());  // 列出所有文件
                }
                refresh();
                getch(); // 等待用户按任意键
            } else if (command_buffer.rfind("b ", 0) == 0) {
                string buffer_number_str = command_buffer.substr(2);
                if (is_number(buffer_number_str)) {
                    size_t buffer_number = stoi(buffer_number_str) - 1;
                    if (buffer_number < file_history.size()) {
                        current_file_index = buffer_number;  // 切换到指定缓冲区
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
                command_buffer.pop_back();  // 删除命令缓冲区中的字符
            }
            return;
        }
        command_buffer += ch;  // 添加字符到命令缓冲区
    }

    // 撤销操作
    void undo() {
        if (!undo_stack.empty()) {
            auto operation = undo_stack.top();
            undo_stack.pop();

            redo_stack.push(operation);

            if (operation.first == "delete") {
                lines.insert(lines.begin() + operation.second.first, operation.second.second);  // 撤销删除
            } else if (operation.first == "paste") {
                lines.erase(lines.begin() + operation.second.first);  // 撤销粘贴
            }
        }
    }

    // 重做操作
    void redo() {
        if (!redo_stack.empty()) {
            auto operation = redo_stack.top();
            redo_stack.pop();

            undo_stack.push(operation);

            if (operation.first == "delete") {
                lines.erase(lines.begin() + operation.second.first);  // 重做删除
            } else if (operation.first == "paste") {
                lines.insert(lines.begin() + operation.second.first, operation.second.second);  // 重做粘贴
            }
        }
    }
};

// 主函数
int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Usage: %s <file1> <file2> ... <fileN>\n", argv[0]);
        return 1;
    }

    vector<string> filenames;
    for (int i = 1; i < argc; ++i) {
        filenames.push_back(argv[i]);  // 获取命令行参数中的文件名
    }
    MiniVim editor(filenames);  // 创建MiniVim对象
    editor.init();  // 初始化
    editor.run();  // 运行
    return 0;
}