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
}
