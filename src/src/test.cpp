#include <ncurses.h>

int main(int argc, char **argv)
{
    initscr();
    clear();
    raw();
    noecho();
    refresh();
    timeout(-1);

    for (size_t i = 0; i < 10; i++)
    {
        int k = getch();
        mvaddch(i,i,k);
        //refresh();
    }
    endwin();
    return 0;
}
