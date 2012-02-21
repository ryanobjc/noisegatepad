#include "keydb.h"
#include "noisegatepad.h"

#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <glib.h>


const char *LOG_DOMAIN="KeyDb";

// the primary hash table:
GHashTable *table = NULL;

typedef struct value_struct {
  // any of these could be '0' meaning 'always valid'
  time_t start_valid;
  time_t end_valid;
} value_struct;


int part_count(gchar **v) {
  int c = 0;
  while(v++ != NULL) {
    c++;
  }
  return c;
}

time_t parse_time(const gchar *s) {
  if (s == NULL) return -1;
  if (*s == '\0') return -1; // nothing

  // returns -1 if nothing there.
  gchar * end;
  time_t r = strtol(s, &end, 10);

  if (*end == '\0') return r;
  return -1;
}

int is_str_digits(const gchar *s) {
  if (s == NULL) return 0;
  if (*s == '\0') return 0;

  do {
    if (!g_ascii_isdigit(*s))
      return 0;
  } while (*++s != '\0');

  return 1;
}

int reload_db_from_file(const char * fname) {
  FILE * file = fopen(fname, "r");

  if (file == NULL) {
    g_warning("Unable to open db file: (%d): %s", errno, strerror(errno));
    return -1;
  }

  GHashTable * new_table = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
  char readbuf[1024];
  int cnt = 0;
  gchar ** parts = NULL;

  while (!feof(file)) {
    if (fgets(readbuf, 1024, file)) {
      cnt++;
      g_strfreev(parts);

      // ignore all-comment lines and empty lines.
      if (readbuf[0] == '#' || readbuf[0] == '\n') continue;
      gchar **parts = g_strsplit(readbuf, ",", 3);

      // check to see how many parts:
      if (part_count(parts) != 4) {
	g_warning("Line %d didn't have 4 parts", cnt);
	continue;
      }

      time_t start_valid = parse_time(parts[1]);
      time_t end_valid = parse_time(parts[2]);
      int disabled = parse_time(parts[3]);

      int failed = 0;

      if (strlen(parts[0]) > MAX_DIGITS ||
	  strlen(parts[0]) < MIN_DIGITS) {
	g_warning("Line %d, code was wrong length: %ld",
		  cnt, strlen(parts[0]));
	failed = 1;
      }

      if (!is_str_digits(parts[0])) {
	g_warning("Line %d, code was not all digits: \"%s\"",
		  cnt, parts[0]);
	failed = 1;
      }

      if (start_valid == -1) {
	g_warning("Line %d, start time was invalid: \"%s\"",
		  cnt, parts[1]);
	failed = 1;
      }
      if (end_valid == -1) {
	g_warning("Line %d, end time was invalid: \"%s\"",
		  cnt, parts[2]);
	failed = 1;
      }

      if (disabled != 0 || disabled != 1) {
	g_warning("Line %d, disabled field wasnt 0 or 1: %s",
		  cnt, parts[3]);
	failed = 1;
      }

      if (failed) continue;

      if (disabled) {
	g_message("Line %d ignoring disabled code",
		  cnt);
	continue;
      }

      gchar *code = g_strdup(parts[0]);
      // populate hashdb:
      value_struct * val = g_new(value_struct,1);
      val->start_valid = start_valid;
      val->end_valid = end_valid;

      g_hash_table_insert(new_table, code, val);

      g_message("Line %d: Added code %s, validity range %ld -> %ld",
		cnt, code, start_valid, end_valid);

    } else {
      g_warning("Unable to fgets line (%d): %s", cnt, strerror(errno));
    }
  }
  fclose(file);

  guint newsize = g_hash_table_size(new_table);

  if (table == NULL) {
    table = new_table;
    return newsize;
  }

  guint oldsize = g_hash_table_size(table);
  if (newsize < oldsize/2) {
    // dont update!
    g_warning("New database is %ud which is much smaller than %ud of existing! Not updating!",
	      newsize, oldsize);
    return -1;
  }

  table = new_table;
  return newsize;
}


int check_code(const char * code) {
  if (table == NULL) {
    g_critical("Check_code called on null database - please init db first with reload_db_from_file!");
  }
  value_struct * value = (value_struct *)g_hash_table_lookup(table, code);
  if (value == NULL)
    return 0;

  time_t now = time(NULL);
  if (value->start_valid != 0) {
    if (now < value->start_valid) {
      g_warning("Code %s used before valid start time of %ld", code, now);
      return 0;
    }
  }

  if (value->end_valid != 0) {
    if (now > value->end_valid) {
      g_warning("Code %s used after valid end time of %ld", code, now);
      return 0;
    }
  }
  g_warning("Code %s is valid", code);
  return 1;
}
