#ifndef CHECK_ASSERTIONS_H_INCLUDED
#define CHECK_ASSERTIONS_H_INCLUDED


#include <check.h>


#define _ck_assert_bin(X, OP, Y, n) do { \
  const char* _ck_x = (X); \
  const char* _ck_y = (Y); \
  const size_t _digits = n*2; \
  ck_assert_msg(0 OP memcmp(_ck_y, _ck_x, n), \
    "Assertion '%s' failed: %s == \"0x%016x\", %s == \"0x%016x\"", #X" "#OP" "#Y, #X, *(uint64_t*)_ck_x, #Y, *(uint64_t*)_ck_y); \
} while (0)

/**
 * Check two binary strings to determine if 0==memcmp(X,Y,n)
 *
 * If not 0==memcmp(X,Y,n), the test fails.
 *
 * @param X string
 * @param Y string to compare against X
 * @param n number of bytes to compare
 *
 * @note If the check fails, the remaining of the test is aborted
 *
 * @since 0.9.6
 */
#define ck_assert_bin_eq(X, Y, n) _ck_assert_bin(X, ==, Y, n)

#endif // CHECK_ASSERTIONS_H_INCLUDED
