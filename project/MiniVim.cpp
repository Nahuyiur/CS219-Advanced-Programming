#include <ncurses.h>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

class MiniVim {
public:
    MiniVim(const string& filename): filename(filename), cursor_x(0), cursor_y(0), insert_mode_active(false), command_mode_active(false) {}
    
    ~MiniVim() {endwin();}

    void run() {
        while (true) {
            draw();
            char ch = getch();
            if (ch == 'q') break; // 临时退出键
            if(command_mode_active){
                command_mode(ch);
            }
            else if(insert_mode_active){
                insert_mode(ch);
            }
            else{
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
    int cursor_x=0, cursor_y=0;
    int screen_width, screen_height;
    bool insert_mode_active;
    bool command_mode_active;
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
    }

    void saveFile() {}

    void draw() {
        clear();
        for (size_t i = 0; i < lines.size(); ++i) {
            mvprintw(i, 0, "%s", lines[i].c_str());
        }
        move(cursor_y, cursor_x);
        refresh();
    }

    void normal_mode(int ch) {
        switch (ch) {
            case 'h':
                if (cursor_x > 0) --cursor_x;
                break;
            case 4:
                if (cursor_x > 0) --cursor_x;
                break;
            case 'j':
                if (cursor_y < lines.size() - 1 && cursor_x < lines[cursor_y+1].length()) ++cursor_y;
                else if (cursor_y < lines.size() - 1) {
                    ++cursor_y;
                    cursor_x = lines[cursor_y].length()-1;
                }
                break;
            case 2:
                if (cursor_y < lines.size() - 1 && cursor_x < lines[cursor_y+1].length()) ++cursor_y;
                else if (cursor_y < lines.size() - 1) {
                    ++cursor_y;
                    cursor_x = lines[cursor_y].length()-1;
                }
                break;
            case 'k':
                if (cursor_y > 0 && cursor_x < lines[cursor_y-1].length()) --cursor_y;
                else if (cursor_y > 0) {
                    --cursor_y;
                    cursor_x = lines[cursor_y].length()-1;
                }
                break;
            case 3:
                if (cursor_y > 0 && cursor_x < lines[cursor_y-1].length()) --cursor_y;
                else if (cursor_y > 0) {
                    --cursor_y;
                    cursor_x = lines[cursor_y].length()-1;
                }
                break;
            case 'l':
                if (cursor_x < lines[cursor_y].length()-1) ++cursor_x;
                break;
            case 5:
                if (cursor_x < lines[cursor_y].length()-1) ++cursor_x;
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
                    ch=0;
                }
                else break;
                break;
            case 'G':
                cursor_y = lines.size() - 1;
                cursor_x = 0;
                break;
            case 'd': 
                if (getch() == 'd' && cursor_y < lines.size()) {
                    lines.erase(lines.begin() + cursor_y);
                    if (lines.empty()) {
                        lines.push_back("");
                    }
                    if (cursor_y >= lines.size()) {
                        --cursor_y;
                        if(cursor_x>=lines[cursor_y].size()) cursor_x=lines[cursor_y].size()-1;
                    }
                    if (cursor_x >= lines[cursor_y].size()) cursor_x=lines[cursor_y].size()-1;
                    break;
                }
                else break;   
            case 'y': // 复制当前行
                if (getch() == 'y') {
                    copied_line = lines[cursor_y];  // 保存当前行
                }
                break;
            case 'p': // 粘贴复制的行
                if (!copied_line.empty()) {
                    lines.insert(lines.begin() + cursor_y + 1, copied_line);
                    cursor_y++;
                }
                break;
            case 'i':
                insert_mode_active = true;
                break;
            case ':':
                command_mode_active = true;
                break;
            default:
                break;
        }
    }

    void insert_mode(int ch) {
        if (ch == 27) {  // ESC key to exit insert mode
            insert_mode_active = false;
        } else if (ch == 10) {  // Enter key to create a new line
            // Split the current line at the cursor position and create a new line
            string new_line = lines[cursor_y].substr(cursor_x);
            lines[cursor_y] = lines[cursor_y].substr(0, cursor_x);
            lines.insert(lines.begin() + cursor_y + 1, new_line);
            cursor_y++;
            cursor_x = 0;
        } else if (ch == 7 || ch == KEY_BACKSPACE) {  // Backspace to delete
            if (cursor_x > 0) {
                lines[cursor_y].erase(cursor_x - 1, 1);
                cursor_x--;
            } else if (cursor_y > 0) {  // Handle backspace at the beginning of a line (merge with the previous line)
                cursor_x = lines[cursor_y - 1].length();
                lines[cursor_y - 1] += lines[cursor_y];
                lines.erase(lines.begin() + cursor_y);
                cursor_y--;
            }
        } else {
            // Insert the character at the current cursor position
            lines[cursor_y].insert(cursor_x, 1, ch);
            cursor_x++;
        }
    }

    void command_mode(int ch){
        //TODO
    }

    void add_char(char c){}

    void delete_char(){}

    void undo(){}

    void redo(){}
};  

int main(int argc, char* argv[]) {
    keypad(stdscr, TRUE);
    //判断命令行参数是不是有两个 e.g. ./minivim filename.txt
    if (argc != 2) {
        printw("Usage: %s <filename>\n", argv[0]);
        return 1;
    }

    MiniVim editor(argv[1]);    
    editor.init();
    editor.run();
    return 0;
}
