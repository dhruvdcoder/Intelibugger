/** @file Tracee.cpp
 */
#include "../include/Tracee.h"
#include "../include/ProcessUtils.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <sys/wait.h>
#include <sys/signal.h> // for the definition of SIGTRAP and other tokens
#include "../include/Error.h"
/** /brief Note that this constructor can throw as the initialization of process_args involves a "new" call.
 */
 
Tracee::Tracee(std::string prog):m_pid(-1),process_args(prog),m_proc_status(NOT_STARTED){
      
}
 
/** 
 * 
 * The destructor
 */
/*
Tracee::~Tracee(){
      
      delete[] m_args;
   }
   */
/*! \brief Brief function description here
 *
 *  Detailed description
 *
 * \return Return parameter description
 */
pid_t Tracee::getid() {
   return m_pid;
}


/** Start the Tracee
 */

void Tracee::load(){
   using namespace std;
   using namespace ProcessUtils;
   pid_t child_pid=createChild();
   try{
      if (child_pid==0){//child
         // Call ptrace with TRACEME to allow the parent process to trace this process
         traceMe();
         // load the given program
         loadUsingExecv(process_args);
      }
      else {//parent
            m_pid=child_pid;
            // wait for the child to recieve SIGTRAP and stop
            parentWaitForSignalFromChild(SIGTRAP);
      }
   }
   catch(const ProcessException& e){
      cout<<e.what()<<endl;
   }
         
 }

/*! \brief Brief function description here
 *
 *  Detailed description
 *
 * \return Return parameter description
 */
void Tracee::continueProc() {
   ProcessUtils::continueChild(m_pid); 
}


/*! \brief Brief function description here
 *
 *  Detailed description
 *  TODO Add test for this function.
 * \return Return parameter description
 */
void Tracee::initDwarf() {
   m_file_descriptor=open(process_args.getName().c_str(),O_RDONLY); // open file readonly
   if(m_file_descriptor==-1){
      fprintf(stderr,"Error: Unable to open the file %s.",process_args.getName().c_str());
      exit(1);
   }
   
   // Get the CU DIE
   // 1. Get the header for the compilation unit ( here assuming that we have only one CU
   // 2. Get the CU DIE by using dwarf_siblingof_b() with NULL.
   Dwarf_Error err;
      int init_res=dwarf_init_b(
            m_file_descriptor,
            DW_DLC_READ,
            0,
            simple_error_handler,
            nullptr,
            &m_dwarf_dbg,
            &err);
      if(init_res!=DW_DLV_OK){
         fprintf(stderr, "Error in Tracee::initDwarf(). In the call to dwarf_init_b");
         exit(1);
      }
      Dwarf_Unsigned cu_header_length,abbrev_offset, typeoffset, next_cu_header;
      Dwarf_Half version_stamp,address_size,offset_size,extension_size,header_cu_type;
      Dwarf_Sig8 sig;

      int head_res=dwarf_next_cu_header_d(
            m_dwarf_dbg,
            true,
            &cu_header_length,
            &version_stamp,
            &abbrev_offset,
            &address_size,
            &offset_size,
            &extension_size,
            &sig,
            &typeoffset,
            &next_cu_header,
            &header_cu_type,
            &err);
      if(head_res!=DW_DLV_OK){
         fprintf(stderr, "Error: %s, in Tracee::initDwarf(). Executing dwarf_next_cu_header_d",dwarf_errmsg(err));
         exit(1);
      }



      int sib_res=dwarf_siblingof_b(
            m_dwarf_dbg,
            NULL,
            true,
            &m_dwarf_cu_die,
            &err);
       if(sib_res!=DW_DLV_OK){
         fprintf(stderr, "Error: %s, in Tracee::initDwarf(). Executing dwarf_siblingof_b",dwarf_errmsg(err));
         exit(1);
      }



      // Initialize the LineInfo Class
      m_line_info.setDiePtr(m_dwarf_cu_die);
     m_line_info.setLineContextAndBuffer(); 

}


/*! \brief Brief function description here
 *
 *  Detailed description
 *
 * \param Parameter Parameter description
 * \param Parameter Parameter description
 * \return Return parameter description
 */
void Tracee::addBreakPoint(Dwarf_Unsigned line_number,const std::string& file_name/* ="" */) {
   using namespace ProcessUtils;
   // @todo Put try-catch to catch if dwarf data to read the file is not initialized
   // get the address of the instruction 
   Dwarf_Addr inst_address=m_line_info.getAddressFromLine(line_number);
   // get the instruction using ptrace. 
   uint64_t instruction=getInstruction(m_pid, inst_address);
   // Write the trap instruction at the address
   uint64_t instruction_with_trap = getInstructionWithTrap(instruction);
   setInstruction(m_pid,inst_address, instruction_with_trap);
   m_breakpoints.add(line_number, m_line_info.getAddressFromLine(line_number), instruction);
   
}

/*! \brief Brief function description here
 *
 *  Detailed description
 *
 * \return Return parameter description
 */
const BreakPoints& Tracee::getBreakPoints() {
   return m_breakpoints;
}
