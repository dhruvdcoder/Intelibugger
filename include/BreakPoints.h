/** @file BreakPoints.h
 * @brief Theader containin the class definition for storing breakpoints
 */

#ifndef _BREAKPOINTS_H
#define _BREAKPOINTS_H 

#include <vector>
#include <stdint.h>
#include "libdwarf.h"
#include <string>

enum bpStatus{
   bp_inactive=0,
   bp_active=1
};
 



/*! \class BreakPoints
 *  \brief The class works as a container for the breakpoints and hides the internal implentation of the storage.
 *
 *  Detailed description
 */
class BreakPoints
{
public:
  void add(Dwarf_Unsigned lineno, Dwarf_Addr address, uint64_t instruction);   

private:
   struct bp {
      Dwarf_Addr m_bp_address;
      Dwarf_Unsigned m_bp_lineno;
      std::string m_src_filename;
      uint64_t m_instruction;
     bpStatus m_bp_status;
      unsigned int m_bp_id;
   };
    std::vector<bp> m_breakpoints;
    static unsigned int m_s_number_of_idx;
};

#endif //_BREAKPOINTS_H
