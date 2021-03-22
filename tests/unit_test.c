#include <CUnit/Basic.h>

#include "../src/lexer.h"

int init_suite(void)
{
    return 0;
}

int clean_suite(void)
{
    return 0;
}

const char *test1_desc = "1: \"(hello world)\"";
void test1(void)
{
    struct token *root = lexer_tokenizer("(hello world)");
    struct token *cursor = root;

    CU_ASSERT_STRING_EQUAL(lexer_next(&cursor), "(");
    CU_ASSERT_STRING_EQUAL(lexer_next(&cursor), "hello");
    CU_ASSERT_STRING_EQUAL(lexer_next(&cursor), "world");
    CU_ASSERT_STRING_EQUAL(lexer_next(&cursor), ")");

    lexer_free(root);
}

const char *test2_desc = "2: \"(fun () -3.0 144)\"";
void test2(void)
{
    struct token *root = lexer_tokenizer("(fun () -3.0 144)");
    struct token *cursor = root;

    CU_ASSERT_STRING_EQUAL(lexer_next(&cursor), "(");
    CU_ASSERT_STRING_EQUAL(lexer_next(&cursor), "fun");
    CU_ASSERT_STRING_EQUAL(lexer_next(&cursor), "(");
    CU_ASSERT_STRING_EQUAL(lexer_next(&cursor), ")");
    CU_ASSERT_STRING_EQUAL(lexer_next(&cursor), "-3.0");
    CU_ASSERT_STRING_EQUAL(lexer_next(&cursor), "144");
    CU_ASSERT_STRING_EQUAL(lexer_next(&cursor), ")");

    lexer_free(root);
}

const char *test3_desc = "3: \"\"";
void test3(void)
{
    struct token *root = lexer_tokenizer("(hello world)");
    struct token *cursor = root;

    CU_ASSERT_STRING_EQUAL(lexer_next(&cursor), "(");
    CU_ASSERT_STRING_EQUAL(lexer_next(&cursor), "hello");
    CU_ASSERT_STRING_EQUAL(lexer_next(&cursor), "world");
    CU_ASSERT_STRING_EQUAL(lexer_next(&cursor), ")");

    lexer_free(root);
}

int main(void)
{
    CU_pSuite test_suite = NULL;

    if (CUE_SUCCESS != CU_initialize_registry()) {
	    return CU_get_error();
    }

    test_suite = CU_add_suite("Tests", init_suite, clean_suite);

    if (!test_suite) {
	    CU_cleanup_registry();
	    return CU_get_error();
    }
    if (
        !CU_add_test(test_suite, test1_desc, test1) ||
        !CU_add_test(test_suite, test2_desc, test2)
	) {
	    CU_cleanup_registry();
	    return CU_get_error();
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    CU_cleanup_registry();
    return CU_get_error();
}
