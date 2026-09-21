#ifndef KV_H
#define KV_H

#include <stdlib.h>

#define TOMBSTONE ((char *)0x1)

typedef struct {
  char *key;
  char *value;
} kv_entry_t;

typedef struct {
  size_t capacity;
  size_t count;
  kv_entry_t *entries;
} kv_t;

/// @brief Initialized a new key-value database.
/// @param[in] capacity The capacity of the new database being made.
/// @return returns a pointer to the newly created database, if error occurs
/// NULL will be returned.
kv_t *kv_init(size_t capacity);

/// @brief Puts a key-value pair into the database
/// @details Puts a key-value pair into the data base, if the space is free
/// inserts there, if the space is occupied increments the index until an empty
/// spot is found
/// @param[in] db A pointer to the database to insert into.
/// @param[in] key A pointer to the key value.
/// @param[in] value A pointer to the value associated with the given key.
/// @return Returns the index of the key unless an error occurs then -1 is
/// returned.
int kv_put(kv_t *db, char *key, char *value);

/// @brief Searches for the provided key in the database and returns the
/// associated value.
/// @details Will search for the provided key, skipping over tombstones but
/// returning NULL for the first NULL value found or if the key is not found.
/// @param[in] db A pointer to the database to search.
/// @param[in] key The key to search for in the database.
/// @return Returns the associated value if found otherwise NULL is returned.
char *kv_get(kv_t *db, char *key);

#endif
