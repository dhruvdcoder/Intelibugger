# README #
[![MIT Licence](https://badges.frapsoft.com/os/mit/mit.svg?v=103)](https://opensource.org/licenses/mit-license.php)
[![experimental](http://badges.github.io/stability-badges/dist/experimental.svg)](http://github.com/badges/stability-badges)

### What is this repository for? ###

* This is an attempt to make an intelligent debugger for C/C++ code on Linux. Following are the first major features:
	1. Presenting variour views of the code like call graph at different granularities using the execution profile.
	2. Map the top-level changes in the program behavior (can be represented using test programs) to sites in code. (The intelligent part)
* Version : 1.0
### Progress so far ###
* Working on the basic skeleton of debugger and the first feature of creating callgraphs.
### How do I get set up? ###


* Righnow the code is in development. The classes are defined in src and include folders and are tested directly. The driver test code is in test directory. To build the tests see the readme in the test dir.

* Dependencies: libz libelf libdwarf google-test
* Installing the dependencies on Linux:
	1. libz, libelf
		`sudo apt-get install zlib1g zlib1g-dev libelf-dev`

	2. libdwarf can be cloned and built from source : https://github.com/tomhughes/libdwarf

	3. google-test can be cloned and built from source : https://github.com/google/googletest



### Contribution guidelines ###

Not looking for contributions as of now.
