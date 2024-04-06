#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>

typedef struct {
    size_t id;
    const char* msg; 
} contact;

typedef struct {
    contact** arr;
    size_t size;
    size_t idx;
} arraylist;

void check_null_ptr(void* ptr, const char* func_name, const char* operation);
contact* new_contact(int id, const char* msg);
void print_list(arraylist* list);

arraylist new_arraylist(int initial_size);
void free_arraylist(arraylist* list);
bool push(arraylist* list, contact* c);
bool pop(arraylist* list);
bool remove_idx(arraylist* list, size_t idx);
contact* get(arraylist* list, size_t idx);
int search(arraylist* arr, const char* name); // returns the index, or -1 if not found


int main() {
    arraylist list = new_arraylist(2); 

    printf("[Inserting into list]\n");
    
    push(&list, new_contact(1, "hello"));
    push(&list, new_contact(2, "how")); 
    push(&list, new_contact(3, "are"));
    push(&list, new_contact(4, "you"));
    push(&list, new_contact(5, "doing"));
    push(&list, new_contact(6, "on"));
    push(&list, new_contact(7, "this"));
    push(&list, new_contact(8, "fine day\n"));

    print_list(&list);

    printf("[removing 4 items with pop]\n");

    pop(&list);
    pop(&list);
    pop(&list);
    pop(&list);

    print_list(&list);

    printf("\n[removing idx 2 ]\n");

    remove_idx(&list, 2);

    print_list(&list);

    printf("Searching for 'you' (nohomo) returns: %d\n", search(&list, "you"));
    printf("Searching for 'nonexistent_name' returns: %d\n", search(&list, "nonexistent_name"));

    free_arraylist(&list);  
}


// ==============
// IMPLEMENTATION
// ==============

void check_null_ptr(void* ptr, const char* func_name, const char* operation) {
    if (ptr == NULL) {
        fprintf(stderr, "%s: %s fail, exitting 'gracefully'\n", func_name, operation);
        exit(EXIT_FAILURE);
    }
}


contact* new_contact(int id, const char* msg) {
    contact* c = (contact*) malloc(sizeof(contact));
    check_null_ptr(c, "new_contact", "malloc");
    c->id = id;
    c->msg = msg;
    return c;
}


// mutates the value of the arraylist.arr pointer
bool push(arraylist* list, contact* c) {
    if (list->size == list->idx - 1) {
        int newsize = list->size * 2;
        list->arr = (contact**) realloc(list->arr, sizeof(contact) * newsize);
        check_null_ptr(list->arr, "push", "realloc");
        list->size = newsize;
        printf("[realloc: list->arr copied into new dynamic array of double the size\n");
    }

    list->arr[list->idx++] = c;
    return true;
}


bool pop(arraylist* list) {
    if (list->size == 0 || list->idx == 0) return false;

    free(list->arr[list->idx]); // free the last element
    list->idx--;

    if (list->idx < list->size / 2) {
        size_t newsize = list->size / 2;
        list->arr = (contact**) realloc(list->arr, sizeof(contact) * newsize);
        check_null_ptr(list->arr, "pop", "realloc");
    }
    return true;
}


/* 
 * This is a costly operation, inserts and removes take O(n)
 * linked list will have constant time complexity for inserts and removes, consider using it instead
*/
bool remove_idx(arraylist* list, size_t idx) {
    if (list->idx < idx) {
        printf("remove from arraylist: index %ld out of bounds\n", idx);
        return false;
    } 

    if (idx == list->idx) {
        pop(list);
        return true;
    }

    contact** buff = (contact**) malloc(sizeof(contact) * list->idx - 1); 
    check_null_ptr(buff, "remove_idx", "malloc");

    size_t buff_idx = 0, arr_idx = 0;
    while (arr_idx < list->idx) {
        if (arr_idx == idx) {
            arr_idx++;
            continue;
        }
        buff[buff_idx] = list->arr[arr_idx];
        arr_idx++;
        buff_idx++;
    }

    free(list->arr[idx]);   // free the element 
    free(list->arr);        // free the dynamic array of pointers 
    list->arr = buff;       // assign list->arr to newarr
    list->idx--;
    
    return true;
}


contact* get(arraylist* list, size_t idx) {
    if (list->idx < idx) {
        fprintf(stderr, "get: index %ld is out of bounds\n", idx);
        exit(EXIT_FAILURE);
    }
    return list->arr[idx];
}


int search(arraylist* list, const char* name) {
    for (size_t i = 0; i < list->idx; i++)
        if (strcmp(name, list->arr[i]->msg) == 0)
            return (int) i;
    return -1;
}


void print_list(arraylist* list) {
    for (size_t i = 0; i < list->idx; i++) // use idx as list.length 
        printf("%ld, %s\n", list->arr[i]->id, list->arr[i]->msg);
}


/*
*   Makes arraylist on stack, returns arraylist struct by value
*/
arraylist new_arraylist(int size) {
    arraylist list;
    list.size = size;
    list.idx = 0;
    //list.arr = (contact**) malloc(sizeof(contact) * size); THIS WAS WRONG BUT PRODUCED NO ERRORS
    list.arr = (contact**) malloc(sizeof(contact*) * size);
    check_null_ptr(list.arr, "new_arraylist_stack", "malloc");

    for (int i = 0; i < size; i++) 
        list.arr[i] = (contact*) malloc(sizeof(contact));
         
    return list;
}


void free_arraylist(arraylist* list) {
    for (size_t i = 0; i < list->idx; i++) free(list->arr[i]);
    free(list->arr);
}

