/*! @file Test_BreakPointsTraceeProcessUtils.cpp 
 * @brief Contains the tests for the class LineInfo
 */

#include "../include/BreakPoints.h"
#include "../include/ProcessUtils.h"
#include "../include/Tracee.h"
#include "../include/Error.h"
#include "gtest/gtest.h"
#include <fcntl.h> // for open() create() file descriptors
#include <unistd.h> // for close() file descriptor
#include <cstddef> // for nullptr
#include <stdio.h>
#include <iostream>
namespace{
   class BreakPointsTraceeProcessUtilsTest : public ::testing::Test{
      public:
      
      
     BreakPointsTraceeProcessUtilsTest(){
   
    }
    virtual ~BreakPointsTraceeProcessUtilsTest(){
       
    }
      protected:
         //virtual void SetUp() {

         //}

         //virtual void TearDown() {
            
         //}

   };
 

   TEST_F(BreakPointsTraceeProcessUtilsTest,testFetchingCurrentBP){
      using namespace std;
      string binary ="../bin/dummy";
      Tracee process (binary);
      process.initDwarf();
      process.load();
      process.addBreakPoint(7);
      process.addBreakPoint(9);
      const BreakPoints& bps=process.getBreakPoints();
      process.continueProc();
      int status1=0;
      wait(&status1);
      ASSERT_TRUE(WIFSTOPPED(status1));
      ASSERT_TRUE(SIGTRAP==WSTOPSIG(status1));
      const BreakPoints::breakpoint bp1=bps.getCurrentBreakpoint(process.getid());
      ASSERT_TRUE(bp1.getAddress() == 0x004008ac);
      cout<<" First bp verified. Continuing..."<<endl;
      //process.continueProc();
      bps.continueAfterBreakPoint(process.getid());
      int status2=0;
      wait(&status2);
      ASSERT_TRUE(WIFSTOPPED(status2));
      ASSERT_TRUE(SIGTRAP==WSTOPSIG(status2));
      const BreakPoints::breakpoint bp2 = bps.getCurrentBreakpoint(process.getid());
      ASSERT_TRUE(bp2.getAddress() == 0x004008c8);
      cout<<" Second bp verified. Continuing..."<<endl;

      //process.continueProc();
      bps.continueAfterBreakPoint(process.getid());





   }
}

int main(int argc,char **argv){
   ::testing::InitGoogleTest(&argc,argv);
   return RUN_ALL_TESTS();
}
