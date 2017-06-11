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
#include <sys/signal.h> // for the definition of SIGTRAP and other tokens

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
/*
   TEST_F(TraceeTest,teststart){
      using namespace std;
      string binary ="../bin/dummy";
      
      Tracee process (binary);
      EXPECT_TRUE(NOT_STARTED==process.getProcessStatus());
      process.start();
      EXPECT_TRUE(STOPPED_AT_FIRST_INST==process.getProcessStatus());
      process.continueProc();

      
      

   }
 */
   TEST_F(TraceeTest,testaddBreakPoint){
      using namespace std;
      string binary ="../bin/dummy";
      Tracee process (binary);
      process.initDwarf();
      process.load();
      process.addBreakPoint(9);
      process.continueProc();
      int status=0;
      wait(&status);
      ASSERT_TRUE(WIFSTOPPED(status));
      ASSERT_TRUE(SIGTRAP==WSTOPSIG(status));
      cout<< "Stopped at breakpoint" <<endl;
      process.continueProc();


   }


}

int main(int argc,char **argv){
   ::testing::InitGoogleTest(&argc,argv);
   return RUN_ALL_TESTS();
}
