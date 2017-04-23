/** @file debugee.hpp **/
#ifndef _TRACEE_H
#define _TRACEE_H

#include <sys/types.h> // for pid_t
#include <sys/ptrace.h> // for ptrace
#include <unistd.h> // for getpid
#include <iostream>
#include <string>
#include <stdio.h>
#include <errno.h> // for perror()
class tracee {
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
   tracee(std::string prog):e_pid(-1),e_name(prog),args(set_args()){}

// destructor : We need one now beacuse we are managing args using new
   ~tracee(void){
      int i=1;
      const char* next =args[i];
      for(;next!=NULL;){
         delete[] next;
      }
      delete[] args;
   }
 // utility functions
 //
 // Function to convert the args according to the requirements of ptrace
char*const*  set_args(){
       char** temp_args = new char* [2]; // allocate an array of char* 
       // TODO: Make a resouce managing class for the args since now you need to add a delete somewhere as you are using a new.
      // temp_args[0]=e_name.c_str(); // the first member will point to the name of the prog
       temp_args[0]=NULL; // since there are now args the second will point to null
       temp_args[1]=NULL;
      return static_cast<char*const*>(temp_args);

   }
   void start_as_tracee(){
      // set the pid
      e_pid=getpid();
      std::cout << " Tracee process " << e_name <<" with pid" << static_cast<int>(e_pid) << " started" << std::endl;
      // Call ptrace with TRACEME to allow the parent process to trace this process
      if (ptrace(PTRACE_TRACEME,0,0,0)<0){
         perror("start_as_tracee :: ptrace");
         return;
      }
      // load the given program
      if(execv(e_name.c_str(),args)<0){
	      perror("execv ::");
	      return;
      }
 }
};

#endif // _TRACEE_H
