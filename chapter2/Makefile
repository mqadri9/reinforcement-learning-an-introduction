test.exe: test.o
	g++  -o /cygdrive/c/Users/moham/eclipse-workspace/test/Debug/test.exe -I/usr/include/python3.6m  test.o -L/bin -lpython3.6m
	
test.o: test.cpp
	g++ "test.cpp" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"test.d" -MT"test.o" -o "test.o"