#ifndef MY_CLASSES
#define MY_CLASSES

#include "defines.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <iostream>
#include <stdio.h>
#include <math.h>
#include "mat_funcs.h"

class base_Shape{
private:
    GLuint Vertices_VboID,Colors_VboID,Normals_VboID,Indices_VboID,TexCoord_VboID,Height_VboID;
    short VertexCount;
    GLuint VaoID;

public:
    float ObjCentre[4];

    base_Shape(){
        ObjCentre[0]=ObjCentre[1]=ObjCentre[2]=ObjCentre[3]=0.0f;

        Height_VboID=TexCoord_VboID=Vertices_VboID=Indices_VboID=Colors_VboID=Normals_VboID=VaoID=0;
        VertexCount=0;
    }

    void createVao(){
        glGenVertexArrays(1,&VaoID);
        glBindVertexArray(0);
    }

    void createHeightBuffer(GLfloat Data[],GLuint DataSize){
        glBindVertexArray(VaoID);
        glGenBuffers(1, &Height_VboID);
        glBindBuffer(GL_ARRAY_BUFFER, Height_VboID);
        glBufferData(GL_ARRAY_BUFFER, DataSize, Data, GL_STATIC_DRAW);
        glVertexAttribPointer(0,1,GL_FLOAT,GL_FALSE,0,0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void createVertexBuffer(GLfloat Data[],GLuint DataSize){
        glBindVertexArray(VaoID);
        glGenBuffers(1, &Vertices_VboID);
        glBindBuffer(GL_ARRAY_BUFFER, Vertices_VboID);
        glBufferData(GL_ARRAY_BUFFER, DataSize, Data, GL_STATIC_DRAW);
        glVertexAttribPointer(0,4,GL_FLOAT,GL_FALSE,0,0);
        glEnableVertexAttribArray(0);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void createColorBuffer(GLfloat Data[],GLuint DataSize){
        glBindVertexArray(VaoID);
        glGenBuffers(1, &Colors_VboID);
        glBindBuffer(GL_ARRAY_BUFFER, Colors_VboID);
        glBufferData(GL_ARRAY_BUFFER, DataSize, Data, GL_STATIC_DRAW);
        glVertexAttribPointer(1,4,GL_FLOAT,GL_FALSE,0,0);
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void createTexCoordBuffer(GLfloat Data[],GLuint DataSize){
        glBindVertexArray(VaoID);
        glGenBuffers(1, &TexCoord_VboID);
        glBindBuffer(GL_ARRAY_BUFFER, TexCoord_VboID);
        glBufferData(GL_ARRAY_BUFFER, DataSize, Data, GL_STATIC_DRAW);
        glVertexAttribPointer(1,2,GL_FLOAT,GL_FALSE,0,0);
        glEnableVertexAttribArray(1);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void createNormalBuffer(GLfloat Data[],GLuint DataSize){
        glBindVertexArray(VaoID);
        glGenBuffers(1, &Normals_VboID);
        glBindBuffer(GL_ARRAY_BUFFER, Normals_VboID);
        glBufferData(GL_ARRAY_BUFFER, DataSize, Data, GL_STATIC_DRAW);
        glVertexAttribPointer(2,3,GL_FLOAT,GL_FALSE,0,0);
        glEnableVertexAttribArray(2);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void createIndexBuffer(GLushort Data[],GLuint DataSize){
        VertexCount=DataSize/sizeof(Data[0]);
        glGenBuffers(1, &Indices_VboID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Indices_VboID);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, DataSize, Data, GL_STATIC_DRAW);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    GLuint getVaoID(){
        return VaoID;
    }

    void drawElements(GLenum mode){
        glBindVertexArray(VaoID);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Indices_VboID);
        glDrawElements(mode, VertexCount, GL_UNSIGNED_SHORT, NULL);
        glBindVertexArray(0);
    }

    void destroyVboVao(){
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
};

class shader_Prog{
private:
    GLuint VertexShaderID,FragmentShaderID,ProgramID;
    static GLuint CurrentProg;

public:
    shader_Prog(){
        VertexShaderID=FragmentShaderID=ProgramID=0;
    }

    static GLuint getCurrentProg(){
        return shader_Prog::CurrentProg;
    }

    GLuint getProgramID(){
        return ProgramID;
    }

    GLuint getVertexShaderID(){
        return VertexShaderID;
    }

    GLuint getFragmentShaderID(){
        return FragmentShaderID;
    }

    void createShaders(char const * const VertPrg,char const * const FragPrg){
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

    void createShaders(const GLuint VertPrg,char const * const FragPrg){
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

    void createShaders(char const * const VertPrg,const GLuint FragPrg){
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

    void destroyShaders(){
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

    static void disableProgram(){
        if(CurrentProg!=0)
        {
            glUseProgram(0);
            CurrentProg=0;
        }
    }

    void useProgram(){
        if(CurrentProg!=ProgramID)
        {
            glUseProgram(ProgramID);
            CurrentProg=ProgramID;
        }
    }

    void useProgram(const char* UName,float Proj[]){
        if(CurrentProg!=ProgramID)
        {
            glUseProgram(ProgramID);
            CurrentProg=ProgramID;
            glUniformMatrix4fv(glGetUniformLocation(CurrentProg,UName),1,GL_FALSE,Proj);
        }
    }

    void useProgram(const char* UName1,float Proj[],const char* UName2,float View[]){
        if(CurrentProg!=ProgramID)
        {
            glUseProgram(ProgramID);
            CurrentProg=ProgramID;
            glUniformMatrix4fv(glGetUniformLocation(CurrentProg,UName1),1,GL_FALSE,Proj);
            glUniformMatrix4fv(glGetUniformLocation(CurrentProg,UName2),1,GL_FALSE,View);
        }
    }

};

class shape_Object{
protected:
    float ObjVelocity[4];
    bool Floating;
public:
    base_Shape *Shape;
    GLuint Texture;
    float ModelMat[16];

    shape_Object(){
        ObjVelocity[0]=ObjVelocity[1]=ObjVelocity[2]=ObjVelocity[3]=0.0f;
        initMat(ModelMat);
        Shape=NULL;
        Texture=0;
        Floating=true;
    }

    shape_Object(const shape_Object& Obj){
        for(int i=0;i<4;i++)
            ObjVelocity[i]=Obj.ObjVelocity[i];

        for(int i=0;i<16;i++)
            ModelMat[i]=Obj.ModelMat[i];

        Shape=Obj.Shape;
        Texture=Obj.Texture;
        Floating=Obj.Floating;
    }

    void setTexture(GLuint Tex){
        Texture=Tex;
    }

    void setShape(base_Shape* Shp){
        Shape=Shp;
    }

    bool isFloating() const{
        return Floating;
    }

    void resetVelocity(){
        ObjVelocity[0]=0.0f;
        ObjVelocity[1]=0.0f;
        ObjVelocity[2]=0.0f;
        Floating=false;
    }

    void velUpdate(float const Acc[4],float Time){
        if(ObjVelocity[0]>MAX_VELOCITY||
           ObjVelocity[1]>MAX_VELOCITY||
           ObjVelocity[2]>MAX_VELOCITY)
            return;
        ObjVelocity[0]+=Acc[0]*Time;
        ObjVelocity[1]+=Acc[1]*Time;
        ObjVelocity[2]+=Acc[2]*Time;
    }

    virtual void posUpdate(float Time){
        translateMat(ModelMat,ObjVelocity[0]*Time,ObjVelocity[1]*Time,ObjVelocity[2]*Time);
    }


    float velocityMaxComponent() const{
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

    bool onFloor() const{
        if(ModelMat[13]-ModelMat[5]<=FLOOR_HEIGHT+MAX_OVERLAP)
            return true;
        return false;
    }

    bool simpleCuboidCollision (const shape_Object& Piece) const{
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

    float reduceOverlap(const shape_Object& Piece,bool Overlap){
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

};

class player_Class:public shape_Object{
private:
    float Height,Width;
    enum playerPart{Whole,Body,Feet,Head} PlayerPart;
    float Camera[4],Forward[4],Up[4],MouseXAngle,MouseYAngle;
    short Lives;

    void setMatPosition(){
        ModelMat[12]=Camera[0];
        ModelMat[13]=Camera[1];
        ModelMat[14]=Camera[2];
        if(PlayerPart==Feet)
            ModelMat[13]+=-Height*0.95;
        else if(PlayerPart==Head)
            ModelMat[13]+=Height*0.95;
    }

public:

    player_Class(): shape_Object(){
        PlayerPart=Whole;
        Lives=100;
        Width=Height=0.0f;

         Camera[0]=0.0f; Camera[1]=PLAYER_HEIGHT+5;  Camera[2]=0.0f; Camera[3]=1.0f;
        Forward[0]=0.0f;Forward[1]=0;               Forward[2]=1.0f;Forward[3]=0.0f;
             Up[0]=0.0f;     Up[1]=1.0f;                 Up[2]=0.0f;     Up[3]=0.0f;

        setMatPosition();

        MouseXAngle=MouseYAngle=0.0f;
    }

    char getLives(){
        return Lives;
    }

    bool isFloating(){
        return Floating;
    }

    void reduceLives(const char FlrN){
        --Lives;
        Camera[1]=(FlrN+8)*UNIT_DISTANCE;
        setMatPosition();
        Floating=true;
    }

    void reduceLives(){
        --Lives;
    }

    void setPlayerHeightWidth(float H,float W){
        Height=H;Width=W;
        initMat(ModelMat);
        scaleMat(ModelMat,Width*BODY_WIDTH_SCALE,
                 Height*(BODY_HEIGHT_SCALE+FEET_HEAD_HEIGHT_SCALE+FEET_HEAD_HEIGHT_SCALE),
                 Width*BODY_WIDTH_SCALE);
        translateMat(ModelMat,0,Height,0);
        PlayerPart=Whole;
    }

    void switchToPlayerHead(){
        if(PlayerPart==Body||PlayerPart==Whole)
            ModelMat[13]+=Height*0.95f;
        else if(PlayerPart==Feet)
            ModelMat[13]+=Height*1.9f;

        ModelMat[0]=Width*FEET_HEAD_WIDTH_SCALE;
        ModelMat[5]=Height*FEET_HEAD_HEIGHT_SCALE;
        ModelMat[10]=Width*FEET_HEAD_WIDTH_SCALE;
        PlayerPart=Head;
    }

    void switchToPlayerFeet(){
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

    void switchToPlayerBody(){
        if(PlayerPart==Feet)
            ModelMat[13]+=Height*0.95f;
        else if(PlayerPart==Head)
            ModelMat[13]-=Height*0.95f;

        ModelMat[0]=Width*BODY_WIDTH_SCALE;
        ModelMat[5]=Height*BODY_HEIGHT_SCALE;
        ModelMat[10]=Width*BODY_WIDTH_SCALE;
        PlayerPart=Body;
    }

    void switchToPlayer(){
        if(PlayerPart==Feet)
            ModelMat[13]+=Height*0.95f;
        else if(PlayerPart==Head)
            ModelMat[13]-=Height*0.95f;

        ModelMat[0]=Width*BODY_WIDTH_SCALE;
        ModelMat[5]=Height*(BODY_HEIGHT_SCALE+FEET_HEAD_HEIGHT_SCALE+FEET_HEAD_HEIGHT_SCALE);
        ModelMat[10]=Width*BODY_WIDTH_SCALE;
        PlayerPart=Whole;
    }

    void getViewMat(float ViewMat[]) const{
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

    void mouseMove(float X,float Y){
        MouseXAngle=-(X-0.5f)*M_PI*0.4;
        MouseYAngle=-(Y-0.5f)*M_PI*0.4;
    }

    void rotateAboutCamera(float RotateSpeed,float Time){
        float TempMat[16];
        initMat(TempMat);
        rotateYMatrix(KEY_ROTATESPEED*RotateSpeed*Time,TempMat);
        multiplyMatMV(TempMat,Forward,4);
        multiplyMatMV(TempMat,Up,4);
    }

    void getForwardVelocity(float TempVec[],float MoveSpeed){
        TempVec[0]=Forward[0]*MoveSpeed;
        TempVec[1]=Forward[1]*MoveSpeed;
        TempVec[2]=Forward[2]*MoveSpeed;
        TempVec[3]=Forward[3]*MoveSpeed;
    }

    void jumpPlayer(){
        if(Floating || velocityMaxComponent()>1e-10)
            return;
        ObjVelocity[1]=JUMP_VELOCITY;
        Floating=true;
    }

    void posUpdate(float Time){
        Camera[0]+=ObjVelocity[0]*Time;
        Camera[1]+=ObjVelocity[1]*Time;
        Camera[2]+=ObjVelocity[2]*Time;

        setMatPosition();
    }

    void forwardPosUpdate(int Direction,float Time){
        Camera[0]+=Direction*KEY_MOVESPEED*Forward[0]*Time;
        Camera[1]+=Direction*KEY_MOVESPEED*Forward[1]*Time;
        Camera[2]+=Direction*KEY_MOVESPEED*Forward[2]*Time;

        setMatPosition();
    }

    float reduceForwardOverlap(int Direction,const shape_Object& Piece,bool Overlap){
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

};

class control_Interface{
    static bool ArrowKeys[5];
    static int MouseX;
    static int MouseY;

    static void arrowKeyUp(int key,int x,int y){
        if(key == GLUT_KEY_RIGHT)
            {ArrowKeys[RIGHT_KEY]=false;}
        else if(key == GLUT_KEY_LEFT)
            {ArrowKeys[LEFT_KEY]=false;}
        else if(key == GLUT_KEY_UP)
            {ArrowKeys[UP_KEY]=false;}
        else if(key == GLUT_KEY_DOWN)
            {ArrowKeys[DOWN_KEY]=false;}
    }

    static void arrowKeyDown(int key,int x,int y){
        if(key == GLUT_KEY_RIGHT)
            {ArrowKeys[RIGHT_KEY]=true;}
        else if(key == GLUT_KEY_LEFT)
            {ArrowKeys[LEFT_KEY]=true;}
        else if(key == GLUT_KEY_UP)
            {ArrowKeys[UP_KEY]=true;}
        else if(key == GLUT_KEY_DOWN)
            {ArrowKeys[DOWN_KEY]=true;}
    }

    static void otherKeysUp(unsigned char key,int x,int y){
        if(key == GLUT_KEY_SPACEBAR)
            ArrowKeys[SPACE_KEY]=false;
    }

    static void otherKeysDown(unsigned char key,int x,int y){
        if(key == GLUT_KEY_SPACEBAR)
            ArrowKeys[SPACE_KEY]=true;
    }

    static void setMouse(int x,int y){
        MouseX=x;MouseY=y;
    }

public:

    static bool jump(){
        return ArrowKeys[SPACE_KEY];
    }

    static int horizontalCtrl(){
        if(!(ArrowKeys[RIGHT_KEY]^ArrowKeys[LEFT_KEY]))
            return 0;
        if(ArrowKeys[RIGHT_KEY])
            return -1;
        else
            return 1;
    }

    static int verticalCtrl(){
        if(!(ArrowKeys[UP_KEY]^ArrowKeys[DOWN_KEY]))
            return 0;
        if(ArrowKeys[DOWN_KEY])
            return -1;
        else
            return 1;
    }

    static int getMouseX(){
        return MouseX;
    }

    static int getMouseY(){
        return MouseY;
    }

    static void inputSetup(){
        glutKeyboardFunc(otherKeysDown);
        glutKeyboardUpFunc(otherKeysUp);
        glutSpecialFunc(arrowKeyDown);
        glutSpecialUpFunc(arrowKeyUp);
        glutPassiveMotionFunc(setMouse);
    }
};

#endif // MY_CLASSES
