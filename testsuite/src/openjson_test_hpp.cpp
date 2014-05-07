#include <stdio.h>
#include "openjson.hpp"

int main(int argc, char* argv[]) {

	//*/ test for normal parse
	const char   *str = "[                                                            \
		              1, -2, 3.4, -5.6, 7.8e+9, -12.3E-4, true, false, null,    \
					  \"hello c++ json object\",                              \
					  [ 1, -2, 3.4, -5.6, 7.8e+9, -12.3E-4, true, false, null ],\
					  {                                                         \
						\"name\"  : \"wang wei\",                               \
						\"school\": \"pku\",                                    \
						\"age\"   : 25                                          \
					  }                                                         \
					]";
	Object &test1 = parse(str);
	test1.print();
	printf("\n");
	test1.print_addr();
	printf("\n");
	//*/
	
	//*/ test for vector constructor
	Object test2;
	test2[0] = (uint8_t) 1;
	test2[1] = (uint16_t)2;
	test2[2] = (uint32_t)4;
	test2[3] = (uint64_t)8;
	/* test2[4] will be null */
	test2[5] = (int8_t) -1;
	test2[6] = (int16_t)-2;
	test2[7] = (int32_t)-4;
	test2[8] = (int64_t)-8;

	test2[10] = (float32_t)7.2;
	test2[11] = (float64_t)-123.456e+7;

	test2[13] = false;
	test2[15] = "hello c++ json object";

	test2[17] = test1;

	test2.print();
	printf("\n\n");
	//*/ end of test for vector constructor;

	//*/ test for map constructor
	Object test3;
	test3["zero"]  = (uint8_t) 1;
	test3["one"]   = (uint16_t)2;
	test3["two"]   = (uint32_t)4;
	test3["three"] = (uint64_t)8;

	test3["five"]  = (int8_t) -1;
	test3["six"]   = (int16_t)-2;
	test3["seven"] = (int32_t)-4;
	test3["eight"] = (int64_t)-8;

	test3["ten"]    = (float32_t)7.2;
	test3["eleven"] = (float64_t)-123.456e+7;

	test3["thirteen"]  = false;

	test3["fifteen"]   = "hello c++ json object";

	test3["seventeen"] = test1;

	test3.print();
	printf("\n\n");



	//*/ end of test for vector constructor;

	//*/ test for arithmetic operator
	const int len = 14;
	Object part1[] = { 5, -2,  15, 1.2,    3,    -6, 23.567,       6, -23.567, -1.28, 12.3, 178.2, -3.14, -199.0};
	Object part2[] = { 7,  4, -12,   2,  7.8, 1.223,     -7,  -1.223,       7, -2.67, 12.3, -0.93, 128.6, -196.8}; 
	printf("len:%d\n",(int)(sizeof(part1)/sizeof(Object)));
	Object test4;
	for (int i=0; i<len; ++i) {
	#define NICE_PRINT_EXPR_2( OPRAND1, OPERATOR, OPRAND2, RESULT ) do {  \
		    OPRAND1.print();                                              \
			printf( #OPERATOR );                                          \
			OPRAND2.print();                                              \
			printf(" ::= ");                                              \
			RESULT = OPRAND1 OPERATOR OPRAND2;                            \
			RESULT.print();                                               \
			printf("\n");                                                 \
	} while(0)
	#define NICE_PRINT_EXPR_1_prefix( OPERATOR, OPRAND ) do {             \
			printf( #OPERATOR );                                          \
            printf("(");                                                  \
		    OPRAND.print();                                               \
			printf(")");                                                  \
			printf(" ::= ");                                              \
			(OPERATOR OPRAND).print();                                    \
			printf("\n");                                                 \
	} while(0)
	#define NICE_PRINT_EXPR_1_suffix( OPRAND, OPERATOR ) do {             \
			printf("(");                                                  \
		    OPRAND.print();                                               \
			printf(")");                                                  \
            printf( #OPERATOR );                                          \
			printf(" ::= ");                                              \
			(OPRAND OPERATOR).print();                                    \
			printf("\n");                                                 \
	} while(0)

		NICE_PRINT_EXPR_2( part1[i], +, part2[i], test4);
		NICE_PRINT_EXPR_2( part1[i], -, part2[i], test4);
		NICE_PRINT_EXPR_2( part1[i], *, part2[i], test4);
		NICE_PRINT_EXPR_2( part1[i], /, part2[i], test4);

		part1[i].print();
		printf(" %% "); /*because % is special, so we can't use NICE_PRINT*/
		part2[i].print();
		printf(" == ");
		test4 = part1[i] % part2[i];
		test4.print();
		printf("\n");

		NICE_PRINT_EXPR_1_prefix(++, test4);
		NICE_PRINT_EXPR_1_prefix(--, test4);
		NICE_PRINT_EXPR_1_suffix(test4, ++);
		NICE_PRINT_EXPR_1_suffix(test4, --);
		printf("\n");
	}
	//*/ end of arithmetic operator test

	//*/ test for logic operator
	const int logic_test_len = 12;
	Object part3[] = { true, true,  false, false,    1,    2,     true,  false,     0,    0,   true,  false};
	Object part4[] = { true, false, true,  false,    true, false,     1,     2,  true, false,  0,     0    }; 
	Object test5;
	for (int i=0; i<logic_test_len; ++i) {
		NICE_PRINT_EXPR_2(part3[i], &&, part4[i], test5);
		NICE_PRINT_EXPR_2(part3[i], ||, part4[i], test5);
		NICE_PRINT_EXPR_1_prefix(!, test5);
	}
	test5 = 0;
	NICE_PRINT_EXPR_1_prefix(!, test5);
	test5 = 1;
	NICE_PRINT_EXPR_1_prefix(!, test5);
	test5 = 2;
	NICE_PRINT_EXPR_1_prefix(!, test5);
	//*/ end of logic operator test

	printf("\n");

	//*/ begin of string function test
	Object test6 = "I want to implement an universal data structure. Yes, I mean universal! --by wang wei.";
	Object test7 = "universal";
	printf("src string:\n");
	test6.print();
	printf("\n");
	printf("op  string:\n");
	test7.print();
	printf("\n");

	int src_len = test6.str_len();
	printf("str_len : %d\n", src_len);

	printf("find_str : %lu\n",test6.find_str(test7));

	printf("rfind_str : %lu\n",test6.rfind_str(test7));

	test6.insert_str(test7, 7);
	printf("after insert:\n");
	test6.print();
	printf("\n");
	
	test6.replace_str("simple",23);
	printf("after replace:\n");
	test6.print();
	printf("\n");

	test6.erase_str(23,29);
	printf("after erase:\n");
	test6.print();
	printf("\n");

	
	test6.append_str(test7);
	printf("after append:\n");
	test6.print();
	printf("\n");

	test6.trunc_str(src_len);
	printf("after trunc:\n");
	test6.print();
	printf("\n");
	//*/ end of string function test

	//* begin of compare operator test
	const int comp_test_len = 12;
	Object part5[] = { 1, -2345,  234.5,    283,  3.141,    1,  true,  "wang",     0,    "hello",   true,  "that's it"  };
	Object part6[] = { 2,   656,    267,  365.6, -2.718, true,     0,       2,  "wei",    false,   "world", "this is it"}; 
	Object test8;
	for (int i=0; i<comp_test_len; ++i) {
		printf("\n");
		NICE_PRINT_EXPR_2(part5[i], <, part6[i], test8);
		NICE_PRINT_EXPR_2(part5[i], >, part6[i], test8);
		NICE_PRINT_EXPR_2(part5[i], ==, part6[i], test8);
		NICE_PRINT_EXPR_2(part5[i], !=, part6[i], test8);
		NICE_PRINT_EXPR_2(part5[i], <=, part6[i], test8);
		NICE_PRINT_EXPR_2(part5[i], >=, part6[i], test8);
	}
	//*/ end of compare operator test
	char *dump_str = dump(test1);
	printf("%s\n", dump_str);
	Object test9 = parse(dump_str);
	test9.print();
	printf("\n");
	getchar();
    return 0;
}

