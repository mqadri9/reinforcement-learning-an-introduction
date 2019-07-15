wp = /home/moham
exp = ${wp}
pypath = /usr/include

all: car_rental.exe clean

car_rental.exe: car_rental.o
	g++  -o ${exp}/car_rental.exe -I${pypath}/python3.6m  car_rental.o -L/bin -lpython3.6m

clean:
	rm -rf  ${wp}/*.o  ${wp}/*.d