# Binary Search Tree

Balanced binary search tree.  
All operations have `O(logn)` time complexity, where `n` is the number of elements in the binary search tree.

---

## Macros

### `lagus_declare_bst`

```c
lagus_declare_bst (E)
```

#### Parameters

- `E` is the type of the binary search tree.

#### Description

Declares the required types and functions for a binary search tree of type `E`.

!!! info

	- `E` **must** be an assignable type and a single keyword or identifier.

---

### `lagus_implement_bst`

```c
lagus_implement_bst (E)
```

#### Parameters

- `E` is the type of the binary search tree.

#### Description

Implements all functions declared by `lagus_implement_bst (E)`.

!!! info

    - `E` **must** be an assignable type and a single keyword or identifier.
    - `lagus_implement_bst (E)` **must** be expanded after `lagus_declare_bst (E)`.

---

## Types



### `lagus_compare_E`

```c
typedef int64_t (* lagus_compare_E) (E, E);
```

#### Description

A pointer to a compare function of type `E`.  
Values returned by the function are interpreted as follows:

- Negative integer: the first argument is less than the second argument.
- Zero: the first argument is equal to the second argument.
- Positive integer: the first argument is greater than the second argument.

---

### `lagus_bst_E`

```c
typedef struct lagus_bst_E lagus_bst_E;
```

#### Description

A binary search tree of type `E`.

---

### `lagus_bst_iterator_E`

```c
typedef struct lagus_bst_iterator_E lagus_bst_iterator_E;
```

#### Description

A binary search tree iterator of type `E`.

---

## Initialize

### `lagus_bst_E_initialize`

```c
lagus_result lagus_bst_E_initialize (
	lagus_bst_E * bst
);
```

#### Parameters

- `bst` is a pointer to the binary search tree.

#### Results

- `lagus_success` if the binary search tree was initialized.
- `lagus_memory_allocation` if memory cannot be allocated to initialize the binary search tree.

#### Description

Initializes `bst` to a new empty binary search tree with default capacity, compare function and memory management.

!!! info

	- `bst` **must** be a valid `lagus_bst_E` pointer.

---

### `lagus_bst_E_custom_initialize`

```c
lagus_result lagus_bst_E_custom_initialize (
	lagus_bst_E * bst,
	uint64_t capacity,
	lagus_compare_E compare,
	lagus_memory_management * memory_management
);
```

#### Parameters

- `bst` is a pointer to the binary search tree.
- `capacity` is the initial capacity.
- `compare` is a pointer to the compare function the binary search tree will use to compare elements.
- `memory_management` is a pointer to an `lagus_memory_management` struct with the callbacks the binary search tree will use to manage dynamic memory.

#### Results

- `lagus_success` if the binary search tree was initialized.
- `lagus_memory_allocation` if memory cannot be allocated to initialize the binary search tree.

#### Description

Initializes `bst` to a new empty binary search tree with the specified capacity, compare function and memory management.

!!! info

	- `bst` **must** be a valid `lagus_bst_E` pointer.
	- If `capacity` is `0`, the default capacity is used.
	- `compare` **must** be a valid `lagus_compare_E` function or `null`. If `null`, the default compare function is used.
	- `memory_management` **must** be a valid `lagus_memory_management` pointer or `null`. If `null`, the default memory management is used.

---

## Finalize

### `lagus_bst_E_finalize`

```c
lagus_result lagus_bst_E_finalize (
	lagus_bst_E * bst
);
```

#### Parameters

- `bst` is a pointer to the binary search tree.

#### Results

- `lagus_success` if the binary search tree was finalized.

#### Description

Frees all memory allocated by `bst`.

!!! info

	- `bst` **must** be initialized.

---

## Operations

### `lagus_bst_E_size`

```c
lagus_result lagus_bst_E_size (
	lagus_bst_E * bst,
	uint64_t * size
);
```

#### Parameters

- `bst` is a pointer to the binary search tree.
- `size` is a pointer in which the size is returned.

#### Results

- `lagus_success` if the size of the binary search tree was retured.

#### Description

Returns in `size` the number of elements in `bst`.

!!! info

	- `bst` **must** be initialized.
	- `size` **must** be a valid `uint64_t` pointer.

---

### `lagus_bst_E_add_left`

```c
lagus_result lagus_bst_E_add_left (
	lagus_bst_E * bst,
	E element
);
```

#### Parameters

- `bst` is a pointer to the binary search tree.
- `element` is the element to insert.

#### Results

- `lagus_success` if the element was inserted.
- `lagus_memory_allocation` if memory cannot be allocated to insert the element.

#### Description

Inserts `element` into `bst`.  
`element` is inserted at its sorted position.  
If `element` is compared as equal to other elements in `bst`, it is inserted before all such elements.

!!! info

	- `bst` **must** be initialized and sorted.

---

### `lagus_bst_E_add_right`

```c
lagus_result lagus_bst_E_add_right (
	lagus_bst_E * bst,
	E element
);
```

#### Parameters

- `bst` is a pointer to the binary search tree.
- `element` is the element to insert.

#### Results

- `lagus_success` if the element was inserted.
- `lagus_memory_allocation` if memory cannot be allocated to insert the element.

#### Description

Inserts `element` into `bst`.  
`element` is inserted at its sorted position.  
If `element` is compared as equal to other elements in `bst`, it is inserted after all such elements.

!!! info

	- `bst` **must** be initialized and sorted.

---

### `lagus_bst_E_add_at`

```c
lagus_result lagus_bst_E_add_at (
	lagus_bst_E * bst,
	uint64_t index,
	E element
);
```

#### Parameters

- `bst` is a pointer to the binary search tree.
- `index` is the insertion position.
- `element` is the element to insert.

#### Results

- `lagus_success` if the element was inserted.
- `lagus_memory_allocation` if memory cannot be allocated to insert the element.

#### Description

Inserts `element` into `bst`.  
`element` is inserted at `index`.

!!! info

	- `bst` **must** be initialized.
	- `index` **must** satisfy `1 ≤ index ≤ size + 1` where `size` is the number of elements in the binary search tree.

---

### `lagus_bst_E_search_left`

```c
lagus_result lagus_bst_E_search_left (
	lagus_bst_E * bst,
	E * element,
	uint64_t * index
);
```

#### Parameters

- `bst` is a pointer to the binary search tree.
- `element` is a pointer to the element.
- `index` is a pointer in which the index is returned.

#### Results

- `lagus_success` if the element was found.
- `lagus_not_contained` if the element is not compared as equal to any other element in the binary search tree.

#### Description

Searchs the value of `element` in `bst`.  
Returns in `element` the element found.  
Returns in `index` the position of the element found.  
If the value of `element` is compared as equal to other elements in `bst`, the first of all such elements is returned.

!!! info

	- `bst` **must** be initialized and sorted.
	- `element` **must** be a valid `E` pointer.
	- `index` **must** be a valid `uint64_t` pointer.

---

### `lagus_bst_E_search_right`

```c
lagus_result lagus_bst_E_search_right (
	lagus_bst_E * bst,
	E * element,
	uint64_t * index
);
```

#### Parameters

- `bst` is a pointer to the binary search tree.
- `element` is a pointer to the element.
- `index` is a pointer in which the index is returned.

#### Results

- `lagus_success` if the element was found.
- `lagus_not_contained` if the element is not compared as equal to any other element in the binary search tree.

#### Description

Searchs the value of `element` in `bst`.  
Returns in `element` the element found.  
Returns in `index` the position of the element found.  
If the value of `element` is compared as equal to other elements in `bst`, the last of all such elements is returned.

!!! info

	- `bst` **must** be initialized and sorted.
	- `element` **must** be a valid `E` pointer.
	- `index` **must** be a valid `uint64_t` pointer.

---

### `lagus_bst_E_remove_left`

```c
lagus_result lagus_bst_E_remove_left (
	lagus_bst_E * bst,
	E * element
);
```

#### Parameters

- `bst` is a pointer to the binary search tree.
- `element` is a pointer to the element.

#### Results

- `lagus_success` if the element was removed.
- `lagus_memory_allocation` if the element was removed but memory reallocation failed.
- `lagus_not_contained` if the element is not compared as equal to any other element in the binary search tree.

#### Description

Removes the value of `element` from `bst`.  
Returns in `element` the removed element.  
If the value of `element` is compared as equal to other elements in `bst`, the first of all such elements is removed.

!!! info

	- `bst` **must** be initialized and sorted.
	- `element` **must** be a valid `E` pointer.

---

### `lagus_bst_E_remove_right`

```c
lagus_result lagus_bst_E_remove_right (
	lagus_bst_E * bst,
	E * element
);
```

#### Parameters

- `bst` is a pointer to the binary search tree.
- `element` is a pointer to the element.

#### Results

- `lagus_success` if the element was removed.
- `lagus_memory_allocation` if the element was removed but memory reallocation failed.
- `lagus_not_contained` if the element is not compared as equal to any other element in the binary search tree.

#### Description

Removes the value of `element` from `bst`.  
Returns in `element` the removed element.  
If the value of `element` is compared as equal to other elements in `bst`, the last of all such elements is removed.

!!! info

	- `bst` **must** be initialized and sorted.
	- `element` **must** be a valid `E` pointer.

---

### `lagus_bst_E_remove_at`

```c
lagus_result lagus_bst_E_remove_at (
	lagus_bst_E * bst,
	uint64_t index,
	E * element
);
```

#### Parameters

- `bst` is a pointer to the binary search tree.
- `index` is the deletion position.
- `element` is a pointer in which the removed element is returned.

#### Results

- `lagus_success` if the element was removed.
- `lagus_memory_allocation` if the element was removed but memory reallocation failed.

#### Description

Removes the element at position `index` from `bst`.  
Returns in `element` the removed element.

!!! info

	- `bst` **must** be initialized.
	- `index` **must** satisfy `1 ≤ index ≤ size` where `size` is the number of elements in the binary search tree.
	- `element` **must** be a valid `E` pointer.

---

### `lagus_bst_E_get_at`

```c
lagus_result lagus_bst_E_get_at (
	lagus_bst_E * bst,
	uint64_t index,
	E * element
);
```

#### Parameters

- `bst` is a pointer to the binary search tree.
- `index` is the position of the element.
- `element` is a pointer in which the element is returned.

#### Results

- `lagus_success` if the element was returned.

#### Description

Returns in `element` the element of `bst` at position `index`.

!!! info

	- `bst` **must** be initialized.
	- `index` **must** satisfy `1 ≤ index ≤ size` where `size` is the number of elements in the binary search tree.
	- `element` **must** be a valid `E` pointer.

---

### `lagus_bst_E_set_at`

```c
lagus_result lagus_bst_E_get_at (
	lagus_bst_E * bst,
	uint64_t index,
	E * element
);
```

#### Parameters

- `bst` is a pointer to the binary search tree.
- `index` is the position of the element.
- `element` is a pointer to the element.

#### Results

- `lagus_success` if the element was set.

#### Description

Sets the element of `bst` at position `index` to the value of `element`.  
Returns in `element` the previous element of the binary search tree at `index`.

!!! info

	- `bst` **must** be initialized.
	- `index` **must** satisfy `1 ≤ index ≤ size` where `size` is the number of elements in the binary search tree.
	- `element` **must** be a valid `E` pointer.

---

## Iteration

### `lagus_bst_iterator_E_initialize`

```c
lagus_result lagus_bst_iterator_E_initialize (
	lagus_bst_iterator_E * iterator,
	lagus_bst_E * bst
);
```

#### Parameters

- `iterator` is a pointer to the iterator.
- `bst` is a pointer to the binary search tree to iterate.

#### Results

- `lagus_success` if the iterator was initialized.

#### Description

Initializes `ìterator` to iterate `bst`.

!!! info

	- `iterator` **must** be a valid `lagus_bst_iterator_E` pointer.
	- `bst` **must** be initialized.

---

### `lagus_bst_iterator_E_next`

```c
lagus_result lagus_bst_iterator_E_next (
	lagus_bst_iterator_E * iterator,
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

### `lagus_bst_iterator_E_finalize`

```c
lagus_result lagus_bst_iterator_E_finalize (
	lagus_bst_iterator_E * iterator
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
	No `lagus_bst_iterator_E` operation allocates any memory.  
	This function does nothing.

---
