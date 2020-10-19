

/*
  Buenas tardes profesor soy Anely Florez Suarez. Aquí le dare una breve explicación de como manejar los fractales.
  
  (0) Teclé n para cambiar al siguiente fractal.

  (i) Conjuntos de Mandelbrot y los dos ejemplos de Julia teclear "c" para que cambie de color.

  (ii) Para los otros fractales Koch, Dragon, Levy, Carpeta de menger y Triángulo de Sierspinski precione "i" para subir de iteración y "I" para baja de iteración.

  (iii) El Arbol de Pitagoras no se le puede subir de iteración pero hay una sorpresa hasta el final.

  (iv) A segurese de recorrer todos los fractales precionando "n" ya que estan en lista circular.
*/



#include <stdio.h>
#include <math.h>
#include <GL/glut.h>
#include "Vector3D.h"
#include "geometry.h"

GLfloat light_diffuse[] = {1.0, 1.0, 1.0, 1.0};
GLfloat light_position[] = {1.0, 1.0, 0.25, 0.0};

/*variables*/ 
int ciclo = 0;
int cicloSegund = 0;
int color = 0;
double count = 0.0;
Vector3D center;
Vector3D I;
Vector3D J;
Vector3D K;
Vector3D test0;
int iter = 5;

/*Estructuras Matematicas*/
struct rotationMats U;
struct CubicLattice3D lattice;
struct CubicLattice3D juliaLattice;
struct CubicLattice3D juliaLattice1;
struct KochCurve kochCurve;
struct DragonCurve dragonCurve;
struct DragonCurve LevyCurve;
struct MengerCarpet menger;
struct SierspinskiTriangle sier;

struct TreePythagoras tree;
struct TreePythagoras tree1;
struct TreePythagoras tree2;
struct TreePythagoras tree3;

void display(void);
void init(double theta);
void TimerFunction(int value);
void keyboard(unsigned char key, int x, int y);
void ProcessMenu(int value);

int main(int argc, char **argv)
{
  
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
  glutInitWindowSize(1300, 1300);
  glutCreateWindow(" ------- Fractals ------- ");
  ProcessMenu(1);
  init(count);

  glutDisplayFunc(display);
  glutKeyboardFunc(keyboard);
  glutTimerFunc(20, TimerFunction, 1);

  glutMainLoop();
  return 0;             /* ANSI C requires main to return int. */
}


///////////////////////////////////////////

void display(void)
{
  center = initVector3D(0, 0, 0);
  I = initVector3D(1, 0, 0);
  J = initVector3D(0, 1, 0);
  K = initVector3D(0, 0, 1);
  
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glClearColor(1.0, 1.0, 1.0, 1.0);

  if (ciclo == 0) {

    U = initRotation(count);
    printf("--------- > Inicializando Mandelbrot.\n");
    lattice = initMandelBrot(0.006, 2, 0, center, 250, 200, 0, 0);
    printf("--------- > Inicializando Julia 1.\n");
    juliaLattice = initMandelBrot(0.0045, 2, 0, center, 300, 200, 0.285, 0.01);
    printf("--------- > Inicializando Julia 2.\n");
    juliaLattice1 = initMandelBrot(0.0045, 2, 0, center, 300, 200, -0.74543, 0.11301);
    printf("--------- > Inicializando Curva de Koch.\n");
    initKochCurve(&kochCurve, initVector3D(0.2, 0.2, 0), add(initVector3D( 0.7, 0.7, 0), I), add(initVector3D( 0.7, 0.7, 0), J), 3);
    printf("--------- > Inicializando La Curva del Dragon.\n");
    initDragonCurve(&dragonCurve, initVector3D(-1.2,-1.2, 0), initVector3D(-0.2,-0.2, 0), 9);
    printf("--------- > Inicializando La Curva del Dragon.\n");
    initLevyCurve(&LevyCurve, initVector3D(-1.7, 1.7, 0), initVector3D(-0.8, 0.8, 0), 9);
    printf("--------- > Inicializando Carpeta de Menger.\n");
    initMengerCarpet(&menger, initVector3D(2.5, 2.5, 0), initVector3D(-2.5, 2.5, 0), initVector3D(-2.5,-2.5, 0), initVector3D(2.5,-2.5, 0), 5);    
    printf("--------- > Inicializando Triangulo de Sierspinski.\n");
    initSierspinskiTriangle(&sier, scale(4, initVector3D(-0.5, -0.5, 0)), scale(4, I), scale(4, J), 9);

    initTreePythagoras(&tree, initVector3D(1, 1, 0), initVector3D(-1, 1, 0), initVector3D(-1,-1, 0), initVector3D(1,-1, 0), 8);
    initTreePythagoras(&tree1, initVector3D(-1,-1, 0), initVector3D(1,-1, 0), initVector3D(1, 1, 0), initVector3D(-1, 1, 0), 8);
    initTreePythagoras(&tree2, initVector3D(-1, 1, 0), initVector3D(-1,-1, 0), initVector3D(1,-1, 0), initVector3D(1, 1, 0), 8);
    initTreePythagoras(&tree3, initVector3D(1,-1, 0), initVector3D(1, 1, 0), initVector3D(-1, 1, 0), initVector3D(-1,-1, 0), 8);
    printf("Datos preparados correctamente.\n");
  
  }

  updateRotation(&U, count);
  //moveMandelbrot(&lattice, count);
  if (cicloSegund%10 == 0) renderMandelBrot(lattice, U, color);
  if (cicloSegund%10 == 1) renderMandelBrot(juliaLattice, U, color);
  if (cicloSegund%10 == 2) renderMandelBrot(juliaLattice1, U, color);
  if (cicloSegund%10 == 3) {
    if (iter < 0) iter += 1;
    if (iter > 9) iter -= 1;
    renderKochCurve(kochCurve, U, iter);
    renderDragonCurve(dragonCurve, U, iter);
    renderDragonCurve(LevyCurve, U, iter);
  }
  if (cicloSegund%10 == 4) {
    if (iter < 0) iter = 0;
    if (iter > 5) iter = 5;
    renderMengerCarpet(menger, U, iter);
  }
  if (cicloSegund%10 == 5) {
    if (iter < 0) iter = 0;
    if (iter > 7) iter = 7;
    renderSierspinskiTriangle(sier, U, iter);
  }
  if (cicloSegund%10 == 6) {
    renderTreePythagoras(tree, U, iter);
  }

  if (cicloSegund%10 == 7) {
    renderTreePythagoras(tree, U, iter);
    renderTreePythagoras(tree1, U, iter);
  }

  if (cicloSegund%10 == 8) {
    renderTreePythagoras(tree, U, iter);
    renderTreePythagoras(tree1, U, iter);
    renderTreePythagoras(tree2, U, iter);
  }

  if (cicloSegund%10 == 9) {
    renderTreePythagoras(tree, U, iter);
    renderTreePythagoras(tree1, U, iter);
    renderTreePythagoras(tree2, U, iter);
    renderTreePythagoras(tree3, U, iter);
  }

  //renderMiniPythagoras(mini, U);
  //renderCube3D(cube, U);
  
  //renderTreePythagoras(tree, U, iter);
  //renderTreePythagoras(tree1, U, iter);
  //renderTreePythagoras(tree2, U, iter);
  //renderTreePythagoras(tree3, U, iter);
  glutSwapBuffers();
}

///////////////////////////////////////////

void init(double theta)
{
  /* Setup data. */
  GLfloat ambientLight[] = { 0.3f, 0.3f, 0.3f, 1.0f };
  GLfloat diffuseLight[] = { 0.7f, 0.7f, 0.7f, 1.0f };
  GLfloat specular[] = { 1.0f, 1.0f, 1.0f, 1.0f};
  GLfloat specref[] = { 1.0f, 1.0f, 1.0f, 1.0f };

  /* Enable a single OpenGL light. */
  glLightfv(GL_LIGHT0, GL_DIFFUSE, light_diffuse);
  glLightfv(GL_LIGHT0, GL_POSITION, light_position);
  glEnable(GL_LIGHT0);
  glEnable(GL_LIGHTING);

  /* Use depth buffering for hidden surface elimination. */
  glEnable(GL_DEPTH_TEST);
  glFrontFace(GL_CCW);
  //glEnable(GL_CULL_FACE);

  /*Enable color tracking*/
  glEnable(GL_COLOR_MATERIAL);

  /* Set material properties to follow glColor values*/
  glColorMaterial(GL_FRONT, GL_AMBIENT_AND_DIFFUSE);

  /*All materials have high shine*/
  glMaterialfv(GL_FRONT, GL_SPECULAR, specref);
  glMateriali(GL_FRONT, GL_SHININESS, 128);

  /* Setup the view of the cube. */
  glMatrixMode(GL_PROJECTION);
  gluPerspective( /* field of view in degree */ 40.0,
                              /* aspect ratio */ 1.0,
                                    /* Z near */ 0.5, 
                                    /* Z far */ 10000.0);
  glMatrixMode(GL_MODELVIEW);
  gluLookAt(  3,  3, 5,      /* eye is at (0,0,5) */
            0.0, 0.0, 0.0,      /* center is at (0,0,0) */
            0.0, 0.0, 1.0);      /* up is in positive Y direction */

  /* Adjust Board position to be asthetic angle. */
  //glTranslatef(0.0, 0.15, -0.0);
  glRotatef(90, 0.0, 0.0, 1.0);

  glEnable(GL_NORMALIZE);
}

void TimerFunction(int value) {

  count += 0.01;
  ciclo += 1;

  if (count > 2 * M_PI) count = 0;
  if (ciclo > 100) ciclo = 1;

  glutPostRedisplay();
  glutTimerFunc(20, TimerFunction, 1);
}

void keyboard(unsigned char key, int x, int y) {
  GLint params[2];

  switch (key) {
    case 'w': 
      ciclo += 1;
      break;

    case 'n': 
      cicloSegund += 1;
      break;

    case 'c': 
      color += 1;
      break;

    case 'i': 
      iter += 1;
      break;

    case 'I': 
      iter -= 1;
      break;
  }

  glutPostRedisplay();
}

void ProcessMenu(int value) {
  switch(value) {
    case 1:
      glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
      glEnable(GL_BLEND);
      glEnable(GL_POINT_SMOOTH);
      glHint(GL_POINT_SMOOTH_HINT, GL_NICEST);
      glEnable(GL_LINE_SMOOTH);
      glHint(GL_LINE_SMOOTH_HINT, GL_NICEST);
      glEnable(GL_POLYGON_SMOOTH);
      glHint(GL_POLYGON_SMOOTH_HINT, GL_NICEST);
      break;

    case 2:
      glDisable(GL_BLEND);
      glDisable(GL_LINE_SMOOTH);
      glDisable(GL_POINT_SMOOTH);
      glDisable(GL_POLYGON_SMOOTH);
      break;
    
    default:
      break;
  }

  glutPostRedisplay();
}
