/*! @file Test_BreakPoints.cpp 
 * @brief Contains the tests for the class LineInfo
 */

#include "../include/BreakPoints.h"
#include "../include/Error.h"
#include "gtest/gtest.h"
#include <fcntl.h> // for open() create() file descriptors
#include <unistd.h> // for close() file descriptor
#include <cstddef> // for nullptr
#include <stdio.h>
#include <iostream>
namespace{
   class BreakPointsTest : public ::testing::Test{
      public:
      
      
     BreakPointsTest(){
   
    }
    virtual ~BreakPointsTest(){
       
    }
      protected:
         //virtual void SetUp() {

         //}

         //virtual void TearDown() {
            
         //}


   };


   TEST_F(BreakPointsTest,testaddmethod){
      BreakPoints myBPs; // create the object
      myBPs.add(1,20,30);
      myBPs.add(3,223,34);
      myBPs.add(56,345,66); 
      myBPs.add(23,345,88);
      myBPs.print();
      std::cout<< "Query: address 223"<<std::endl;
      myBPs.print(223);


   }
   TEST_F(BreakPointsTest,testQueryUsingId){
      BreakPoints myBPs; // create the object
      myBPs.add(1,20,30);
      myBPs.add(3,223,34);
      myBPs.add(56,345,66); 
      myBPs.add(23,345,88);
      myBPs.print();
      std::cout<< "Query: id 2"<<std::endl;
      const BreakPoints::breakpoint bp = myBPs.getBreakpointUsingIdx(2);
      bp.print();

   }

}

int main(int argc,char **argv){
   ::testing::InitGoogleTest(&argc,argv);
   return RUN_ALL_TESTS();
}
