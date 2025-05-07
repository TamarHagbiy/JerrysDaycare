//
// Created by tamar on 19/12/2024.
//

#include "KeyValuePair.h"

// Definition of the Key_Value structure, which contains key-value pairs and their associated functions
typedef struct Key_Value {
  Element key;                      // Key element
  Element value;                    // Value element

  CopyFunction copyKey;             // Function to copy the key
  CopyFunction copyValue;           // Function to copy the value
  EqualFunction compareKey;         // Function to compare keys
  FreeFunction destroyKey;          // Function to destroy the key
  FreeFunction destroyValue;        // Function to destroy the value
  PrintFunction printKey;           // Function to print the key
  PrintFunction printValue;         // Function to print the value
} Key_Value_Pair;

// Function to create a KeyValuePair
KeyValuePair createKeyValuePair(Element key, Element val, CopyFunction copyKey, CopyFunction copyValue, EqualFunction comperKey, FreeFunction destroyKey, FreeFunction destroyValue, PrintFunction printKey, PrintFunction printValue) {
  // Ensure all function pointers are not NULL
  if (copyKey == NULL || copyValue == NULL || comperKey == NULL || destroyKey == NULL || destroyValue == NULL || printValue == NULL || printKey == NULL) {
    return NULL;
  }

  // Allocate memory for the Key_Value_Pair structure
  Key_Value_Pair *key_val = (Key_Value_Pair *)malloc(sizeof(Key_Value_Pair));
  if (key_val == NULL) {
    return NULL; // Return NULL if memory allocation fails
  }

  // Copy the key and check if the copy was successful
  key_val->key = copyKey(key);
  if (key_val->key == NULL) {
    free(key_val); // Free allocated memory if key copy fails
    return NULL;
  }

  // Copy the value and check if the copy was successful
  key_val->value = copyValue(val);
  if (key_val->value == NULL) {
    destroyKey(key_val->key); // Destroy the copied key if value copy fails
    free(key_val); // Free allocated memory
    return NULL;
  }

  // Initialize function pointers
  key_val->copyKey = copyKey;
  key_val->copyValue = copyValue;
  key_val->compareKey = comperKey;
  key_val->destroyKey = destroyKey;
  key_val->destroyValue = destroyValue;
  key_val->printKey = printKey;
  key_val->printValue = printValue;

  return key_val; // Return the created KeyValuePair
}

// Function to destroy a KeyValuePair and free its resources
status destroyKeyValuePair(KeyValuePair pair) {
  if (pair == NULL) {
    return failure; // Return failure if the pair is NULL
  }

  // Destroy the key and value if they exist
  if (pair->key != NULL) {
    pair->destroyKey(pair->key);
  }
  if (pair->value != NULL) {
    pair->destroyValue(pair->value);
  }

  free(pair); // Free the memory of the KeyValuePair
  return success; // Return success
}

// Function to display the value of a KeyValuePair
status displayValue(KeyValuePair keyValuePair) {
  if (keyValuePair == NULL) {
    return failure; // Return failure if the KeyValuePair is NULL
  }

  if (keyValuePair->value != NULL) {
    keyValuePair->printValue(keyValuePair->value); // Print the value
    return success;
  }

  return failure; // Return failure if the value is NULL
}

// Function to display the key of a KeyValuePair
status displayKey(KeyValuePair keyValuePair) {
  if (keyValuePair == NULL) {
    return failure; // Return failure if the KeyValuePair is NULL
  }

  if (keyValuePair->key != NULL) {
    keyValuePair->printKey(keyValuePair->key); // Print the key
    return success;
  }

  return failure; // Return failure if the key is NULL
}

// Function to get the key from a KeyValuePair
KeyValuePair getKey(KeyValuePair keyValuePair) {
  if (keyValuePair == NULL) {
    return NULL; // Return NULL if the KeyValuePair is NULL
  }

  return keyValuePair->copyKey(keyValuePair->key); // Return the key
}

// Function to get the value from a KeyValuePair
KeyValuePair getValue(KeyValuePair keyValuePair) {
  if (keyValuePair == NULL) {
    return NULL; // Return NULL if the KeyValuePair is NULL
  }

  KeyValuePair pair = (KeyValuePair)keyValuePair;
  return keyValuePair->copyValue(pair->value); // Return the value
}

// Function to check if the key in a KeyValuePair is equal to a given key
bool isEqualkey(KeyValuePair pair, Element key) {
  if (key == NULL || pair == NULL) {
    return false; // Return false if either the key or pair is NULL
  }

  if (pair->key == NULL) {
    return false; // Return false if the key in the pair is NULL
  }

  return pair->compareKey(pair->key, key); // Compare the keys and return the result
}

// Function to display both the key and value of a KeyValuePair
status displaypair(KeyValuePair pair) {
  if (!pair) {
    return failure; // Return failure if the KeyValuePair is NULL
  }

  status a = displayKey(pair); // Display the key
  status b = displayValue(pair); // Display the value

  // Return failure if either displayKey or displayValue fails
  if (b == failure || a == failure) {
    return failure;
  }

  return success; // Return success if both displayKey and displayValue succeed
}
