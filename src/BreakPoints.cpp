/**@file BreakPoints.cpp
 *
 */
#include "../include/BreakPoints.h"
#include<iostream>
#include<exception>
#include<stdexcept>
#include<new>
#include <cassert>

constexpr unsigned int max_offset_after_int3 = 1;


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
   /// @todo Clean the sequence below. Use one try catch
   bool ok1 = m_bp_map.insert({address,new_bp}).second;
   if(ok1!=true)
      m_breakpoints.pop_front();
   try {
      std::vector<const bp* >::iterator at;
      at=m_bp_vector.begin() + m_s_number_of_idx-1; // 0-based vs 1-based id
      m_bp_vector.insert(at,new_bp);
   }
   catch (const std::bad_alloc& e){
      m_breakpoints.pop_front();
      m_bp_map.erase(address);
      /// @todo create a new exception of user defined type and throw it to indicate the failure to the client      
   }


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

/*! \brief Brief function description here
 *
 *  Detailed description
 *
 * \param Parameter Parameter description
 * \return Return parameter description
 */
const BreakPoints::breakpoint BreakPoints::getBreakpointUsingIdx(unsigned int id) {
   return breakpoint{m_bp_vector[id-1]}; /// @todo check for bounds error
   // 0-based idx base vs 1-based idx
}

/*! \brief Brief function description here
 *
 *  Detailed description
 *
 * \return Return parameter description
 */
void BreakPoints::breakpoint::print() const{
   m_Impl->print();
}

/*! \brief Brief function description here
 *
 *  Detailed description
 *
 * \param Parameter Parameter description
 * \return Return parameter description
 */
const BreakPoints::breakpoint BreakPoints::getCurrentBreakpoint(pid_t pid) const {
   uint64_t rip = ProcessUtils::getInstructionPointer(pid);
   const bp* pbp=nullptr;
   for (unsigned int ii=1; ii <= max_offset_after_int3;++ii){
      uint64_t instruction_address = rip - ii;
      try {
         pbp = m_bp_map.at(instruction_address);
      }
      catch (const std::out_of_range& e) {
         if ( ii ==max_offset_after_int3) // throw now
            throw ProcessUtils::ProcessException ("In BreakPoints::getCurrentBreakpoint : Could not find breakpoint.");
         continue; // retry
      }
      break; // break if try succeeds
   }
   assert(pbp != nullptr);
   return breakpoint{pbp};
}

/*! \brief Brief function description here
 *
 *  Detailed description
 *
 * \param Parameter Parameter description
 * \return Return parameter description
 */
const BreakPoints::breakpoint BreakPoints::getCurrentBreakpoint(pid_t pid) {
   const BreakPoints* cBP = static_cast<const BreakPoints* >(this);
   return cBP->getCurrentBreakpoint(pid);
}

/*! \brief Brief function description here
 *
 *  Detailed description
 *
 * \return Return parameter description
 */
uint64_t BreakPoints::breakpoint::getAddress() const {
   return m_Impl->m_bp_address;
}

/*! \brief Brief function description here
 *
 *  Detailed description
 *
 * \return Return parameter description
 */
uint64_t BreakPoints::breakpoint::getAddress()  {
   const BreakPoints::breakpoint* cbp = static_cast<const BreakPoints::breakpoint* >(this);
   return cbp->getAddress();
}

/*! \brief Brief function description here
 *
 *  Detailed description
 *
 * \return Return parameter description
 */
uint64_t BreakPoints::breakpoint::getInstruction() const {
   return m_Impl->m_instruction;
   
}


/*! \brief Brief function description here
 *
 *  Detailed description
 *
 * \param Parameter Parameter description
 * \return Return parameter description
 */
void BreakPoints::continueAfterBreakPoint(pid_t pid) const {
   const breakpoint bp = getCurrentBreakpoint(pid);
   uint64_t original_instruction = bp.getInstruction();
   uint64_t address = bp.getAddress();
   ProcessUtils::setInstruction(pid,address,original_instruction);
   ProcessUtils::setInstructionPointer(pid,address);
   ProcessUtils::singleStepChild(pid);
   ProcessUtils::parentWaitForSignalFromChild(SIGTRAP);
   ProcessUtils::setInstruction(pid,address,ProcessUtils::getInstructionWithTrap(original_instruction));
   ProcessUtils::continueChild(pid);
    
}
