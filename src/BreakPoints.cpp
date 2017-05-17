/**@file BreakPoints.cpp
 *
 */
#include "../include/BreakPoints.h"
#include <string>

struct BreakPoints::bp {
   Dwarf_addr m_bp_address;
   Dwarf_Unsigned m_bp_lineno;
   std::string m_src_filename;
   char m_instruction;
   enum bpStatus{
      bp_inactive=0,
      bp_active=1
   }
   bpStatus m_bp_status;
   unsigned int m_bp_id;
};


/** @brief Function to add a breakpoint to the container
 */
void BreakPoints::add(Dwarf_Unsigned lineno, Dwarf_Addr address, uint64_t instruction) {
   m_breakpoints.push_back(bp {address, lineno,"",instruction, bp_active,++m_s_number_of_idx});

}
