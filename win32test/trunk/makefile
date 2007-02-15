timedDMXDir = ../SDMX_Win32

main : testIt.o copyLib
	g++ -o testIt.exe testIt.o -L$(timedDMXDir) -ltimeddmx
copyLib : 
	-cp $(timedDMXDir)/timeddmx.dll ./
testIt.o : testIt.cpp
	g++ -I$(timedDMXDir) -ggdb -c testIt.cpp 
all :
	${MAKE} main
clean :
	-rm testIt.o 
	-rm testIt.exe 
	-rm timeddmx.dll