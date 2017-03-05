#ifndef PLAYER_CLASS_CLASS
#define PLAYER_CLASS_CLASS

#include "shape_Object.h"

class player_Class:public shape_Object{
private:
    float Height,Width;
    float Forward[4],Up[4];
    float MouseXAngle,MouseYAngle;
    short Lives;

public:

    float *Camera;

    player_Class();

    char getLives();

    void reduceLives();

    void getViewMat(float ViewMat[]);

    void mouseMove(float X,float Y);

    void rotateAboutCamera(float RotateSpeed,float Time);

    void getForwardVelocity(float TempVec[],float MoveSpeed);

    void jumpPlayer();

    void forwardPosUpdate(float Time);

    bool playerCollision(const shape_Object& Piece);

    bool playerHeadCollision(const shape_Object& Piece);

    bool playerBodyCollision(const shape_Object& Piece);

    bool playerFeetCollision(const shape_Object& Piece);

    float reduceForwardOverlap(int Direction,const shape_Object& Piece,bool Overlap);

};


#endif //PLAYER_CLASS_CLASS
