#include "defines.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include "base_Shape.h"


base_Shape::base_Shape(): Vertices_VboID(),Colors_VboID(),Normals_VboID(),Indices_VboID(),
                          TexCoord_VboID(),Height_VboID(),VertexCount(),VaoID(){

}

void base_Shape::createVao(){
    glGenVertexArrays(1,&VaoID);
    glBindVertexArray(0);
}

void base_Shape::createHeightBuffer(GLfloat Data[],GLuint DataSize){
    glBindVertexArray(VaoID);
    glGenBuffers(1, &Height_VboID);
    glBindBuffer(GL_ARRAY_BUFFER, Height_VboID);
    glBufferData(GL_ARRAY_BUFFER, DataSize, Data, GL_STATIC_DRAW);
    glVertexAttribPointer(0,1,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void base_Shape::createVertexBuffer(GLfloat Data[],GLuint DataSize){
    glBindVertexArray(VaoID);
    glGenBuffers(1, &Vertices_VboID);
    glBindBuffer(GL_ARRAY_BUFFER, Vertices_VboID);
    glBufferData(GL_ARRAY_BUFFER, DataSize, Data, GL_STATIC_DRAW);
    glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void base_Shape::createColorBuffer(GLfloat Data[],GLuint DataSize){
    glBindVertexArray(VaoID);
    glGenBuffers(1, &Colors_VboID);
    glBindBuffer(GL_ARRAY_BUFFER, Colors_VboID);
    glBufferData(GL_ARRAY_BUFFER, DataSize, Data, GL_STATIC_DRAW);
    glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void base_Shape::createTexCoordBuffer(GLfloat Data[],GLuint DataSize){
    glBindVertexArray(VaoID);
    glGenBuffers(1, &TexCoord_VboID);
    glBindBuffer(GL_ARRAY_BUFFER, TexCoord_VboID);
    glBufferData(GL_ARRAY_BUFFER, DataSize, Data, GL_STATIC_DRAW);
    glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void base_Shape::createNormalBuffer(GLfloat Data[],GLuint DataSize){
    glBindVertexArray(VaoID);
    glGenBuffers(1, &Normals_VboID);
    glBindBuffer(GL_ARRAY_BUFFER, Normals_VboID);
    glBufferData(GL_ARRAY_BUFFER, DataSize, Data, GL_STATIC_DRAW);
    glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,0);
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void base_Shape::createIndexBuffer(GLushort Data[],GLuint DataSize){
    VertexCount=DataSize/sizeof(Data[0]);
    glGenBuffers(1, &Indices_VboID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Indices_VboID);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, DataSize, Data, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

GLuint base_Shape::getVaoID(){
    return VaoID;
}

void base_Shape::drawElements(GLenum mode){
    glBindVertexArray(VaoID);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Indices_VboID);
    glDrawElements(mode, VertexCount, GL_UNSIGNED_SHORT, NULL);
    glBindVertexArray(0);
}

void base_Shape::destroyVboVao(){
    glBindVertexArray(VaoID);
    if(Height_VboID != 0)
    {
        glDisableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glDeleteBuffers(1,&Height_VboID);
    }
    if(Vertices_VboID != 0)
    {
        glDisableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glDeleteBuffers(1,&Vertices_VboID);
    }
    if(Colors_VboID != 0)
    {
        glDisableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glDeleteBuffers(1,&Colors_VboID);
    }
    if(TexCoord_VboID != 0)
    {
        glDisableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glDeleteBuffers(1,&TexCoord_VboID);
    }
    if(Normals_VboID != 0)
    {
        glDisableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER,0);
        glDeleteBuffers(1,&Normals_VboID);
    }
    if(Indices_VboID != 0)
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER,0);
        glDeleteBuffers(1,&Indices_VboID);
    }

    glBindVertexArray(0);
    glDeleteVertexArrays(1,&VaoID);
}
