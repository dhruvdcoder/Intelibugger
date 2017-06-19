/*! @file ProcessUtils.cpp
 *
 */

#include "../include/ProcessUtils.h"
#include <exception>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <cstring> // for std::strerror
#include <sys/types.h>// for wait
#include <sys/wait.h> // for wait
#include <sys/user.h> // for user_reg_struct
#include <unistd.h> // for alarm()
#include <string.h> // for sys_signame()
#include <vector>
#include <string>
#include <sstream> // istringstream
#include <iterator>


// initialize the static member 
std::ostringstream ProcessUtils::ProcessException::buff;

namespace ProcessUtils {
   constexpr char basic_error_prifix[] = "Process Error: ";
   constexpr char default_extra_error_message[] = "";
   constexpr unsigned int time_out_seconds=5;
   constexpr unsigned int error_message_buffer_size=50;
   constexpr uint64_t CLEARMASK=0xffffffffffffff00;
   constexpr uint64_t INT3=0x00000000000000cc;
   /*! \brief Brief function description here
    *
    *  Detailed description
    *
    * \return Return parameter description
    */
   ProcessException::ProcessException():runtime_error(basic_error_prifix),m_extraMessage(default_extra_error_message),m_errno(0) {
      
   }
/*! \brief Brief function description here
    *
    *  Detailed description
    *
    * \return Return parameter description
    */
   ProcessException::ProcessException(const std::string exp_str):runtime_error(basic_error_prifix),m_extraMessage(exp_str),m_errno(0) {
   }
 /*! \brief Brief function description here
    *
    *  Detailed description
    *
    * \return Return parameter description
    */
   ProcessException::ProcessException(int err):runtime_error(basic_error_prifix),m_extraMessage(default_extra_error_message),m_errno(err) {
      errno=0; // reset the errno once we read its value.
   }

   /*! \brief Brief function description here
    *
    *  Detailed description
    *
    * \param Parameter Parameter description
    * \return Return parameter description
    */
   ProcessException::ProcessException(int err,const std::string exp_str):runtime_error(basic_error_prifix),m_extraMessage(exp_str),m_errno(err) {
      errno=0; // reset the errno once we read its value.

   }
   
   /*! \brief Brief function description here
    *
    *  Detailed description
    *
    * \return Return parameter description
    */
   const char* ProcessException::what() const noexcept {
      buff.str(""); // clear the stream buffer first
      buff<<runtime_error::what()<<m_extraMessage<<getErrorStringFromErrno(m_errno);
      return buff.str().c_str();
      
   }
   
   /*! \brief It reads the error from "errno" and returns the error message.
    *
    *  It creates 
    *  1) an intermediate std::error_condition object and then reads the message through it.
    *  OR
    *  2) uses strerror(errno)
    * \return Return parameter description
    */
   const std::string getErrorStringFromErrno(int err)
   {
      char buff[error_message_buffer_size];
      return err>0 ? strerror_r(err,buff,error_message_buffer_size) : "NIL";
   }

   /*! \brief Wrapper for fork() system call
    *
    *  Detailed description of the function
    *
    * \return Return parameter description
    */
   pid_t createChild()
   {
      pid_t child_pid= fork();
      if (child_pid ==-1)//error
         throw ProcessException (errno,"In createChild: ");
      return child_pid;
   }
   
   void loadUsingExecv(const char* path, char * const * args){
      if (execv(path,args)<0)
         throw ProcessException (errno,"In loadUsingExecv: ");
   }

   /*! \brief This overload takes in the struc of type ExecvArgs.
    *
    *  Detailed description of the function
    *
    * \param Parameter Parameter description
    * \return Return parameter description
    */
   void loadUsingExecv(const ExecvArgs& arg_struct)
   { 
      if(execv(arg_struct.exe_path.c_str(),arg_struct.args)<0)
         throw ProcessException (errno,"In loadUsingExecv: ");
   }

   
   /*! \brief The funtion which warps wait(). Waits for paticular signal.
    *
    *  Detailed description of the function
    *
    * \param Parameter Parameter description
    * \return Return parameter description
    */
   void parentWaitForSignalFromChild(int signal)
   {
      int status;
      //alarm(0); // cancel any pending alarm
      //alarm(time_out_seconds); // setup a new alarm. It will raise SIGALRM after time_out_seconds secs.
      
      struct timespec timeout {time_out_seconds,0}; // create a timespec for giving to sigtimedwait
      sigset_t sigs_to_wait_for, original_mask;
      sigemptyset(&sigs_to_wait_for); // empty the mask
      sigaddset(&sigs_to_wait_for, SIGCHLD); // add SIGCHLD to the mask. SIGCHILD is sent to parent when the child stops or terminates.
      if (sigprocmask(SIG_BLOCK, &sigs_to_wait_for, &original_mask) < 0) { // replaces the signal mask of the thread. Saves the original
         throw ProcessException (errno,"In parentWaitForSignalFromChild : In sigprocmask : ");
      }
      // wait for the SIGCHLD
      if (sigtimedwait(&sigs_to_wait_for,NULL,&timeout)<0){
         throw ProcessException (errno,"In parentWaitForSignalFromChild : In sigtimedwait : ");
      }
      // execute wait on child to ensure that 
      // 1. it has stopped
      // 2. stopped due to SIGTRAP
      // 3. and not terminated or stopped due to something else
      wait(&status);
      if(WIFSTOPPED(status)){
         if(SIGTRAP!=WSTOPSIG(status)){
            std::ostringstream buff;
            buff << "In parentWaitForSignalFromChild : The child was stopped due to signal " << sys_siglist[WSTOPSIG(status)] << " instead of SIGTRAP ";  
            throw ProcessException(buff.str());
         }
      }
      else {
         throw ProcessException("In parentWaitForSignalFromChild : The child was not stopped  or it was terminated");
      }
       
   }
   
   /*! \brief Takes in the string of form "filename_path arg1 arg2 ..." and creates the structure which has members suitable to be used with "execv".
    *  Detailed description
    *
    * \param Parameter Parameter description
    * \return Return parameter description
    */
   ExecvArgs::ExecvArgs(std::string full_args_string) {
      using namespace std;
      std::istringstream iss(full_args_string);//create stream for processing
      vector<string> args_vector; // string vector to store the strings for argument. One string per arg
      iss >> exe_path; //TODO: error checking
      copy(istream_iterator<string>(iss),istream_iterator<string>(),back_inserter(args_vector));
      char** temp_args = new char* [args_vector.size()+1]; // allocate an array of char* 
      int i=0;
      temp_args[args_vector.size()+1]=NULL; // set the last pointer as NULL as required by execv
      // fill in the other pointers with the args
      for(auto argi=args_vector.begin(); argi != args_vector.end(); ++argi){
         temp_args[i]=const_cast<char *>(argi->c_str()); // Potentially unsafe cast
         i++;
      }
      args = static_cast<char* const *>(temp_args);
   }
   /*! \brief Brief function description here
    *
    *  Detailed description
    *
    * \return Return parameter description
    */
   ExecvArgs::~ExecvArgs() {
     delete[] args; 
   }
   /*! \brief Wrapper to ptrace PTRACE_TRACEME
    *
    *  Detailed description of the function
    *
    * \return Return parameter description
    */
   void traceMe()
   {
      if(ptrace(PTRACE_TRACEME,0,0,0)<0)
         throw ProcessException(errno, "In traceMe: ");
   } 
   /*! \brief Wrapper for PTRACE_CONT
    *
    *  Detailed description of the function
    *
    * \param Parameter Parameter description
    * \return Return parameter description
    */
   void continueChild(pid_t child_pid)
   {
      if(ptrace(PTRACE_CONT,child_pid,NULL,0)<0)
         throw ProcessException(errno,"In continueChild: ");
   }
   
   /*! \brief Wrapper for getting instruction using PTRACE_PEEKTEXT
    *
    *  Detailed description of the function
    *
    * \param Parameter Parameter description
    * \param Parameter Parameter description
    * \return Return parameter description
    */
   uint64_t getInstruction(pid_t child_pid, uint64_t instruction_address)
   {
      //since PEEKTEXT can return -1 as a valid data/instruction we need to check errno explicitly
      errno=0; // reset errno
      uint64_t inst =ptrace(PTRACE_PEEKTEXT, child_pid,(void*)(instruction_address),0);
      if (errno != 0)
         throw ProcessException (errno,"In getInstruction: ");
      return inst;
   }

   /*! \brief Wrapper for PTRACE_POKETEXT to insert instruction at address
    *
    *  Detailed description of the function
    *
    * \param child_pid PID of the process to poke into
    * \param Parameter Parameter description
    * \param Parameter Parameter description
    * \return Return parameter description
    */
   void setInstruction(pid_t child_pid, uint64_t address, uint64_t instruction)
   {
      if(ptrace(PTRACE_POKETEXT, child_pid, (void*)address, (void*)instruction)<0)
         throw ProcessException (errno,"In setInstruction");
   }

   /*! \brief Function to get instruction modified to act as a trap
    *
    *  Detailed description of the function
    *
    * \param Parameter Parameter description
    * \return Return parameter description
    */
   uint64_t getInstructionWithTrap(uint64_t instruction)
   {
      return ((instruction & CLEARMASK)|INT3);
   }

   /*! \brief Fetches the values of the registers and stores in the member.
    *
    *  Detailed description
    *
    * \param Parameter Parameter description
    * \return Return parameter description
    */
   void Registers::fetch(pid_t pid) {
      if (ptrace(PTRACE_GETREGS,pid,NULL,&m_registers)<0)
         throw ProcessException (errno,"In Registers::fetch");
      isInitialized=true;
   }

   /*! \brief Brief function description here
    *
    *  Detailed description
    *
    * \return Return parameter description
    */
   uint64_t Registers::getInstructionPointer() {
     if (isInitialized)
        return m_registers.rip;
     else 
        throw ProcessException("In Registers::getInstruction : The user_regs_struct was not initialized");
   }
   
   /*! \brief Brief function description here
    *
    *  Detailed description of the function
    *
    * \param pid Parameter description
    * \return Return parameter description
    */
   uint64_t getInstructionPointer(pid_t pid)
   {
      Registers temp_regs;
      temp_regs.fetch(pid);
      return temp_regs.getInstructionPointer();
   }

   /*! \brief Brief function description here
    *
    *  Detailed description
    *
    * \param Parameter Parameter description
    * \return Return parameter description
    */
   void Registers::setInstructionPointer(uint64_t rip) {
      if (isInitialized)
         m_registers.rip = rip;
      else 
         throw ProcessException ("In Registers::setInstructionPointer : The user_regs_struct was not initialized."); 
   }

   /*! \brief Brief function description here
    *
    *  Detailed description of the function
    *
    * \param Parameter Parameter description
    * \return Return parameter description
    */
   void setInstructionPointer(pid_t pid,uint64_t rip)
   {
      Registers temp_regs;
      temp_regs.fetch(pid);
      temp_regs.setInstructionPointer(rip);
      if (ptrace(PTRACE_SETREGS,pid,NULL,&(temp_regs.m_registers)) < 0)
         throw ProcessException (errno, "In ProcessUtils::setInstructionPointer");
   }

   /*! \brief Brief function description here
    *
    *  Detailed description of the function
    *
    * \param pid Parameter description
    * \return Return parameter description
    */
   void singleStepChild(pid_t pid)
   {
      if (ptrace(PTRACE_SINGLESTEP,pid,NULL,NULL) < 0)
         throw ProcessException (errno, "In ProcessUtils::singleStepChild");
   }
}



