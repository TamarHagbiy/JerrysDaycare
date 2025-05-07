# JerrysDaycare
A C project simulating a daycare system for Jerries (from *Rick and Morty*), including data structures like linked lists and multi-value hash tables. Built for practicing modular programming, memory management, and abstract data types in C.
# README: JerryBoree System

---

## System Overview
The **JerryBoree** system is a memory-efficient, multi-layered data management application designed for storing, managing, and interacting with entities such as Jerries, planets, and their physical characteristics. The system emphasizes efficient memory management, data integrity, and quick data retrieval while maintaining logical consistency across all structures.

---

## System Architecture

1. **Planets Storage**:
   - **Planets** are stored in a fixed-size array.
   - Each planet has unique properties and can be referenced directly when associated with Jerries.

2. **Jerries Management**:
   - Each **Jerry** is stored exactly once in memory, ensuring no duplication.
   - Jerries are the central entities of the system, managed primarily through a hash table for efficient operations.

3. **Hash Table for Jerries**:
   - A **hash table** is the primary structure for managing Jerry objects, enabling fast lookup by their unique IDs.
   - This structure is also responsible for **memory deallocation**, ensuring that Jerries are properly freed only when removed from the hash table.
   - The size of the hash table is determined by finding the closest prime number to the estimated number of Jerries, ensuring an even distribution of entries for optimal performance.

4. **Multi-Value Hash Table for Characteristics**:
   - A **multi-value hash table** stores mappings from characteristic names to linked lists of Jerries that share the same characteristics.
   - **Deep copies** of characteristic keys are maintained to avoid issues when removing Jerries from the system.
   - Linked lists are used as the value for each key, providing efficient access to all Jerries associated with a specific characteristic.

5. **Linked List of Jerries**:
   - A **linked list** maintains the order of insertion of Jerries for sequential access.
   - This structure allows the system to print all Jerries in the order they were added, which is useful for debugging or displaying data.

---

## Key Features and Design Considerations

- **Deep vs. Shallow Copy**:
  - **Deep copies** are used for characteristic keys to prevent issues during Jerry removal.
  - Other structures use **shallow copies** where possible, ensuring that changes in one structure reflect across all related structures, minimizing redundancy.

- **Centralized Management**:
  - The **hash table** is the authoritative structure for managing Jerries, with most operations routed through it to ensure consistency and meet time complexity requirements.

- **Memory Management**:
  - Proper memory management ensures that no duplicate copies of Jerries are created.
  - The hash table is solely responsible for releasing memory allocated to Jerry objects, ensuring a single point of control for deallocation.

- **Optimized Hash Table Sizes**:
  - The sizes of hash tables are chosen as the closest prime numbers greater than the initial data estimates. This choice helps ensure a relatively uniform distribution of entries, improving lookup and insertion times.

- **Insertion Order Preservation**:
  - A linked list is maintained to preserve the order of insertion, allowing the system to print all Jerries in the order they were added.

---

## System Operations and Efficiency

The JerryBoree system is designed to support the following efficiently:
- **Adding Jerries and Planets**: Ensures proper memory allocation and consistency across all structures.
- **Querying by ID**: Directly supported through the hash table for constant-time lookups.
- **Querying by Characteristic**: Efficient retrieval through the multi-value hash table.
- **Updating Characteristics**: Reflects changes across all structures with minimal overhead.
- **Memory Cleanup**: Centralized deallocation via the hash table, ensuring no memory leaks.

---

## Notes for Developers
- Ensure deep copies for keys and proper shallow copying elsewhere to maintain consistency.
- Hash table sizes should always be optimized for the expected data size.
- The linked list is auxiliary but crucial for insertion order-related operations.
- Always route Jerry-related operations through the hash table for consistency and efficiency.
