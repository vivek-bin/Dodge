//#include "classes.cpp"
//#include <math.h>
//#include "physics.h"
//bool bubbleCollision(shape_Object const * const A,shape_Object const * const B){
// float CCDistSq, Rad1Sq, Rad2Sq;
// const float *AMM, *BMM;
// AMM=A->ModelMat;
// BMM=B->ModelMat;
// CCDistSq=((BMM[12]-AMM[12])*(BMM[12]-AMM[12]))
//         +((BMM[13]-AMM[13])*(BMM[13]-AMM[13]))
//         +((BMM[14]-AMM[14])*(BMM[14]-AMM[14]));
//
//Rad1Sq=(AMM[0]*AMM[0])
//      +(AMM[4]*AMM[4])
//      +(AMM[8]*AMM[8]);
//
//Rad2Sq=(BMM[0]*BMM[0])
//      +(BMM[4]*BMM[4])
//      +(BMM[8]*BMM[8]);
//
//if(CCDistSq>Rad1Sq+Rad2Sq+(2.0f*sqrt(Rad1Sq*Rad2Sq)))
//    return false;
//else
//    return true;
//}
//
//bool cuboidCollision(shape_Object const * const A,shape_Object const * const B){
//const float *N1, *N2, *N3, *T;
//float D1n, D1p, D2n, D2p, D3n, D3p, P[3];
//short i, DttFlag1, DttFlag2, DttFlag3;
//const float *AMM, *BMM;
//AMM=A->ModelMat;
//BMM=B->ModelMat;
//N1=&AMM[0];
//N2=&AMM[4];
//N3=&AMM[8];
//T=&AMM[12];
//
//D1n=(N1[0]*(-N1[0]+T[0]))+(N1[1]*(-N1[1]+T[1]))+(N1[2]*(-N1[2]+T[2]));
//D1p=(N1[0]*(N1[0]+T[0]))+(N1[1]*(N1[1]+T[1]))+(N1[2]*(N1[2]+T[2]));
//
//D2n=(N2[0]*(-N2[0]+T[0]))+(N2[1]*(-N2[1]+T[1]))+(N2[2]*(-N2[2]+T[2]));
//D2p=(N2[0]*(N2[0]+T[0]))+(N2[1]*(N2[1]+T[1]))+(N2[2]*(N2[2]+T[2]));
//
//D3n=(N3[0]*(-N3[0]+T[0]))+(N3[1]*(-N3[1]+T[1]))+(N3[2]*(-N3[2]+T[2]));
//D3p=(N3[0]*(N3[0]+T[0]))+(N3[1]*(N3[1]+T[1]))+(N3[2]*(N3[2]+T[2]));
//
//P[0]=BMM[0]+BMM[4]+BMM[8]+BMM[12];
//P[1]=BMM[1]+BMM[5]+BMM[9]+BMM[13];
//P[2]=BMM[2]+BMM[6]+BMM[10]+BMM[14];
//if((N1[0]*P[0]+N1[1]*P[1]+N1[2]*P[2])>D1n)
//    DttFlag1=1;
//else
//    DttFlag1=-1;
//if((N2[0]*P[0]+N2[1]*P[1]+N2[2]*P[2])>D2n)
//    DttFlag2=1;
//else
//    DttFlag2=-1;
//if((N3[0]*P[0]+N3[1]*P[1]+N3[2]*P[2])>D3n)
//    DttFlag3=1;
//else
//    DttFlag3=-1;
//
//if((N1[0]*P[0]+N1[1]*P[1]+N1[2]*P[2])>D1p)
//    {   if(DttFlag1==-1)DttFlag1=0;}
//else
//    {   if(DttFlag1==1)DttFlag1=0;}
//if((N2[0]*P[0]+N2[1]*P[1]+N2[2]*P[2])>D2p)
//    {   if(DttFlag2==-1)DttFlag2=0;}
//    else
//    {   if(DttFlag2==1)DttFlag2=0;}
//if((N3[0]*P[0]+N3[1]*P[1]+N3[2]*P[2])>D3p)
//    {   if(DttFlag3==-1)DttFlag3=0;}
//    else
//    {   if(DttFlag3==1)DttFlag3=0;}
//for(i=1;i<8;++i){
//    P[0]=((i>>0)&1?-BMM[0]:BMM[0])
//        +((i>>1)&1?-BMM[4]:BMM[4])
//        +((i>>2)&1?-BMM[8]:BMM[8])
//        +((i>>3)&1?-BMM[12]:BMM[12]);
//    P[1]=((i>>0)&1?-BMM[1]:BMM[1])
//        +((i>>1)&1?-BMM[5]:BMM[5])
//        +((i>>2)&1?-BMM[9]:BMM[9])
//        +((i>>3)&1?-BMM[13]:BMM[13]);
//    P[2]=((i>>0)&1?-BMM[2]:BMM[2])
//        +((i>>1)&1?-BMM[6]:BMM[6])
//        +((i>>2)&1?-BMM[10]:BMM[10])
//        +((i>>3)&1?-BMM[14]:BMM[14]);
//
//    if((N1[0]*P[0]+N1[1]*P[1]+N1[2]*P[2])>D1n)
//    {   if(DttFlag1==-1)DttFlag1=0;}
//    else
//    {   if(DttFlag1==1)DttFlag1=0;}
//    if((N1[0]*P[0]+N1[1]*P[1]+N1[2]*P[2])>D1p)
//    {   if(DttFlag1==-1)DttFlag1=0;}
//    else
//    {   if(DttFlag1==1)DttFlag1=0;}
//
//    if((N2[0]*P[0]+N2[1]*P[1]+N2[2]*P[2])>D2n)
//    {   if(DttFlag2==-1)DttFlag2=0;}
//    else
//    {   if(DttFlag2==1)DttFlag2=0;}
//    if((N2[0]*P[0]+N2[1]*P[1]+N2[2]*P[2])>D2p)
//    {   if(DttFlag2==-1)DttFlag2=0;}
//    else
//    {   if(DttFlag2==1)DttFlag2=0;}
//
//    if((N3[0]*P[0]+N3[1]*P[1]+N3[2]*P[2])>D3n)
//    {   if(DttFlag3==-1)DttFlag3=0;}
//    else
//    {   if(DttFlag3==1)DttFlag3=0;}
//    if((N3[0]*P[0]+N3[1]*P[1]+N3[2]*P[2])>D3p)
//    {   if(DttFlag3==-1)DttFlag3=0;}
//    else
//    {   if(DttFlag3==1)DttFlag3=0;}
//}
//if(DttFlag1==0&&DttFlag2==0&&DttFlag3==0)
//    return true;
//else
//    return false;
//}
//
//bool simpleCuboidCollision(shape_Object const * const A,shape_Object const * const B){
//    float AB,ABS;
//    AB=A->ModelMat[14]-B->ModelMat[14];if(AB<0.0f)AB*=-1.0f;
//    ABS=A->ModelMat[10]+B->ModelMat[10];
//    if(AB<ABS){
//        AB=A->ModelMat[13]-B->ModelMat[13];if(AB<0.0f)AB*=-1.0f;
//        ABS=A->ModelMat[5]+B->ModelMat[5];
//        if(AB<ABS){
//            AB=A->ModelMat[12]-B->ModelMat[12];if(AB<0.0f)AB*=-1.0f;
//            ABS=A->ModelMat[0]+B->ModelMat[0];
//            if(AB<ABS){
//                return true;
//            }
//        }
//    }
//    return false;
//}
//
//bool simpleCuboidPointCollision(shape_Object const * const A,float const P[]){
//    float APDiff;
//    APDiff=A->ModelMat[14]-P[2];
//    if(APDiff<0.0f)APDiff*=-1.0f;
//
//    if(APDiff<A->ModelMat[10])
//    {
//        APDiff=A->ModelMat[13]-P[1];
//        if(APDiff<0.0f)APDiff*=-1.0f;
//
//        if(APDiff<A->ModelMat[5])
//        {
//            APDiff=A->ModelMat[12]-P[0];
//            if(APDiff<0.0f)APDiff*=-1.0f;
//
//            if(APDiff<A->ModelMat[0])
//            {   return true;   }
//        }
//    }
//    return false;
//}
//
//
