#include <stdio.h>
#include <stdlib.h>
#include "list.h" 

typedef struct Node {
    int data;
    struct Node *next;
} Node;

struct List {
    Node *head;
    Node *tail;
    size_t size;
};

/* Lifespan / Memory Management */
List *list_create(void) {
    List *l = malloc(sizeof(List));
    if (!l) return NULL;

    l->head = NULL;
    l->tail = NULL;
    l->size = 0;

    return l;
}

void list_clear(List *l) {
    if (!l) return;

    Node *current = l->head;
    while (current) {
        Node *next = current->next;
        free(current);
        current = next;
    }

    l->head = NULL;
    l->tail = NULL;
    l->size = 0;
}

void list_destroy(List *l) {
    if (!l) return;

    list_clear(l);
    free(l);
}

/* Accessors */
size_t list_size(const List *l) {
    return l ? l->size : 0;
}

int list_empty(const List *l) {
    return (!l || !l->head) ? 1 : 0;
}

int list_front(const List *l, int *out) {
    if (!l || !l->head || !out) return -1;

    *out = l->head->data;
    return 0;
}

int list_back(const List *l, int *out) {
    if (!l || !l->head || !out) return -1;

    *out = l->tail->data;
    return 0;
}

int list_get(const List *l, size_t pos, int *out) {
    if (!l || !l->head || !out || pos >= l->size) return -1;

    Node *current = l->head;
    for (size_t i = 0; i < pos; i++) {
        current = current->next;
    }
    
    *out = current->data;
    return 0;
}

/* Insertion */
int list_push_front(List *l, int value) {
    if (!l) return -1;

    Node *new_node = malloc(sizeof(Node));
    if (!new_node) return -1;

    new_node->data = value;
    new_node->next = l->head;
    l->head = new_node;

    if (!l->size) {
        l->tail = new_node;
    }
    
    l->size++;
    return 0;
}

int list_push_back(List *l, int value) {
    if (!l) return -1;

    Node *new_node = malloc(sizeof(Node));
    if (!new_node) return -1;

    new_node->data = value;
    new_node->next = NULL;

    if (!l->head) {
        l->head = new_node;
        l->tail = new_node;
        l->size++;
        return 0;
    }
    
    l->tail->next = new_node;
    l->tail = new_node;
    l->size++;
    return 0;
}

int list_insert_at(List *l, size_t pos, int value) {
    if (!l || pos > l->size) return -1;

    /* 1. Handle edge cases */
    if (pos == 0) {
        return list_push_front(l, value);
    }

    if (pos == l->size) {
        return list_push_back(l, value);
    }
    
    /* 2. Middle insertion (O(n)) */
    Node *new_node = malloc(sizeof(Node));
    if (!new_node) return -1;

    new_node->data = value;

    /* 3. Find previous node */
    Node *prev = l->head;
    for (size_t i = 0; i < pos - 1; i++) {
        prev = prev->next;
    }
    
    /* 4. Pointer manipulation */
    new_node->next = prev->next;
    prev->next = new_node;

    l->size++;
    return 0;
}

/* Deletion */
int list_pop_front(List *l, int *out) {
    if (!l || !l->head || !out) return -1;

    *out = l->head->data;

    Node *temp = l->head;
    l->head = temp->next;

    if (!l->head) {
        l->tail = NULL;
    }

    free(temp);
    l->size--;
    
    return 0;
}

int list_pop_back(List *l, int *out) {
    if (!l || !l->head || !out) return -1;

    *out = l->tail->data;

    /* Case: Only one node in the list */
    if (!l->head->next) {
        free(l->head);
        l->head = NULL;
        l->tail = NULL;
        l->size--;
        return 0;
    }

    /* Case: Multiple nodes in the list. Find the second-to-last node */
    Node *penultimate = l->head;
    while (penultimate->next != l->tail) {
        penultimate = penultimate->next;
    }
    
    free(l->tail);
    l->tail = penultimate;
    l->tail->next = NULL;
    l->size--;

    return 0;
}

int list_remove(List *l, int value, int *out) {
    if (!l || !l->head || !out) return -1;

    /* 1. Handle edge cases */
    if (l->head->data == value) {
        return list_pop_front(l, out);
    }
    
    if (l->tail->data == value) {
        return list_pop_back(l, out);
    }
    
    /* 2. Middle deletion */
    Node *current = l->head->next;
    Node *prev = l->head;

    while (current) {
        if (current->data == value) {
            *out = current->data;
            prev->next = current->next;
            free(current);
            l->size--;
            return 0;
        }
        prev = current;
        current = current->next;
    }
    
    return -1; /* Value not found in the list */
}

/* Operations */
int list_reverse(List *l) {
    if (!l) return -1;

    Node *prev = NULL;
    Node *current = l->head;
    Node *next = NULL;

    /* Save current head, which will become the new tail */
    Node *old_head = l->head;

    while (current) {
        /* Save next link so we don't lose the list references */
        next = current->next;

        /* Reverse link direction */
        current->next = prev;

        /* Advance the pointers one step */
        prev = current;
        current = next;
    }
    
    l->head = prev;
    l->tail = old_head;

    return 0;
}

/* Utilities / Printing */
void list_print(const List *l) {
    if (!l) return;

    Node *current = l->head;
    while (current) {
        printf("%d -> ", current->data);
        current = current->next;
    }
    printf("NULL\n");
}
