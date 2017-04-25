/**@file LineInfo.h 
 * @brief contains the interface to map line number to address and vise-versa.
 */

#ifndef _LINEINFO_H
#define _LINEINFO_H
/** @brief Class to provide interface with libdwarf to map line number to address and vice-versa
 */
class LineInfo{
   private:

   public:
	   getAddressFromLine(unsigned int line);
}

#endif // LINEINFO_H
