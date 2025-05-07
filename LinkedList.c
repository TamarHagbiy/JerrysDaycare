//
// Created by tamar on 19/12/2024.
#include "LinkedList.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Node structure: contains data and a pointer to the next node
typedef struct node_h{
    Element data; // Data stored in the node
    struct node_h * next; // Pointer to the next node
}Node;

// LinkedList structure: manages the head node, size, and function pointers
typedef struct List_h {
    Node * head; // Pointer to the head node
    int size; // Number of elements in the list
    CopyFunction copy_func; // Function pointer for copying elements
    FreeFunction free_func; // Function pointer for freeing elements
    EqualFunction cmp_func; // Function pointer for comparing elements
    PrintFunction print_func; // Function pointer for printing elements
}LinkedList;

// Function to create a new linked list
linkedlist createLinkedList(CopyFunction copy_func, FreeFunction free_func, EqualFunction cmp_func, PrintFunction print_func) {
    if (copy_func == NULL || free_func == NULL || cmp_func == NULL || print_func == NULL) {
        return NULL; // Ensure all function pointers are provided
    }
    linkedlist list = (linkedlist)malloc(sizeof(struct List_h));
    if (!list) {
        return NULL; // Memory allocation failed
    }
    list->copy_func = copy_func;
    list->free_func = free_func;
    list->cmp_func = cmp_func;
    list->print_func = print_func;
    list->size = 0;
    list->head = NULL; // Initialize the list as empty
    return list;
};

// Function to create a new node with given data
Node *createNode(Element data, linkedlist List) {
    if (!data) {
        return NULL; // Data must not be NULL
    }
    if (List == NULL) {
        return NULL; // Ensure the list is valid
    }
    Node *node = malloc(sizeof(Node));
    if (!node) {
        return NULL; // Memory allocation failed
    }
    node->data = List->copy_func(data); // Copy data using the provided function
    if (node->data == NULL) {
        free(node); // Free node if data copy fails
        return NULL;
    }
    node->next = NULL; // Initialize next pointer to NULL
    return node;
}

// Function to destroy the linked list and free all allocated memory
status destroyList(linkedlist List) {
    if (!List) {
        return failure; // Ensure the list is valid
    }
    linkedlist list = (linkedlist)List;
    if (list->size == 0) {
        free(list); // Free the list structure if empty
        return success;
    }

    Node *current = list->head;
    for (int i = 0; i < list->size; i++) {
        Node *temp = current;
        current = current->next;
        list->free_func(temp->data); // Free the data in the node
        free(temp); // Free the node itself
    }
    list->head = NULL; // Set the head to NULL
    list->size = 0; // Reset the size
    free(list); // Free the list structure
    return success;
}

// Function to append a new node with data to the end of the list
status appendNode(linkedlist list, Element data) {
    if (!list) return failure; // Ensure the list is valid
    if (!data) return failure; // Ensure the data is valid

    Node *new_node = createNode(data, list);
    if (!new_node) {
        return failure; // Node creation failed
    }
    if (!list->head) {
        list->head = new_node; // Set as head if the list is empty
        list->size++;
        return success;
    }
    Node *current = list->head;
    while (current->next != NULL) {
        current = current->next; // Traverse to the last node
    }

    current->next = new_node; // Append the new node to the end
    list->size++;
    return success;
}

// Function to delete a node with the specified data from the list
status deleteNode(linkedlist list, Element data) {
    if (!list) return failure; // Ensure the list is valid
    if (!data) return failure; // Ensure the data is valid
    if (!list->head) {
        return failure; // List is empty
    }
    if (list->cmp_func(list->head->data, data)) {
        Node *temp = list->head;
        list->head = list->head->next; // Update the head to the next node
        list->free_func(temp->data); // Free the data in the node
        free(temp); // Free the node itself
        list->size--;
        return success;
    }

    Node *current = list->head;
    for (int i = 0; i < list->size - 1; i++) { // Traverse until the second-to-last node
        Node *temp = current->next;
        if (list->cmp_func(temp->data, data)) {
            current->next = temp->next; // Remove the node from the list
            list->free_func(temp->data); // Free the data in the node
            free(temp); // Free the node itself
            list->size--;
            return success;
        }
        current = current->next;
    }
    return failure; // Node not found
}

// Function to print the linked list
status printList(linkedlist list) {
    if (!list || list->size == 0) {
        return failure;
    }

    Node *current = list->head;
    while (current != NULL) {
        if (current->data) {
            if (list->print_func) {
                list->print_func(current->data); // Print the data using the provided function
            } else {
                printf("Data is NULL.\n");
            }
        }
        current = current->next; // Move to the next node
    }

    return success;
}

// Function to get a copy of the data at a specific index in the list
Element getDataByIndex(linkedlist list, int index) {
    if (!list || index < 0 || index >= list->size) {
        return NULL; // Check if the list is valid and index is within bounds
    }

    Node *current = list->head;
    if (current == NULL) {
        return NULL; // List is empty
    }
    for (int i = 0; i < index; i++) {
        current = current->next; // Traverse to the desired index
    }
    return list->copy_func(current->data); // Return a copy of the data
}

// Function to get the length of the list
int getLengthList(linkedlist list) {
    if (!list) return failure; // Ensure the list is valid
    return list->size; // Return the size of the list
}

// Function to search for an element in the list by key
Element searchByKeyInList(linkedlist list, Element key) {
    if (!key) {
        return NULL;
    }
    if (!list) return NULL; // Ensure the list is valid
    if (!list->head) return NULL; // List is empty
    Node *current = list->head;
    while (current) {
        if (list->cmp_func(current->data, key)) {
            return list->copy_func(current->data); // Return a copy of the matching data
        }
        current = current->next; // Move to the next node
    }
    return NULL; // Key not found
}
