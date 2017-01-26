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
#include <list>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "mat_funcs.h"
#include "classes.cpp"

GLuint shader_Prog::CurrentProg;
bool control_Interface::ArrowKeys[];
int  control_Interface::MouseX;
int  control_Interface::MouseY;

float const Gravity[4]={0.0f, -9.0f, 0.0f, 0.0f};

int CurrentWidth = 1366, CurrentHeight = 705, WindowHandle = 0, FrameCount = 0;

GLuint Textures[NO_OF_TEXTURES];

base_Shape Box;

shader_Prog TexShader, SimpleTexShader;

char Heights[FLOOR_SIZE][FLOOR_SIZE];

int FloorNo, NumFloatingBlocks;

player_Class Player1;

shape_Object SceneryCube;

std::vector<shape_Object> BlockStack;

typedef std::vector<shape_Object>::iterator block_Iterator;
typedef std::vector<shape_Object>::const_iterator const_Block_Iterator;
typedef std::vector<shape_Object>::reverse_iterator reverse_Block_Iterator;
typedef std::vector<shape_Object>::const_reverse_iterator const_Reverse_Block_Iterator;

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
void timerFunction(int );
void idleFunction();
void renderFunction();
void resizeFunction(int ,int );
void cleanUp();
void createProjMat(float[],float,float,float,float);
GLuint loadTexture(const char *,const GLuint,const GLuint,const GLuint);
void createBox();
void nextBlock();
void moveBlock(block_Iterator&);
void movePlayer();
void initGame();
void checkInput();

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
    if(WindowHandle < 1){
        fprintf(stderr, "UNABLE TO CREATE WINDOW");
        exit(-1);
    }
    glEnable(GL_DEPTH_TEST);

    control_Interface::inputSetup();

    glutIdleFunc(idleFunction);
    glutDisplayFunc(renderFunction);
    glutReshapeFunc(resizeFunction);
    glutCloseFunc(cleanUp);
    glutTimerFunc(0,timerFunction,0);
}

void checkInput(){
    int ForwardDirection,SideDirection;

    ForwardDirection=control_Interface::verticalCtrl();
    SideDirection=control_Interface::horizontalCtrl();

    Player1.switchToPlayerBody();
    Player1.rotateAboutCamera(SideDirection,UNIT_TIME);
    if(ForwardDirection!=0){
        Player1.forwardPosUpdate(ForwardDirection,UNIT_TIME);
        for(const_Reverse_Block_Iterator Block=BlockStack.rbegin();Block!=BlockStack.rend();++Block){
            if(Player1.simpleCuboidCollision(*Block)){
                Player1.reduceForwardOverlap(ForwardDirection,*Block,false);
                break;
            }
        }
    }
    if(control_Interface::jump()){
        Player1.jumpPlayer();
    }
    Player1.mouseMove((float)control_Interface::getMouseX()/CurrentWidth,
                      (float)control_Interface::getMouseY()/CurrentHeight);
}

void idleFunction(){
//    glutPostRedisplay();
}

void renderFunction(){
    float ShadowData[5*MAX_FLOATING_BLOCKS];
    GLuint MMUniformLocation;
    GLfloat ViewMat[16], ProjMat[16];

    glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);

    Player1.getViewMat(ViewMat);
    createProjMat(ProjMat,MAX_OVERLAP,100.0f,67.0f,(float)CurrentWidth/CurrentHeight);

    TexShader.useProgram("P",ProjMat);
    glUniformMatrix4fv(glGetUniformLocation(shader_Prog::getCurrentProg(),"V")
                       ,1,GL_FALSE,ViewMat);
    int i;
    for(i=0;i<MAX_FLOATING_BLOCKS*5; ++i){
        ShadowData[i]=0;
    }
    i=0;
    for(const_Reverse_Block_Iterator FloatingBlock=BlockStack.rbegin();FloatingBlock!=BlockStack.rend();++FloatingBlock){
        if(i>=NumFloatingBlocks)break;
        if(!FloatingBlock->isFloating())
            continue;
        ShadowData[0+i*5]=FloatingBlock->ModelMat[0];
        ShadowData[1+i*5]=FloatingBlock->ModelMat[10];
        ShadowData[2+i*5]=FloatingBlock->ModelMat[12];
        ShadowData[3+i*5]=FloatingBlock->ModelMat[13]-FloatingBlock->ModelMat[5];
        ShadowData[4+i*5]=FloatingBlock->ModelMat[14];
        ++i;
    }

    glUniform1fv(glGetUniformLocation(shader_Prog::getCurrentProg(),"shadow_coords")
                 ,5*MAX_FLOATING_BLOCKS,ShadowData);

    MMUniformLocation=glGetUniformLocation(shader_Prog::getCurrentProg(),"M");
    for(const_Reverse_Block_Iterator Block=BlockStack.rbegin();Block!=BlockStack.rend();++Block){
        glBindTexture(GL_TEXTURE_2D,Block->Texture);
        glUniformMatrix4fv(MMUniformLocation,1,GL_FALSE,Block->ModelMat);
        (Block->Shape)->drawElements(GL_TRIANGLES);
    }

    SimpleTexShader.useProgram("P",ProjMat);
    glUniformMatrix4fv(glGetUniformLocation(shader_Prog::getCurrentProg(),"V"),
                       1,GL_FALSE,ViewMat);

    glBindTexture(GL_TEXTURE_2D,SceneryCube.Texture);
    glUniformMatrix4fv(glGetUniformLocation(shader_Prog::getCurrentProg(),"M"),
                       1,GL_FALSE,SceneryCube.ModelMat);
    (SceneryCube.Shape)->drawElements(GL_TRIANGLES);

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
    if(rand()%NEW_BLOCK_FREQUENCY==0){
        if(NumFloatingBlocks<MAX_FLOATING_BLOCKS && FloorNo<MAX_FLOOR){
            nextBlock();
        }
    }
    checkInput();
    movePlayer();
    for(block_Iterator Block=BlockStack.begin();Block!=BlockStack.end();++Block){
        if(!Block->isFloating())
            continue;
        moveBlock(Block);
    }
    glutTimerFunc(UNIT_TIME*1000,timerFunction,1);
    FrameCount=0;
    glutPostRedisplay();
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
    };

    Box.createVao();
    Box.createVertexBuffer(Vertices,sizeof(Vertices));
    Box.createNormalBuffer(Normals,sizeof(Normals));
    Box.createTexCoordBuffer(TexCoords,sizeof(TexCoords));
    Box.createIndexBuffer(Indices,sizeof(Indices));
    glBindVertexArray(0);
}

void nextBlock(){
    int BlockPos,BlocksLeft;
    int x,y,xn,xp,yn,yp;

    //get block position
    do{
        BlocksLeft=0;
        for(int i=0;i<FLOOR_SIZE;++i)
            for(int j=0;j<FLOOR_SIZE;++j)
                if(Heights[i][j]<FloorNo*HEIGHT_DIVISIONS)
                    ++BlocksLeft;

        if(BlocksLeft==0)++FloorNo;
        if(FloorNo>MAX_FLOOR)return;
    }while(BlocksLeft==0);

    BlockPos=rand()%BlocksLeft;
    for(x=0; x<FLOOR_SIZE ;++x){
        for(y=0; y<FLOOR_SIZE ;++y){
            if(Heights[x][y]<FloorNo*HEIGHT_DIVISIONS)
                --BlockPos;
            if(BlockPos<0)break;
        }
        if(BlockPos<0)break;
    }

    //get block size
    int Width,Length;
    Width=(rand()%MAX_BLOCK_SIZE)+1;
    Length=(rand()%MAX_BLOCK_SIZE)+1;

    xn=xp=yn=yp=1;
    while((x-xn>=0) && (xn<Width) && (Heights[x-xn][y]<FloorNo*HEIGHT_DIVISIONS)){
        ++xn;
    }
    while((x+xp<FLOOR_SIZE) && (xp<Width) && (Heights[x+xp][y]<FloorNo*HEIGHT_DIVISIONS)){
        ++xp;
    }
    while((y-yn>=0) && (yn<Length) && (Heights[x][y-yn]<FloorNo*HEIGHT_DIVISIONS)){
        ++yn;
    }
    while((y+yp<FLOOR_SIZE) && (yp<Length) && (Heights[x][y+yp]<FloorNo*HEIGHT_DIVISIONS)){
        ++yp;
    }

    bool flag;
    do{
        flag=false;
        for(int i=x-xn+1;i<x+xp && !flag;++i){
            for(int j=y-yn+1;j<y+yp && !flag;++j){
                if(Heights[i][j]>=FloorNo*HEIGHT_DIVISIONS){
                    flag=true;
                    if(abs(i-x)>abs(j-y)){
                        if(i>x)xp=abs(i-x);
                        else   xn=abs(i-x);
                    }
                    else{
                        if(j>y)yp=abs(j-y);
                        else   yn=abs(j-y);
                    }
                }
            }
        }
    }while(flag);

    while(xn+xp-1>Width){
        if(xn>xp)--xn;
        else     --xp;
    }

    while(yn+yp-1>Length){
        if(yn>yp)--yn;
        else     --yp;
    }

    //generate random height
    int Height=(rand()%MAX_BLOCK_HEIGHT)+1;

    int MaxHeight=0;
    for(int i=x-xn+1;i<x+xp;++i){
        for(int j=y-yn+1;j<y+yp;++j){
            if(Heights[i][j]>MaxHeight)
                MaxHeight=Heights[i][j];
        }
    }
    for(int i=x-xn+1;i<x+xp;++i){
        for(int j=y-yn+1;j<y+yp;++j){
            Heights[i][j]=Height+MaxHeight;
        }
    }

    //create the block
    shape_Object Next;
    Next.Shape=&Box;
    Next.Texture=Textures[(rand()%NO_OF_TEXTURES)];
    scaleMat(Next.ModelMat,UNIT_DISTANCE*0.5f*(xn+xp-1)*0.999f,
                           UNIT_DISTANCE*0.5f*(Height/HEIGHT_DIVISIONS),
                           UNIT_DISTANCE*0.5f*(yn+yp-1)*0.999f);
    translateMat(Next.ModelMat,UNIT_DISTANCE*(x-(FLOOR_SIZE-1)*0.5f+(xp-xn)*0.5f),
                               UNIT_DISTANCE*(MAX_FLOOR+FloorNo)*2,
                               UNIT_DISTANCE*(y-(FLOOR_SIZE-1)*0.5f+(yp-yn)*0.5f));
    ++NumFloatingBlocks;
    BlockStack.push_back(Next);
}

void initGame(){
    FloorNo=1;
    NumFloatingBlocks=0;

    for(int i=0;i<FLOOR_SIZE;++i)
        for(int j=0;j<FLOOR_SIZE;++j)
            Heights[i][j]=0.0f;

    TexShader.createShaders(TexVertShader,TexFragShader);
    SimpleTexShader.createShaders(SimpleTexVertShader,SimpleTexFragShader);

    createBox();
    Textures[0]=loadTexture("./images/block0.bmp",GL_REPEAT,GL_REPEAT,GL_LINEAR);
    Textures[1]=loadTexture("./images/block1.bmp",GL_REPEAT,GL_REPEAT,GL_LINEAR);
    Textures[2]=loadTexture("./images/block2.bmp",GL_REPEAT,GL_REPEAT,GL_LINEAR);
    Textures[3]=loadTexture("./images/block3.bmp",GL_REPEAT,GL_REPEAT,GL_LINEAR);

    SceneryCube.Shape=&Box;
    scaleMat(SceneryCube.ModelMat,SCENE_CUBE_SIZE,SCENE_CUBE_SIZE*0.5f,SCENE_CUBE_SIZE);
    translateMat(SceneryCube.ModelMat,0,SCENE_CUBE_SIZE*0.5f,0);
    SceneryCube.Texture=loadTexture("./images/scenery.bmp",GL_CLAMP_TO_EDGE,GL_CLAMP_TO_EDGE,GL_LINEAR);

    float WallW=0.02f;
    float WallH=MAX_FLOOR*UNIT_DISTANCE;
    float WallL=FLOOR_SIZE*UNIT_DISTANCE*0.5;
    shape_Object Wall;

    Wall.Texture=loadTexture("./images/walls.bmp",GL_REPEAT,GL_REPEAT,GL_LINEAR);
    Wall.Shape=&Box;
    Wall.resetVelocity();
    initMat(Wall.ModelMat);
    scaleMat(Wall.ModelMat,WallW,WallH,WallL);
    translateMat(Wall.ModelMat,-(WallW+WallL),WallH, 0.0f);
    BlockStack.push_back(Wall);

    initMat(Wall.ModelMat);
    scaleMat(Wall.ModelMat,WallW,WallH,WallL);
    translateMat(Wall.ModelMat, (WallW+WallL),WallH, 0.0f);
    BlockStack.push_back(Wall);

    initMat(Wall.ModelMat);
    scaleMat(Wall.ModelMat,WallL,WallH,WallW);
    translateMat(Wall.ModelMat, 0.0f,WallH,-(WallW+WallL));
    BlockStack.push_back(Wall);

    initMat(Wall.ModelMat);
    scaleMat(Wall.ModelMat,WallL,WallH,WallW);
    translateMat(Wall.ModelMat, 0.0f,WallH, (WallW+WallL));
    BlockStack.push_back(Wall);

    shape_Object Floor;
    Floor.Shape=&Box;
    Floor.Texture=loadTexture("./images/floor.bmp",GL_REPEAT,GL_REPEAT,GL_LINEAR);
    Floor.resetVelocity();
    scaleMat(Floor.ModelMat,FLOOR_SIZE*UNIT_DISTANCE*0.5,0.5f,FLOOR_SIZE*UNIT_DISTANCE*0.5);
    translateMat(Floor.ModelMat,0,-0.25f,0);
    BlockStack.push_back(Floor);

    Player1.setPlayerHeightWidth(PLAYER_HEIGHT,PLAYER_WIDTH);
    Player1.Texture=0;
    Player1.setShape(&Box);
}

void movePlayer(){
    bool VelResetFlag=false;

    Player1.posUpdate(UNIT_TIME);

    for(const_Block_Iterator Block=BlockStack.begin();Block!=BlockStack.end();++Block){
        Player1.switchToPlayer();
        if(Player1.simpleCuboidCollision(*Block)){
            float TimeRev;
            if(Player1.velocityMaxComponent()>1e-10)
                TimeRev=Player1.reduceOverlap(*Block,true);
            Player1.switchToPlayerFeet();
            if(Player1.simpleCuboidCollision(*Block)){
                if(Player1.velocityMaxComponent()>=MAX_VELOCITY*LETHAL_VELOCITY_MULTIPLIER)
                    Player1.reduceLives();
                VelResetFlag=true;
                break;
            }
            Player1.switchToPlayerHead();
            if(Player1.simpleCuboidCollision(*Block)){
                VelResetFlag=true;
                break;
            }
            Player1.posUpdate(TimeRev);
        }
    }
    if(VelResetFlag)
        Player1.resetVelocity();
    else
        Player1.velUpdate(Gravity,UNIT_TIME);
}

void moveBlock(block_Iterator& FloatingBlock){
    FloatingBlock->velUpdate(Gravity,UNIT_TIME);
    FloatingBlock->posUpdate(UNIT_TIME);

    Player1.switchToPlayer();
    if(FloatingBlock->simpleCuboidCollision((shape_Object)Player1)){
        float TimeRev=FloatingBlock->reduceOverlap((shape_Object)Player1,true);
        Player1.switchToPlayerHead();
        if(FloatingBlock->simpleCuboidCollision((shape_Object)Player1)){
            Player1.reduceLives(FloorNo);
        }
        FloatingBlock->posUpdate(TimeRev);
    }

    for(const_Block_Iterator Block=BlockStack.begin();Block!=BlockStack.end();++Block){
        if(Block==FloatingBlock)
            continue;
        if(FloatingBlock->simpleCuboidCollision(*Block)){
            --NumFloatingBlocks;
            FloatingBlock->reduceOverlap(*Block,true);
            FloatingBlock->resetVelocity();
            break;
        }
    }
}

