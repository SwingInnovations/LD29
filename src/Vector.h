#ifndef VECTOR_H
#define	VECTOR_H

#include <cmath>

class Vector2D{
public:
    Vector2D(){;}
    Vector2D(float x, float y){
        this->x = x;
        this->y = y;
    }
    
    void SetX(float x){this->x = x;}
    void SetY(float y){this->y = y;}
    
    float GetX(){return x;}
    float GetY(){return y;}
    
    virtual ~Vector2D(){;}
    
private:
    float x, y;
};


#endif	/* VECTOR_H */

