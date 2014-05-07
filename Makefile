SRC_DIR=./src
BIN_DIR=./bin
INCLUDE_DIR=./include
LIB_DIR=./lib
TEST_DIR=./testsuite

CXX=g++
AR=ar

CFLAGS := -I$(INCLUDE_DIR) -O2 -std=c++0x -fPIC
LFLAGS := -L$(LIB_DIR)

all: generate_dirs generate_headers generate_libs generate_bins generate_tests
################################################################
##
################################################################
generate_dirs:
	mkdir -pv $(BIN_DIR) $(INCLUDE_DIR) $(LIB_DIR)

################################################################
# copy headers                                                 #
################################################################
generate_headers:
	cp $(SRC_DIR)/$(INCLUDE_DIR)/* $(INCLUDE_DIR)

################################################################
# build the libs                                               #
################################################################
generate_libs: libopenjson.a libopenjson.so libs_mv_clean
openjson.o: include/openjson.h src/lib/openjson.cpp
	$(CXX) -c $(CFLAGS)  $(SRC_DIR)/$(LIB_DIR)/openjson.cpp -o $(SRC_DIR)/$(LIB_DIR)/openjson.o
libopenjson.a: openjson.o
	$(AR) crv $(LIB_DIR)/libopenjson.a $(SRC_DIR)/$(LIB_DIR)/openjson.o
libopenjson.so: openjson.o
	$(CXX) -shared  $(SRC_DIR)/$(LIB_DIR)/openjson.o -o $(LIB_DIR)/libopenjson.so
libs_mv_clean:
	rm $(SRC_DIR)/$(LIB_DIR)/openjson.o 
#############
##
#############
generate_bins:


################################################################
# build the testsuite                                          #
################################################################
generate_tests: openjson_test_a openjson_test_so openjson_test_hpp tests_mv_clean
openjson_test_h.o : include/openjson.h testsuite/src/openjson_test_h.cpp
	$(CXX)  -c $(CFLAGS) $(TEST_DIR)/src/openjson_test_h.cpp -o $(TEST_DIR)/src/openjson_test_h.o
openjson_test_a : openjson_test_h.o
	$(CXX)  -static -o $(TEST_DIR)/bin/openjson_test_a  $(TEST_DIR)/src/openjson_test_h.o $(LFLAGS) -lopenjson
openjson_test_so: openjson_test_h.o
	$(CXX)          -o $(TEST_DIR)/bin/openjson_test_so $(TEST_DIR)/src/openjson_test_h.o $(LFLAGS) -lopenjson
openjson_test_hpp: testsuite/src/openjson_test_hpp.cpp
	$(CXX) $(CFLAGS) testsuite/src/openjson_test_hpp.cpp -o testsuite/bin/openjson_test_hpp
tests_mv_clean:
	rm $(TEST_DIR)/src/openjson_test_h.o
install:
	cp include/*          /usr/include
	cp lib/libopenjson.so /usr/lib/ 
	cp lib/libopenjson.a  /usr/lib/
