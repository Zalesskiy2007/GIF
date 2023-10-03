#include "gif.h"

#include <glut.h>

#define W 800
#define H 800

BYTE Frame[W * H * 3];

VOID PutPixel ( INT x, INT y, INT r, INT g, INT b )
{
  if (x >= 0 && x < W && y >= 0 && y < H)
  {
    Frame[y * 3 * W + x * 3] = b;
    Frame[y * 3 * W + x * 3 + 1] = g;
    Frame[y * 3 * W + x * 3 + 2] = r;
  }
}

VOID Clean( INT x, INT y, INT w, INT h )
{
  INT i, j;

  for (i = 0; i < h; i++)
    for (j = 0; j < w; j++)
      PutPixel(x + j, y + i, 0, 0, 0);
}


VOID Display( VOID )
{

  /* clear background */
  glClear(GL_COLOR_BUFFER_BIT);

  /* output frame buffer */
  glPixelZoom(3, -5);
  glRasterPos2d(-1, 1);

  glDrawPixels(W, H, GL_BGR_EXT, GL_UNSIGNED_BYTE, Frame);

  glFinish();
  glutSwapBuffers();
  glutPostRedisplay();
}

VOID Keyboard( BYTE Key, INT X, INT Y )
{
  if (Key == 27)
    exit(0);
}

INT main( VOID )
{
  gif::gif f;

  f.Load("img/M.gif");
  f.Display();
  for (INT i = 0; i < f.Screen.GlobalTableSize; i++)
    PutPixel(100 + i, 100, f.GlobalTable[i].R, f.GlobalTable[i].G, f.GlobalTable[i].B);
  f.Free();

  /* создание окна */
  glutInitDisplayMode(GLUT_RGB);
  glutInitWindowPosition(0, 0);
  glutInitWindowSize(W, H);
  glutCreateWindow("MZ2 CGSG");

  /* установка CALLBACKs */
  glutDisplayFunc(Display);
  glutKeyboardFunc(Keyboard);

  /* установка цвета фона */
  glClearColor(0.30, 0.47, 0.8, 1);

  /* уходим в вечный цикл */
  glutMainLoop();

  return 0;
}