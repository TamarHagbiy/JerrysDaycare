//
// Created by tamar on 20/12/2024.
//

#ifndef MULTIVALUEHASHTABLE_H
#define MULTIVALUEHASHTABLE_H
#include "Defs.h"

/**
 * @file MultiValueHashTable.h
 * @brief Interface for a hash table where each key maps to a list of values.
 */

/**
 * Type definition for a MultiValueHashTable.
 * A hash table structure that supports multiple values for each key.
 */
typedef struct multihashTable_s *multiValueHashTable;

/**
 * @brief Creates a new MultiValueHashTable.
 * @param copyKey Function to copy keys.
 * @param freeKey Function to free keys.
 * @param printKey Function to print keys.
 * @param copyValue Function to copy values.
 * @param freeValue Function to free values.
 * @param printValue Function to print values.
 * @param equalKey Function to compare keys.
 * @param transformIntoNumber Function to hash keys into indices.
 * @param hashNumber Number of buckets in the hash table.
 * @param equalValue Function to compare values.
 * @return A pointer to the created MultiValueHashTable or NULL if creation fails.
 */
multiValueHashTable createMultiValueHashTable(CopyFunction copyKey, FreeFunction freeKey, PrintFunction printKey,
                                              CopyFunction copyValue, FreeFunction freeValue, PrintFunction printValue,
                                              EqualFunction equalKey, TransformIntoNumberFunction transformIntoNumber,
                                              int hashNumber, EqualFunction equalValue);

/**
 * @brief Destroys a MultiValueHashTable and frees all associated memory.
 * @param multiHashTable The MultiValueHashTable to destroy.
 * @return Status of the operation (success or failure).
 */
status destroyMultiValueHashTable(multiValueHashTable multiHashTable);

/**
 * @brief Adds a value to the MultiValueHashTable for a specific key.
 * If the key does not exist, a new key is created with an associated list of values.
 * @param multiHashTable The MultiValueHashTable.
 * @param key The key to add the value to.
 * @param value The value to add to the list associated with the key.
 * @return Status of the operation (success or failure).
 */
status addToMultiValueHashTable(multiValueHashTable multiHashTable, Element key, Element value);

/**
 * @brief Looks up the list of values associated with a specific key.
 * Returns the list directly (not a copy). Do not free the returned list.
 * @param multiHashTable The MultiValueHashTable.
 * @param key The key to look up.
 * @return The linked list of values associated with the key, or NULL if the key does not exist.
 */
linkedlist lookupInMultiValueHashTable(multiValueHashTable multiHashTable, Element key);

/**
 * @brief Removes a specific value associated with a key in the MultiValueHashTable.
 * If the list becomes empty after removing the value, the key is removed from the hash table.
 * @param multiHashTable The MultiValueHashTable.
 * @param key The key whose value needs to be removed.
 * @param val The value to remove from the list associated with the key.
 * @return Status of the operation (success or failure).
 */
status removeFromMultiValueHashTable(multiValueHashTable multiHashTable, Element key, Element val);

/**
 * @brief Displays all values associated with a specific key in the MultiValueHashTable.
 * Prints the values in the list associated with the key.
 * @param multiHashTable The MultiValueHashTable.
 * @param key The key whose values need to be displayed.
 * @return Status of the operation (success or failure).
 */
status displayMultiValueHashElementsByKey(multiValueHashTable multiHashTable, Element key);

#endif //MULTIVALUEHASHTABLE_H
