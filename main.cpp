#include <ncurses.h>
#include <iostream>
#include <map>
#include <stdexcept>
#include <cctype>
#include <string>
#include <algorithm>

struct Vec2
{
    int x, y;

    Vec2() : x(0), y(0) {}
    Vec2(int x, int y) : x(x), y(y) {}
};

namespace Helpers
{
    namespace Window
    {
        Vec2 GetWindowBounds()
        {
            Vec2 bounds;
            getmaxyx(stdscr, bounds.y, bounds.x);
            return bounds;
        }
    };
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
        Size newSize = static_cast<Size>(static_cast<int>(m_Size) + 1);
        if (m_Size < Size::Large && canScale(newSize))
        {
            m_Size = newSize;
        }
    }

    void ScaleDown()
    {
        Size newSize = static_cast<Size>(static_cast<int>(m_Size) - 1);
        if (m_Size > Size::Small && canScale(newSize))
        {
            m_Size = newSize;
        }
    }

    int GetWidth()
    {
        return m_Sizes.at(m_Size).x;
    }

    int GetHeight()
    {
        return m_Sizes.at(m_Size).y;
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

    Vec2 GetPosition()
    {
        return m_Position;
    }

private:
    Size m_Size = Size::Medium;
    Vec2 m_Position;
    char m_Character;

    std::map<Size, Vec2> m_Sizes{
        { Size::Small, { 18, 7 } },
        { Size::Medium, { 27, 9 } },
        { Size::Large, { 41, 11 } },
    };

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

    bool canScale(Size size)
    {
        Vec2 bounds;
        getmaxyx(stdscr, bounds.y, bounds.x);

        Vec2 scaledDimensions = m_Sizes.at(size);

        return (m_Position.x + scaledDimensions.x <= bounds.x) &&
               (m_Position.y + scaledDimensions.y <= bounds.y);
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
                if (m_Shape.GetPosition().y > 0)
                {
                    m_Shape.Move(0, -1);
                }
                break;
            case KEY_DOWN:
                if (m_Shape.GetPosition().y < GetWindowBounds().y - m_Shape.GetHeight())
                {
                    m_Shape.Move(0, 1);
                }
                break;
            case KEY_LEFT:
                if (m_Shape.GetPosition().x > 0)
                {
                    m_Shape.Move(-1, 0);
                }
                break;
            case KEY_RIGHT:
                if (m_Shape.GetPosition().x < GetWindowBounds().x - m_Shape.GetWidth())
                {
                    m_Shape.Move(1, 0);
                }
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

        for (char& c : input.size)
        {
            c = tolower(c);
        }

        m_Shape.SetCharacter(input.character);
        m_Shape.SetSize(Shape::SizeFromString(input.size));
    }

    Vec2 GetWindowBounds()
    {
        Vec2 bounds;
        getmaxyx(stdscr, bounds.y, bounds.x);
        return bounds;
    }
};

int main()
{
    Application application;
    application.Run();
}
