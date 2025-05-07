//
// Created by tamar on 19/12/2024.
//

#ifndef LINKEDLIST_H
#define LINKEDLIST_H
#include "Defs.h"
/**
 * @file linkedlist.h
 * @brief Interface for a generic linked list.
 */

/** A type for a linked list handle. */
typedef struct List_h* linkedlist;

/**
 * @brief Creates a new linked list.
 * @param copy_func A function pointer for copying elements.
 * @param free_func A function pointer for freeing elements.
 * @param cmp_func A function pointer for comparing elements.
 * @param print_func A function pointer for printing elements.
 * @return A handle to the new linked list or NULL if the creation failed.
 */
linkedlist createLinkedList(CopyFunction copy_func, FreeFunction free_func, EqualFunction cmp_func, PrintFunction print_func);

/**
 * @brief Destroys the linked list and frees all allocated memory.
 * Frees the memory for each node and its data, as well as the linked list itself.
 * @param List The linked list to destroy.
 * @return Status of the operation (success or failure).
 */
status destroyList(linkedlist List);

/**
 * @brief Appends a new node with the given data to the linked list.
 * Adds the data to the end of the list by creating a new node.
 * @param List The linked list.
 * @param data The data to add. This data is copied into the list.
 * @return Status of the operation (success or failure).
 */
status appendNode(linkedlist List, Element data);

/**
 * @brief Deletes a node with the specified data from the linked list.
 * Removes the first node containing the specified data.
 * The data is compared using the comparison function provided during list creation.
 * @param List The linked list.
 * @param data The data of the node to delete.
 * @return Status of the operation (success or failure).
 */
status deleteNode(linkedlist List, Element data);

/**
 * @brief Prints the linked list.
 * Traverses the list and prints each element using the provided print function.
 * @param List The linked list to print.
 * @return Status of the operation (success or failure).
 */
status printList(linkedlist List);

/**
 * @brief Gets the data of a node by its index.
 * Traverses the list to find the node at the specified index and returns a copy of its data.
 * @param List The linked list.
 * @param index The index of the node (0-based).
 * @return A copy of the data of the node, or NULL if the index is invalid or the list is empty.
 */
Element getDataByIndex(linkedlist List, int index);

/**
 * @brief Gets the length of the linked list.
 * Returns the number of nodes currently in the list.
 * @param List The linked list.
 * @return The number of nodes in the linked list or failure if the list is invalid.
 */
int getLengthList(linkedlist List);

/**
 * @brief Searches for a node by key in the linked list.
 * Traverses the list and returns a copy of the data for the first node that matches the key.
 * Matching is determined using the comparison function provided during list creation.
 * @param List The linked list.
 * @param key The key to search for.
 * @return A copy of the matching element's data, or NULL if no match is found.
 */
Element searchByKeyInList(linkedlist List, Element key);

#endif //LINKEDLIST_H
