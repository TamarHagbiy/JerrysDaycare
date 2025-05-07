//
// Created by tamar on 19/12/2024.
//

#ifndef KEYVALUEPAIR_H
#define KEYVALUEPAIR_H

#include "Defs.h"

// Define KeyValuePair as a pointer to the Key_Value structure
typedef struct Key_Value* KeyValuePair;

// The following typedefs (commented out) might be useful for understanding the required function signatures for elements:
// typedef element (*copyKey)(element); // Function to copy a key element
// typedef element (*copyValue)(element); // Function to copy a value element
// typedef bool (*compareKey)(element, element); // Function to compare two key elements
// typedef status (*destroyKey)(element); // Function to destroy a key element
// typedef status (*destroyValue)(element); // Function to destroy a value element
// typedef status (*printKey)(element); // Function to print a key element
// typedef status (*printValue)(element); // Function to print a value element

/**
 * Creates a new KeyValuePair object.
 *
 * @param key - The key element for the pair.
 * @param val - The value element for the pair.
 * @param copyKey - Function to copy the key element.
 * @param copyValue - Function to copy the value element.
 * @param comperKey - Function to compare two key elements.
 * @param destroyKey - Function to free memory of the key element.
 * @param destroyValue - Function to free memory of the value element.
 * @param printKey - Function to print the key element.
 * @param printValue - Function to print the value element.
 *
 * @return A pointer to the created KeyValuePair, or NULL if an error occurs.
 *
 * Notes:
 * - The provided functions must not be NULL; otherwise, the creation will fail.
 * - The caller is responsible for ensuring that the key and value are valid and compatible with the provided functions.
 */
KeyValuePair createKeyValuePair(Element key, Element val, CopyFunction copyKey, CopyFunction copyValue, EqualFunction comperKey, FreeFunction destroyKey, FreeFunction destroyValue, PrintFunction printKey, PrintFunction printValue);

/**
 * Frees the memory used by a KeyValuePair.
 *
 * @param keyValuePair - The KeyValuePair to destroy.
 *
 * @return `success` if the operation was successful, `failure` otherwise.
 *
 * Notes:
 * - Safely frees both the key and value elements using their respective destroy functions.
 * - If the KeyValuePair is NULL, no operation is performed.
 */
status destroyKeyValuePair(KeyValuePair keyValuePair);

/**
 * Displays the value of the KeyValuePair using the provided print function.
 *
 * @param keyValuePair - The KeyValuePair whose value will be displayed.
 *
 * @return `success` if the value was displayed, `failure` if the value or KeyValuePair is NULL.
 */
status displayValue(KeyValuePair keyValuePair);

/**
 * Displays the key of the KeyValuePair using the provided print function.
 *
 * @param keyValuePair - The KeyValuePair whose key will be displayed.
 *
 * @return `success` if the key was displayed, `failure` if the key or KeyValuePair is NULL.
 */
status displayKey(KeyValuePair keyValuePair);

/**
 * Retrieves the key from the KeyValuePair.
 *
 * @param keyValuePair - The KeyValuePair to query.
 *
 * @return The key element, or NULL if the KeyValuePair is NULL.
 */
KeyValuePair getKey(KeyValuePair keyValuePair);

/**
 * Retrieves the value from the KeyValuePair.
 *
 * @param keyValuePair - The KeyValuePair to query.
 *
 * @return The value element, or NULL if the KeyValuePair is NULL.
 */
KeyValuePair getValue(KeyValuePair keyValuePair);

/**
 * Checks if the key in a KeyValuePair matches a given key element.
 *
 * @param a - The KeyValuePair to query.
 * @param b - The key element to compare against.
 *
 * @return `true` if the keys are equal, `false` otherwise.
 *
 * Notes:
 * - Uses the compare function provided during KeyValuePair creation.
 * - Returns `false` if either the KeyValuePair or the key is NULL.
 */
bool isEqualkey(KeyValuePair a, Element b);

/**
 * Displays both the key and value of a KeyValuePair.
 *
 * @param keyValuePair - The KeyValuePair to display.
 *
 * @return `success` if both the key and value were displayed, `failure` otherwise.
 *
 * Notes:
 * - Calls `displayKey` and `displayValue` internally.
 * - Failure of either operation results in a `failure` return value.
 */
status displaypair(KeyValuePair keyValuePair);

#endif //KEYVALUEPAIR_H
