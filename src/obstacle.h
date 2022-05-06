#pragma once
#include <random>
#include "SDL.h"
#include "snake.h"


class Obstacle{

public:

    enum class obstacleState {hitWallLeft, hitWallUp, hitWallRight, hitWallDown, hitSnake, hitCorner, hitNone};

    // Constructor & deconstructor
     
    Obstacle(int x, int y, int h, const int screenWidth, const int screenHeight, const int, const int);
    ~Obstacle();

    // member func move takes the boundaries of the screen W, H
    void move(Snake &); // randomised move
    
    // getter
    SDL_Rect getBlock() const;
    int getSpeedX(){return _speedX;}
    int getSpeedY(){return _speedY;}

    // setter (square block H = W)
    void setHeight(int newH);
    void updateSpeed(); // change speed and direction after every collision
    void resetSize(); // reset obstacle size

private:

    int _oX {0}; // X point 
    int _oY {0}; // Y point 
    int _oH {0}; //  
    
    obstacleState _state = obstacleState::hitNone;

    // screen area where the contains the obstacle
    // obstacle needs to know the grid as well to locate snake
    const int _screenWidth;
    const int _screenHeight;
    const int _gridW;
    const int _gridH;

    // speed along x and y axis so can travel diagonally
    const int maxSpeed {6}; // maximum speed 1 seconds half screen
    int _speedX;
    int _speedY;

    // rect block object for render
    SDL_Rect _block;

    // private functions
    bool collision(); // detect if collision happened to wall and or snake head
    bool collision(const int &x, const int &y); // OVERLOADED function collision check if head of snake collide into the obstacle
    void updatePosition();
    void bounce(int); // helper function to change internal state
    void cutSnakeInHalf(Snake &snake);

    // speed randomiser
    std::random_device _dev;
    std::mt19937 _eng;
    std::uniform_int_distribution<> _dis;

};