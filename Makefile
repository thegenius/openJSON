all: openjson.o libopenjson.a libopenjson.so openjson_test.o  openjson_test

openjson.o: openjson.h openjson.cpp
	g++ -c -O3 -std=c++0x  openjson.cpp -o openjson.o
libopenjson.a: openjson.o
	ar crv libopenjson.a openjson.o
libopenjson.so: openjson.o
	g++ -std=c++0x -shared -fPIC openjson.o -o libopenjson.so
openjson_test.o : openjson.h openjson_test.cpp
	g++ -c -O3 -std=c++0x openjson_test.cpp -o openjson_test.o
openjson_test : openjson_test.o
	g++ -static -o openjson_test openjson_test.o -L. -lopenjson
clean:
	rm openjson.o
	rm openjson_test.o
install:
	cp libopenjson.so /usr/lib/ 
