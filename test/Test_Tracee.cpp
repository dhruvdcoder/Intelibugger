/*! @file Test_Tracee.cpp 
 * @brief Contains the tests for the class LineInfo
 */

#include "../include/Tracee.h"
#include "../include/Error.h"
#include "gtest/gtest.h"
#include <fcntl.h> // for open() create() file descriptors
#include <unistd.h> // for close() file descriptor
#include <cstddef> // for nullptr
#include <stdio.h>
#include <iostream>
namespace{
   class TraceeTest : public ::testing::Test{
      public:
      
      
     TraceeTest(){
   
    }
    virtual ~TraceeTest(){
       
    }
      protected:
         //virtual void SetUp() {

         //}

         //virtual void TearDown() {
            
         //}


   };


   TEST_F(TraceeTest,testinitialization){
      using namespace std;
      string binary ="../bin/dummy";
      Tracee process (binary);
   }
   TEST_F(TraceeTest,teststart){
      using namespace std;
      string binary ="../bin/dummy";
            pid_t child_pid=fork();
      if (child_pid==0){//child
         Tracee process (binary);
         process.start();
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
      }

   }


}

int main(int argc,char **argv){
   ::testing::InitGoogleTest(&argc,argv);
   return RUN_ALL_TESTS();
}
