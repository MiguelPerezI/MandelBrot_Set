#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include <stdbool.h>
#include <stdarg.h>
#include "Vector3D.h"
#include "Matrix.h"

#ifndef _GEOMETRY_
#define _GEOMETRY_

#ifdef __cplusplus
extern "C" {
#endif

	struct Vector3DList{
		int n;
		Vector3D * v;
	};

	struct Vector3DList startVectorSpace(int n);
	struct Vector3DList VectorSpace(int count, ... );
	void printVectorList(struct Vector3DList lista);

	struct Simplex{
		int dim;
		struct Vector3DList vertexSpace;
		MATRIZ Adj;
		Vector3D normal;
	};

	struct Simplex startSimplex(int dim, struct Vector3DList lista);
	struct Simplex SimplexND(int dim_plus_one, ... );
	struct Simplex facet(Vector3D a, Vector3D b, Vector3D c);
	void updateFacet(struct Simplex * Facet, Vector3D new_a, Vector3D new_b, Vector3D new_c);
	void renderFacet(struct Simplex sim, struct rotationMats U, int mod);
	void printSimplexInfo(struct Simplex sim);

	typedef struct _matrixMesh_{
		int m, n;
		struct Simplex **A;
	} MatrixMesh;

	MatrixMesh creaMatrixMesh(int m, int n);
	void printMatrixMesh(MatrixMesh mesh);

	

	struct Arista { // Will only work if its vectors are in the XY plane.

		Vector3D base;
		Vector3D head;
		struct Vector3DList arrow;
		MatrixMesh mesh;

	};

	void initArista(struct Arista * arista, Vector3D a, Vector3D b, double width);
	void renderArista(struct Arista arista, struct rotationMats U);

	typedef struct _matrixArista_{
		int m, n;
		struct Arista **A;
	} MatrixArista;

	MatrixArista creaMatrixArista(int m, int n);

	typedef struct _MatrixAristaList_{
		int n;
		MatrixArista * m;
	} MatrixAristaList;

	/////////////////////////

	//	  KOCH UTILITIES

	/////////////////////////

	MatrixAristaList initMatrixAristaList(int n);

	struct KochArista {

		struct Vector3DList points;
		MatrixArista aristas;
		double d;

	};

	void initKochArista(struct KochArista * kochArista, Vector3D a, Vector3D b, double width);
	void renderKochArista(struct KochArista kochArista, struct rotationMats U);

	struct KochCurve {
		int order;
		Vector3D point0;
		Vector3D point1;
		Vector3D point2;
		MatrixAristaList AristaList;
	};

	void initKochCurve (struct KochCurve * koch, Vector3D a, Vector3D b, Vector3D c, int n);
	void renderKochCurve(struct KochCurve koch, struct rotationMats U, int iter);

	////////////////////////

	//  DRAGON UTILITIES

	////////////////////////

	struct DragonArista {

		struct Vector3DList points;
		MatrixArista aristas;

	};   

	void initDragonArista(struct DragonArista * dragonArista, Vector3D a, Vector3D b, bool T);
	void renderDragonArista(struct DragonArista dragonArista, struct rotationMats U);

	struct DragonCurve {
		int order;
		Vector3D point0;
		Vector3D point1;
		MatrixAristaList AristaList;
	};

	void initDragonCurve(struct DragonCurve * dragon, Vector3D a, Vector3D b, int n);
	void initLevyCurve (struct DragonCurve * dragon, Vector3D a, Vector3D b, int n);
	void renderDragonCurve(struct DragonCurve dragon, struct rotationMats U, int iter);

	////////////////////////

	//  CARPET UTILITIES

	////////////////////////

	struct Square {

		struct Vector3DList square;
		MatrixMesh mesh;

	};

	void initSquare(struct Square * square, Vector3D a, Vector3D b, Vector3D c, Vector3D d);
	void renderSquare(struct Square cuadro, struct rotationMats U);
	struct Square reflectSquare(struct Square mini, Vector3D auxPoint);
	struct Square dilateSquare(struct Square mini, Vector3D auxPoint, double d);

	typedef struct _matrixSquare_{
		int m, n;
		struct Square **A;
	} MatrixSquare;

	typedef struct _MatrixSquareList_{
		int n;
		MatrixSquare * m;
	} MatrixSquareList;

	MatrixSquare creaMatrixSquare(int m, int n);
	MatrixSquareList initMatrixSquareList(int n);

	struct MiniCarpet {

		struct Vector3DList mainPoints;
		MatrixSquare cuadritos;

	};

	void initMiniCarpet(struct MiniCarpet * mini, Vector3D a, Vector3D b, Vector3D c, Vector3D d);
	void renderMiniCarpet(struct MiniCarpet mini, struct rotationMats U);
	
	struct MengerCarpet{
		int order;
		Vector3D point0;
		Vector3D point1;
		Vector3D point2;
		Vector3D point3;
		MatrixSquareList SquareList;
	};

	void initMengerCarpet(struct MengerCarpet * menger, Vector3D a, Vector3D b, Vector3D c, Vector3D d, int n);
	void renderMengerCarpet(struct MengerCarpet menger, struct rotationMats U, int iter);

	struct MiniPythagoras {

		struct Vector3DList mainPoints;
		MatrixSquare cuadritos;

	};

	void initMiniPythagoras(struct MiniPythagoras * mini, Vector3D a, Vector3D b, Vector3D c, Vector3D d);
	void renderMiniPythagoras(struct MiniPythagoras mini, struct rotationMats U);
	
	struct TreePythagoras{
		int order;
		Vector3D point0;
		Vector3D point1;
		Vector3D point2;
		Vector3D point3;
		MatrixSquareList SquareList;
	};

	void initTreePythagoras(struct TreePythagoras * tree, Vector3D a, Vector3D b, Vector3D c, Vector3D d, int n);
	void renderTreePythagoras(struct TreePythagoras Tree, struct rotationMats U, int iter);


	////////////////////////

	//  Sierspinski UTILITIES

	////////////////////////

	struct Simplex dilateSimplex(struct Simplex mini, Vector3D auxPoint, double d);

	typedef struct _MatrixMeshList_{
		int n;
		MatrixMesh * m;
	} MatrixMeshList;

	MatrixMeshList initMatrixMeshList(int n);

	struct MiniSierspinski {

		struct Vector3DList mainPoints;
		MatrixMesh triangulitos;

	};

	void initMiniSierspinski(struct MiniSierspinski * mini, Vector3D a, Vector3D b, Vector3D c);
	void renderMiniSierspinski(struct MiniSierspinski mini, struct rotationMats U);

	struct SierspinskiTriangle {
		int order;
		Vector3D point0;
		Vector3D point1;
		Vector3D point2;
		MatrixMeshList FacetList;
	};
	
	void initSierspinskiTriangle(struct SierspinskiTriangle * mini, Vector3D a, Vector3D b, Vector3D c, int n);
	void renderSierspinskiTriangle(struct SierspinskiTriangle mini, struct rotationMats U, int iter);

	//////////////////////

	//	SIMPLEX RING

	//////////////////////

	struct SimplexRing {

		struct Vector3DList outer;
		struct Vector3DList inner;
		struct Vector3DList a;
		int disable;
		Vector3D centerMass;
		MatrixMesh mesh;
	};

	typedef struct _matrixSimplexRing_{
		int m, n;
		struct SimplexRing **A;
	} MatrixSimplexRing;

	MatrixSimplexRing creaMatrixSimplexRing(int m, int n);

	Vector3D centerMass(struct Vector3DList set);
	void initSimplexRing(struct SimplexRing * sim, Vector3D a, Vector3D b, Vector3D c, double lambda);
	void renderSimplexRing(struct SimplexRing sim, struct rotationMats U, int color);
	struct SimplexRing reflectSimplexRing(struct SimplexRing sim, Vector3D reflectionPoint);
	struct SimplexRing dilateSimplexRing(struct SimplexRing sim, Vector3D dilationPoint, double lambda);
	struct SimplexRing reflectAndDilate(struct SimplexRing sim, Vector3D actionPoint, double lambda);

	struct flake {

		MatrixSimplexRing S;
		struct SimplexRing mainSimplex;
		int id;
		bool type;
	};

	void initFlake(struct flake * star, struct SimplexRing sim);
	void updateFlakeType(struct flake * star, int type);
	void renderFlake(struct flake star, struct rotationMats U);

	typedef struct _matrixFlake_{
		int m, n;
		struct flake **A;
	} MatrixFlake;

	MatrixFlake creaMatrixFlake(int m, int n);

	typedef struct _matrixList_{
		int n;
		MatrixFlake * m;
	} MatrixList;

	MatrixList initMatrixList(int n);

	struct KochFractal2D {
		
		MatrixFlake pieces;
		int order;
		struct SimplexRing mainSimplex;
		MatrixList pieceCollector;
		
	};

	void initKochFractal2D(struct KochFractal2D * curve, struct SimplexRing sim, int n);
	void renderKochFractal2D(struct KochFractal2D koch, struct rotationMats U);

/////////////////////

//	PYTHAGORAS TREE

/////////////////////

	struct Cube3D {
		double r;
		Vector3D position;
		struct Vector3DList vertices;
		MatrixMesh mesh;
		double invRad;
		Vector3D gamma;
		int counter;

	};

	struct Cube3D initCube3D(double r, Vector3D position);
	void printCube3D(struct Cube3D cube);
	void renderCube3D(struct Cube3D cube, struct rotationMats U);
	void updatedCube3D(struct Cube3D cube, struct rotationMats U, Vector3D position);

	struct Cube3D initInversive3D(double r, Vector3D position, double invRad, Vector3D gamma);

	typedef struct _matrixCube_{
		int m, n;
		struct Cube3D **A;
	} MatrixCube;

	MatrixCube creaMatrixCube(int m, int n);

	typedef struct _MatrixCubeList_{
		int n;
		MatrixCube * m;
	} MatrixCubeList;

	MatrixCubeList initMatrixCubeList(int n);

	typedef struct _matrixCube3_{
		int m, n, k;
		struct Cube3D ***A;
	} MatrixCube3;

	MatrixCube3 creaMatrix3Cube(int m, int n, int k);

	struct CubicLattice3D {
		MatrixCube cubicSpace;
		double r;
		Vector3D position;
		int mod;
		int color;
		int sizeX;
		int sizeY;
		int sizeZ;
		MATRIZ code;
		MatrixMesh latticeMesh;
		MatrixMesh latticeMesh2;
		Vector3D primePosition;
		int levels;
		int i1;
		int j1;
		int k1;
	};

	struct CubicLattice3D initCubicLattice3D(double r, int mod, int color, Vector3D position, int m, int n);
	struct CubicLattice3D initMandelBrot(double r, int mod, int color, Vector3D position, int m, int n, double re, double img);
	struct CubicLattice3D initInvLattice3D(double r, int mod, int color, Vector3D position, int m, int n, double invRad, Vector3D gamma);
	void renderLattice(struct CubicLattice3D lattice, struct rotationMats U);
	void renderInvLattice(struct CubicLattice3D lattice, struct rotationMats U, Vector3D position);
	void renderMandelBrot(struct CubicLattice3D lattice, struct rotationMats U, int mood1);
	
	struct CubicLattice3D3 {
		MatrixCube3 cubicSpace;
		double r;
		Vector3D position;
		int mod;
		int color;
		int sizeX;
		int sizeY;
		int sizeZ;
		int i1;
		int j1;
		int k1;
	};

	struct Lattice3D {
		MatrixCube * cubicSpace;
		double r;
		Vector3D position;
		int mod;
		int color;
		int sizeX;
		int sizeY;
		int sizeZ;
		int i1;
		int j1;
		int k1;
	};

	struct CubicLattice3D3 initCubicLattice3D3(double r, int mod, int color, Vector3D position, int m, int n, int k);
	void renderLattice3(struct CubicLattice3D3 lattice, struct rotationMats U, int mod, double bound);

#ifdef __cplusplus
}
#endif

#endif