#ifndef __JSON_H__
#define __JSON_H__
#include <cstring>
#include <cmath>
#include <string>
#include <iostream>
#include <vector>
#include <unordered_map>
using namespace std;

#define _strdup strdup

#ifdef _MSC_VER
typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;
typedef unsigned long  uint64_t;
typedef char  int8_t;
typedef short int16_t;
typedef int   int32_t;
typedef long  int64_t;
#else
#include <stdint.h>
#endif

typedef float  float32_t;
typedef double float64_t;

enum DATA_TYPE {
	NULL_TYPE,
	UINT8,    /* begin of number type */
	UINT16,
	UINT32,
	UINT64,
	INT8,
	INT16,
	INT32,
	INT64,
	FLOAT32,
	FLOAT64,
	MPZ_PTR,
	MPQ_PTR,
	MPF_PTR,  
	NUMBER_END,   /* end of number type */
	BOOL,     /* logic type */
	CHAR_PTR, /* string type */
	VEC_PTR,  /* 1st structure type */
	MAP_PTR   /* 2nd structure type */
};

class Object {
private:
	union DATA {
		uint8_t  u8;  /* number type */
		uint16_t u16;
		uint32_t u32;
		uint64_t u64;

		int8_t   s8;
		int16_t  s16;
		int32_t  s32;
		int64_t  s64;

		float32_t f32;
		float64_t f64;

		bool     b;   /* logic type */
		char *   str; /* string type */

		void *   mpz; /* reserved for gmp */
		void *   mpq; /* reserved for gmp */
		void *   mpf; /* reserved for gmp */

		vector<Object*>              *vec; /* ordered sturcture type */
		unordered_map<char*,Object*> *map; /* unordered structure type */

	} data;
	
	int32_t ref_count;
	uint32_t type;

public:
	/* constructor for primative type*/
	Object();                 /* default will be a object pointed to NULL */
	Object(uint8_t u8);
	Object(uint16_t u16);
	Object(uint32_t u32);
	Object(uint64_t u64);
	Object(int8_t s8) ;
	Object(int16_t s16);
	Object(int32_t s32);
	Object(int64_t s64);
	Object(float32_t f32);
	Object(float64_t f64);
	Object(bool b);
	Object(const char *str) ;
	Object(const Object &obj); /* copy constructor */
	/* reserve for gmp */
	//Object(void *mpz);
	//Object(void *mpq);
	//Object(void *mpf);
	/* end of constuctor */

	/* virtual destructor */
	~Object();

	/* assign operator = */
	Object& operator= (const uint8_t u8);
	Object& operator= (const uint16_t u16);
	Object& operator= (const uint32_t u32);
	Object& operator= (const uint64_t u64);
	Object& operator= (const int8_t s8);
	Object& operator= (const int16_t s16);
	Object& operator= (const int32_t s32);
	Object& operator= (const int64_t s64);
	Object& operator= (const float32_t f32);
	Object& operator= (const float64_t f64);
	Object& operator= (const bool b);
	Object& operator= (const char* str);
	Object& operator= (const Object& obj);
	/* end of assign operator = */


	/****************** begin of  predicate  **************************/
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
	/************ end of predicate ***************************/
	
	/* begin of ref count manipulation*/
	bool count_no_positive();
	void increase_count();
	void decrease_count();
	/* end of ref count manipulation*/

	/* function used to check the type
	   according to the DATA_TYPE enum */
	enum DATA_TYPE get_type() const;

	/* begin of getter */
	uint8_t  get_uint8() const;
	uint16_t get_uint16() const;
	uint32_t get_uint32() const;
	uint64_t get_uint64() const;
	int8_t  get_int8() const;
	int16_t get_int16() const;
	int32_t get_int32() const;
	int64_t get_int64() const;
	float32_t get_float32() const;
	float64_t get_float64() const;
	bool    get_bool() const;
	char*   get_char_ptr() const;
	vector<Object*>*              get_vec() const;
	unordered_map<char*,Object*>* get_map() const;
	/*  end of getter  */

	/* begin of setter */
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
	/*  end of setter  */

	void null_to_vec();
	void null_to_map();
	
	/* begin of operator[] */
	/* as location referrence */
	Object& operator[](const int idx);
	Object& operator[](const char *key) ;
	Object& operator[](Object &obj);
	/* as r referrence */
	const Object& operator[](const uint64_t idx) const;
	const Object& operator[](const char *key) const ;
	const Object& operator[](Object &obj) const ;
	/* end of operator[] */

	/* begin of compare operator */
	bool operator<(const Object &obj) const;
	bool operator>(const Object &obj) const;
	bool operator==(const Object obj) const;
	bool operator!=(const Object &obj) const;
	bool operator<=(const Object &obj) const;
	bool operator>=(const Object &obj) const;
	/* end of compare operator */

	/* begin of arithmetic operator */
	Object& operator+(const Object &obj);
	Object& operator-(const Object &obj);
	Object& operator*(const Object &obj);
	Object& operator/(const Object &obj);
	Object& operator%(const Object &obj);
	Object& operator++();
	const Object operator++(int);
	Object& operator--();
	const Object operator--(int);
	/* end of arithmetic operator */

	/* begin of logic operator */
	Object& operator&&(const Object &obj);
	Object& operator||(const Object &obj);
	Object& operator!();
	/* end of logic operator */

	/* begin of string operator */
	uint64_t str_len() const;
	uint64_t find_str(Object str, uint64_t off=0) const;
	uint64_t rfind_str(Object str, uint64_t off=string::npos) const;
	void trunc_str(uint64_t pos);
	void append_str(Object &str);
	void insert_str(const Object &str, uint64_t pos);
	void replace_str(const Object &obj, uint64_t pos);
	void erase_str(uint64_t t_beg, uint64_t t_end);
	/* end of string operator */


	/* print function used for debug */
	/* it will attemp to print it as nice as possible */
	void print(int level = 0) const;
	void print_addr(int level = 0) const;
	/* three important friend functions */
	friend Object* clone(const Object *obj);
	friend Object& parse(const char *str);        /* from string to Object */
	friend char*   dump(const Object &obj); /* from Object to string */
	/* end of friend functions */
};

Object* clone(const Object *obj);
Object& parse(const char *str);
char*   dump(const Object &obj);

#endif
