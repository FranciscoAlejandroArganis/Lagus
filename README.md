# Lagus

![Lagus Icon](http://franciscoalejandroarganis.github.io/Lagus/assets/Lagus.svg)

General-purpose generic data structures in C.  
[Documentation](http://franciscoalejandroarganis.github.io/Lagus/)

## Example

Using a trie to implement a string to integer map.

```c
#include <stdio.h>
#include <string.h>
#include "Lagus.h"

lagus_declare_trie(int)
lagus_implement_trie(int)

const char *key;

uint64_t string_bit_size(int) {
    return strlen(key) << 3;
}

uint8_t string_bit_at(int, uint64_t index) {
    index--;
    return key[index >> 3] >> (index & 7) & 1;
}

int main(void) {
    int value;
    lagus_bit_stream_int bit_stream = {
        .size = string_bit_size,
        .at = string_bit_at
    };
    lagus_trie_int map;
    lagus_trie_int_custom_initialize(& map, 0, & bit_stream, NULL);

    printf("Inserting key-value pairs:\n");
    const char *keys_to_insert[] = { "Apple", "Cat", "Juice", "Night", "Whisper", "Hello" };
    int values_to_insert[] = { 54, 11, 32, 84, 500, 21 };
    for (int i = 0; i < 6; i++) {
        key = keys_to_insert[i];
        value = values_to_insert[i];
        lagus_trie_int_add_replace( & map, & value);
        printf("Inserted \"%s\" -> %u\n", key, value);
    }

    printf("\nSearching for keys:\n");
    const char *keys_to_lookup[] = { "Seal", "Ring", "Cat", "Whisper", "Draw", "Apple" };
    for (int i = 0; i < 6; i++) {
        key = keys_to_lookup[i];
        if (lagus_trie_int_search(& map, & value) == lagus_success)
            printf("Found key \"%s\" with value %d\n", key, value);
        else
            printf("Key \"%s\" not found\n", key);
    }

    printf("\nValues in the map:\n");
    lagus_trie_iterator_int iterator;
    lagus_trie_iterator_int_initialize(& iterator, & map);
    printf("[ ");
    while (lagus_trie_iterator_int_next(& iterator, & value) != lagus_stop)
        printf("%d ", value);
    printf("]\n");
    lagus_trie_iterator_int_finalize(& iterator);

    lagus_trie_int_finalize(& map);
    return 0;
}
```

Output

```
Inserting key-value pairs:
Inserted "Apple" -> 54
Inserted "Cat" -> 11
Inserted "Juice" -> 32
Inserted "Night" -> 84
Inserted "Whisper" -> 500
Inserted "Hello" -> 21

Searching for keys:
Key "Seal" not found
Key "Ring" not found
Found key "Cat" with value 11
Found key "Whisper" with value 500
Key "Draw" not found
Found key "Apple" with value 54

Values in the map:
[ 54 11 32 84 500 21 ]
```
