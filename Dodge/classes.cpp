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
public:
    base_Shape *Shape;
    GLuint Texture;
    float ModelMat[16];

    shape_Object(){
        ObjVelocity[0]=ObjVelocity[1]=ObjVelocity[2]=ObjVelocity[3]=0.0f;
        initMat(ModelMat);
        Shape=NULL;
        Texture=0;
    }

    void resetVelocity(){
        ObjVelocity[0]=0.0f;
        ObjVelocity[1]=0.0f;
        ObjVelocity[2]=0.0f;
    }

    void setVelocity(float Vx,float Vy,float Vz){
        ObjVelocity[0]=Vx;
        ObjVelocity[1]=Vy;
        ObjVelocity[2]=Vz;
    }

    void velUpdate(float Acc[4],float Time){
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

    virtual void posUpdate(float const Velocity[],float Time){
        translateMat(ModelMat,Velocity[0]*Time,Velocity[1]*Time,Velocity[2]*Time);
    }

    float velocityMaxComponent(){
        if(ObjVelocity[0]*ObjVelocity[0]>ObjVelocity[1]*ObjVelocity[1])
            if(ObjVelocity[0]*ObjVelocity[0]>ObjVelocity[2]*ObjVelocity[2])
                return (ObjVelocity[0]>0?ObjVelocity[0]:-ObjVelocity[0]);
            else
                return (ObjVelocity[2]>0?ObjVelocity[2]:-ObjVelocity[2]);
        else
            if(ObjVelocity[1]*ObjVelocity[1]>ObjVelocity[2]*ObjVelocity[2])
                return (ObjVelocity[1]>0?ObjVelocity[1]:-ObjVelocity[1]);
            else
                return (ObjVelocity[2]>0?ObjVelocity[2]:-ObjVelocity[2]);
    }

    bool onFloor(){
        if(ModelMat[13]-ModelMat[5]<=FLOOR_HEIGHT+MAX_OVERLAP)
            return true;
        return false;
    }

    bool simpleCuboidCollision(float const * const BlockModelMat){
        float Pos,Size;
        Pos=ModelMat[14]-BlockModelMat[14];if(Pos<0.0f)Pos*=-1.0f;
        Size=ModelMat[10]+BlockModelMat[10];
        if(Pos<Size){
            Pos=ModelMat[13]-BlockModelMat[13];if(Pos<0.0f)Pos*=-1.0f;
            Size=ModelMat[5]+BlockModelMat[5];
            if(Pos<Size){
                Pos=ModelMat[12]-BlockModelMat[12];if(Pos<0.0f)Pos*=-1.0f;
                Size=ModelMat[0]+BlockModelMat[0];
                if(Pos<Size){
                    return true;
                }
            }
        }
        return false;
    }

    float reduceOverlap(shape_Object const * const Piece,bool Overlap){
        float TimeSlice,TimeRev;
        TimeRev=TimeSlice=UNIT_TIME/2;
        posUpdate(-TimeSlice);
        while(velocityMaxComponent()*TimeSlice>MAX_OVERLAP){
            TimeSlice/=2;
            if(simpleCuboidCollision(Piece->ModelMat)){
                posUpdate(-TimeSlice);
                TimeRev+=TimeSlice;
            }
            else{
                posUpdate(TimeSlice);
                TimeRev-=TimeSlice;
            }
        }
        if(simpleCuboidCollision(Piece->ModelMat)){
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

    float reduceOverlap(float const * const Velocity,shape_Object const * const Piece,bool Overlap){
        float TimeSlice,TimeRev;
        TimeRev=TimeSlice=UNIT_TIME/2;
        posUpdate(Velocity,-TimeSlice);
        while(velocityMaxComponent()*TimeSlice>MAX_OVERLAP){
            TimeSlice/=2;
            if(simpleCuboidCollision(Piece->ModelMat)){
                posUpdate(Velocity,-TimeSlice);
                TimeRev+=TimeSlice;
            }
            else{
                posUpdate(Velocity,TimeSlice);
                TimeRev-=TimeSlice;
            }
        }
        if(simpleCuboidCollision(Piece->ModelMat)){
            if(!Overlap){
                posUpdate(Velocity,-TimeSlice);
                TimeRev+=TimeSlice;
            }
        }
        else{
            if(Overlap){
                posUpdate(Velocity,TimeSlice);
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
    char Lives;

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
        Lives=1;
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

    void reduceLives(const char FlrN){
        --Lives;std::cout<<Lives;
        Camera[1]=(MAX_FLOOR+FlrN+5)*UNIT_DISTANCE;
        setMatPosition();
    }

    void reduceLives(){
        --Lives;std::cout<<Lives;
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
        PlayerPart=Body;
    }

    void setTexture(GLuint Tex){
        Texture=Tex;
    }

    void setShape(base_Shape* Shp){
        Shape=Shp;
    }

    void getViewMat(float ViewMat[]){
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

    void mouseCam(float X,float Y){
        MouseXAngle=-(X-0.5f)*M_PI*0.4;
        MouseYAngle=-(Y-0.5f)*M_PI*0.4;
    }

    void rotateAboutCamera(float RotateSpeed,float Time){
        float TempMat[16];
        initMat(TempMat);
        rotateYMatrix(RotateSpeed*Time,TempMat);
        multiplyMatMV(TempMat,Forward,4);
        multiplyMatMV(TempMat,Up,4);
    }

    void getForwardVelocityVector(float Velocity[],float MoveSpeed){
        Velocity[0]=Forward[0];Velocity[1]=Forward[1];Velocity[2]=Forward[2];
        normalizeVector(Velocity);
        Velocity[0]*=MoveSpeed;
        Velocity[1]*=MoveSpeed;
        Velocity[2]*=MoveSpeed;
    }

    void jumpPlayer(){
        setVelocity(0.0f,JUMP_VELOCITY,0.0f);
//        setMatPosition();
    }

    void posUpdate(float Time){
        Camera[0]+=ObjVelocity[0]*Time;
        Camera[1]+=ObjVelocity[1]*Time;
        Camera[2]+=ObjVelocity[2]*Time;

        setMatPosition();
    }

    void posUpdate(float const Velocity[],float Time){
        Camera[0]+=Velocity[0]*Time;
        Camera[1]+=Velocity[1]*Time;
        Camera[2]+=Velocity[2]*Time;

        setMatPosition();
    }

};

#endif // MY_CLASSES
