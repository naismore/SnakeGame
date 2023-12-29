// Отрисовка поля
// Отрисовка яблока

#include <string>
#include <iterator>
#include <iostream>
#include <algorithm>
#include <vector>
#include <cstdlib>
#include <map>
#include <Windows.h>
#include <thread>
#include <chrono>
#include <conio.h>
#include <queue>

using FieldType = std::vector<std::vector<char>>;
using Tail = std::queue<std::pair<int, int>>;

enum Directions
{
    up,
    down,
    right,
    left
};

class Snake
{
public:
    char itemChar;
    //std::vector<std::map<int, int>> coords;
    int coordX;
    int coordY;
    std::queue<std::pair<int, int>> tail;
};

class Apple
{
public:
    char itemChar;
    int coordX;
    int coordY;
};

class Field
{
public:
    char wallChar;
    char fieldChar;
    int height;
    int width;
};

Apple InitApple(char itemChar, int fieldHeight, int fieldWidth)
{
    int appleCoordX = rand() % fieldWidth;
    int appleCoordY = rand() % fieldHeight;
    Apple apple =
    {
        itemChar,
        appleCoordX,
        appleCoordY,
    };
    return apple;
}

FieldType InitField(Field field, Snake snake, Apple apple, Tail tail) // Инициализация поля. Добавляем, отрисовываем.
{
    FieldType vectorField;
    std::vector<char> fieldString;
    // Initialize field strings
    bool externalString = true;
    bool externalChar = true;
    for (int i = 0; i < field.height + 2; i++)
    {
        if (i == field.height + 1) { externalString = true; };
        for (int j = 0; j < field.width + 2; j++)
        {
            Tail tempTail = tail;
            if (externalString)
            {
                fieldString.push_back(field.wallChar);
            }
            else
            {
                if (j == 0 || j == field.width + 1) { externalChar = true; };
                if (externalChar) 
                {
                    fieldString.push_back(field.wallChar);
                    externalChar = false;
                }
                else
                {
                    if(j == apple.coordX && i == apple.coordY)
                    {
                        fieldString.push_back(apple.itemChar);
                    }
                    else if(j == snake.coordX && i == snake.coordY)
                    {
                        fieldString.push_back(snake.itemChar);
                    }
                    else
                    {
                        fieldString.push_back(field.fieldChar);
                    }
                    while (!tempTail.empty())
                    {
                        if (j == tempTail.front().first && i == tempTail.front().second)
                        {
                            fieldString.push_back(snake.itemChar);
                            tempTail.pop();
                        }
                    }
                }
            }
        }
        vectorField.push_back(fieldString);
        externalString = false;
        fieldString.clear();
    }
    return vectorField;
}

Tail Move(Snake snake, Tail tail, bool eatedApple)
{
    if (!eatedApple)
    {
        if (!tail.empty())
        {
            tail.pop();
            tail.push({ snake.coordX, snake.coordY });
        }
    }
    else
    {
        tail.push({ snake.coordX, snake.coordY });
    }
    return tail;
}

void gotoxy(int x, int y) // Переход в начало консоли
{
    COORD position;
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    position.X = x;
    position.Y = y;
    SetConsoleCursorPosition(hConsole, position);
}

void PrintField(FieldType field, int score) // Вывод поля
{
    for (std::vector<char> fieldString : field)
    {
        std::copy(fieldString.begin(), fieldString.end(), std::ostream_iterator<char>(std::cout, ""));
        std::cout << std::endl;
    }
    std::cout << "Ваш счет:" << score << std::flush;
}

int main()
{
    setlocale(LC_ALL, "");
    // Consts
    const char fieldChar = ' ';
    const char wallChar = '#';
    const char appleChar = '$';
    const char snakeChar = '@';
    const int fieldHeight = 6;
    const int fieldWidth = 10;

    Field field = Field{ wallChar, fieldChar, fieldHeight, fieldWidth };
    Apple apple = InitApple(appleChar, fieldHeight, fieldWidth);
    Snake snake = {snakeChar, 2, 5};  
    int score = 0;
    int direction = 1;
    bool eatedApple = false;
    Tail tail;

    //Сокрытие курсора
    void* handle = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO structCursorInfo;
    GetConsoleCursorInfo(handle, &structCursorInfo);
    structCursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(handle, &structCursorInfo);

    bool gameOver = false;
    while(!gameOver)
    {
        gotoxy(0, 0);

        if (_kbhit()) {
            char input = _getch();
            if (input == 'w') direction = up;
            else if (input == 'a') direction = left;
            else if (input == 's') direction = down;
            else if (input == 'd') direction = right;
        }
        
        switch (direction)
        {
        case up:
            snake.coordY--;
          
            break;
        case down:
            snake.coordY++;
            break;
        case left:
            snake.coordX--;
            break;
        case right:
            snake.coordX++;
            break;
        default:
            break;
        }

        if (snake.coordX == apple.coordX && snake.coordY == apple.coordY)
        {
            score++;
            eatedApple = true;
            apple.coordX = 1 + rand() % fieldWidth;
            apple.coordY = 1 + rand() % fieldHeight;
        }
        else if(snake.coordX <= 1 || snake.coordX >= fieldWidth || snake.coordY <= 1 || snake.coordY >= fieldHeight)
        {
            gameOver = true;
        }


        FieldType vectorField = InitField(field, snake, apple, tail);
        PrintField(vectorField, score); Sleep(1000);
        eatedApple = false;
    }
    
    system("cls");

    std::cout << "GAME OVER!" << std::endl;
    
    // сделать змейку фулл из очереди, написать вывод очереди и т.д
}
