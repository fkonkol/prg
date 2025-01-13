#include <ncurses.h>

class Application
{
public:
    Application() {
        Initialize();
    }

    ~Application() {
        Clean();
    }

    void Initialize()
    {
        initscr();
        cbreak();
        noecho();
        keypad(stdscr, TRUE);
        curs_set(FALSE);
    }

    void Clean()
    {
        endwin();
    }

    void Run()
    {
        printw("Hello");
        getch();
    }
};

int main()
{
    Application application;
    application.Run();
}
