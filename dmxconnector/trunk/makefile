timedDMXDir = ../SDMX_Win32
jniTestDir = ../SDMX_JNI_Test
javaHome =  c:/Program Files/Java/jdk1.5.0_07
jniImports = -I"$(javaHome)/include" -I"$(javaHome)/include/win32"

main : works
	-cp dmxConnect.dll $(jniTestDir)/dmxConnect.dll
	-cp $(timedDMXDir)/timeddmx.dll $(jniTestDir)/
	
dmxConnect.dll : dmxConnect.o
	g++ -shared -o dmxConnect.dll -Wl,--out-implib,libdmxconnectdll.a dmxConnect.o $(timedDMXDir)/timeddmxdll.lib
	
dmxConnect.o :
	g++ -c -D_JNI_IMPLEMENTATION_ $(jniImports) -I$(timedDMXDir) dmxConnect.cpp
	
works :
	gcc -Wall -D_JNI_IMPLEMENTATION_ -Wl,--kill-at $(jniImports) -I$(timedDMXDir) -shared dmxConnect.cpp $(timedDMXDir)/timeddmx.lib -o dmxConnect.dll
	
all :
	${MAKE} main
	
clean :
	-rm dmxConnect.o 
	-rm dmxConnect.dll