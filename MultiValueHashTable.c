//
// Created by tamar on 20/12/2024.
//
#include "HashTable.h"
#include "LinkedList.h"
#include "MultiValueHashTable.h"
#include "Defs.h"

// Structure definition for MultiValueHashTable
typedef struct multihashTable_s {
    hashTable table; ///< The hash table used to store keys and lists of values
    PrintFunction printValue; ///< Function to print values in the lists
    PrintFunction printKey;
    FreeFunction freeValue; ///< Function to free values in the lists
    CopyFunction copyValue; ///< Function to copy values in the lists
    EqualFunction equalValue; ///< Function to compare values in the lists
} MultiValueHashTable;

// Helper function to copy a linked list
Element copylist(Element list) {
    linkedlist newlist = (linkedlist)list;
    return newlist;
}

// Helper function to destroy a linked list
status destroyList1(Element list) {
    if (list == NULL) {
        return failure; // Check if the list is NULL
    }
    linkedlist newlist = (linkedlist)list;
    return destroyList(newlist); // Destroy the list
}

// Helper function to print a linked list
status printList1(Element list) {
    if (list == NULL) {
        return failure; // Check if the list is NULL
    }
    linkedlist newlist = (linkedlist)list;
    return printList(newlist); // Print the list
}

// Create a MultiValueHashTable
multiValueHashTable createMultiValueHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey,
                                              CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue,
                                              EqualFunction equalKey, TransformIntoNumberFunction transformIntoNumber,
                                              int hashNumber, EqualFunction equalValue) {
    MultiValueHashTable *multiHashTable = malloc(sizeof(struct multihashTable_s));
    if (multiHashTable == NULL) {
        return NULL; // Return NULL if memory allocation fails
    }
    multiHashTable->table = createHashTable(copyKey, freeKey, printKey, copylist,
                                            destroyList1, printList1, equalKey, transformIntoNumber, hashNumber);
    if (multiHashTable->table == NULL) {
        free(multiHashTable); // Free MultiValueHashTable if HashTable creation fails
        return NULL;
    }
    multiHashTable->printValue = printValue;
    multiHashTable->freeValue = freeValue;
    multiHashTable->copyValue = copyValue;
    multiHashTable->equalValue = equalValue;
    multiHashTable->printKey = printKey;
    return multiHashTable;
}

// Destroy a MultiValueHashTable
status destroyMultiValueHashTable(multiValueHashTable multiHashTable) {
    if (multiHashTable == NULL) {
        return failure; // Check if the MultiValueHashTable is NULL
    }
    destroyHashTable(multiHashTable->table); // Destroy the underlying hash table
    free(multiHashTable); // Free the MultiValueHashTable structure
    multiHashTable = NULL;
    return success;
}

// Add a value to the MultiValueHashTable for a specific key
status addToMultiValueHashTable(multiValueHashTable multiHashTable, Element key, Element value) {
    if (multiHashTable == NULL || key == NULL || value == NULL) {
        return failure; // Check for NULL inputs
    }
    linkedlist existingList = (linkedlist)lookupInHashTable(multiHashTable->table, key);
    if (existingList == NULL) {
        linkedlist newList = createLinkedList(multiHashTable->copyValue, multiHashTable->freeValue,
                                              multiHashTable->equalValue, multiHashTable->printValue);
        if (newList == NULL) {
            return failure; // Return failure if list creation fails
        }
        if (addToHashTable(multiHashTable->table, key, newList) == failure) {
            destroyList(newList); // Free the list if adding fails
            return failure;
        }
        return appendNode(newList, value); // Add the value to the new list
    }
    return appendNode(existingList, value); // Add the value to the existing list
}

// Lookup a list of values in the MultiValueHashTable by key
linkedlist lookupInMultiValueHashTable(multiValueHashTable multiHashTable, Element key) {
    if (multiHashTable == NULL || key == NULL) {
        return NULL; // Check for NULL inputs
    }
    if (lookupInHashTable(multiHashTable->table, key) == NULL) {
        return NULL; // Return NULL if key is not found
    }
    linkedlist existingList = (linkedlist)lookupInHashTable(multiHashTable->table, key);
    if (existingList == NULL) {
        return NULL; // Return NULL if no list exists for the key
    }
    return existingList;
}

// Remove a specific value for a key in the MultiValueHashTable
status removeFromMultiValueHashTable(multiValueHashTable multiHashTable, Element key, Element val) {
    if (multiHashTable == NULL || key == NULL) {
        return failure; // Check for NULL inputs
    }
    linkedlist existingList = (linkedlist)lookupInHashTable(multiHashTable->table, key);
    if (!existingList) {
        return failure; // Return failure if the key does not exist
    }
    deleteNode(existingList, val); // Remove the value from the list
    if (getLengthList(existingList) == 0) {
        removeFromHashTable(multiHashTable->table, key); // Remove the key if the list is empty
    }
    return success;
}

// Display all values for a specific key in the MultiValueHashTable
status displayMultiValueHashElementsByKey(multiValueHashTable multiHashTable, Element key) {
    if (multiHashTable == NULL) {
        return failure; // Check if the MultiValueHashTable is NULL
    }
    linkedlist existingList = (linkedlist)lookupInHashTable(multiHashTable->table, key);
    if (existingList == NULL) {
        return failure; // Return failure if the key does not exist
    }
    status s = multiHashTable->printKey(key);
    if (s != success) {
        return s;
    }
    s = printList(existingList);
    if (s != success) {
        return s;
    }
    return success;// Print the list of values for the key
}
