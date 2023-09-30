/* FILE NAME   : read.h
 * PROGRAMMER  : MZ2
 * LAST UPDATE : 01.09.2023
 * PURPOSE     : GIF compression project.
 *               Read header file.
 */

#ifndef __read_h_
#define __read_h_

#include "def.h"

/* Project namespace */
namespace gif
{
  /* Read class */
  class read
  {
  private:
    INT BitAccum, BitPos;
    std::fstream File;
  public:
    /* Class constructor */ 
    read( VOID ) : BitAccum(0), BitPos(-1)
    {
    }
    /* Class destructor */ 
    ~read( VOID )
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
      BitPos = -1;
      File = std::fstream(Fn, std::fstream::in | std::fstream::binary);

      return (bool)File;
    } /* End of 'Open' function */

    /* Close function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Close( VOID )
    {
      if (File.is_open())
        File.close();
    } /* End of 'Close' function */
    
    /* Read bit function.
     * ARGUMENTS: None.
     * RETURNS:
     *   (INT) Readed bit.
     */
    INT ReadBit( VOID )
    {
      if (BitPos < 0)
      {
        INT x = File.get();

        if (x == EOF)
          return EOF;
        BitAccum = x, BitPos = 7;
      }
      return (BitAccum >> BitPos--) & 1;
    }
  }; /* End of 'read' class */
} /* end of 'gif' namespace */


#endif /* __read_h_ */

/* END OF 'read.h' FILE */