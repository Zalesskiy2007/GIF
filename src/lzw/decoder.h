/* FILE NAME   : decoder.h
 * PROGRAMMER  : MZ2
 * LAST UPDATE : 01.09.2023
 * PURPOSE     : GIF compression project.
 *               Decoder header file.
 */

#ifndef __decoder_h_
#define __decoder_h_

#include "def.h"
#include "bitrw/read.h"
#include "bitrw/write.h"
#include <iostream>

#include <map>

/* Project namespace */
namespace gif
{
  /* Decoder class */
  class decoder
  {
  private:
    std::fstream Out;
    read In;
  public:
    /* Class constructor */ 
    decoder( VOID )
    {

    }
    /* Class destructor */ 
    ~decoder( VOID )
    {
    }

    /* Open function.
     * ARGUMENTS:
     *	  - File In name:
     *	      const std::string &FnIn;
     * 	  - File Out name:
     *	      const std::string &FnOut;
     * RETURNS:
     *	(BOOL) success of opening.
     */
    BOOL Open( const std::string &FnIn, const std::string &FnOut );

    /* Read as bits function.
     * ARGUMENTS:
     * 	  - Count of bits:
     *	      INT B;
     * RETURNS: Readed Data.
     */
    INT ReadAsBits( INT B );

    /* Decompress function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Decompress( VOID );

    /* Close function.
     * ARGUMENTS: None.
     * RETURNS: None.
     */
    VOID Close( VOID );


  }; /* End of 'decoder' class */
} /* end of 'gif' namespace */


#endif /* __decoder_h_ */

/* END OF 'decoder.h' FILE */