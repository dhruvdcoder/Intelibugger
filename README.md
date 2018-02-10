# README #

This README would normally document whatever steps are necessary to get your application up and running.

### What is this repository for? ###

* This is an attempt to make an intellegent debugger for C/C++ code on Linux. Following are the first major features:
	1. Presenting variour views of the code like call graph at different granularities using the execution profile.
	2. Map the top-level changes in the program behavior (can be represented using test programs) to sites in code. (The intellegent part)
* Version : 1.0

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