/*! @file PtraceUtils.h 
 * @brief Header for the class PtraceUtils
 */

#ifndef _PROCESSUTILS_H 
#define _PROCESSUTILS_H

#include <sys/ptrace.h>
#include <sys/signal.h> // for the definition of SIGTRAP and other tokens
#include <exception>
#include <stdexcept> //for run_time error

namespace ProcessUtils {
   //ptrace related
   //void traceMe();

   //other process control calls
   pid_t createChild();
/*! \struct ExecvArgs
    *  \brief Brief class description for a base class.
    *
    *  Detailed description
    */
   struct ExecvArgs
   {
   public:
      ExecvArgs(std::string full_args_string);
       ~ExecvArgs();

       std::string exe_path; /*!< Member description */
       char * const * args;
   } ;
   typedef struct ExecvArgs ExecvArgs;

   void loadUsingExecv(const char * path, char * const * args);
   void loadUsingExecv(const ExecvArgs& args_struct);
   void parentWaitForSignalFromChild(int SIGNAL);
   //void continueChild(pid_t child_pid);
   
   //poke peek calls
   //uint64_t getInstruction(pid_t child_pid, uint64_t instruction_address);
   //void setInstruction(pid_t child_pid, uint64_t instruction_address, uint64_t instruction);
   

   // exceptions
   /*! \classProcessException : public std::runtime_errors
    *  \brief Brief class description for a base class.
    *
    *  Detailed description
    */
   class ProcessException : public std::runtime_error
   {
   public:
      ProcessException(int err, const std::string& expString);
      ProcessException(int err);
      ProcessException(const std::string& expString);
      ProcessException();
      virtual const char* what() const noexcept;

   private:
      static std::ostringstream buff; /*!< Member description */
      const std::string& m_extraMessage;
      int m_errno;
      
   };
   const std::string getErrorStringFromErrno(int err); 

}

#endif // _PTRACEUTILS_H
