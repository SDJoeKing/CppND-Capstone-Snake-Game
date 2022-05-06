#include <iostream>
 
#include "SDL.h"
#include "obstacle.h"

Obstacle::Obstacle(int x, int y, int h, int W, int H, int  gridW, int  gridH)
:_oX{x}, _oY{y}, _oH{h}, _screenWidth{W}, _screenHeight{H}, _gridW{gridW}, _gridH{gridH}, _eng(_dev()),_dis(1, maxSpeed)
{   // initialise SDL_Rect
    _block.x = _oX;
    _block.y = _oY;
    _block.w = _oH;
    _block.h = _oH;
     // state:
    _state = obstacleState::hitNone;

    // random speed generator
    _speedX = 2;
    _speedY = 2;
}

// empty decon
Obstacle::~Obstacle(){} 

// Helper function
void Obstacle::bounce(int scaling){
// std::cout << "Collide! " << static_cast<int>(_state) << std::endl;
        // update the next move steps for the obstacle block
        switch(_state){
            // all scenarios, bounce off with faster speed
            // if hit side walls, reverse the speed of X
            case obstacleState::hitWallLeft:
            case obstacleState::hitWallRight:
            _speedX = (-scaling) * _speedX;
            _speedY = (scaling ) *_speedY;
            // reset state to nonhitting
            _state = obstacleState::hitNone;
            break;

            // if hit top/bottom walls, reverse the speed of Y
            case obstacleState::hitWallDown:
            case obstacleState::hitWallUp:
             _speedX = ( scaling) * _speedX;
            _speedY = (-scaling ) *_speedY;
            _state = obstacleState::hitNone;
            break;    

            case obstacleState::hitCorner:
            _speedX =(-scaling ) *_speedX;
            _speedY = (-scaling ) *_speedY;
            _state = obstacleState::hitNone;

            default:
            _state = obstacleState::hitNone;        
        }
}

// defines how the obstacle moves around the screen, takes in the reference to snake
void Obstacle::move(Snake &snake)
{   
    // get true location of the snake head
    const int x = _screenWidth/_gridW * static_cast<int>(snake.head_x) ;
    const int y = _screenHeight/_gridH * static_cast<int>(snake.head_y) ;

    //check if collide to walls
    if(collision()){
        // normal smooth bounce off walls
       bounce(1);
       updatePosition();
    }else if (collision(x, y)){
        // check snake collision if true shrink snake
        int scale = 10;
        
         bounce(scale);
         updatePosition();
         if (snake.body.size() == 0){
             snake.alive = false;
         }
         
         cutSnakeInHalf(snake);

        // dial back speed after snake collision
        _speedX/=scale;
        _speedY/=scale;
         }
    else{
    // update regularly if no collision 
    updatePosition();
    }
}

// update the position of the obstacle block
void Obstacle::updatePosition(){
    _block.x += _speedX;
    _block.y += _speedY;

    // need to wrap around the edges, if outside the 4 boundaries, let the block stop at the boundery to simplify collision check
    if (_block.x <= 0){
        _block.x = 0;
    }else if (_block.x + _block.w >= _screenWidth){
        _block.x = _screenWidth - _block.w;
    }
  

    if (_block.y <= 0){
        _block.y = 0;
    }else if (_block.y + _block.h >= _screenHeight){
        _block.y = _screenHeight - _block.h;
    }

}

void Obstacle::cutSnakeInHalf(Snake &snake){

    snake.body.resize( snake.body.size()/2);
    
}


//reset obstacle size to pixel default
void Obstacle::resetSize(){
    _block.w = _oH;
    _block.h = _oH;
}

// Randomise speed and direction everytime snake eats food
void Obstacle::updateSpeed(){
    _speedX  = _dis(_eng);
    _speedY  = _dis(_eng);
    
    // debugging console out put
    // std::cout << "SpeedX " << getSpeedX() << " SpeedY "<< getSpeedY() << std::endl; 
}

// check if collision happened, and update the status internally
bool Obstacle::collision(){

    bool collide = false;
    // first check if the 4 corners of the rect are in the corners of the screen
    if((_block.x == 0 && _block.y == 0) || (_block.x == 0 && (_block.y+_block.w) == _screenHeight) || ((_block.x+_block.w) == _screenWidth && _block.y == 0) || ((_block.x + _block.w) == _screenWidth && (_block.y+_block.h) == _screenHeight))
    {_state = obstacleState::hitCorner;
    collide = true;
 
    return collide;}

    // check individual boundary
    if(_block.x == 0) {_state = obstacleState::hitWallLeft; collide = true; }
    else if((_block.x + _block.w) == _screenWidth) {_state = obstacleState::hitWallRight; collide = true;  }
    else if(_block.y == 0) {_state = obstacleState::hitWallUp; collide = true;  }
    else if((_block.y+_block.h) == _screenHeight) {_state = obstacleState::hitWallDown; collide = true;  }
    else {_state = obstacleState::hitNone; collide = false;}
    
    return collide;
}
// overloaded function to check collision of snake
bool Obstacle::collision(const int &x, const int &y){
    
    bool collide = false;
    if (x == _block.x && y >= _block.y && y <= (_block.y + _block.h)) {_state = obstacleState::hitWallLeft; collide = true; }
    else if (y == _block.y && x >= _block.x && x <= (_block.x + _block.w)) {_state = obstacleState::hitWallUp; collide = true; }
    else if (x == (_block.x + _block.w) && y >= _block.y && y <= (_block.y + _block.h)) {_state = obstacleState::hitWallRight; collide = true; }
    else if (y == (_block.y + _block.h) && x >= _block.x && x <= (_block.x + _block.w)) {_state = obstacleState::hitWallDown; collide = true; }
    else {_state = obstacleState::hitNone; collide = false; }
    
    return collide;
}


// getter
SDL_Rect Obstacle::getBlock() const {
    return this->_block;
}

// setter (square block H = W)
void Obstacle::setHeight(int newH){
    _block.h = newH;
    _block.w = newH;
}

 