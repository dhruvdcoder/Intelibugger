/** @file Tracee.h **/
#ifndef _TRACEE_H
#define _TRACEE_H

#include <sys/types.h> // for pid_t
#include <sys/ptrace.h> // for ptrace
#include <unistd.h> // for getpid
#include <iostream>
#include <string>
#include <stdio.h>
#include <errno.h> // for perror()
#include <vector>
#include "LineInfo.h"
#include "BreakPoints.h"
#include <fcntl.h> // for open()
#include <unistd.h> // for close()
#include "ProcessUtils.h"
//#define CLEARMASK 0xffffffffffffff00
//#define INT3 0xcc
enum ProcessStatus {
   NOT_STARTED=0,
   STOPPED_AT_FIRST_INST
};



class Tracee {
   private:
  pid_t m_pid;
  ProcessUtils::ExecvArgs process_args;
   ProcessStatus m_proc_status;   
   int m_file_descriptor; // TODO better handling of file desc in Tracee.
   Dwarf_Debug m_dwarf_dbg;
   Dwarf_Die m_dwarf_cu_die;
   LineInfo m_line_info;
   BreakPoints m_breakpoints; 

//   void continueAfterBreakPoint(
   public:
  //getters
  pid_t getid();
 //constructor
//   tracee(pid_t id):e_pid(id),e_name("unpecified name"),args(NULL) {}
   Tracee(std::string prog); 
// destructor : We need one now beacuse we are managing args using new
   //~Tracee(){}
   // utility functions
 //
    void load(); 
    void continueProc(); 
    //dwarf related
    void initDwarf();
    ProcessStatus getProcessStatus() const {return m_proc_status;}

    // BreakPoints related
    void addBreakPoint(Dwarf_Unsigned line_number,const std::string& src_file="");

     
};

#endif // _TRACEE_
