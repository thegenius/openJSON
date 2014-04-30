all: openjson.o libopenjson.a libopenjson.so openjson_test_h.o  openjson_test_h openjson_test_hpp clean

openjson.o: openjson.h openjson.cpp
	g++ -c -O2  -std=c++0x -fPIC openjson.cpp -o openjson.o
libopenjson.a: openjson.o
	ar crv libopenjson.a openjson.o
libopenjson.so: openjson.o
	g++  -std=c++0x -shared  openjson.o -o libopenjson.so
openjson_test_h.o : openjson.h openjson_test_h.cpp
	g++  -c -O2 -std=c++0x openjson_test_h.cpp -o openjson_test_h.o
openjson_test_h : openjson_test_h.o
	g++  -static -o openjson_test_h openjson_test_h.o -L. -lopenjson
openjson_test_hpp: openjson_test_hpp.cpp
	g++ -std=c++0x -O2 openjson_test_hpp.cpp -o openjson_test_hpp
clean:
	rm openjson.o; 
	rm openjson_test_h.o
install:
	cp libopenjson.so /usr/lib/ 
