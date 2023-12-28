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

using FieldType = std::vector<std::vector<char>>;

class Snake
{
public:
    char itemChar;
    //std::vector<std::map<int, int>> coords;
    int coordX;
    int coordY;
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
    int appleCoordX = 1 + rand() % fieldWidth;
    int appleCoordY = 1 + rand() % fieldHeight;
    Apple apple =
    {
        itemChar,
        appleCoordX,
        appleCoordY,
    };
    return apple;
}

FieldType InitField(Field field, Snake snake, Apple apple) // Инициализация поля. Добавляем, отрисовываем.
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
                }
            }
        }
        vectorField.push_back(fieldString);
        externalString = false;
        fieldString.clear();
    }
    return vectorField;
}

void gotoxy(int x, int y)
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
    Snake snake = {snakeChar, 2, 2};  
    int score = 0;

    //Test
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
            if (input == 'w') snake.coordY--;
            else if (input == 'a') snake.coordX--;
            else if (input == 's') snake.coordY++;
            else if (input == 'd') snake.coordX++;
        }
        
        if (snake.coordX == apple.coordX && snake.coordY == apple.coordY)
        {
            score++;
        }
        FieldType vectorField = InitField(field, snake, apple);
        PrintField(vectorField, score);
    }
    
    
}
