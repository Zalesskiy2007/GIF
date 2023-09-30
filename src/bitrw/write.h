/* FILE NAME   : write.h
 * PROGRAMMER  : MZ2
 * LAST UPDATE : 01.09.2023
 * PURPOSE     : GIF compression project.
 *               Write header file.
 */

#ifndef __write_h_
#define __write_h_

#include "def.h"

/* Project namespace */
namespace gif
{
  /* Write class */
  class write
  {
  private:
    INT BitAccum, BitPos;
    std::fstream File;
  public:
    /* Class constructor */ 
    write( VOID ) : BitAccum(0), BitPos(7)
    {
    }
    /* Class destructor */ 
    ~write( VOID )
    {
    }

    /* Open function.
     * ARGUMENTS:
     *	  - File name:
     *	      const std::string &Fn;
     * RETURNS:
     *	(BOOL) success of opening.
     */
    BOOL Open( const std::string &Fn )
    {
      Close();

      BitAccum = 0;
      BitPos = 7;
      File = std::fstream(Fn, std::fstream::out | std::fstream::binary);

      return (BOOL)File;
    } /* End of 'Open' function */

    /* Close function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Close( VOID )
    {
      if (File.is_open())
      {
	WriteBitClose();
	File.close();
      }
    } /* End of 'Close' function */

    /* Write bit function.
     * ARGUMENTS:
     *	  - Bit to write:
     *	      INT Bit;
     * RETURNS: None.
     */
    VOID WriteBit( INT Bit )
    {
      BitAccum |= Bit << BitPos--; 
      if (BitPos < 0)
      {
	File.put(BitAccum);
	BitAccum = 0;
	BitPos = 7;
      }
    } /* End of 'WriteBit' function */

    /* Write bit close function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID WriteBitClose( VOID )
    {
      if (BitPos < 7)
	File.put(BitAccum); 
    } /* End of 'WriteBitClose' function */

  }; /* End of 'write' class */
} /* end of 'gif' namespace */


#endif /* __write_h_ */

/* END OF 'write.h' FILE */