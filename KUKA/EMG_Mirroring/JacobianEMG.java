package EMG_Mirroring;

import userAdded.CrossProduction;
import Jama.Matrix;

public class JacobianEMG {
	/**
	 * @param args
	 */
	public  static Matrix JacobianMatrix(int LinkNumber,double[] Teta){
//double[] r={0 ,   0,    0.42  ,  0 ,   0.4 ,   0 ,  0.152};// Considering 7th link length		      
double[] r={0.1525 ,   0.2075,    0.2210  ,  0.1990 ,   0.1795 ,   0.2205 ,   0.0761+0.175};// Considering 7th link length
double[] d={0 ,  0 ,  0,     0,  0 ,   0,   0};
double[] alpha={0 ,  Math.PI/2,  -Math.PI/2 ,-Math.PI/2 , Math.PI/2 , Math.PI/2, -Math.PI/2};
double[][] aa1={{Math.cos(Math.PI), -Math.sin(Math.PI), 0, 0},{Math.sin(Math.PI), Math.cos(Math.PI), 0, 0},{0 ,0 ,1 ,0},{0, 0, 0, 1}};
double[][] aa2={{1, 0, 0, 0},{0, 1, 0, 0},{0, 0, 1, .360},{0, 0, 0, 1}};// Considering robot base + Robot Current frame
Matrix AA1=new Matrix(aa1);
Matrix AA2=new Matrix(aa2);
Matrix A08=AA1.times(AA2);
Matrix P=new Matrix(3,7); //return zero matrix of the size 3*7
Matrix z=new Matrix(3,7);
Matrix Jacobian=new Matrix(6,7);
Matrix zi=new Matrix(3,1);
Matrix z1=new Matrix(3,1);



int res=7-LinkNumber;
if (res!=0){
for(int ii1=0;ii1<res-1;++ii1){
r[LinkNumber+ii1]=0;
}
}
for(int ii=0;ii<7;++ii){
int[] i1={ii};
double[][] a1 = {{1, 0, 0, 0},{0, 1, 0, 0},{0, 0, 1, r[ii]},{0, 0, 0, 1}};
Matrix Tdi=new Matrix(a1);
double[][] a2={{Math.cos(Teta[ii]), -Math.sin(Teta[ii]) ,0 ,0},{Math.sin(Teta[ii]), Math.cos(Teta[ii]), 0 ,0},{0 ,0 ,1, 0},{0, 0, 0, 1}};
Matrix Rti=new Matrix(a2);
double[][] a3={{1, 0, 0, d[ii]},{0, 1, 0, 0},{0, 0, 1 ,0},{0, 0, 0, 1}};
Matrix Tai=new Matrix(a3);
double[][] a4={{1, 0, 0, 0},{0, Math.cos(alpha[ii]), -Math.sin(alpha[ii]), 0},{0, Math.sin(alpha[ii]), Math.cos(alpha[ii]), 0},{0, 0, 0, 1}};
Matrix Rai=new Matrix(a4);

Matrix A1=Rai.times(Tai);
Matrix A2=A1.times(Rti);
Matrix A=A2.times(Tdi); //transform matrix

Matrix A0i=A08.times(A);
A08=A0i; 
Matrix R0=A0i.getMatrix(0,2,0,2);

double[][] r1={{0},{0},{1}};
Matrix R1=new Matrix(r1);
Matrix z0=R0.times(R1);
z.setMatrix(0, 2, i1,z0);

double[][] r2={{0},{0},{0},{1}};
Matrix R2=new Matrix(r2);
Matrix Pp=A0i.times(R2);
P.setMatrix(0, 2, i1,Pp); //position matrix

}
if(LinkNumber==7){
int[] i7={6};

Matrix P_eff=P.getMatrix(0,2,i7); //contact link position 
Matrix P1=new Matrix(3,8);
P1.setMatrix(0, 2, 1, 7, P); //P1 is P with the base position added

for (int jj=0;jj<6;++jj){
int[] i3={jj};

zi=z.getMatrix(0,2,i3);   
z1=P_eff.minus(P1.getMatrix(0, 2,i3));

Matrix cross= CrossProduction.crossProduct(zi,z1); 
Jacobian.setMatrix(0,2,i3,cross);
Jacobian.setMatrix(3,5,i3,zi);
}
}
else if(LinkNumber!=7){
int[] i2={LinkNumber-1};
Matrix P_eff=P.getMatrix(0,2,i2); //contact link position 
Matrix P1=new Matrix(3,8);
P1.setMatrix(0, 2, 1, 7, P); //P1 is P with the base position added

for (int jj=0;jj<LinkNumber;++jj){
int[] i3={jj};

zi=z.getMatrix(0,2,i3);   
z1=P_eff.minus(P1.getMatrix(0, 2,i3));

Matrix cross= CrossProduction.crossProduct(zi,z1); 
Jacobian.setMatrix(0,2,i3,cross);
Jacobian.setMatrix(3,5,i3,zi);
}

}   
return Jacobian;
}
}
