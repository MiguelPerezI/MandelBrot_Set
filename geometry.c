#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include <stdbool.h>
#include <stdarg.h>
#include "Vector3D.h"
#include "Matrix.h"
#include "geometry.h"

struct Vector3DList startVectorSpace(int n) {
	struct Vector3DList ret = {n:n, v:NULL};
	ret.v = (Vector3D *) malloc (n * sizeof(Vector3D));

	return ret;
}

struct Vector3DList VectorSpace(int count, ... ) {
	va_list list;
	struct Vector3DList retList = startVectorSpace(count);

	int i;
	va_start(list, count);

	for (i = 0; i < count; i++) {
		retList.v[i] = va_arg(list, Vector3D);
	}

	va_end(list);

	return retList;
}

void printVectorList(struct Vector3DList lista) {
	int i;
	printf("\n");
	if (lista.v != NULL) {
		for (i = 0; i < lista.n; i++) {
			printf("%d.- ", i);
			writeVector3D(lista.v[i]);
			printf("\n");
		}
	} else {
		printf("\n\n---- List Empty ----\n\n");
	}
}

///////////////////////
//					 //
//     ND FACETS     //
//					 //   
///////////////////////

struct Simplex startSimplex(int dim, struct Vector3DList lista) {

	MATRIZ adjacent;
	adjacent = tableZeroReal(adjacent, dim + 1);
	struct Simplex ret = {dim:dim, vertexSpace:lista, Adj:adjacent};

	return ret;
}

struct Simplex SimplexND(int dim_plus_one, ... ) {
	va_list list;
	struct Vector3DList retList = startVectorSpace(dim_plus_one);

	int i;
	va_start(list, dim_plus_one);

	for (i = 0; i < dim_plus_one; i++) {
		retList.v[i] = va_arg(list, Vector3D);
	}

	va_end(list);

	return startSimplex(retList.n - 1, retList);
}

struct Simplex facet(Vector3D a, Vector3D b, Vector3D c) {
	struct Vector3DList retList = startVectorSpace(3);
	retList.v[0] = a;
	retList.v[1] = b;
	retList.v[2] = c;
	struct Simplex ret = startSimplex(3, retList);

	Vector3D normal = cruz(sub(retList.v[1], retList.v[0]), sub(retList.v[2], retList.v[0]));
  	normal = unit(normal);

  	ret.normal = normal;

  	return ret;
}

void updateFacet(struct Simplex * Facet, Vector3D new_a, Vector3D new_b, Vector3D new_c) {
	
	updateVector3D(&Facet->vertexSpace.v[0], new_a);
	updateVector3D(&Facet->vertexSpace.v[1], new_b);
	updateVector3D(&Facet->vertexSpace.v[2], new_c);

	updateVector3D(&Facet->normal, unit(cruz(sub(Facet->vertexSpace.v[1], Facet->vertexSpace.v[0]), sub(Facet->vertexSpace.v[2], Facet->vertexSpace.v[0]))) );
}

struct Simplex dilateSimplex(struct Simplex mini, Vector3D auxPoint, double d) {

	Vector3D aux0 = dilate(mini.vertexSpace.v[0], auxPoint, d);
	Vector3D aux1 = dilate(mini.vertexSpace.v[1], auxPoint, d);
	Vector3D aux2 = dilate(mini.vertexSpace.v[2], auxPoint, d);

	return facet(aux0, aux1, aux2);	
}

void printSimplexInfo(struct Simplex sim) {
	printf("\n\n-- We have defined the following simplex:");
	printf("-- Simplex dimension: %d\n", sim.dim);
	printf("-- Simplex vertex space:");
	printVectorList(sim.vertexSpace);
	printf("-- Simplex adjecency matrix: \n");
	escMatriz(sim.Adj);
}

void renderFacet(struct Simplex sim, struct rotationMats U, int mod) {

	int i;
	for (i = 0; i < 20; i++)
		if (mod%180 == i) glColor3ub(200 - (10 * i), 200 - (10 * i), 200 - (10 * i));//Blanco y baja a negro

	for (i = 0; i < 20; i++)
		if (mod%180 == i + 20) glColor3ub(10 * i, 0, 10 * i);//Negro y sube a purpura
	
	for (i = 0; i < 20; i++)
		if (mod%180 == i + 40) glColor3ub(200 - (10 * i), 0, 200 - (10 * i));//Purpura y baja a negro

	for (i = 0; i < 20; i++)
		if (mod%180 == i + 60) glColor3ub(10 * i, 10 * i, 0);//Oscuro y sube a amarillo

	for (i = 0; i < 20; i++)
		if (mod%180 == i + 80) glColor3ub(200 - (10 * i), 200 - (10 * i), 200 - (10 * i));//Amarillo y baja a negro

	for (i = 0; i < 20; i++)
		if (mod%180 == i + 100) glColor3ub(0, 0, 10 * i);//Negro y sube a Azul

	for (i = 0; i < 20; i++)
		if (mod%180 == i + 120) glColor3ub(200 - (10 * i), 0, 200 - (10 * i));//Azul y baja a negro

	for (i = 0; i < 20; i++)
		if (mod%180 == i + 140) glColor3ub(10 * i, 0, 0);//Negro y sube a rojo

	for (i = 0; i < 20; i++)
		if (mod%180 == i + 160) glColor3ub(200 - (10 * i), 0, 0);//Rojo y baja a negro

	for (i = 0; i < 20; i++)
		if (mod%180 == i + 160) glColor3ub(10 * i, 10 * i, 10 * i);//Negro y sube a blanco


    rot3D(&U, sim.normal);
  	glBegin(GL_TRIANGLES);
	glNormal3f( U.aux.x, U.aux.y, U.aux.z);
	rot3D(&U, sim.vertexSpace.v[0]);
	glVertex3f( U.aux.x, U.aux.y, U.aux.z);
	rot3D(&U, sim.vertexSpace.v[1]);
	glVertex3f( U.aux.x, U.aux.y, U.aux.z);
	rot3D(&U, sim.vertexSpace.v[2]);
	glVertex3f( U.aux.x, U.aux.y, U.aux.z);
	glEnd();

}

///////////////////////
//					 //
//    MATRIX MESH    //
//					 //   
///////////////////////


MatrixMesh creaMatrixMesh(int m, int n) {
  MatrixMesh ret = {
                  m : m,
                  n : n, 
                  A : NULL
                };
  int i;
  ret.A = (struct Simplex**) malloc (m * sizeof(struct Simplex*));
  if (ret.A != NULL){
    for(i = 0; i < m; i++){
      ret.A[i] = (struct Simplex *) malloc (n * sizeof(struct Simplex));
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

void printMatrixMesh(MatrixMesh mesh) {
	
	int i, j;
	for (i = 0; j < mesh.m; i++) {
		for (j = 0; j < mesh.n; j++) {
			printf("HELLLLO\n");
			printSimplexInfo(mesh.A[i][j]);
			printf("\n\n");
		}
	}
}

MatrixSimplexRing creaMatrixSimplexRing(int m, int n) {
  MatrixSimplexRing ret = {
                  m : m,
                  n : n, 
                  A : NULL
                };
  int i;
  ret.A = (struct SimplexRing**) malloc (m * sizeof(struct SimplexRing*));
  if (ret.A != NULL){
    for(i = 0; i < m; i++){
      ret.A[i] = (struct SimplexRing *) malloc (n * sizeof(struct SimplexRing));
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

MatrixFlake creaMatrixFlake(int m, int n) {
  MatrixFlake ret = {
                  m : m,
                  n : n, 
                  A : NULL
                };
  int i;
  ret.A = (struct flake**) malloc (m * sizeof(struct flake*));
  if (ret.A != NULL){
    for(i = 0; i < m; i++){
      ret.A[i] = (struct flake *) malloc (n * sizeof(struct flake));
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

MatrixArista creaMatrixArista(int m, int n) {
	  MatrixArista ret = {
                  m : m,
                  n : n, 
                  A : NULL
                };
  int i;
  ret.A = (struct Arista**) malloc (m * sizeof(struct Arista*));
  if (ret.A != NULL){
    for(i = 0; i < m; i++){
      ret.A[i] = (struct Arista *) malloc (n * sizeof(struct Arista));
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

MatrixSquare creaMatrixSquare(int m, int n) {
	  MatrixSquare ret = {
                  m : m,
                  n : n, 
                  A : NULL
                };
  int i;
  ret.A = (struct Square**) malloc (m * sizeof(struct Square*));
  if (ret.A != NULL){
    for(i = 0; i < m; i++){
      ret.A[i] = (struct Square *) malloc (n * sizeof(struct Square));
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

MatrixSquareList initMatrixSquareList(int n) {
	MatrixSquareList ret = {
						n : n,
						m : NULL
					};

	ret.m = (MatrixSquare *) malloc (n * sizeof(MatrixSquare));

	return ret;
}

MatrixMeshList initMatrixMeshList(int n) {
	MatrixMeshList ret = {
						n : n,
						m : NULL
					};

	ret.m = (MatrixMesh *) malloc (n * sizeof(MatrixMesh));

	return ret;
}

MatrixCubeList initMatrixCubeList(int n) {
	MatrixCubeList ret = {
						n : n,
						m : NULL
					};

	ret.m = (MatrixCube *) malloc (n * sizeof(MatrixCube));

	return ret;
}

MatrixAristaList initMatrixAristaList(int n) {
	MatrixAristaList ret = {
						n : n,
						m : NULL
					};

	ret.m = (MatrixArista *) malloc (n * sizeof(MatrixArista));

	return ret;
}

MatrixList initMatrixList(int n) {

	MatrixList ret = {
						n : n,
						m : NULL
					};

	ret.m = (MatrixFlake *) malloc (n * sizeof(MatrixFlake));

	return ret;
}

void initSimplexRing(struct SimplexRing * sim, Vector3D a, Vector3D b, Vector3D c, double lambda) {

	sim->outer = startVectorSpace(3);
	sim->outer.v[0] = a;
	sim->outer.v[1] = b;
	sim->outer.v[2] = c;

	sim->a = startVectorSpace(3);
	sim->a.v[0] = dilate(sim->outer.v[1], sim->outer.v[0], 0.5);
	sim->a.v[1] = dilate(sim->outer.v[2], sim->outer.v[1], 0.5);
	sim->a.v[2] = dilate(sim->outer.v[2], sim->outer.v[0], 0.5);

	sim->centerMass = scale(0.3333, add(c, add(a, b)));
	Vector3D a1 = add(sim->centerMass, scale(lambda, sub(a, sim->centerMass)));
	Vector3D b1 = add(sim->centerMass, scale(lambda, sub(b, sim->centerMass)));
	Vector3D c1 = add(sim->centerMass, scale(lambda, sub(c, sim->centerMass)));
	
	sim->inner = startVectorSpace(3);
	sim->inner.v[0] = a1;
	sim->inner.v[1] = b1;
	sim->inner.v[2] = c1;

	sim->mesh = creaMatrixMesh(3, 2);

	sim->mesh.A[0][0] = facet(sim->outer.v[0], sim->outer.v[1], sim->inner.v[1]);
	sim->mesh.A[0][1] = facet(sim->outer.v[0], sim->inner.v[1], sim->inner.v[0]);

	sim->mesh.A[1][0] = facet(sim->outer.v[1], sim->outer.v[2], sim->inner.v[2]);
	sim->mesh.A[1][1] = facet(sim->outer.v[1], sim->inner.v[2], sim->inner.v[1]);

	sim->mesh.A[2][0] = facet(sim->outer.v[2], sim->outer.v[0], sim->inner.v[0]);
	sim->mesh.A[2][1] = facet(sim->outer.v[2], sim->inner.v[0], sim->inner.v[2]);

	sim->disable = -1;

}

void renderSimplexRing(struct SimplexRing sim, struct rotationMats U, int color) {

	if (sim.disable == -1) {
		renderFacet(sim.mesh.A[0][0], U, color);
		renderFacet(sim.mesh.A[0][1], U, color);
		renderFacet(sim.mesh.A[1][0], U, color);
		renderFacet(sim.mesh.A[1][1], U, color);
		renderFacet(sim.mesh.A[2][0], U, color);
		renderFacet(sim.mesh.A[2][1], U, color);
	}

	if (sim.disable == 0) {
		renderFacet(sim.mesh.A[1][0], U, color);
		renderFacet(sim.mesh.A[1][1], U, color);
		renderFacet(sim.mesh.A[2][0], U, color);
		renderFacet(sim.mesh.A[2][1], U, color);
	}

	if (sim.disable == 1) {
		renderFacet(sim.mesh.A[0][0], U, color);
		renderFacet(sim.mesh.A[0][1], U, color);
		renderFacet(sim.mesh.A[2][0], U, color);
		renderFacet(sim.mesh.A[2][1], U, color);
	}

	if (sim.disable == 2) {
		renderFacet(sim.mesh.A[0][0], U, color);
		renderFacet(sim.mesh.A[0][1], U, color);
		renderFacet(sim.mesh.A[1][0], U, color);
		renderFacet(sim.mesh.A[1][1], U, color);
	}
	
}

struct SimplexRing reflectSimplexRing(struct SimplexRing sim, Vector3D reflectionPoint) {

	struct SimplexRing ret;

	ret.outer = startVectorSpace(3);
	ret.inner = startVectorSpace(3);
	ret.a = startVectorSpace(3);

	ret.outer.v[0] = reflectRespectPoint(sim.outer.v[0], reflectionPoint);
	ret.outer.v[1] = reflectRespectPoint(sim.outer.v[1], reflectionPoint);
	ret.outer.v[2] = reflectRespectPoint(sim.outer.v[2], reflectionPoint);

	ret.inner.v[0] = reflectRespectPoint(sim.inner.v[0], reflectionPoint);
	ret.inner.v[1] = reflectRespectPoint(sim.inner.v[1], reflectionPoint);
	ret.inner.v[2] = reflectRespectPoint(sim.inner.v[2], reflectionPoint);

	ret.a.v[0] = dilate(ret.outer.v[1], ret.outer.v[0], 0.5);
	ret.a.v[1] = dilate(ret.outer.v[2], ret.outer.v[1], 0.5);
	ret.a.v[2] = dilate(ret.outer.v[2], ret.outer.v[0], 0.5);

	ret.centerMass = scale(0.333, add(ret.outer.v[2], add(ret.outer.v[0], ret.outer.v[1])));

	ret.mesh = creaMatrixMesh(3, 2);

	ret.mesh.A[0][0] = facet(ret.outer.v[0], ret.outer.v[1], ret.inner.v[1]);
	ret.mesh.A[0][1] = facet(ret.outer.v[0], ret.inner.v[1], ret.inner.v[0]);

	ret.mesh.A[1][0] = facet(ret.outer.v[1], ret.outer.v[2], ret.inner.v[2]);
	ret.mesh.A[1][1] = facet(ret.outer.v[1], ret.inner.v[2], ret.inner.v[1]);

	ret.mesh.A[2][0] = facet(ret.outer.v[2], ret.outer.v[0], ret.inner.v[0]);
	ret.mesh.A[2][1] = facet(ret.outer.v[2], ret.inner.v[0], ret.inner.v[2]);

	ret.disable = -1;

	return ret;
}

struct SimplexRing dilateSimplexRing(struct SimplexRing sim, Vector3D dilationPoint, double lambda) {

	struct SimplexRing ret;

	ret.outer = startVectorSpace(3);
	ret.inner = startVectorSpace(3);
	ret.a = startVectorSpace(3);

	ret.outer.v[0] = dilate(sim.outer.v[0], dilationPoint, lambda);
	ret.outer.v[1] = dilate(sim.outer.v[1], dilationPoint, lambda);
	ret.outer.v[2] = dilate(sim.outer.v[2], dilationPoint, lambda);

	ret.inner.v[0] = dilate(sim.inner.v[0], dilationPoint, lambda);
	ret.inner.v[1] = dilate(sim.inner.v[1], dilationPoint, lambda);
	ret.inner.v[2] = dilate(sim.inner.v[2], dilationPoint, lambda);

	ret.a.v[0] = dilate(ret.outer.v[1], ret.outer.v[0], 0.5);
	ret.a.v[1] = dilate(ret.outer.v[2], ret.outer.v[1], 0.5);
	ret.a.v[2] = dilate(ret.outer.v[2], ret.outer.v[0], 0.5);

	ret.centerMass = scale(0.333, add(ret.outer.v[2], add(ret.outer.v[0], ret.outer.v[1])));

	ret.mesh = creaMatrixMesh(3, 2);

	ret.mesh.A[0][0] = facet(ret.outer.v[0], ret.outer.v[1], ret.inner.v[1]);
	ret.mesh.A[0][1] = facet(ret.outer.v[0], ret.inner.v[1], ret.inner.v[0]);

	ret.mesh.A[1][0] = facet(ret.outer.v[1], ret.outer.v[2], ret.inner.v[2]);
	ret.mesh.A[1][1] = facet(ret.outer.v[1], ret.inner.v[2], ret.inner.v[1]);

	ret.mesh.A[2][0] = facet(ret.outer.v[2], ret.outer.v[0], ret.inner.v[0]);
	ret.mesh.A[2][1] = facet(ret.outer.v[2], ret.inner.v[0], ret.inner.v[2]);

	ret.disable = -1;

	return ret;
}

struct SimplexRing reflectAndDilate(struct SimplexRing sim, Vector3D actionPoint, double lambda) {

	struct SimplexRing ret;

	ret = reflectSimplexRing(sim, actionPoint);
	ret = dilateSimplexRing(ret, actionPoint, lambda);

	return ret;
}

Vector3D centerMass(struct Vector3DList set) {

	Vector3D ret;
	ret = initVector3D(0, 0, 0);

	int i;
	for (i = 0; i < set.n; i++) {
		ret = add(ret, set.v[i]);
	}

	ret = scale(1/((double)set.n), ret);
	return ret;
}

void initFlake(struct flake * star, struct SimplexRing sim) {

	star->mainSimplex = sim;
	star->id = 1;
	star->S = creaMatrixSimplexRing(2, 3);

	star->S.A[0][0] = reflectAndDilate(sim, sim.a.v[0], 0.33);
	star->S.A[0][0].disable = 0;

	star->S.A[0][1] = reflectAndDilate(sim, sim.a.v[1], 0.33);
	star->S.A[0][1].disable = 1;

	star->S.A[0][2] = reflectAndDilate(sim, sim.a.v[2], 0.33);
	star->S.A[0][2].disable = 2;

	star->S.A[1][0] = dilateSimplexRing(sim, sim.outer.v[0], 0.33);
	star->S.A[1][0].disable = 1;

	star->S.A[1][1] = dilateSimplexRing(sim, sim.outer.v[1], 0.33);
	star->S.A[1][1].disable = 2;

	star->S.A[1][2] = dilateSimplexRing(sim, sim.outer.v[2], 0.33);
	star->S.A[1][2].disable = 0;

}

void updateFlakeType(struct flake * star, int type) {
	star->type = type;
}

void renderFlake(struct flake star, struct rotationMats U) {

	if (star.id == 1) {
		if (star.mainSimplex.disable == -1){
			renderSimplexRing(star.S.A[0][0], U, 0);
			renderSimplexRing(star.S.A[0][1], U, 0);
			renderSimplexRing(star.S.A[0][2], U, 0);
			renderSimplexRing(star.S.A[1][0], U, 0);
			renderSimplexRing(star.S.A[1][1], U, 0);
			renderSimplexRing(star.S.A[1][2], U, 0);
		}
	
		if (star.mainSimplex.disable == 0) {
			renderSimplexRing(star.S.A[0][1], U, 0);
			renderSimplexRing(star.S.A[0][2], U, 0);
			renderSimplexRing(star.S.A[1][0], U, 0);
			renderSimplexRing(star.S.A[1][1], U, 0);
			renderSimplexRing(star.S.A[1][2], U, 0);
		}
	
		if (star.mainSimplex.disable == 1) {
			renderSimplexRing(star.S.A[0][0], U, 0);
			renderSimplexRing(star.S.A[0][2], U, 0);
			renderSimplexRing(star.S.A[1][0], U, 0);
			renderSimplexRing(star.S.A[1][1], U, 0);
			renderSimplexRing(star.S.A[1][2], U, 0);
		}
	
		if (star.mainSimplex.disable == 2) {
	
			renderSimplexRing(star.S.A[0][0], U, 0);
			renderSimplexRing(star.S.A[0][1], U, 0);
			renderSimplexRing(star.S.A[1][0], U, 0);
			renderSimplexRing(star.S.A[1][1], U, 0);
			renderSimplexRing(star.S.A[1][2], U, 0);
		}
	}
	
}

void initArista(struct Arista * arista, Vector3D a, Vector3D b, double width) {

	arista->head = a;
	arista->base = b;
	arista->arrow = startVectorSpace(4);
	arista->arrow.v[0] = b;
	arista->arrow.v[3] = a;
	struct rotationMats RotZ;
	struct rotationMats mRotZ;
	RotZ = initRotation( 0.5 * 3.14159265358979);
	mRotZ = initRotation(-0.5 * 3.14159265358979);

	Vector3D magicVec = scale(0.2, sub(b, a));
	rot3D(&RotZ, magicVec);

	arista->arrow.v[1] = add(RotZ.aux, arista->arrow.v[0]);
	arista->arrow.v[1] = dilate(arista->arrow.v[1], arista->arrow.v[3], 0.3);

	rot3D(&mRotZ, magicVec);

	arista->arrow.v[2] = add(mRotZ.aux, arista->arrow.v[0]);
	arista->arrow.v[2] = dilate(arista->arrow.v[2], arista->arrow.v[3], 0.3);

	arista->mesh = creaMatrixMesh(1, 2);
	arista->mesh.A[0][0] = facet(arista->arrow.v[0], arista->arrow.v[3], arista->arrow.v[1]);
	arista->mesh.A[0][1] = facet(arista->arrow.v[0], arista->arrow.v[2], arista->arrow.v[3]);

}

void renderArista(struct Arista arista, struct rotationMats U) {
	
	renderFacet(arista.mesh.A[0][0], U, 20);
	renderFacet(arista.mesh.A[0][1], U, 20);
}

void initKochArista(struct KochArista * kochArista, Vector3D a, Vector3D b, double width) {

	kochArista->points = startVectorSpace(5);
	kochArista->aristas = creaMatrixArista(1, 4);

	kochArista->points.v[0] = b;
	kochArista->points.v[4] = a;
	kochArista->points.v[1] = dilate(kochArista->points.v[4], kochArista->points.v[0], 0.333);
	kochArista->points.v[3] = dilate(kochArista->points.v[4], kochArista->points.v[0], 0.666);

	struct rotationMats RotZ;
	RotZ = initRotation(0.333 * 3.14159265358979);
	Vector3D magicVec = sub(kochArista->points.v[3], kochArista->points.v[1]);
	rot3D(&RotZ, magicVec);
	kochArista->points.v[2] = add(RotZ.aux, kochArista->points.v[1]);

	initArista(&kochArista->aristas.A[0][0], kochArista->points.v[1], kochArista->points.v[0], width);
	initArista(&kochArista->aristas.A[0][1], kochArista->points.v[2], kochArista->points.v[1], width);
	initArista(&kochArista->aristas.A[0][2], kochArista->points.v[3], kochArista->points.v[2], width);
	initArista(&kochArista->aristas.A[0][3], kochArista->points.v[4], kochArista->points.v[3], width);

}

void renderKochArista(struct KochArista kochArista, struct rotationMats U) {

	renderArista(kochArista.aristas.A[0][0], U);
	renderArista(kochArista.aristas.A[0][1], U);
	renderArista(kochArista.aristas.A[0][2], U);
	renderArista(kochArista.aristas.A[0][3], U);

}

void initKochCurve (struct KochCurve * koch, Vector3D a, Vector3D b, Vector3D c, int n) {

	koch->point0 = a;
	koch->point1 = b;
	koch->point2 = c;
	koch->order = n;
	struct KochArista auxPiece;

	koch->AristaList = initMatrixAristaList(koch->order + 1);

	koch->AristaList.m[0] = creaMatrixArista(1, 3);

	initArista(&koch->AristaList.m[0].A[0][0], koch->point1, koch->point0, 1.0);
	initArista(&koch->AristaList.m[0].A[0][1], koch->point2, koch->point1, 1.0);
	initArista(&koch->AristaList.m[0].A[0][2], koch->point0, koch->point2, 1.0);

	int i, j, k;
	int pow = 1;	
	int pow1 = 1;
	int cycle = 1;
	
	if (n > 0) {
		while (cycle < n + 1) {

			if (cycle == 1) pow *= 3;
			else pow *= 4;
			
			koch->AristaList.m[cycle] = creaMatrixArista(pow, 4);

			if (cycle == 1) {
				initKochArista(&auxPiece, koch->AristaList.m[cycle-1].A[0][0].head, koch->AristaList.m[cycle-1].A[0][0].base, 1.0);
				koch->AristaList.m[cycle].A[0][0] = auxPiece.aristas.A[0][0];
				koch->AristaList.m[cycle].A[0][1] = auxPiece.aristas.A[0][1];
				koch->AristaList.m[cycle].A[0][2] = auxPiece.aristas.A[0][2];
				koch->AristaList.m[cycle].A[0][3] = auxPiece.aristas.A[0][3];

				initKochArista(&auxPiece, koch->AristaList.m[cycle-1].A[0][1].head, koch->AristaList.m[cycle-1].A[0][1].base, 1.0);
				koch->AristaList.m[cycle].A[1][0] = auxPiece.aristas.A[0][0];
				koch->AristaList.m[cycle].A[1][1] = auxPiece.aristas.A[0][1];
				koch->AristaList.m[cycle].A[1][2] = auxPiece.aristas.A[0][2];
				koch->AristaList.m[cycle].A[1][3] = auxPiece.aristas.A[0][3];

				initKochArista(&auxPiece, koch->AristaList.m[cycle-1].A[0][2].head, koch->AristaList.m[cycle-1].A[0][2].base, 1.0);
				koch->AristaList.m[cycle].A[2][0] = auxPiece.aristas.A[0][0];
				koch->AristaList.m[cycle].A[2][1] = auxPiece.aristas.A[0][1];
				koch->AristaList.m[cycle].A[2][2] = auxPiece.aristas.A[0][2];
				koch->AristaList.m[cycle].A[2][3] = auxPiece.aristas.A[0][3];

			} else {

				int counted = 1;
				int cc = 0;
				int mod = 0;

				for ( counted = 0; counted < koch->AristaList.m[cycle].m; counted += 4) {
					
					for ( i = 0; i < 4; i++ ) {
						initKochArista(&auxPiece, koch->AristaList.m[cycle-1].A[mod][i].head, koch->AristaList.m[cycle-1].A[mod][i].base, 1.0);
						koch->AristaList.m[cycle].A[counted + i][0] = auxPiece.aristas.A[0][0];
						koch->AristaList.m[cycle].A[counted + i][1] = auxPiece.aristas.A[0][1];
						koch->AristaList.m[cycle].A[counted + i][2] = auxPiece.aristas.A[0][2];
						koch->AristaList.m[cycle].A[counted + i][3] = auxPiece.aristas.A[0][3];
						
					}
					mod += 1;
				}
			}
			cycle += 1;	
		}
	}
}

void renderKochCurve(struct KochCurve koch, struct rotationMats U, int iter) {

	int i, j;
	//printf("...  %d\n", koch.AristaList.m[2].m);
	for ( i = koch.order; i < koch.order + 1; i++) {
		for (j = 0; j < koch.AristaList.m[koch.order].m; j++) {
			renderArista(koch.AristaList.m[koch.order].A[j][0], U);
			renderArista(koch.AristaList.m[koch.order].A[j][1], U);
			renderArista(koch.AristaList.m[koch.order].A[j][2], U);
			renderArista(koch.AristaList.m[koch.order].A[j][3], U);
		}
	}

}

void initDragonArista(struct DragonArista * dragonArista, Vector3D a, Vector3D b, bool T) {

	dragonArista->points = startVectorSpace(3);
	dragonArista->aristas = creaMatrixArista(1, 2);

	dragonArista->points.v[0] = b;
	dragonArista->points.v[2] = a;

	struct rotationMats RotZ;

	if (T == true)
		RotZ = initRotation( 0.25 * 3.14159265358979);
	else RotZ = initRotation(-0.25 * 3.14159265358979);

	Vector3D magicVec = sub(dragonArista->points.v[2], dragonArista->points.v[0]);
	rot3D(&RotZ, magicVec);
	RotZ.aux = scale(0.7, RotZ.aux);
	dragonArista->points.v[1] = add(RotZ.aux, dragonArista->points.v[0]);

	initArista(&dragonArista->aristas.A[0][0], dragonArista->points.v[1], dragonArista->points.v[0], 1.0);
	initArista(&dragonArista->aristas.A[0][1], dragonArista->points.v[2], dragonArista->points.v[1], 1.0);

}

void renderDragonArista(struct DragonArista dragonArista, struct rotationMats U) {

	renderArista(dragonArista.aristas.A[0][0], U);
	renderArista(dragonArista.aristas.A[0][1], U);

}

void initDragonCurve (struct DragonCurve * dragon, Vector3D a, Vector3D b, int n) {

	dragon->point0 = b;
	dragon->point1 = a;
	dragon->order = n;
	struct DragonArista auxPiece;

	dragon->AristaList = initMatrixAristaList(dragon->order + 1);
	dragon->AristaList.m[0] = creaMatrixArista(1, 1);

	initArista(&dragon->AristaList.m[0].A[0][0], dragon->point1, dragon->point0, 1.0);

	int i, j, k;
	int pow = 1;	
	int pow1 = 1;
	int cycle = 1;
	
	if (n > 0) {
		while (cycle < n + 1) {

			pow *= 2;
			
			dragon->AristaList.m[cycle] = creaMatrixArista(1, pow);
			//printf("Constructed a list of %d edges\n", pow);
			int counted = 1;
			int cc = 0;
			int mod = 0;

			for ( i = 0; i < dragon->AristaList.m[cycle].n; i += 2) {
			//	printf("edge - %d         mod - %d\n", i, mod);
			//	printf("edge - %d \n", i+1);
				
				if (mod%2 == 0)
					initDragonArista(&auxPiece, dragon->AristaList.m[cycle-1].A[0][mod].head, dragon->AristaList.m[cycle-1].A[0][mod].base, true);
				else
					initDragonArista(&auxPiece, dragon->AristaList.m[cycle-1].A[0][mod].head, dragon->AristaList.m[cycle-1].A[0][mod].base, false);
				
				dragon->AristaList.m[cycle].A[0][i] = auxPiece.aristas.A[0][0];
				dragon->AristaList.m[cycle].A[0][i+1] = auxPiece.aristas.A[0][1];

				mod += 1;
			}
			//printf("\n");
			//printf("\n");
			//printf("\n");
			cycle += 1;	
		}
	}
}

void initLevyCurve (struct DragonCurve * dragon, Vector3D a, Vector3D b, int n) {

	dragon->point0 = b;
	dragon->point1 = a;
	dragon->order = n;
	struct DragonArista auxPiece;

	dragon->AristaList = initMatrixAristaList(dragon->order + 1);
	dragon->AristaList.m[0] = creaMatrixArista(1, 1);

	initArista(&dragon->AristaList.m[0].A[0][0], dragon->point1, dragon->point0, 1.0);

	int i, j, k;
	int pow = 1;	
	int pow1 = 1;
	int cycle = 1;
	
	if (n > 0) {
		while (cycle < n + 1) {

			pow *= 2;
			
			dragon->AristaList.m[cycle] = creaMatrixArista(1, pow);
			//printf("Constructed a list of %d edges\n", pow);
			int counted = 1;
			int cc = 0;
			int mod = 0;

			for ( i = 0; i < dragon->AristaList.m[cycle].n; i += 2) {
			//	printf("edge - %d         mod - %d\n", i, mod);
			//	printf("edge - %d \n", i+1);
				
				if (mod%2 == 0)
					initDragonArista(&auxPiece, dragon->AristaList.m[cycle-1].A[0][mod].head, dragon->AristaList.m[cycle-1].A[0][mod].base, true);
				else
					initDragonArista(&auxPiece, dragon->AristaList.m[cycle-1].A[0][mod].head, dragon->AristaList.m[cycle-1].A[0][mod].base, true);
				
				dragon->AristaList.m[cycle].A[0][i] = auxPiece.aristas.A[0][0];
				dragon->AristaList.m[cycle].A[0][i+1] = auxPiece.aristas.A[0][1];

				mod += 1;
			}
			//printf("\n");
			//printf("\n");
			//printf("\n");
			cycle += 1;	
		}
	}
}

void renderDragonCurve(struct DragonCurve dragon, struct rotationMats U, int iter) {

	int i, j;
	//printf("...  %d\n", dragon.AristaList.m[2].m);
	for ( i = iter; i < iter + 1; i++) {
		for (j = 0; j < dragon.AristaList.m[iter].n; j++) {
			renderArista(dragon.AristaList.m[iter].A[0][j], U);
		}
	}
}


/////////////////////

//     CARPET  

/////////////////////

void initSquare(struct Square * square, Vector3D a, Vector3D b, Vector3D c, Vector3D d) {

	square->square = startVectorSpace(4);
	square->mesh = creaMatrixMesh(1, 2);

	square->square.v[0] = a;
	square->square.v[1] = b;
	square->square.v[2] = c;
	square->square.v[3] = d;

	square->mesh.A[0][0] = facet(a, b, c);
	square->mesh.A[0][1] = facet(c, d, a);

}

void renderSquare(struct Square cuadro, struct rotationMats U) {

	renderFacet(cuadro.mesh.A[0][0], U, 0);
	renderFacet(cuadro.mesh.A[0][1], U, 0);
}

struct Square reflectSquare(struct Square mini, Vector3D auxPoint) {

	struct Square ret;
	ret.square = startVectorSpace(4);
	int i;
	for (i = 0; i < mini.square.n; i++)
		ret.square.v[i] = reflectRespectPoint(mini.square.v[i], auxPoint);

	ret.mesh = creaMatrixMesh(1, 2);
	ret.mesh.A[0][0] = facet(ret.square.v[0], ret.square.v[1], ret.square.v[2]);
	ret.mesh.A[0][1] = facet(ret.square.v[2], ret.square.v[3], ret.square.v[0]);

	return ret;
}

struct Square dilateSquare(struct Square mini, Vector3D auxPoint, double d) {
	
	struct Square ret;
	ret.square = startVectorSpace(4);
	int i;
	for (i = 0; i < mini.square.n; i++)
		ret.square.v[i] = dilate(mini.square.v[i], auxPoint, d);
	
	ret.mesh = creaMatrixMesh(1, 2);
	ret.mesh.A[0][0] = facet(ret.square.v[0], ret.square.v[1], ret.square.v[2]);
	ret.mesh.A[0][1] = facet(ret.square.v[2], ret.square.v[3], ret.square.v[0]);
	
	return ret;
}

void initMiniCarpet(struct MiniCarpet * mini, Vector3D a, Vector3D b, Vector3D c, Vector3D d) {

	mini->mainPoints = startVectorSpace(4);
	mini->mainPoints.v[0] = a;
	mini->mainPoints.v[1] = b;
	mini->mainPoints.v[2] = c;
	mini->mainPoints.v[3] = d;

	struct Square auxSquare;
	initSquare(&auxSquare, a, b, c, d);
	struct Square auxSquare1;

	Vector3D ce = centerSquare(a, b, c, d);
	auxSquare1 = dilateSquare(auxSquare, ce, 0.333333333);

	Vector3D aux0 = dilate(auxSquare1.square.v[1], auxSquare1.square.v[0], 0.5);
	Vector3D aux1 = dilate(auxSquare1.square.v[2], auxSquare1.square.v[1], 0.5);
	Vector3D aux2 = dilate(auxSquare1.square.v[3], auxSquare1.square.v[2], 0.5);
	Vector3D aux3 = dilate(auxSquare1.square.v[0], auxSquare1.square.v[3], 0.5);

	mini->cuadritos = creaMatrixSquare(1, 8);
	mini->cuadritos.A[0][0] = reflectSquare(auxSquare1, auxSquare1.square.v[0]);
	mini->cuadritos.A[0][1] = reflectSquare(auxSquare1, auxSquare1.square.v[1]);
	mini->cuadritos.A[0][2] = reflectSquare(auxSquare1, auxSquare1.square.v[2]);
	mini->cuadritos.A[0][3] = reflectSquare(auxSquare1, auxSquare1.square.v[3]);
	mini->cuadritos.A[0][4] = reflectSquare(auxSquare1, aux0);
	mini->cuadritos.A[0][5] = reflectSquare(auxSquare1, aux1);
	mini->cuadritos.A[0][6] = reflectSquare(auxSquare1, aux2);
	mini->cuadritos.A[0][7] = reflectSquare(auxSquare1, aux3);

}

void initMiniPythagoras(struct MiniPythagoras * mini, Vector3D a, Vector3D b, Vector3D c, Vector3D d) {

	mini->mainPoints = startVectorSpace(4);
	mini->mainPoints.v[0] = a;
	mini->mainPoints.v[1] = b;
	mini->mainPoints.v[2] = c;
	mini->mainPoints.v[3] = d;
	mini->cuadritos = creaMatrixSquare(1, 3);

	struct rotationMats RotZ;
	RotZ = initRotation(-0.25 * 3.14159265358979);
	struct Square auxSquare;
	initSquare(&auxSquare, a, b, c, d);
	mini->cuadritos.A[0][0] = auxSquare;

	auxSquare = reflectSquare(auxSquare, mini->mainPoints.v[0]);
	
	Vector3D magicVec0 = sub(auxSquare.square.v[1], auxSquare.square.v[0]);
	Vector3D magicVec1 = sub(auxSquare.square.v[2], auxSquare.square.v[0]);
	Vector3D magicVec2 = sub(auxSquare.square.v[3], auxSquare.square.v[0]);

	rot3D(&RotZ, magicVec0);
	Vector3D new_b = add(RotZ.aux, auxSquare.square.v[0]);

	rot3D(&RotZ, magicVec1);
	Vector3D new_c = add(RotZ.aux, auxSquare.square.v[0]);

	rot3D(&RotZ, magicVec2);
	Vector3D new_d = add(RotZ.aux, auxSquare.square.v[0]);

	struct Square auxSquare1;
	initSquare(&auxSquare1, auxSquare.square.v[0], new_b, new_c, new_d);

	mini->cuadritos.A[0][1] = dilateSquare(auxSquare1, auxSquare.square.v[0], 0.5);
	
	Vector3D pleaseHelp = dilate(mini->cuadritos.A[0][1].square.v[3], mini->cuadritos.A[0][1].square.v[1], 1.205);
	mini->cuadritos.A[0][2] = reflectSquare(mini->cuadritos.A[0][1], pleaseHelp);

	Vector3D ce = centerSquare(mini->cuadritos.A[0][1].square.v[0], mini->cuadritos.A[0][1].square.v[1], mini->cuadritos.A[0][1].square.v[2], mini->cuadritos.A[0][1].square.v[3]);
	mini->cuadritos.A[0][1] = reflectSquare(mini->cuadritos.A[0][1], ce);
	initSquare(&mini->cuadritos.A[0][1], mini->cuadritos.A[0][1].square.v[3], mini->cuadritos.A[0][1].square.v[0], mini->cuadritos.A[0][1].square.v[1], mini->cuadritos.A[0][1].square.v[2]);
}

void renderMiniPythagoras(struct MiniPythagoras mini, struct rotationMats U) {

	renderSquare(mini.cuadritos.A[0][0], U);
	renderSquare(mini.cuadritos.A[0][1], U);
	renderSquare(mini.cuadritos.A[0][2], U);
}

void renderMiniCarpet(struct MiniCarpet mini, struct rotationMats U) {

	renderSquare(mini.cuadritos.A[0][0], U);
	renderSquare(mini.cuadritos.A[0][1], U);
	renderSquare(mini.cuadritos.A[0][2], U);
	renderSquare(mini.cuadritos.A[0][3], U);
	renderSquare(mini.cuadritos.A[0][4], U);
	renderSquare(mini.cuadritos.A[0][5], U);
	renderSquare(mini.cuadritos.A[0][6], U);
	renderSquare(mini.cuadritos.A[0][7], U);

}

void renderTreePythagoras(struct TreePythagoras tree, struct rotationMats U, int iter){

	int i, j;
	for ( i = 0; i < tree.order; i++)
		for ( j = 0; j < tree.SquareList.m[i].n; j++)
			renderSquare(tree.SquareList.m[i].A[0][j], U);
}


void initTreePythagoras(struct TreePythagoras * tree, Vector3D a, Vector3D b, Vector3D c, Vector3D d, int n) {
	tree->point0 = a;
	tree->point1 = b;
	tree->point2 = c;
	tree->point3 = d;
	tree->order = n;
	struct MiniPythagoras auxPiece;

	tree->SquareList = initMatrixSquareList(tree->order + 1);
	tree->SquareList.m[0] = creaMatrixSquare(1, 1);

	initSquare(&tree->SquareList.m[0].A[0][0], tree->point0, tree->point1, tree->point2, tree->point3);

	int i, j, k;
	int pow = 1;	
	int pow1 = 1;
	int cycle = 1;

	if (n > 0) {
		while (cycle < n + 1) {

			pow *= 2;
			
			tree->SquareList.m[cycle] = creaMatrixSquare(1, pow);
			int counted = 1;
			int cc = 0;
			int mod = 0;

			for ( i = 0; i < tree->SquareList.m[cycle].n; i += 2) {

				initMiniPythagoras(&auxPiece, 
											tree->SquareList.m[cycle-1].A[0][mod].square.v[0],
											tree->SquareList.m[cycle-1].A[0][mod].square.v[1],
											tree->SquareList.m[cycle-1].A[0][mod].square.v[2],
											tree->SquareList.m[cycle-1].A[0][mod].square.v[3]
								);		

				tree->SquareList.m[cycle].A[0][i+0] = auxPiece.cuadritos.A[0][1];
				tree->SquareList.m[cycle].A[0][i+1] = auxPiece.cuadritos.A[0][2];

				mod += 1;
			}
			cycle += 1;	
		}
	}
}

void initMengerCarpet(struct MengerCarpet * menger, Vector3D a, Vector3D b, Vector3D c, Vector3D d, int n) {

	menger->point0 = a;
	menger->point1 = b;
	menger->point2 = c;
	menger->point3 = d;
	menger->order = n;
	struct MiniCarpet auxPiece;

	menger->SquareList = initMatrixSquareList(menger->order + 1);
	menger->SquareList.m[0] = creaMatrixSquare(1, 1);

	initSquare(&menger->SquareList.m[0].A[0][0], menger->point0, menger->point1, menger->point2, menger->point3);

	int i, j, k;
	int pow = 1;	
	int pow1 = 1;
	int cycle = 1;
	
	if (n > 0) {
		while (cycle < n + 1) {

			pow *= 8;
			
			menger->SquareList.m[cycle] = creaMatrixSquare(1, pow);
			//printf("Cycle %d -- constructed a list of %d squares\n", cycle, pow);
			int counted = 1;
			int cc = 0;
			int mod = 0;

			for ( i = 0; i < menger->SquareList.m[cycle].n; i += 8) {
				//printf("Square - %d         mod - %d\n", i, mod);
				//printf("Square - %d \n", i+1);
				//printf("Square - %d \n", i+2);
				//printf("Square - %d \n", i+3);
				//printf("Square - %d \n", i+4);
				//printf("Square - %d \n", i+5);
				//printf("Square - %d \n", i+6);
				//printf("Square - %d \n", i+7);
				
				initMiniCarpet(&auxPiece, 
											menger->SquareList.m[cycle-1].A[0][mod].square.v[0],
											menger->SquareList.m[cycle-1].A[0][mod].square.v[1],
											menger->SquareList.m[cycle-1].A[0][mod].square.v[2],
											menger->SquareList.m[cycle-1].A[0][mod].square.v[3]
								);		

				menger->SquareList.m[cycle].A[0][i+0] = auxPiece.cuadritos.A[0][0];
				menger->SquareList.m[cycle].A[0][i+1] = auxPiece.cuadritos.A[0][1];
				menger->SquareList.m[cycle].A[0][i+2] = auxPiece.cuadritos.A[0][2];
				menger->SquareList.m[cycle].A[0][i+3] = auxPiece.cuadritos.A[0][3];
				menger->SquareList.m[cycle].A[0][i+4] = auxPiece.cuadritos.A[0][4];
				menger->SquareList.m[cycle].A[0][i+5] = auxPiece.cuadritos.A[0][5];
				menger->SquareList.m[cycle].A[0][i+6] = auxPiece.cuadritos.A[0][6];
				menger->SquareList.m[cycle].A[0][i+7] = auxPiece.cuadritos.A[0][7];
						
				mod += 1;
			}
			//printf("\n");
			//printf("\n");
			//printf("\n");
			cycle += 1;	
		}
	}
}

void renderMengerCarpet(struct MengerCarpet menger, struct rotationMats U, int iter) {

	int i, j;
	for ( i = iter; i < iter + 1; i++) {
		for (j = 0; j < menger.SquareList.m[iter].n; j++) {
			renderSquare(menger.SquareList.m[iter].A[0][j], U);
		}
	}
}

void initMiniSierspinski(struct MiniSierspinski * mini, Vector3D a, Vector3D b, Vector3D c) {

	mini->mainPoints = startVectorSpace(3);
	mini->mainPoints.v[0] = a;
	mini->mainPoints.v[1] = b;
	mini->mainPoints.v[2] = c;

	struct Simplex auxTriangle = facet(a, b, c);
	
	mini->triangulitos = creaMatrixMesh(1, 3);
	mini->triangulitos.A[0][0] = dilateSimplex(auxTriangle, auxTriangle.vertexSpace.v[0], 0.5);
	mini->triangulitos.A[0][1] = dilateSimplex(auxTriangle, auxTriangle.vertexSpace.v[1], 0.5);
	mini->triangulitos.A[0][2] = dilateSimplex(auxTriangle, auxTriangle.vertexSpace.v[2], 0.5);	
}

void renderMiniSierspinski(struct MiniSierspinski mini, struct rotationMats U) {

	renderFacet(mini.triangulitos.A[0][0], U, 0);
	renderFacet(mini.triangulitos.A[0][1], U, 0);
	renderFacet(mini.triangulitos.A[0][2], U, 0);
}

void initSierspinskiTriangle(struct SierspinskiTriangle * mini, Vector3D a, Vector3D b, Vector3D c, int n) {

	mini->point0 = a;
	mini->point1 = b;
	mini->point2 = c;
	mini->order = n;
	struct MiniSierspinski auxPiece;

	mini->FacetList = initMatrixMeshList(mini->order + 1);
	mini->FacetList.m[0] = creaMatrixMesh(1, 1);

	mini->FacetList.m[0].A[0][0] = facet(mini->point0, mini->point1, mini->point2);

	int i, j, k;
	int pow = 1;	
	int pow1 = 1;
	int cycle = 1;
	
	if (n > 0) {
		while (cycle < n + 1) {

			pow *= 3;
			
			mini->FacetList.m[cycle] = creaMatrixMesh(1, pow);
//			printf("Cycle %d -- constructed a list of %d squares\n", cycle, pow);
			int counted = 1;
			int cc = 0;
			int mod = 0;

			for ( i = 0; i < mini->FacetList.m[cycle].n; i += 3) {
//				//printf("Square - %d         mod - %d\n", i, mod);
//				//printf("Square - %d \n", i+1);
//				//printf("Square - %d \n", i+2);
//				//printf("Square - %d \n", i+3);
//				//printf("Square - %d \n", i+4);
//				//printf("Square - %d \n", i+5);
//				//printf("Square - %d \n", i+6);
//				//printf("Square - %d \n", i+7);
//				
				initMiniSierspinski(&auxPiece, 
											mini->FacetList.m[cycle-1].A[0][mod].vertexSpace.v[0],
											mini->FacetList.m[cycle-1].A[0][mod].vertexSpace.v[1],
											mini->FacetList.m[cycle-1].A[0][mod].vertexSpace.v[2]
								);		

				mini->FacetList.m[cycle].A[0][i+0] = auxPiece.triangulitos.A[0][0];
				mini->FacetList.m[cycle].A[0][i+1] = auxPiece.triangulitos.A[0][1];
				mini->FacetList.m[cycle].A[0][i+2] = auxPiece.triangulitos.A[0][2];
//				mini->FacetList.m[cycle].A[0][i+3] = auxPiece.triangulitos.A[0][3];
//				mini->FacetList.m[cycle].A[0][i+4] = auxPiece.cuadritos.A[0][4];
//				mini->FacetList.m[cycle].A[0][i+5] = auxPiece.cuadritos.A[0][5];
//				mini->FacetList.m[cycle].A[0][i+6] = auxPiece.cuadritos.A[0][6];
//				mini->FacetList.m[cycle].A[0][i+7] = auxPiece.cuadritos.A[0][7];
//						
				mod += 1;
			}
			//printf("\n");
			//printf("\n");
			//printf("\n");
			cycle += 1;	
		}
	}
}

void renderSierspinskiTriangle(struct SierspinskiTriangle mini, struct rotationMats U, int iter) {

	int i, j;
	for ( i = iter; i < iter + 1; i++) {
		for (j = 0; j < mini.FacetList.m[iter].n; j++) {
			renderFacet(mini.FacetList.m[iter].A[0][j], U, 0);
		}
	}

}

void initKochFractal2D(struct KochFractal2D * curve, struct SimplexRing sim, int n) {

	curve->mainSimplex = sim;
	curve->order = n;
	int rows = 1;
	int i;
	curve->pieceCollector = initMatrixList(n);
	int cycle = 1;

	struct flake star1;
	

	int j, k;

	/*Here we build our memory space for our koch fractal.*/
	while (cycle < n + 1) {
		int numOfFlakes = 1;

		if (cycle-1 == 0) {

			int pow = 1;
			curve->pieceCollector.m[cycle-1] = creaMatrixFlake(1, pow);
			initFlake(&curve->pieceCollector.m[cycle-1].A[0][0], sim);
			printf("%d) --  Matrix of 1 row with n := %d flakes built.\n", cycle, pow);

		} else {

			int pow = 1;
			for (i = 0; i < cycle - 1; i++) {
				pow *= 6;
			}

			printf("The previous Matrix has %d flakes.\n", pow/6);
			curve->pieceCollector.m[cycle-1] = creaMatrixFlake(1, pow);
			printf("%d) --  Matrix of 1 row with n := %d flakes built.\n", cycle, pow);

			printf("Now we iterate through each flake.\n");
			
			//		initFlake(&curve->pieceCollector.m[cycle-1].A[0][i], curve->pieceCollector.m[cycle-2].A[0][k].S[0][0]);
			int count = 0;
			int count1 = 0;
			while (count < pow) {

				initFlake(&curve->pieceCollector.m[cycle-1].A[0][count + 0], curve->pieceCollector.m[cycle-2].A[0][count1].S.A[0][0]);
				updateFlakeType(&curve->pieceCollector.m[cycle-1].A[0][count + 0], 0);
				initFlake(&curve->pieceCollector.m[cycle-1].A[0][count + 1], curve->pieceCollector.m[cycle-2].A[0][count1].S.A[0][1]);
				updateFlakeType(&curve->pieceCollector.m[cycle-1].A[0][count + 1], 1);
				initFlake(&curve->pieceCollector.m[cycle-1].A[0][count + 2], curve->pieceCollector.m[cycle-2].A[0][count1].S.A[0][2]);
				updateFlakeType(&curve->pieceCollector.m[cycle-1].A[0][count + 2], 2);
				initFlake(&curve->pieceCollector.m[cycle-1].A[0][count + 3], curve->pieceCollector.m[cycle-2].A[0][count1].S.A[1][0]);
				updateFlakeType(&curve->pieceCollector.m[cycle-1].A[0][count + 3], 3);
				initFlake(&curve->pieceCollector.m[cycle-1].A[0][count + 4], curve->pieceCollector.m[cycle-2].A[0][count1].S.A[1][1]);
				updateFlakeType(&curve->pieceCollector.m[cycle-1].A[0][count + 4], 4);
				initFlake(&curve->pieceCollector.m[cycle-1].A[0][count + 5], curve->pieceCollector.m[cycle-2].A[0][count1].S.A[1][2]);
				updateFlakeType(&curve->pieceCollector.m[cycle-1].A[0][count + 5], 5);

				printf("Filled current matrix with six flakes from %d----matrixFlake[%d] ....\n", count1, cycle-2);

				count += 6;
				count1 += 1;
			}

		}

		printf("\n\n\n");
		cycle += 1;
	}

}

void renderKochFractal2D(struct KochFractal2D koch, struct rotationMats U) {

	int i, j;

	for (j = 1; j < 2; j++)//for (j = koch.order - 1; j < koch.order; j++)
		for (i = 0; i < koch.pieceCollector.m[j].n; i++) {
			renderFlake(koch.pieceCollector.m[j].A[0][i], U);
		}
}

///////////////////////
//					 //
//   CUBE GEOMETRY   //
//					 //   
///////////////////////



struct Cube3D initCube3D(double r, Vector3D position) {

	Vector3D w0 = initVector3D( r + position.x, r + position.y, r + position.z);
	Vector3D w1 = initVector3D(-r + position.x, r + position.y, r + position.z);
	Vector3D w2 = initVector3D(-r + position.x,-r + position.y, r + position.z);
	Vector3D w3 = initVector3D( r + position.x,-r + position.y, r + position.z);
	Vector3D w4 = initVector3D( r + position.x, r + position.y,-r + position.z);
	Vector3D w5 = initVector3D(-r + position.x, r + position.y,-r + position.z);
	Vector3D w6 = initVector3D(-r + position.x,-r + position.y,-r + position.z);
	Vector3D w7 = initVector3D( r + position.x,-r + position.y,-r + position.z);

	struct Vector3DList vertices = VectorSpace(8, w0, w1, w2, w3, w4, w5, w6, w7);

	struct Cube3D cube = {
							r : r,
							position : position,
							vertices : vertices
						 };

	cube.mesh = creaMatrixMesh(6, 2);

	cube.mesh.A[0][0] = facet(vertices.v[0], vertices.v[1], vertices.v[2]); 
	cube.mesh.A[0][1] = facet(vertices.v[2], vertices.v[3], vertices.v[0]);

	cube.mesh.A[1][0] = facet(vertices.v[4], vertices.v[7], vertices.v[6]);
	cube.mesh.A[1][1] = facet(vertices.v[6], vertices.v[5], vertices.v[4]);

	cube.mesh.A[2][0] = facet(vertices.v[0], vertices.v[3], vertices.v[7]);
	cube.mesh.A[2][1] = facet(vertices.v[7], vertices.v[4], vertices.v[0]);

	cube.mesh.A[3][0] = facet(vertices.v[1], vertices.v[5], vertices.v[6]);
	cube.mesh.A[3][1] = facet(vertices.v[6], vertices.v[2], vertices.v[1]);

	cube.mesh.A[4][0] = facet(vertices.v[0], vertices.v[4], vertices.v[5]);
	cube.mesh.A[4][1] = facet(vertices.v[5], vertices.v[1], vertices.v[0]);

	cube.mesh.A[5][0] = facet(vertices.v[3], vertices.v[2], vertices.v[6]);
	cube.mesh.A[5][1] = facet(vertices.v[6], vertices.v[7], vertices.v[3]);

	return cube;
}

struct Cube3D initInversive3D(double r, Vector3D position, double invRad, Vector3D gamma) {

	Vector3D w0 = initVector3D( r + position.x, r + position.y, r + position.z);
	Vector3D w1 = initVector3D(-r + position.x, r + position.y, r + position.z);
	Vector3D w2 = initVector3D(-r + position.x,-r + position.y, r + position.z);
	Vector3D w3 = initVector3D( r + position.x,-r + position.y, r + position.z);
	Vector3D w4 = initVector3D( r + position.x, r + position.y,-r + position.z);
	Vector3D w5 = initVector3D(-r + position.x, r + position.y,-r + position.z);
	Vector3D w6 = initVector3D(-r + position.x,-r + position.y,-r + position.z);
	Vector3D w7 = initVector3D( r + position.x,-r + position.y,-r + position.z);

	struct Vector3DList vertices = VectorSpace(8, w0, w1, w2, w3, w4, w5, w6, w7);

	struct Cube3D cube = {
							r : r,
							position : position,
							vertices : vertices,
							invRad : invRad,
							gamma : gamma, 
							counter : 0
						 };

	cube.mesh = creaMatrixMesh(6, 2);

	int i;
	for (i = 0; i < vertices.n; i++)
		vertices.v[i] = inversion3D(vertices.v[i], invRad, gamma);

	cube.mesh.A[0][0] = facet(vertices.v[0], vertices.v[1], vertices.v[2]); 
	cube.mesh.A[0][1] = facet(vertices.v[2], vertices.v[3], vertices.v[0]);
	
	cube.mesh.A[1][0] = facet(vertices.v[4], vertices.v[7], vertices.v[6]);
	cube.mesh.A[1][1] = facet(vertices.v[6], vertices.v[5], vertices.v[4]);

	cube.mesh.A[2][0] = facet(vertices.v[0], vertices.v[3], vertices.v[7]);
	cube.mesh.A[2][1] = facet(vertices.v[7], vertices.v[4], vertices.v[0]);

	cube.mesh.A[3][0] = facet(vertices.v[1], vertices.v[5], vertices.v[6]);
	cube.mesh.A[3][1] = facet(vertices.v[6], vertices.v[2], vertices.v[1]);

	cube.mesh.A[4][0] = facet(vertices.v[0], vertices.v[4], vertices.v[5]);
	cube.mesh.A[4][1] = facet(vertices.v[5], vertices.v[1], vertices.v[0]);

	cube.mesh.A[5][0] = facet(vertices.v[3], vertices.v[2], vertices.v[6]);
	cube.mesh.A[5][1] = facet(vertices.v[6], vertices.v[7], vertices.v[3]);

	return cube;
}

void updateInvCube3D(struct Cube3D * invCube, Vector3D newPosition, Vector3D pos) {

	updateVector3D1(&invCube->vertices.v[0], invCube->r + newPosition.x + pos.x, invCube->r + newPosition.y + pos.y, invCube->r + newPosition.z + pos.z);
	updateVector3D1(&invCube->vertices.v[1],-invCube->r + newPosition.x + pos.x, invCube->r + newPosition.y + pos.y, invCube->r + newPosition.z + pos.z);
	updateVector3D1(&invCube->vertices.v[2],-invCube->r + newPosition.x + pos.x,-invCube->r + newPosition.y + pos.y, invCube->r + newPosition.z + pos.z);
	updateVector3D1(&invCube->vertices.v[3], invCube->r + newPosition.x + pos.x,-invCube->r + newPosition.y + pos.y, invCube->r + newPosition.z + pos.z);
	updateVector3D1(&invCube->vertices.v[4], invCube->r + newPosition.x + pos.x, invCube->r + newPosition.y + pos.y,-invCube->r + newPosition.z + pos.z);
	updateVector3D1(&invCube->vertices.v[5],-invCube->r + newPosition.x + pos.x, invCube->r + newPosition.y + pos.y,-invCube->r + newPosition.z + pos.z);
	updateVector3D1(&invCube->vertices.v[6],-invCube->r + newPosition.x + pos.x,-invCube->r + newPosition.y + pos.y,-invCube->r + newPosition.z + pos.z);
	updateVector3D1(&invCube->vertices.v[7], invCube->r + newPosition.x + pos.x,-invCube->r + newPosition.y + pos.y,-invCube->r + newPosition.z + pos.z);

	for (invCube->counter = 0; invCube->counter < 8; invCube->counter++)
		updateInversion3D(&invCube->vertices.v[invCube->counter], invCube->invRad, invCube->gamma);

	invCube->position = newPosition;

	updateFacet(&invCube->mesh.A[0][0], invCube->vertices.v[0], invCube->vertices.v[1], invCube->vertices.v[2]);
	updateFacet(&invCube->mesh.A[0][1], invCube->vertices.v[2], invCube->vertices.v[3], invCube->vertices.v[0]);
	updateFacet(&invCube->mesh.A[1][0], invCube->vertices.v[4], invCube->vertices.v[7], invCube->vertices.v[6]);
	updateFacet(&invCube->mesh.A[1][1], invCube->vertices.v[6], invCube->vertices.v[5], invCube->vertices.v[4]);
	updateFacet(&invCube->mesh.A[2][0], invCube->vertices.v[0], invCube->vertices.v[3], invCube->vertices.v[7]);
	updateFacet(&invCube->mesh.A[2][1], invCube->vertices.v[7], invCube->vertices.v[4], invCube->vertices.v[0]);
	updateFacet(&invCube->mesh.A[3][0], invCube->vertices.v[1], invCube->vertices.v[5], invCube->vertices.v[6]);
	updateFacet(&invCube->mesh.A[3][1], invCube->vertices.v[6], invCube->vertices.v[2], invCube->vertices.v[1]);
	updateFacet(&invCube->mesh.A[4][0], invCube->vertices.v[0], invCube->vertices.v[4], invCube->vertices.v[5]);
	updateFacet(&invCube->mesh.A[4][1], invCube->vertices.v[5], invCube->vertices.v[1], invCube->vertices.v[0]);
	updateFacet(&invCube->mesh.A[5][0], invCube->vertices.v[3], invCube->vertices.v[2], invCube->vertices.v[6]);
	updateFacet(&invCube->mesh.A[5][1], invCube->vertices.v[6], invCube->vertices.v[7], invCube->vertices.v[3]);

}

void printCube3D(struct Cube3D cube) {
	
	int i;
	printf("\n");
	for (i = 0; i < cube.vertices.n; i++) {
		writeVector3D(cube.vertices.v[i]);
		printf("\n");
	}
	printf("\n%d faces with %d triangles each\n", cube.mesh.m, cube.mesh.n);
}

void renderCube3D(struct Cube3D cube, struct rotationMats U) {

	renderFacet(cube.mesh.A[0][0], U, 0);
	renderFacet(cube.mesh.A[0][1], U, 0);

	renderFacet(cube.mesh.A[1][0], U, 0);
	renderFacet(cube.mesh.A[1][1], U, 0);

	renderFacet(cube.mesh.A[2][0], U, 0);
	renderFacet(cube.mesh.A[2][1], U, 0);

	renderFacet(cube.mesh.A[3][0], U, 0);
	renderFacet(cube.mesh.A[3][1], U, 0);

	renderFacet(cube.mesh.A[4][0], U, 0);
	renderFacet(cube.mesh.A[4][1], U, 0);

	renderFacet(cube.mesh.A[5][0], U, 0);
	renderFacet(cube.mesh.A[5][1], U, 0);
}

//struct Cube3D reflectCube(struct Cube3D cube, Vector3D auxPoint) {
//
//	struct Square ret;
//	ret.square = startVectorSpace(4);
//	int i;
//	for (i = 0; i < mini.square.n; i++)
//		ret.square.v[i] = reflectRespectPoint(mini.square.v[i], auxPoint);
//
//	ret.mesh = creaMatrixMesh(1, 2);
//	ret.mesh.A[0][0] = facet(ret.square.v[0], ret.square.v[1], ret.square.v[2]);
//	ret.mesh.A[0][1] = facet(ret.square.v[2], ret.square.v[3], ret.square.v[0]);
//
//	return ret;
//	struct Cube3D ret;
//	ret.
//}

//struct Cube3D dilateCube(struct Cube3D cube, Vector3D auxPoint, double d) {
//	
//	struct Square ret;
//	ret.square = startVectorSpace(4);
//	int i;
//	for (i = 0; i < mini.square.n; i++)
//		ret.square.v[i] = dilate(mini.square.v[i], auxPoint, d);
//	
//	ret.mesh = creaMatrixMesh(1, 2);
//	ret.mesh.A[0][0] = facet(ret.square.v[0], ret.square.v[1], ret.square.v[2]);
//	ret.mesh.A[0][1] = facet(ret.square.v[2], ret.square.v[3], ret.square.v[0]);
//	
//	return ret;
//}

////////////////////////////
//					      //
//    3D CUBIC LATTICE    //
//					      //
////////////////////////////


MatrixCube creaMatrixCube(int m, int n) {
  MatrixCube ret = {
                  m : m,
                  n : n, 
                  A : NULL
                };
  int i;
  ret.A = (struct Cube3D**) malloc (m * sizeof(struct Cube3D*));
  if (ret.A != NULL){
    for(i = 0; i < m; i++){
      ret.A[i] = (struct Cube3D *) malloc (n * sizeof(struct Cube3D));
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

MatrixCube3 creaMatrix3Cube(int m, int n, int k) {
	MatrixCube3 ret = {
                  m : m,
                  n : n,
                  k : k, 
                  A : NULL
                };
    int i;
    int j;
    ret.A = (struct Cube3D***) malloc (m * sizeof(struct Cube3D**));
    if (ret.A != NULL) {
    	for (i = 0; i < m; i++) {

    		ret.A[i] = (struct Cube3D**) malloc (n * sizeof(struct Cube3D*));
    		if (ret.A[i] != NULL) {
    			for (j = 0; j < n; j++) {
    				ret.A[i][j] = (struct Cube3D*) malloc (k * sizeof(struct Cube3D));
    				
    			}
    		}
    	
    	}
    }

    return ret;
}

struct CubicLattice3D initCubicLattice3D(double r, int mod, int color, Vector3D position, int m, int n) {
	
	int i, j;
	int sizeX = 2 * m;
	int sizeY = 2 * n;

	struct CubicLattice3D lattice = {
								r : r,
								position : position,
								mod : mod,
								color : color,
								sizeX : sizeX,
								sizeY : sizeY,
								i1 : 0,
								j1 : 0,
								k1 : 0
							};

	lattice.cubicSpace = creaMatrixCube(sizeX, sizeY);

	for (i = -m; i < m; i++) {
		for (j = -n; j < n; j++) {
			Vector3D primePosition = initVector3D(2 * r * i, 2 * r * j, 0); 
			lattice.cubicSpace.A[i + m][j + n] = initCube3D(r, add(primePosition, position));
		}
	}

	return lattice;
}



/***********************/

/*	    MANDELBROT     */

/***********************/

struct CubicLattice3D initMandelBrot(double r, int mod, int color, Vector3D position, int m, int n, double re, double img) {
	
	int i, j;
	int sizeX = 2 * m;
	int sizeY = 2 * n;

	struct CubicLattice3D lattice = {
								r : r,
								position : position,
								mod : mod,
								color : color,
								sizeX : sizeX,
								sizeY : sizeY,
								i1 : 0,
								j1 : 0,
								k1 : 0,
								levels : 300
							};

	lattice.cubicSpace = creaMatrixCube(sizeX, sizeY);
	printf("Intit MandelBrot .. \n");

	double a = 0.0;
	double b = 0.0;
	double ca = 0.0;
	double cb = 0.0;
	double aa = 0.0;
	double bb = 0.0;
	double twoab = 0.0;

	double * pa = &a;
	double * pb = &b;
	double * pca = &ca;
	double * pcb = &cb;
	double * paa = &aa;
	double * pbb = &bb;
	double * ptwoab = &twoab;
	int N = 0;
	int * pN = &N;
	int maxIterations = 2000;

	lattice.code = creaEspacioMatriz(sizeX, sizeY);

	for (i =-m; i < m; i++) {
		for (j =-n; j < n; j++) {

			* pa = r * (double)i;
			* pb = r * (double)j;

			if (re == 0 && img == 0) {
				* pca = a;
				* pcb = b;
			} else {
				* pca = re;
				* pcb = img;
			}

			* pN = 0;
			while (N < maxIterations) {
				
				* paa = a * a;
				* pbb = b * b;
				* ptwoab = 2.0 * a * b;
				a = aa - bb + ca;
				b = twoab + cb;

				if (a*a + b*b > 5.0) break;

				N += 1; 
			}


			Vector3D primePosition;
			lattice.primePosition = initVector3D(0, 0, 0);

				int k;
				double df = 0.0;

				for (k = 0; k < lattice.levels + 1; k += 1) {
					if (k < N && N <= k + 2) {
						primePosition = scale(2, initVector3D(r * (double)i + 0.5, r * (double)j, df));
						lattice.cubicSpace.A[i + m][j + n] = initCube3D(r, primePosition);
						lattice.code.A[i + m][j + n] = (double)N;}
						df -= 0.003;
				}

				if (lattice.levels + 2 < N && N <= maxIterations) {
						primePosition = scale(2, initVector3D(r * (double)i + 0.5, r * (double)j, df));
						lattice.cubicSpace.A[i + m][j + n] = initCube3D(r, primePosition);
						lattice.code.A[i + m][j + n] = (double)N;}
	}
}

	lattice.latticeMesh = creaMatrixMesh(sizeX, sizeY);
	lattice.latticeMesh2 = creaMatrixMesh(sizeX, sizeY);

	for (i = 0; i < sizeX-1; i++)
		for (j = 0; j < sizeY-1; j++)
			lattice.latticeMesh.A[i][j] = facet(
													lattice.cubicSpace.A[i][j].position, 
													lattice.cubicSpace.A[i + 1][j].position,
													lattice.cubicSpace.A[i + 1][j + 1].position);

	for (i = 0; i < sizeX-1; i++)
		for (j = 0; j < sizeY-1; j++)
			lattice.latticeMesh2.A[i][j] = facet(
													lattice.cubicSpace.A[i][j].position, 
													lattice.cubicSpace.A[i + 1][j + 1].position,
													lattice.cubicSpace.A[i][j + 1].position);

	return lattice;
}

struct CubicLattice3D3 initCubicLattice3D3(double r, int mod, int color, Vector3D position, int m, int n, int k) {
	
	int i, j, k0;
	int sizeX = 2 * m;
	int sizeY = 2 * n;
	int sizeZ = 2 * k;

	struct CubicLattice3D3 lattice = {
								r : r,
								position : position,
								mod : mod,
								color : color,
								sizeX : sizeX,
								sizeY : sizeY,
								sizeZ : sizeZ,
								i1 : 0,
								j1 : 0,
								k1 : 0
							};

	lattice.cubicSpace = creaMatrix3Cube(sizeX, sizeY, sizeZ);
	double R = 8;
	R /= 50;
	for (i = -m; i < m; i++) {
		for (j = -n; j < n; j++) {
			for (k0 = -k; k0 < k; k0++) {
				Vector3D primePosition = initVector3D(2 * R * i, 2 * R * j, 2 * R * k0); 
				lattice.cubicSpace.A[i + m][j + n][k0 + k] = initCube3D(R, add(primePosition, position));
			}
		}
	}

	return lattice;
}

struct CubicLattice3D initInvLattice3D(double r, int mod, int color, Vector3D position, int m, int n, double invRad, Vector3D gamma) {
	
	int i, j;
	int sizeX = 2 * m;
	int sizeY = 2 * n;

	struct CubicLattice3D lattice = {
								r : r,
								position : position,
								mod : mod,
								color : color,
								sizeX : sizeX,
								sizeY : sizeY,
								i1 : 0,
								j1 : 0,
								k1 : 0
							};

	lattice.cubicSpace = creaMatrixCube(sizeX, sizeY);

	for (i = -m; i < m; i++) {
		for (j = -n; j < n; j++) {
			Vector3D primePosition = initVector3D(2 * r * i, 2 * r * j, 0); 
			lattice.cubicSpace.A[i + m][j + n] = initInversive3D(r, add(primePosition, position), invRad, gamma);
		}
	}

	return lattice;
}

void renderLattice(struct CubicLattice3D lattice, struct rotationMats U) {

	for (lattice.i1 = 0; lattice.i1 < lattice.sizeX; lattice.i1++) {
		for (lattice.j1 = 0; lattice.j1 < lattice.sizeY; lattice.j1++) {
			if (lattice.i1%2 == 0 && lattice.j1%2 == 0 || lattice.i1%2 != 0 && lattice.j1%2 != 0){
				renderCube3D(lattice.cubicSpace.A[lattice.i1][lattice.j1], U);
			}
		}
	}
}

void renderLattice3(struct CubicLattice3D3 lattice, struct rotationMats U, int mod, double bound) {

	for (lattice.i1 = 0; lattice.i1 < lattice.sizeX; lattice.i1++) {
		for (lattice.j1 = 0; lattice.j1 < lattice.sizeY; lattice.j1++) {
			for (lattice.k1 = 0; lattice.k1 < lattice.sizeZ; lattice.k1++)
				if ((lattice.i1 + lattice.j1 + lattice.k1)%1 == mod)
					if ((lattice.i1 + lattice.j1 + lattice.k1) > -1){
						if ((8 - bound) < norma(lattice.cubicSpace.A[lattice.i1][lattice.j1][lattice.k1].position) && norma(lattice.cubicSpace.A[lattice.i1][lattice.j1][lattice.k1].position) < 8) {
							renderCube3D(lattice.cubicSpace.A[lattice.i1][lattice.j1][lattice.k1], U);
						}
					}
		}
	}
}

void renderInvLattice(struct CubicLattice3D lattice, struct rotationMats U, Vector3D position) {

	for (lattice.i1 = 0; lattice.i1 < lattice.sizeX; lattice.i1++) {
		for (lattice.j1 = 0; lattice.j1 < lattice.sizeY; lattice.j1++) {
			if (lattice.i1%2 == 0 && lattice.j1%2 == 0 || lattice.i1%2 != 0 && lattice.j1%2 != 0)
				if (lattice.i1 != lattice.sizeX/2 && lattice.j1 != lattice.sizeY/2) {
					updateInvCube3D(&lattice.cubicSpace.A[lattice.i1][lattice.j1], lattice.cubicSpace.A[lattice.i1][lattice.j1].position, position);
					renderCube3D(lattice.cubicSpace.A[lattice.i1][lattice.j1], U);
				}
		}
	}
}

void moveMandelbrot(struct CubicLattice3D * lattice, double var) {

	for (lattice->i1 = 0; lattice->i1 < lattice->sizeX; lattice->i1++) {
		for (lattice->j1 = 0; lattice->j1 < lattice->sizeY; lattice->j1++) {
			if (lattice->code.A[lattice->i1][lattice->j1] == 200.0) {
				
				//lattice->cubicSpace.A[lattice->i1][lattice->j1].position.x += cos(var);
			}
    	}
	}	
}

void renderMandelBrot(struct CubicLattice3D lattice, struct rotationMats U, int mood1) {

	for (lattice.i1 = 0; lattice.i1 < lattice.sizeX; lattice.i1++) {
		for (lattice.j1 = 0; lattice.j1 < lattice.sizeY; lattice.j1++) {
			
			if (norma(lattice.cubicSpace.A[lattice.i1][lattice.j1].position) < 2.5) {
			//	rot3D(&U, lattice.cubicSpace.A[lattice.i1][lattice.j1].position);
			//	glPointSize(1.7);
    		//	glBegin(GL_POINTS);
    		//	glColor3f(0, 0, 0);
    		//	glVertex3f( U.aux.x, U.aux.y, U.aux.z);
    		//	glEnd();
			//}

				if (lattice.i1 < lattice.sizeX - 1 && lattice.j1 < lattice.sizeY-1) {

					double k;
					int mood = mood1;

					if ( (double)lattice.levels + 2 < lattice.code.A[lattice.i1][lattice.j1] && lattice.code.A[lattice.i1][lattice.j1] <= 2000) {
							renderFacet(lattice.latticeMesh.A[lattice.i1][lattice.j1], U, mood);
							renderFacet(lattice.latticeMesh2.A[lattice.i1][lattice.j1], U, mood);
						}

					for (k = 2; k < (double)lattice.levels + 1; k += 2) {
						mood += 1;
						if (k < lattice.code.A[lattice.i1][lattice.j1] && lattice.code.A[lattice.i1][lattice.j1] <= k + 2) {
							renderFacet(lattice.latticeMesh.A[lattice.i1][lattice.j1], U, mood);
							renderFacet(lattice.latticeMesh2.A[lattice.i1][lattice.j1], U, mood);
						}
					}
				}


			}
    	}
	}
}