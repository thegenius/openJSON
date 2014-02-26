#openJSON#

__JSON parser that easy in use, elegant in coding, effcient in performance.__\n
If you find any bug, please email me : wangwei@mprc.pku.edu.cn__

#Quick Start:#
	Use openJSON as fast as possible:
		#include <openjson.hpp>

	Use openJSON as dynamic library:
		Install on Linux:
			make
			sudo make install
			#include <openjson.h> in your project

		Install on windows:	
			Create a win32 console project(DLL).
			Generate the .dll file 
			#include <openjson.h> in your project

#Use Example:#
##1. Parse JSON string:##
    const char   *str = 
      "[                                                            \
        1, -2, 3.4, -5.6, 7.8e+9, -12.3E-4, true, false, null,		  \
        \"hello c++ json object\",                                  \
        [ 1, -2, 3.4, -5.6, 7.8e+9, -12.3E-4, true, false, null ],	\
        {                                                           \
          \"姓名 \"  : \"汪蔚\",                                    \
          \"name\"  : \"wang wei\",                                	\
          \"school\": \"pku\",                               			  \
          \"age\"   : 25                                            \
        }                                                           \
      ]";
    Object test1 = parse(str);/*The parse() function takes a C string as input and gives a JSON Object as output.*/
    test1.print();            /*The print() function with print the JSON with nice indent */
    
##2.Get values from Object:##
    test1[1];
    test1[11];
    test1[12];
    test1[11][5];
    test1[12]["name"];
    
    The first one will return the value:
      1
    The second one will return the value: 
      [ 1, -2, 3.4, -5.6, 7.8e+9, -12.3E-4, true, false, null ]
	  The third one will return the value:
      {                                                                	 
        "姓名"		:	"汪蔚",                                       	 
        "name"		:	"wang wei",                                 	 
        "school"	: "pku",                               			 
        "age"		  : 25                                                 
      }
	  The fourth one will return the value:
		  -12.3E-4
	  The fifth one will return the value:
		  “wang wei”
    
##3.Construct object from primitive data type:##
    Object test2;
    test2[0]  = 128;
    test2[2]  = -512;
    test2[4]  = (float)7.2;
    test2[8]  = -3.6e+7;
    test2[10] = false;
    test2[12] = "hello c++ json object";


#API:#
##Type defines:##
	typedef unsigned char  	uint8_t 
	typedef unsigned short 	uint16_t 
	typedef unsigned int    uint32_t 
	typedef unsigned long  	uint64_t 
	typedef char  	int8_t 
	typedef short 	int16_t 
	typedef int   	int32_t 
	typedef long  	int64_t 

##Most important API:##
	Object& parse(const char *str) :
		The parsing api that take a C string(\0 terminated) as input and generate an Object. 
		You can treat the_		Object 	as Object in javascript._
	char* dump(const Object &obj) :
		The dumping api that take an Object as input and generate its string-representation 
		as output.
	Object* clone(const Object *obj) :
		Deep clone api which will copy everything recursively and return it.
	
##Print API:##
	void print() :
		API for printing the content of the Object. It works just like the dump api but with 
		nice indent. It supposed to be used when debugging the program.
	void print_addr() :
		API for printing the address of the Object. It will recursively print all the address 
		of its children with nice indent. It also supposed to be used when debugging.

##Constructor API:##
	Object()
	Object(uint8_t u8):
	Object(uint16_t u16):
	Object(uint32_t u32):
	Object(uint64_t u64):
	Object(int8_t s8) :
	Object(int16_t s16):
	Object(int32_t s32):
	Object(int64_t s64):
	Object(float32_t f32):
	Object(float64_t f64):
	Object(bool b):
	Object(const char *str) :
	Object(const Object &obj)

##Destructor API:##
	Virtual ~Object():
		Destructor that will release the resources with zero reference count.
		
##Assign API:##
	Object& operator= (const uint8_t u8):
	Object& operator= (const uint16_t u16):
	Object& operator= (const uint32_t u32):
	Object& operator= (const uint64_t u64):
	Object& operator= (const int8_t s8):
	Object& operator= (const int16_t s16):
	Object& operator= (const int32_t s32):
	Object& operator= (const int64_t s64):
	Object& operator= (const float32_t f32):
	Object& operator= (const float64_t f64):
	Object& operator= (const bool b):
	Object& operator= (char *str):
	Object& operator= (const Object &obj):

##Type API:##
	enum DATA_TYPE get_type() const;
	bool is_int8();
	bool is_int16();
	bool is_int32();
	bool is_int64();
	bool is_uint8();
	bool is_uint16();
	bool is_uint32();
	bool is_uint64();
	bool is_float32();
	bool is_float64();
	bool is_bool();
	bool is_str();
	bool is_vec();
	bool is_map();
	
##Getter API:##
	uint8_t   get_uint8() const;
	uint16_t  get_uint16() const;
	uint32_t  get_uint32() const;
	uint64_t  get_uint64() const;
	int8_t    get_int8() const;
	int16_t   get_int16() const;
	int32_t   get_int32() const;
	int64_t   get_int64() const;
	float32_t get_float32() const;
	float64_t get_float64() const;
	bool      get_bool() const;
	char*     get_char_ptr() const;
	vector<Object*>* get_vec() const;
	unordered_map<char*,Object*,CHAR_PTR_HASH,CHAR_PTR_EQU>* get_map() const;

##Setter API:##
	void set_uint8(uint8_t u8);
	void set_uint16(uint16_t u16);
	void set_uint32(uint32_t u32);
	void set_uint64(uint64_t u64);
	void set_int8(int8_t s8);
	void set_int16(int16_t s16);
	void set_int32(int32_t s32);
	void set_int64(int64_t s64);
	void set_bool(bool b);
	void set_char_ptr(const char *str);
	
##Operator [] API:##
	Object& operator[](const int idx);
	Object& operator[](char *key) ;
	Object& operator[](Object &obj);
	const Object& operator[](const uint64_t idx) const;
	const Object& operator[](char *key) const ;
	const Object& operator[](Object &obj) const ;
	
##Compare operator API:##
	bool operator<(const Object &obj) const;
	bool operator>(const Object &obj) const;
	bool operator==(const Object obj) const;
	bool operator!=(const Object &obj) const;
	bool operator<=(const Object &obj) const;
	bool operator>=(const Object &obj) const;

##Arithmetic operator API:##
	Object& operator+(const Object &obj);
	Object& operator-(const Object &obj);
	Object& operator*(const Object &obj);
	Object& operator/(const Object &obj);
	Object& operator%(const Object &obj);
	Object& operator++();
	const Object operator++(int);
	Object& operator--();
	const Object operator--(int);

##String API:##
	uint64_t str_len() const;
	uint64_t find_str(Object str, uint64_t off=0) const;
	uint64_t rfind_str(Object str, uint64_t off=string::npos) const;
	void trunc_str(uint64_t pos);
	void append_str(Object &str);
	void insert_str(const Object &str, uint64_t pos);
	void replace_str(const Object &obj, uint64_t pos);
	void erase_str(uint64_t t_beg, uint64_t t_end);

    
    
















