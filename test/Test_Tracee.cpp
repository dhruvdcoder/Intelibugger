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
      
      Tracee process (binary);
      EXPECT_TRUE(NOT_STARTED==process.getProcessStatus());
      process.start();
      EXPECT_TRUE(STOPPED_AT_FIRST_INST==process.getProcessStatus());
      process.continueProc();

      
      

   }


}

int main(int argc,char **argv){
   ::testing::InitGoogleTest(&argc,argv);
   return RUN_ALL_TESTS();
}
