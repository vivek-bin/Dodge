#include "defines.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include "mat_funcs.h"
#include "classes.h"


base_Shape::base_Shape(){
    ObjCentre[0]=ObjCentre[1]=ObjCentre[2]=ObjCentre[3]=0.0f;

    Height_VboID=TexCoord_VboID=Vertices_VboID=Indices_VboID=Colors_VboID=Normals_VboID=VaoID=0;
    VertexCount=0;
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




shader_Prog::shader_Prog(){
    VertexShaderID=FragmentShaderID=ProgramID=0;
}

GLuint shader_Prog::getCurrentProg(){
    return shader_Prog::CurrentProg;
}

GLuint shader_Prog::getProgramID(){
    return ProgramID;
}

GLuint shader_Prog::getVertexShaderID(){
    return VertexShaderID;
}

GLuint shader_Prog::getFragmentShaderID(){
    return FragmentShaderID;
}

void shader_Prog::createShaders(char const * const VertPrg,char const * const FragPrg){
    GLint success = 0;
    VertexShaderID=glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShaderID,1,&VertPrg,NULL);
    glCompileShader(VertexShaderID);
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE)
        fprintf(stdout,"error compiling vertex shader");
    FragmentShaderID=glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShaderID,1,&FragPrg,NULL);
    glCompileShader(FragmentShaderID);

    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE)
        fprintf(stdout,"error compiling frag shader");
    ProgramID=glCreateProgram();
    glAttachShader(ProgramID,VertexShaderID);
    glAttachShader(ProgramID,FragmentShaderID);

    glLinkProgram(ProgramID);
}

void shader_Prog::createShaders(const GLuint VertPrg,char const * const FragPrg){
    GLint success = 0;
    VertexShaderID=VertPrg;

    FragmentShaderID=glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(FragmentShaderID,1,&FragPrg,NULL);
    glCompileShader(FragmentShaderID);
    glGetShaderiv(FragmentShaderID, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE)
        fprintf(stdout,"error compiling frag shader");

    ProgramID=glCreateProgram();
    glAttachShader(ProgramID,VertexShaderID);
    glAttachShader(ProgramID,FragmentShaderID);

    glLinkProgram(ProgramID);
}

void shader_Prog::createShaders(char const * const VertPrg,const GLuint FragPrg){
    GLint success = 0;
    VertexShaderID=glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(VertexShaderID,1,&VertPrg,NULL);
    glCompileShader(VertexShaderID);
    glGetShaderiv(VertexShaderID, GL_COMPILE_STATUS, &success);
    if(success == GL_FALSE)
        fprintf(stdout,"error compiling vertex shader");

    FragmentShaderID=FragPrg;

    ProgramID=glCreateProgram();
    glAttachShader(ProgramID,VertexShaderID);
    glAttachShader(ProgramID,FragmentShaderID);

    glLinkProgram(ProgramID);
}

void shader_Prog::destroyShaders(){
    GLenum ErrorValue = glGetError();

    glUseProgram(0);

    glDetachShader(ProgramID,FragmentShaderID);
    glDetachShader(ProgramID,VertexShaderID);
    glDeleteShader(VertexShaderID);
    glDeleteShader(FragmentShaderID);

    glDeleteProgram(ProgramID);

    ErrorValue=glGetError();
    if(ErrorValue != GL_NO_ERROR)
    {
        fprintf(stderr,"ERROR : %s",gluErrorString(ErrorValue));
    }
}

void shader_Prog::disableProgram(){
    if(CurrentProg!=0)
    {
        glUseProgram(0);
        CurrentProg=0;
    }
}

void shader_Prog::useProgram(){
    if(CurrentProg!=ProgramID)
    {
        glUseProgram(ProgramID);
        CurrentProg=ProgramID;
    }
}

void shader_Prog::useProgram(const char* UName,float Proj[]){
    if(CurrentProg!=ProgramID)
    {
        glUseProgram(ProgramID);
        CurrentProg=ProgramID;
        glUniformMatrix4fv(glGetUniformLocation(CurrentProg,UName),1,GL_FALSE,Proj);
    }
}

void shader_Prog::useProgram(const char* UName1,float Proj[],const char* UName2,float View[]){
    if(CurrentProg!=ProgramID)
    {
        glUseProgram(ProgramID);
        CurrentProg=ProgramID;
        glUniformMatrix4fv(glGetUniformLocation(CurrentProg,UName1),1,GL_FALSE,Proj);
        glUniformMatrix4fv(glGetUniformLocation(CurrentProg,UName2),1,GL_FALSE,View);
    }
}

GLuint shader_Prog::CurrentProg;



shape_Object::shape_Object(){
    ObjVelocity[0]=ObjVelocity[1]=ObjVelocity[2]=ObjVelocity[3]=0.0f;
    initMat(ModelMat);
    Shape=NULL;
    Texture=0;
    Floating=true;
}

shape_Object::shape_Object(const shape_Object& Obj){
    for(int i=0;i<4;i++)
        ObjVelocity[i]=Obj.ObjVelocity[i];

    for(int i=0;i<16;i++)
        ModelMat[i]=Obj.ModelMat[i];

    Shape=Obj.Shape;
    Texture=Obj.Texture;
    Floating=Obj.Floating;
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





void player_Class::setMatPosition(){
    ModelMat[12]=Camera[0];
    ModelMat[13]=Camera[1];
    ModelMat[14]=Camera[2];
    if(PlayerPart==Feet)
        ModelMat[13]+=-Height*0.95;
    else if(PlayerPart==Head)
        ModelMat[13]+=Height*0.95;
}

player_Class::player_Class(): shape_Object(){
    PlayerPart=Whole;
    Lives=100;
    Width=Height=0.0f;

     Camera[0]=0.0f; Camera[1]=PLAYER_HEIGHT+5;  Camera[2]=0.0f; Camera[3]=1.0f;
    Forward[0]=0.0f;Forward[1]=0;               Forward[2]=1.0f;Forward[3]=0.0f;
         Up[0]=0.0f;     Up[1]=1.0f;                 Up[2]=0.0f;     Up[3]=0.0f;

    setMatPosition();

    MouseXAngle=MouseYAngle=0.0f;
}

char player_Class::getLives(){
    return Lives;
}

void player_Class::reduceLives(const char FlrN){
    --Lives;
    Camera[1]=(FlrN+8)*UNIT_DISTANCE;
    setMatPosition();
    Floating=true;
}

void player_Class::reduceLives(){
    --Lives;
}

void player_Class::setPlayerHeightWidth(float H,float W){
    Height=H;Width=W;
    initMat(ModelMat);
    scaleMat(ModelMat,Width*BODY_WIDTH_SCALE,
             Height*(BODY_HEIGHT_SCALE+FEET_HEAD_HEIGHT_SCALE+FEET_HEAD_HEIGHT_SCALE),
             Width*BODY_WIDTH_SCALE);
    translateMat(ModelMat,0,Height,0);
    PlayerPart=Whole;
}

void player_Class::switchToPlayerHead(){
    if(PlayerPart==Body||PlayerPart==Whole)
        ModelMat[13]+=Height*0.95f;
    else if(PlayerPart==Feet)
        ModelMat[13]+=Height*1.9f;

    ModelMat[0]=Width*FEET_HEAD_WIDTH_SCALE;
    ModelMat[5]=Height*FEET_HEAD_HEIGHT_SCALE;
    ModelMat[10]=Width*FEET_HEAD_WIDTH_SCALE;
    PlayerPart=Head;
}

void player_Class::switchToPlayerFeet(){
    if(PlayerPart==Body||PlayerPart==Whole)
        ModelMat[13]-=Height*0.95f;
    else if(PlayerPart==Head)
        ModelMat[13]-=Height*1.9f;
    else if(PlayerPart==Whole)

    ModelMat[0]=Width*FEET_HEAD_WIDTH_SCALE;
    ModelMat[5]=Height*FEET_HEAD_HEIGHT_SCALE;
    ModelMat[10]=Width*FEET_HEAD_WIDTH_SCALE;
    PlayerPart=Feet;
}

void player_Class::switchToPlayerBody(){
    if(PlayerPart==Feet)
        ModelMat[13]+=Height*0.95f;
    else if(PlayerPart==Head)
        ModelMat[13]-=Height*0.95f;

    ModelMat[0]=Width*BODY_WIDTH_SCALE;
    ModelMat[5]=Height*BODY_HEIGHT_SCALE;
    ModelMat[10]=Width*BODY_WIDTH_SCALE;
    PlayerPart=Body;
}

void player_Class::switchToPlayer(){
    if(PlayerPart==Feet)
        ModelMat[13]+=Height*0.95f;
    else if(PlayerPart==Head)
        ModelMat[13]-=Height*0.95f;

    ModelMat[0]=Width*BODY_WIDTH_SCALE;
    ModelMat[5]=Height*(BODY_HEIGHT_SCALE+FEET_HEAD_HEIGHT_SCALE+FEET_HEAD_HEIGHT_SCALE);
    ModelMat[10]=Width*BODY_WIDTH_SCALE;
    PlayerPart=Whole;
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

void player_Class::posUpdate(float Time){
    Camera[0]+=ObjVelocity[0]*Time;
    Camera[1]+=ObjVelocity[1]*Time;
    Camera[2]+=ObjVelocity[2]*Time;

    setMatPosition();
}

void player_Class::forwardPosUpdate(int Direction,float Time){
    Camera[0]+=Direction*KEY_MOVESPEED*Forward[0]*Time;
    Camera[1]+=Direction*KEY_MOVESPEED*Forward[1]*Time;
    Camera[2]+=Direction*KEY_MOVESPEED*Forward[2]*Time;

    setMatPosition();
}

float player_Class::reduceForwardOverlap(int Direction,const shape_Object& Piece,bool Overlap){
    float TimeSlice,TimeRev;
    TimeRev=TimeSlice=UNIT_TIME/2;
    forwardPosUpdate(Direction,-TimeSlice);
    while(TimeSlice>MAX_OVERLAP/MAX_VELOCITY){
        TimeSlice/=2;
        if(simpleCuboidCollision(Piece)){
            forwardPosUpdate(Direction,-TimeSlice);
            TimeRev+=TimeSlice;
        }
        else{
            forwardPosUpdate(Direction,TimeSlice);
            TimeRev-=TimeSlice;
        }
    }
    if(simpleCuboidCollision(Piece)){
        if(!Overlap){
            forwardPosUpdate(Direction,-TimeSlice);
            TimeRev+=TimeSlice;
        }
    }
    else{
        if(Overlap){
            forwardPosUpdate(Direction,TimeSlice);
            TimeRev-=TimeSlice;
        }
    }

    return TimeRev;
}





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

