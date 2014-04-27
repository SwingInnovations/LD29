#ifndef ENTITY_H
#define	ENTITY_H

#include <SDL2/SDL_rect.h>
#include <SDL2/SDL_image.h>
#include "Vector.h"
#include "../GameWindow.h"


class Entity{
public:
    Entity(){;}
    Entity(Vector2D pos, Vector2D dim){
        this->position = pos;
        this->dimension = dim;
        rect.x = pos.GetX();
        rect.y = pos.GetY();
        rect.w = dim.GetX();
        rect.h = dim.GetY();
    }
    
    virtual ~Entity(){
        SDL_DestroyTexture(image);
        image = 0;
    }
    
    void SetDirection(int dir){this->direction = dir;}
    void SetPosX(float x){position.SetX(x); rect.x = x;}
    void SetPosY(float y){position.SetY(y); rect.y = y;}
    void SetPassable(bool pass){this->IsPassable = pass;}
    void SetOnGround(bool ground){this->OnGround = ground;}
    void SetHealth(int h){this->health = h;}
    void SetImage(SDL_Texture* source){image = source;}
    void SetValuable(bool val){this->IsValuable = val;}
    void SetSpeed(float val){this->speed = val;}
    
    void Hunt(Entity& other)//Will go to the other entity if requested
    {
        if(this->position.GetX() > other.position.GetX())
        {
            float _x = other.position.GetX();
            _x += speed; 
            other.SetPosX(_x);
            this->SetDirection(EAST);
        }else{
            float _x = other.position.GetX();
            _x += 0.0;
            other.SetPosX(_x);
        }
        
        if(this->position.GetX() < other.position.GetX())
        {
            float _x = other.position.GetX();
            _x += speed;
            other.SetPosX(_x);
            this->SetDirection(WEST);
        }else{
            float _x = other.position.GetX();
            _x += 0.0;
            other.SetPosX(_x);
        }
        
        if(this->position.GetY() > other.position.GetY())
        {
            float _y = other.position.GetY();
            _y += speed;
            other.SetPosY(_y);
            this->SetDirection(SOUTH);
        }else{
            float _x = other.position.GetX();
            _x += 0.0;
            other.SetPosX(_x);
        }
        
        if(this->position.GetY() < other.position.GetY())
        {
            float _y = other.position.GetY();
            _y -= speed;
            other.SetPosY(_y);
            this->SetDirection(NORTH);
        }else{
            float _x = other.position.GetX();
            _x += 0.0;
            other.SetPosX(_x);
        }
        
    }
    
    int GetHealth(){return health;}
    
    bool GetPassable(){return IsPassable;}
    bool GetValuable(){return IsValuable;}
    bool GetGroundStatus(){return OnGround;}
    bool Intersected(Entity &other)
    {
        return SDL_HasIntersection(&this->rect, &other.rect);
    }
    
    SDL_Rect GetRect(){return rect;}
    
    Vector2D GetPositionVector(){ return position; }
    Vector2D GetDimensionVector(){ return dimension; }
    
private:
    bool IsPassable, IsValuable, OnGround;
    SDL_Rect rect;
    Vector2D position, dimension;
    int health, direction;
    int NORTH = 0, EAST = 1, SOUTH = 2, WEST = 3;
    SDL_Texture* image = NULL;
    float speed;
};


#endif	/* ENTITY_H */

