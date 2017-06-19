/** @file BreakPoints.h
 * @brief Theader containin the class definition for storing breakpoints
 */

#ifndef _BREAKPOINTS_H
#define _BREAKPOINTS_H 

#include <vector>
#include <stdint.h>
#include "libdwarf.h"
#include <string>
#include <unordered_map>
#include <forward_list>
#include "ProcessUtils.h"

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
private:
   struct bp {
      Dwarf_Addr m_bp_address;
      Dwarf_Unsigned m_bp_lineno;
      std::string m_src_filename;
      uint64_t m_instruction;
     bpStatus m_bp_status;
      unsigned int m_bp_id;
      // methods
      bp(Dwarf_Addr address,Dwarf_Unsigned lineno, std::string src, uint64_t instruction, bpStatus s,unsigned int id);
      void print() const;
   };
    typedef struct bp bp;
    std::unordered_map< uint64_t,const bp* > m_bp_map;
    std::forward_list<bp> m_breakpoints;
    std::vector<const bp* > m_bp_vector;
    unsigned int m_s_number_of_idx {0};

public:
  void add(Dwarf_Unsigned lineno, Dwarf_Addr address, uint64_t instruction);   
  /** @todo Add a version which takes in a src file name and default it to empty */
  void print();
  void print(Dwarf_Addr address) ;
  struct breakpoint {
     
     const bp* m_Impl;
     
     void print() const;
     uint64_t getAddress() const;
     uint64_t getAddress();
     uint64_t getInstruction();
     uint64_t getInstruction() const;
  };
  typedef struct breakpoint breakpoint;
  const breakpoint getBreakpointUsingIdx(unsigned int id);
  const breakpoint getCurrentBreakpoint(pid_t pid);
  const breakpoint getCurrentBreakpoint(pid_t pid) const;
  void continueAfterBreakPoint(pid_t pid) const;
  void continueAfterBreakPoint(pid_t pid);
};

#endif //_BREAKPOINTS_H
