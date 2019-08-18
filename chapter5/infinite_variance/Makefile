wp = /cygdrive/c/Users/moham/eclipse-workspace/infinite_variance
exp = ${wp}/Debug
pypath = /usr/include

all: infinite_variance.exe clean

infinite_variance.exe: infinite_variance.o
	g++  -o ${exp}/infinite_variance.exe -I${pypath}/python3.6m  infinite_variance.o -L/bin -lpython3.6m

clean:
	rm -rf  ${wp}/src/*.o  ${wp}/src/*.d