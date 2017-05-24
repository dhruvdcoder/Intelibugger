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

enum ProcessStatus {
   NOT_STARTED=0,
   STOPPED_AT_FIRST_INST
};



class Tracee {
   private:
  pid_t m_pid;
  std::string m_name;
  std::vector<std::string> m_args_vector;
  char*const* m_args; // args for the traced process as array of char pointers. The last pointer should point to NULL and first to the executable.
  // TODO: modify the mechanism to take in args. Accept a string of arguments sperated by space and parse the string inside the class. 
   ProcessStatus m_proc_status;   
   int m_file_descriptor; // TODO better handling of file desc in Tracee.
   Dwarf_Debug m_dwarf_dbg;
   Dwarf_Die m_dwarf_cu_die;
   LineInfo m_line_info; 
   public:
  //getters
 inline pid_t getid(){return m_pid;}

 //constructor
//   tracee(pid_t id):e_pid(id),e_name("unpecified name"),args(NULL) {}
   Tracee(std::string prog);
// destructor : We need one now beacuse we are managing args using new
   ~Tracee();
   // utility functions
 //
    void start(); 
    void continueProc(); 
    //dwarf related
    void initDwarf();
    ProcessStatus getProcessStatus() const {return m_proc_status;}

     
};

#endif // _TRACEE_
