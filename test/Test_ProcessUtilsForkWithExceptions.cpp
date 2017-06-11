/*! @file Test_ProcessUtilsFork.cpp 
 * @brief Contains the tests for the class LineInfo
 */
#include "../include/ProcessUtils.h"
#include "gtest/gtest.h"
#include <iostream>
#include <unistd.h>
namespace{
   class ProcessUtilsForkTest : public ::testing::Test{
      public:
      
      
     ProcessUtilsForkTest(){
   
    }
    virtual ~ProcessUtilsForkTest(){
       
    }
      protected:
         //virtual void SetUp() {

         //}

         //virtual void TearDown() {
            
         //}


   };


   TEST_F(ProcessUtilsForkTest,createChild_loadUsingExecv){
      using namespace std;
      string binary_with_args {"Boom_IamNotThere"};
      ProcessUtils::ExecvArgs args_struct {binary_with_args};      
      // fork new process
      try{
         pid_t child_pid=ProcessUtils::createChild();
         if(child_pid==0){//child
            
            if(ptrace(PTRACE_TRACEME,0,0,0)<0)
               perror("traceme:");
            sleep(30);
           // ProcessUtils::loadUsingExecv(args_struct);
         }
         else {//parent
            ProcessUtils::parentWaitForSignalFromChild(SIGTRAP);
            cout<<"SIGTRAP recieved"<<endl;
            ptrace(PTRACE_CONT,child_pid,NULL,0);
         }
      }
      catch(const exception& e){
         cout<<e.what()<<endl;
      }
     
   }


}

int main(int argc,char **argv){
   ::testing::InitGoogleTest(&argc,argv);
   return RUN_ALL_TESTS();
}
