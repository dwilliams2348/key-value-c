#include <kv.h>
#include <string.h>

size_t hash(char *value, int capacity) {
    size_t hash = 0x1337133713371337;

    while (*value) {
        hash ^= *value;
        hash = hash << 8;
        hash += *value;

        value++;
    }

    return hash % capacity;
}

kv_t *kv_init(size_t capacity) {
    if (capacity == 0) {
        return NULL;
    }

    kv_t *table = malloc(sizeof(kv_t));
    if (table == NULL) {
        return NULL;
    }

    table->capacity = capacity;
    table->count = 0;

    table->entries = calloc(sizeof(kv_entry_t), capacity);
    if (table->entries == NULL) {
        return NULL;
    }

    return table;
}

int kv_put(kv_t *db, char *key, char *value) {
    if (!db || !key || !value) {
        return -1;
    }

    size_t idx = hash(key, db->capacity);
    // -1 means tomb was not encountered, a valid idx means we found a tomb we
    // are now looking if key already exists.
    int first_tomb = -1;

    for (int i = 0; i < db->capacity - 1; i++) {
        int real_idx = (idx + i) % db->capacity;
        kv_entry_t *entry = &db->entries[real_idx];

        // the key is already set, update the value
        if (entry->key && entry->key != TOMBSTONE && !strcmp(entry->key, key)) {
            // we do not know lifetime of value being passed in so duplicate it
            // to heap
            char *new_value = strdup(value);
            if (!new_value) {
                return -1;
            }

            entry->value = new_value;
            return 0;
        }

        // landing in an empty slot, whether NULL or tombstone
        if (!entry->key || entry->key == TOMBSTONE) {
            if (entry->key == TOMBSTONE) {
                // tombstone found at current idx
                if (first_tomb == -1) {
                    first_tomb = real_idx;
                }

                continue;
            }

            // lifetime of params not known, duplicate them.
            char *new_key = strdup(key);
            char *new_value = strdup(value);
            if (!new_key || !new_value) {
                // in case of one working and not the other free dup mem
                free(new_key);
                free(new_value);
                return -1;
            }

            entry->key = new_key;
            entry->value = new_value;
            db->count++;

            return 0;
        }
    }

    if (first_tomb != -1 && first_tomb < db->capacity) {
        char *new_key = strdup(key);
        char *new_value = strdup(value);
        if (!new_key || !new_value) {
            free(new_key);
            free(new_value);
            return -1;
        }

        db->entries[first_tomb].key = new_key;
        db->entries[first_tomb].value = new_value;
        db->count++;

        return 0;
    }

    // db is occupied
    return -2;
}

char *kv_get(kv_t *db, char *key) {
    if (!db || !key) {
        // either value is NULL thus cannot properly search
        return NULL;
    }

    size_t start = hash(key, db->capacity);
    for (int i = 0; i < db->capacity - 1; i++) {
        size_t real_idx = (start + i) % db->capacity;
        kv_entry_t *entry = &db->entries[real_idx];

        // if key is NULL return NULL since we wont find the value;
        if (!entry->key) {
            return NULL;
        }

        // if the key is TOMBSTONE the value can still be found in db, or if the
        // key is not the same.
        if (entry->key == TOMBSTONE || strcmp(key, entry->key) != 0) {
            continue;
        }

        if (strcmp(key, entry->key) == 0) {
            return entry->value;
        }
    }

    return NULL;
}

int kv_delete(kv_t *db, char *key) {
    if (!db || !key) {
        return -1;
    }

    size_t start = hash(key, db->capacity);
    for (int i = 0; i < db->capacity; i++) {
        size_t real_idx = (start + i) % db->capacity;
        kv_entry_t *entry = &db->entries[real_idx];

        if (!entry->key) {
            return -1;
        }

        if (entry->key == TOMBSTONE || strcmp(key, entry->key) != 0) {
            continue;
        }

        if (strcmp(key, entry->key) == 0) {
            free(entry->key);
            free(entry->value);

            entry->key = TOMBSTONE;
            db->count--;

            return 0;
        }
    }

    return -1;
}

void kv_free(kv_t *db) {
    // need to loop through all elements of array to delete the keys and values
    // then the entry itself
    for (int i = 0; i < db->capacity; i++) {
        if (db->entries[i].key == TOMBSTONE) {
            // do not delete, just skip.
            continue;
        }

        // first free the key and value.
        free(db->entries[i].key);
        free(db->entries[i].value);
    }

    // once all elements are freed first free the array of entries then the
    // database pointer.
    free(db->entries);
    free(db);
}
