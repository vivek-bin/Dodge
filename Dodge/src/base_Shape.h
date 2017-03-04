#ifndef BASE_SHAPE_CLASS
#define BASE_SHAPE_CLASS

class base_Shape{
private:
    GLuint Vertices_VboID,Colors_VboID,Normals_VboID,Indices_VboID,TexCoord_VboID,Height_VboID;
    short VertexCount;
    GLuint VaoID;

public:
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

#endif //BASE_SHAPE_CLASS
