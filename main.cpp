#include <ncurses.h>
#include <iostream>
#include <stdexcept>

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

    static Size SizeFromString(const std::string& size)
    {
        if (size == "small") return Size::Small;
        if (size == "medium") return Size::Medium;
        if (size == "large") return Size::Large;
        throw std::invalid_argument("Invalid size: " + size);
    }

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
            drawOuterLine(m_Position.x, m_Position.y, 18);
            drawInnerLine(m_Position.x, m_Position.y + 1, 0, 2);
            drawInnerLine(m_Position.x, m_Position.y + 2, 1, 2);
            drawInnerLine(m_Position.x, m_Position.y + 3, 2, 2);
            drawInnerLine(m_Position.x, m_Position.y + 4, 1, 2);
            drawInnerLine(m_Position.x, m_Position.y + 5, 0, 2);
            drawOuterLine(m_Position.x, m_Position.y + 6, 18);
            break;
        case Size::Medium:
            drawOuterLine(m_Position.x, m_Position.y, 27);
            drawInnerLine(m_Position.x, m_Position.y + 1, 0, 3);
            drawInnerLine(m_Position.x, m_Position.y + 2, 1, 3);
            drawInnerLine(m_Position.x, m_Position.y + 3, 2, 3);
            drawInnerLine(m_Position.x, m_Position.y + 4, 3, 3);
            drawInnerLine(m_Position.x, m_Position.y + 5, 2, 3);
            drawInnerLine(m_Position.x, m_Position.y + 6, 1, 3);
            drawInnerLine(m_Position.x, m_Position.y + 7, 0, 3);
            drawOuterLine(m_Position.x, m_Position.y + 8, 27);
            break;
        case Size::Large:
            drawOuterLine(m_Position.x, m_Position.y, 41);
            drawInnerLine(m_Position.x, m_Position.y + 1, 0, 4);
            drawInnerLine(m_Position.x, m_Position.y + 2, 1, 4);
            drawInnerLine(m_Position.x, m_Position.y + 3, 2, 4);
            drawInnerLine(m_Position.x, m_Position.y + 4, 3, 4);
            drawInnerLine(m_Position.x, m_Position.y + 5, 4, 4);
            drawInnerLine(m_Position.x, m_Position.y + 6, 3, 4);
            drawInnerLine(m_Position.x, m_Position.y + 7, 2, 4);
            drawInnerLine(m_Position.x, m_Position.y + 8, 1, 4);
            drawInnerLine(m_Position.x, m_Position.y + 9, 0, 4);
            drawOuterLine(m_Position.x, m_Position.y + 10, 41);
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

    void SetSize(const Size size)
    {
        m_Size = size;
    }

    void SetCharacter(const char character)
    {
        m_Character = character;
    }

private:
    Size m_Size = Size::Medium;
    Vec2 m_Position;
    char m_Character;

    void drawOuterLine(int x, int y, int length)
    {
        std::string line(length, m_Character);
        mvprintw(y, x, line.c_str());
    }

    void drawInnerLine(int x, int y, int depth, int length)
    {
        std::string space(length * depth, ' ');
        mvprintw(y, x, space.c_str());
        drawOuterLine(x + space.length(), y, length);
    }
};

class Application
{
public:
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
        readSettings();

        Initialize();

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

        Clean();
    }

private:
    Shape m_Shape;

    void readSettings()
    {
        using std::cout, std::cin, std::string;

        struct Input
        {
            char character; 
            string size;
        } input;

        cout << "Character: ";
        cin >> input.character;

        cout << "Size (small, medium, large): ";
        cin >> input.size;

        m_Shape.SetCharacter(input.character);
        m_Shape.SetSize(Shape::SizeFromString(input.size));
    }
};

int main()
{
    Application application;
    application.Run();
}
