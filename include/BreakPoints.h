/** @file BreakPoints.h
 * @brief Theader containin the class definition for storing breakpoints
 */

#ifndef _BREAKPOINTS_H
#define _BREAKPOINTS_H 

#include <vector>
#include <stdint.h>

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
    struct bp; /*!< Struct nested inside the class. To not expose the impl and change it later. */
    std::vector<bp> m_breakpoints;
    static unsigned int m_s_number_of_idx;
};

#endif //_BREAKPOINTS_H
