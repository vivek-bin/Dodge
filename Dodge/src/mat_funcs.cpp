#include "defines.h"
#include <iostream>
#include <math.h>
#include "mat_funcs.h"

void multiplyMatMM(float mat1[],float mat2[],int n,float result[]){
    int i,j,k;
    float temp[16];
    for(i=0;i<n;++i)
        for(j=0;j<n;++j)
            for(k=temp[i*n+j]=0;k<n;++k)
                temp[i*n+j]+=mat1[k*n+j]*mat2[i*n+k];
    for(i=0;i<n*n;i++)
        result[i]=temp[i];
}

void translateMat(float m[],float x,float y,float z){
    m[0]+=m[3]*x; m[4]+=m[7]*x; m[8]+=m[11]*x; m[12]+=m[15]*x;
    m[1]+=m[3]*y; m[5]+=m[7]*y; m[9]+=m[11]*y; m[13]+=m[15]*y;
    m[2]+=m[3]*z; m[6]+=m[7]*z; m[10]+=m[11]*z; m[14]+=m[15]*z;
}

void scaleMat(float m[],float sx,float sy,float sz){
    m[0]*=sx; m[4]*=sx; m[8]*=sx; m[12]*=sx;
    m[1]*=sy; m[5]*=sy; m[9]*=sy; m[13]*=sy;
    m[2]*=sz; m[6]*=sz; m[10]*=sz;m[14]*=sz;
}

void rotateXMatrix(float angle,float m[]){
    float c,s;
    angle=angle*M_PI/180.0f;
    c=cos(angle);
    s=sin(angle);
    if(c<1e-8&&c>-1e-8){
        m[1]=-m[2]*s;
        m[2]= m[1]*s;

        m[5]=-m[6]*s;
        m[6]= m[5]*s;

        m[9]=-m[10]*s;
        m[10]=m[9]*s;

        m[13]=-m[14]*s;
        m[14]= m[13]*s;
    }
    else{
        m[1]=m[1]*c-m[2]*s;
        m[2]=(m[2]+m[1]*s)/c;

        m[5]=m[5]*c-m[6]*s;
        m[6]=(m[6]+m[5]*s)/c;

        m[9]=m[9]*c-m[10]*s;
        m[10]=(m[10]+m[9]*s)/c;

        m[13]=m[13]*c-m[14]*s;
        m[14]=(m[14]+m[13]*s)/c;
    }


    /*
    initMat(mat);
    mat[5]=c;   mat[6]=s;
    mat[9]=-s;mat[10]=c;

    multiplyMatMM(mat,m,4,m);
    */
}

void rotateYMatrix(float angle,float m[]){
    float c,s;
    angle=angle*M_PI/180.0f;
    c=cos(angle);
    s=sin(angle);

    if(c<1e-8&&c>-1e-8){
        m[0]= m[2]*s;
        m[2]=-m[0]*s;

        m[4]= m[6]*s;
        m[6]=-m[4]*s;

        m[8]= m[10]*s;
        m[10]=-m[8]*s;

        m[12]= m[14]*s;
        m[14]=-m[12]*s;
    }
    else{
        m[0]=m[0]*c+m[2]*s;
        m[2]=(m[2]-m[0]*s)/c;

        m[4]=m[4]*c+m[6]*s;
        m[6]=(m[6]-m[4]*s)/c;

        m[8]=m[8]*c+m[10]*s;
        m[10]=(m[10]-m[8]*s)/c;

        m[12]=m[12]*c+m[14]*s;
        m[14]=(m[14]-m[12]*s)/c;
    }

/*
    initMat(mat);
    mat[0]=c; mat[2]=-s;
    mat[8]=s; mat[10]=c;

    multiplyMatMM(mat,m,4,m);
*/
}

void rotateZMatrix(float angle,float m[]){
    float c,s;
    angle=angle*M_PI/180.0f;
    c=cos(angle);
    s=sin(angle);
    if(c<1e-8&&c>-1e-8){
        m[0]=-m[1]*s;
        m[1]= m[0]*s;

        m[4]=-m[5]*s;
        m[5]= m[4]*s;

        m[8]=-m[9]*s;
        m[9]= m[8]*s;

        m[12]=-m[13]*s;
        m[13]= m[12]*s;
    }
    else{
        m[0]=m[0]*c-m[1]*s;
        m[1]=(m[1]+m[0]*s)/c;

        m[4]=m[4]*c-m[5]*s;
        m[5]=(m[5]+m[4]*s)/c;

        m[8]=m[8]*c-m[9]*s;
        m[9]=(m[9]+m[8]*s)/c;

        m[12]=m[12]*c-m[13]*s;
        m[13]=(m[13]+m[12]*s)/c;
    }


/*
    initMat(mat);
    mat[0]=c;   mat[1]=s;
    mat[4]=-s;mat[5]=c;

    multiplyMatMM(mat,m,4,m);
*/
}

void initMat(float mat[]){
    mat[0]=1;  mat[1]=0;  mat[2]=0;  mat[3]=0;
    mat[4]=0;  mat[5]=1;  mat[6]=0;  mat[7]=0;
    mat[8]=0;  mat[9]=0;  mat[10]=1; mat[11]=0;
    mat[12]=0; mat[13]=0; mat[14]=0; mat[15]=1;
}

void multiplyMatMV(float mat[],float v[],int n){
    float temp[4];
    short i;
    for(i=0;i<n;++i)
        temp[i]=mat[0*n+i]*v[0]+mat[1*n+i]*v[1]+mat[2*n+i]*v[2]+mat[3*n+i]*v[3];
    for(i=0;i<n;++i)
        v[i]=temp[i];
}

float normalizeVector(float v[]){
    float mag;
    mag = v[0]*v[0] + v[1]*v[1] + v[2]*v[2];
    if(mag>0.9999999 && mag<1.0000001)
        return mag;
    if(mag<1e-10){
        std::cout<<"zero vector";
        v[0]=v[1]=v[2]=0;
        return 0;
    }
    mag = (float)sqrt(mag);
    v[0]/=mag; v[1]/=mag; v[2]/=mag;
    return mag;
}

void crossProduct(float v1[],float v2[],float result[]){
    float t[3];
    t[0]=v1[1]*v2[2]-v1[2]*v2[1];
    t[1]=v1[2]*v2[0]-v1[0]*v2[2];
    t[2]=v1[0]*v2[1]-v1[1]*v2[0];
    result[0]=t[0];
    result[1]=t[1];
    result[2]=t[2];

    result[3]=v1[3]*v2[3];
}

void distance3D(float v1[],float v2[],float dist[]){
    dist[0]=v2[0]-v1[0];
    dist[1]=v2[1]-v1[1];
    dist[2]=v2[2]-v1[2];
    dist[3]=v2[3]-v1[3];
}

void copyMat(float Src[],float Dest[],int Size){
    for(Size=Size*Size-1;Size>=0;--Size)
        Dest[Size]=Src[Size];
}

void multiplyQuatQQ(float quat1[],float quat2[],float result[]){
    float Temp[4];
    Temp[QUAT_A]=+quat1[QUAT_A]*quat2[QUAT_A]-quat1[QUAT_X]*quat2[QUAT_X]-quat1[QUAT_Y]*quat2[QUAT_Y]-quat1[QUAT_Z]*quat2[QUAT_Z];
    Temp[QUAT_X]=+quat1[QUAT_A]*quat2[QUAT_X]+quat1[QUAT_X]*quat2[QUAT_A]-quat1[QUAT_Y]*quat2[QUAT_Z]+quat1[QUAT_Z]*quat2[QUAT_Y];
    Temp[QUAT_Y]=+quat1[QUAT_A]*quat2[QUAT_Y]+quat1[QUAT_X]*quat2[QUAT_Z]+quat1[QUAT_Y]*quat2[QUAT_A]-quat1[QUAT_Z]*quat2[QUAT_X];
    Temp[QUAT_Z]=+quat1[QUAT_A]*quat2[QUAT_Z]-quat1[QUAT_X]*quat2[QUAT_Y]+quat1[QUAT_Y]*quat2[QUAT_X]+quat1[QUAT_Z]*quat2[QUAT_A];

    result[QUAT_X]=Temp[QUAT_X];
    result[QUAT_Y]=Temp[QUAT_Y];
    result[QUAT_Z]=Temp[QUAT_Z];
    result[QUAT_A]=Temp[QUAT_A];
}

void rotateMatrixByQuat(float Quat[],float Mat[]){
    float Mag;
    Mat[0]=Quat[QUAT_X]*Quat[QUAT_X]+Quat[QUAT_A]*Quat[QUAT_A]
          -Quat[QUAT_Y]*Quat[QUAT_Y]-Quat[QUAT_Z]*Quat[QUAT_Z];
    Mat[1]=  2*(Quat[QUAT_X]*Quat[QUAT_Y]+Quat[QUAT_A]*Quat[QUAT_Z]);
    Mat[2]=  2*(Quat[QUAT_X]*Quat[QUAT_Z]-Quat[QUAT_A]*Quat[QUAT_Y]);

    Mat[4]=  2*(Quat[QUAT_X]*Quat[QUAT_Y]-Quat[QUAT_A]*Quat[QUAT_Z]);
    Mat[5]=Quat[QUAT_Y]*Quat[QUAT_Y]+Quat[QUAT_A]*Quat[QUAT_A]
          -Quat[QUAT_X]*Quat[QUAT_X]-Quat[QUAT_Z]*Quat[QUAT_Z];
    Mat[6]=  2*(Quat[QUAT_Y]*Quat[QUAT_Z]+Quat[QUAT_A]*Quat[QUAT_X]);

    Mat[8]=  2*(Quat[QUAT_X]*Quat[QUAT_Z]+Quat[QUAT_A]*Quat[QUAT_Y]);
    Mat[9]=  2*(Quat[QUAT_Y]*Quat[QUAT_Z]-Quat[QUAT_A]*Quat[QUAT_X]);
    Mat[10]=Quat[QUAT_Z]*Quat[QUAT_Z]+Quat[QUAT_A]*Quat[QUAT_A]
           -Quat[QUAT_X]*Quat[QUAT_X]-Quat[QUAT_Y]*Quat[QUAT_Y];

    Mag=1/(Quat[QUAT_A]*Quat[QUAT_A]+Quat[QUAT_X]*Quat[QUAT_X]
          +Quat[QUAT_Y]*Quat[QUAT_Y]+Quat[QUAT_Z]*Quat[QUAT_Z]);
    Mat[0]*=Mag;Mat[4]*=Mag;Mat[8]*=Mag;
    Mat[1]*=Mag;Mat[5]*=Mag;Mat[9]*=Mag;
    Mat[2]*=Mag;Mat[6]*=Mag;Mat[10]*=Mag;

    Mat[3]=Mat[7]=Mat[11]=Mat[12]=Mat[13]=Mat[14]=0;
    Mat[15]=1;
}

void createQuat(float Axis[],float Angle,float Quat[]){
    Angle/=2;
    Quat[QUAT_X]=Axis[0]*sin(Angle);
    Quat[QUAT_Y]=Axis[1]*sin(Angle);
    Quat[QUAT_Z]=Axis[2]*sin(Angle);
    Quat[QUAT_A]=cos(Angle);
}

