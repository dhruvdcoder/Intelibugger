/*! @file Test_LineInfo.cpp 
 * @brief Contains the tests for the class LineInfo
 */

#include "../include/LineInfo.h"
#include "../include/Error.h"
#include "gtest/gtest.h"
#include <fcntl.h> // for open() create() file descriptors
#include <unistd.h> // for close() file descriptor
#include <cstddef> // for nullptr
#include <stdio.h>
#include <iostream>
namespace{
   class LineInfoTest : public ::testing::Test{
      public:
      int t_fd;
      Dwarf_Debug t_dbg;
      LineInfo t_line_info;
      Dwarf_Die t_cu_die;
      
         LineInfoTest(){
            // open the file
            const char file[]="../bin/dummy";
            t_fd = open(file,O_RDONLY); // open read only
            if(t_fd==-1){
               fprintf(stderr,"Error: Unable to open the file %s.",file);
               exit(1);
            }
            // Get the CU DIE
            // 1. Get the header for the compilation unit ( here assuming that we have only one CU
            // 2. Get the CU DIE by using dwarf_siblingof_b() with NULL.
            Dwarf_Error err;
            int init_res=dwarf_init_b(
                  t_fd,
                  DW_DLC_READ,
                  0,
                  simple_error_handler,
                  nullptr,
                  &t_dbg,
                  &err);
            if(init_res!=DW_DLV_OK){
               fprintf(stderr, "Error in LineInfoTest().");
               exit(1);
            }
            Dwarf_Unsigned cu_header_length,abbrev_offset, typeoffset, next_cu_header;
            Dwarf_Half version_stamp,address_size,offset_size,extension_size,header_cu_type;
            Dwarf_Sig8 sig;

            int head_res=dwarf_next_cu_header_d(
                  t_dbg,
                  true,
                  &cu_header_length,
                  &version_stamp,
                  &abbrev_offset,
                  &address_size,
                  &offset_size,
                  &extension_size,
                  &sig,
                  &typeoffset,
                  &next_cu_header,
                  &header_cu_type,
                  &err);
            if(head_res!=DW_DLV_OK){
               fprintf(stderr, "Error: %s, in LineInfoTest(). Executing dwarf_next_cu_header_d",dwarf_errmsg(err));
               exit(1);
            }



            int sib_res=dwarf_siblingof_b(
                  t_dbg,
                  NULL,
                  true,
                  &t_cu_die,
                  &err);
             if(sib_res!=DW_DLV_OK){
               fprintf(stderr, "Error: %s, in LineInfoTest(). Executing dwarf_siblingof_b",dwarf_errmsg(err));
               exit(1);
            }



            // Initialize the LineInfo Class
            //t_line_info.setDiePtr(t_cu_die);
           //setLineContextAndBuffer(); 
         }
        virtual ~LineInfoTest(){
            
            dwarf_dealloc(t_dbg,t_cu_die,DW_DLA_DIE); // free the storage pointed to by the CU DIE
            Dwarf_Error err;
            dwarf_finish(t_dbg,&err);// deallocate the storage pointed to by t_dbg
            t_dbg=nullptr;
         }
   };


   TEST_F(LineInfoTest,Initializing){
      t_line_info.setDiePtr(t_cu_die);
      t_line_info.setLineContextAndBuffer();
   }
   TEST_F(LineInfoTest,testgetLineIndexInBuffer){
      t_line_info.setDiePtr(t_cu_die);
      t_line_info.setLineContextAndBuffer();
      Dwarf_Unsigned i1=t_line_info.getLineIndexInBuffer(3);
      EXPECT_EQ(Dwarf_Unsigned(0),i1);
      Dwarf_Unsigned i2=t_line_info.getLineIndexInBuffer(4);
      EXPECT_EQ(Dwarf_Unsigned(1),i2);
      Dwarf_Unsigned i3=t_line_info.getLineIndexInBuffer(5);
      EXPECT_EQ(Dwarf_Unsigned(1),i3);
      Dwarf_Unsigned i4=t_line_info.getLineIndexInBuffer(6);
      EXPECT_EQ(Dwarf_Unsigned(2),i4);
      Dwarf_Unsigned i5=t_line_info.getLineIndexInBuffer(12);
      EXPECT_EQ(Dwarf_Unsigned(5),i5);
   
   }

   TEST_F(LineInfoTest,testgetAddressFromLine){
      t_line_info.setDiePtr(t_cu_die);
      t_line_info.setLineContextAndBuffer();
      Dwarf_Addr i1=t_line_info.getAddressFromLine(2);
      EXPECT_EQ(Dwarf_Addr(0x0040089d),i1);
      Dwarf_Addr i2=t_line_info.getAddressFromLine(3);
      EXPECT_EQ(Dwarf_Addr(0x0040089d),i2);
      Dwarf_Addr i3=t_line_info.getAddressFromLine(6);
      EXPECT_EQ(Dwarf_Addr(0x004008ac),i3);



   }
}

int main(int argc,char **argv){
   ::testing::InitGoogleTest(&argc,argv);
   return RUN_ALL_TESTS();
}
