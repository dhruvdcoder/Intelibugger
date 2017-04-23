#include "tracee.h"

int main(){
   pid_t child_pid=fork();
   if (child_pid==0){
      tracee myProc ("/home/dhruvesp/Documents/ptrace/dummy");
      myProc.start_as_tracee();
   }
   else if (child_pid>0){
	   int status;
	   // wait for the child to stop on its first instruction after exec()
	   wait(&status);
	   std::cout << "The child has stoped on first instruction " << std::endl;
	   // ask the child to continue
	   ptrace(PTRACE_CONT,child_pid,NULL,0);
   }
   else {
      perror("fork");
      return -1;
   }
   return 0;
}
