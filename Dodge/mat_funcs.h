#ifndef MAT_FUNCS_INCLUDE
#define MAT_FUNCS_INCLUDE
void multiplyMatMM(float mat1[],float mat2[],int n,float result[]);
void translateMat(float m[],float x,float y,float z);
void rotateXMatrix(float angle,float m[]);
void rotateYMatrix(float angle,float m[]);
void rotateZMatrix(float angle,float m[]);
void initMat(float mat[]);
void multiplyMatMV(float mat[],float v[],int n);
void scaleMat(float m[],float sx,float sy,float sz);
float normalizeVector(float v[]);
void crossProduct(float v1[],float v2[],float fin[]);
void distance3D(float v1[],float v2[],float dist[]);
void copyMat(float Src[],float Dest[],int Size);
void multiplyQuatQQ(float quat1[],float quat2[],float result[]);
void rotateMatrixByQuat(float Quat[],float Mat[]);
void createQuat(float Axis[],float Angle,float Quat[]);
#endif // MAT_FUNCS_INCLUDE
