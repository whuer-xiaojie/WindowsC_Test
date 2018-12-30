// ConsoleApplication3.cpp : 定义控制台应用程序的入口点。
//
#include "stdafx.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#define NP 10 //矩阵的维度
/*************************************************
tred2：将对称矩阵通过HouseHolder变换化为3对角矩阵
input：*OriginalsymMatrix 原始对称矩阵
        n 矩阵维数   
output：*d三对角阵对角线元素
        *e三对角矩阵次对角元素 其中e[0]为不确定值，无作用
		*Q 变换矩阵，是N*N数组 
***************************************************/
//void tred2(const double *OriginalsymMatrix, const int n, double *d, double *e, double *Q)
//{
//	int l, k, j, i;
//	//	DP scale, hh, h, g, f;
//	double scale, hh, h, g, f;
//	double **a = &Q;
//	int it = 0;
//	int jt = 0;
//	for (it = 0; it < n; it++)
//	{
//		for (jt = 0; jt < n; jt++)
//		{
//			//	5.0, 4.3, 3.0, 2.0, 1.0, 0.0, -1.0, -2.0, -3.0, -4.0,
//			//4.3, 5.1, 4.0, 3.0, 2.0, 1.0, 0.0, -1.0, -2.0, -3.0,
//			*(*a + it*n + jt) = *(OriginalsymMatrix + it*n + jt);
//		}
//
//	}
//
//	for (i = n - 1; i>0; i--) {
//		l = i - 1;
//		h = scale = 0.0;
//		if (l > 0) {
//			for (k = 0; k<l + 1; k++)
//				scale += fabs(*(*a + i*n + k));
//			if (scale == 0.0)
//				e[i] = *(*a + i*n + l);
//			else {
//				for (k = 0; k<l + 1; k++) {
//					*(*a + i*n + k) /= scale;
//					h += *(*a + i*n + k) **(*a + i*n + k);
//				}
//				f = *(*a + i*n + l);
//				g = (f >= 0.0 ? -sqrt(h) : sqrt(h));
//				e[i] = scale*g;
//				h -= f*g;
//				*(*a + i*n + l) = f - g;
//				f = 0.0;
//				for (j = 0; j<l + 1; j++) {
//					// Next statement can be omitted if eigenvectors not wanted
//					*(*a + j*n + i) = *(*a + i*n + j) / h;
//					g = 0.0;
//					for (k = 0; k<j + 1; k++)
//						g += *(*a + j*n + k) * *(*a + i*n + k);
//					for (k = j + 1; k<l + 1; k++)
//						g += *(*a + k*n + j) * *(*a + i*n + k);
//					e[j] = g / h;
//					f += e[j] * *(*a + i*n + j);
//				}
//				hh = f / (h + h);
//				for (j = 0; j<l + 1; j++) {
//					f = *(*a + i*n + j);
//					e[j] = g = e[j] - hh*f;
//					for (k = 0; k<j + 1; k++)
//						*(*a + j*n + k) -= (f*e[k] + g**(*a + i*n + k));
//				}
//			}
//		}
//		else
//			e[i] = *(*a + i*n + l);
//		d[i] = h;
//	}
//	// Next statement can be omitted if eigenvectors not wanted
//	d[0] = 0.0;
//	e[0] = 0.0;
//	// Contents of this loop can be omitted if eigenvectors not
//	//	wanted except for statement d[i]=a[i][i];
//	for (i = 0; i<n; i++) {
//		l = i;
//		if (d[i] != 0.0) {
//			for (j = 0; j<l; j++) {
//				g = 0.0;
//				for (k = 0; k<l; k++)
//					g += *(*a + i*n + k) * *(*a + k*n + j);
//				for (k = 0; k<l; k++)
//					*(*a + k*n + j) -= g**(*a + k*n + i);
//			}
//		}
//		d[i] = *(*a + i*n + i);
//		*(*a + i*n + i) = 1.0;
//		for (j = 0; j<l; j++) *(*a + j*n + i) = *(*a + i*n + j) = 0.0;
//	}
//}

void tred2(const double *OriginalsymMatrix, const int n, double *d, double *e, double *Q)
{
	int l, k, j, i;
	//	DP scale, hh, h, g, f;
	double scale, hh, h, g, f;
    //double **a = &Q;
	int it = 0;
	int jt = 0;
	for (it = 0; it < n; it++)
	{
		for (jt = 0; jt < n; jt++)
		{
			//	5.0, 4.3, 3.0, 2.0, 1.0, 0.0, -1.0, -2.0, -3.0, -4.0,
			//4.3, 5.1, 4.0, 3.0, 2.0, 1.0, 0.0, -1.0, -2.0, -3.0,
			*(Q + it*n + jt) = *(OriginalsymMatrix + it*n + jt);
		}

	}

	for (i = n - 1; i>0; i--) {
		l = i - 1;
		h = scale = 0.0;
		if (l > 0) {
			for (k = 0; k<l + 1; k++)
				scale += fabs(*(Q+ i*n + k));
			if (scale == 0.0)
				e[i] = *(Q + i*n + l);
			else {
				for (k = 0; k<l + 1; k++) {
					*(Q + i*n + k) /= scale;
					h += *(Q+ i*n + k) **(Q + i*n + k);
				}
				f = *(Q + i*n + l);
				g = (f >= 0.0 ? -sqrt(h) : sqrt(h));
				e[i] = scale*g;
				h -= f*g;
				*(Q + i*n + l) = f - g;
				f = 0.0;
				for (j = 0; j<l + 1; j++) {
					// Next statement can be omitted if eigenvectors not wanted
					*(Q + j*n + i) = *(Q + i*n + j) / h;
					g = 0.0;
					for (k = 0; k<j + 1; k++)
						g += *(Q + j*n + k) * *(Q + i*n + k);
					for (k = j + 1; k<l + 1; k++)
						g += *(Q + k*n + j) * *(Q + i*n + k);
					e[j] = g / h;
					f += e[j] * *(Q + i*n + j);
				}
				hh = f / (h + h);
				for (j = 0; j<l + 1; j++) {
					f = *(Q + i*n + j);
					e[j] = g = e[j] - hh*f;
					for (k = 0; k<j + 1; k++)
						*(Q + j*n + k) -= (f*e[k] + g**(Q + i*n + k));
				}
			}
		}
		else
			e[i] = *(Q + i*n + l);
		d[i] = h;
	}
	// Next statement can be omitted if eigenvectors not wanted
	d[0] = 0.0;
	e[0] = 0.0;
	// Contents of this loop can be omitted if eigenvectors not
	//	wanted except for statement d[i]=a[i][i];
	for (i = 0; i<n; i++) {
		l = i;
		if (d[i] != 0.0) {
			for (j = 0; j<l; j++) {
				g = 0.0;
				for (k = 0; k<l; k++)
					g += *(Q+ i*n + k) * *(Q + k*n + j);
				for (k = 0; k<l; k++)
					*(Q + k*n + j) -= g**(Q + k*n + i);
			}
		}
		d[i] = *(Q + i*n + i);
		*(Q + i*n + i) = 1.0;
		for (j = 0; j<l; j++) *(Q + j*n + i) = *(Q+ i*n + j) = 0.0;
	}
}



/*小函数，计算sign值，用于函数tqli*/
double SIGN(const double a, const double b)
{
	if (b>=0)
	{
		if (a >= 0)
			return a;
		else
		{
			return -a;
		}
	}
	else
	{
		if (a >= 0)
			return -a;
		else
		{
			return a;
		}
	}
	//return b >= 0 ? (a >= 0 ? a : -a) : (a >= 0 ? -a : a);
}
/*用于函数tqli*/
double pythag(const double a, const double b)
{
	double absa, absb;
	absa = fabs(a);
	absb = fabs(b);
	if (absa > absb) return absa*sqrt(1.0 + (absb / absa)*(absb / absa));
	else return (absb == 0.0 ? 0.0 : absb*sqrt(1.0 + (absa / absb)*(absa / absb)));
}

/***********************************************
tqli计算三对角矩阵的特征值
input： *d：三对角矩阵的对角元素
        *e：三对角矩阵的次对角元素，其中e[0]无意义，为不确定值
		*Q：从对角阵到三对角阵的变化矩阵
		n：对角阵的维数
output：*d：特征值
        *Q：Q列向量为对应的特征向量
**************************************************/
void tqli(double *d, double *e, double *Q, const int  n)
{
	int m, l, iter, i, k;
	double s, r, p, g, f, dd, c, b;
	for (i = 1; i<n; i++) e[i - 1] = e[i];
	e[n - 1] = 0.0;
	for (l = 0; l<n; l++) {
		iter = 0;
		do {
			for (m = l; m<n - 1; m++) {
				dd = fabs(d[m]) + fabs(d[m + 1]);
				if (fabs(e[m]) + dd == dd) break;
			}
			if (m != l) {
				if (iter++ == 30) { printf("Too many iterations in tqli"); exit(1); }
				g = (d[l + 1] - d[l]) / (2.0*e[l]);
				r = pythag(g, 1.0);
				g = d[m] - d[l] + e[l] / (g + SIGN(r, g));
				s = c = 1.0;
				p = 0.0;
				for (i = m - 1; i >= l; i--) {
					f = s*e[i];
					b = c*e[i];
					e[i + 1] = (r = pythag(f, g));
					if (r == 0.0) {
						d[i + 1] -= p;
						e[m] = 0.0;
						break;
					}
					s = f / r;
					c = g / r;
					g = d[i + 1] - p;
					r = (d[i] - g)*s + 2.0*c*b;
					d[i + 1] = g + (p = s*r);
					g = c*r - b;
					// Next loop can be omitted if eigenvectors not wanted
					for (k = 0; k<n; k++) {
						f = Q[k*n + i + 1];
						Q[k*n + i + 1] = s*Q[k*n + i] + c*f;
						Q[k*n + i] = c*Q[k*n + i] - s*f;
					}
				}
				if (r == 0.0 && i >= l) continue;
				d[l] -= p;
				e[l] = g;
				e[m] = 0.0;
			}
		} while (m != l);
	}
}

/**********************************************
eigset对特征值从大到小排序
input： *d 矩阵的特征值
        *v 对应的特征向量，按列存储
		n  矩阵的维度

**************************************************/
void eigsrt(double *d, double *v, const int n)
{
	int k, j, i;
	double p;

	for (i = 0; i<n - 1; i++) {
		p = d[k = i];
		for (j = i; j < n; j++)
		if (d[j] >= p) p = d[k = j];
		if (k != i) {
			d[k] = d[i];
			d[i] = p;
			for (j = 0; j < n; j++) {
				p = v[j*n + i];
				v[j*n + i] = v[j*n + k];
				v[j*n + k] = p;
			}
		}
	}
}
/***************************************************
testSym 判断矩阵是不是对称矩阵
****************************************************/
int testSym(const double *Matrix, const int n)
{
	int i, j;
	for ( i = 0; i < n; i++)
	{
		for (j = 0; j < i;j++)
		if (*(Matrix + i*n + j) != *(Matrix + j*n + i))
		{
			printf("not symmetry matrix");
			return 0;
		}
	}
	return 1;
}
/****************************************************
eigsymMatrix:计算实对称矩阵的特征值与特征向量

***************************************************/
int eigsymMatrix(const int n,const double *Matrix,double *eig,double *eigvect)
{
	if (!testSym(Matrix, n))
		return 0;
	//double *d; d = (double*)malloc(n*sizeof(double));
	double *e; e = (double*)malloc(n*sizeof(double));
	//double *Q; Q = (double*)malloc(n*sizeof(double));
	tred2(Matrix, n, eig, e, eigvect);
	tqli(eig, e, eigvect, n);
	eigsrt(eig, eigvect, n);
	free(e);
	return 1;
}

int _tmain(int argc, _TCHAR* argv[])
{
	int i, j, k, l, m;
	//float *d, *e, **a, **f;
	//void tred2(const double *OriginalsymMatrix, const int n, double *d, double *e, double *Q)
	const double c[NP*NP] = {
		5.0, 4.3, 3.0, 2.0, 1.0, 0.0, -1.0, -2.0, -3.0, -4.0,
		4.3, 5.1, 4.0, 3.0, 2.0, 1.0, 0.0, -1.0, -2.0, -3.0,
		3.0, 4.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0, -1.0, -2.0,
		2.0, 3.0, 4.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0, -1.0,
		1.0, 2.0, 3.0, 4.0, 5.0, 4.0, 3.0, 2.0, 1.0, 0.0,
		0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 4.0, 3.0, 2.0, 1.0,
		-1.0, 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 4.0, 3.0, 2.0,
		-2.0, -1.0, 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 4.0, 3.0,
		-3.0, -2.0, -1.0, 0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 4.0,
		-4.0, -3.0, -2.0, -1.0, 0.0, 1.0, 2.0, 3.0, 4.0, 5.0 };
	/*double d[NP ];
	double e[NP ];
	double Q[NP*NP];*/
	double *d; d = (double*)malloc(NP*sizeof(double));
	double *e; e = (double*)malloc(NP*sizeof(double));
	double *Q; Q = (double*)malloc(NP*NP*sizeof(double));
	//eigsymMatrix(NP, c, d, Q);
	tred2(c, NP, d, e, Q);

	printf("diagonal elements\n");
	for (i = 0; i < NP; i++) {
		printf("%12.6f", d[i]);
		if ((i % 5) == 0) printf("\n");
	}
	printf("off-diagonal elements\n");
	for (i = 1; i < NP; i++) {
		printf("%12.6f", e[i]);
		if ((i % 5) == 0) printf("\n");
	}
	double f[NP][NP];
	/* Check transformation matrix */
	for (j = 0; j < NP; j++) {
		for (k = 0; k <NP; k++) {
			f[j][k] = 0.0;
			for (l = 0; l < NP; l++) {
				for (m = 0; m < NP; m++)
					f[j][k] += Q[l*NP + j] * c[l*NP + m] * Q[m*NP + k];
			}
		}
	}
	///* How does it look? */
	//printf("tridiagonal matrix\n");
	//for (i = 0; i < NP; i++) {
	//	for (j = 0; j < NP; j++) printf("%7.2f", f[i][j]);
	//	printf("\n");
	//}
	////void tqli(double *d, double *e, double *Q, const int  n)
	//tqli(d, e, Q, NP);
	//eigsrt(d, Q, NP);
	//输出特征值
	/*for (i = 0; i < NP; i++)
	{
		printf("\n特征值：\n");
		printf("%lf   ", d[i]);
		for (j = 0; j < NP; j++)
		{
			printf("    %lf", Q[i+j*NP]);
		}

	}*/
	free(d);
	free(e);
	free(Q);
	getchar();
    return 0;
}