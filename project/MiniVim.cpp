#include <ncurses.h>
#include <string>
#include <vector>
using namespace std;

Class MiniVim {
private:
    vector<string> lines;
    int cursor_x, cursor_y;
    int screen_width, screen_height;
    bool insert_mode;
    bool command_mode_active;
public:
    MiniVim(): cursor_x(0), cursor_y(0), insert_mode(false), command_mode_active(false) {}
    void init();
    void run();
    void normal_mode(int ch);
    void insert_mode(int ch);
    void command_mode(int ch);

    void move_cursor_left();
    void move_cursor_right();
    void move_cursor_up();
    void move_cursor_down();
    void add_char(char c);
    void delete_char();
    void undo();
    void redo();
};  

void MiniVim::init() {
    initscr();
    raw();
    keypad(stdscr, TRUE);
    noecho();
    curs_set(TRUE);
    getmaxyx(stdscr, screen_height, screen_width);
}