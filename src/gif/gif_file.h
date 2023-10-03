/* FILE NAME   : gif_file.h
 * PROGRAMMER  : MZ2
 * LAST UPDATE : 01.09.2023
 * PURPOSE     : GIF compression project.
 *               GIF header file.
 */

#ifndef __gif_file_h_
#define __gif_file_h_

#include "def.h"

#include <iostream>
#include <vector>

/* Project namespace */
namespace gif
{
  struct DSCREEN
  {
    INT W;
    INT H;
    INT Background;
    BOOL GlobalTable;
    INT BitsPerPixel;
    INT GlobalTableSize;
  };

  struct DIMAGE
  {
    INT StartLeft;
    INT StartTop;
    INT W;
    INT H;
    BOOL LocalTable;
    INT Format;
    INT BitsPerPixel;
    INT LocalTableSize;
  };

  struct RGB
  {
    BYTE R;
    BYTE G;
    BYTE B;
  };

  /* GIF representation type */
  class gif
  {
  public:
    std::fstream File;
    DWORD Sign;
    DSCREEN Screen;
    DIMAGE ImageDesc; // LATER DO STOCK OF IMAGES
    RGB *GlobalTable = nullptr;
    RGB *LocalTable = nullptr;

    INT BitAccum = 0;
    INT BitPos = -1;

    /* Class constructor */ 
    gif( VOID )
    {

    }
    /* Class destructor */ 
    ~gif( VOID )
    {
    }

    INT ReadBit(VOID)
    {
      if (BitPos < 0)
      {
        INT x;

        File.read((CHAR *)&x, 1);

        if (x == EOF)
          return EOF;
        BitAccum = x, BitPos = 7;
      }
      return (BitAccum >> BitPos--) & 1;
    }

    /* Read as bits function.
     * ARGUMENTS:
     * 	  - Count of bits:
     *	      INT B;
     * RETURNS: Readed Data.
     */
    INT ReadAsBits( INT B )
    {
      INT x = 0;

      for (INT i = B - 1; i >= 0; i--)
      {
        INT z = ReadBit();
        x += z << i;
        if (z == EOF)
          return EOF;
      }
      return x;
    } /* End of 'ReadAsBits' function */

    VOID FRead( VOID *Ptr, INT Size )
    {
      File.read((CHAR *)Ptr, Size);
    }

    VOID Display( VOID )
    {
      std::cout << "Width: " << Screen.W << "\n";
      std::cout << "Height: " << Screen.H << "\n";
      std::cout << "Global table: " << Screen.GlobalTable << "\n";
      std::cout << "Bits per pixel: " << Screen.BitsPerPixel << "\n";
      std::cout << "Background: " << Screen.Background << "\n";

      if (GlobalTable != nullptr)
      {
        std::cout << "GLOBAL TABLE (R G B): \n";
        for (INT i = 0; i < Screen.GlobalTableSize; i++)
          std::cout << (INT)GlobalTable[i].R << " " << (INT)GlobalTable[i].G << " " << (INT)GlobalTable[i].B << "\n";
      }

      std::cout << "IMAGE \n";
      std::cout << "Start left: " << ImageDesc.StartLeft << "\n";
      std::cout << "Start top: " << ImageDesc.StartTop << "\n";
      std::cout << "Width: " << ImageDesc.W << "\n";
      std::cout << "Height: " << ImageDesc.H << "\n";
      std::cout << "Is Local table: " << ImageDesc.LocalTable << "\n";
      std::cout << "Local table size: " << ImageDesc.LocalTableSize << "\n";
      std::cout << "Format: " << ImageDesc.Format << "\n";
      std::cout << "Bits per pixels: " << ImageDesc.BitsPerPixel << "\n";
    }

    /* Load function.
     * ARGUMENTS:
     *	  - File In name:
     *	      const std::string &FnIn;
     * RETURNS: None.
     */
    VOID Load( const std::string &FnIn )
    {
      Free();
      File = std::fstream(FnIn, std::fstream::in | std::fstream::binary);

      FRead(&Sign, 6);

      if (Sign != *(DWORD *)"GIF89a")
        return;

      /* start reading screen descriptor */
      FRead(&Screen.W, 2);
      FRead(&Screen.H, 2);

      INT Data, tmp, x = 0;

      FRead(&Data, 1);

      tmp = (Data >> 7) & 1;
      if (tmp == 1)
        Screen.GlobalTable = TRUE;
      else
        Screen.GlobalTable = FALSE;


      x += ((Data >> 2) & 1) << 2;
      x += ((Data >> 1) & 1) << 1;
      x += ((Data >> 0) & 1) << 0;

      Screen.BitsPerPixel = x + 1;

      Screen.GlobalTableSize = pow(2, Screen.BitsPerPixel);

      FRead(&Screen.Background, 1);
      FRead(&x, 1);
      /* end reading screen descriptor */

      // load global table palette
      if (Screen.GlobalTable == TRUE)
      {
        GlobalTable = new RGB[Screen.GlobalTableSize];
        FRead(GlobalTable, Screen.GlobalTableSize * 3);
      }

      // check ',' symbol
      BYTE zap;
      FRead(&zap, 1);
      if (zap != ',')
        return;

      // Reaqd Image description
      FRead(&ImageDesc.StartLeft, 2);
      FRead(&ImageDesc.StartTop, 2);
      FRead(&ImageDesc.W, 2);
      FRead(&ImageDesc.H, 2);
      FRead(&Data, 1);

      tmp = (Data >> 7) & 1;
      if (tmp == 1)
        ImageDesc.LocalTable = TRUE;
      else
        ImageDesc.LocalTable = FALSE;

      ImageDesc.Format = (Data >> 6) & 1;

      x += ((Data >> 2) & 1) << 2;
      x += ((Data >> 1) & 1) << 1;
      x += ((Data >> 0) & 1) << 0;

      ImageDesc.BitsPerPixel = x + 1;
      ImageDesc.LocalTableSize = pow(2, ImageDesc.BitsPerPixel);

      if (ImageDesc.LocalTable == TRUE)
      {
        LocalTable = new RGB[ImageDesc.LocalTableSize];
        FRead(LocalTable, ImageDesc.LocalTableSize * 3);
      }

      // Choose palette
      RGB *Palette = GlobalTable;
      if (LocalTable != nullptr)
        Palette = LocalTable;

      /* DECOMPRESS */

      BYTE start_bits, cur_bits;
      FRead(&start_bits, 1);

     /* INT Cur, Next;
      INT code_clear = pow(2, start_bits);
      INT code_term = code_clear + 1;
      INT DictSize = code_term + 1;
      INT CurBit = start_bits + 1;
      INT i = 0, code;

      Cur = ReadAsBits(CurBit);
      if (Cur == CODE_TERM)
        return;
     // Out.put(Cur);

      while ((Next = ReadAsBits(CurBit)) != EOF)
      {
        if (DictSize >= pow(2, CurBit) - 1)
        {
          CurBit++;
          Next <<= 1;
          Next |= ReadBit();
        }

        if (Next == CODE_TERM)
          break;

        if (CurBit >= MAX_BITS)
          break;

        if (Next <= 255)
        {
          Dict[DictSize++] = { Cur, Next };
          //Out.put(Next);
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
            ;// Out.put(Stack[i]);
          Dict[DictSize++] = { Cur, code };
        }
        else
        {
          INT f = Cur;
          while (f > 257)
            f = Dict[f].P;
          Dict[DictSize++] = { Cur, f };

          i = 0;
          code = Next;

          while (code > 257)
          {
            Stack[i++] = Dict[code].S;
            code = Dict[code].P;
          }
          Stack[i++] = code;
          while (i--)
            ;//Out.put(Stack[i]);
        }

        Cur = Next;
      }
      */
      
    } /* End of 'Load' function */

    /* Free function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Free( VOID )
    {
      if (GlobalTable != nullptr)
        delete[] GlobalTable;
      if (LocalTable != nullptr)
        delete[] LocalTable;

      if (File.is_open())
        File.close();
    } /* End of 'Free' function */

  }; /* End of 'gif' class */
} /* end of 'gif' namespace */


#endif /* __gif_file_h_ */

/* END OF 'gif_file.h' FILE */