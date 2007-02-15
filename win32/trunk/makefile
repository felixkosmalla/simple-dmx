main : makedll
	lib /machine:i386 /def:timeddmx.def

makedll : timedDMX.o
	g++ -shared -o timeddmx.dll -Wl,--output-def,timeddmx.def --out-implib,libtimeddmxdll.a timedDMX.o FTD2XX.lib
	
timedDMX.o : timedDMX.cpp
	g++ -c -DBUILD_DLL timedDMX.cpp
all :
	${MAKE} main
clean :
	-rm timedDMX.o
	-rm timeddmx.dll
	-rm timeddmx.def
	-rm timeddmx.exp
	-rm timeddmx.lib