/**@file LineInfo.h 
 * @brief contains the interface to map line number to address and vise-versa.
 */

#ifndef _LINEINFO_H
#define _LINEINFO_H

#include "libdwarf.h"
/** @brief Class to provide interface with libdwarf to map line number to address and vice-versa
 */
class LineInfo{
   private:
	   Dwarf_Die m_die_ptr;
	   Dwarf_Line* m_line_buffer_ptr;
	   Dwarf_Line_Context m_line_context; /**< This is typedef struct Dwarf_Line_Context_s* . It lets us access line information convinitently. */
	   Dwarf_Unsigned m_line_version;
	   Dwarf_Small m_table_count;
      Dwarf_Signed m_line_count;
      
         public:
	   // Constructors
      LineInfo();
	   int setDiePtr(Dwarf_Die die_ptr);
	   int setLineContextAndBuffer();
	  /** @brief Function to get the index of the Line in Line Buffer corresponding to the supplied source line. 
       */
      Dwarf_Unsigned getLineIndexInBuffer(Dwarf_Unsigned q_line_number);
      /** @brief Function which returns Address for a particular line of code.*/
      Dwarf_Addr getAddressFromLine(unsigned int line);
      

      // destructors
      ~LineInfo();
};

#endif // 
