wp = /cygdrive/c/Users/moham/eclipse-workspace/gamblers_problem
exp = ${wp}/Debug
pypath = /usr/include

all: gamblers_problem.exe clean

gamblers_problem.exe: gamblers_problem.o
	g++  -o ${exp}/gamblers_problem.exe -I${pypath}/python3.6m  gamblers_problem.o -L/bin -lpython3.6m

clean:
	rm -rf  ${wp}/src/*.o  ${wp}/src/*.d