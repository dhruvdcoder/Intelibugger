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
class Tracee {
   private:
  pid_t m_pid;
  std::string m_name;
  std::vector<std::string> m_args_vector;
  char*const* m_args; // args for the traced process as array of char pointers. The last pointer should point to NULL and first to the executable.
  // TODO: modify the mechanism to take in args. Accept a string of arguments sperated by space and parse the string inside the class. 
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
};

#endif // _TRACEE_
