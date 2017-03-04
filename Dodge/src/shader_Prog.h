#ifndef SHADER_PROG_CLASS
#define SHADER_PROG_CLASS


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


#endif //SHADER_PROG_CLASS
