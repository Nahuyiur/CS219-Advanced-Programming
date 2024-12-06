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
        noecho();
        cbreak();
        raw();
        keypad(stdscr, TRUE);
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
            case KEY_LEFT:
                if (cursor_x > 0) --cursor_x;
                break;
            case 'j':
                if (cursor_y < lines.size() - 1 && cursor_x < lines[cursor_y+1].length()) ++cursor_y;
                else if (cursor_y < lines.size() - 1) {
                    ++cursor_y;
                    cursor_x = lines[cursor_y].length();
                }
                break;
            case KEY_DOWN:
                if (cursor_y < lines.size() - 1 && cursor_x < lines[cursor_y+1].length()) ++cursor_y;
                else if (cursor_y < lines.size() - 1) {
                    ++cursor_y;
                    cursor_x = lines[cursor_y].length();
                }
                break;
            case 'k':
                if (cursor_y > 0 && cursor_x < lines[cursor_y-1].length()) --cursor_y;
                else if (cursor_y > 0) {
                    --cursor_y;
                    cursor_x = lines[cursor_y].length();
                }
                break;
            case KEY_UP:
                if (cursor_y > 0 && cursor_x < lines[cursor_y-1].length()) --cursor_y;
                else if (cursor_y > 0) {
                    --cursor_y;
                    cursor_x = lines[cursor_y].length();
                }
                break;
            case 'l':
                if (cursor_x < lines[cursor_y].length()) ++cursor_x;
                break;
            case KEY_RIGHT:
                if (cursor_x < lines[cursor_y].length()) ++cursor_x;
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

    void insert_mode(int ch){
        // ch==27 ESC键
        if (ch==27){
            insert_mode_active=false;
        }
        //TODO
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
