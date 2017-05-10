/** @file LineInfo.cpp
 * @breaf
 */

#include "../include/LineInfo.h"
#include <stdio.h>
#include <cstddef> //for nullptr

/** @brief Sets the Line Context for the line info. This is used later in all member functions 
 */

int LineInfo::setLineContextAndBuffer(){
   Dwarf_Error error;
   if(m_die_ptr==nullptr){
      fprintf(stderr,"Error: In LineInfo::setLineContextAndBuffer. 'm_die_ptr' not set.");
      return DW_DLV_ERROR;
   }
   Dwarf_Bool is_single_table=true;
   int res=dwarf_srclines_b(m_die_ptr,&m_line_version,&m_table_count,&m_line_context,&error);
   if(res != DW_DLV_OK){// DW_DLV_NO_ENTRY or DW_DLV_ERROR
      fprintf(stderr,"Error: In LineInfo::setLineContextAndBuffer. In call to dwarf_srclines_b.");
      return res;
   }
    if (m_table_count ==0){ // A line table with no entries.  
      fprintf(stderr, "Warning: In LineInfo::setLineContextAndBuffer. A line table with no entries.");
      // deallocate the memory
      dwarf_srclines_dealloc_b(m_line_context);
      m_line_context=nullptr;
      m_line_buffer_ptr=nullptr;
    } else if (m_table_count==1){
        res=dwarf_srclines_from_linecontext(m_line_context,&m_line_buffer_ptr,&m_line_count,&error);
   if(res != DW_DLV_OK){// DW_DLV_NO_ENTRY or DW_DLV_ERROR
      fprintf(stderr,"Error: In LineInfo::setLineContextAndBuffer. In call to dwarf_srclines_from_linecontext.");
      dwarf_srclines_dealloc_b(m_line_context);
      m_line_context=nullptr;
      m_line_buffer_ptr=nullptr;
      return res;
   }
    }
   return DW_DLV_OK;
}

/**@brief Set the reference CU DIE for the LineInfo object
 */
int LineInfo::setDiePtr(Dwarf_Die die_ptr){
   m_die_ptr=die_ptr;
   return DW_DLV_OK;
}

/** @brief Destructor to deallocate the memory managed as the client of libdwarf/
 */
LineInfo::~LineInfo(){
   if(m_line_context){
      Dwarf_Error error;
      dwarf_srclines_dealloc_b(m_line_context);
      }
   m_line_buffer_ptr=nullptr;
   m_die_ptr=nullptr; 
}

