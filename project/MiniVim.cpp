#include <ncurses.h>
#include <string>
#include <vector>
#include <fstream>
using namespace std;

Class MiniVim {
public:
    MiniVim(const string& filename): filename(filename), cursor_x(0), cursor_y(0), insert_mode(false), command_mode_active(false) {}
    
    ~MiniVim() {endwin();}

    void run() {
        while (true) {
            if(command_mode_active){
                //TODO
            }
            else if(insert_mode_active){
                //TODO
            }
            else{
                normal_mode(getch());
            }
        }
    }

private:
    string filename;
    vector<string> lines;
    int cursor_x, cursor_y;
    int screen_width, screen_height;
    bool insert_mode_active;
    bool command_mode_active;

    void init() {
        initscr();
        noecho();
        cbreak();
        raw();
        keypad(stdscr, TRUE);
        curs_set(TRUE);
        getmaxyx(stdscr, screen_height, screen_width);
    }

    void loadFile() {}

    void saveFile() {}

    void normal_mode(int ch) {
        switch (ch) {
            case 'h':
                move_cursor_left();
                break;
            case 'j':
                move_cursor_down();
                break;
            case 'k':
                move_cursor_up();
                break;
            case 'l':
                move_cursor_right();
                break;
            case '0':
                cursor_x = 0;
                break;
            case '$':
                cursor_x = lines[cursor_y].length();
                break;
            case 'i':
                insert_mode = true;
                break;
            case ':':
                command_mode_active = true;
                break;
            default:
                break;
        }
    }

    void insert_mode(int ch){}

    void command_mode(int ch){}

    void move_cursor_left(){}

    void move_cursor_right(){}

    void move_cursor_up(){}

    void move_cursor_down(){}

    void add_char(char c){}

    void delete_char(){}

    void undo(){}

    void redo(){}
};  

int main() {
    //判断命令行参数是不是有两个 e.g. ./minivim filename.txt
    if (argc != 2) {
        printw("Usage: %s <filename>\n", arg[0]);
        return 1;
    }

    MiniVim editor(arg[1]);    
    editor.init();
    editor.run();

    return 0;
}
