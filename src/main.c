#include <assert.h>
#include <kv.h>
#include <stdio.h>

int main() {
    kv_t *db = kv_init(16);

    printf("Pointer address: %p\n", db);
    printf("Database capacity: %ld\n", db->capacity);
    printf("Database count : %ld\n", db->count);

    kv_put(db, "hehe", "haha");

    for (int i = 0; i < db->capacity; i++) {
        if (db->entries[i].key) {
            printf("[%d] key: %s, value: %s\n", i, db->entries[i].key,
                   db->entries[i].value);
        }
    }

    char *value = kv_get(db, "hehe");
    if (value) {
        printf("found value: %s, at key: %s\n", value, "hehe");
    }

    printf("Count in database is %ld.\n", db->count);

    if (kv_delete(db, "foo") == -1) {
        printf("Could not find key 'foo' in database.\n");
    }

    if (kv_delete(db, "hehe") == 0) {
        printf("Successfully deleted key 'hehe' from db.\n");
        printf("Database count after deletion: %ld\n", db->count);
    } else {
        printf("Could not delete 'hehe' from db.\n");
    }

    kv_free(db);
}
