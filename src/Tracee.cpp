/** @file Tracee.cpp
 */
#include "../include/Tracee.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <sys/wait.h>
#include <sys/signal.h> // for the definition of SIGTRAP and other tokens

/** The constructor
 */

Tracee::Tracee(std::string prog):m_pid(-1),m_proc_status(NOT_STARTED){
   using namespace std;
   std::istringstream iss(prog);
   iss >> m_name; //TODO: error checking
   copy(istream_iterator<string>(iss),istream_iterator<string>(),back_inserter(m_args_vector));
   char** temp_args = new char* [m_args_vector.size()+1]; // allocate an array of char* 
   int i=0;
   temp_args[m_args_vector.size()+1]=NULL;
   for(auto argi=m_args_vector.begin(); argi != m_args_vector.end(); ++argi){
      temp_args[i]=const_cast<char *>(argi->c_str()); // Potentially unsafe cast
   }
   m_args=static_cast<char*const*>(temp_args);
}

/** 
 * 
 * The destructor
 */

Tracee::~Tracee(){
      
      delete[] m_args;
   }



/** Start the Tracee
 */

void Tracee::start(){
   using namespace std;

   pid_t child_pid=fork();
   if (child_pid==0){//child
   
   // set the pid
      m_pid=getpid();
      std::cout << " Tracee process " << m_name <<" with pid" << static_cast<int>(m_pid) << " started" << std::endl;
      // Call ptrace with TRACEME to allow the parent process to trace this process
      if (ptrace(PTRACE_TRACEME,0,0,0)<0){
         perror("start_as_tracee :: ptrace");
         return;
      }
      // load the given program
      if(execv(m_name.c_str(),m_args)<0){
	      perror("execv ::");
	      return;
      }
   }
   else if (child_pid>0) {//parent
         int status;
         // wait for the child to stop on its first instruction after exec()
         wait(&status); 
         if(WIFSTOPPED(status)) {// true if the child was stopped due to a signal. Which should be the case here.
            if(SIGTRAP!=WSTOPSIG(status)) {
               cout<<"Child started. Stopped due to signal other than SIGTRAP"<<endl;
               return;
            }
            m_proc_status=STOPPED_AT_FIRST_INST;
            return;
         }
         else 
            cout<< "Child not stopped after loading" <<endl;
         //cout << "The child has stopped on first instruction" << endl;
         // ask the child to continue
         //ptrace(PTRACE_CONT,child_pid,NULL,0);
      }
      else {
         perror("fork");
         return;
      }

 }

/*! \brief Brief function description here
 *
 *  Detailed description
 *
 * \return Return parameter description
 */
void Tracee::continueProc() {
   ptrace(PTRACE_CONT,m_pid,NULL,0);
   perror("Tracee::continueProc");   
}


/*! \brief Brief function description here
 *
 *  Detailed description
 *  TODO Add test for this function.
 * \return Return parameter description
 */
void Tracee::initDwarf() {
   m_file_descriptor=open(m_name.c_str(),O_RDONLY); // open file readonly
   if(t_fd==-1){
      fprintf(stderr,"Error: Unable to open the file %s.",file);
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
      m_line_info.setDiePtr(t_cu_die);
     m_line_info.setLineContextAndBuffer(); 

}
