#include "keydb.h"

#include <glib.h>


const char *LOG_DOMAIN="KeyDb";

// the primary hash table:
GHashTable *table = NULL;

typedef struct value_struct {
  // any of these could be '0' meaning 'always valid'
  time_t start_valid;
  time_t end_valid;

} value_struct;

int reload_db_from_file(const char * fname) {

  GHashTable * new_table = g_hash_table_new_full(g_str_hash, g_str_equal, g_free, g_free);
  

}


int check_code(const char * code) {
  if (table == NULL) {
    g_critical("Check_code called on null database - please init db first with reload_db_from_file!");
  }

}
