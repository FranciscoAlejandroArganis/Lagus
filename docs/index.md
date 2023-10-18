# Lagus

## Types

### `lagus_memory_management`

```c
typedef struct lagus_memory_management {
    void * (* allocate) (uint64_t, uint64_t);
    void * (* reallocate) (void *, uint64_t);
    void (* deallocate) (void *);
} lagus_memory_management;
```

#### Members

- `allocate` is a pointer to a function with the same semantics as that of `aligned_alloc` from `<stdlib.h>`.
- `reallocate` is a pointer to a function with the same semantics as that of `realloc` from `<stdlib.h>`.
- `deallocate` is a pointer to a function with the same semantics as that of `free` from `<stdlib.h>`.

#### Description

Structure with pointers to dynamic memory management functions.

---

### `lagus_result`

```c
typedef enum lagus_result {
    lagus_success,
    lagus_memory_allocation,
    lagus_not_contained,
    lagus_already_contained,
    lagus_stop
} lagus_result;
```

#### Members

- `lagus_success`: successful operation. 
- `lagus_memory_allocation`: memory allocation error.
- `lagus_not_contained`: a requested element is not contained in the data structure.
- `lagus_already_contained`: a requested element is already contained in the data structure.
- `lagus_stop`: iteration stop condition.

#### Description

Codes returned by lagus functions.
