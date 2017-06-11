/**@file BreakPoints.cpp
 *
 */
#include "../include/BreakPoints.h"



unsigned int BreakPoints::m_s_number_of_idx; // definition of the static member

/** @brief Function to add a breakpoint to the container
 */
void BreakPoints::add(Dwarf_Unsigned lineno, Dwarf_Addr address, uint64_t instruction) {
   m_breakpoints.push_back(bp {address, lineno,"",instruction, bp_active,++m_s_number_of_idx});
}

