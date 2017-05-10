#ifndef _ERROR_H
#define _ERROR_H

#include "libdwarf.h"
#include <stdio.h>
#include <stdlib.h> // for exit()
static void simple_error_handler(Dwarf_Error error, Dwarf_Ptr errarg){
   fprintf(stderr,"Error: In libdwarf. %d %s ",dwarf_errno(error),dwarf_errmsg(error));
   exit(1);
}

#endif // _ERROR_H
