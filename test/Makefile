.PHONY: clean, mrproper
VPATH=../src:../obj:../include:../../dwarf-20170416-build/libdwarf
LIB_GTEST=-L../../googletest_build_nopthread/install/lib -lgtest
LIB_GTEST_P=-lgtest
LIB_DWARF=-L../../dwarf-20170416-build/libdwarf -ldwarf -lelf -lz
INCLUDE_GTEST=-I../../googletest_build_nopthread/install/include/
INCLUDE_GTEST_P=
INCLUDE_DWARF=-I../../dwarf-20170416-build/libdwarf/
CC = g++
CFLAGS = -g -Wall -std=c++11 
CXX=g++
CXXFLAGS=-g -Wall -std=c++11 
LD=g++
LDFLAGS=-g

OBJDIR=../obj


all : Test_Tracee Test_LineInfo Test_BreakPoints

.PHONY: ProcessUtils_obj
ProcessUtils_obj : $(OBJDIR)/ProcessUtils.o


Test_Tracee : $(OBJDIR)/Test_Tracee.o $(OBJDIR)/Tracee.o $(OBJDIR)/LineInfo.o $(OBJDIR)/BreakPoints.o
	$(LD) $(LDFLAGS) -o ../bin/$@ $+ $(LIB_DWARF) $(LIB_GTEST)

Test_LineInfo : $(OBJDIR)/Test_LineInfo.o $(OBJDIR)/LineInfo.o
	$(LD) $(LDFLAGS) -o ../bin/$@ $+ $(LIB_DWARF) $(LIB_GTEST)

Test_BreakPoints : $(OBJDIR)/Test_BreakPoints.o $(OBJDIR)/BreakPoints.o
	$(LD) $(LDFLAGS) -o ../bin/$@ $+ $(LIB_DWARF) $(LIB_GTEST)

Test_ProcessUtilsFork : $(OBJDIR)/Test_ProcessUtilsFork.o $(OBJDIR)/ProcessUtils.o
	$(LD) $(LDFLAGS) -o ../bin/$@ $+ $(LIB_GTEST)
Test_ProcessUtilsForkWithExceptions : $(OBJDIR)/Test_ProcessUtilsForkWithExceptions.o $(OBJDIR)/ProcessUtils.o
	$(LD) $(LDFLAGS) -o ../bin/$@ $+ $(LIB_GTEST)


# using implicit rules to build the object files
$(OBJDIR)/BreakPoints.o : BreakPoints.cpp 
	$(CXX) $(CXXFLAGS) -c -o $@ $+ $(INCLUDE_DWARF)
$(OBJDIR)/LineInfo.o : LineInfo.cpp 
	$(CXX) $(CXXFLAGS) -c -o $@ $+ $(INCLUDE_DWARF)

$(OBJDIR)/Tracee.o : Tracee.cpp 
	$(CXX) $(CXXFLAGS) -c -o $@ $+ $(INCLUDE_DWARF)

$(OBJDIR)/Test_Tracee.o : Test_Tracee.cpp 
	$(CXX) $(CXXFLAGS) -c -o $@ $+ $(INCLUDE_GTEST) $(INCLUDE_DWARF)

$(OBJDIR)/Test_LineInfo.o : Test_LineInfo.cpp 
	$(CXX) $(CXXFLAGS) -c -o $@ $+ $(INCLUDE_GTEST) $(INCLUDE_DWARF)

$(OBJDIR)/Test_BreakPoints.o : Test_BreakPoints.cpp 
	$(CXX) $(CXXFLAGS) -c -o $@ $+ $(INCLUDE_GTEST) $(INCLUDE_DWARF)
$(OBJDIR)/ProcessUtils.o : ProcessUtils.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $+ 

$(OBJDIR)/Test_ProcessUtilsFork.o : Test_ProcessUtilsFork.cpp 
	$(CXX) $(CXXFLAGS) -c -o $@ $+ $(INCLUDE_GTEST) 
$(OBJDIR)/Test_ProcessUtilsForkWithExceptions.o : Test_ProcessUtilsForkWithExceptions.cpp 
	$(CXX) $(CXXFLAGS) -c -o $@ $+ $(INCLUDE_GTEST) 
#all: Test_BreakPoints

#Test_BreakPoints: Test_BreakPoints.cpp BreakPoints.cpp
#	$(CC) $(CFLAGS) -o ../bin/$@ $+ -I../../dwarf-20170416-build/libdwarf -lelf -L../../dwarf-20170416-build/libdwarf -ldwarf -lz -lgtest

#Test_Tracee: Test_Tracee.cpp Tracee.cpp LineInfo.cpp BreakPoints.cpp
#	$(CC) $(CFLAGS) -o ../bin/$@ $+ -I../../dwarf-20170416-build/libdwarf -L../../dwarf-20170416-build/libdwarf -ldwarf -lelf -lz -lgtest


clean:
	rm -f *.o core.*

mrproper: clean
	rm -f Test_BreakPoints
