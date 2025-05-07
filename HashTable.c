//
// Created by tamar on 19/12/2024.
//

#include "HashTable.h"
#include "Defs.h"
#include <stdio.h>
#include "LinkedList.h"
#include "KeyValuePair.h"

// HashTable structure definition
typedef struct hashTable_s {
    int size; // Size of the hash table (number of buckets)
    linkedlist * hashTablearray; // Array of linked lists (buckets)
    CopyFunction copykey; // Function to copy keys
    FreeFunction freekey; // Function to free keys
    PrintFunction printkey; // Function to print keys
    PrintFunction printvalue; // Function to print values
    CopyFunction copyvalue; // Function to copy values
    FreeFunction freevalue; // Function to free values
    EqualFunction equalkey; // Function to compare keys
    EqualFunction equalkeyforlist; // Function to compare keys in linked list
    TransformIntoNumberFunction transformIntoNumber; // Function to transform a key into a number (hash function)
}HashTable;

// Helper function to return a copy of a key-value pair
Element getCopypair(Element keyValuePair) {
    if (!keyValuePair) return NULL; // Check if keyValuePair is NULL
    KeyValuePair pair = (KeyValuePair)keyValuePair;
    return pair;
}

// Helper function to destroy a key-value pair
status destroyKeyValuePair1(Element pair){
    if ( pair == NULL ) {
        return failure; // Check if the pair is NULL
    }
    KeyValuePair kvpair = (KeyValuePair)pair;
    return destroyKeyValuePair(kvpair); // Use destroyKeyValuePair to free the pair
}

// Helper function to display a key-value pair
status displaypair1(Element pair){
    if ( pair == NULL ) {
        return failure; // Check if the pair is NULL
    }
    KeyValuePair kv_pair = (KeyValuePair)pair;
    return displaypair(kv_pair); // Use displaypair to print the pair
}

// Helper function to compare a key-value pair with a key
bool getKeycmp(Element pair, Element key){
    if ( pair == NULL || key == NULL ) {
        return false; // Check if the pair or key is NULL
    }
    KeyValuePair kvpair = (KeyValuePair)pair;
    return isEqualkey(kvpair, key); // Use isEqualkey to compare the key
}

// Function to create a new hash table
hashTable createHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey, CopyFunction copyValue,
                          FreeFunction freeValue, PrintFunction printValue, EqualFunction equalKey,
                          TransformIntoNumberFunction transformIntoNumber, int hashNumber){
    if (!copyKey || !freeKey || !printKey || !copyValue || !freeValue || !printValue || !equalKey || !transformIntoNumber || hashNumber <= 0) {
        return NULL; // Validate input parameters
    }
    HashTable *newhashTable = malloc(sizeof(HashTable));
    if (!newhashTable) {
        return NULL; // Memory allocation failed
    }
    newhashTable->size = hashNumber;
    newhashTable->copykey = copyKey;
    newhashTable->freekey = freeKey;
    newhashTable->printkey = printKey;
    newhashTable->copyvalue = copyValue;
    newhashTable->freevalue = freeValue;
    newhashTable->printvalue = printValue;
    newhashTable->equalkey = equalKey;
    newhashTable->transformIntoNumber = transformIntoNumber;
    newhashTable->hashTablearray = malloc(sizeof(linkedlist) * hashNumber);
    if (!newhashTable->hashTablearray) {
        free(newhashTable); // Free allocated memory if allocation fails
        return NULL;
    }
    for (int i = 0; i < hashNumber; i++) {
        newhashTable->hashTablearray[i] = NULL; // Initialize buckets to NULL
    }
    return newhashTable;
}

// Function to destroy the hash table
status destroyHashTable(hashTable hashTable){
    if (!hashTable) {
        return failure; // Check if the hash table is NULL
    }
    for (int i = 0; i < hashTable->size; i++) {
        if (hashTable->hashTablearray[i]) {
            destroyList(hashTable->hashTablearray[i]); // Destroy each linked list
        }
    }
    free(hashTable->hashTablearray); // Free the array of linked lists
    free(hashTable); // Free the hash table structure
    return success;
}

// Function to add a key-value pair to the hash table
status addToHashTable(hashTable hashTable, Element key, Element value){
    if (!hashTable || !key || !value) {
        return failure; // Validate input
    }
    KeyValuePair new = createKeyValuePair(key, value, hashTable->copykey, hashTable->copyvalue, hashTable->equalkey, hashTable->freekey, hashTable->freevalue, hashTable->printkey, hashTable->printvalue);
    if (new == NULL) {
        return failure; // Creation of key-value pair failed
    }
    int idx = hashTable->transformIntoNumber(key); // Compute the hash index
    idx = idx % hashTable->size; // Ensure the index is within bounds
    if (hashTable->hashTablearray[idx] == NULL) {
        hashTable->hashTablearray[idx] = createLinkedList(getCopypair, destroyKeyValuePair1, getKeycmp, displaypair1); // Create a new linked list
        if (!hashTable->hashTablearray[idx]) {
            destroyKeyValuePair(new); // Cleanup if creation failed
            return failure;
        }
    }
    if (searchByKeyInList(hashTable->hashTablearray[idx], key) == NULL) {
        status add = appendNode(hashTable->hashTablearray[idx], new); // Add the key-value pair to the linked list
        if (add == failure) {
            destroyKeyValuePair(new); // Cleanup on failure
        }
        return add;
    }
    destroyKeyValuePair(new); // Cleanup duplicate key-value pair
    return failure;
}

// Function to lookup a value in the hash table by key
Element lookupInHashTable(hashTable hashTable, Element key){
    if (!hashTable || !key) {
        return NULL; // Validate input
    }
    int idx = hashTable->transformIntoNumber(key); // Compute the hash index
    idx = idx % hashTable->size; // Ensure the index is within bounds
    if (hashTable->hashTablearray[idx] == NULL) {
        return NULL; // Bucket is empty
    }
    Element val = searchByKeyInList(hashTable->hashTablearray[idx], key); // Search for the key in the bucket
    if (val == NULL) {
        return NULL; // Key not found
    }
    return getValue(val); // Return the value associated with the key
}

// Function to remove a key-value pair from the hash table
status removeFromHashTable(hashTable hashTable, Element key){
    if (!hashTable || !key) {
        return failure; // Validate input
    }
    int idx = hashTable->transformIntoNumber(key); // Compute the hash index
    idx = idx % hashTable->size; // Ensure the index is within bounds
    if (hashTable->hashTablearray[idx] == NULL) {
        return failure; // Bucket is empty
    }
    Element val = searchByKeyInList(hashTable->hashTablearray[idx], key); // Search for the key in the bucket
    if (val == NULL) {
        return failure; // Key not found
    }
    deleteNode(hashTable->hashTablearray[idx], key); // Remove the key-value pair
    if (getLengthList(hashTable->hashTablearray[idx]) == 0) {
        destroyList(hashTable->hashTablearray[idx]); // Destroy the bucket if empty
        hashTable->hashTablearray[idx] = NULL;
    }
    return success;
}

// Function to display all elements in the hash table
status displayHashElements(hashTable hashTable){
    if (!hashTable) {
        return failure; // Validate input
    }
    for (int i = 0; i < hashTable->size; i++) {
        if (hashTable->hashTablearray[i]) {
            if (printList(hashTable->hashTablearray[i]) == failure) {
                return failure; // Print each non-empty bucket
            }
        }
    }
    return success;
}
