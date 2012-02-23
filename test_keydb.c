#include "keydb.h"
#include <stdio.h>
#include <glib.h>
#include <sys/time.h>

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

double subt(struct timeval *start, struct timeval *end) {
  double secondPart = end->tv_sec - start->tv_sec;
  printf("Second: %f ", secondPart);
  double ms = (end->tv_usec - start->tv_usec) / 1000.0;
  printf("ms: %f\n", ms);
  return ms + secondPart*1000.0;
}


int main(int argc, char ** argv) {
  test_parse_time();
  test_is_str_digits();

  struct timeval start;
  struct timeval end;
  gettimeofday(&start, NULL);
  int l = reload_db_from_file("keyz2.txt");
  gettimeofday(&end, NULL);

  printf("Load time: %f (ms)\n",
	 subt(&start, &end));

  printf("Reloading db from file: %ld\n",
	 l);

  gettimeofday(&start, NULL);
  int c = check_code("1234");
  gettimeofday(&end, NULL);

  printf("Check time: %f (ms)\n",
	 subt(&start, &end));

  printf("Code: %d\n", c);

}
