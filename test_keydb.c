#include "keydb.h"
#include <stdio.h>
#include <glib.h>

time_t parse_time(const gchar*);


void test_parse_time() {
  g_assert(parse_time("123") == 123);
  g_assert(parse_time("a") == -1);
  g_assert(parse_time("") == -1);
  g_assert(parse_time("0") == 0);
  g_assert(parse_time(NULL) == -1);
}

int is_str_digits(const gchar *);

void test_is_str_digits() {
  g_assert(is_str_digits("1234") == 1);
  g_assert(!is_str_digits("abcd"));
  g_assert(!is_str_digits("123sdf"));
  g_assert(!is_str_digits("123_23"));
  g_assert(!is_str_digits(""));
  g_assert(!is_str_digits(NULL));

}

int main(int argc, char ** argv) {
  test_parse_time();
  test_is_str_digits();
}
