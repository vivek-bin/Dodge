
#include "defines.h"

#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctime>
#include <cstdlib>
#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <math.h>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "mat_funcs.h"
#include "classes.cpp"

GLuint shader_Prog::CurrentProg;

int CurrentWidth = 800*3/2, CurrentHeight = 600*3/2, WindowHandle = 0;
unsigned int FrameCount = 0;

GLubyte ArrowKeys[5];

GLfloat ProjMat[16],ViewMat[16];

GLuint ProjMatUbo,ViewMatUbo;
GLuint Textures[NO_OF_TEXTURES+4];

base_Shape Box;
shader_Prog TexShader, SimpleTexShader;
object_Stack BlockStack;
small_Queue FloatingBlocksQ(MAX_FLOATING_BLOCKS);

shape_Object SceneryCube,Clouds;

player_Class Player1;

char Heights[FLOOR_SIZE][FLOOR_SIZE];

char FloorNo;
float Gravity[4]={0.0f,-8.0f,0.0f,0.0f};

const GLchar* TexVertShader = { SHADERCODE(
 layout(location=0) in vec4 in_Position;
 layout(location=1) in vec2 in_TexCoords;
 layout(location=2) in vec3 in_Normal;
 uniform mat4 M;
 uniform mat4 V;
 uniform mat4 P;
 out vec2 fg_TexCoords;
 out vec3 fg_Normal;
 out vec4 fg_Position;
 void main(){
     fg_TexCoords.x=in_TexCoords.x*2*(M[0][0]+M[2][2]);
     fg_TexCoords.y=in_TexCoords.y*(M[1][1]+M[2][2]);
     fg_Position=M*in_Position;
     gl_Position=P*V*M*in_Position;
     fg_Normal=vec3(normalize(M*vec4(in_Normal,0.0)));
 }
)};

const GLchar* TexFragShader = { SHADERCODE(
 in vec2 fg_TexCoords;
 in vec3 fg_Normal;
 in vec4 fg_Position;
 uniform mat4 M;
 uniform mat4 V;
 uniform sampler2D basic_texture;
 uniform vec4 transparent_colour=vec4(1.0f,1.0f,1.0f,1.0f);
 uniform float shadow_coords[5*MAX_FLOATING_BLOCKS];
 out vec4 frag_colour;

 void main(){

     frag_colour = texture(basic_texture,fg_TexCoords);
     if(frag_colour == transparent_colour)
        discard;

     vec4 light_position_world = vec4 (0.0, 25.0, 0.0, 1.0);
     vec3 Ls = vec3 (0.0, 0.0, 0.0); // white specular colour // no specular lighting (all zeros)
     vec3 Ld = vec3 (0.8, 0.8, 0.8); // dull white diffuse light colour
     vec3 La = vec3 (0.4, 0.4, 0.4); // grey ambient colour
     // surface reflectance
     vec3 Ks = vec3 (1.0, 1.0, 1.0); // fully reflect specular light
     vec3 Kd = vec3 (1.0, 1.0, 1.0); // diffuse surface reflectance
     vec3 Ka = vec3 (1.0, 1.0, 1.0); // fully reflect ambient light
     float specular_exponent = 80.0; // specular 'power'
     // ambient intensity
     vec3 Ia = La * Ka;
     // diffuse intensity
     vec3 direction_to_light =vec3(normalize(light_position_world - fg_Position));
     vec3 Id = Ld * Kd * max(dot(direction_to_light, fg_Normal),0.0); // final diffuse intensity

     //specular light
     vec3 reflection = reflect (direction_to_light, fg_Normal);
     vec3 surface_to_viewer = normalize(vec3(V[0][3]-fg_Position.x,V[1][3]-fg_Position.y,V[2][3]-fg_Position.z));
     float dot_prod_specular = max (dot (reflection, surface_to_viewer), 0.0);
     float specular_factor = pow (dot_prod_specular, specular_exponent);
     vec3 Is = Ls * Ks * specular_factor; // final specular intensity

     frag_colour = vec4(Is + Id + Ia, 1.0) * frag_colour;

     float HL;
     float HW;
     float HeightDistance;
     float MinHD=1e10;
     for(int i=0;i<MAX_FLOATING_BLOCKS;++i){
        HeightDistance=shadow_coords[3+i*5]-fg_Position.y;
        HL=shadow_coords[0+i*5]*(1+0.1*HeightDistance);
        HW=shadow_coords[1+i*5]*(1+0.1*HeightDistance);

        if(shadow_coords[2+i*5]-HL<fg_Position.x
        && shadow_coords[2+i*5]+HL>fg_Position.x
        && shadow_coords[4+i*5]-HW<fg_Position.z
        && shadow_coords[4+i*5]+HW>fg_Position.z
        && shadow_coords[0+i*5] < 1e19
        && HeightDistance > 0
        && HeightDistance < MinHD)
            MinHD=HeightDistance;
     }
     if(MinHD<1e9){
        if(MinHD>18)MinHD=18;
        frag_colour = (MinHD*0.05+0.1) * frag_colour;
     }
 }
)};

const GLchar* SimpleTexVertShader = { SHADERCODE(
 layout(location=0) in vec4 in_Position;
 layout(location=1) in vec2 in_TexCoords;
 uniform mat4 M;
 uniform mat4 V;
 uniform mat4 P;
 out vec2 fg_TexCoords;
 void main(){
     fg_TexCoords=in_TexCoords;
     gl_Position=P*V*M*in_Position;
 }
)};

const GLchar* SimpleTexFragShader = { SHADERCODE(
 in vec2 fg_TexCoords;
 uniform sampler2D basic_texture;
 uniform vec4 transparent_colour=vec4(1.0f,1.0f,1.0f,1.0f);
 out vec4 frag_colour;

 void main(){
     frag_colour = texture(basic_texture,fg_TexCoords);
     if(frag_colour == transparent_colour)
        discard;

 }
)};


void initialize(int ,char* []);
void initWindow(int ,char* []);
void initializeData();
void timerFunction(int );
void idleFunction();
void renderFunction();
void resizeFunction(int ,int );
void cleanUp();
void arrowKeyDown(int ,int ,int );
void arrowKeyUp(int ,int ,int );
void otherKeysDown(unsigned char ,int ,int );
void otherKeysUp(unsigned char ,int ,int );
void createProjMat(float[],float,float,float,float);
GLuint loadTexture(const char *,const GLuint,const GLuint,const GLuint);
void createBox();
void nextBlock();
void moveBlocks();
void movePlayer();
void initGame();
void setMouseCam(int,int);

int main(int argc,char* argv[]){
    srand(time(0));

    initialize(argc,argv);

    glutMainLoop();

    exit(EXIT_SUCCESS);
}

void initialize(int argc,char* argv[]){
    GLenum GlewErrorCheck;
    glewExperimental=GL_TRUE;
    initWindow(argc,argv);
    GlewErrorCheck=glewInit();
    if(GLEW_OK!=GlewErrorCheck){
        fprintf(stderr,"ERROR:%s",glewGetErrorString(GlewErrorCheck));
        exit(EXIT_FAILURE);
    }
    fprintf(stdout,"OpenGL Version : %s",glGetString(GL_VERSION));
    glEnable(GL_TEXTURE_2D);
    initGame();
    createProjMat(ProjMat,MAX_OVERLAP,100.0f,67.0f,(float)CurrentWidth/CurrentHeight);
    glClearColor(0.68f,0.68f,1.0f,1.0f);
}

void initWindow(int argc,char* argv[]){
    glutInit(&argc,argv);
    glutInitContextVersion(4,0);
    glutInitContextFlags(GLUT_FORWARD_COMPATIBLE);
    glutInitContextProfile(GLUT_CORE_PROFILE);
    glutSetOption(GLUT_ACTION_ON_WINDOW_CLOSE,GLUT_ACTION_GLUTMAINLOOP_RETURNS);
    glutInitWindowSize(CurrentWidth,CurrentHeight);
    glutInitDisplayMode(GLUT_DEPTH|GLUT_RGBA|GLUT_DOUBLE);

    WindowHandle = glutCreateWindow(WINDOW_TITLE_PREFIX);

    if(WindowHandle < 1)
    {
        fprintf(stderr, "UNABLE TO CREATE WINDOW");
        exit(-1);
    }

    glEnable(GL_DEPTH_TEST);

    glutKeyboardFunc(otherKeysDown);
    glutKeyboardUpFunc(otherKeysUp);
    glutSpecialFunc(arrowKeyDown);
    glutSpecialUpFunc(arrowKeyUp);
    glutIdleFunc(idleFunction);
    glutDisplayFunc(renderFunction);
    glutReshapeFunc(resizeFunction);
    glutCloseFunc(cleanUp);
    glutTimerFunc(0,timerFunction,0);
    glutPassiveMotionFunc(setMouseCam);
}

void setMouseCam(int x,int y){
    Player1.mouseCam((float)x/CurrentWidth,(float)y/CurrentHeight);
}

void keyCheck(){
    float RotateSpeed, MoveSpeed;
    float TempVec[4];
    RotateSpeed=60.0f;
    MoveSpeed=2.0f;
    void *Temp;
    if((ArrowKeys[RIGHT_KEY]^ArrowKeys[LEFT_KEY])==1){
        if(ArrowKeys[RIGHT_KEY]==1)
        {RotateSpeed*=-1;}

        Player1.switchToPlayerBody();
        Player1.rotateAboutCamera(RotateSpeed,UNIT_TIME);
    }
    if((ArrowKeys[UP_KEY]^ArrowKeys[DOWN_KEY])==1){
        if(ArrowKeys[DOWN_KEY]==1)
        {MoveSpeed*=-1;}

        Player1.switchToPlayerBody();
        Player1.getForwardVelocityVector(TempVec,MoveSpeed);
        Player1.posUpdate(TempVec,UNIT_TIME);
        for(Temp=BlockStack.getTop();Temp!=NULL;Temp=BlockStack.getNext(Temp)){
            if(Player1.simpleCuboidCollision((BlockStack.getData(Temp))->ModelMat)){
                Player1.reduceOverlap(TempVec,BlockStack.getData(Temp),false);
                break;
            }
        }
    }
    if(ArrowKeys[SPACE_KEY]==1){
        if(Player1.onFloor()){
            Player1.jumpPlayer();
        }
        else{
            Player1.switchToPlayerFeet();
            for(Temp=BlockStack.getTop();Temp!=NULL;Temp=BlockStack.getNext(Temp)){
                if(Player1.simpleCuboidCollision((BlockStack.getData(Temp))->ModelMat)){
                    Player1.jumpPlayer();
                    break;
                }
            }
        }
    }
}

void arrowKeyUp(int key,int x,int y){
    if(key == GLUT_KEY_RIGHT)
        {ArrowKeys[RIGHT_KEY]=0;}
    else if(key == GLUT_KEY_LEFT)
        {ArrowKeys[LEFT_KEY]=0;}
    else if(key == GLUT_KEY_UP)
        {ArrowKeys[UP_KEY]=0;}
    else if(key == GLUT_KEY_DOWN)
        {ArrowKeys[DOWN_KEY]=0;}
}

void arrowKeyDown(int key,int x,int y){
    if(key == GLUT_KEY_RIGHT)
        {ArrowKeys[RIGHT_KEY]=1;}
    else if(key == GLUT_KEY_LEFT)
        {ArrowKeys[LEFT_KEY]=1;}
    else if(key == GLUT_KEY_UP)
        {ArrowKeys[UP_KEY]=1;}
    else if(key == GLUT_KEY_DOWN)
        {ArrowKeys[DOWN_KEY]=1;}
}

void otherKeysUp(unsigned char key,int x,int y){
    if(key == GLUT_KEY_SPACEBAR)
        ArrowKeys[SPACE_KEY]=0;
}

void otherKeysDown(unsigned char key,int x,int y){
    if(key == GLUT_KEY_SPACEBAR)
        ArrowKeys[SPACE_KEY]=1;
}

void idleFunction(){
    glutPostRedisplay();
}

void renderFunction(){
    void * Temp;
    shape_Object *Block,*Shadow;

    float ShadowData[5*MAX_FLOATING_BLOCKS];
    float *MM;
    short i,j;
    GLuint MMUniformLocation;

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    Player1.getViewMat(ViewMat);

    SimpleTexShader.useProgram("P",ProjMat);
    glUniformMatrix4fv(glGetUniformLocation(shader_Prog::getCurrentProg(),"V")
                       ,1,GL_FALSE,ViewMat);

    glBindTexture(GL_TEXTURE_2D,SceneryCube.Texture);
    glUniformMatrix4fv(glGetUniformLocation(shader_Prog::getCurrentProg(),"M"),
                           1,GL_FALSE,SceneryCube.ModelMat);
    (SceneryCube.Shape)->drawElements(GL_TRIANGLES);


    glBindTexture(GL_TEXTURE_2D,Clouds.Texture);
    glUniformMatrix4fv(glGetUniformLocation(shader_Prog::getCurrentProg(),"M"),
                           1,GL_FALSE,Clouds.ModelMat);
    (Clouds.Shape)->drawElements(GL_TRIANGLES);
    rotateYMatrix(0.02,Clouds.ModelMat);

    TexShader.useProgram("P",ProjMat);
    glUniformMatrix4fv(glGetUniformLocation(shader_Prog::getCurrentProg(),"V")
                       ,1,GL_FALSE,ViewMat);

    for(j=0;j<MAX_FLOATING_BLOCKS; ++j){
        ShadowData[j*5]=1e20;
        Shadow=FloatingBlocksQ.getData(j);
        if(Shadow!=NULL){
            MM=Shadow->ModelMat;
            ShadowData[0+j*5]=MM[0];
            ShadowData[1+j*5]=MM[10];
            ShadowData[2+j*5]=MM[12];
            ShadowData[3+j*5]=MM[13]-MM[5];
            ShadowData[4+j*5]=MM[14];
        }
    }

    glUniform1fv(glGetUniformLocation(shader_Prog::getCurrentProg(),"shadow_coords")
                 ,5*MAX_FLOATING_BLOCKS,ShadowData);

    MMUniformLocation=glGetUniformLocation(shader_Prog::getCurrentProg(),"M");
    Temp=BlockStack.getTop();
    while(Temp!=NULL){
        Block=BlockStack.getData(Temp);
        glBindTexture(GL_TEXTURE_2D,Block->Texture);
        glUniformMatrix4fv(MMUniformLocation,1,GL_FALSE,Block->ModelMat);
        (Block->Shape)->drawElements(GL_TRIANGLES);
        Temp=BlockStack.getNext(Temp);
    }
    glutSwapBuffers();

    ++FrameCount;
}

void resizeFunction(int Width, int Height){
    CurrentWidth=Width;
    CurrentHeight=Height;
    glViewport(0,0,CurrentWidth,CurrentHeight);
}

void cleanUp(){
   TexShader.destroyShaders();
   SimpleTexShader.destroyShaders();
}

void timerFunction(int n){
    if(n==1){
        char temp[120];
        sprintf(temp,"%s %d x %d  %d FPS",WINDOW_TITLE_PREFIX,
                CurrentWidth,CurrentHeight,(int)(FrameCount/UNIT_TIME));
        glutSetWindowTitle(temp);
    }
    if(rand()%NEW_BLOCK_FREQUENCY==0)
    {  nextBlock(); }
    keyCheck();
    moveBlocks();
    movePlayer();
    Player1.switchToPlayer();
    glutTimerFunc(UNIT_TIME*1000,timerFunction,1);
    FrameCount=0;
}

void createProjMat(float ProjMat[],float Near,float Far,float Fov,float Aspect){

    float Range,Sx,Sy,Sz,Pz;
    Fov *= M_PI/180.0f; // convert degrees to radians

    Range = tan (Fov * 0.5f) * Near;
    Sx = Near / (Range * Aspect);
    Sy = Near / Range;
    Sz = -(Far + Near) / (Far - Near);
    Pz = -(2.0f * Far * Near) / (Far - Near);

    ProjMat[0]=Sx;   ProjMat[4]=0.0f; ProjMat[8]=0.0f; ProjMat[12]=0.0f;
    ProjMat[1]=0.0f; ProjMat[5]=Sy;   ProjMat[9]=0.0f; ProjMat[13]=0.0f;
    ProjMat[2]=0.0f; ProjMat[6]=0.0f; ProjMat[10]=Sz;  ProjMat[14]=Pz;
    ProjMat[3]=0.0f; ProjMat[7]=0.0f; ProjMat[11]=-1;  ProjMat[15]=0.0f;
}

GLuint loadTexture(const char * path,const GLuint Params,const GLuint Paramt,const GLuint FilterParm){
    GLuint tex=0;
    int x,y,comp;
    unsigned char* image;
    image = stbi_load(path,&x,&y,&comp,STBI_rgb_alpha);
    if(!image){
        fprintf(stdout,"error loading image");
    }
    glGenTextures (1, &tex);
    glActiveTexture (GL_TEXTURE0);
    glBindTexture (GL_TEXTURE_2D, tex);
    glTexImage2D (GL_TEXTURE_2D,0,GL_RGBA,x,y,0,GL_RGBA,GL_UNSIGNED_BYTE,image);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, Params);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, Paramt);
    glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, FilterParm);
    glTexParameterf (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, FilterParm);
    STBI_FREE(image);
    return tex;
}

void createBox(){
    GLfloat Vertices[]={
        //front
         1.0f, 1.0f, 1.0f,1.0f,
        -1.0f, 1.0f, 1.0f,1.0f,
        -1.0f,-1.0f, 1.0f,1.0f,
         1.0f,-1.0f, 1.0f,1.0f,
        //left
        -1.0f, 1.0f, 1.0f,1.0f,
        -1.0f, 1.0f,-1.0f,1.0f,
        -1.0f,-1.0f,-1.0f,1.0f,
        -1.0f,-1.0f, 1.0f,1.0f,
        //back
        -1.0f, 1.0f,-1.0f,1.0f,
         1.0f, 1.0f,-1.0f,1.0f,
         1.0f,-1.0f,-1.0f,1.0f,
        -1.0f,-1.0f,-1.0f,1.0f,
        //right
         1.0f, 1.0f,-1.0f,1.0f,
         1.0f, 1.0f, 1.0f,1.0f,
         1.0f,-1.0f, 1.0f,1.0f,
         1.0f,-1.0f,-1.0f,1.0f,
        //top
         1.0f, 1.0f, 1.0f,1.0f,
         1.0f, 1.0f,-1.0f,1.0f,
        -1.0f, 1.0f,-1.0f,1.0f,
        -1.0f, 1.0f, 1.0f,1.0f,
        //bottom
         1.0f,-1.0f, 1.0f,1.0f,
        -1.0f,-1.0f, 1.0f,1.0f,
        -1.0f,-1.0f,-1.0f,1.0f,
         1.0f,-1.0f,-1.0f,1.0f
    };

    GLfloat Normals[]={
        0.0f,0.0f,1.0f,
        0.0f,0.0f,1.0f,
        0.0f,0.0f,1.0f,
        0.0f,0.0f,1.0f,

       -1.0f,0.0f,0.0f,
       -1.0f,0.0f,0.0f,
       -1.0f,0.0f,0.0f,
       -1.0f,0.0f,0.0f,

        0.0f,0.0f,-1.0f,
        0.0f,0.0f,-1.0f,
        0.0f,0.0f,-1.0f,
        0.0f,0.0f,-1.0f,

        1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,
        1.0f,0.0f,0.0f,

        0.0f,1.0f,0.0f,
        0.0f,1.0f,0.0f,
        0.0f,1.0f,0.0f,
        0.0f,1.0f,0.0f,

        0.0f,-1.0f,0.0f,
        0.0f,-1.0f,0.0f,
        0.0f,-1.0f,0.0f,
        0.0f,-1.0f,0.0f
    };

    GLushort Indices[]={
        0,1,2,
        0,2,3,

        4,5,6,
        4,6,7,

        8,9,10,
        8,10,11,

        12,13,14,
        12,14,15,

        16,17,18,
        16,18,19,

        20,21,22,
        20,22,23
    };

    GLfloat TexCoords[]={
        1.0f,0.499f,
        0.75f,0.499f,
        0.75f,1.0f,
        1.0f,1.0f,

        0.75f,0.499f,
        0.5f,0.499f,
        0.5f,1.0f,
        0.75f,1.0f,

        0.5f,0.499f,
        0.25f,0.499f,
        0.25f,1.0f,
        0.5f,1.0f,

        0.25f,0.499f,
        0.0f,0.499f,
        0.0f,1.0f,
        0.25f,1.0f,

        0.0f,0.499f,
        0.25f,0.499f,
        0.25f,0.0f,
        0.0f,0.0f,

        0.75f,0.499f,
        1.0f,0.499f,
        1.0f,0.0f,
        0.75f,0.0f

//        0.25f,0.5f,
//        0.5f,0.5f,
//        0.5f,0.0f,
//        0.25f,0.0f
    };

    Box.createVao();
    Box.createVertexBuffer(Vertices,sizeof(Vertices));
    Box.createNormalBuffer(Normals,sizeof(Normals));
    Box.createTexCoordBuffer(TexCoords,sizeof(TexCoords));
    Box.createIndexBuffer(Indices,sizeof(Indices));
    glBindVertexArray(0);
}

void nextBlock(){
    int BlockPos,RNo;
    short BlocksLeft;
    short x,y,i,j;
    short xn,xp,yn,yp,Height,MaxHeight;
    char flag;
    shape_Object* Next;

    if(FloatingBlocksQ.numElements()>=MAX_FLOATING_BLOCKS || FloorNo>MAX_FLOOR)
        return;

    do{
        for(BlocksLeft=i=0;i<FLOOR_SIZE;++i)
            for(j=0;j<FLOOR_SIZE;++j)
                if(Heights[i][j]<FloorNo*HEIGHT_DIVISIONS)
                    ++BlocksLeft;

        if(BlocksLeft==0)++FloorNo;
        if(FloorNo>MAX_FLOOR)return;
    }while(BlocksLeft==0);

    BlockPos=(rand()>>2)%BlocksLeft;
    for(x=0; x<FLOOR_SIZE ;++x){
        for(y=0; y<FLOOR_SIZE ;++y){
            if(Heights[x][y]<FloorNo*HEIGHT_DIVISIONS)
                --BlockPos;
            if(BlockPos<0)break;
        }
        if(BlockPos<0)break;
    }
    for(i=x,j=y,xn=1;i>0;--i){
        if(Heights[i-1][j] >= FloorNo*HEIGHT_DIVISIONS)
            break;
        ++xn;if(xn>=MAX_BLOCK_SIZE)break;
    }
    for(i=x,j=y,xp=1;i<FLOOR_SIZE-1;++i){
        if(Heights[i+1][j] >= FloorNo*HEIGHT_DIVISIONS)
            break;
        ++xp;if(xp>=MAX_BLOCK_SIZE)break;
    }
    for(i=x,j=y,yn=1;j>0;--j){
        if(Heights[i][j-1] >= FloorNo*HEIGHT_DIVISIONS)
            break;
        ++yn;if(yn>=MAX_BLOCK_SIZE)break;
    }
    for(i=x,j=y,yp=1;j<FLOOR_SIZE-1;++j){
        if(Heights[i][j+1] >= FloorNo*HEIGHT_DIVISIONS)
            break;
        ++yp;if(yp>=MAX_BLOCK_SIZE)break;
    }

    do{
        flag=0;
        for(i=x-xn+1;(i<=x+xp-1)&&(flag==0);++i){
            for(j=y-yn+1;(j<=y+yp-1)&&(flag==0);++j){
                if(Heights[i][j]>=FloorNo*HEIGHT_DIVISIONS){
                    flag=1;
                    i-=x;j-=y;
                    if(i<0 && j<0)
                            if(-i>-j)xn=-i;else yn=-j;
                    else if(i<0 && j>0)
                            if(-i>j)xn=-i;else yp=j;
                    else if(i>0 && j<0)
                            if(i>-j)xp=i;else yn=-j;
                    else
                            if(i>j)xp=i;else yp=j;
                }
            }
        }
    }while(flag==1);

    RNo=rand();
    i=(RNo%MAX_BLOCK_SIZE)+1;
    while(xn+xp>i+1){
        if(xn>xp)--xn;
        else --xp;
    }

    RNo>>=4;
    i=(RNo%MAX_BLOCK_SIZE)+1;
    while(yn+yp>i+1){
        if(yn>yp)--yn;
        else --yp;
    }

    //generate random height
    RNo>>=4;
    Height=(RNo%MAX_BLOCK_HEIGHT)+1;

    MaxHeight=0;
    for(i=x-xn+1;i<=x+xp-1;++i){
        for(j=y-yn+1;j<=y+yp-1;++j){
            if(Heights[i][j]>MaxHeight)
                MaxHeight=Heights[i][j];
        }
    }
    for(i=x-xn+1;i<=x+xp-1;++i){
        for(j=y-yn+1;j<=y+yp-1;++j){
            Heights[i][j]=Height+MaxHeight;
        }
    }
    Next=new shape_Object();
    Next->Shape=&Box;
    scaleMat(Next->ModelMat,UNIT_DISTANCE*0.5f*(xn+xp-1)*0.999f,
                            UNIT_DISTANCE*0.5f*(Height/HEIGHT_DIVISIONS),
                            UNIT_DISTANCE*0.5f*(yn+yp-1)*0.999f);
    translateMat(Next->ModelMat,UNIT_DISTANCE*(x-(FLOOR_SIZE-1)*0.5f+(xp-xn)*0.5f),
                                UNIT_DISTANCE*(MAX_FLOOR+FloorNo)*2,
                                UNIT_DISTANCE*(y-(FLOOR_SIZE-1)*0.5f+(yp-yn)*0.5f));
    Next->Texture=Textures[(rand()%NO_OF_TEXTURES)];
    BlockStack.Add(Next);
    FloatingBlocksQ.addElement(Next);
}

void initGame(){
    int i,j;
    shape_Object* Floor;
    initMat(ProjMat);
    ArrowKeys[0]=ArrowKeys[1]=ArrowKeys[2]=ArrowKeys[3]=0;
    FloorNo=1;

    for(i=0;i<FLOOR_SIZE;++i)
        for(j=0;j<FLOOR_SIZE;++j)
            Heights[i][j]=0.0f;

    TexShader.createShaders(TexVertShader,TexFragShader);
    SimpleTexShader.createShaders(SimpleTexVertShader,SimpleTexFragShader);

    createBox();
    Textures[0]=loadTexture("./images/block0.bmp",GL_REPEAT,GL_REPEAT,GL_LINEAR);
    Textures[1]=loadTexture("./images/block1.bmp",GL_REPEAT,GL_REPEAT,GL_LINEAR);
    Textures[2]=loadTexture("./images/block2.bmp",GL_REPEAT,GL_REPEAT,GL_LINEAR);
    Textures[3]=loadTexture("./images/block3.bmp",GL_REPEAT,GL_REPEAT,GL_LINEAR);
    Textures[NO_OF_TEXTURES]=loadTexture("./images/floor.bmp",GL_REPEAT,GL_REPEAT,GL_LINEAR);
    Textures[NO_OF_TEXTURES+1]=loadTexture("./images/walls.bmp",GL_REPEAT,GL_REPEAT,GL_LINEAR);

    SceneryCube.Shape=&Box;
    scaleMat(SceneryCube.ModelMat,SCENE_CUBE_SIZE,SCENE_CUBE_SIZE*0.5f,SCENE_CUBE_SIZE);
    translateMat(SceneryCube.ModelMat,0,SCENE_CUBE_SIZE*0.5f,0);
    SceneryCube.Texture=loadTexture("./images/scenery.bmp",GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR);

    Clouds.Shape=&Box;
    scaleMat(Clouds.ModelMat,SCENE_CUBE_SIZE,0.01,SCENE_CUBE_SIZE);
    translateMat(Clouds.ModelMat,0,SCENE_CUBE_SIZE-0.5f,0);
    Clouds.Texture=loadTexture("./images/clouds.bmp",GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_NEAREST);

    float WallW=0.02f;
    float WallH=4.5f*2;
    float WallL=FLOOR_SIZE*UNIT_DISTANCE*0.5;
    shape_Object *Wall1,*Wall2,*Wall3,*Wall4;

    Wall1=new shape_Object();
    Wall2=new shape_Object();
    Wall3=new shape_Object();
    Wall4=new shape_Object();
    Wall1->Texture=Textures[NO_OF_TEXTURES+1];
    Wall2->Texture=Textures[NO_OF_TEXTURES+1];
    Wall3->Texture=Textures[NO_OF_TEXTURES+1];
    Wall4->Texture=Textures[NO_OF_TEXTURES+1];
    Wall1->Shape=&Box;
    Wall2->Shape=&Box;
    Wall3->Shape=&Box;
    Wall4->Shape=&Box;
    scaleMat(Wall1->ModelMat,WallW,WallH,WallL);
    scaleMat(Wall2->ModelMat,WallW,WallH,WallL);
    scaleMat(Wall3->ModelMat,WallL,WallH,WallW);
    scaleMat(Wall4->ModelMat,WallL,WallH,WallW);
    translateMat(Wall1->ModelMat,-(WallW+WallL),WallH, 0.0f);
    translateMat(Wall2->ModelMat, (WallW+WallL),WallH, 0.0f);
    translateMat(Wall3->ModelMat, 0.0f,WallH,-(WallW+WallL));
    translateMat(Wall4->ModelMat, 0.0f,WallH, (WallW+WallL));
    BlockStack.Add(Wall1);
    BlockStack.Add(Wall2);
    BlockStack.Add(Wall3);
    BlockStack.Add(Wall4);

    Player1.setPlayerHeightWidth(PLAYER_HEIGHT,PLAYER_WIDTH);
    Player1.Texture=Textures[NO_OF_TEXTURES];
    Player1.setShape(&Box);

    Floor=new shape_Object();
    Floor->Shape=&Box;
    scaleMat(Floor->ModelMat,FLOOR_SIZE*UNIT_DISTANCE*0.5,0.5f,FLOOR_SIZE*UNIT_DISTANCE*0.5);
    translateMat(Floor->ModelMat,0,-0.25f,0);
    Floor->Texture=Textures[NO_OF_TEXTURES];
    BlockStack.Add(Floor);
}

void movePlayer(){
    void *Temp;
    shape_Object* Block;
    float TimeRev;
    bool VelResetFlag=false;

    Player1.posUpdate(UNIT_TIME);
    Temp=BlockStack.getTop();
    while(Temp!=NULL){
        Block=BlockStack.getData(Temp);
        Player1.switchToPlayer();
        if(Player1.simpleCuboidCollision(Block->ModelMat)){
            TimeRev=0;
            if(Player1.velocityMaxComponent()>1e-5)
                TimeRev=Player1.reduceOverlap(Block,true);
            Player1.switchToPlayerFeet();
            if(Player1.simpleCuboidCollision(Block->ModelMat)){
                if(Player1.velocityMaxComponent()>=MAX_VELOCITY*LETHAL_VELOCITY_MULTIPLIER)
                    Player1.reduceLives();
                VelResetFlag=true;
                break;
            }
            Player1.switchToPlayerHead();
            if(Player1.simpleCuboidCollision(Block->ModelMat)){
                VelResetFlag=true;
                break;
            }
            Player1.posUpdate(TimeRev);
        }
        Temp=BlockStack.getNext(Temp);
    }
    if(VelResetFlag)
        Player1.resetVelocity();
    else
        Player1.velUpdate(Gravity,UNIT_TIME);

}

void moveBlocks(){
    short i;
    float TimeRev;
    void *Temp;
    shape_Object *Block;
    shape_Object *TempBlock;

    for(i=FloatingBlocksQ.getFront();i!=-1;i=FloatingBlocksQ.getNext(i)){
        Block=FloatingBlocksQ.getData(i);
        Block->velUpdate(Gravity,UNIT_TIME);
        Block->posUpdate(UNIT_TIME);

        Player1.switchToPlayer();
        if(Block->simpleCuboidCollision(Player1.ModelMat)){
            TimeRev=Block->reduceOverlap((shape_Object*)&Player1,true);
            Player1.switchToPlayerHead();
            if(Block->simpleCuboidCollision(Player1.ModelMat))
                Player1.reduceLives(FloorNo);
            Block->posUpdate(TimeRev);
        }

        Temp=BlockStack.getTop();
        while(Temp!=NULL){
            TempBlock=BlockStack.getData(Temp);
            if(Block->simpleCuboidCollision(TempBlock->ModelMat)){
                if(Block!=TempBlock){
                    FloatingBlocksQ.removeElement(Block);
                    TimeRev=Block->reduceOverlap(TempBlock,true);
                    Block->resetVelocity();
                    break;
                }
            }
            Temp=BlockStack.getNext(Temp);
        }
    }
}


