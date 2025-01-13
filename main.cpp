#include <ncurses.h>

struct Vec2
{
    int x, y;

    Vec2() : x(0), y(0) {}
};

class Shape
{
public:
    enum class Size
    {
        Small, Medium, Large,
    };

    Size GetSize()
    {
        return m_Size;
    }

    void ScaleUp()
    {
        if (m_Size < Shape::Size::Large)
        {
            m_Size = static_cast<Size>(static_cast<int>(m_Size) + 1);
        }
    }

    void ScaleDown()
    {
        if (m_Size > Shape::Size::Small)
        {
            m_Size = static_cast<Size>(static_cast<int>(m_Size) - 1);
        }
    }

    void Draw()
    {
        switch (m_Size)
        {
        case Size::Small:
            mvprintw(m_Position.y, m_Position.x, "Small");
            break;
        case Size::Medium:
            mvprintw(m_Position.y, m_Position.x, "Medium");
            break;
        case Size::Large:
            mvprintw(m_Position.y, m_Position.x, "Large");
            break;
        default:
            break;
        }
    }

    void Move(const int xa, const int ya)
    {
        m_Position.x += xa;
        m_Position.y += ya;
    }

private:
    Size m_Size = Size::Medium;
    Vec2 m_Position;
};

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
        while (true)
        {
            clear();
            m_Shape.Draw();
            refresh();

            int key = getch();

            if (key == 'q') {
                break;
            }

            switch (key)
            {
            case KEY_UP:
                m_Shape.Move(0, -1);
                break;
            case KEY_DOWN:
                m_Shape.Move(0, 1);
                break;
            case KEY_LEFT:
                m_Shape.Move(-1, 0);
                break;
            case KEY_RIGHT:
                m_Shape.Move(1, 0);
                break;
            case '+':
                m_Shape.ScaleUp();
                break;
            case '-':
                m_Shape.ScaleDown();
                break;
            default:
                break;
            }
        }
    }

private:
    Shape m_Shape;
};

int main()
{
    Application application;
    application.Run();
}
