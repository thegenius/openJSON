#include "openjson.h"

/* local function will be used by parse() */
void skip_ws_ctrl(const char *&ptr);
static Object& parse_object(const char *str, const char *&ptr);
static Object& parse_array(const char *str, const char *&ptr);
static Object& parse_value(const char *str, const char *&ptr);
static Object& parse_string(const char *str, const char *&ptr);
static Object& parse_number(const char *str, const char* &ptr);
static uint16_t parse_hex_digits(const char* str, char* &ptr);
static Object& parse_true(const char *str, const char *&ptr);
static Object& parse_false(const char *str, const char *&ptr);
static Object& parse_null(const char *str, const char *&ptr);
/* end of local function */


/************* begin of member function of class **************/
	/* constructor for primative type*/
	Object::Object() {
		this->data.u64 = 0;
		this->type     = NULL_TYPE;
	}
	Object::Object(uint8_t u8) {
		this->data.u64 = u8;      /* we store it into 64bits, but when we fetch it as low 8bits, get the right value*/
		this->type    = UINT8;
	}
	Object::Object(uint16_t u16) {
		this->data.u64 = u16;
		this->type     = UINT16;
	}
	Object::Object(uint32_t u32) {
		this->data.u64 = u32;
		this->type     = UINT32;
	}
	Object::Object(uint64_t u64) {
		this->data.u64 = u64;
		this->type     = UINT64;
	}
	Object::Object(int8_t s8) {
		this->data.s64 = s8;  /* we store it into 64bits, but when we fetch it as low 8bits, get the right value*/
		this->type    = INT8;
	}
	Object::Object(int16_t s16) {
		this->data.s64 = s16;
		this->type     = INT16;
	}
	Object::Object(int32_t s32) {
		this->data.s64 = s32;
		this->type     = INT32;
	}
	Object::Object(int64_t s64) {
		this->data.s64 = s64;
		this->type     = INT64;
	}
	Object::Object(float32_t f32) {
		this->data.f32 = f32;
		this->type     = FLOAT32;
	}

	Object::Object(float64_t f64) {
		this->data.f64 = f64;
		this->type     = FLOAT64;
	}
	Object::Object(bool b) {
		this->data.b = b;
		this->type   = BOOL;
	}
	Object::Object(const char *str) {
		char *dup = strdup(str);
		this->data.str = dup;
		this->type     = CHAR_PTR;
	}
	Object::Object(const Object &obj) {
		this->type     = obj.type;
		switch (obj.type) {
		case NULL_TYPE: this->data.u64 = 0           ; break;
		case UINT8:		this->data.u8  = obj.data.u8 ; break;
		case UINT16:	this->data.u16 = obj.data.u16; break;
		case UINT32:	this->data.u32 = obj.data.u32; break;
		case UINT64:	this->data.u64 = obj.data.u64; break;
		case INT8:		this->data.s8  = obj.data.s8 ; break;
		case INT16:		this->data.s16 = obj.data.s16; break;
		case INT32:		this->data.s32 = obj.data.s32; break;
		case INT64:		this->data.s64 = obj.data.s64; break;
		case FLOAT32:   this->data.f32 = obj.data.f32; break;
		case FLOAT64:   this->data.f64 = obj.data.f64; break;
		//case MPZ_PTR:		this->data.s64 = obj.data.s64; break;
		//case MPQ_PTR:		this->data.s64 = obj.data.s64; break;
		//case MPF_PTR:		this->data.s64 = obj.data.s64; break;
		case BOOL:      this->data.b   = obj.data.b  ; break;
		case CHAR_PTR:	this->data.str = strdup(obj.data.str); break;

		case VEC_PTR:	this->data.vec = new vector<Object*>();
						for (auto it=obj.data.vec->begin(); it!=obj.data.vec->end(); ++it) {
							Object *t = clone(*it);
							this->data.vec->push_back( t );
						}
						break;

		case MAP_PTR:	this->data.map = new unordered_map<char*,Object*>();
						for(auto it=(*(obj.data.map)).begin(); it!=(*(obj.data.map)).end(); ++it) {
							Object *t = clone(it->second);
							(*this->data.map)[it->first] = t;
						};
						break;
		}
	}
	/*/reserved for gmp
	Object(void *mpz) {
		this->data.mpz = mpz;
		this->type     = MPZ_PTR;
	}
	Object(void *mpq) {
		this->data.mpq = mpq;
		this->type     = MPQ_PTR;
	}
	Object(void *mpf) {
		this->data.mpf = mpf;
		this->type     = MPF_PTR;
	}
	//*/

	/* end of constuctor */

	/* destructor */
	Object::~Object() { }

	/* assign operator = */
	Object& Object::operator= (const uint8_t u8) {
		this->data.u8 = u8;
		this->type    = UINT8;
		return *this;
	}
	Object& Object::operator= (const uint16_t u16) {
		this->data.u16 = u16;
		this->type     = UINT16;
		return *this;
	}
	Object& Object::operator= (const uint32_t u32) {
		this->data.u32 = u32;
		this->type     = UINT32;
		return *this;
	}
	Object& Object::operator= (const uint64_t u64) {
		this->data.u64 = u64;
		this->type     = UINT64;
		return *this;
	}
	Object& Object::operator= (const int8_t s8) {
		this->data.s8 = s8;
		this->type    = INT8;
		return *this;
	}
	Object& Object::operator= (const int16_t s16) {
		this->data.s16 = s16;
		this->type     = INT16;
		return *this;
	}
	Object& Object::operator= (const int32_t s32) {
		this->data.s32 = s32;
		this->type     = INT32;
		return *this;
	}
	Object& Object::operator= (const int64_t s64) {
		this->data.s64 = s64;
		this->type     = INT64;
		return *this;
	}
	Object& Object::operator= (const float f32) {
		this->data.f32 = f32;
		this->type     = FLOAT32;
		return *this;
	}
	Object& Object::operator= (const double f64) {
		this->data.f64 = f64;
		this->type     = FLOAT64;
		return *this;
	}
	Object& Object::operator= (const bool b) {
		this->data.b = b;
		this->type   = BOOL;
		return *this;
	}

	Object& Object::operator= (char *str) {
		this->data.str = strdup(str);
		this->type     = CHAR_PTR;
		return *this;
	}

	Object& Object::operator= (const Object &obj) {
		this->type     = obj.type;
		switch (obj.type) {
		case NULL_TYPE: this->data.u64 = 0           ; break;
		case UINT8:		this->data.u8  = obj.data.u8 ; break;
		case UINT16:	this->data.u16 = obj.data.u16; break;
		case UINT32:	this->data.u32 = obj.data.u32; break;
		case UINT64:	this->data.u64 = obj.data.u64; break;
		case INT8:		this->data.s8  = obj.data.s8 ; break;
		case INT16:		this->data.s16 = obj.data.s16; break;
		case INT32:		this->data.s32 = obj.data.s32; break;
		case INT64:		this->data.s64 = obj.data.s64; break;
		case FLOAT32:   this->data.f32 = obj.data.f32; break;
		case FLOAT64:   this->data.f64 = obj.data.f64; break;
		//case MPZ_PTR:		this->data.s64 = obj.data.s64; break;
		//case MPQ_PTR:		this->data.s64 = obj.data.s64; break;
		//case MPF_PTR:		this->data.s64 = obj.data.s64; break;
		case BOOL:      this->data.b   = obj.data.b  ; break;
		case CHAR_PTR:	this->data.str = strdup(obj.data.str); break;

		case VEC_PTR:	this->data.vec = new vector<Object*>();
						for (auto it=obj.data.vec->begin(); it!=obj.data.vec->end(); ++it) {
							Object *t = clone(*it);
							this->data.vec->push_back( t );
						}
						break;

		case MAP_PTR:	this->data.map = new unordered_map<char*,Object*>();
						for(auto it=(*(obj.data.map)).begin(); it!=(*(obj.data.map)).end(); ++it) {
							Object *t = clone(it->second);
							(*this->data.map)[it->first] = t;
						};
						break;
		}

		return *this;
	}
	/* end of assign operator = */



	/**************** begin of  predicate  *****************/
	bool Object::is_int8() {
		return this->type == INT8;
	};
	bool Object::is_int16() {
		return this->type == INT16;
	};
	bool Object::is_int32() {
		return this->type == INT32;
	};
	bool Object::is_int64() {
		return this->type == INT64;
	};
	bool Object::is_uint8() {
		return this->type == UINT8;
	};
	bool Object::is_uint16() {
		return this->type == UINT16;
	};
	bool Object::is_uint32() {
		return this->type == UINT32;
	};
	bool Object::is_uint64() {
		return this->type == UINT64;
	};
	bool Object::is_float32() {
		return this->type == FLOAT32;
	};
	bool Object::is_float64() {
		return this->type == FLOAT64;
	};
	bool Object::is_bool() {
		return this->type == BOOL;
	};
	bool Object::is_str() {
		return this->type == CHAR_PTR;
	}
	bool Object::is_vec() {
		return this->type == VEC_PTR;
	};
	bool Object::is_map() {
		return this->type == MAP_PTR;
	};
	/************* end of predicate *************************/

	/* function used to check the type
	   according to the DATA_TYPE enum */
	enum DATA_TYPE Object::get_type() const{
		return (enum DATA_TYPE)this->type;
	}


	/*************** begin of getter **************************/
	/* we do not check the type to give the flexibility of use */
	uint8_t  Object::get_uint8() const{
		return this->data.u8;
	};
	uint16_t Object::get_uint16() const{
		return this->data.u16;
	};
	uint32_t Object::get_uint32() const{
		return this->data.u32;
	};
	uint64_t Object::get_uint64() const{
		return this->data.u64;
	}
	int8_t  Object::get_int8() const{
		return this->data.s8;
	};
	int16_t Object::get_int16() const{
		return this->data.s16;
	};
	int32_t Object::get_int32() const{
		return this->data.s32;
	};
	int64_t Object::get_int64() const{
		return this->data.s64;
	};
	float32_t Object::get_float32() const {
		return this->data.f32;
	};
	float64_t Object::get_float64() const {
		return this->data.f64;
	};
	bool    Object::get_bool() const{
		return this->data.b;
	};
	char*   Object::get_char_ptr() const {
		return this->data.str;
	}
	vector<Object*>*              Object::get_vec() const {
		return this->data.vec;
	};
	unordered_map<char*,Object*>* Object::get_map() const {
		return this->data.map;
	};
	/**************** end of getter *************************/
	
	/* begin of setter */
	void Object::set_uint8(uint8_t u8) {
		this->type = UINT8;
		this->data.u64 = u8;
	}
	void Object::set_uint16(uint16_t u16) {
		this->type = UINT16;
		this->data.u64 = u16;
	}
	void Object::set_uint32(uint32_t u32) {
		this->type = UINT32;
		this->data.u64 = u32;
	}
	void Object::set_uint64(uint64_t u64) {
		this->type = UINT64;
		this->data.u64 = u64;
	}
	void Object::set_int8(int8_t s8) {
		this->type = INT8;
		this->data.s64 = s8;
	};
	void Object::set_int16(int16_t s16) {
		this->type = INT16;
		this->data.s64 = s16;
	};
	void Object::set_int32(int32_t s32) {
		this->type = INT32;
		this->data.s64 = s32;
	};
	void Object::set_int64(int64_t s64) {
		this->type = INT64;
		this->data.s64 = s64;
	};
	void Object::set_bool(bool b) {
		this->type = BOOL;
		this->data.b = b;
	};
	void Object::set_char_ptr(const char *str) {
		this->type = CHAR_PTR;
		char *dup = strdup(str);
		this->data.str = dup;
	};
	/*  end of setter  */


	void Object::null_to_vec() {
		this->type = VEC_PTR;
		this->data.vec = new vector<Object*>();
	}
	void Object::null_to_map() {
		this->type = MAP_PTR;
		this->data.map = new unordered_map<char*,Object*>();
	}
	
	/* begin of operator[] */
	/* as location referrence */
	Object& Object::operator[](const int idx) {
		if (this->type != VEC_PTR) { //it is NULL_PTR or type other than VEC_PTR
			this->data.vec = new vector<Object*>(idx+1);
			this->type = VEC_PTR;

			Object *ptr = new Object[idx+1];
			for (int i=0; i<=idx; ++i) {
				(*this->data.vec)[i] = &ptr[i];
			}

		} else { //now we should deal with the space
			int size = (*this->data.vec).size();
			int less = (idx+1) - size;
			if (less > 0) { //if it is not enought space
				this->data.vec->resize(idx+1);
				Object *ptr = new Object[less];
				for (int i=0; i<less; ++i) {
					(*this->data.vec)[size+i] = &ptr[i];
				}
			}
		}
		return *(*this->data.vec)[idx];
	}

	Object& Object::operator[](char *key) {
		if (this->type != MAP_PTR) {
			this->data.map = new unordered_map<char*,Object*>;
			(*this->data.map)[key] = new Object();
			this->type = MAP_PTR;
		} else {
			if (this->data.map->find(key) == this->data.map->end()) { //not exist
				(*this->data.map)[key] = new Object();
			}
		}
		return *(*this->data.map)[key];
	}

	Object& Object::operator[](Object &obj) {
		if (!obj.is_str()) {
			puts("error: object is not CHAR_PTR, can't be treat as string");
			Object *ret = new Object();
			return *ret;
		}
		char *key = obj.get_char_ptr();
		if (this->type != MAP_PTR) {
			this->data.map = new unordered_map<char*,Object*>;
			(*this->data.map)[key] = new Object();
			this->type = MAP_PTR;
		} else {
			if (this->data.map->find(key) == this->data.map->end()) { //not exist
				(*this->data.map)[key] = new Object();
			}
		}
		return *(*this->data.map)[key];
	}

	const Object& Object::operator[](const uint64_t idx) const {
		if (idx+1 > this->data.vec->size()) {
			Object *tmp = new Object();
			return *tmp;
		} else {
			return *(*this->data.vec)[idx];
		}
	}

	const Object& Object::operator[](char *key) const {
		return *(*this->data.map)[key];
	}

	const Object& Object::operator[](Object &obj) const {
		char *key = obj.get_char_ptr();
		return *(*this->data.map)[key];
	}
	/* end of operator[] */

	/* begin of compare operator */
	bool Object::operator<(const Object &obj) const {
		if (this->type<=FLOAT64 && obj.get_type()<=FLOAT64) {
			// they are both number
			if (this->type<=UINT64 && obj.get_type()<=UINT64) {
				return this->data.u64 < obj.get_uint64();
			} else if (this->type<=INT64 && obj.get_type()<=INT64) {
				return this->data.s64 < obj.get_int64(); /* there may be need fix*/
			} else { 
				// one of it must be float
				float64_t part1;
				float64_t part2;
				//
				if (this->type<=UINT64) {
					part1 = (float64_t) this->data.u64;
				} else if (this->type<=INT64) {
					part1 = (float64_t) this->data.s64;
				} else if (this->type==FLOAT32) {
					part1 = (float64_t)this->data.f32;
				} else {
					part1 = this->data.f64;
				}
				//
				if (obj.get_type()<=UINT64) {
					part2 = (float64_t) obj.get_uint64();
				} else if (obj.get_type()<=INT64) {
					part2 = (float64_t) obj.get_int64();
				} else if (obj.get_type()==FLOAT32) {
					part2 = (float64_t) obj.get_float32();
				} else {
					part2 = obj.get_float64();
				}
				//
				return part1 < part2; /* there maybe bug, fix needed*/
			} 
		} else if (this->type<=FLOAT64 && obj.get_type()>FLOAT64) {
			// number is the smallest
			return true;
		} else if (this->type>FLOAT64 && obj.get_type()<=FLOAT64) {
			return false;
		} else if (this->type==BOOL && obj.get_type()==CHAR_PTR) {
			// bool is smaller than string
			return true;
		} else if (this->type==CHAR_PTR && obj.get_type()==BOOL) {
			return false;
		} else if (this->type==BOOL && obj.get_type()==BOOL) {
			// false is smaller than true
			if (this->data.b == false) {
				return true;
			} else {
				return false;
			}
		} else if (this->type==CHAR_PTR && obj.get_type()==CHAR_PTR) {
			int ret = strcmp(this->data.str, obj.data.str);
			return ret<0;
		} else {
			puts("error: uncomparable type");
			return false;
		}
	};
	bool Object::operator>(const Object &obj) const {
		return !((*this)<obj) && !((*this)==obj);
	};
	bool Object::operator==(const Object obj) const{
		/* now only work for primative data, */
		if (this->type == obj.get_type()) {
			if (this->type<=UINT64) {
				return this->data.u64 == obj.get_uint64();
			} else if (this->type<=INT64) {
				return this->data.s64 == obj.get_int64();
			}
			if (this->type==FLOAT32) {
				float32_t p = (float32_t)0.0000001;
				return fabs((this->data.f32 - obj.get_float32()))<p;
			} else if (this->type==FLOAT64) {
				float64_t p = 0.0000001;
				return fabs((this->data.f64 - obj.get_float64()))<p;
			} else if (this->type==BOOL) {
				return this->data.b == obj.get_bool();
			} else { //str
				return !strcmp(this->data.str, obj.get_char_ptr());
			}
		} else if (this->type<=INT64 && obj.get_type()<=INT64) {
			return this->data.u64 == obj.get_uint64();
		} else if (this->type==FLOAT32 || obj.get_type()==FLOAT32) { 
			// can't be the same type
			float64_t part1;
			float64_t part2;
			if (this->get_type() == FLOAT32) {
				part1 = (float64_t)this->data.f32;
			} else {
				part1 = this->data.f64;
			}
			if (obj.get_type() == FLOAT32) {
				part2 = (float64_t)obj.data.f32;
			} else {
				part2 = obj.data.f64;
			}
			float64_t p = 0.0000001;
			return fabs(part1-part2) < p;
		} else {
			return false;
		}
	};
	bool Object::operator!=(const Object &obj) const {
		return !((*this)==obj);
	};
	bool Object::operator<=(const Object &obj) const {
		return (*this)<obj || (*this)==obj;
	};
	bool Object::operator>=(const Object &obj) const {
		return (*this)>obj || (*this)==obj;
	};
	/* end of compare operator */

	
	/* begin of arithmetic operator */
	Object& Object::operator+(const Object &obj) {
		Object *ret = new Object();
		if (obj.get_type()>NULL_TYPE && obj.get_type()<= FLOAT64) {
			if (this->get_type()<=UINT64 && obj.get_type() <= UINT64) {
				uint64_t part1 = this->data.u64;
				uint64_t part2 = obj.data.u64;
				ret->type = UINT64;
				ret->data.u64 = part1 + part2;
			} else if (this->get_type()<=INT64 && obj.get_type()<=INT64) {
				int64_t part1 = this->data.s64;
				int64_t part2 = obj.data.s64;
				ret->type = INT64;
				ret->data.s64 = part1 + part2;
			} else if (this->get_type()<=FLOAT64 && obj.get_type()<=FLOAT64) {
				float64_t part1;
				float64_t part2;
				if (this->get_type() <=UINT64) {
					part1 = (double)this->data.u64;
				} else if (this->get_type() <= INT64) {
					part1 = (double)this->data.s64;
				} else if (this->get_type() == FLOAT32) {
					part1 = (double)this->data.f32;
				} else {
					part1 = this->data.f64;
				}
				if (obj.get_type() <=UINT64) {
					part2 = (double)obj.data.u64;
				} else if (obj.get_type() <= INT64) {
					part2 = (double)obj.data.s64;
				} else if (obj.get_type() == FLOAT32) {
					part2 = (double)obj.data.f32;
				} else {
					part2 = obj.data.f64;
				}

				ret->type = FLOAT64;
				ret->data.f64 = part1 + part2;
			}
			return *ret;
		} else {
			puts("error: wrong oprand with + operator");
			return *this;
		}
	};
	Object& Object::operator-(const Object &obj) {
		Object *ret = new Object();
		if (obj.get_type()>NULL_TYPE && obj.get_type()<= FLOAT64) {
			if (this->get_type()<=UINT64 && obj.get_type() <= UINT64) {
				uint64_t part1 = this->data.u64;
				uint64_t part2 = obj.data.u64;
				ret->type = UINT64;
				ret->data.u64 = part1 - part2;
			} else if (this->get_type()<=INT64 && obj.get_type()<=INT64) {
				int64_t part1 = this->data.s64;
				int64_t part2 = obj.data.s64;
				ret->type = INT64;
				ret->data.s64 = part1 - part2;
			} else if (this->get_type()<=FLOAT64 && obj.get_type()<=FLOAT64) {
				float64_t part1;
				float64_t part2;
				if (this->get_type() <=UINT64) {
					part1 = (double)this->data.u64;
				} else if (this->get_type() <= INT64) {
					part1 = (double)this->data.s64;
				} else if (this->get_type() == FLOAT32) {
					part1 = (double)this->data.f32;
				} else {
					part1 = this->data.f64;
				}
				if (obj.get_type() <=UINT64) {
					part2 = (double)obj.data.u64;
				} else if (obj.get_type() <= INT64) {
					part2 = (double)obj.data.s64;
				} else if (obj.get_type() == FLOAT32) {
					part2 = (double)obj.data.f32;
				} else {
					part2 = obj.data.f64;
				}
				ret->type = FLOAT64;
				ret->data.f64 = part1 - part2;
			}
			return *ret;
		} else {
			puts("error: wrong oprand with - operator");
			return *this;
		}
	};
	Object& Object::operator*(const Object &obj) {
		Object *ret = new Object();
		if (obj.get_type()>NULL_TYPE && obj.get_type()<= FLOAT64) {
			if (this->get_type()<=UINT64 && obj.get_type() <= UINT64) {
				uint64_t part1 = this->data.u64;
				uint64_t part2 = obj.data.u64;
				ret->type = UINT64;
				ret->data.u64 = part1 * part2;
			} else if (this->get_type()<=INT64 && obj.get_type()<=INT64) {
				int64_t part1 = this->data.s64;
				int64_t part2 = obj.data.s64;
				ret->type = INT64;
				ret->data.s64 = part1 * part2;
			} else if (this->get_type()<=FLOAT64 && obj.get_type()<=FLOAT64) {
				float64_t part1;
				float64_t part2;
				if (this->get_type() <=UINT64) {
					part1 = (double)this->data.u64;
				} else if (this->get_type() <= INT64) {
					part1 = (double)this->data.s64;
				} else if (this->get_type() == FLOAT32) {
					part1 = (double)this->data.f32;
				} else {
					part1 = this->data.f64;
				}
				if (obj.get_type() <=UINT64) {
					part2 = (double)obj.data.u64;
				} else if (obj.get_type() <= INT64) {
					part2 = (double)obj.data.s64;
				} else if (obj.get_type() == FLOAT32) {
					part2 = (double)obj.data.f32;
				} else {
					part2 = obj.data.f64;
				}
				ret->type = FLOAT64;
				ret->data.f64 = part1 * part2;
			}
			return *ret;
		} else {
			puts("error: wrong oprand with * operator");
			return *this;
		}
	};
	Object& Object::operator/(const Object &obj) {
		Object *ret = new Object();
		if (obj.get_type()>NULL_TYPE && obj.get_type()<= FLOAT64) {
			if (this->get_type()<=UINT64 && obj.get_type() <= UINT64) {
				uint64_t part1 = this->data.u64;
				uint64_t part2 = obj.data.u64;
				ret->type = UINT64;
				ret->data.u64 = part1 / part2;
			} else if (this->get_type()<=INT64 && obj.get_type()<=INT64) {
				int64_t part1 = this->data.s64;
				int64_t part2 = obj.data.s64;
				ret->type = INT64;
				ret->data.s64 = part1 / part2;
			} else if (this->get_type()<=FLOAT64 && obj.get_type()<=FLOAT64) {
				float64_t part1;
				float64_t part2;
				if (this->get_type() <=UINT64) {
					part1 = (double)this->data.u64;
				} else if (this->get_type() <= INT64) {
					part1 = (double)this->data.s64;
				} else if (this->get_type() == FLOAT32) {
					part1 = (double)this->data.f32;
				} else {
					part1 = this->data.f64;
				}
				if (obj.get_type() <=UINT64) {
					part2 = (double)obj.data.u64;
				} else if (obj.get_type() <= INT64) {
					part2 = (double)obj.data.s64;
				} else if (obj.get_type() == FLOAT32) {
					part2 = (double)obj.data.f32;
				} else {
					part2 = obj.data.f64;
				}
				ret->type = FLOAT64;
				ret->data.f64 = part1 / part2;
			}
			return *ret;
		} else {
			puts("error: wrong oprand with / operator");
			return *this;
		}
	};
	Object& Object::operator%(const Object &obj) {
		Object *ret = new Object();
		if (obj.get_type()>NULL_TYPE && obj.get_type()<= FLOAT64) {
			if (this->get_type()<=UINT64 && obj.get_type() <= UINT64) {
				uint64_t part1 = this->data.u64;
				uint64_t part2 = obj.data.u64;
				ret->type = UINT64;
				ret->data.u64 = part1 % part2;
			} else if (this->get_type()<=INT64 && obj.get_type()<=INT64) {
				int64_t part1 = this->data.s64;
				int64_t part2 = obj.data.s64;
				ret->type = INT64;
				ret->data.s64 = part1 % part2;
			} else if (this->get_type()<=FLOAT64 && obj.get_type()<=FLOAT64) {
				float64_t part1;
				float64_t part2;
				if (this->get_type() <=UINT64) {
					part1 = (double)this->data.u64;
				} else if (this->get_type() <= INT64) {
					part1 = (double)this->data.s64;
				} else if (this->get_type() == FLOAT32) {
					part1 = (double)this->data.f32;
				} else {
					part1 = this->data.f64;
				}
				if (obj.get_type() <=UINT64) {
					part2 = (double)obj.data.u64;
				} else if (obj.get_type() <= INT64) {
					part2 = (double)obj.data.s64;
				} else if (obj.get_type() == FLOAT32) {
					part2 = (double)obj.data.f32;
				} else {
					part2 = obj.data.f64;
				}
				ret->type = FLOAT64;
				int64_t factor = (int64_t)(part1/part2);
				ret->data.f64 = part1-(factor*part2); /* attention, there must be fixed */
			}
			return *ret;
		} else {
			puts("error: wrong oprand with % operator");
			return *this;
		}
	};
	Object& Object::operator++() {
		if (this->get_type()<=UINT64) {
			++this->data.u64;
		} else if (this->get_type()<=INT64) {
			++this->data.s64;
		} else if (this->get_type()==FLOAT32) {
			this->data.f32 = this->data.f32 + (float32_t)1.0;
		} else if (this->get_type()==FLOAT64) {
			this->data.f64 = this->data.f64 + 1.0;
		}
		return *this;
	};
	const Object Object::operator++(int) {
		Object *ret = new Object();
		if (this->get_type()<=UINT64) {
			ret->type     = UINT64;
			ret->data.u64 = this->data.u64;
			++this->data.u64;
		} else if (this->get_type()<=INT64) {
			ret->type     = INT64;
			ret->data.s64 = this->data.s64;
			++this->data.s64;
		} else if (this->get_type()==FLOAT32) {
			ret->type      = FLOAT32;
			ret->data.f32  = this->data.f32;
			this->data.f32 = this->data.f32 + (float32_t)1.0;
		} else if (this->get_type() == FLOAT64) {
			ret->type      = FLOAT64;
			ret->data.f64  = this->data.f64;
			this->data.f64 = this->data.f64 + 1.0;
		}
		return *ret;
	};
	Object& Object::operator--() {
		if (this->get_type()<=UINT64) {
			--this->data.u64;
		} else if (this->get_type()<=INT64) {
			--this->data.s64;
		} else if (this->get_type()==FLOAT32) {
			this->data.f32 = this->data.f32 - (float32_t)1.0;
		} else if (this->get_type()==FLOAT64) {
			this->data.f64 = this->data.f64 - 1.0;
		}
		return *this;
	};
	const Object Object::operator--(int) {
		Object *ret = new Object();
		if (this->get_type()<=UINT64) {
			ret->type     = UINT64;
			ret->data.u64 = this->data.u64;
			--this->data.u64;
		} else if (this->get_type()<=INT64) {
			ret->type     = INT64;
			ret->data.s64 = this->data.s64;
			--this->data.s64;
		} else if (this->get_type()==FLOAT32) {
			ret->type      = FLOAT32;
			ret->data.f32  = this->data.f32;
			this->data.f32 = this->data.f32 - (float32_t)1.0;
		} else if (this->get_type() == FLOAT64) {
			ret->type      = FLOAT64;
			ret->data.f64  = this->data.f64;
			this->data.f64 = this->data.f64 - 1.0;
		}
		return *ret;
	};
	/* end of arithmetic operator */

	/* begin of logic operator */
	Object& Object::operator&&(const Object &obj) {
		bool part1 = true;
		bool part2 = true;
		if (this->type <= INT64) {
			part1 = (this->data.s64!=0);
		} else if (this->type == BOOL) {
			part1 = this->data.b;
		}
		if (obj.type <= INT64) {
			part2 = (obj.data.s64!=0);
		} else if (obj.type == BOOL) {
			part2 = obj.data.b;
		}
		Object *ret = new Object(part1&&part2);
		return *ret;
	};
	Object& Object::operator||(const Object &obj) {
		bool part1 = false;
		bool part2 = false;
		if (this->type <= INT64) {
			part1 = (this->data.s64!=0);
		} else if (this->type == BOOL) {
			part1 = this->data.b;
		}
		if (obj.type <= INT64) {
			part2 = (obj.data.s64!=0);
		} else if (obj.type == BOOL) {
			part2 = obj.data.b;
		}
		Object *ret = new Object(part1||part2);
		return *ret;
	};
	Object& Object::operator!() {
		bool value = true;
		if (this->type <= INT64) {
			value = (this->data.s64!=0);
		} else if (this->type == BOOL) {
			value = this->data.b;
		}
		Object *ret = new Object(!value);
		return *ret;
	};
	/* end of logic operator */

	/* begin of string operator */
	uint64_t Object::str_len() const {
		if (this->type == CHAR_PTR) {
			return strlen(this->data.str);
		} else {
			return 0;
			puts("error: str_len: operand is not string type");
		}
	};
	uint64_t Object::find_str(Object obj, uint64_t off) const{
		if (this->type == CHAR_PTR) {
			string tmp = this->data.str;
			return tmp.find(obj.data.str, off);
		} else {
			puts("error: find_str: operand is not string type");
			return -1;
		}
	};
	uint64_t Object::rfind_str(Object obj, uint64_t off) const{
		if (this->type == CHAR_PTR) {
			string tmp = this->data.str;
			return tmp.rfind(obj.data.str, off);
		} else {
			puts("error: rfind_str: operand is not string type");
			return -1;
		}
	};
	void Object::append_str(Object &obj) {
		if (this->type==CHAR_PTR && obj.get_type()==CHAR_PTR) {
			uint64_t len1 = strlen(this->data.str);
			uint64_t len2 = strlen(obj.data.str);
			this->data.str = (char*)realloc(this->data.str,len1+len2+1);
			uint64_t src = 0;
			uint64_t dest = len1;
			while (obj.data.str[src] != '\0') {
				this->data.str[dest] = obj.data.str[src];
				++src;
				++dest;
			}
			this->data.str[len1+len2] = '\0';
		} else {
			puts("error: concat_str: operand is not string type");
		}
	};
	void Object::trunc_str(uint64_t pos) {
		if (this->type == CHAR_PTR) {
			if (strlen(this->data.str) >= pos+1) {
				this->data.str[pos] = '\0';
			}
		} else {
			puts("error: trunc_str: operand is not string type");
		}
	};
	void Object::insert_str(const Object &obj, uint64_t pos) {
		if (this->type==CHAR_PTR && obj.get_type()==CHAR_PTR) {
			uint64_t len1 = strlen(this->data.str);
			uint64_t len2 = strlen(obj.data.str);
			this->data.str = (char*)realloc(this->data.str,len1+len2+1);
			this->data.str[len1+len2] = '\0';
			uint64_t src  = len1-1;
			uint64_t dest = len1+len2-1;
			while (src != pos-1) {
				this->data.str[dest] = this->data.str[src];
				--src;
				--dest;
			}
			uint64_t k = 0;
			src  = 0;
			dest = pos;
			while (k < len2) {
				this->data.str[dest] = obj.data.str[src];
				++src;
				++dest;
				++k;
			}
		} else {
			puts("error: insert_str: operand is not string type");
		}
	};
	void Object::replace_str(const Object &obj, uint64_t pos) {
		if (this->type==CHAR_PTR && obj.get_type()==CHAR_PTR) {
			uint64_t len1 = strlen(this->data.str);
			uint64_t len2 = strlen(obj.data.str);
			if (pos+len2 > len1) {
				this->data.str = (char*)realloc(this->data.str,pos+len2+1);
			}
			int dest = pos;
			int src  = 0;
			while (obj.data.str[src]!= '\0') {
				this->data.str[dest] = obj.data.str[src];
				++dest;
				++src;
			}
		} else {
			puts("error: replace_str: operand is not string type");
		}
	};
	
	void Object::erase_str(uint64_t t_beg, uint64_t t_end) {
		if (this->type == CHAR_PTR) {
			if (strlen(this->data.str)>=t_beg+1 && t_end>=t_beg) {
				uint64_t dest = t_beg;
				uint64_t src  = t_end;
				uint64_t len  = strlen(this->data.str);
				while (src<len && this->data.str[src]!='\0') {
					this->data.str[dest] = this->data.str[src];
					++src;
					++dest;
				}
				this->data.str[dest] = '\0';
			}
		} else {
			puts("error: erase_str: operand is not string type");
		}
	};
	/* end of string operator */



	/* print function used for debug */
	/* it will attemp to print it as nice as possible */
	void Object::print(int level) const{
		for (int i=0; i<level; ++i) {
			printf("\t");
		}
		switch(this->type) {
		/* print the primative type */
		case NULL_TYPE: printf("null");break;
		case UINT8:		printf("%hu",this->data.u8 );break;
		case UINT16:	printf("%hu",this->data.u16);break;
		case UINT32:	printf("%u" ,this->data.u32);break;
		case UINT64:	printf("%llu",this->data.u64);break;
		case INT8:		printf("%hd",this->data.s8 );break;
		case INT16:		printf("%hd",this->data.s16);break;
		case INT32:		printf("%d" ,this->data.s32);break;
		case INT64:		printf("%lld",this->data.s64);break;
		case FLOAT32:   printf("%f" ,this->data.f32);break;
		case FLOAT64:	printf("%lf",this->data.f64);break;
		case BOOL:      if (this->data.b) {
							printf("true");
						} else {
							printf("false");
						};
						break;
		case CHAR_PTR:	printf("\"%s\"",this->data.str);break;

		/* print the structure type */
		case VEC_PTR:	printf("[\n");
						for (auto it=(*(this->data.vec)).begin(); it!=(*this->data.vec).end(); ++it) {
							if (it != this->data.vec->begin()) {
								printf(" ,\n");
							}
							(*it)->print(level+1);
						}
						printf("\n");
						for (int i=0; i<level; ++i) {
							printf("\t");
						}
						printf("]");
						break;

		case MAP_PTR:	printf("{\n");
						for(auto it=(*this->data.map).begin(); it!=(*this->data.map).end(); ++it) {
							if (it != this->data.map->begin()) {
								printf(" ,\n");
							}
							/* it->first is char*, so we can not use print(level+1) */
							for (int i=0; i<level+1; ++i) {
								printf("\t");
							}
							printf("\"%s\" : ",(*it).first);
							if (it->second->get_type() == VEC_PTR ||
								it->second->get_type() == MAP_PTR) {
									printf("\n");
									it->second->print(level+2);
							} else {
								it->second->print(0);
							}
						}
						printf("\n");
						for (int i=0; i<level; ++i) {
							printf("\t");
						}
						printf("}");
						break;
		}
	}
	void Object::print_addr(int level) const{
		for (int i=0; i<level; ++i) {
			printf("\t");
		}
		switch(this->type) {
		/* print the primative type */
		default :		printf("%llu", (uint64_t)&this->data);break;

		/* print the structure type */
		case VEC_PTR:	printf("[\n");
						for (auto it=(*(this->data.vec)).begin(); it!=(*this->data.vec).end(); ++it) {
							if (it != this->data.vec->begin()) {
								printf(" ,\n");
							}
							(*it)->print_addr(level+1);
						}
						printf("\n");
						for (int i=0; i<level; ++i) {
							printf("\t");
						}
						printf("]");
						break;

		case MAP_PTR:	printf("{\n");
						for(auto it=(*this->data.map).begin(); it!=(*this->data.map).end(); ++it) {
							if (it != this->data.map->begin()) {
								printf(" ,\n");
							}
							/* it->first is char*, so we can not use print(level+1) */
							for (int i=0; i<level+1; ++i) {
								printf("\t");
							}
							printf("%llu : ",(uint64_t)&((*it).first));
							if (it->second->get_type() == VEC_PTR ||
								it->second->get_type() == MAP_PTR) {
									printf("\n");
									it->second->print_addr(level+2);
							} else {
								it->second->print_addr(0);
							}
						}
						printf("\n");
						for (int i=0; i<level; ++i) {
							printf("\t");
						}
						printf("}");
						break;
		}
	}
/************* end of member function of class **************/


/* three important friend functions */
Object* clone(const Object *obj) {
 	Object *clone_obj   = new Object();
	clone_obj->type     = obj->type;
	switch (obj->type) {
	case NULL_TYPE: clone_obj->data.u64 = 0           ; break;
	case UINT8:		clone_obj->data.u8  = obj->data.u8 ; break;
	case UINT16:	clone_obj->data.u16 = obj->data.u16; break;
	case UINT32:	clone_obj->data.u32 = obj->data.u32; break;
	case UINT64:	clone_obj->data.u64 = obj->data.u64; break;
	case INT8:		clone_obj->data.s8  = obj->data.s8 ; break;
	case INT16:		clone_obj->data.s16 = obj->data.s16; break;
	case INT32:		clone_obj->data.s32 = obj->data.s32; break;
	case INT64:		clone_obj->data.s64 = obj->data.s64; break;
	case FLOAT32:   clone_obj->data.f32 = obj->data.f32; break;
	case FLOAT64:   clone_obj->data.f64 = obj->data.f64; break;
	//case MPZ_PTR:		clone_obj->data.s64 = obj->data.s64; break;
	//case MPQ_PTR:		clone_obj->data.s64 = obj->data.s64; break;
	//case MPF_PTR:		clone_obj->data.s64 = obj->data.s64; break;
	case BOOL:      clone_obj->data.b   = obj->data.b  ; break;
	case CHAR_PTR:	clone_obj->data.str = strdup(obj->data.str); break;

	case VEC_PTR:	clone_obj->data.vec = new vector<Object*>();
					for (auto it=obj->data.vec->begin(); it!=obj->data.vec->end(); ++it) {
						Object *t = clone(*it);
						clone_obj->data.vec->push_back( t );
					}
					break;

	case MAP_PTR:	clone_obj->data.map = new unordered_map<char*,Object*>();
					for(auto it=(*(obj->data.map)).begin(); it!=(*(obj->data.map)).end(); ++it) {
						Object *t = clone(it->second);
						(*clone_obj->data.map)[it->first] = t;
					};
					break;
	}
	return clone_obj;
}
Object& parse(const char *str) {
	const char *ptr = str;

	skip_ws_ctrl(ptr);

	if ((*ptr) == '{') {
		return parse_object(str, ptr);
	} else if ((*ptr) == '[' ) {
		return parse_array(str, ptr);
	} else {
		puts("error: wrong root format.");
		Object *ret = new Object();
		return *ret;
	}
}

char* dump(const Object &obj) {
	string ret_str;
	char *buffer = (char*)calloc(128,sizeof(char));
		switch(obj.get_type()) {
		/* dump the primative type */
		case NULL_TYPE: sprintf(buffer,"null"); return buffer;
		case UINT8:		sprintf(buffer,"%hu",obj.get_uint8()); return buffer;
		case UINT16:	sprintf(buffer,"%hu",obj.get_uint16()); return buffer;
		case UINT32:	sprintf(buffer,"%u" ,obj.get_uint32()); return buffer;
		case UINT64:	sprintf(buffer,"%llu",obj.get_uint64()); return buffer;
		case INT8:		sprintf(buffer,"%hd",obj.get_int8() ); return buffer;
		case INT16:		sprintf(buffer,"%hd",obj.get_int16()); return buffer;
		case INT32:		sprintf(buffer,"%d" ,obj.get_int32()); return buffer;
		case INT64:		sprintf(buffer,"%lld",obj.get_int64()); return buffer;
		case FLOAT32:   sprintf(buffer,"%f" ,obj.get_float32()); return buffer;
		case FLOAT64:	sprintf(buffer,"%lf",obj.get_float64()); return buffer;
		case BOOL:      if (obj.get_bool()) {
							sprintf(buffer, "true");
						} else {
							sprintf(buffer, "false");
						};
						return buffer;
		case CHAR_PTR:	buffer = (char*)realloc(buffer, (strlen(obj.get_char_ptr())+3) );
						sprintf(buffer, "\"%s\"",obj.get_char_ptr());
						return buffer;

		/* dump the structure type */
		case VEC_PTR:	ret_str.append("[");
						for (auto it=(*(obj.get_vec())).begin(); it!=(*obj.get_vec()).end(); ++it) {
							if (it != obj.get_vec()->begin()) {
								ret_str.append(",");
							}
							char *ele = dump(**it);
							ret_str.append(ele);
							//printf("%s\n",buffer);
						}
						ret_str.append("]");
						free(buffer);
						buffer = strdup(ret_str.c_str());
						return buffer;

		case MAP_PTR:	ret_str.append("{");
						for(auto it=(*obj.get_map()).begin(); it!=(*obj.get_map()).end(); ++it) {
							if (it != obj.get_map()->begin()) {
								ret_str.append(",");
							}
							/* it->first is char*, so we can not use print(level+1) */
							ret_str.append("\"");
							ret_str.append(it->first);
							ret_str.append("\":");
							char *ele = dump(*(it->second));
							ret_str.append(ele);
						}
						ret_str.append("}");
						free(buffer);
						buffer = strdup(ret_str.c_str());
						return buffer;
		}
	return buffer;
}
/* end of friend functions */


/****************** begin of local parse function *******************/

/* we must deal with whitespace carefully */
void skip_ws_ctrl(const char *&ptr) {
	while ((*ptr)<=' ' || (*ptr)==127) {
		++ptr; 
	}
}

/*************************************
       define of object use BNF
**************************************
object ::=
	{}
	{ members }
members ::=
	pair
	pair , members
pair ::=
	string : value
***************************************/
static Object& parse_object(const char *str, const char *&ptr) {
	
	skip_ws_ctrl(ptr); /* carefully, there may be whitespace */

	if ((*ptr) == '{') {
		++ptr; /* skip the { */

		skip_ws_ctrl(ptr); /* carefully, there may be whitespace */

		Object *ret = new Object();
		ret->null_to_map();
		while ((*ptr) != '}') {
			if ((*ptr) != ',') {
				Object *key = &parse_string(str, ptr);
				
				skip_ws_ctrl(ptr); /* carefully, there may be whitespace */

				if (*ptr == ':') {
					++ptr; /* skip the : */
					Object *value = &parse_value(str, ptr);
					(*ret)[key->get_char_ptr()] = *value;
				} else {
					puts("error: wrong format of object.");
				}
			} else {
				++ptr; /* skip the ,*/
			}
			skip_ws_ctrl(ptr); /* carefully, there may be whitespace */
		}
		++ptr; /* skip the } */
		return *ret;
	} else {
		Object *ret = new Object();
		return *ret;
	}
}

/*************************************
       define of array use BNF
**************************************
array ::=
	[]
	[ elements ]
elements ::=
	value 
	value , elements
value ::=
	string
	number
	object
	array
	true
	false
	null
**************************************/
static Object& parse_array(const char *str, const char *&ptr) {
	
	skip_ws_ctrl(ptr);

	Object *ret = new Object();
	if ((*ptr) == '[') {
		++ptr; /* skip the [ */

		skip_ws_ctrl(ptr); /* carefully, there may be whitespace */

		ret->null_to_vec();
		int k = 0;
		while ((*ptr) != ']') {
			if ((*ptr) != ',') {
				(*ret)[k] = parse_value(str, ptr);
			} else {
				++ptr; /* skip the , */
				++k;
			}

			skip_ws_ctrl(ptr); /* carefully, there may be whitespace */
		}
		++ptr; /* skip the ] */
		return *ret;
	} else {
		puts("error: wrong format of array.");
		return *ret;
	}
}

/*************************************
       define of value use BNF
**************************************
value ::=
	string
	number
	object
	array
	true
	false
	null
**************************************/
static Object& parse_value(const char *str, const char *&ptr) {

	skip_ws_ctrl(ptr);

	if ((*ptr) == '"') {
		return parse_string(str, ptr);
	} else if ((*ptr)>='0' && (*ptr)<='9') {
		return parse_number(str, ptr);
	} else if ((*ptr)=='+' || (*ptr)=='-') {
		return parse_number(str, ptr);
	} else if ((*ptr)=='{') {
		return parse_object(str, ptr);
	} else if ((*ptr)=='[') {
		return parse_array(str, ptr);
	} else if ((*ptr)=='t') {
		return parse_true(str, ptr);
	} else if ((*ptr)=='f') {
		return parse_false(str, ptr);
	} else if ((*ptr)=='n') {
		return parse_null(str, ptr);
	} else {
		puts("error: wrong format of value.");
		Object *ret = new Object();
		return *ret;
	}
}

/*************************************
       define of string use BNF
**************************************
string ::=
	""
	" chars "
chars ::=
	char
	char chars
char ::=
	any-Unicode-character-except-"-or-\-or-control-character
	\"
	\\
	\/
	\b
	\f
	\n
	\r
	\t
	\u four-hex-digits
**************************************/
static Object& parse_string(const char *str, const char *&ptr) {
	/* find the string begin with ", end with " */
	const char *beg_ptr = ptr + 1; /* skip the first " */
	const char *end_ptr = ptr + 1;
	while ((*end_ptr) != '"') {
		++end_ptr;
	}
	int size = (end_ptr-beg_ptr); 

	ptr = end_ptr+1; /* skip the string */

	char *dup = new char[size+1]; /* +1 for \0 */
	memcpy(dup,beg_ptr,size);
	dup[size] = '\0';
	Object *tmp = new Object(dup);
	return *tmp;
}

/*************************************
       define of number use BNF
**************************************
	number ::=
		int
		int frac
		int exp
		int frac exp
	int ::=
		digit
		digit1-9 digits 
		- digit
		- digit1-9 digits
	frac ::=
		. digits
	exp ::=
		e digits
	digits ::=
		digit
		digit digits
	e ::=
		e
		e+
		e-
		E
		E+
		E-
**************************************/
static Object& parse_number(const char *str, const char* &ptr) {
	int     type         = INT64; /* we are not sure the type at the begining */
	int     flag         = 1;         /* 1 or -1 */
	int64_t result_int   = 0;         /* integer part */
	double  result_float = 0.0;       /* float   part */
	int     exp_flag     = 1;
	int     exp          = 0;         /* 10^0 = 1 as the exp */
	
	if (ptr[0] == '-') {
		flag = -1;
		++ptr;
	} else if (ptr[0] == '+') {
		++ptr;
	}

	while ('0'<=(*ptr) && (*ptr)<='9') {
		result_int *= 10;         /**/
		result_int += (*ptr)-'0'; /**/
		++ptr;
	}

	if ((*ptr) == '.') {
		type = FLOAT64;
		++ptr;
		double frac_exp = 1;
		while ('0'<=(*ptr) && (*ptr)<= '9') {
			frac_exp     *= 0.1;       /**/
			result_float += ((*ptr)-'0')*frac_exp; /**/
			++ptr;
		}
	}

	if ((*ptr) == 'e' || (*ptr)=='E') {
		++ptr;
		if ((*ptr) == '-'){
			exp_flag = -1;
			++ptr;
		} else if ((*ptr) == '+') {
			++ptr;
		}
		while ('0'<=(*ptr) && (*ptr)<='9') {
			exp *= 10;
			exp += (*ptr) - '0';
			++ptr;
		}
	}

	/* calculate power */
	double pow = 1.0;
	double tmp = 10.0;
	if (exp_flag == -1) {
		type = FLOAT64; /* with -1 exp_flag, value must be float */
		tmp  = 0.1;
	} 

	while (exp != 0) {
		if ((exp&1) == 0) {
			tmp = tmp*tmp;
			exp = exp>>1;
		} else {
			pow *= tmp;
			--exp;
		}
	}
	/* end of calculate power */
	
	Object *ret = NULL; 
	if (type == INT64) {
		int64_t result = flag * result_int * (int64_t)pow;
		ret = new Object(result);
	} else {
		double  result = flag *(result_int+result_float)*pow;
		ret = new Object(result);
	}

	return *ret;
}


/*************************************
       define of \uxxxx use BNF
**************************************
x ::= 0-9 | a-f | A-F
**************************************/
/* only pase four hex digits for unicode */
uint16_t parse_hex_digits(const char* str, char* &ptr) {
	if (ptr[0]=='\\' && ptr[1]=='u') {
		ptr = ptr+2;
	}

	uint16_t result = 0;
	for (int i=0; i<4; ++i) {
		result =  result<<4;
		int tmp = 0;
		if ('0'<=ptr[i] && ptr[i]<='9') {
			tmp = ptr[i] - '0';
		} else if ('a'<=ptr[i] && ptr[i]<='f') {
			tmp = ptr[i] - 'a' + 10;
		} else if ('A'<=ptr[i] && ptr[i]<='F') {
			tmp = ptr[i] - 'A' + 10;
		} else {
			puts("error unicode fomat");
		}
		result += tmp;
	}
	ptr += 4;
	return result;
}

/*************************************
       define of true use BNF
**************************************
true ::= true
**************************************/
static Object& parse_true(const char *str, const char *&ptr) {
	if (ptr[0] == 't' &&
		ptr[1] == 'r' &&
		ptr[2] == 'u' &&
		ptr[3] == 'e') {
			ptr += 4;
			Object *ret = new Object(true);
			return *ret;
	} else {
		puts("error: wrong true format.");
		Object *ret = new Object();
		return *ret;
	}
}

/*************************************
       define of false use BNF
**************************************
false ::= false
**************************************/
static Object& parse_false(const char *str, const char *&ptr) {
	if (ptr[0] == 'f' &&
		ptr[1] == 'a' &&
		ptr[2] == 'l' &&
		ptr[3] == 's' &&
		ptr[4] == 'e') {
			ptr += 5;
			Object *ret = new Object(false);
			return *ret;
	} else {
		puts("error: wrong false format.");
		Object *ret = new Object();
		return *ret;
	}
}

/*************************************
       define of null use BNF
**************************************
null ::= null
**************************************/
static Object& parse_null(const char *str, const char *&ptr) {
	if (ptr[0] == 'n' &&
		ptr[1] == 'u' &&
		ptr[2] == 'l' &&
		ptr[3] == 'l') {
			ptr += 4;
			Object *ret = new Object();
			return *ret;
	} else {
		puts("error: wrong null format.");
		Object *ret = new Object();
		return *ret;
	}
}
/****************** end of local function *********************/





//int main(int argc, char* argv[]) {
//
//	//*/ test for normal parse
//	const char   *str = "[                                                            \
//		              1, -2, 3.4, -5.6, 7.8e+9, -12.3E-4, true, false, null,    \
//					  \"hello c++ json object\",                              \
//					  [ 1, -2, 3.4, -5.6, 7.8e+9, -12.3E-4, true, false, null ],\
//					  {                                                         \
//					    \"ÐÕÃû\"  : \"ÍôÎµ\",                                   \
//						\"name\"  : \"wang wei\",                               \
//						\"school\": \"pku\",                                    \
//						\"age\"   : 25                                          \
//					  }                                                         \
//					]";
//	Object &test1 = parse(str);
//	test1.print();
//	printf("\n");
//	test1.print_addr();
//	printf("\n");
//	//*/
//	
//	//*/ test for vector constructor
//	Object test2;
//	test2[0] = (uint8_t) 1;
//	test2[1] = (uint16_t)2;
//	test2[2] = (uint32_t)4;
//	test2[3] = (uint64_t)8;
//	/* test2[4] will be null */
//	test2[5] = (int8_t) -1;
//	test2[6] = (int16_t)-2;
//	test2[7] = (int32_t)-4;
//	test2[8] = (int64_t)-8;
//
//	test2[10] = (float32_t)7.2;
//	test2[11] = (float64_t)-123.456e+7;
//
//	test2[13] = false;
//	test2[15] = "hello c++ json object";
//
//	test2[17] = test1;
//
//	test2.print();
//	printf("\n\n");
//	//*/ end of test for vector constructor;
//
//	//*/ test for map constructor
//	Object test3;
//	test3["zero"]  = (uint8_t) 1;
//	test3["one"]   = (uint16_t)2;
//	test3["two"]   = (uint32_t)4;
//	test3["three"] = (uint64_t)8;
//
//	test3["five"]  = (int8_t) -1;
//	test3["six"]   = (int16_t)-2;
//	test3["seven"] = (int32_t)-4;
//	test3["eight"] = (int64_t)-8;
//
//	test3["ten"]    = (float32_t)7.2;
//	test3["eleven"] = (float64_t)-123.456e+7;
//
//	test3["thirteen"]  = false;
//
//	test3["fifteen"]   = "hello c++ json object";
//
//	test3["seventeen"] = test1;
//
//	test3.print();
//	printf("\n\n");
//
//
//
//	//*/ end of test for vector constructor;
//
//	//*/ test for arithmetic operator
//	const int len = 14;
//	Object part1[] = { 5, -2,  15, 1.2,    3,    -6, 23.567,       6, -23.567, -1.28, 12.3, 178.2, -3.14, -199.0};
//	Object part2[] = { 7,  4, -12,   2,  7.8, 1.223,     -7,  -1.223,       7, -2.67, 12.3, -0.93, 128.6, -196.8}; 
//	printf("len:%d\n",sizeof(part1)/sizeof(Object));
//	Object test4;
//	for (int i=0; i<len; ++i) {
//	#define NICE_PRINT_EXPR_2( OPRAND1, OPERATOR, OPRAND2, RESULT ) do {  \
//		    OPRAND1.print();                                              \
//			printf( #OPERATOR );                                          \
//			OPRAND2.print();                                              \
//			printf(" ::= ");                                              \
//			RESULT = OPRAND1 OPERATOR OPRAND2;                            \
//			RESULT.print();                                               \
//			printf("\n");                                                 \
//	} while(0)
//	#define NICE_PRINT_EXPR_1_prefix( OPERATOR, OPRAND ) do {             \
//			printf( #OPERATOR );                                          \
//            printf("(");                                                  \
//		    OPRAND.print();                                               \
//			printf(")");                                                  \
//			printf(" ::= ");                                              \
//			(OPERATOR OPRAND).print();                                    \
//			printf("\n");                                                 \
//	} while(0)
//	#define NICE_PRINT_EXPR_1_suffix( OPRAND, OPERATOR ) do {             \
//			printf("(");                                                  \
//		    OPRAND.print();                                               \
//			printf(")");                                                  \
//            printf( #OPERATOR );                                          \
//			printf(" ::= ");                                              \
//			(OPRAND OPERATOR).print();                                    \
//			printf("\n");                                                 \
//	} while(0)
//
//		NICE_PRINT_EXPR_2( part1[i], +, part2[i], test4);
//		NICE_PRINT_EXPR_2( part1[i], -, part2[i], test4);
//		NICE_PRINT_EXPR_2( part1[i], *, part2[i], test4);
//		NICE_PRINT_EXPR_2( part1[i], /, part2[i], test4);
//
//		part1[i].print();
//		printf(" %% "); /*because % is special, so we can't use NICE_PRINT*/
//		part2[i].print();
//		printf(" == ");
//		test4 = part1[i] % part2[i];
//		test4.print();
//		printf("\n");
//
//		NICE_PRINT_EXPR_1_prefix(++, test4);
//		NICE_PRINT_EXPR_1_prefix(--, test4);
//		NICE_PRINT_EXPR_1_suffix(test4, ++);
//		NICE_PRINT_EXPR_1_suffix(test4, --);
//		printf("\n");
//	}
//	//*/ end of arithmetic operator test
//
//	//*/ test for logic operator
//	const int logic_test_len = 12;
//	Object part3[] = { true, true,  false, false,    1,    2,     true,  false,     0,    0,   true,  false};
//	Object part4[] = { true, false, true,  false,    true, false,     1,     2,  true, false,  0,     0    }; 
//	Object test5;
//	for (int i=0; i<logic_test_len; ++i) {
//		NICE_PRINT_EXPR_2(part3[i], &&, part4[i], test5);
//		NICE_PRINT_EXPR_2(part3[i], ||, part4[i], test5);
//		NICE_PRINT_EXPR_1_prefix(!, test5);
//	}
//	test5 = 0;
//	NICE_PRINT_EXPR_1_prefix(!, test5);
//	test5 = 1;
//	NICE_PRINT_EXPR_1_prefix(!, test5);
//	test5 = 2;
//	NICE_PRINT_EXPR_1_prefix(!, test5);
//	//*/ end of logic operator test
//
//	printf("\n");
//
//	//*/ begin of string function test
//	Object test6 = "I want to implement an universal data structure. Yes, I mean universal! --by wang wei.";
//	Object test7 = "universal";
//	printf("src string:\n");
//	test6.print();
//	printf("\n");
//	printf("op  string:\n");
//	test7.print();
//	printf("\n");
//
//	int src_len = test6.str_len();
//	printf("str_len : %d\n", src_len);
//
//	printf("find_str : %llu\n",test6.find_str(test7));
//
//	printf("rfind_str : %llu\n",test6.rfind_str(test7));
//
//	test6.insert_str(test7, 7);
//	printf("after insert:\n");
//	test6.print();
//	printf("\n");
//	
//	test6.replace_str("simple",23);
//	printf("after replace:\n");
//	test6.print();
//	printf("\n");
//
//	test6.erase_str(23,29);
//	printf("after erase:\n");
//	test6.print();
//	printf("\n");
//
//	
//	test6.append_str(test7);
//	printf("after append:\n");
//	test6.print();
//	printf("\n");
//
//	test6.trunc_str(src_len);
//	printf("after trunc:\n");
//	test6.print();
//	printf("\n");
//	//*/ end of string function test
//
//	//* begin of compare operator test
//	const int comp_test_len = 12;
//	Object part5[] = { 1, -2345,  234.5,    283,  3.141,    1,  true,  "wang",     0,    "hello",   true,  "that's it"  };
//	Object part6[] = { 2,   656,    267,  365.6, -2.718, true,     0,       2,  "wei",    false,   "world", "this is it"}; 
//	Object test8;
//	for (int i=0; i<comp_test_len; ++i) {
//		printf("\n");
//		NICE_PRINT_EXPR_2(part5[i], <, part6[i], test8);
//		NICE_PRINT_EXPR_2(part5[i], >, part6[i], test8);
//		NICE_PRINT_EXPR_2(part5[i], ==, part6[i], test8);
//		NICE_PRINT_EXPR_2(part5[i], !=, part6[i], test8);
//		NICE_PRINT_EXPR_2(part5[i], <=, part6[i], test8);
//		NICE_PRINT_EXPR_2(part5[i], >=, part6[i], test8);
//	}
//	//*/ end of compare operator test
//	char *dump_str = dump(test1);
//	printf("%s\n", dump_str);
//	Object test9 = parse(dump_str);
//	test9.print();
//	printf("\n");
//	getchar();
//    return 0;
//}

