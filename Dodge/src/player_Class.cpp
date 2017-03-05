#include "defines.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include "mat_funcs.h"
#include "player_Class.h"


player_Class::player_Class(): shape_Object(),Height(PLAYER_HEIGHT),Width(PLAYER_WIDTH),
                              MouseXAngle(),MouseYAngle(),Lives(100),Camera(&ModelMat[12]){

    scaleMat(ModelMat,Width,Height,Width);
    translateMat(ModelMat,0,Height + 5.0f,0);

    Forward[0]=0.0f;Forward[1]=0;   Forward[2]=1.0f;Forward[3]=0.0f;
         Up[0]=0.0f;     Up[1]=1.0f;     Up[2]=0.0f;     Up[3]=0.0f;
}

char player_Class::getLives(){
    return Lives;
}

void player_Class::reduceLives(){
    --Lives;
    translateMat(ModelMat,0,MAX_BLOCK_HEIGHT*2*UNIT_DISTANCE,0);
    Floating=true;
}

void player_Class::getViewMat(float ViewMat[]){
    float TempForward[4],TempRight[4],TempUp[4];
    float TmpQuatX[4],TmpQuatY[4],TmpQuatXY[4],TempMat[16];

    TempForward[0]=Forward[0];TempForward[1]=Forward[1];TempForward[2]=Forward[2];TempForward[3]=0;
    TempUp[0]=Up[0];TempUp[1]=Up[1];TempUp[2]=Up[2];TempUp[3]=0;
    normalizeVector(TempForward);normalizeVector(TempUp);
    crossProduct(TempForward,TempUp,TempRight);

    createQuat(TempUp,MouseXAngle,TmpQuatX);
    createQuat(TempRight,MouseYAngle,TmpQuatY);
    multiplyQuatQQ(TmpQuatY,TmpQuatX,TmpQuatXY);

    rotateMatrixByQuat(TmpQuatXY,TempMat);
    multiplyMatMV(TempMat,TempRight,4);multiplyMatMV(TempMat,TempUp,4);multiplyMatMV(TempMat,TempForward,4);

    //prepare view
    ViewMat[0]= TempRight[0];  ViewMat[4]= TempRight[1];  ViewMat[8]= TempRight[2];
    ViewMat[1]= TempUp[0];     ViewMat[5]= TempUp[1];     ViewMat[9]= TempUp[2];
    ViewMat[2]=-TempForward[0];ViewMat[6]=-TempForward[1];ViewMat[10]=-TempForward[2];
    ViewMat[3]=0;              ViewMat[7]=0;              ViewMat[11]=0;
        ViewMat[12]=-Camera[0]*ViewMat[0]-Camera[1]*ViewMat[4]-Camera[2]*ViewMat[8];
        ViewMat[13]=-Camera[0]*ViewMat[1]-Camera[1]*ViewMat[5]-Camera[2]*ViewMat[9];
        ViewMat[14]=-Camera[0]*ViewMat[2]-Camera[1]*ViewMat[6]-Camera[2]*ViewMat[10];
        ViewMat[15]=1;
}

void player_Class::mouseMove(float X,float Y){
    MouseXAngle=-(X-0.5f)*M_PI*0.4;
    MouseYAngle=-(Y-0.5f)*M_PI*0.4;
}

void player_Class::rotateAboutCamera(float RotateSpeed,float Time){
    float TempMat[16];
    initMat(TempMat);
    rotateYMatrix(KEY_ROTATESPEED*RotateSpeed*Time,TempMat);
    multiplyMatMV(TempMat,Forward,4);
    multiplyMatMV(TempMat,Up,4);
}

void player_Class::getForwardVelocity(float TempVec[],float MoveSpeed){
    TempVec[0]=Forward[0]*MoveSpeed;
    TempVec[1]=Forward[1]*MoveSpeed;
    TempVec[2]=Forward[2]*MoveSpeed;
    TempVec[3]=Forward[3]*MoveSpeed;
}

void player_Class::jumpPlayer(){
    if(Floating || velocityMaxComponent()>1e-10)
        return;
    ObjVelocity[1]=JUMP_VELOCITY;
    Floating=true;
}

void player_Class::forwardPosUpdate(float Time){
    translateMat(ModelMat,KEY_MOVESPEED*Forward[0]*Time,KEY_MOVESPEED*Forward[1]*Time,KEY_MOVESPEED*Forward[2]*Time);
}

bool player_Class::playerCollision(const shape_Object& Piece){
    bool result;
    result=simpleCuboidCollision(Piece);
    return result;
}

bool player_Class::playerHeadCollision(const shape_Object& Piece){
    bool result;
    Camera[1]+=Height*0.95f;
    ModelMat[0]=Width*FEET_HEAD_WIDTH_SCALE;
    ModelMat[5]=Height*FEET_HEAD_HEIGHT_SCALE;
    ModelMat[10]=Width*FEET_HEAD_WIDTH_SCALE;

    result=simpleCuboidCollision(Piece);

    Camera[1]-=Height*0.95f;
    ModelMat[0]=Width*BODY_WIDTH_SCALE;
    ModelMat[5]=Height;
    ModelMat[10]=Width*BODY_WIDTH_SCALE;
    return result;
}

bool player_Class::playerBodyCollision(const shape_Object& Piece){
    bool result;
    ModelMat[5]=Height*BODY_HEIGHT_SCALE;

    result=simpleCuboidCollision(Piece);

    ModelMat[5]=Height;
    return result;
}

bool player_Class::playerFeetCollision(const shape_Object& Piece){
    bool result;
    Camera[1]-=Height*0.95f;
    ModelMat[0]=Width*FEET_HEAD_WIDTH_SCALE;
    ModelMat[5]=Height*FEET_HEAD_HEIGHT_SCALE;
    ModelMat[10]=Width*FEET_HEAD_WIDTH_SCALE;

    result=simpleCuboidCollision(Piece);

    Camera[1]+=Height*0.95f;
    ModelMat[0]=Width*BODY_WIDTH_SCALE;
    ModelMat[5]=Height;
    ModelMat[10]=Width*BODY_WIDTH_SCALE;
    return result;
}

float player_Class::reduceForwardOverlap(int Direction,const shape_Object& Piece,bool Overlap){

    float TimeSlice,TimeRev;
    TimeRev=TimeSlice=UNIT_TIME/2;
    forwardPosUpdate(Direction*(-TimeSlice));
    while(TimeSlice>MAX_OVERLAP/MAX_VELOCITY){
        TimeSlice/=2;
        if(simpleCuboidCollision(Piece)){
            forwardPosUpdate(Direction*(-TimeSlice));
            TimeRev+=TimeSlice;
        }
        else{
            forwardPosUpdate(Direction*TimeSlice);
            TimeRev-=TimeSlice;
        }
    }
    if(simpleCuboidCollision(Piece)){
        if(!Overlap){
            forwardPosUpdate(Direction*(-TimeSlice));
            TimeRev+=TimeSlice;
        }
    }
    else{
        if(Overlap){
            forwardPosUpdate(Direction*TimeSlice);
            TimeRev-=TimeSlice;
        }
    }
    return TimeRev;
}

