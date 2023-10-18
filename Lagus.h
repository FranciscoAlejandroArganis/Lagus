# pragma once

# include <stdlib.h>
# include <stdint.h>
# include <stdbool.h>

typedef struct lagus_memory_management {
    void * (* allocate) (uint64_t, uint64_t);
    void * (* reallocate) (void *, uint64_t);
    void (* deallocate) (void *);
} lagus_memory_management;

typedef enum lagus_result {
    lagus_success,
    lagus_memory_allocation,
    lagus_not_contained,
    lagus_already_contained,
    lagus_stop
} lagus_result;

# define lagus_none 0
# define lagus_root 1
# define lagus_left 0
# define lagus_right 1
# define lagus_one ((__uint128_t) 1)

/* List */

// Declaration

# define lagus_declare_list(E) \
\
typedef struct lagus_list_##E {\
    uint64_t start;\
    uint64_t end;\
    uint64_t size;\
    uint64_t capacity;\
    E * buffer;\
    lagus_memory_management memory_management;\
} lagus_list_##E;\
\
typedef struct lagus_list_iterator_##E {\
    E * lower;\
    E * upper;\
    E * current;\
    E * end;\
} lagus_list_iterator_##E;\
\
lagus_result lagus_list_##E##_initialize (lagus_list_##E * list);\
\
lagus_result lagus_list_##E##_custom_initialize (lagus_list_##E * list, uint64_t capacity, lagus_memory_management * memory_management);\
\
lagus_result lagus_list_##E##_finalize (lagus_list_##E * list);\
\
lagus_result lagus_list_##E##_size (lagus_list_##E * list, uint64_t * size);\
\
lagus_result lagus_list_##E##_add_first (lagus_list_##E * list, E element);\
\
lagus_result lagus_list_##E##_add_last (lagus_list_##E * list, E element);\
\
lagus_result lagus_list_##E##_add_at (lagus_list_##E * list, uint64_t index, E element);\
\
lagus_result lagus_list_##E##_remove_first (lagus_list_##E * list, E * element);\
\
lagus_result lagus_list_##E##_remove_last (lagus_list_##E * list, E * element);\
\
lagus_result lagus_list_##E##_remove_at (lagus_list_##E * list, uint64_t index, E * element);\
\
lagus_result lagus_list_##E##_get_first (lagus_list_##E * list, E * element);\
\
lagus_result lagus_list_##E##_get_last (lagus_list_##E * list, E * element);\
\
lagus_result lagus_list_##E##_get_at (lagus_list_##E * list, uint64_t index, E * element);\
\
lagus_result lagus_list_##E##_set_first (lagus_list_##E * list, E * element);\
\
lagus_result lagus_list_##E##_set_last (lagus_list_##E * list, E * element);\
\
lagus_result lagus_list_##E##_set_at (lagus_list_##E * list, uint64_t index, E * element);\
\
lagus_result lagus_list_iterator_##E##_initialize (lagus_list_iterator_##E * iterator, lagus_list_##E * list);\
\
lagus_result lagus_list_iterator_##E##_next (lagus_list_iterator_##E * iterator, E * element);\
\
lagus_result lagus_list_iterator_##E##_finalize (lagus_list_iterator_##E * iterator);

// Implementation

# define lagus_implement_list(E) \
\
lagus_result lagus_list_##E##_initialize (lagus_list_##E * list) {\
    E * buffer = aligned_alloc (_Alignof (E), sizeof (E));\
    if (! buffer)\
        return lagus_memory_allocation;\
    * list = (lagus_list_##E) {\
        .start = 1,\
        .end = 1,\
        .size = 0,\
        .capacity = 1,\
        .buffer = buffer - 1,\
        .memory_management = {\
            .allocate = aligned_alloc,\
            .reallocate = realloc,\
            .deallocate = free\
        }\
    };\
    return lagus_success;\
}\
\
lagus_result lagus_list_##E##_custom_initialize (lagus_list_##E * list, uint64_t capacity, lagus_memory_management * memory_management) {\
    E * buffer;\
    if (! capacity)\
        capacity = 1;\
    if (memory_management) {\
        buffer = memory_management -> allocate (_Alignof (E), capacity * sizeof (E));\
        if (! buffer)\
            return lagus_memory_allocation;\
        list -> memory_management = * memory_management;\
    } else {\
        buffer = aligned_alloc (_Alignof (E), capacity * sizeof (E));\
        if (! buffer)\
            return lagus_memory_allocation;\
        list -> memory_management = (lagus_memory_management) {\
            .allocate = aligned_alloc,\
            .reallocate = realloc,\
            .deallocate = free\
        };\
    }\
    list -> start = 1;\
    list -> end = 1;\
    list -> size = 0;\
    list -> capacity = capacity;\
    list -> buffer = buffer - 1;\
    return lagus_success;\
}\
\
lagus_result lagus_list_##E##_finalize (lagus_list_##E * list) {\
    list -> memory_management.deallocate (list -> buffer + 1);\
    return lagus_success;\
}\
\
lagus_result lagus_list_##E##_size (lagus_list_##E * list, uint64_t * size) {\
    * size = list -> size;\
    return lagus_success;\
}\
\
lagus_result lagus_list_##E##_add_first (lagus_list_##E * list, E element) {\
    uint64_t capacity, a, b, c;\
    E * buffer;\
    if (list -> size > 0) {\
        if (list -> size == list -> capacity) {\
            capacity = list -> capacity << 1;\
            buffer = list -> memory_management.reallocate (list -> buffer + 1, capacity * sizeof (E));\
            if (! buffer)\
                return lagus_memory_allocation;\
            list -> capacity = capacity;\
            list -> buffer = buffer - 1;\
            if (list -> start == 1)\
                list -> start = capacity;\
            else {\
                if (list -> end > list -> size - list -> start + 1) {\
                    a = list -> start;\
                    b = list -> size;\
                    c = capacity;\
                    while (a <= b) {\
                        list -> buffer [c] = list -> buffer [b];\
                        b --;\
                        c --;\
                    }\
                    list -> start = c;\
                } else {\
                    a = 1;\
                    b = list -> end;\
                    c = list -> size + 1;\
                    while (a <= b) {\
                        list -> buffer [c] = list -> buffer [a];\
                        a ++;\
                        c ++;\
                    }\
                    list -> start --;\
                    list -> end = c - 1;\
                }\
            }\
        } else {\
            if (list -> start == 1)\
                list -> start = list -> capacity;\
            else\
                list -> start --;\
        }\
    }\
    list -> size ++;\
    list -> buffer [list -> start] = element; \
    return lagus_success;\
}\
\
lagus_result lagus_list_##E##_add_last (lagus_list_##E * list, E element) {\
    uint64_t capacity, a, b, c;\
    E * buffer;\
    if (list -> size > 0) {\
        if (list -> size == list -> capacity) {\
            capacity = list -> capacity << 1;\
            buffer = list -> memory_management.reallocate (list -> buffer + 1, capacity * sizeof (E));\
            if (! buffer)\
                return lagus_memory_allocation;\
            list -> capacity = capacity;\
            list -> buffer = buffer - 1;\
            if (list -> end == list -> size)\
                list -> end ++;\
            else {\
                if (list -> end > list -> size - list -> start + 1) {\
                    a = list -> start;\
                    b = list -> size;\
                    c = capacity;\
                    while (a <= b) {\
                        list -> buffer [c] = list -> buffer [b];\
                        b --;\
                        c --;\
                    }\
                    list -> start = c + 1;\
                    list -> end ++;\
                } else {\
                    a = 1;\
                    b = list -> end;\
                    c = list -> size + 1;\
                    while (a <= b) {\
                        list -> buffer [c] = list -> buffer [a];\
                        a ++;\
                        c ++;\
                    }\
                    list -> end = c;\
                }\
            }\
        } else {\
            if (list -> end == list -> capacity)\
                list -> end = 1;\
            else\
                list -> end ++;\
        }\
    }\
    list -> size ++;\
    list -> buffer [list -> end] = element;\
    return lagus_success;\
}\
\
lagus_result lagus_list_##E##_add_at (lagus_list_##E * list, uint64_t index, E element) {\
    uint64_t capacity, a, b, c;\
    E * buffer;\
    if (list -> size == list -> capacity) {\
        capacity = list -> capacity << 1;\
        buffer = list -> memory_management.reallocate (list -> buffer + 1, capacity * sizeof (E));\
        if (! buffer)\
            return lagus_memory_allocation;\
        list -> capacity = capacity;\
        list -> buffer = buffer - 1;\
        if (index == 1) {\
            if (list -> start == 1)\
                list -> start = capacity;\
            else {\
                if (list -> end > list -> size - list -> start + 1) {\
                    a = list -> start;\
                    b = list -> size;\
                    c = capacity;\
                    while (a <= b) {\
                        list -> buffer [c] = list -> buffer [b];\
                        b --;\
                        c --;\
                    }\
                    list -> start = c;\
                } else {\
                    a = 1;\
                    b = list -> end;\
                    c = list -> size + 1;\
                    while (a <= b) {\
                        list -> buffer [c] = list -> buffer [a];\
                        a ++;\
                        c ++;\
                    }\
                    list -> start --;\
                    list -> end = c - 1;\
                }\
            }\
            list -> buffer [list -> start] = element;\
        } else if (index == list -> size + 1) {\
            if (list -> end == list -> size)\
                list -> end ++;\
            else {\
                if (list -> end > list -> size - list -> start + 1) {\
                    a = list -> start;\
                    b = list -> size;\
                    c = capacity;\
                    while (a <= b) {\
                        list -> buffer [c] = list -> buffer [b];\
                        b --;\
                        c --;\
                    }\
                    list -> start = c + 1;\
                    list -> end ++;\
                } else {\
                    a = 1;\
                    b = list -> end;\
                    c = list -> size + 1;\
                    while (a <= b) {\
                        list -> buffer [c] = list -> buffer [a];\
                        a ++;\
                        c ++;\
                    }\
                    list -> end = c;\
                }\
            }\
            list -> buffer [list -> end] = element;\
        } else {\
            index += list -> start - 1;\
            if (index > list -> size) {\
                index -= list -> size;\
                a = index - 1;\
                b = list -> end - index + 1;\
                c = list -> size - list -> start + 1;\
                if (a >= b) {\
                    if (c >= a)\
                        goto left_region_3;\
                    else {\
                        a = list -> start;\
                        b = list -> size;\
                        c = capacity;\
                        while (a <= b) {\
                            list -> buffer [c] = list -> buffer [b];\
                            b --;\
                            c --;\
                        }\
                        a = index;\
                        b = list -> end;\
                        c = list -> end + 1;\
                        while (a <= b) {\
                            list -> buffer [c] = list -> buffer [b];\
                            b --;\
                            c --;\
                        }\
                        list -> start += list -> size;\
                        list -> end ++;\
                        list -> buffer [index] = element;\
                    }\
                } else {\
                    if (c >= b) {\
                        left_region_3:\
                        a = 1;\
                        b = index - 1;\
                        c = list -> size + 1;\
                        while (a <= b) {\
                            list -> buffer [c] = list -> buffer [a];\
                            a ++;\
                            c ++;\
                        }\
                        list -> buffer [c] = element;\
                        a = index;\
                        b = list -> end;\
                        c ++;\
                        while (a <= b) {\
                            list -> buffer [c] = list -> buffer [a];\
                            a ++;\
                            c ++;\
                        }\
                        list -> end = c - 1;\
                    } else {\
                        a = list -> start;\
                        b = list -> size;\
                        c = capacity - 1;\
                        while (a <= b) {\
                            list -> buffer [c] = list -> buffer [b];\
                            b --;\
                            c --;\
                        }\
                        list -> start += list -> size - 1;\
                        if (index == 1)\
                            list -> buffer [capacity] = element;\
                        else {\
                            list -> buffer [capacity] = list -> buffer [1];\
                            a = 1;\
                            b = 2;\
                            c = index - 1;\
                            while (b <= c) {\
                                list -> buffer [a] = list -> buffer [b];\
                                a ++;\
                                b ++;\
                            }\
                            list -> buffer [index - 1] = element;\
                        }\
                    }\
                }\
            } else if (list -> start > list -> end) {\
                a = list -> end;\
                b = index - list -> start;\
                c = list -> size - index + 1;\
                if (a > b) {\
                    if (a > c) {\
                        a = index;\
                        b = list -> size;\
                        c = capacity;\
                        while (a <= b) {\
                            list -> buffer [c] = list -> buffer [b];\
                            b --;\
                            c --;\
                        }\
                        list -> buffer [c] = element;\
                        a = list -> start;\
                        b = index - 1;\
                        c --;\
                        while (a <= b) {\
                            list -> buffer [c] = list -> buffer [b];\
                            b --;\
                            c --;\
                        }\
                        list -> start = c + 1;\
                    } else\
                        goto right_region_3;\
                } else {\
                    if (c > b) {\
                        right_region_3:\
                        a = 1;\
                        b = list -> end;\
                        c = list -> size + 1;\
                        while (a <= b) {\
                            list -> buffer [c] = list -> buffer [a];\
                            a ++;\
                            c ++;\
                        }\
                        list -> end = c - 1;\
                        a = list -> start - 1;\
                        b = list -> start;\
                        c = index - 1;\
                        while (b <= c) {\
                            list -> buffer [a] = list -> buffer [b];\
                            a ++;\
                            b ++;\
                        }\
                        list -> start --;\
                        list -> buffer [index - 1] = element;\
                    } else {\
                        a = index;\
                        b = list -> size;\
                        c = list -> size + 1;\
                        while (a <= b) {\
                            list -> buffer [c] = list -> buffer [b];\
                            b --;\
                            c --;\
                        }\
                        a = 1;\
                        b = list -> end;\
                        c = list -> size + 2;\
                        while (a <= b) {\
                            list -> buffer [c] = list -> buffer [a];\
                            a ++;\
                            c ++;\
                        }\
                        list -> end = c - 1;\
                        list -> buffer [index] = element;\
                    }\
                }\
            } else {\
                if (index - list -> start >= list -> end - index + 1) {\
                    a = index;\
                    b = list -> end;\
                    c = list -> end + 1;\
                    while (a <= b) {\
                        list -> buffer [c] = list -> buffer [b];\
                        b --;\
                        c --;\
                    }\
                    list -> end ++;\
                    list -> buffer [index] = element;\
                } else {\
                    list -> buffer [capacity] = list -> buffer [1];\
                    a = 1;\
                    b = 2;\
                    c = index - 1;\
                    while (b <= c) {\
                        list -> buffer [a] = list -> buffer [b];\
                        a ++;\
                        b ++;\
                    }\
                    list -> start = capacity;\
                    list -> buffer [index - 1] = element;\
                }\
            }\
        }\
    } else {\
        if (index == 1) {\
            if (list -> start == 1)\
                list -> start = list -> capacity;\
            else\
                list -> start --;\
            list -> buffer [list -> start] = element;\
        } else if (index == list -> size + 1) {\
            if (list -> end == list -> capacity)\
                list -> end = 1;\
            else\
                list -> end ++;\
            list -> buffer [list -> end] = element;\
        } else {\
            index += list -> start - 1;\
            if (index > list -> capacity) {\
                a = index - list -> start;\
                index -= list -> capacity;\
                if (a >= list -> end - index + 1) {\
                    a = index;\
                    b = list -> end;\
                    c = list -> end + 1;\
                    while (a <= b) {\
                        list -> buffer [c] = list -> buffer [b];\
                        b --;\
                        c --;\
                    }\
                    list -> end ++;\
                    list -> buffer [index] = element;\
                } else {\
                    a = list -> start - 1;\
                    b = list -> start;\
                    c = list -> capacity;\
                    while (b <= c) {\
                        list -> buffer [a] = list -> buffer [b];\
                        a ++;\
                        b ++;\
                    }\
                    if (index == 1)\
                        list -> buffer [list -> capacity] = element;\
                    else {\
                        list -> buffer [list -> capacity] = list -> buffer [1];\
                        a = 1;\
                        b = 2;\
                        c = index - 1;\
                        while (b <= c) {\
                            list -> buffer [a] = list -> buffer [b];\
                            a ++;\
                            b ++;\
                        }\
                        list -> buffer [index - 1] = element;\
                    }\
                    list -> start --;\
                }\
            } else if (list -> start > list -> end) {\
                if (list -> capacity - index + 1 + list -> end >= index - list -> start) {\
                    a = list -> start - 1;\
                    b = list -> start;\
                    c = index - 1;\
                    while (b <= c) {\
                        list -> buffer [a] = list -> buffer [b];\
                        a ++;\
                        b ++;\
                    }\
                    list -> start --;\
                    list -> buffer [index - 1] = element;\
                } else {\
                    a = 1;\
                    b = list -> end;\
                    c = list -> end + 1;\
                    while (a <= b) {\
                        list -> buffer [c] = list -> buffer [b];\
                        b --;\
                        c --;\
                    }\
                    list -> buffer [1] = list -> buffer [list -> capacity];\
                    a = index;\
                    b = list -> capacity - 1;\
                    c = list -> capacity;\
                    while (a <= b) {\
                        list -> buffer [c] = list -> buffer [b];\
                        b --;\
                        c --;\
                    }\
                    list -> end ++;\
                    list -> buffer [index] = element;\
                }\
            } else {\
                if (index - list -> start > list -> end - index + 1) {\
                    if (list -> end == list -> capacity) {\
                        list -> buffer [1] = list -> buffer [list -> capacity];\
                        a = index;\
                        b = list -> capacity - 1;\
                        c = list -> capacity;\
                        while (a <= b) {\
                            list -> buffer [c] = list -> buffer [b];\
                            b --;\
                            c --;\
                        }\
                        list -> end = 1;\
                    } else {\
                        a = index;\
                        b = list -> end;\
                        c = list -> end + 1;\
                        while (a <= b) {\
                            list -> buffer [c] = list -> buffer [b];\
                            b --;\
                            c --;\
                        }\
                        list -> end ++;\
                    }\
                    list -> buffer [index] = element;\
                } else {\
                    if (list -> start == 1) {\
                        list -> buffer [list -> capacity] = list -> buffer [1];\
                        a = 1;\
                        b = 2;\
                        c = index - 1;\
                        while (b <= c) {\
                            list -> buffer [a] = list -> buffer [b];\
                            a ++;\
                            b ++;\
                        }\
                        list -> start = list -> capacity;\
                    } else {\
                        a = list -> start - 1;\
                        b = list -> start;\
                        c = index - 1;\
                        while (b <= c) {\
                            list -> buffer [a] = list -> buffer [b];\
                            a ++;\
                            b ++;\
                        }\
                        list -> start --;\
                    }\
                    list -> buffer [index - 1] = element;\
                }\
            }\
        }\
    }\
    list -> size ++;\
    return lagus_success;\
}\
\
lagus_result lagus_list_##E##_remove_first (lagus_list_##E * list, E * element) {\
    uint64_t capacity, a, b, c;\
    E * buffer;\
    * element = list -> buffer [list -> start];\
    list -> size --;\
    if (list -> size > 0) {\
        if (list -> capacity < 4)\
            capacity = 1;\
        else\
            capacity = list -> capacity >> 2;\
        if (list -> size <= capacity) {\
            if (list -> start <= list -> end) {\
                a = 1;\
                b = list -> start + 1;\
                c = list -> end;\
                while (b <= c) {\
                    list -> buffer [a] = list -> buffer [b];\
                    a ++;\
                    b ++;\
                }\
                list -> start = 1;\
                list -> end = capacity;\
                buffer = list -> memory_management.reallocate (list -> buffer + 1, capacity * sizeof (E));\
                if (! buffer)\
                    return lagus_memory_allocation;\
            } else {\
                if (list -> start == list -> capacity) {\
                    list -> start = 1;\
                    buffer = list -> memory_management.reallocate (list -> buffer + 1, capacity * sizeof (E));\
                    if (! buffer)\
                        return lagus_memory_allocation;\
                } else {\
                    a = capacity;\
                    b = list -> start + 1;\
                    c = list -> capacity;\
                    while (b <= c) {\
                        list -> buffer [a] = list -> buffer [c];\
                        a --;\
                        c --;\
                    }\
                    buffer = list -> memory_management.reallocate (list -> buffer + 1, capacity * sizeof (E));\
                    if (! buffer) {\
                        list -> start ++;\
                        return lagus_memory_allocation;\
                    }\
                    list -> start = a + 1;\
                }\
            }\
            list -> capacity = capacity;\
            list -> buffer = buffer - 1;\
        } else {\
            if (list -> start == list -> capacity)\
                list -> start = 1;\
            else\
                list -> start ++;\
        }\
    }\
    return lagus_success;\
}\
\
lagus_result lagus_list_##E##_remove_last (lagus_list_##E * list, E * element) {\
    uint64_t capacity, a, b, c;\
    E * buffer;\
    * element = list -> buffer [list -> end];\
    list -> size --;\
    if (list -> size > 0) {\
        if (list -> capacity < 4)\
            capacity = 1;\
        else\
            capacity = list -> capacity >> 2;\
        if (list -> size <= capacity) {\
            if (list -> start <= list -> end) {\
                if (list -> start > 1) {\
                    a = 1;\
                    b = list -> start;\
                    c = list -> end - 1;\
                    while (b <= c) {\
                        list -> buffer [a] = list -> buffer [b];\
                        a ++;\
                        b ++;\
                    }\
                    list -> start = 1;\
                }\
                list -> end = list -> size;\
                buffer = list -> memory_management.reallocate (list -> buffer + 1, capacity * sizeof (E));\
                if (! buffer)\
                    return lagus_memory_allocation;\
            } else {\
                if (list -> end == 1) {\
                    a = 1;\
                    b = list -> start;\
                    c = list -> capacity;\
                    while (b <= c) {\
                        list -> buffer [a] = list -> buffer [b];\
                        a ++;\
                        b ++;\
                    }\
                    list -> start = 1;\
                    list -> end = list -> size;\
                    buffer = list -> memory_management.reallocate (list -> buffer + 1, capacity * sizeof (E));\
                    if (! buffer)\
                        return lagus_memory_allocation;\
                } else {\
                    a = capacity;\
                    b = list -> start;\
                    c = list -> capacity;\
                    while (b <= c) {\
                        list -> buffer [a] = list -> buffer [c];\
                        a --;\
                        c --;\
                    }\
                    list -> end --;\
                    buffer = list -> memory_management.reallocate (list -> buffer + 1, capacity * sizeof (E));\
                    if (! buffer)\
                        return lagus_memory_allocation;\
                    list -> start = a + 1;\
                }\
            }\
            list -> capacity = capacity;\
            list -> buffer = buffer - 1;\
        } else {\
            if (list -> end == 1)\
                list -> end = list -> capacity;\
            else\
                list -> end --;\
        }\
    }\
    return lagus_success;\
}\
\
lagus_result lagus_list_##E##_remove_at (lagus_list_##E * list, uint64_t index, E * element) {\
    uint64_t capacity, a, b, c;\
    E * buffer;\
    list -> size --;\
    if (list -> size > 0) {\
        if (list -> capacity < 4)\
            capacity = 1;\
        else\
            capacity = list -> capacity >> 2;\
        if (list -> size <= capacity) {\
            if (index == 1) {\
                if (list -> start <= list -> end) {\
                    a = 1;\
                    b = list -> start + 1;\
                    c = list -> end;\
                    while (b <= c) {\
                        list -> buffer [a] = list -> buffer [b];\
                        a ++;\
                        b ++;\
                    }\
                    list -> start = 1;\
                    list -> end = capacity;\
                    buffer = list -> memory_management.reallocate (list -> buffer + 1, capacity * sizeof (E));\
                    if (! buffer)\
                        return lagus_memory_allocation;\
                } else {\
                    if (list -> start == list -> capacity) {\
                        list -> start = 1;\
                        buffer = list -> memory_management.reallocate (list -> buffer + 1, capacity * sizeof (E));\
                        if (! buffer)\
                            return lagus_memory_allocation;\
                    } else {\
                        a = capacity;\
                        b = list -> start + 1;\
                        c = list -> capacity;\
                        while (b <= c) {\
                            list -> buffer [a] = list -> buffer [c];\
                            a --;\
                            c --;\
                        }\
                        buffer = list -> memory_management.reallocate (list -> buffer + 1, capacity * sizeof (E));\
                        if (! buffer) {\
                            list -> start ++;\
                            return lagus_memory_allocation;\
                        }\
                        list -> start = a + 1;\
                    }\
                }\
            } else if (index > list -> size) {\
                if (list -> start <= list -> end) {\
                    if (list -> start > 1) {\
                        a = 1;\
                        b = list -> start;\
                        c = list -> end - 1;\
                        while (b <= c) {\
                            list -> buffer [a] = list -> buffer [b];\
                            a ++;\
                            b ++;\
                        }\
                        list -> start = 1;\
                    }\
                    list -> end = list -> size;\
                    buffer = list -> memory_management.reallocate (list -> buffer + 1, capacity * sizeof (E));\
                    if (! buffer)\
                        return lagus_memory_allocation;\
                } else {\
                    if (list -> end == 1) {\
                        a = 1;\
                        b = list -> start;\
                        c = list -> capacity;\
                        while (b <= c) {\
                            list -> buffer [a] = list -> buffer [b];\
                            a ++;\
                            b ++;\
                        }\
                        list -> start = 1;\
                        list -> end = list -> size;\
                        buffer = list -> memory_management.reallocate (list -> buffer + 1, capacity * sizeof (E));\
                        if (! buffer)\
                            return lagus_memory_allocation;\
                    } else {\
                        a = capacity;\
                        b = list -> start;\
                        c = list -> capacity;\
                        while (b <= c) {\
                            list -> buffer [a] = list -> buffer [c];\
                            a --;\
                            c --;\
                        }\
                        list -> end --;\
                        buffer = list -> memory_management.reallocate (list -> buffer + 1, capacity * sizeof (E));\
                        if (! buffer)\
                            return lagus_memory_allocation;\
                        list -> start = a + 1;\
                    }\
                }\
            } else {\
                index += list -> start - 1;\
                if (index > list -> capacity) {\
                    index -= list -> capacity;\
                    * element = list -> buffer [index];\
                    if (index - 1 > list -> end - index) {\
                        a = index;\
                        b = index + 1;\
                        c = list -> end;\
                        while (b <= c) {\
                            list -> buffer [a] = list -> buffer [b];\
                            a ++;\
                            b ++;\
                        }\
                        a = capacity;\
                        b = list -> start;\
                        c = list -> capacity;\
                        while (b <= c) {\
                            list -> buffer [a] = list -> buffer [c];\
                            a --;\
                            c --;\
                        }\
                        list -> end --;\
                        buffer = list -> memory_management.reallocate (list -> buffer + 1, capacity * sizeof (E));\
                        if (! buffer)\
                            return lagus_memory_allocation;\
                        list -> start = a + 1;\
                    } else {\
                        a = 1;\
                        b = index - 1;\
                        c = index;\
                        while (a <= b) {\
                            list -> buffer [c] = list -> buffer [b];\
                            b --;\
                            c --;\
                        }\
                        list -> buffer [1] = list -> buffer [list -> capacity];\
                        a = capacity;\
                        b = list -> start;\
                        c = list -> capacity - 1;\
                        while (b <= c) {\
                            list -> buffer [a] = list -> buffer [c];\
                            a --;\
                            c --;\
                        }\
                        buffer = list -> memory_management.reallocate (list -> buffer + 1, capacity * sizeof (E));\
                        if (! buffer) {\
                            a = list -> start;\
                            b = list -> capacity - 1;\
                            c = list -> capacity;\
                            while (a <= b) {\
                                list -> buffer [c] = list -> buffer [b];\
                                b --;\
                                c --;\
                            }\
                            if (list -> start == list -> capacity)\
                                list -> start = 1;\
                            else\
                                list -> start ++;\
                            return lagus_memory_allocation;\
                        }\
                        if (list -> start == list -> capacity)\
                            list -> start = 1;\
                        else\
                            list -> start = a + 1;\
                    }\
                } else {\
                    * element = list -> buffer [index];\
                    if (list -> start > list -> end) {\
                        a = capacity;\
                        b = index + 1;\
                        c = list -> capacity;\
                        while (b <= c) {\
                            list -> buffer [a] = list -> buffer [c];\
                            a --;\
                            c --;\
                        }\
                        b = list -> start;\
                        c = index - 1;\
                        while (b <= c) {\
                            list -> buffer [a] = list -> buffer [c];\
                            a --;\
                            c --;\
                        }\
                        buffer = list -> memory_management.reallocate (list -> buffer + 1, capacity * sizeof (E));\
                        if (! buffer) {\
                            a = list -> start;\
                            b = index - 1;\
                            c = index;\
                            while (a <= b) {\
                                list -> buffer [c] = list -> buffer [b];\
                                b --;\
                                c --;\
                            }\
                            list -> start ++;\
                            return lagus_memory_allocation;\
                        }\
                        list -> start = a + 1;\
                    } else {\
                        if (list -> start > 1) {\
                            a = 1;\
                            b = list -> start;\
                            c = index - 1;\
                            while (b <= c) {\
                                list -> buffer [a] = list -> buffer [b];\
                                a ++;\
                                b ++;\
                            }\
                            list -> start = 1;\
                        } else\
                            a = index;\
                        b = index + 1;\
                        c = list -> end;\
                        while (b <= c) {\
                            list -> buffer [a] = list -> buffer [b];\
                            a ++;\
                            b ++;\
                        }\
                        list -> end = list -> size;\
                        buffer = list -> memory_management.reallocate (list -> buffer + 1, capacity * sizeof (E));\
                        if (! buffer)\
                            return lagus_memory_allocation;\
                    }\
                }\
            }\
            list -> capacity = capacity;\
            list -> buffer = buffer - 1;\
        } else {\
            if (index == 1) {\
                if (list -> start == list -> capacity)\
                    list -> start = 1;\
                else\
                    list -> start ++;\
            } else if (index > list -> size) {\
                if (list -> end == 1)\
                    list -> end = list -> capacity;\
                else\
                    list -> end --;\
            } else {\
                index += list -> start - 1;\
                if (index > list -> capacity) {\
                    a = index - list -> start;\
                    index -= list -> capacity;\
                    * element = list -> buffer [index];\
                    if (a > list -> end - index) {\
                        a = index;\
                        b = index + 1;\
                        c = list -> end;\
                        while (b <= c) {\
                            list -> buffer [a] = list -> buffer [b];\
                            a ++;\
                            b ++;\
                        }\
                        list -> end --;\
                    } else {\
                        a = 1;\
                        b = index - 1;\
                        c = index;\
                        while (a <= b) {\
                            list -> buffer [c] = list -> buffer [b];\
                            b --;\
                            c --;\
                        }\
                        list -> buffer [1] = list -> buffer [list -> capacity];\
                        a = list -> start;\
                        b = list -> capacity - 1;\
                        c = list -> capacity;\
                        while (a <= b) {\
                            list -> buffer [c] = list -> buffer [b];\
                            b --;\
                            c --;\
                        }\
                        if (list -> start == list -> capacity)\
                            list -> start = 1;\
                        else\
                            list -> start ++;\
                    }\
                } else {\
                    * element = list -> buffer [index];\
                    if (list -> start > list -> end) {\
                        if (list -> capacity - index + list -> end > index - list -> start) {\
                            a = list -> start;\
                            b = index - 1;\
                            c = index;\
                            while (a <= b) {\
                                list -> buffer [c] = list -> buffer [b];\
                                b --;\
                                c --;\
                            }\
                            list -> start ++;\
                        } else {\
                            a = index;\
                            b = index + 1;\
                            c = list -> capacity;\
                            while (b <= c) {\
                                list -> buffer [a] = list -> buffer [b];\
                                a ++;\
                                b ++;\
                            }\
                            list -> buffer [list -> capacity] = list -> buffer [1];\
                            a = 1;\
                            b = 2;\
                            c = list -> end;\
                            while (b <= c) {\
                                list -> buffer [a] = list -> buffer [b];\
                                a ++;\
                                b ++;\
                            }\
                            if (list -> end == 1)\
                                list -> end = list -> capacity;\
                            else\
                                list -> end --;\
                        }\
                    } else {\
                        if (index - list -> start >= list -> end - index) {\
                            a = index;\
                            b = index + 1;\
                            c = list -> end;\
                            while (b <= c) {\
                                list -> buffer [a] = list -> buffer [b];\
                                a ++;\
                                b ++;\
                            }\
                            list -> end --;\
                        } else {\
                            a = list -> start;\
                            b = index - 1;\
                            c = index;\
                            while (a <= b) {\
                                list -> buffer [c] = list -> buffer [b];\
                                b --;\
                                c --;\
                            }\
                            list -> start ++;\
                        }\
                    }\
                }\
            }\
        }\
    } else\
        * element = list -> buffer [list -> start];\
    return lagus_success;\
}\
\
lagus_result lagus_list_##E##_get_first (lagus_list_##E * list, E * element) {\
    * element = list -> buffer [list -> start];\
    return lagus_success;\
}\
\
lagus_result lagus_list_##E##_get_last (lagus_list_##E * list, E * element) {\
    * element = list -> buffer [list -> end];\
    return lagus_success;\
}\
\
lagus_result lagus_list_##E##_get_at (lagus_list_##E * list, uint64_t index, E * element) {\
    index += list -> start - 1;\
    if (index > list -> capacity)\
        index -= list -> capacity;\
    * element = list -> buffer [index];\
    return lagus_success;\
}\
\
lagus_result lagus_list_##E##_set_first (lagus_list_##E * list, E * element) {\
    E temporal = list -> buffer [list -> start];\
    list -> buffer [list -> start] = * element;\
    * element = temporal;\
    return lagus_success;\
}\
\
lagus_result lagus_list_##E##_set_last (lagus_list_##E * list, E * element) {\
    E temporal = list -> buffer [list -> end];\
    list -> buffer [list -> end] = * element;\
    * element = temporal;\
    return lagus_success;\
}\
\
lagus_result lagus_list_##E##_set_at (lagus_list_##E * list, uint64_t index, E * element) {\
    index += list -> start - 1;\
    if (index > list -> capacity)\
        index -= list -> capacity;\
    E temporal = list -> buffer [index];\
    list -> buffer [index] = * element;\
    * element = temporal;\
    return lagus_success;\
}\
\
lagus_result lagus_list_iterator_##E##_initialize (lagus_list_iterator_##E * iterator, lagus_list_##E * list) {\
    if (list -> size) {\
        E * buffer = list -> buffer;\
        * iterator = (lagus_list_iterator_##E) {\
            .lower = buffer + 1,\
            .upper = buffer + list -> capacity,\
            .current = buffer + list -> start,\
            .end = buffer + list -> end\
        };\
    } else\
        iterator -> current = lagus_none;\
    return lagus_success;\
}\
\
lagus_result lagus_list_iterator_##E##_next (lagus_list_iterator_##E * iterator, E * element) {\
    if (! iterator -> current)\
        return lagus_stop;\
    * element = * iterator -> current;\
    if (iterator -> current == iterator -> end)\
        iterator -> current = lagus_none;\
    else if (iterator -> current == iterator -> upper)\
        iterator -> current = iterator -> lower;\
    else\
        iterator -> current ++;\
    return lagus_success;\
}\
\
lagus_result lagus_list_iterator_##E##_finalize (lagus_list_iterator_##E * iterator) {\
    return lagus_success;\
}

/* BST */

// Declaration

# define lagus_declare_bst(E) \
\
typedef int64_t (* lagus_compare_##E) (E, E);\
\
typedef struct lagus_bst_node_##E {\
    uint64_t parent;\
    uint64_t left_child;\
    uint64_t right_child;\
    uint64_t size;\
    uint8_t configuration;\
    E element;\
} lagus_bst_node_##E;\
\
typedef struct lagus_bst_##E {\
    uint64_t height;\
    uint64_t capacity;\
    lagus_bst_node_##E * buffer;\
    lagus_compare_##E compare;\
    lagus_memory_management memory_management;\
} lagus_bst_##E;\
\
typedef struct lagus_bst_iterator_##E {\
    __uint128_t index;\
    uint64_t height;\
    uint64_t node;\
    lagus_bst_node_##E * buffer;\
} lagus_bst_iterator_##E;\
\
int64_t lagus_binary_compare_##E (E a, E b);\
\
lagus_result lagus_bst_##E##_initialize (lagus_bst_##E * bst);\
\
lagus_result lagus_bst_##E##_custom_initialize (lagus_bst_##E * bst, uint64_t capacity, lagus_compare_##E compare, lagus_memory_management * memory_management);\
\
lagus_result lagus_bst_##E##_finalize (lagus_bst_##E * bst);\
\
lagus_result lagus_bst_##E##_size (lagus_bst_##E * bst, uint64_t * size);\
\
lagus_result lagus_bst_##E##_add_left (lagus_bst_##E * bst, E element);\
\
lagus_result lagus_bst_##E##_add_right (lagus_bst_##E * bst, E element);\
\
lagus_result lagus_bst_##E##_add_at (lagus_bst_##E * bst, uint64_t index, E element);\
\
lagus_result lagus_bst_##E##_search_left (lagus_bst_##E * bst, E * element, uint64_t * index);\
\
lagus_result lagus_bst_##E##_search_right (lagus_bst_##E * bst, E * element, uint64_t * index);\
\
lagus_result lagus_bst_##E##_remove_left (lagus_bst_##E * bst, E * element);\
\
lagus_result lagus_bst_##E##_remove_right (lagus_bst_##E * bst, E * element);\
\
lagus_result lagus_bst_##E##_remove_at (lagus_bst_##E * bst, uint64_t index, E * element);\
\
lagus_result lagus_bst_##E##_get_at (lagus_bst_##E * bst, uint64_t index, E * element);\
\
lagus_result lagus_bst_##E##_set_at (lagus_bst_##E * bst, uint64_t index, E * element);\
\
lagus_result lagus_bst_iterator_##E##_initialize (lagus_bst_iterator_##E * iterator, lagus_bst_##E * bst);\
\
lagus_result lagus_bst_iterator_##E##_next (lagus_bst_iterator_##E * iterator, E * element);\
\
lagus_result lagus_bst_iterator_##E##_finalize (lagus_bst_iterator_##E * iterator);

// Implementation

# define lagus_implement_bst(E) \
\
int64_t lagus_binary_compare_##E (E a, E b) {\
    uint8_t * bytes_a = (uint8_t *) & a;\
    uint8_t * bytes_b = (uint8_t *) & b;\
    uint64_t index = sizeof (E) - 1;\
    while (true) {\
        if (bytes_a [index] < bytes_b [index])\
            return -1;\
        if (bytes_a [index] > bytes_b [index])\
            return 1;\
        if (index == 0)\
            return 0;\
        index --;\
    }\
}\
\
lagus_result lagus_bst_##E##_initialize (lagus_bst_##E * bst) {\
    lagus_bst_node_##E * buffer = aligned_alloc (_Alignof (lagus_bst_node_##E), sizeof (lagus_bst_node_##E));\
    if (! buffer)\
        return lagus_memory_allocation;\
    * bst = (lagus_bst_##E) {\
        .height = 0,\
        .capacity = 1,\
        .buffer = buffer - 1,\
        .compare = lagus_binary_compare_##E,\
        .memory_management.allocate = aligned_alloc,\
        .memory_management.reallocate = realloc,\
        .memory_management.deallocate = free\
    };\
    * buffer = (lagus_bst_node_##E) {\
        .parent = lagus_none,\
        .left_child = lagus_none,\
        .right_child = lagus_none,\
        .size = 0\
    };\
    return lagus_success;\
}\
\
lagus_result lagus_bst_##E##_custom_initialize (lagus_bst_##E * bst, uint64_t capacity, lagus_compare_##E compare, lagus_memory_management * memory_management) {\
    lagus_bst_node_##E * buffer;\
    if (! capacity)\
        capacity = 1;\
    if (memory_management) {\
        buffer = memory_management -> allocate (_Alignof (lagus_bst_node_##E), capacity * sizeof (lagus_bst_node_##E));\
        if (! buffer)\
            return lagus_memory_allocation;\
        bst -> memory_management = * memory_management;\
    } else {\
        buffer = aligned_alloc (_Alignof (lagus_bst_node_##E), capacity * sizeof (lagus_bst_node_##E));\
        if (! buffer)\
            return lagus_memory_allocation;\
        bst -> memory_management = (lagus_memory_management) {\
            .allocate = aligned_alloc,\
            .reallocate = realloc,\
            .deallocate = free\
        };\
    }\
    bst -> height = 0;\
    bst -> capacity = 1;\
    bst -> buffer = buffer - 1;\
    if (compare)\
        bst -> compare = compare;\
    else\
        bst -> compare = lagus_binary_compare_##E;\
    * buffer = (lagus_bst_node_##E) {\
        .parent = lagus_none,\
        .left_child = lagus_none,\
        .right_child = lagus_none,\
        .size = 0\
    };\
}\
\
lagus_result lagus_bst_##E##_finalize (lagus_bst_##E * bst) {\
    bst -> memory_management.deallocate (bst -> buffer + 1);\
    return lagus_success;\
}\
\
lagus_result lagus_bst_##E##_size (lagus_bst_##E * bst, uint64_t * size) {\
    * size = bst -> buffer [lagus_root].size;\
    return lagus_success;\
}\
\
lagus_result lagus_bst_##E##_add_left (lagus_bst_##E * bst, E element) {\
    if (bst -> height == 0) {\
        bst -> buffer [lagus_root].size = 1;\
        bst -> buffer [lagus_root].configuration = 0b00;\
        bst -> buffer [lagus_root].element = element;\
        bst -> height = 1;\
    } else {\
        uint64_t size = bst -> buffer [lagus_root].size;\
        if (size == bst -> capacity) {\
            uint64_t capacity = bst -> capacity << 1;\
            lagus_bst_node_##E * buffer = bst -> memory_management.reallocate (bst -> buffer + 1, capacity * sizeof (lagus_bst_node_##E));\
            if (! buffer)\
                return lagus_memory_allocation;\
            bst -> capacity = capacity;\
            bst -> buffer = buffer - 1;\
        }\
        \
        size ++;\
        bst -> buffer [size] = (lagus_bst_node_##E) {\
            .left_child = lagus_none,\
            .right_child = lagus_none,\
            .size = 1,\
            .configuration = 0b00,\
            .element = element\
        };\
        uint8_t direction;\
        uint64_t node = lagus_root;\
        while (true) {\
            uint64_t child;\
            int64_t comparison = bst -> compare (element, bst -> buffer [node].element);\
            if (comparison <= 0) {\
                child = bst -> buffer [node].left_child;\
                if (! child) {\
                    bst -> buffer [node].left_child = size;\
                    direction = lagus_left;\
                    break;\
                }\
            }\
            else {\
                child = bst -> buffer [node].right_child;\
                if (! child) {\
                    bst -> buffer [node].right_child = size;\
                    direction = lagus_right;\
                    break;\
                }\
            }\
            node = child;\
        }\
        bst -> buffer [size].parent = node;\
        uint64_t child;\
        while (true) {\
            bst -> buffer [node].size ++;\
            if (bst -> buffer [node].configuration == 0b00) {\
                if (direction == lagus_left)\
                    bst -> buffer [node].configuration = 0b10;\
                else\
                    bst -> buffer [node].configuration = 0b01;\
                if (node == lagus_root) {\
                    bst -> height ++;\
                    return lagus_success;\
                }\
                child = node;\
                node = bst -> buffer [node].parent;\
                if (bst -> buffer [node].left_child == child)\
                    direction = lagus_left;\
                else\
                    direction = lagus_right;\
                continue;\
            }\
            if (bst -> buffer [node].configuration == 0b10) {\
                if (direction == lagus_left) {\
                    child = bst -> buffer [node].left_child;\
                    if (bst -> buffer [child].configuration == 0b10) {\
                        uint64_t b = child;\
                        uint64_t a = bst -> buffer [b].left_child;\
                        bst -> buffer [b].size -= bst -> buffer [a].size;\
                        uint64_t X = bst -> buffer [b].right_child;\
                        bst -> buffer [b].left_child = X;\
                        uint64_t W = bst -> buffer [node].right_child;\
                        bst -> buffer [b].right_child = W;\
                        if (W) {\
                            bst -> buffer [W].parent = b;\
                            bst -> buffer [b].size += bst -> buffer [W].size;\
                        }\
                        bst -> buffer [node].left_child = a;\
                        bst -> buffer [a].parent = node;\
                        bst -> buffer [node].right_child = b;\
                        bst -> buffer [b].parent = node;\
                        E temporal = bst -> buffer [node].element;\
                        bst -> buffer [node].element = bst -> buffer [b].element;\
                        bst -> buffer [b].element = temporal;\
                        bst -> buffer [child].configuration = 0b00;\
                    } else {\
                        uint64_t a = child;\
                        uint64_t b = bst -> buffer [a].right_child;\
                        bst -> buffer [a].size -= bst -> buffer [b].size;\
                        uint64_t Y = bst -> buffer [b].left_child;\
                        bst -> buffer [a].right_child = Y;\
                        if (Y) {\
                            bst -> buffer [Y].parent = a;\
                            bst -> buffer [a].size += bst -> buffer [Y].size;\
                            bst -> buffer [b].size -= bst -> buffer [Y].size;\
                        }\
                        uint64_t X = bst -> buffer [b].right_child;\
                        bst -> buffer [b].left_child = X;\
                        uint64_t W = bst -> buffer [node].right_child;\
                        bst -> buffer [b].right_child = W;\
                        if (W) {\
                            bst -> buffer [W].parent = b;\
                            bst -> buffer [b].size += bst -> buffer [W].size;\
                        }\
                        bst -> buffer [node].right_child = b;\
                        bst -> buffer [b].parent = node;\
                        E temporal = bst -> buffer [node].element;\
                        bst -> buffer [node].element = bst -> buffer [b].element;\
                        bst -> buffer [b].element = temporal;\
                        if (bst -> buffer [b].configuration == 0b00)\
                            bst -> buffer [a].configuration = 0b00;\
                        else if (bst -> buffer [b].configuration == 0b10) {\
                            bst -> buffer [a].configuration = 0b00;\
                            bst -> buffer [b].configuration = 0b01;\
                        } else {\
                            bst -> buffer [a].configuration = 0b10;\
                            bst -> buffer [b].configuration = 0b00;\
                        }\
                    }\
                }\
            } else {\
                if (direction == lagus_right) {\
                    child = bst -> buffer [node].right_child;\
                    if (bst -> buffer [child].configuration == 0b01) {\
                        uint64_t b = child;\
                        uint64_t c = bst -> buffer [b].right_child;\
                        bst -> buffer [b].size -= bst -> buffer [c].size;\
                        uint64_t Y = bst -> buffer [b].left_child;\
                        bst -> buffer [b].right_child = Y;\
                        uint64_t Z = bst -> buffer [node].left_child;\
                        bst -> buffer [b].left_child = Z;\
                        if (Z) {\
                            bst -> buffer [Z].parent = b;\
                            bst -> buffer [b].size += bst -> buffer [Z].size;\
                        }\
                        bst -> buffer [node].right_child = c;\
                        bst -> buffer [c].parent = node;\
                        bst -> buffer [node].left_child = b;\
                        bst -> buffer [b].parent = node;\
                        E temporal = bst -> buffer [node].element;\
                        bst -> buffer [node].element = bst -> buffer [b].element;\
                        bst -> buffer [b].element = temporal;\
                        bst -> buffer [child].configuration = 0b00;\
                    } else {\
                        uint64_t c = child;\
                        uint64_t b = bst -> buffer [c].left_child;\
                        bst -> buffer [c].size -= bst -> buffer [b].size;\
                        uint64_t X = bst -> buffer [b].right_child;\
                        bst -> buffer [c].left_child = X;\
                        if (X) {\
                            bst -> buffer [X].parent = c;\
                            bst -> buffer [c].size += bst -> buffer [X].size;\
                            bst -> buffer [b].size -= bst -> buffer [X].size;\
                        }\
                        uint64_t Y = bst -> buffer [b].left_child;\
                        bst -> buffer [b].right_child = Y;\
                        uint64_t Z = bst -> buffer [node].left_child;\
                        bst -> buffer [b].left_child = Z;\
                        if (Z) {\
                            bst -> buffer [Z].parent = b;\
                            bst -> buffer [b].size += bst -> buffer [Z].size;\
                        }\
                        bst -> buffer [node].left_child = b;\
                        bst -> buffer [b].parent = node;\
                        E temporal = bst -> buffer [node].element;\
                        bst -> buffer [node].element = bst -> buffer [b].element;\
                        bst -> buffer [b].element = temporal;\
                        if (bst -> buffer [b].configuration == 0b00) {\
                            bst -> buffer [c].configuration = 0b00;\
                        } else if (bst -> buffer [b].configuration == 0b01) {\
                            bst -> buffer [c].configuration = 0b00;\
                            bst -> buffer [b].configuration = 0b10;\
                        } else {\
                            bst -> buffer [c].configuration = 0b01;\
                            bst -> buffer [b].configuration = 0b00;\
                        }\
                    }\
                }\
            }\
            bst -> buffer [node].configuration = 0b00;\
            break;\
        }\
        while (node != lagus_root) {\
            node = bst -> buffer [node].parent;\
            bst -> buffer [node].size ++;\
        }\
    }\
    return lagus_success;\
}\
\
lagus_result lagus_bst_##E##_add_right (lagus_bst_##E * bst, E element) {\
    if (bst -> height == 0) {\
        bst -> buffer [lagus_root].size = 1;\
        bst -> buffer [lagus_root].configuration = 0b00;\
        bst -> buffer [lagus_root].element = element;\
        bst -> height = 1;\
    } else {\
        uint64_t size = bst -> buffer [lagus_root].size;\
        if (size == bst -> capacity) {\
            uint64_t capacity = bst -> capacity << 1;\
            lagus_bst_node_##E * buffer = bst -> memory_management.reallocate (bst -> buffer + 1, capacity * sizeof (lagus_bst_node_##E));\
            if (! buffer)\
                return lagus_memory_allocation;\
            bst -> capacity = capacity;\
            bst -> buffer = buffer - 1;\
        }\
        size ++;\
        bst -> buffer [size] = (lagus_bst_node_##E) {\
            .left_child = lagus_none,\
            .right_child = lagus_none,\
            .size = 1,\
            .configuration = 0b00,\
            .element = element\
        };\
        uint8_t direction;\
        uint64_t node = lagus_root;\
        while (true) {\
            uint64_t child;\
            int64_t comparison = bst -> compare (element, bst -> buffer [node].element);\
            if (comparison < 0) {\
                child = bst -> buffer [node].left_child;\
                if (! child) {\
                    bst -> buffer [node].left_child = size;\
                    direction = lagus_left;\
                    break;\
                }\
            }\
            else {\
                child = bst -> buffer [node].right_child;\
                if (! child) {\
                    bst -> buffer [node].right_child = size;\
                    direction = lagus_right;\
                    break;\
                }\
            }\
            node = child;\
        }\
        bst -> buffer [size].parent = node;\
        \
        uint64_t child;\
        while (true) {\
            bst -> buffer [node].size ++;\
            if (bst -> buffer [node].configuration == 0b00) {\
                if (direction == lagus_left)\
                    bst -> buffer [node].configuration = 0b10;\
                else\
                    bst -> buffer [node].configuration = 0b01;\
                if (node == lagus_root) {\
                    bst -> height ++;\
                    return lagus_success;\
                }\
                child = node;\
                node = bst -> buffer [node].parent;\
                if (bst -> buffer [node].left_child == child)\
                    direction = lagus_left;\
                else\
                    direction = lagus_right;\
                continue;\
            }\
            if (bst -> buffer [node].configuration == 0b10) {\
                if (direction == lagus_left) {\
                    child = bst -> buffer [node].left_child;\
                    if (bst -> buffer [child].configuration == 0b10) {\
                        uint64_t b = child;\
                        uint64_t a = bst -> buffer [b].left_child;\
                        bst -> buffer [b].size -= bst -> buffer [a].size;\
                        uint64_t X = bst -> buffer [b].right_child;\
                        bst -> buffer [b].left_child = X;\
                        uint64_t W = bst -> buffer [node].right_child;\
                        bst -> buffer [b].right_child = W;\
                        if (W) {\
                            bst -> buffer [W].parent = b;\
                            bst -> buffer [b].size += bst -> buffer [W].size;\
                        }\
                        bst -> buffer [node].left_child = a;\
                        bst -> buffer [a].parent = node;\
                        bst -> buffer [node].right_child = b;\
                        bst -> buffer [b].parent = node;\
                        E temporal = bst -> buffer [node].element;\
                        bst -> buffer [node].element = bst -> buffer [b].element;\
                        bst -> buffer [b].element = temporal;\
                        bst -> buffer [child].configuration = 0b00;\
                    } else {\
                        uint64_t a = child;\
                        uint64_t b = bst -> buffer [a].right_child;\
                        bst -> buffer [a].size -= bst -> buffer [b].size;\
                        uint64_t Y = bst -> buffer [b].left_child;\
                        bst -> buffer [a].right_child = Y;\
                        if (Y) {\
                            bst -> buffer [Y].parent = a;\
                            bst -> buffer [a].size += bst -> buffer [Y].size;\
                            bst -> buffer [b].size -= bst -> buffer [Y].size;\
                        }\
                        uint64_t X = bst -> buffer [b].right_child;\
                        bst -> buffer [b].left_child = X;\
                        uint64_t W = bst -> buffer [node].right_child;\
                        bst -> buffer [b].right_child = W;\
                        if (W) {\
                            bst -> buffer [W].parent = b;\
                            bst -> buffer [b].size += bst -> buffer [W].size;\
                        }\
                        bst -> buffer [node].right_child = b;\
                        bst -> buffer [b].parent = node;\
                        E temporal = bst -> buffer [node].element;\
                        bst -> buffer [node].element = bst -> buffer [b].element;\
                        bst -> buffer [b].element = temporal;\
                        if (bst -> buffer [b].configuration == 0b00)\
                            bst -> buffer [a].configuration = 0b00;\
                        else if (bst -> buffer [b].configuration == 0b10) {\
                            bst -> buffer [a].configuration = 0b00;\
                            bst -> buffer [b].configuration = 0b01;\
                        } else {\
                            bst -> buffer [a].configuration = 0b10;\
                            bst -> buffer [b].configuration = 0b00;\
                        }\
                    }\
                }\
            } else {\
                if (direction == lagus_right) {\
                    child = bst -> buffer [node].right_child;\
                    if (bst -> buffer [child].configuration == 0b01) {\
                        uint64_t b = child;\
                        uint64_t c = bst -> buffer [b].right_child;\
                        bst -> buffer [b].size -= bst -> buffer [c].size;\
                        uint64_t Y = bst -> buffer [b].left_child;\
                        bst -> buffer [b].right_child = Y;\
                        uint64_t Z = bst -> buffer [node].left_child;\
                        bst -> buffer [b].left_child = Z;\
                        if (Z) {\
                            bst -> buffer [Z].parent = b;\
                            bst -> buffer [b].size += bst -> buffer [Z].size;\
                        }\
                        bst -> buffer [node].right_child = c;\
                        bst -> buffer [c].parent = node;\
                        bst -> buffer [node].left_child = b;\
                        bst -> buffer [b].parent = node;\
                        E temporal = bst -> buffer [node].element;\
                        bst -> buffer [node].element = bst -> buffer [b].element;\
                        bst -> buffer [b].element = temporal;\
                        bst -> buffer [child].configuration = 0b00;\
                    } else {\
                        uint64_t c = child;\
                        uint64_t b = bst -> buffer [c].left_child;\
                        bst -> buffer [c].size -= bst -> buffer [b].size;\
                        uint64_t X = bst -> buffer [b].right_child;\
                        bst -> buffer [c].left_child = X;\
                        if (X) {\
                            bst -> buffer [X].parent = c;\
                            bst -> buffer [c].size += bst -> buffer [X].size;\
                            bst -> buffer [b].size -= bst -> buffer [X].size;\
                        }\
                        uint64_t Y = bst -> buffer [b].left_child;\
                        bst -> buffer [b].right_child = Y;\
                        uint64_t Z = bst -> buffer [node].left_child;\
                        bst -> buffer [b].left_child = Z;\
                        if (Z) {\
                            bst -> buffer [Z].parent = b;\
                            bst -> buffer [b].size += bst -> buffer [Z].size;\
                        }\
                        bst -> buffer [node].left_child = b;\
                        bst -> buffer [b].parent = node;\
                        E temporal = bst -> buffer [node].element;\
                        bst -> buffer [node].element = bst -> buffer [b].element;\
                        bst -> buffer [b].element = temporal;\
                        if (bst -> buffer [b].configuration == 0b00) {\
                            bst -> buffer [c].configuration = 0b00;\
                        } else if (bst -> buffer [b].configuration == 0b01) {\
                            bst -> buffer [c].configuration = 0b00;\
                            bst -> buffer [b].configuration = 0b10;\
                        } else {\
                            bst -> buffer [c].configuration = 0b01;\
                            bst -> buffer [b].configuration = 0b00;\
                        }\
                    }\
                }\
            }\
            bst -> buffer [node].configuration = 0b00;\
            break;\
        }\
        while (node != lagus_root) {\
            node = bst -> buffer [node].parent;\
            bst -> buffer [node].size ++;\
        }\
    }\
    return lagus_success;\
}\
\
lagus_result lagus_bst_##E##_add_at (lagus_bst_##E * bst, uint64_t index, E element) {\
    if (bst -> height == 0) {\
        bst -> buffer [lagus_root].size = 1;\
        bst -> buffer [lagus_root].configuration = 0b00;\
        bst -> buffer [lagus_root].element = element;\
        bst -> height = 1;\
    } else {\
        uint64_t size = bst -> buffer [lagus_root].size;\
        if (size == bst -> capacity) {\
            uint64_t capacity = bst -> capacity << 1;\
            lagus_bst_node_##E * buffer = bst -> memory_management.reallocate (bst -> buffer + 1, capacity * sizeof (lagus_bst_node_##E));\
            if (! buffer)\
                return lagus_memory_allocation;\
            bst -> capacity = capacity;\
            bst -> buffer = buffer - 1;\
        }\
        \
        size ++;\
        bst -> buffer [size] = (lagus_bst_node_##E) {\
            .left_child = lagus_none,\
            .right_child = lagus_none,\
            .size = 1,\
            .configuration = 0b00,\
            .element = element\
        };\
        uint8_t direction;\
        uint64_t node = lagus_root;\
        if (index == 1) {\
            uint64_t child;\
            while (true) {\
                child = bst -> buffer [node].left_child;\
                if (! child)\
                    break;\
                node = child;\
            }\
            bst -> buffer [node].left_child = size;\
            direction = lagus_left;\
        } else if (index == size) {\
            uint64_t child;\
            while (true) {\
                child = bst -> buffer [node].right_child;\
                if (! child)\
                    break;\
                node = child;\
            }\
            bst -> buffer [node].right_child = size;\
            direction = lagus_right;\
        } else {\
            while (true) {\
                uint64_t left_child = bst -> buffer [node].left_child;\
                uint64_t size;\
                if (left_child)\
                    size = bst -> buffer [left_child].size;\
                else\
                    size = 0;\
                if (index == size + 1)\
                    break;\
                if (index <= size)\
                    node = left_child;\
                else {\
                    index -= size + 1;\
                    node = bst -> buffer [node].right_child;\
                }\
            }\
            uint64_t child = bst -> buffer [node].left_child;\
            if (child) {\
                node = child;\
                while (true) {\
                    child = bst -> buffer [node].right_child;\
                    if (! child)\
                        break;\
                    node = child;\
                }\
                bst -> buffer [node].right_child = size;\
                direction = lagus_right;\
            } else {\
                bst -> buffer [node].left_child = size;\
                direction = lagus_left;\
            }\
        }\
        bst -> buffer [size].parent = node;\
        \
        uint64_t child;\
        while (true) {\
            bst -> buffer [node].size ++;\
            if (bst -> buffer [node].configuration == 0b00) {\
                if (direction == lagus_left)\
                    bst -> buffer [node].configuration = 0b10;\
                else\
                    bst -> buffer [node].configuration = 0b01;\
                if (node == lagus_root) {\
                    bst -> height ++;\
                    return lagus_success;\
                }\
                child = node;\
                node = bst -> buffer [node].parent;\
                if (bst -> buffer [node].left_child == child)\
                    direction = lagus_left;\
                else\
                    direction = lagus_right;\
                continue;\
            }\
            if (bst -> buffer [node].configuration == 0b10) {\
                if (direction == lagus_left) {\
                    child = bst -> buffer [node].left_child;\
                    if (bst -> buffer [child].configuration == 0b10) {\
                        uint64_t b = child;\
                        uint64_t a = bst -> buffer [b].left_child;\
                        bst -> buffer [b].size -= bst -> buffer [a].size;\
                        uint64_t X = bst -> buffer [b].right_child;\
                        bst -> buffer [b].left_child = X;\
                        uint64_t W = bst -> buffer [node].right_child;\
                        bst -> buffer [b].right_child = W;\
                        if (W) {\
                            bst -> buffer [W].parent = b;\
                            bst -> buffer [b].size += bst -> buffer [W].size;\
                        }\
                        bst -> buffer [node].left_child = a;\
                        bst -> buffer [a].parent = node;\
                        bst -> buffer [node].right_child = b;\
                        bst -> buffer [b].parent = node;\
                        E temporal = bst -> buffer [node].element;\
                        bst -> buffer [node].element = bst -> buffer [b].element;\
                        bst -> buffer [b].element = temporal;\
                        bst -> buffer [child].configuration = 0b00;\
                    } else {\
                        uint64_t a = child;\
                        uint64_t b = bst -> buffer [a].right_child;\
                        bst -> buffer [a].size -= bst -> buffer [b].size;\
                        uint64_t Y = bst -> buffer [b].left_child;\
                        bst -> buffer [a].right_child = Y;\
                        if (Y) {\
                            bst -> buffer [Y].parent = a;\
                            bst -> buffer [a].size += bst -> buffer [Y].size;\
                            bst -> buffer [b].size -= bst -> buffer [Y].size;\
                        }\
                        uint64_t X = bst -> buffer [b].right_child;\
                        bst -> buffer [b].left_child = X;\
                        uint64_t W = bst -> buffer [node].right_child;\
                        bst -> buffer [b].right_child = W;\
                        if (W) {\
                            bst -> buffer [W].parent = b;\
                            bst -> buffer [b].size += bst -> buffer [W].size;\
                        }\
                        bst -> buffer [node].right_child = b;\
                        bst -> buffer [b].parent = node;\
                        E temporal = bst -> buffer [node].element;\
                        bst -> buffer [node].element = bst -> buffer [b].element;\
                        bst -> buffer [b].element = temporal;\
                        if (bst -> buffer [b].configuration == 0b00)\
                            bst -> buffer [a].configuration = 0b00;\
                        else if (bst -> buffer [b].configuration == 0b10) {\
                            bst -> buffer [a].configuration = 0b00;\
                            bst -> buffer [b].configuration = 0b01;\
                        } else {\
                            bst -> buffer [a].configuration = 0b10;\
                            bst -> buffer [b].configuration = 0b00;\
                        }\
                    }\
                }\
            } else {\
                if (direction == lagus_right) {\
                    child = bst -> buffer [node].right_child;\
                    if (bst -> buffer [child].configuration == 0b01) {\
                        uint64_t b = child;\
                        uint64_t c = bst -> buffer [b].right_child;\
                        bst -> buffer [b].size -= bst -> buffer [c].size;\
                        uint64_t Y = bst -> buffer [b].left_child;\
                        bst -> buffer [b].right_child = Y;\
                        uint64_t Z = bst -> buffer [node].left_child;\
                        bst -> buffer [b].left_child = Z;\
                        if (Z) {\
                            bst -> buffer [Z].parent = b;\
                            bst -> buffer [b].size += bst -> buffer [Z].size;\
                        }\
                        bst -> buffer [node].right_child = c;\
                        bst -> buffer [c].parent = node;\
                        bst -> buffer [node].left_child = b;\
                        bst -> buffer [b].parent = node;\
                        E temporal = bst -> buffer [node].element;\
                        bst -> buffer [node].element = bst -> buffer [b].element;\
                        bst -> buffer [b].element = temporal;\
                        bst -> buffer [child].configuration = 0b00;\
                    } else {\
                        uint64_t c = child;\
                        uint64_t b = bst -> buffer [c].left_child;\
                        bst -> buffer [c].size -= bst -> buffer [b].size;\
                        uint64_t X = bst -> buffer [b].right_child;\
                        bst -> buffer [c].left_child = X;\
                        if (X) {\
                            bst -> buffer [X].parent = c;\
                            bst -> buffer [c].size += bst -> buffer [X].size;\
                            bst -> buffer [b].size -= bst -> buffer [X].size;\
                        }\
                        uint64_t Y = bst -> buffer [b].left_child;\
                        bst -> buffer [b].right_child = Y;\
                        uint64_t Z = bst -> buffer [node].left_child;\
                        bst -> buffer [b].left_child = Z;\
                        if (Z) {\
                            bst -> buffer [Z].parent = b;\
                            bst -> buffer [b].size += bst -> buffer [Z].size;\
                        }\
                        bst -> buffer [node].left_child = b;\
                        bst -> buffer [b].parent = node;\
                        E temporal = bst -> buffer [node].element;\
                        bst -> buffer [node].element = bst -> buffer [b].element;\
                        bst -> buffer [b].element = temporal;\
                        if (bst -> buffer [b].configuration == 0b00) {\
                            bst -> buffer [c].configuration = 0b00;\
                        } else if (bst -> buffer [b].configuration == 0b01) {\
                            bst -> buffer [c].configuration = 0b00;\
                            bst -> buffer [b].configuration = 0b10;\
                        } else {\
                            bst -> buffer [c].configuration = 0b01;\
                            bst -> buffer [b].configuration = 0b00;\
                        }\
                    }\
                }\
            }\
            bst -> buffer [node].configuration = 0b00;\
            break;\
        }\
        while (node != lagus_root) {\
            node = bst -> buffer [node].parent;\
            bst -> buffer [node].size ++;\
        }\
    }\
    return lagus_success;\
}\
\
lagus_result lagus_bst_##E##_search_left (lagus_bst_##E * bst, E * element, uint64_t * index) {\
    if (bst -> height == 0)\
        return lagus_not_contained;\
    uint64_t offset = 0;\
    uint64_t match = lagus_none;\
    uint64_t node = lagus_root;\
    while (true) {\
        uint64_t child;\
        int64_t comparison = bst -> compare (* element, bst -> buffer [node].element);\
        if (comparison == 0) {\
            child = bst -> buffer [node].left_child;\
            if (! child) {\
                * element = bst -> buffer [node].element;\
                * index = offset + 1;\
                return lagus_success;\
            }\
            * index = offset + bst -> buffer [child].size + 1;\
            match = node;\
        } else {\
            if (comparison < 0)\
                child = bst -> buffer [node].left_child;\
            else {\
                child = bst -> buffer [node].right_child;\
                uint64_t left_child = bst -> buffer [node].left_child;\
                if (left_child)\
                    offset += bst -> buffer [left_child].size + 1;\
                else\
                    offset ++;\
            }\
            if (! child)\
                break;\
        }\
        node = child;\
    }\
    if (! match)\
        return lagus_not_contained;\
    * element = bst -> buffer [match].element;\
    return lagus_success;\
}\
\
lagus_result lagus_bst_##E##_search_right (lagus_bst_##E * bst, E * element, uint64_t * index) {\
    if (bst -> height == 0)\
        return lagus_not_contained;\
    uint64_t offset = 0;\
    uint64_t match = lagus_none;\
    uint64_t node = lagus_root;\
    while (true) {\
        uint64_t child;\
        int64_t comparison = bst -> compare (* element, bst -> buffer [node].element);\
        if (comparison == 0) {\
            child = bst -> buffer [node].right_child;\
            uint64_t left_child = bst -> buffer [node].left_child;\
            if (left_child)\
                offset += bst -> buffer [left_child].size + 1;\
            else\
                offset ++;\
            * index = offset;\
            if (! child) {\
                * element = bst -> buffer [node].element;\
                return lagus_success;\
            }\
            match = node;\
        } else {\
            if (comparison < 0)\
                child = bst -> buffer [node].left_child;\
            else {\
                child = bst -> buffer [node].right_child;\
                uint64_t left_child = bst -> buffer [node].left_child;\
                if (left_child)\
                    offset += bst -> buffer [left_child].size + 1;\
                else\
                    offset ++;\
            }\
            if (! child)\
                break;\
        }\
        node = child;\
    }\
    if (! match)\
        return lagus_not_contained;\
    * element = bst -> buffer [match].element;\
    return lagus_success;\
}\
\
lagus_result lagus_bst_##E##_remove_left (lagus_bst_##E * bst, E * element) {\
    if (bst -> height == 0)\
        return lagus_not_contained;\
    if (bst -> height == 1) {\
        int64_t comparison = bst -> compare (* element, bst -> buffer [lagus_root].element);\
        if (comparison == 0) {\
            bst -> height = 0;\
            bst -> buffer [lagus_root].size = 0;\
            * element = bst -> buffer [lagus_root].element;\
            return lagus_success;\
        }\
        return lagus_not_contained;\
    }\
    uint64_t child;\
    uint64_t match = lagus_none;\
    uint64_t node = lagus_root;\
    while (true) {\
        int64_t comparison = bst -> compare (* element, bst -> buffer [node].element);\
        if (comparison == 0) {\
            child = bst -> buffer [node].left_child;\
            if (! child) {\
                * element = bst -> buffer [node].element;\
                child = bst -> buffer [node].right_child;\
                break;\
            }\
            match = node;\
        } else {\
            if (comparison < 0) {\
                child = bst -> buffer [node].left_child;\
            } else\
                child = bst -> buffer [node].right_child;\
            if (! child) {\
                if (! match)\
                    return lagus_not_contained;\
                * element = bst -> buffer [match].element;\
                bst -> buffer [match].element = bst -> buffer [node].element;\
                child = bst -> buffer [node].left_child;\
                break;\
            }\
        }\
        node = child;\
    }\
    if (child) {\
        bst -> buffer [node].element = bst -> buffer [child].element;\
        node = child;\
    }\
    \
    uint8_t direction;\
    uint64_t parent = bst -> buffer [node].parent;\
    uint64_t size = bst -> buffer [lagus_root].size;\
    if (parent == size) {\
        bst -> buffer [node].configuration = bst -> buffer [parent].configuration;\
        bst -> buffer [node].element = bst -> buffer [parent].element;\
        if (bst -> buffer [parent].left_child == node) {\
            direction = lagus_left;\
            child = bst -> buffer [parent].right_child;\
            bst -> buffer [node].right_child = child;\
        } else {\
            direction = lagus_right;\
            child = bst -> buffer [parent].left_child;\
            bst -> buffer [node].left_child = child;\
        }\
        if (child)\
            bst -> buffer [child].parent = node;\
        parent = bst -> buffer [parent].parent;\
        bst -> buffer [node].parent = parent;\
        if (bst -> buffer [parent].left_child == size)\
            bst -> buffer [parent].left_child = node;\
        else\
            bst -> buffer [parent].right_child = node;\
    } else {\
        if (bst -> buffer [parent].left_child == node) {\
            direction = lagus_left;\
            bst -> buffer [parent].left_child = lagus_none;\
        } else {\
            direction = lagus_right;\
            bst -> buffer [parent].right_child = lagus_none;\
        }\
        if (node != size) {\
            bst -> buffer [node] = bst -> buffer [size];\
            uint64_t parent = bst -> buffer [node].parent;\
            if (bst -> buffer [parent].left_child == size)\
                bst -> buffer [parent].left_child = node;\
            else\
                bst -> buffer [parent].right_child = node;\
            child = bst -> buffer [node].left_child;\
            if (child)\
                bst -> buffer [child].parent = node;\
            child = bst -> buffer [node].right_child;\
            if (child)\
                bst -> buffer [child].parent = node;\
        }\
        node = parent;\
    }\
    size --;\
    \
    while (true) {\
        bst -> buffer [node].size --;\
        if (bst -> buffer [node].configuration == 0b00) {\
            if (direction == lagus_left)\
                bst -> buffer [node].configuration = 0b01;\
            else\
                bst -> buffer [node].configuration = 0b10;\
            break;\
        } else if (bst -> buffer [node].configuration == 0b10) {\
            if (direction == lagus_right) {\
                child = bst -> buffer [node].left_child;\
                if (bst -> buffer [child].configuration == 0b01) {\
                    uint64_t a = child;\
                    uint64_t b = bst -> buffer [a].right_child;\
                    bst -> buffer [a].size -= bst -> buffer [b].size;\
                    uint64_t Y = bst -> buffer [b].left_child;\
                    bst -> buffer [a].right_child = Y;\
                    if (Y) {\
                        bst -> buffer [Y].parent = a;\
                        bst -> buffer [a].size += bst -> buffer [Y].size;\
                        bst -> buffer [b].size -= bst -> buffer [Y].size;\
                    }\
                    uint64_t X = bst -> buffer [b].right_child;\
                    bst -> buffer [b].left_child = X;\
                    uint64_t W = bst -> buffer [node].right_child;\
                    bst -> buffer [b].right_child = W;\
                    if (W) {\
                        bst -> buffer [W].parent = b;\
                        bst -> buffer [b].size += bst -> buffer [W].size;\
                    }\
                    bst -> buffer [node].right_child = b;\
                    bst -> buffer [b].parent = node;\
                    E temporal = bst -> buffer [node].element;\
                    bst -> buffer [node].element = bst -> buffer [b].element;\
                    bst -> buffer [b].element = temporal;\
                    if (bst -> buffer [b].configuration == 0b00) {\
                        bst -> buffer [a].configuration = 0b00;\
                    } else if (bst -> buffer [b].configuration == 0b10) {\
                        bst -> buffer [a].configuration = 0b00;\
                        bst -> buffer [b].configuration = 0b01;\
                    } else {\
                        bst -> buffer [a].configuration = 0b10;\
                        bst -> buffer [b].configuration = 0b00;\
                    }\
                } else {\
                    \
                    uint64_t b = child;\
                    uint64_t a = bst -> buffer [b].left_child;\
                    bst -> buffer [b].size -= bst -> buffer [a].size;\
                    uint64_t X = bst -> buffer [b].right_child;\
                    bst -> buffer [b].left_child = X;\
                    uint64_t W = bst -> buffer [node].right_child;\
                    bst -> buffer [b].right_child = W;\
                    if (W) {\
                        bst -> buffer [W].parent = b;\
                        bst -> buffer [b].size += bst -> buffer [W].size;\
                    }\
                    bst -> buffer [node].left_child = a;\
                    bst -> buffer [a].parent = node;\
                    bst -> buffer [node].right_child = b;\
                    bst -> buffer [b].parent = node;\
                    E temporal = bst -> buffer [node].element;\
                    bst -> buffer [node].element = bst -> buffer [b].element;\
                    bst -> buffer [b].element = temporal;\
                    if (bst -> buffer [child].configuration == 0b00) {\
                        bst -> buffer [child].configuration = 0b10;\
                        bst -> buffer [node].configuration = 0b01;\
                        break;\
                    } else\
                        bst -> buffer [child].configuration = 0b00;\
                    \
                }\
            }\
        } else {\
            if (direction == lagus_left) {\
                child = bst -> buffer [node].right_child;\
                if (bst -> buffer [child].configuration == 0b10) {\
                    uint64_t c = child;\
                    uint64_t b = bst -> buffer [c].left_child;\
                    bst -> buffer [c].size -= bst -> buffer [b].size;\
                    uint64_t X = bst -> buffer [b].right_child;\
                    bst -> buffer [c].left_child = X;\
                    if (X) {\
                        bst -> buffer [X].parent = c;\
                        bst -> buffer [c].size += bst -> buffer [X].size;\
                        bst -> buffer [b].size -= bst -> buffer [X].size;\
                    }\
                    uint64_t Y = bst -> buffer [b].left_child;\
                    bst -> buffer [b].right_child = Y;\
                    uint64_t Z = bst -> buffer [node].left_child;\
                    bst -> buffer [b].left_child = Z;\
                    if (Z) {\
                        bst -> buffer [Z].parent = b;\
                        bst -> buffer [b].size += bst -> buffer [Z].size;\
                    }\
                    bst -> buffer [node].left_child = b;\
                    bst -> buffer [b].parent = node;\
                    E temporal = bst -> buffer [node].element;\
                    bst -> buffer [node].element = bst -> buffer [b].element;\
                    bst -> buffer [b].element = temporal;\
                    if (bst -> buffer [b].configuration == 0b00) {\
                        bst -> buffer [c].configuration = 0b00;\
                    } else if (bst -> buffer [b].configuration == 0b01) {\
                        bst -> buffer [c].configuration = 0b00;\
                        bst -> buffer [b].configuration = 0b10;\
                    } else {\
                        bst -> buffer [c].configuration = 0b01;\
                        bst -> buffer [b].configuration = 0b00;\
                    }\
                } else {\
                    uint64_t b = child;\
                    uint64_t c = bst -> buffer [b].right_child;\
                    bst -> buffer [b].size -= bst -> buffer [c].size;\
                    uint64_t Y = bst -> buffer [b].left_child;\
                    bst -> buffer [b].right_child = Y;\
                    uint64_t Z = bst -> buffer [node].left_child;\
                    bst -> buffer [b].left_child = Z;\
                    if (Z) {\
                        bst -> buffer [Z].parent = b;\
                        bst -> buffer [b].size += bst -> buffer [Z].size;\
                    }\
                    bst -> buffer [node].right_child = c;\
                    bst -> buffer [c].parent = node;\
                    bst -> buffer [node].left_child = b;\
                    bst -> buffer [b].parent = node;\
                    E temporal = bst -> buffer [node].element;\
                    bst -> buffer [node].element = bst -> buffer [b].element;\
                    bst -> buffer [b].element = temporal;\
                    if (bst -> buffer [child].configuration == 0b00) {\
                        bst -> buffer [child].configuration = 0b01;\
                        bst -> buffer [node].configuration = 0b10;\
                        break;\
                    } else\
                        bst -> buffer [child].configuration = 0b00;\
                    \
                }\
            }\
        }\
        bst -> buffer [node].configuration = 0b00;\
        if (node == lagus_root) {\
            bst -> height --;\
            return lagus_success;\
        }\
        child = node;\
        node = bst -> buffer [node].parent;\
        if (bst -> buffer [node].left_child == child)\
            direction = lagus_left;\
        else\
            direction = lagus_right;\
    }\
    while (node != lagus_root) {\
        node = bst -> buffer [node].parent;\
        bst -> buffer [node].size --;\
    }\
    \
    uint64_t capacity;\
    if (bst -> capacity < 4)\
        capacity = 1;\
    else\
        capacity = bst -> capacity >> 2;\
    if (size <= capacity) {\
        lagus_bst_node_##E * buffer = bst -> memory_management.reallocate (bst -> buffer + 1, capacity * sizeof (lagus_bst_node_##E));\
        if (! buffer)\
            return lagus_memory_allocation;\
        bst -> capacity = capacity;\
        bst -> buffer = buffer - 1;\
    }\
    return lagus_success;\
}\
\
lagus_result lagus_bst_##E##_remove_right (lagus_bst_##E * bst, E * element) {\
    if (bst -> height == 0)\
        return lagus_not_contained;\
    if (bst -> height == 1) {\
        int64_t comparison = bst -> compare (* element, bst -> buffer [lagus_root].element);\
        if (comparison == 0) {\
            bst -> height = 0;\
            bst -> buffer [lagus_root].size = 0;\
            * element = bst -> buffer [lagus_root].element;\
            return lagus_success;\
        }\
        return lagus_not_contained;\
    }\
    uint64_t child;\
    uint64_t match = lagus_none;\
    uint64_t node = lagus_root;\
    while (true) {\
        int64_t comparison = bst -> compare (* element, bst -> buffer [node].element);\
        if (comparison == 0) {\
            child = bst -> buffer [node].right_child;\
            if (! child) {\
                * element = bst -> buffer [node].element;\
                child = bst -> buffer [node].left_child;\
                break;\
            }\
            match = node;\
        } else {\
            if (comparison < 0) {\
                child = bst -> buffer [node].left_child;\
            } else\
                child = bst -> buffer [node].right_child;\
            if (! child) {\
                if (! match)\
                    return lagus_not_contained;\
                * element = bst -> buffer [match].element;\
                bst -> buffer [match].element = bst -> buffer [node].element;\
                child = bst -> buffer [node].right_child;\
                break;\
            }\
        }\
        node = child;\
    }\
    if (child) {\
        bst -> buffer [node].element = bst -> buffer [child].element;\
        node = child;\
    }\
    \
    uint8_t direction;\
    uint64_t parent = bst -> buffer [node].parent;\
    uint64_t size = bst -> buffer [lagus_root].size;\
    if (parent == size) {\
        bst -> buffer [node].configuration = bst -> buffer [parent].configuration;\
        bst -> buffer [node].element = bst -> buffer [parent].element;\
        if (bst -> buffer [parent].left_child == node) {\
            direction = lagus_left;\
            child = bst -> buffer [parent].right_child;\
            bst -> buffer [node].right_child = child;\
        } else {\
            direction = lagus_right;\
            child = bst -> buffer [parent].left_child;\
            bst -> buffer [node].left_child = child;\
        }\
        if (child)\
            bst -> buffer [child].parent = node;\
        parent = bst -> buffer [parent].parent;\
        bst -> buffer [node].parent = parent;\
        if (bst -> buffer [parent].left_child == size)\
            bst -> buffer [parent].left_child = node;\
        else\
            bst -> buffer [parent].right_child = node;\
    } else {\
        if (bst -> buffer [parent].left_child == node) {\
            direction = lagus_left;\
            bst -> buffer [parent].left_child = lagus_none;\
        } else {\
            direction = lagus_right;\
            bst -> buffer [parent].right_child = lagus_none;\
        }\
        if (node != size) {\
            bst -> buffer [node] = bst -> buffer [size];\
            uint64_t parent = bst -> buffer [node].parent;\
            if (bst -> buffer [parent].left_child == size)\
                bst -> buffer [parent].left_child = node;\
            else\
                bst -> buffer [parent].right_child = node;\
            child = bst -> buffer [node].left_child;\
            if (child)\
                bst -> buffer [child].parent = node;\
            child = bst -> buffer [node].right_child;\
            if (child)\
                bst -> buffer [child].parent = node;\
        }\
        node = parent;\
    }\
    size --;\
    \
    while (true) {\
        bst -> buffer [node].size --;\
        if (bst -> buffer [node].configuration == 0b00) {\
            if (direction == lagus_left)\
                bst -> buffer [node].configuration = 0b01;\
            else\
                bst -> buffer [node].configuration = 0b10;\
            break;\
        } else if (bst -> buffer [node].configuration == 0b10) {\
            if (direction == lagus_right) {\
                child = bst -> buffer [node].left_child;\
                if (bst -> buffer [child].configuration == 0b01) {\
                    uint64_t a = child;\
                    uint64_t b = bst -> buffer [a].right_child;\
                    bst -> buffer [a].size -= bst -> buffer [b].size;\
                    uint64_t Y = bst -> buffer [b].left_child;\
                    bst -> buffer [a].right_child = Y;\
                    if (Y) {\
                        bst -> buffer [Y].parent = a;\
                        bst -> buffer [a].size += bst -> buffer [Y].size;\
                        bst -> buffer [b].size -= bst -> buffer [Y].size;\
                    }\
                    uint64_t X = bst -> buffer [b].right_child;\
                    bst -> buffer [b].left_child = X;\
                    uint64_t W = bst -> buffer [node].right_child;\
                    bst -> buffer [b].right_child = W;\
                    if (W) {\
                        bst -> buffer [W].parent = b;\
                        bst -> buffer [b].size += bst -> buffer [W].size;\
                    }\
                    bst -> buffer [node].right_child = b;\
                    bst -> buffer [b].parent = node;\
                    E temporal = bst -> buffer [node].element;\
                    bst -> buffer [node].element = bst -> buffer [b].element;\
                    bst -> buffer [b].element = temporal;\
                    if (bst -> buffer [b].configuration == 0b00) {\
                        bst -> buffer [a].configuration = 0b00;\
                    } else if (bst -> buffer [b].configuration == 0b10) {\
                        bst -> buffer [a].configuration = 0b00;\
                        bst -> buffer [b].configuration = 0b01;\
                    } else {\
                        bst -> buffer [a].configuration = 0b10;\
                        bst -> buffer [b].configuration = 0b00;\
                    }\
                } else {\
                    \
                    uint64_t b = child;\
                    uint64_t a = bst -> buffer [b].left_child;\
                    bst -> buffer [b].size -= bst -> buffer [a].size;\
                    uint64_t X = bst -> buffer [b].right_child;\
                    bst -> buffer [b].left_child = X;\
                    uint64_t W = bst -> buffer [node].right_child;\
                    bst -> buffer [b].right_child = W;\
                    if (W) {\
                        bst -> buffer [W].parent = b;\
                        bst -> buffer [b].size += bst -> buffer [W].size;\
                    }\
                    bst -> buffer [node].left_child = a;\
                    bst -> buffer [a].parent = node;\
                    bst -> buffer [node].right_child = b;\
                    bst -> buffer [b].parent = node;\
                    E temporal = bst -> buffer [node].element;\
                    bst -> buffer [node].element = bst -> buffer [b].element;\
                    bst -> buffer [b].element = temporal;\
                    if (bst -> buffer [child].configuration == 0b00) {\
                        bst -> buffer [child].configuration = 0b10;\
                        bst -> buffer [node].configuration = 0b01;\
                        break;\
                    } else\
                        bst -> buffer [child].configuration = 0b00;\
                    \
                }\
            }\
        } else {\
            if (direction == lagus_left) {\
                child = bst -> buffer [node].right_child;\
                if (bst -> buffer [child].configuration == 0b10) {\
                    uint64_t c = child;\
                    uint64_t b = bst -> buffer [c].left_child;\
                    bst -> buffer [c].size -= bst -> buffer [b].size;\
                    uint64_t X = bst -> buffer [b].right_child;\
                    bst -> buffer [c].left_child = X;\
                    if (X) {\
                        bst -> buffer [X].parent = c;\
                        bst -> buffer [c].size += bst -> buffer [X].size;\
                        bst -> buffer [b].size -= bst -> buffer [X].size;\
                    }\
                    uint64_t Y = bst -> buffer [b].left_child;\
                    bst -> buffer [b].right_child = Y;\
                    uint64_t Z = bst -> buffer [node].left_child;\
                    bst -> buffer [b].left_child = Z;\
                    if (Z) {\
                        bst -> buffer [Z].parent = b;\
                        bst -> buffer [b].size += bst -> buffer [Z].size;\
                    }\
                    bst -> buffer [node].left_child = b;\
                    bst -> buffer [b].parent = node;\
                    E temporal = bst -> buffer [node].element;\
                    bst -> buffer [node].element = bst -> buffer [b].element;\
                    bst -> buffer [b].element = temporal;\
                    if (bst -> buffer [b].configuration == 0b00) {\
                        bst -> buffer [c].configuration = 0b00;\
                    } else if (bst -> buffer [b].configuration == 0b01) {\
                        bst -> buffer [c].configuration = 0b00;\
                        bst -> buffer [b].configuration = 0b10;\
                    } else {\
                        bst -> buffer [c].configuration = 0b01;\
                        bst -> buffer [b].configuration = 0b00;\
                    }\
                } else {\
                    uint64_t b = child;\
                    uint64_t c = bst -> buffer [b].right_child;\
                    bst -> buffer [b].size -= bst -> buffer [c].size;\
                    uint64_t Y = bst -> buffer [b].left_child;\
                    bst -> buffer [b].right_child = Y;\
                    uint64_t Z = bst -> buffer [node].left_child;\
                    bst -> buffer [b].left_child = Z;\
                    if (Z) {\
                        bst -> buffer [Z].parent = b;\
                        bst -> buffer [b].size += bst -> buffer [Z].size;\
                    }\
                    bst -> buffer [node].right_child = c;\
                    bst -> buffer [c].parent = node;\
                    bst -> buffer [node].left_child = b;\
                    bst -> buffer [b].parent = node;\
                    E temporal = bst -> buffer [node].element;\
                    bst -> buffer [node].element = bst -> buffer [b].element;\
                    bst -> buffer [b].element = temporal;\
                    if (bst -> buffer [child].configuration == 0b00) {\
                        bst -> buffer [child].configuration = 0b01;\
                        bst -> buffer [node].configuration = 0b10;\
                        break;\
                    } else\
                        bst -> buffer [child].configuration = 0b00;\
                    \
                }\
            }\
        }\
        bst -> buffer [node].configuration = 0b00;\
        if (node == lagus_root) {\
            bst -> height --;\
            return lagus_success;\
        }\
        child = node;\
        node = bst -> buffer [node].parent;\
        if (bst -> buffer [node].left_child == child)\
            direction = lagus_left;\
        else\
            direction = lagus_right;\
    }\
    while (node != lagus_root) {\
        node = bst -> buffer [node].parent;\
        bst -> buffer [node].size --;\
    }\
    uint64_t capacity;\
    if (bst -> capacity < 4)\
        capacity = 1;\
    else\
        capacity = bst -> capacity >> 2;\
    if (size <= capacity) {\
        lagus_bst_node_##E * buffer = bst -> memory_management.reallocate (bst -> buffer + 1, capacity * sizeof (lagus_bst_node_##E));\
        if (! buffer)\
            return lagus_memory_allocation;\
        bst -> capacity = capacity;\
        bst -> buffer = buffer - 1;\
    }\
    return lagus_success;\
}\
\
lagus_result lagus_bst_##E##_remove_at (lagus_bst_##E * bst, uint64_t index, E * element) {\
    if (bst -> height == 1) {\
        bst -> height = 0;\
        bst -> buffer [lagus_root].size = 0;\
        * element = bst -> buffer [lagus_root].element;\
        return lagus_success;\
    }\
    uint64_t child;\
    uint64_t node = lagus_root;\
    while (true) {\
        child = bst -> buffer [node].left_child;\
        uint64_t size;\
        if (child)\
            size = bst -> buffer [child].size;\
        else\
            size = 0;\
        if (index == size + 1)\
            break;\
        if (index <= size)\
            node = child;\
        else {\
            index -= size + 1;\
            node = bst -> buffer [node].right_child;\
        }\
    }\
    * element = bst -> buffer [node].element;\
    child = bst -> buffer [node].left_child;\
    if (child) {\
        uint64_t min = bst -> buffer [node].right_child;\
        if (min) {\
            while (true) {\
                child = bst -> buffer [min].left_child;\
                if (! child)\
                    break;\
                min = child;\
            }\
            bst -> buffer [node].element = bst -> buffer [min].element;\
            child = bst -> buffer [min].right_child;\
            if (child) {\
                bst -> buffer [min].element = bst -> buffer [child].element;\
                node = child;\
            } else\
                node = min;\
        } else  {\
            bst -> buffer [node].element = bst -> buffer [child].element;\
            node = child;\
        }\
    } else {\
        child = bst -> buffer [node].right_child;\
        if (child) {\
            bst -> buffer [node].element = bst -> buffer [child].element;\
            node = child;\
        }\
    }\
    uint8_t direction;\
    uint64_t parent = bst -> buffer [node].parent;\
    uint64_t size = bst -> buffer [lagus_root].size;\
    if (parent == size) {\
        bst -> buffer [node].configuration = bst -> buffer [parent].configuration;\
        bst -> buffer [node].element = bst -> buffer [parent].element;\
        if (bst -> buffer [parent].left_child == node) {\
            direction = lagus_left;\
            child = bst -> buffer [parent].right_child;\
            bst -> buffer [node].right_child = child;\
        } else {\
            direction = lagus_right;\
            child = bst -> buffer [parent].left_child;\
            bst -> buffer [node].left_child = child;\
        }\
        if (child)\
            bst -> buffer [child].parent = node;\
        parent = bst -> buffer [parent].parent;\
        bst -> buffer [node].parent = parent;\
        if (bst -> buffer [parent].left_child == size)\
            bst -> buffer [parent].left_child = node;\
        else\
            bst -> buffer [parent].right_child = node;\
    } else {\
        if (bst -> buffer [parent].left_child == node) {\
            direction = lagus_left;\
            bst -> buffer [parent].left_child = lagus_none;\
        } else {\
            direction = lagus_right;\
            bst -> buffer [parent].right_child = lagus_none;\
        }\
        if (node != size) {\
            bst -> buffer [node] = bst -> buffer [size];\
            uint64_t parent = bst -> buffer [node].parent;\
            if (bst -> buffer [parent].left_child == size)\
                bst -> buffer [parent].left_child = node;\
            else\
                bst -> buffer [parent].right_child = node;\
            child = bst -> buffer [node].left_child;\
            if (child)\
                bst -> buffer [child].parent = node;\
            child = bst -> buffer [node].right_child;\
            if (child)\
                bst -> buffer [child].parent = node;\
        }\
        node = parent;\
    }\
    size --;\
    \
    while (true) {\
        bst -> buffer [node].size --;\
        if (bst -> buffer [node].configuration == 0b00) {\
            if (direction == lagus_left)\
                bst -> buffer [node].configuration = 0b01;\
            else\
                bst -> buffer [node].configuration = 0b10;\
            break;\
        } else if (bst -> buffer [node].configuration == 0b10) {\
            if (direction == lagus_right) {\
                child = bst -> buffer [node].left_child;\
                if (bst -> buffer [child].configuration == 0b01) {\
                    uint64_t a = child;\
                    uint64_t b = bst -> buffer [a].right_child;\
                    bst -> buffer [a].size -= bst -> buffer [b].size;\
                    uint64_t Y = bst -> buffer [b].left_child;\
                    bst -> buffer [a].right_child = Y;\
                    if (Y) {\
                        bst -> buffer [Y].parent = a;\
                        bst -> buffer [a].size += bst -> buffer [Y].size;\
                        bst -> buffer [b].size -= bst -> buffer [Y].size;\
                    }\
                    uint64_t X = bst -> buffer [b].right_child;\
                    bst -> buffer [b].left_child = X;\
                    uint64_t W = bst -> buffer [node].right_child;\
                    bst -> buffer [b].right_child = W;\
                    if (W) {\
                        bst -> buffer [W].parent = b;\
                        bst -> buffer [b].size += bst -> buffer [W].size;\
                    }\
                    bst -> buffer [node].right_child = b;\
                    bst -> buffer [b].parent = node;\
                    E temporal = bst -> buffer [node].element;\
                    bst -> buffer [node].element = bst -> buffer [b].element;\
                    bst -> buffer [b].element = temporal;\
                    if (bst -> buffer [b].configuration == 0b00) {\
                        bst -> buffer [a].configuration = 0b00;\
                    } else if (bst -> buffer [b].configuration == 0b10) {\
                        bst -> buffer [a].configuration = 0b00;\
                        bst -> buffer [b].configuration = 0b01;\
                    } else {\
                        bst -> buffer [a].configuration = 0b10;\
                        bst -> buffer [b].configuration = 0b00;\
                    }\
                } else {\
                    \
                    uint64_t b = child;\
                    uint64_t a = bst -> buffer [b].left_child;\
                    bst -> buffer [b].size -= bst -> buffer [a].size;\
                    uint64_t X = bst -> buffer [b].right_child;\
                    bst -> buffer [b].left_child = X;\
                    uint64_t W = bst -> buffer [node].right_child;\
                    bst -> buffer [b].right_child = W;\
                    if (W) {\
                        bst -> buffer [W].parent = b;\
                        bst -> buffer [b].size += bst -> buffer [W].size;\
                    }\
                    bst -> buffer [node].left_child = a;\
                    bst -> buffer [a].parent = node;\
                    bst -> buffer [node].right_child = b;\
                    bst -> buffer [b].parent = node;\
                    E temporal = bst -> buffer [node].element;\
                    bst -> buffer [node].element = bst -> buffer [b].element;\
                    bst -> buffer [b].element = temporal;\
                    if (bst -> buffer [child].configuration == 0b00) {\
                        bst -> buffer [child].configuration = 0b10;\
                        bst -> buffer [node].configuration = 0b01;\
                        break;\
                    } else\
                        bst -> buffer [child].configuration = 0b00;\
                    \
                }\
            }\
        } else {\
            if (direction == lagus_left) {\
                child = bst -> buffer [node].right_child;\
                if (bst -> buffer [child].configuration == 0b10) {\
                    uint64_t c = child;\
                    uint64_t b = bst -> buffer [c].left_child;\
                    bst -> buffer [c].size -= bst -> buffer [b].size;\
                    uint64_t X = bst -> buffer [b].right_child;\
                    bst -> buffer [c].left_child = X;\
                    if (X) {\
                        bst -> buffer [X].parent = c;\
                        bst -> buffer [c].size += bst -> buffer [X].size;\
                        bst -> buffer [b].size -= bst -> buffer [X].size;\
                    }\
                    uint64_t Y = bst -> buffer [b].left_child;\
                    bst -> buffer [b].right_child = Y;\
                    uint64_t Z = bst -> buffer [node].left_child;\
                    bst -> buffer [b].left_child = Z;\
                    if (Z) {\
                        bst -> buffer [Z].parent = b;\
                        bst -> buffer [b].size += bst -> buffer [Z].size;\
                    }\
                    bst -> buffer [node].left_child = b;\
                    bst -> buffer [b].parent = node;\
                    E temporal = bst -> buffer [node].element;\
                    bst -> buffer [node].element = bst -> buffer [b].element;\
                    bst -> buffer [b].element = temporal;\
                    if (bst -> buffer [b].configuration == 0b00) {\
                        bst -> buffer [c].configuration = 0b00;\
                    } else if (bst -> buffer [b].configuration == 0b01) {\
                        bst -> buffer [c].configuration = 0b00;\
                        bst -> buffer [b].configuration = 0b10;\
                    } else {\
                        bst -> buffer [c].configuration = 0b01;\
                        bst -> buffer [b].configuration = 0b00;\
                    }\
                } else {\
                    uint64_t b = child;\
                    uint64_t c = bst -> buffer [b].right_child;\
                    bst -> buffer [b].size -= bst -> buffer [c].size;\
                    uint64_t Y = bst -> buffer [b].left_child;\
                    bst -> buffer [b].right_child = Y;\
                    uint64_t Z = bst -> buffer [node].left_child;\
                    bst -> buffer [b].left_child = Z;\
                    if (Z) {\
                        bst -> buffer [Z].parent = b;\
                        bst -> buffer [b].size += bst -> buffer [Z].size;\
                    }\
                    bst -> buffer [node].right_child = c;\
                    bst -> buffer [c].parent = node;\
                    bst -> buffer [node].left_child = b;\
                    bst -> buffer [b].parent = node;\
                    E temporal = bst -> buffer [node].element;\
                    bst -> buffer [node].element = bst -> buffer [b].element;\
                    bst -> buffer [b].element = temporal;\
                    if (bst -> buffer [child].configuration == 0b00) {\
                        bst -> buffer [child].configuration = 0b01;\
                        bst -> buffer [node].configuration = 0b10;\
                        break;\
                    } else\
                        bst -> buffer [child].configuration = 0b00;\
                    \
                }\
            }\
        }\
        bst -> buffer [node].configuration = 0b00;\
        if (node == lagus_root) {\
            bst -> height --;\
            return lagus_success;\
        }\
        child = node;\
        node = bst -> buffer [node].parent;\
        if (bst -> buffer [node].left_child == child)\
            direction = lagus_left;\
        else\
            direction = lagus_right;\
    }\
    while (node != lagus_root) {\
        node = bst -> buffer [node].parent;\
        bst -> buffer [node].size --;\
    }\
    uint64_t capacity;\
    if (bst -> capacity < 4)\
        capacity = 1;\
    else\
        capacity = bst -> capacity >> 2;\
    if (size <= capacity) {\
        lagus_bst_node_##E * buffer = bst -> memory_management.reallocate (bst -> buffer + 1, capacity * sizeof (lagus_bst_node_##E));\
        if (! buffer)\
            return lagus_memory_allocation;\
        bst -> capacity = capacity;\
        bst -> buffer = buffer - 1;\
    }\
    return lagus_success;\
}\
\
lagus_result lagus_bst_##E##_get_at (lagus_bst_##E * bst, uint64_t index, E * element) {\
    uint64_t node = lagus_root;\
    while (true) {\
        uint64_t left_child = bst -> buffer [node].left_child;\
        uint64_t size;\
        if (left_child)\
            size = bst -> buffer [left_child].size;\
        else\
            size = 0;\
        if (index == size + 1) {\
            * element = bst -> buffer [node].element;\
            return lagus_success;\
        }\
        if (index <= size)\
            node = left_child;\
        else {\
            index -= size + 1;\
            node = bst -> buffer [node].right_child;\
        }\
    }\
}\
\
lagus_result lagus_bst_##E##_set_at (lagus_bst_##E * bst, uint64_t index, E * element) {\
    uint64_t node = lagus_root;\
    while (true) {\
        uint64_t left_child = bst -> buffer [node].left_child;\
        uint64_t size;\
        if (left_child)\
            size = bst -> buffer [left_child].size;\
        else\
            size = 0;\
        if (index == size + 1) {\
            E temporal = bst -> buffer [node].element;\
            bst -> buffer [node].element = * element;\
            * element = temporal;\
            return lagus_success;\
        }\
        if (index <= size)\
            node = left_child;\
        else {\
            index -= size + 1;\
            node = bst -> buffer [node].right_child;\
        }\
    }\
}\
\
lagus_result lagus_bst_iterator_##E##_initialize (lagus_bst_iterator_##E * iterator, lagus_bst_##E * bst) {\
    if (bst -> height) {\
        * iterator = (lagus_bst_iterator_##E) {\
            .height = bst -> height,\
            .node = lagus_root,\
            .buffer = bst -> buffer\
        };\
        while (true) {\
            uint64_t child = bst -> buffer [iterator -> node].left_child;\
            if (! child)\
                break;\
            iterator -> height --;\
            iterator -> node = child;\
        }\
        iterator -> index = lagus_one << iterator -> height - 1;\
    } else\
        iterator -> node = lagus_none;\
    return lagus_success;\
}\
\
lagus_result lagus_bst_iterator_##E##_next (lagus_bst_iterator_##E * iterator, E * element) {\
    if (! iterator -> node)\
        return lagus_stop;\
    * element = iterator -> buffer [iterator -> node].element;\
    uint64_t child = iterator -> buffer [iterator -> node].right_child;\
    if (child) {\
        iterator -> height --;\
        iterator -> index += lagus_one << iterator -> height - 1;\
        while (true) {\
            iterator -> node = child;\
            child = iterator -> buffer [iterator -> node].left_child;\
            if (! child)\
                break;\
            iterator -> height --;\
            iterator -> index -= lagus_one << iterator -> height - 1;\
        }\
    } else {\
        iterator -> index += lagus_one << iterator -> height - 1;\
        __uint128_t index = iterator -> index;\
        uint64_t height = 1;\
        if (! (index & 0xFFFFFFFFFFFFFFFF)) {\
            index >>= 64;\
            height += 64;\
        }\
        if (! (index & 0x00000000FFFFFFFF)) {\
            index >>= 32;\
            height += 32;\
        }\
        if (! (index & 0x000000000000FFFF)) {\
            index >>= 16;\
            height += 16;\
        }\
        if (! (index & 0x00000000000000FF)) {\
            index >>= 8;\
            height += 8;\
        }\
        if (! (index & 0x0000000000000000F)) {\
            index >>= 4;\
            height += 4;\
        }\
        if (! (index & 0x00000000000000003)) {\
            index >>= 2;\
            height += 2;\
        }\
        if (! (index & 0x00000000000000001))\
            height += 1;\
        \
        while (true) {\
            iterator -> height ++;\
            iterator -> node = iterator -> buffer [iterator -> node].parent;\
            if (iterator -> height == height)\
                break;\
        }\
    }\
    return lagus_success;\
}\
\
lagus_result lagus_bst_iterator_##E##_finalize (lagus_bst_iterator_##E * iterator) {\
    return lagus_success;\
}

/* Trie */

// Declaration

# define lagus_declare_trie(E) \
\
typedef struct lagus_bit_stream_##E {\
    uint64_t (* size) (E);\
    uint8_t (* at) (E, uint64_t);\
} lagus_bit_stream_##E;\
\
typedef struct lagus_trie_node_##E {\
    uint64_t parent;\
    uint64_t left_child;\
    uint64_t right_child;\
    uint8_t active;\
    E element;\
} lagus_trie_node_##E;\
\
typedef struct lagus_trie_##E {\
    uint64_t size;\
    uint64_t nodes;\
    uint64_t capacity;\
    lagus_trie_node_##E * buffer;\
    lagus_bit_stream_##E bit_stream;\
    lagus_memory_management memory_management;\
} lagus_trie_##E;\
\
typedef struct lagus_trie_iterator_##E {\
    uint64_t index;\
    uint64_t size;\
    lagus_trie_node_##E * node;\
} lagus_trie_iterator_##E;\
\
uint64_t lagus_binary_bit_stream_##E##_size (E element);\
\
uint8_t lagus_binary_bit_stream_##E##_at (E element, uint64_t index);\
\
lagus_result lagus_trie_##E##_initialize (lagus_trie_##E * trie);\
\
lagus_result lagus_trie_##E##_custom_initialize (lagus_trie_##E * trie, uint64_t capacity, lagus_bit_stream_##E * bit_stream, lagus_memory_management * memory_management);\
\
lagus_result lagus_trie_##E##_finalize (lagus_trie_##E * trie);\
\
lagus_result lagus_trie_##E##_size (lagus_trie_##E * trie, uint64_t * size);\
\
lagus_result lagus_trie_##E##_add_unique (lagus_trie_##E * trie, E * element);\
\
lagus_result lagus_trie_##E##_add_replace (lagus_trie_##E * trie, E * element);\
\
lagus_result lagus_trie_##E##_search (lagus_trie_##E * trie, E * element);\
\
lagus_result lagus_trie_##E##_remove (lagus_trie_##E * trie, E * element);\
\
lagus_result lagus_trie_iterator_##E##_initialize (lagus_trie_iterator_##E * iterator, lagus_trie_##E * trie);\
\
lagus_result lagus_trie_iterator_##E##_next (lagus_trie_iterator_##E * iterator, E * element);\
\
lagus_result lagus_trie_iterator_##E##_finalize (lagus_trie_iterator_##E * iterator);

// Implementation

# define lagus_implement_trie(E) \
\
uint64_t lagus_binary_bit_stream_##E##_size (E element) {\
    uint8_t * bytes = (uint8_t *) & element;\
    uint64_t index = (sizeof (E) << 3) - 1;\
    while (true) {\
        if (bytes [index >> 3] >> (index & 7) & 1)\
            return index;\
        if (index == 0)\
            return sizeof (E) << 3;\
        index --;\
    }\
}\
\
uint8_t lagus_binary_bit_stream_##E##_at (E element, uint64_t index) {\
    uint8_t * bytes = (uint8_t *) & element;\
    index --;\
    return bytes [index >> 3] >> (index & 7) & 1;\
}\
\
lagus_result lagus_trie_##E##_initialize (lagus_trie_##E * trie) {\
    lagus_trie_node_##E * buffer = aligned_alloc (_Alignof (lagus_trie_node_##E), sizeof (lagus_trie_node_##E));\
    if (! buffer)\
        return lagus_memory_allocation;\
    * trie = (lagus_trie_##E) {\
        .size = 0,\
        .nodes = 1,\
        .capacity = 1,\
        .buffer = buffer - 1,\
        .bit_stream = {\
            .size = lagus_binary_bit_stream_##E##_size,\
            .at = lagus_binary_bit_stream_##E##_at\
        },\
        .memory_management = {\
            .allocate = aligned_alloc,\
            .reallocate = realloc,\
            .deallocate = free\
        }\
    };\
    * buffer = (lagus_trie_node_##E) {\
        .parent = lagus_none,\
        .left_child = lagus_none,\
        .right_child = lagus_none,\
        .active = false\
    };\
    return lagus_success;\
}\
\
lagus_result lagus_trie_##E##_custom_initialize (lagus_trie_##E * trie, uint64_t capacity, lagus_bit_stream_##E * bit_stream, lagus_memory_management * memory_management) {\
    lagus_trie_node_##E * buffer;\
    if (! capacity)\
        capacity = 1;\
    if (memory_management) {\
        buffer = memory_management -> allocate (_Alignof (lagus_trie_node_##E), capacity * sizeof (lagus_trie_node_##E));\
        if (! buffer)\
            return lagus_memory_allocation;\
        trie -> memory_management = * memory_management;\
    } else {\
        buffer = aligned_alloc (_Alignof (lagus_trie_node_##E), capacity * sizeof (lagus_trie_node_##E));\
        if (! buffer)\
            return lagus_memory_allocation;\
        trie -> memory_management = (lagus_memory_management) {\
            .allocate = aligned_alloc,\
            .reallocate = realloc,\
            .deallocate = free\
        };\
    }\
    trie -> size = 0;\
    trie -> nodes = 1;\
    trie -> capacity = capacity;\
    trie -> buffer = buffer - 1;\
    if (bit_stream)\
        trie -> bit_stream = * bit_stream;\
    else\
        trie -> bit_stream = (lagus_bit_stream_##E) {\
            .size = lagus_binary_bit_stream_##E##_size,\
            .at = lagus_binary_bit_stream_##E##_at\
        };\
    * buffer = (lagus_trie_node_##E) {\
        .parent = lagus_none,\
        .left_child = lagus_none,\
        .right_child = lagus_none,\
        .active = false\
    };\
    return lagus_success;\
}\
\
lagus_result lagus_trie_##E##_finalize (lagus_trie_##E * trie) {\
    trie -> memory_management.deallocate (trie -> buffer + 1);\
    return lagus_success;\
}\
\
lagus_result lagus_trie_##E##_size (lagus_trie_##E * trie, uint64_t * size) {\
    * size = trie -> size;\
    return lagus_success;\
}\
\
lagus_result lagus_trie_##E##_add_unique (lagus_trie_##E * trie, E * element) {\
    uint64_t child, parent, capacity;\
    lagus_trie_node_##E * buffer;\
    uint64_t node = lagus_root;\
    uint64_t size = trie -> bit_stream.size (* element);\
    uint64_t index = 1;\
    while (index <= size) {\
        uint8_t bit = trie -> bit_stream.at (* element, index);\
        if (bit)\
            child = trie -> buffer [node].left_child;\
        else\
            child = trie -> buffer [node].right_child;\
        if (child)\
            node = child;\
        else {\
            if (trie -> nodes == trie -> capacity) {\
                capacity = trie -> capacity << 1;\
                buffer = trie -> memory_management.reallocate (trie -> buffer + 1, capacity * sizeof (lagus_trie_node_##E));\
                if (! buffer) {\
                    while (! (node == lagus_root || trie -> buffer [node].left_child || trie -> buffer [node].right_child || trie -> buffer [node].active)) {\
                        parent = trie -> buffer [node].parent;\
                        if (parent == trie -> nodes) {\
                            trie -> buffer [node].active = trie -> buffer [parent].active;\
                            trie -> buffer [node].element = trie -> buffer [parent].element;\
                            if (trie -> buffer [parent].left_child == node) {\
                                child = trie -> buffer [parent].right_child;\
                                trie -> buffer [node].right_child = child;\
                            } else {\
                                child = trie -> buffer [parent].left_child;\
                                trie -> buffer [node].left_child = child;\
                            }\
                            if (child)\
                                trie -> buffer [child].parent = node;\
                            parent = trie -> buffer [parent].parent;\
                            trie -> buffer [node].parent = parent;\
                            if (trie -> buffer [parent].left_child == trie -> nodes)\
                                trie -> buffer [parent].left_child = node;\
                            else\
                                trie -> buffer [parent].right_child = node;\
                        } else {\
                            if (trie -> buffer [parent].left_child == node)\
                                trie -> buffer [parent].left_child = lagus_none;\
                            else\
                                trie -> buffer [parent].right_child = lagus_none;\
                            if (node != trie -> nodes) {\
                                trie -> buffer [node] = trie -> buffer [trie -> nodes];\
                                uint64_t parent = trie -> buffer [node].parent;\
                                if (trie -> buffer [parent].left_child == trie -> nodes)\
                                    trie -> buffer [parent].left_child = node;\
                                else\
                                    trie -> buffer [parent].right_child = node;\
                                child = trie -> buffer [node].left_child;\
                                if (child)\
                                    trie -> buffer [child].parent = node;\
                                child = trie -> buffer [node].right_child;\
                                if (child)\
                                    trie -> buffer [child].parent = node;\
                            }\
                            node = parent;\
                        }\
                        trie -> nodes --;\
                        if (trie -> capacity < 4)\
                            capacity = 1;\
                        else\
                            capacity = trie -> capacity >> 2;\
                        if (trie -> nodes <= capacity) {\
                            buffer = trie -> memory_management.reallocate (trie -> buffer + 1, capacity * sizeof (lagus_trie_node_##E));\
                            if (buffer) {\
                                trie -> capacity = capacity;\
                                trie -> buffer = buffer - 1;\
                            }\
                        }\
                    }\
                    return lagus_memory_allocation;\
                }\
                trie -> capacity = capacity;\
                trie -> buffer = buffer - 1;\
            }\
            trie -> nodes ++;\
            trie -> buffer [trie -> nodes] = (lagus_trie_node_##E) {\
                .parent = node,\
                .left_child = lagus_none,\
                .right_child = lagus_none,\
                .active = false\
            };\
            if (bit)\
                trie -> buffer [node].left_child = trie -> nodes;\
            else\
                trie -> buffer [node].right_child = trie -> nodes;\
            node = trie -> nodes;\
        }\
        index ++;\
    }\
    if (trie -> buffer [node].active) {\
        * element = trie -> buffer [node].element;\
        return lagus_already_contained;\
    }\
    trie -> buffer [node].active = true;\
    trie -> buffer [node].element = * element;\
    trie -> size ++;\
    return lagus_success;\
}\
\
lagus_result lagus_trie_##E##_add_replace (lagus_trie_##E * trie, E * element) {\
    uint64_t child, parent, capacity;\
    lagus_trie_node_##E * buffer;\
    uint64_t node = lagus_root;\
    uint64_t size = trie -> bit_stream.size (* element);\
    uint64_t index = 1;\
    while (index <= size) {\
        uint8_t bit = trie -> bit_stream.at (* element, index);\
        if (bit)\
            child = trie -> buffer [node].left_child;\
        else\
            child = trie -> buffer [node].right_child;\
        if (child)\
            node = child;\
        else {\
            if (trie -> nodes == trie -> capacity) {\
                capacity = trie -> capacity << 1;\
                buffer = trie -> memory_management.reallocate (trie -> buffer + 1, capacity * sizeof (lagus_trie_node_##E));\
                if (! buffer) {\
                    while (! (node == lagus_root || trie -> buffer [node].left_child || trie -> buffer [node].right_child || trie -> buffer [node].active)) {\
                        parent = trie -> buffer [node].parent;\
                        if (parent == trie -> nodes) {\
                            trie -> buffer [node].active = trie -> buffer [parent].active;\
                            trie -> buffer [node].element = trie -> buffer [parent].element;\
                            if (trie -> buffer [parent].left_child == node) {\
                                child = trie -> buffer [parent].right_child;\
                                trie -> buffer [node].right_child = child;\
                            } else {\
                                child = trie -> buffer [parent].left_child;\
                                trie -> buffer [node].left_child = child;\
                            }\
                            if (child)\
                                trie -> buffer [child].parent = node;\
                            parent = trie -> buffer [parent].parent;\
                            trie -> buffer [node].parent = parent;\
                            if (trie -> buffer [parent].left_child == trie -> nodes)\
                                trie -> buffer [parent].left_child = node;\
                            else\
                                trie -> buffer [parent].right_child = node;\
                        } else {\
                            if (trie -> buffer [parent].left_child == node)\
                                trie -> buffer [parent].left_child = lagus_none;\
                            else\
                                trie -> buffer [parent].right_child = lagus_none;\
                            if (node != trie -> nodes) {\
                                trie -> buffer [node] = trie -> buffer [trie -> nodes];\
                                uint64_t parent = trie -> buffer [node].parent;\
                                if (trie -> buffer [parent].left_child == trie -> nodes)\
                                    trie -> buffer [parent].left_child = node;\
                                else\
                                    trie -> buffer [parent].right_child = node;\
                                child = trie -> buffer [node].left_child;\
                                if (child)\
                                    trie -> buffer [child].parent = node;\
                                child = trie -> buffer [node].right_child;\
                                if (child)\
                                    trie -> buffer [child].parent = node;\
                            }\
                            node = parent;\
                        }\
                        trie -> nodes --;\
                        if (trie -> capacity < 4)\
                            capacity = 1;\
                        else\
                            capacity = trie -> capacity >> 2;\
                        if (trie -> nodes <= capacity) {\
                            buffer = trie -> memory_management.reallocate (trie -> buffer + 1, capacity * sizeof (lagus_trie_node_##E));\
                            if (buffer) {\
                                trie -> capacity = capacity;\
                                trie -> buffer = buffer - 1;\
                            }\
                        }\
                    }\
                    return lagus_memory_allocation;\
                }\
                trie -> capacity = capacity;\
                trie -> buffer = buffer - 1;\
            }\
            trie -> nodes ++;\
            trie -> buffer [trie -> nodes] = (lagus_trie_node_##E) {\
                .parent = node,\
                .left_child = lagus_none,\
                .right_child = lagus_none,\
                .active = false\
            };\
            if (bit)\
                trie -> buffer [node].left_child = trie -> nodes;\
            else\
                trie -> buffer [node].right_child = trie -> nodes;\
            node = trie -> nodes;\
        }\
        index ++;\
    }\
    if (trie -> buffer [node].active) {\
        E temporal = trie -> buffer [node].element;\
        trie -> buffer [node].element = * element;\
        * element = temporal;\
        return lagus_already_contained;\
    }\
    trie -> buffer [node].active = true;\
    trie -> buffer [node].element = * element;\
    trie -> size ++;\
    return lagus_success;\
}\
\
lagus_result lagus_trie_##E##_search (lagus_trie_##E * trie, E * element) {\
    uint64_t child;\
    uint64_t node = lagus_root;\
    uint64_t size = trie -> bit_stream.size (* element);\
    uint64_t index = 1;\
    while (index <= size) {\
        uint8_t bit = trie -> bit_stream.at (* element, index);\
        if (bit)\
            child = trie -> buffer [node].left_child;\
        else\
            child = trie -> buffer [node].right_child;\
        if (! child)\
            return lagus_not_contained;\
        node = child;\
        index ++;\
    }\
    if (! trie -> buffer [node].active)\
        return lagus_not_contained;\
    * element = trie -> buffer [node].element;\
    return lagus_success;\
}\
\
lagus_result lagus_trie_##E##_remove (lagus_trie_##E * trie, E * element) {\
    uint64_t child, parent, capacity;\
    lagus_trie_node_##E * buffer;\
    uint64_t node = lagus_root;\
    uint64_t size = trie -> bit_stream.size (* element);\
    uint64_t index = 1;\
    while (index <= size) {\
        uint8_t bit = trie -> bit_stream.at (* element, index);\
        if (bit)\
            child = trie -> buffer [node].left_child;\
        else\
            child = trie -> buffer [node].right_child;\
        if (! child)\
            return lagus_not_contained;\
        node = child;\
        index ++;\
    }\
    if (! trie -> buffer [node].active)\
        return lagus_not_contained;\
    * element = trie -> buffer [node].element;\
    trie -> buffer [node].active = false;\
    trie -> size --;\
    uint64_t result = lagus_success;\
    while (! (node == lagus_root || trie -> buffer [node].left_child || trie -> buffer [node].right_child || trie -> buffer [node].active)) {\
        parent = trie -> buffer [node].parent;\
        if (parent == trie -> nodes) {\
            trie -> buffer [node].active = trie -> buffer [parent].active;\
            trie -> buffer [node].element = trie -> buffer [parent].element;\
            if (trie -> buffer [parent].left_child == node) {\
                child = trie -> buffer [parent].right_child;\
                trie -> buffer [node].right_child = child;\
            } else {\
                child = trie -> buffer [parent].left_child;\
                trie -> buffer [node].left_child = child;\
            }\
            if (child)\
                trie -> buffer [child].parent = node;\
            parent = trie -> buffer [parent].parent;\
            trie -> buffer [node].parent = parent;\
            if (trie -> buffer [parent].left_child == trie -> nodes)\
                trie -> buffer [parent].left_child = node;\
            else\
                trie -> buffer [parent].right_child = node;\
        } else {\
            if (trie -> buffer [parent].left_child == node)\
                trie -> buffer [parent].left_child = lagus_none;\
            else\
                trie -> buffer [parent].right_child = lagus_none;\
            if (node != trie -> nodes) {\
                trie -> buffer [node] = trie -> buffer [trie -> nodes];\
                uint64_t parent = trie -> buffer [node].parent;\
                if (trie -> buffer [parent].left_child == trie -> nodes)\
                    trie -> buffer [parent].left_child = node;\
                else\
                    trie -> buffer [parent].right_child = node;\
                child = trie -> buffer [node].left_child;\
                if (child)\
                    trie -> buffer [child].parent = node;\
                child = trie -> buffer [node].right_child;\
                if (child)\
                    trie -> buffer [child].parent = node;\
            }\
            node = parent;\
        }\
        trie -> nodes --;\
        if (trie -> capacity < 4)\
            capacity = 1;\
        else\
            capacity = trie -> capacity >> 2;\
        if (trie -> nodes <= capacity) {\
            buffer = trie -> memory_management.reallocate (trie -> buffer + 1, capacity * sizeof (lagus_trie_node_##E));\
            if (buffer) {\
                trie -> capacity = capacity;\
                trie -> buffer = buffer - 1;\
            } else\
                result = lagus_memory_allocation;\
        }\
    }\
    return result;\
}\
\
lagus_result lagus_trie_iterator_##E##_initialize (lagus_trie_iterator_##E * iterator, lagus_trie_##E * trie) {\
    * iterator = (lagus_trie_iterator_##E) {\
        .index = 1,\
        .size = trie -> size,\
        .node = trie -> buffer\
    };\
    return lagus_success;\
}\
\
lagus_result lagus_trie_iterator_##E##_next (lagus_trie_iterator_##E * iterator, E * element) {\
    if (iterator -> index > iterator -> size)\
        return lagus_stop;\
    iterator -> node ++;\
    while (! iterator -> node -> active)\
        iterator -> node ++;\
    * element = iterator -> node -> element;\
    iterator -> index ++;\
    return lagus_success;\
}\
\
lagus_result lagus_trie_iterator_##E##_finalize (lagus_trie_iterator_##E * iterator) {\
    return lagus_success;\
}
