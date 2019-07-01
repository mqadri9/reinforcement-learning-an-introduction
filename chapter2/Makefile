wp = /cygdrive/c/Users/moham/eclipse-workspace/ten-armed-bandit
exp = ${wp}/Debug
pypath = /usr/include

all: ten-armed-bandit.exe clean

ten-armed-bandit.exe: ten-armed-bandit.o bandit.o softmaxselection.o Plot.o
	g++  -o ${exp}/ten-armed-bandit.exe -I${pypath}/python3.6m  ten-armed-bandit.o bandit.o Plot.o softmaxselection.o -L/bin -lpython3.6m
	
ten-armed-bandit.o: ten-armed-bandit.cpp 
	g++ "ten-armed-bandit.cpp" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"ten-armed-bandit.d" -MT"ten-armed-bandit.o" -o "ten-armed-bandit.o"
	
bandit.o: bandit.cpp
	g++ "bandit.cpp" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"bandit.d" -MT"bandit.o" -o "bandit.o"

softmaxselection.o: softmaxselection.cpp
	g++ "softmaxselection.cpp" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"softmaxselection.d" -MT"softmaxselection.o" -o "softmaxselection.o"

Plot.o: Plot.cpp
	g++ "Plot.cpp" -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"Plot.d" -MT"Plot.o" -o "Plot.o"

clean:
	rm -rf  ${wp}/src/*.o  ${wp}/src/*.d