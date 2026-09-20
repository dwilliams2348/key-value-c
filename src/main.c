#include <assert.h>
#include <kv.h>
#include <stdio.h>

int main() {
    kv_t *db = kv_init(16);

    printf("Pointer address: %p\n", db);
    printf("Database capacity: %ld\n", db->capacity);
    printf("Database count : %ld\n", db->count);

    assert(db != NULL);
    assert(db->capacity == 16);
    assert(db->count == 0);
    // kv_free(db);
}
