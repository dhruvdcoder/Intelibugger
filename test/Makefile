.PHONY: clean, mrproper
CC = g++
CFLAGS = -g -Wall -std=c++11
 
LD=g++
LDFLAGS=
VPATH=../src:../../dwarf-20170416-build/libdwarf

all: Test_BreakPoints

Test_BreakPoints: Test_BreakPoints.cpp BreakPoints.cpp
	$(CC) $(CFLAGS) -o ../bin/$@ $+ -I../../dwarf-20170416-build/libdwarf -lelf -L../../dwarf-20170416-build/libdwarf -ldwarf -lz -lgtest

Test_Tracee: Test_Tracee.cpp Tracee.cpp
	$(CC) $(CFLAGS) -o ../bin/$@ $+ -I../../dwarf-20170416-build/libdwarf -lelf -L../../dwarf-20170416-build/libdwarf -ldwarf -lz -lgtest


clean:
	rm -f *.o core.*

mrproper: clean
	rm -f Test_BreakPoints
