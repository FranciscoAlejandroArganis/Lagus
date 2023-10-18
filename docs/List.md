# List

Linear sequence of elements.  
All deque operations have amortized `O(1)` time complexity.

---

## Macros

### `lagus_declare_list`

```c
lagus_declare_list (E)
```

#### Parameters

- `E` is the type of the list.

#### Description

Declares the required types and functions for a list of type `E`.

!!! info

	- `E` **must** be an assignable type and a single keyword or identifier.

---

### `lagus_implement_list`

```c
lagus_implement_list (E)
```

#### Parameters

- `E` is the type of the list.

#### Description

Implements all functions declared by `lagus_implement_list (E)`.

!!! info

	- `E` **must** be an assignable type and a single keyword or identifier.
	- `lagus_implement_list (E)` **must** be expanded after `lagus_declare_list (E)`.

---

## Types

### `lagus_list_E`

```c
typedef struct lagus_list_E lagus_list_E;
```

#### Description

A list of type `E`.

---

### `lagus_list_iterator_E`

```c
typedef struct lagus_list_iterator_E lagus_list_iterator_E;
```

#### Description

A list iterator of type `E`.

---

## Initialize

### `lagus_list_E_initialize`

```c
lagus_result lagus_list_E_initialize (
	lagus_list_E * list
);
```

#### Parameters

- `list` is a pointer to the list.

#### Results

- `lagus_success` if the list was initialized.
- `lagus_memory_allocation` if memory cannot be allocated to initialize the list.

#### Description

Initializes `list` to a new empty list with default capacity and memory management.

!!! info

	- `list` **must** be a valid `lagus_list_E` pointer.

---

### `lagus_list_E_custom_initialize`

```c
lagus_result lagus_list_E_custom_initialize (
	lagus_list_E * list,
	uint64_t capacity,
	lagus_memory_management * memory_management
);
```

#### Parameters

- `list` is a pointer to the list.
- `capacity` is the initial capacity.
- `memory_management` is a pointer to an `lagus_memory_management` struct with the callbacks the list will use to manage dynamic memory.

#### Results

- `lagus_success` if the list was initialized.
- `lagus_memory_allocation` if memory cannot be allocated to initialize the list.

#### Description

Initializes `list` to a new empty list with the specified capacity and memory management.

!!! info

	- `list` **must** be a valid `lagus_list_E` pointer.
	- If `capacity` is `0`, the default capacity is used.
	- `memory_management` **must** be a valid `lagus_memory_management` pointer or `null`. If `null`, the default memory management is used.

---

## Finalize

### `lagus_list_E_finalize`

```c
lagus_result lagus_bst_E_finalize (
	lagus_list_E * list
);
```

#### Parameters

- `list` is a pointer to the list.

#### Results

- `lagus_success` if the list was finalized.

#### Description

Frees all memory allocated by `list`.

!!! info

	- `list` **must** be initialized.

---

## Operations

### `lagus_list_E_size`

```c
lagus_result lagus_list_E_size (
	lagus_list_E * list,
	uint64_t * size
);
```

#### Parameters

- `list` is a pointer to the list.
- `size` is a pointer in which the size is returned.

#### Results

- `lagus_success` if the size of the list was retured.

#### Description

Returns in `size` the number of elements in `list`.

!!! info

	- `list` **must** be initialized.
	- `size` **must** be a valid `uint64_t` pointer.

---

### `lagus_list_E_add_first`

```c
lagus_result lagus_list_E_add_first (
	lagus_list_E * list,
	E element
);
```

#### Parameters

- `list` is a pointer to the list.
- `element` is the element to insert.

#### Results

- `lagus_success` if the element was inserted.
- `lagus_memory_allocation` if memory cannot be allocated to insert the element.

#### Description

Inserts `element` into `list`.  
`element` is inserted before all other elements of the list.

!!! info

	- `list` **must** be initialized.

---

### `lagus_list_E_add_last`

```c
lagus_result lagus_list_E_add_last (
	lagus_list_E * list,
	E element
);
```

#### Parameters

- `list` is a pointer to the list.
- `element` is the element to insert.

#### Results

- `lagus_success` if the element was inserted.
- `lagus_memory_allocation` if memory cannot be allocated to insert the element.

#### Description

Inserts `element` into `list`.  
`element` is inserted after all other elements of the list.

!!! info

	- `list` **must** be initialized.

---

### `lagus_list_E_add_at`

```c
lagus_result lagus_list_E_add_at (
	lagus_list_E * list,
	uint64_t index,
	E element
);
```

#### Parameters

- `list` is a pointer to the list.
- `index` is the insertion position.
- `element` is the element to insert.

#### Results

- `lagus_success` if the element was inserted.
- `lagus_memory_allocation` if memory cannot be allocated to insert the element.

#### Description

Inserts `element` into `list`.  
`element` is inserted at `index`.

!!! info

	- `list` **must** be initialized.
	- `index` **must** satisfy `1 ≤ index ≤ size + 1` where `size` is the number of elements in the list.

---

### `lagus_list_E_remove_first`

```c
lagus_result lagus_list_E_remove_first (
	lagus_list_E * list,
	E * element
);
```

#### Parameters

- `list` is a pointer to the list.
- `element` is a pointer in which the removed element is returned.

#### Results

- `lagus_success` if the element was removed.
- `lagus_memory_allocation` if the element was removed but memory reallocation failed.

#### Description

Removes the first element from `list`.  
Returns in `element` the removed element.

!!! info

	- `list` **must** be initialized.
	- `element` **must** be a valid `E` pointer.

---

### `lagus_list_E_remove_last`

```c
lagus_result lagus_list_E_remove_last (
	lagus_list_E * list,
	E * element
);
```

#### Parameters

- `list` is a pointer to the list.
- `element` is a pointer in which the removed element is returned.

#### Results

- `lagus_success` if the element was removed.
- `lagus_memory_allocation` if the element was removed but memory reallocation failed.

#### Description

Removes the last element from `list`.  
Returns in `element` the removed element.

!!! info

	- `list` **must** be initialized.
	- `element` **must** be a valid `E` pointer.

---

### `lagus_list_E_remove_at`

```c
lagus_result lagus_list_E_remove_at (
	lagus_list_E * list,
	uint64_t index,
	E * element
);
```

#### Parameters

- `list` is a pointer to the list.
- `index` is the deletion position.
- `element` is a pointer in which the removed element is returned.

#### Results

- `lagus_success` if the element was removed.
- `lagus_memory_allocation` if the element was removed but memory reallocation failed.

#### Description

Removes the element at position `index` from `list`.  
Returns in `element` the removed element.

!!! info

	- `list` **must** be initialized.
	- `index` **must** satisfy `1 ≤ index ≤ size` where `size` is the number of elements in the list.
	- `element` **must** be a valid `E` pointer.

---

### `lagus_list_E_get_first`

```c
lagus_result lagus_list_E_get_first (
	lagus_list_E * list,
	E * element
);
```

#### Parameters

- `list` is a pointer to the list.
- `element` is a pointer in which the element is returned.

#### Results

- `lagus_success` if the element was returned.

#### Description

Returns in `element` the first element of `list`.

!!! info

	- `list` **must** be initialized.
	- `element` **must** be a valid `E` pointer.

---

### `lagus_list_E_get_last`

```c
lagus_result lagus_list_E_get_last (
	lagus_list_E * list,
	E * element
);
```

#### Parameters

- `list` is a pointer to the list.
- `element` is a pointer in which the element is returned.

#### Results

- `lagus_success` if the element was returned.

#### Description

Returns in `element` the last element of `list`.

!!! info

	- `list` **must** be initialized.
	- `element` **must** be a valid `E` pointer.

---

### `lagus_list_E_get_at`

```c
lagus_result lagus_list_E_get_at (
	lagus_list_E * list,
	uint64_t index,
	E * element
);
```

#### Parameters

- `list` is a pointer to the list.
- `index` is the position of the element.
- `element` is a pointer in which the element is returned.

#### Results

- `lagus_success` if the element was returned.

#### Description

Returns in `element` the element of `list` at position `index`.

!!! info

	- `list` **must** be initialized.
	- `index` **must** satisfy `1 ≤ index ≤ size` where `size` is the number of elements in the list.
	- `element` **must** be a valid `E` pointer.

---

### `lagus_list_E_set_first`

```c
lagus_result lagus_list_E_set_first (
	lagus_list_E * list,
	E * element
);
```

#### Parameters

- `list` is a pointer to the list.
- `element` is a pointer to the element.

#### Results

- `lagus_success` if the element was set.

#### Description

Sets the first element of `list` to the value of `element`.  
Returns in `element` the previous first element of the list.

!!! info

	- `list` **must** be initialized.
	- `element` **must** be a valid `E` pointer.

---

### `lagus_list_E_set_last`

```c
lagus_result lagus_list_E_set_last (
	lagus_list_E * list,
	E * element
);
```

#### Parameters

- `list` is a pointer to the list.
- `element` is a pointer to the element.

#### Results

- `lagus_success` if the element was set.

#### Description

Sets the last element of `list` to the value of `element`.  
Returns in `element` the previous last element of the list.

!!! info

	- `list` **must** be initialized.
	- `element` **must** be a valid `E` pointer.

---

### `lagus_list_E_set_at`

```c
lagus_result lagus_list_E_get_at (
	lagus_list_E * list,
	uint64_t index,
	E * element
);
```

#### Parameters

- `list` is a pointer to the list.
- `index` is the position of the element.
- `element` is a pointer to the element.

#### Results

- `lagus_success` if the element was set.

#### Description

Sets the element of `list` at position `index` to the value of `element`.  
Returns in `element` the previous element of the list at `index`.

!!! info

	- `list` **must** be initialized.
	- `index` **must** satisfy `1 ≤ index ≤ size` where `size` is the number of elements in the list.
	- `element` **must** be a valid `E` pointer.

---

## Iteration

### `lagus_list_iterator_E_initialize`

```c
lagus_result lagus_list_iterator_E_initialize (
	lagus_list_iterator_E * iterator,
	lagus_list_E * list
);
```

#### Parameters

- `iterator` is a pointer to the iterator.
- `list` is a pointer to the list to iterate.

#### Results

- `lagus_success` if the iterator was initialized.

#### Description

Initializes `ìterator` to iterate `list`.

!!! info

	- `iterator` **must** be a valid `lagus_list_iterator_E` pointer.
	- `list` **must** be initialized.

---

### `lagus_list_iterator_E_next`

```c
lagus_result lagus_list_iterator_E_next (
	lagus_list_iterator_E * iterator,
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

### `lagus_list_iterator_E_finalize`

```c
lagus_result lagus_list_iterator_E_finalize (
	lagus_list_iterator_E * iterator
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
	No `lagus_list_iterator_E` operation allocates any memory.  
	This function does nothing.

---
