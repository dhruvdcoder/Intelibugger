/**@file BreakPoints.cpp
 *
 */
#include "../include/BreakPoints.h"
#include<iostream>


unsigned int BreakPoints::m_s_number_of_idx; // definition of the static member


/*! \brief Brief function description here
 *
 *  Detailed description
 *
 * \return Return parameter description
 */
BreakPoints::bp::bp(Dwarf_Addr address,Dwarf_Unsigned lineno, std::string src, uint64_t instruction, bpStatus s,unsigned int id) :m_bp_address(address),m_bp_lineno(lineno),m_src_filename(src),m_instruction(instruction),m_bp_status(s),m_bp_id(id){
   
}

/** @brief Function to add a breakpoint to the container
 * @todo Check if the key already exists 
 *
 * The breakpoints are stored in a forward_list. To query the breakpoints using the address of the instruction
 * an unordered_map is used with key=address and value=pointer to the bp of the bp in the vector.
 * This arrangement is used instead of storing the bps directly as the value of the unordered_map becasue
 * we want to be able to go through the breakpoints sequentially while bulk printing operation.
 */
void BreakPoints::add(Dwarf_Unsigned lineno, Dwarf_Addr address, uint64_t instruction) {
   m_breakpoints.emplace_front(address, lineno,"",instruction, bp_active,++m_s_number_of_idx);
   const bp* new_bp=&(m_breakpoints.front());
   // try inserting the ref into the map. Will fail if the key already exists. In that case remove the 
   // freshly inserted element from the list as well and bail 
   bool ok = m_bp_map.insert({address,new_bp}).second;
   if(ok!=true)
      m_breakpoints.pop_front();

}

/*! \brief Brief function description here
 *
 *  Detailed description
 *
 * \return Return parameter description
 */
void BreakPoints::bp::print() const {
   using namespace std;
   cout << "Breakpoint " <<m_bp_id<< " at line number "<<m_bp_lineno<<" ."<<endl;
}

/*! \brief Brief function description here
 *
 *  Detailed description
 *
 * \return Return parameter description
 */
void BreakPoints::print()  {
   
   for(auto it = m_breakpoints.begin(); it !=m_breakpoints.end(); ++it)
      it->print();
}

/*! \brief Brief function description here
 *
 *  Detailed description
 *
 * \param Parameter Parameter description
 * \return Return parameter description
 */
void BreakPoints::print(Dwarf_Addr address)  {
   m_bp_map[address]->print();
}
