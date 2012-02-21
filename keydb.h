#ifndef _KEYDB_H_
#define _KEYDB_H_

/*
 * An interface to a small in-memory database that can reloaded from disk on command.
 */




// Load the DB from the specified file. Returns number of entires on success, < 0 on error.
// Possible errors:
// - cant open file
// - cant read file/file empty
// - file contents way too small (eg: existing DB has X, new one has 1/10th of X)
// - ?
int reload_db_from_file(const char *);

// Check a code against DB. Returns:
// - 1 if code is GOOD
// - 0 if code is BAD
// Also logs
int check_code(const char *);



#endif
