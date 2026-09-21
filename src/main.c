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
        printf("found value: %s, at key: %s", value, "hehe");
    }
}
