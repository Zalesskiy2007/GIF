/*************************************************************
 * Copyright (C) 2023
 *    Computer Graphics Support Group of 30 Phys-Math Lyceum
 *************************************************************/

 /* FILE NAME   : def.h
  * PROGRAMMER  : MZ2.
  * LAST UPDATE : 01.09.2023.
  * PURPOSE     : GIF compression project.
  *               Default common definition module.
  *
  * No part of this file may be changed without agreement of
  * Computer Graphics Support Group of 30 Phys-Math Lyceum
  */

#ifndef __def_h_
#define __def_h_

#include <cassert>


#ifndef WIN32
#define WIN32
#include <commondf.h>
#undef WIN32
#else
#include <commondf.h>
#endif /* WIN32 */
#define BOOL bool

/* Debug memory allocation support */
#ifndef NDEBUG 
# define _CRTDBG_MAP_ALLOC
# include <crtdbg.h> 
# define SetDbgMemHooks() \
  _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
  _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))
static class __Dummy
{
public:
  /* Class constructor */
  __Dummy(VOID)
  {
    SetDbgMemHooks();
  } /* End of '__Dummy' constructor */
} __oops;

#endif /* _DEBUG */ 

#ifdef _DEBUG 
# ifdef _CRTDBG_MAP_ALLOC 
#   define new new(_NORMAL_BLOCK, __FILE__, __LINE__) 
# endif /* _CRTDBG_MAP_ALLOC */ 
#endif /* _DEBUG */

/* Base large integer types */
typedef unsigned long long UINT64;
typedef long long INT64;

#define MAX_BITS 27
#define MAX (1 << MAX_BITS)
#define START_BITS 8
#define CODE_CLEAR (1 << START_BITS)
#define CODE_TERM (CODE_CLEAR + 1)

#include <string>
#include <fstream>

#endif /* __def_h_ */

/* END OF 'def.h' FILE */