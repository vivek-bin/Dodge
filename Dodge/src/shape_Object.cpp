#include "defines.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <math.h>
#include "mat_funcs.h"
#include "shape_Object.h"


shape_Object::shape_Object():ObjVelocity(),Floating(true),Shape(),Texture(),NormalTexture(){
    initMat(ModelMat);
}

shape_Object::shape_Object(const shape_Object& Obj):Floating(Obj.Floating),Shape(Obj.Shape),Texture(Obj.Texture),NormalTexture(Obj.NormalTexture){
    for(int i=0;i<4;i++)
        ObjVelocity[i]=Obj.ObjVelocity[i];

    for(int i=0;i<16;i++)
        ModelMat[i]=Obj.ModelMat[i];
}

void shape_Object::setTexture(GLuint Tex){
    Texture=Tex;
}

void shape_Object::setShape(base_Shape* Shp){
    Shape=Shp;
}

bool shape_Object::isFloating() const{
    return Floating;
}

void shape_Object::resetVelocity(){
    ObjVelocity[0]=0.0f;
    ObjVelocity[1]=0.0f;
    ObjVelocity[2]=0.0f;
    Floating=false;
}

void shape_Object::velUpdate(float const Acc[4],float Time){
    if(ObjVelocity[0]>MAX_VELOCITY||
       ObjVelocity[1]>MAX_VELOCITY||
       ObjVelocity[2]>MAX_VELOCITY)
        return;
    ObjVelocity[0]+=Acc[0]*Time;
    ObjVelocity[1]+=Acc[1]*Time;
    ObjVelocity[2]+=Acc[2]*Time;
}

void shape_Object::posUpdate(float Time){
    translateMat(ModelMat,ObjVelocity[0]*Time,ObjVelocity[1]*Time,ObjVelocity[2]*Time);
}

float shape_Object::velocityMaxComponent(){
    if(ObjVelocity[0]*ObjVelocity[0]>ObjVelocity[1]*ObjVelocity[1])
        if(ObjVelocity[0]*ObjVelocity[0]>ObjVelocity[2]*ObjVelocity[2])
            return fabs(ObjVelocity[0]);
        else
            return fabs(ObjVelocity[2]);
    else
        if(ObjVelocity[1]*ObjVelocity[1]>ObjVelocity[2]*ObjVelocity[2])
            return fabs(ObjVelocity[1]);
        else
            return fabs(ObjVelocity[2]);
}

bool shape_Object::onFloor(){
    if(ModelMat[13]-ModelMat[5]<=FLOOR_HEIGHT+MAX_OVERLAP)
        return true;
    return false;
}

bool shape_Object::simpleCuboidCollision (const shape_Object& Piece){
    float Pos,Size;
    Pos=ModelMat[14]-Piece.ModelMat[14];if(Pos<0.0f)Pos*=-1.0f;
    Size=ModelMat[10]+Piece.ModelMat[10];
    if(Pos<Size){
        Pos=ModelMat[13]-Piece.ModelMat[13];if(Pos<0.0f)Pos*=-1.0f;
        Size=ModelMat[5]+Piece.ModelMat[5];
        if(Pos<Size){
            Pos=ModelMat[12]-Piece.ModelMat[12];if(Pos<0.0f)Pos*=-1.0f;
            Size=ModelMat[0]+Piece.ModelMat[0];
            if(Pos<Size){
                return true;
            }
        }
    }
    return false;
}

float shape_Object::reduceOverlap(const shape_Object& Piece,bool Overlap){
    float TimeSlice,TimeRev;
    TimeRev=TimeSlice=UNIT_TIME/2;
    posUpdate(-TimeSlice);
    while(TimeSlice>MAX_OVERLAP/MAX_VELOCITY){
        TimeSlice/=2;
        if(simpleCuboidCollision(Piece)){
            posUpdate(-TimeSlice);
            TimeRev+=TimeSlice;
        }
        else{
            posUpdate(TimeSlice);
            TimeRev-=TimeSlice;
        }
    }
    if(simpleCuboidCollision(Piece)){
        if(!Overlap){
            posUpdate(-TimeSlice);
            TimeRev+=TimeSlice;
        }
    }
    else{
        if(Overlap){
            posUpdate(TimeSlice);
            TimeRev-=TimeSlice;
        }
    }

    return TimeRev;
}

