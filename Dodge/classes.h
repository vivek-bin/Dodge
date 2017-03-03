#ifndef MY_CLASSES
#define MY_CLASSES

class base_Shape{
private:
    GLuint Vertices_VboID,Colors_VboID,Normals_VboID,Indices_VboID,TexCoord_VboID,Height_VboID;
    short VertexCount;
    GLuint VaoID;

public:
    float ObjCentre[4];

    base_Shape();

    void createVao();

    void createHeightBuffer(GLfloat Data[],GLuint DataSize);

    void createVertexBuffer(GLfloat Data[],GLuint DataSize);

    void createColorBuffer(GLfloat Data[],GLuint DataSize);

    void createTexCoordBuffer(GLfloat Data[],GLuint DataSize);

    void createNormalBuffer(GLfloat Data[],GLuint DataSize);

    void createIndexBuffer(GLushort Data[],GLuint DataSize);

    GLuint getVaoID();

    void drawElements(GLenum mode);

    void destroyVboVao();
};

class shader_Prog{
private:
    GLuint VertexShaderID,FragmentShaderID,ProgramID;
    static GLuint CurrentProg;

public:
    shader_Prog();

    static GLuint getCurrentProg();

    GLuint getProgramID();

    GLuint getVertexShaderID();

    GLuint getFragmentShaderID();

    void createShaders(char const * const VertPrg,char const * const FragPrg);

    void createShaders(const GLuint VertPrg,char const * const FragPrg);

    void createShaders(char const * const VertPrg,const GLuint FragPrg);

    void destroyShaders();

    static void disableProgram();

    void useProgram();

    void useProgram(const char* UName,float Proj[]);

    void useProgram(const char* UName1,float Proj[],const char* UName2,float View[]);

};

class shape_Object{
protected:
    float ObjVelocity[4];
    bool Floating;
public:
    base_Shape *Shape;
    GLuint Texture;
    float ModelMat[16];

    shape_Object();

    shape_Object(const shape_Object& Obj);

    void setTexture(GLuint Tex);

    void setShape(base_Shape* Shp);

    bool isFloating() const;

    void resetVelocity();

    void velUpdate(float const Acc[4],float Time);

    virtual void posUpdate(float Time);

    float velocityMaxComponent();

    bool onFloor();

    bool simpleCuboidCollision (const shape_Object& Piece);

    float reduceOverlap(const shape_Object& Piece,bool Overlap);

};

class player_Class:public shape_Object{
private:
    float Height,Width;
    enum playerPart{Whole,Body,Feet,Head} PlayerPart;
    float Camera[4],Forward[4],Up[4],MouseXAngle,MouseYAngle;
    short Lives;

    void setMatPosition();

public:

    player_Class();

    char getLives();

    void reduceLives(const char FlrN);

    void reduceLives();

    void setPlayerHeightWidth(float H,float W);

    void switchToPlayerHead();

    void switchToPlayerFeet();

    void switchToPlayerBody();

    void switchToPlayer();

    void getViewMat(float ViewMat[]);

    void mouseMove(float X,float Y);

    void rotateAboutCamera(float RotateSpeed,float Time);

    void getForwardVelocity(float TempVec[],float MoveSpeed);

    void jumpPlayer();

    void posUpdate(float Time);

    void forwardPosUpdate(int Direction,float Time);

    float reduceForwardOverlap(int Direction,const shape_Object& Piece,bool Overlap);

};

class control_Interface{
    static bool ArrowKeys[5];
    static int MouseX;
    static int MouseY;

    static void arrowKeyUp(int key,int x,int y);

    static void arrowKeyDown(int key,int x,int y);

    static void otherKeysUp(unsigned char key,int x,int y);

    static void otherKeysDown(unsigned char key,int x,int y);

    static void setMouse(int x,int y);

public:

    static bool jump();

    static int horizontalCtrl();

    static int verticalCtrl();

    static int getMouseX();

    static int getMouseY();

    static void inputSetup();
};

#endif // MY_CLASSES
