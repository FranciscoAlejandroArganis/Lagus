# Trie

Bitwise trie.  
The time complexity of all operations is independent of the number of elements in the trie.

---

## Macros

### `lagus_declare_trie`

```c
lagus_declare_trie (E)
```

#### Parameters

- `E` is the type of the trie.

#### Description

Declares the required types and functions for a trie of type `E`.

!!! info

	- `E` **must** be an assignable type and a single keyword or identifier.

---

### `lagus_implement_trie`

```c
lagus_implement_trie (E)
```

#### Parameters

- `E` is the type of the trie.

#### Description

Implements all functions declared by `lagus_implement_trie (E)`.

!!! info

	- `E` **must** be an assignable type and a single keyword or identifier.
	- `lagus_implement_trie (E)` **must** be expanded after `lagus_declare_trie (E)`.

---

## Types

### `lagus_bit_stream_E`

```c
typedef struct lagus_bit_stream_E {
    uint64_t (* size) (E);
    uint8_t (* at) (E, uint64_t);
} lagus_bit_stream_E;
```

#### Members

- `size` is a pointer to a function that returns the number of bits in the bit stream of an element.
- `at` is a pointer to a function that returns the bit at an specified position from the bit stream of an element.

#### Description

Structure with pointers to functions that define the bit stream of an element of type `E`.

---

### `lagus_trie_E`

```c
typedef struct lagus_trie_E lagus_trie_E;
```

#### Description

A trie of type `E`.

---

### `lagus_trie_iterator_E`

```c
typedef struct lagus_trie_iterator_E lagus_trie_iterator_E;
```

#### Description

A trie iterator of type `E`.

---

## Initialize

### `lagus_trie_E_initialize`

```c
lagus_result lagus_trie_E_initialize (
	lagus_trie_E * trie
);
```

#### Parameters

- `trie` is a pointer to the trie.

#### Results

- `lagus_success` if the trie was initialized.
- `lagus_memory_allocation` if memory cannot be allocated to initialize the trie.

#### Description

Initializes `trie` to a new empty trie with default capacity, bit stream and memory management.

!!! info

	- `trie` **must** be a valid `lagus_trie_E` pointer.

---

### `lagus_trie_E_custom_initialize`

```c
lagus_result lagus_trie_E_custom_initialize (
	lagus_trie_E * trie,
	uint64_t capacity,
	lagus_bit_stream_E * bit_stream,
	lagus_memory_management * memory_management
);
```

#### Parameters

- `trie` is a pointer to the trie.
- `capacity` is the initial capacity.
- `bit_stream` is a pointer to an `lagus_bit_stream_E` struct with the callbacks the trie will use to consume a bit stream.
- `memory_management` is a pointer to an `lagus_memory_management` struct with the callbacks the trie will use to manage dynamic memory.

#### Results

- `lagus_success` if the trie was initialized.
- `lagus_memory_allocation` if memory cannot be allocated to initialize the trie.

#### Description

Initializes `trie` to a new empty trie with the specified capacity, bit stream and memory management.

!!! info

	- `trie` **must** be a valid `lagus_trie_E` pointer.
	- If `capacity` is `0`, the default capacity is used.
	- `bit stream` **must** be a valid `lagus_bit_stream_E` pointer or `null`. If `null`, the default bit stream is used.
	- `memory_management` **must** be a valid `lagus_memory_management` pointer or `null`. If `null`, the default memory management is used.

---

## Finalize

### `lagus_trie_E_finalize`

```c
lagus_result lagus_bst_E_finalize (
	lagus_trie_E * trie
);
```

#### Parameters

- `trie` is a pointer to the trie.

#### Results

- `lagus_success` if the trie was finalized.

#### Description

Frees all memory allocated by `trie`.

!!! info

	- `trie` **must** be initialized.

---

## Operations

### `lagus_trie_E_size`

```c
lagus_result lagus_trie_E_size (
	lagus_trie_E * trie,
	uint64_t * size
);
```

#### Parameters

- `trie` is a pointer to the trie.
- `size` is a pointer in which the size is returned.

#### Results

- `lagus_success` if the size of the trie was retured.

#### Description

Returns in `size` the number of elements in `trie`.

!!! info

	- `trie` **must** be initialized.
	- `size` **must** be a valid `uint64_t` pointer.

---

### `lagus_trie_E_add_unique`

```c
lagus_result lagus_trie_E_add_unique (
	lagus_trie_E * trie,
	E * element
);
```

#### Parameters

- `trie` is a pointer to the trie.
- `element` is a pointer to the element.

#### Results

- `lagus_success` if the element was inserted.
- `lagus_memory_allocation` if memory cannot be allocated to insert the element.
- `lagus_already_contained` if the element was not inserted due to another element in the trie having the same bit stream.

#### Description

Inserts the value of `element` into `trie`.  
If the value of `element` has the same bit stream as another element in the trie, no changes to the trie are made and the other such element is returned in `element`.

!!! info

	- `trie` **must** be initialized.
	- `element` **must** be a valid `E` pointer.

---

### `lagus_trie_E_add_replace`

```c
lagus_result lagus_trie_E_add_replace (
	lagus_trie_E * trie,
	E * element
);
```

#### Parameters

- `trie` is a pointer to the trie.
- `element` is a pointer to the element.

#### Results

- `lagus_success` if the element was inserted.
- `lagus_memory_allocation` if memory cannot be allocated to insert the element.
- `lagus_already_contained` if the element replaced another element in the trie having the same bit stream.

#### Description

Inserts the value of `element` into `trie`.  
If the value of `element` has the same bit stream as another element in the trie, the other such element is replaced in the trie and returned in `element`.

!!! info

	- `trie` **must** be initialized.
	- `element` **must** be a valid `E` pointer.

---

### `lagus_trie_E_search`

```c
lagus_result lagus_trie_E_search (
	lagus_trie_E * trie,
	E * element
);
```

#### Parameters

- `trie` is a pointer to the trie.
- `element` is a pointer to the element.

#### Results

- `lagus_success` if the element was found.
- `lagus_not_contained` if the element does not have the same bit stream as any other element in the trie.

#### Description

Searchs the value of `element` in `bst`.  
Returns in `element` the element found.

!!! info

	- `trie` **must** be initialized.
	- `element` **must** be a valid `E` pointer.

---

### `lagus_trie_E_remove`

```c
lagus_result lagus_trie_E_remove (
	lagus_trie_E * trie,
	E * element
);
```

#### Parameters

- `trie` is a pointer to the trie.
- `element` is a pointer to the element.

#### Results

- `lagus_success` if the element was removed.
- `lagus_memory_allocation` if the element was removed but memory reallocation failed.
- `lagus_not_contained` if the element does not have the same bit stream as any other element in the trie.

#### Description

Removes the value of `element` from `trie`.  
Returns in `element` the removed element.

!!! info

	- `trie` **must** be initialized.
	- `element` **must** be a valid `E` pointer.

---

## Iteration

### `lagus_trie_iterator_E_initialize`

```c
lagus_result lagus_trie_iterator_E_initialize (
	lagus_trie_iterator_E * iterator,
	lagus_trie_E * trie
);
```

#### Parameters

- `iterator` is a pointer to the iterator.
- `trie` is a pointer to the trie to iterate.

#### Results

- `lagus_success` if the iterator was initialized.

#### Description

Initializes `ìterator` to iterate `trie`.

!!! info

	- `iterator` **must** be a valid `lagus_trie_iterator_E` pointer.
	- `trie` **must** be initialized.

---

### `lagus_trie_iterator_E_next`

```c
lagus_result lagus_trie_iterator_E_next (
	lagus_trie_iterator_E * iterator,
	E * element
);
```

#### Parameters

- `iterator` is a pointer to the iterator.
- `element` is a pointer in which the element is returned.

#### Results

- `lagus_success` if the next element was returned.
- `lagus_stop` if the iteration has no more elements.

#### Description

Returns in `element` the next element in the iteration.

!!! info

	- `iterator` **must** be initialized.
	- `element` **must** be a valid `E` pointer.

---

### `lagus_trie_iterator_E_finalize`

```c
lagus_result lagus_trie_iterator_E_finalize (
	lagus_trie_iterator_E * iterator
);
```

#### Parameters

- `iterator` is a pointer to the iterator.

#### Results

- `lagus_success` if the iterator was finalized.

#### Description

Frees all memory allocated by `iterator`.

!!! info

	- `iterator` **must** be initialized.

!!! note

	**Included for completeness.**  
	No `lagus_trie_iterator_E` operation allocates any memory.  
	This function does nothing.

---
