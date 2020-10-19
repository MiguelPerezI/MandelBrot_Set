#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <stdbool.h>
#include "Matrix.h"
#include "Vector3D.h"

int digitCounter(int n) {
    int count = 0;
    if (n == 0) {return 1;}
    while (n != 0) {
          n /= 10; 
          ++count;
    }
    
return count;
}

void space(int n) {
     int i;
     for (i = 0; i < n; i++) 
         printf(" ");
}

MATRIZ creaEspacioMatriz(int m, int n)
{
  MATRIZ ret = {m:m, n:n, A:NULL};
  int i;

  ret.A = (double**) malloc (m * sizeof(double*));
  if (ret.A != NULL){
    for(i = 0; i < m; i++){
      ret.A[i] = (double *) malloc (n * sizeof(double));
      if (ret.A[i] == NULL){
        while(i > 0){
          free(ret.A[--i]);
          ret.A[i] = NULL;
        }
        break;
      }
    }
  }

  return ret;
}

MatrixV creaEspacioMatrixV(int m, int n) {
  MatrixV ret = {
                  m : m,
                  n : n, 
                  A : NULL
                };
  int i;
  ret.A = (Vector3D**) malloc (m * sizeof(Vector3D*));
  if (ret.A != NULL){
    for(i = 0; i < m; i++){
      ret.A[i] = (Vector3D *) malloc (n * sizeof(Vector3D));
      if (ret.A[i] == NULL){
        while(i > 0){
          free(ret.A[--i]);
          ret.A[i] = NULL;
        }
        break;
      }
    }
  }

  return ret;
}

int leeMatriz(MATRIZ *mat)
{
  int m, n, i, j;

  if (mat == NULL) return -1; /*codigo de error*/
  printf("Digite numero de renglones m:");
  scanf("%d", &m);
  printf("Digite numero de columnas n:");
  scanf("%d", &n);
  *mat = creaEspacioMatriz(m, n);
  printf("digite las entradas de la matriz:\n");
  for(i = 0; i < m; i++)
    for (j = 0; j < n; j++){
      printf("[%2d, %2d]=", i, j);
      scanf("%lg", &(mat->A[i][j]));
    }

  return 0; /*codigo de terminacion exita*/
}

int escMatriz(MATRIZ mat)
{
  int i, j;

  for (j = 0; j < mat.n; j++){
    printf("\n");
    for (i = 0; i < mat.m; i++){
      printf("%6lg ", mat.A[i][j]);
    }
    printf("\n");
  }
  printf("\n");

  return 0;
}

int liberaEspacioMatriz(MATRIZ * mat)
{
  int i;

  for (i=0; i<mat->m; i++){
    free(mat->A[i]);
    mat->A[i]=NULL;
  }
  free(mat->A);
  mat->A=NULL;
  mat->m = mat->n = 0;

  return 0;
}

MATRIZ tableZeroReal(MATRIZ Table1, int n) {
      int i;
      int j;
    
      Table1 = creaEspacioMatriz(n, n);
               for (i = 0; i < n; i++)
                   for (j = 0; j < n; j++) 
                       Table1.A[i][j] = 0;
      return Table1;
}

struct rotationMats initRotation(double theta) {

  struct rotationMats U = {theta : theta};

  MATRIZ mat;
  mat = tableZeroReal(mat, 3);
  mat.A[0][0] = cos(theta);  mat.A[0][1] =-sin(theta); mat.A[0][2] = 0;
  mat.A[1][0] = sin(theta);  mat.A[1][1] = cos(theta); mat.A[1][2] = 0;
  mat.A[2][0] = 0;           mat.A[2][1] = 0;          mat.A[2][2] = 1;

  U.aX = initVector3D(mat.A[0][0], mat.A[1][0], mat.A[2][0]);
  U.aY = initVector3D(mat.A[0][1], mat.A[1][1], mat.A[2][1]);
  U.aZ = initVector3D(mat.A[0][2], mat.A[1][2], mat.A[2][2]);

  U.aux = initVector3D(0, 0, 0);

  U.rotZ = mat;

  return U;
}

void updateRotation(struct rotationMats * U, double theta) {
  U->theta = theta;

  U->rotZ.A[0][0] = cos(theta);  U->rotZ.A[0][1] =-sin(theta); U->rotZ.A[0][2] = 0;
  U->rotZ.A[1][0] = sin(theta);  U->rotZ.A[1][1] = cos(theta); U->rotZ.A[1][2] = 0;
  U->rotZ.A[2][0] = 0;           U->rotZ.A[2][1] = 0;          U->rotZ.A[2][2] = 1;

  U->aX.x = cos(theta); U->aX.y =-sin(theta); U->aX.z = 0;
  U->aY.x = sin(theta); U->aY.y = cos(theta); U->aY.z = 0;
  U->aZ.x =          0; U->aZ.y =          0; U->aZ.z = 1;
}

Vector3D matMult3D(MATRIZ mat, Vector3D  v) {
  Vector3D v0 = initVector3D(mat.A[0][0], mat.A[1][0], mat.A[2][0]);
  Vector3D v1 = initVector3D(mat.A[0][1], mat.A[1][1], mat.A[2][1]);
  Vector3D v2 = initVector3D(mat.A[0][2], mat.A[1][2], mat.A[2][2]);

  v0 = scale(v.x, v0);
  v1 = scale(v.y, v1);
  v2 = scale(v.z, v2);

  Vector3D ret = add(v0, v1);
  ret = add(ret, v2);
  return ret; 
}

void rot3D(struct rotationMats * U, Vector3D v) {

  U->aux.x = (v.x * U->aX.x) + (v.y * U->aX.y) + (v.z * U->aX.z);
  U->aux.y = (v.x * U->aY.x) + (v.y * U->aY.y) + (v.z * U->aY.z);
  U->aux.z = (v.x * U->aZ.x) + (v.y * U->aZ.y) + (v.z * U->aZ.z);

}