#include "defines.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GL/freeglut.h>
#include <stdio.h>
#include "shader_Prog.h"


shader_Prog::shader_Prog():VertexShaderID(),FragmentShaderID(),ProgramID(){

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

