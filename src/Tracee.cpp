/** @file Tracee.cpp
 */
#include "../include/Tracee.h"

/** The constructor
 */

Tracee::Tracee(std::string prog):e_pid(-1),e_name(prog),args(set_args()){}

/** 
 * 
 * The destructor
 */

Tracee::~Tracee(){
      int i=1;
      const char* next =args[i];
      for(;next!=NULL;){
         delete[] next;
      }
      delete[] args;
   }

/** Function to convert the args according to the requirements of ptrace
 *
 */
char*const*  Tracee::set_args(){
       char** temp_args = new char* [2]; // allocate an array of char* 
       // TODO: Make a resouce managing class for the args since now you need to add a delete somewhere as you are using a new.
      // temp_args[0]=e_name.c_str(); // the first member will point to the name of the prog
       temp_args[0]=NULL; // since there are now args the second will point to null
       temp_args[1]=NULL;
      return static_cast<char*const*>(temp_args);

   }



/** Start the Tracee
 */

void Tracee::start_as_tracee(){
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

