#include "gif.h"

INT main( VOID)
{
  gif::gif f;

  f.Load("img/M.gif");
  f.Display();
  f.Free();
  return 0;
}