#include <assert.h>
#include <kv.h>
#include <stdio.h>

int main() {
    kv_t *db = kv_init(16);

    printf("Pointer address: %p\n", db);
    printf("Database capacity: %ld\n", db->capacity);
    printf("Database count : %ld\n", db->count);

    int idx = kv_put(db, "hehe", "haha");

    if (idx >= 0) {
        printf("Put key 'hehe' and value 'haha' at idx: %d", idx);
    } else {
        printf("kv_put returned: %d", idx);
    }
}
