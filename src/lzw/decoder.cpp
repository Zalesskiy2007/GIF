/* FILE NAME   : decoder.cpp
 * PROGRAMMER  : MZ2
 * LAST UPDATE : 01.09.2023
 * PURPOSE     : GIF compression project.
 *               Decoder header file.
 */

#include "gif.h"

#include <map>

/* Open function.
 * ARGUMENTS:
 *	  - File In name:
 *	      const std::string &FnIn;
 * 	  - File Out name:
 *	      const std::string &FnOut;
 * RETURNS:
 *	(BOOL) success of opening.
 */
BOOL gif::decoder::Open( const std::string &FnIn, const std::string &FnOut )
{
  Close();

  BOOL i = In.Open(FnIn);
  Out = std::fstream(FnOut, std::fstream::out | std::fstream::binary);

  return (i && (bool)Out);
} /* End of 'Open' function */

/* Read as bits function.
 * ARGUMENTS:
 * 	  - Count of bits:
 *	      INT B;
 * RETURNS: Readed Data.
 */
INT gif::decoder::ReadAsBits( INT B )
{
  INT x = 0;

  for (INT i = B - 1; i >= 0; i--)
  {
    INT z = In.ReadBit();
    x += z << i;
    if (z == EOF)
      return EOF;
  }
  return x;
} /* End of 'ReadAsBits' function */

/* Close function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID gif::decoder::Close( VOID )
{
  In.Close();
  if (Out.is_open())
    Out.close();
} /* End of 'Close' function */

struct _dict
{
  INT P, S;
} Dict[MAX];

BYTE Stack[MAX];

/* Decompress function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID gif::decoder::Decompress( VOID )
{
  INT Cur, Next;
  INT DictSize = 258;
  INT CurBit = 9;
  INT i = 0, code;
  
  Cur = ReadAsBits(CurBit);
  if (Cur == CODE_TERM)
    return;
  Out.put(Cur);

  while ((Next = ReadAsBits(CurBit)) != EOF)
  {
    if (DictSize >= pow(2, CurBit) - 1)
    {
      CurBit++;
      Next <<= 1;
      Next |= In.ReadBit();
    }

    if (Next == CODE_TERM)
      break;
    
    if (CurBit >= MAX_BITS)
      break;

    if (Next <= 255)
    {
      Dict[DictSize++] = {Cur, Next};
      Out.put(Next);
    }
    else if (Next < DictSize)
    {
      i = 0;
      code = Next;

      while (code > 257)
      {
        Stack[i++] = Dict[code].S;
        code = Dict[code].P;
      }
      Stack[i++] = code;
      while (i--)
        Out.put(Stack[i]);
      Dict[DictSize++] = {Cur, code};
    }
    else 
    {
      INT f = Cur;
      while (f > 257)
        f = Dict[f].P;
      Dict[DictSize++] = {Cur, f};

      i = 0;
      code = Next;

      while (code > 257)
      {
        Stack[i++] = Dict[code].S;
        code = Dict[code].P;
      }
      Stack[i++] = code;
      while (i--)
        Out.put(Stack[i]);
    }

    Cur = Next;
  }
} /* End of 'Decompress' function */

/* END OF 'decoder.cpp' FILE */