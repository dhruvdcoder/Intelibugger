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
class Tracee {
   private:
  pid_t e_pid;
  std::string e_name;
  char*const* args; // args for the traced process as array of char pointers. The last pointer should point to NULL and first to the executable.
  // TODO: modify the mechanism to take in args. Accept a string of arguments sperated by space and parse the string inside the class. 
   public:
  //getters
 inline pid_t getid(){return e_pid;}

 //constructor
//   tracee(pid_t id):e_pid(id),e_name("unpecified name"),args(NULL) {}
   Tracee(std::string prog);
// destructor : We need one now beacuse we are managing args using new
   ~Tracee();
   // utility functions
 //
 // Function to convert the args according to the requirements of ptrace
char*const*  set_args();

   void start_as_tracee(); 
};

#endif // _TRACEE_
