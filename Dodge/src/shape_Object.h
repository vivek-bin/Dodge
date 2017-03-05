#ifndef SHAPE_OBJECT_CLASS
#define SHAPE_OBJECT_CLASS

#include "base_Shape.h"

class shape_Object{
protected:
    float ObjVelocity[4];
    bool Floating;
public:
    base_Shape *Shape;
    GLuint Texture;
    GLuint NormalTexture;
    float ModelMat[16];

    shape_Object();

    shape_Object(const shape_Object& Obj);

    void setTexture(GLuint Tex);

    void setShape(base_Shape* Shp);

    bool isFloating() const;

    void resetVelocity();

    void velUpdate(float const Acc[4],float Time);

    void posUpdate(float Time);

    float velocityMaxComponent();

    bool onFloor();

    bool simpleCuboidCollision (const shape_Object& Piece);

    float reduceOverlap(const shape_Object& Piece,bool Overlap);

};

#endif //SHAPE_OBJECT_CLASS
