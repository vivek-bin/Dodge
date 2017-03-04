#include "defines.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include "control_Interface.h"


void control_Interface::arrowKeyUp(int key,int x,int y){
    if(key == GLUT_KEY_RIGHT)
        {ArrowKeys[RIGHT_KEY]=false;}
    else if(key == GLUT_KEY_LEFT)
        {ArrowKeys[LEFT_KEY]=false;}
    else if(key == GLUT_KEY_UP)
        {ArrowKeys[UP_KEY]=false;}
    else if(key == GLUT_KEY_DOWN)
        {ArrowKeys[DOWN_KEY]=false;}
}

void control_Interface::arrowKeyDown(int key,int x,int y){
    if(key == GLUT_KEY_RIGHT)
        {ArrowKeys[RIGHT_KEY]=true;}
    else if(key == GLUT_KEY_LEFT)
        {ArrowKeys[LEFT_KEY]=true;}
    else if(key == GLUT_KEY_UP)
        {ArrowKeys[UP_KEY]=true;}
    else if(key == GLUT_KEY_DOWN)
        {ArrowKeys[DOWN_KEY]=true;}
}

void control_Interface::otherKeysUp(unsigned char key,int x,int y){
    if(key == GLUT_KEY_SPACEBAR)
        ArrowKeys[SPACE_KEY]=false;
}

void control_Interface::otherKeysDown(unsigned char key,int x,int y){
    if(key == GLUT_KEY_SPACEBAR)
        ArrowKeys[SPACE_KEY]=true;
}

void control_Interface::setMouse(int x,int y){
    MouseX=x;MouseY=y;
}

bool control_Interface::jump(){
    return ArrowKeys[SPACE_KEY];
}

int control_Interface::horizontalCtrl(){
    if(!(ArrowKeys[RIGHT_KEY]^ArrowKeys[LEFT_KEY]))
        return 0;
    if(ArrowKeys[RIGHT_KEY])
        return -1;
    else
        return 1;
}

int control_Interface::verticalCtrl(){
    if(!(ArrowKeys[UP_KEY]^ArrowKeys[DOWN_KEY]))
        return 0;
    if(ArrowKeys[DOWN_KEY])
        return -1;
    else
        return 1;
}

int control_Interface::getMouseX(){
    return MouseX;
}

int control_Interface::getMouseY(){
    return MouseY;
}

void control_Interface::inputSetup(){
    glutKeyboardFunc(otherKeysDown);
    glutKeyboardUpFunc(otherKeysUp);
    glutSpecialFunc(arrowKeyDown);
    glutSpecialUpFunc(arrowKeyUp);
    glutPassiveMotionFunc(setMouse);
}

bool control_Interface::ArrowKeys[];
int  control_Interface::MouseX;
int  control_Interface::MouseY;

