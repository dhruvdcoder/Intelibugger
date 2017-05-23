/** @file Tracee.cpp
 */
#include "../include/Tracee.h"
#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <iterator>
#include <sys/wait.h>

/** The constructor
 */

Tracee::Tracee(std::string prog):m_pid(-1){
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
   else if (child_pid>0) {
         int status;
         // wait for the child to stop on its first instruction after exec()
         wait(&status); 
         cout << "The child has stopped on first instruction" << endl;
         // ask the child to continue
         ptrace(PTRACE_CONT,child_pid,NULL,0);
      }
      else {
         perror("fork");
         return;
      }

 }

