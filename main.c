#include <stdlib.h>     // malloc realloc
#include <stdio.h>      // printf, fprintf
#include <stdbool.h>    // bool
#include <string.h>     // strcpy, strcmp
#include <stdint.h>     // int64_t

#define NAME_LEN 50

typedef struct {
    size_t phone_num;
    char name[NAME_LEN];
} contact;

typedef struct {
    contact** arr;
    size_t size;
    size_t idx;
} arraylist;


contact* new_contact(int id, char* name);
void free_contact(contact* c);

arraylist new_arraylist(int initial_size);
void free_arraylist(arraylist* list);
void print_list(arraylist* list);
bool push(arraylist* list, contact* c);
bool pop(arraylist* list);
bool remove_idx(arraylist* list, size_t idx);   // Inserts and removes are constly for arraylists as they take O(n)
contact* get(arraylist* list, size_t idx);
int64_t search(arraylist* arr, char* name);     // returns the index, or -1 if not found

void check_null_ptr(void* ptr, char* func_name, char* operation); // helper function


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

    printf("Searching for 'you' (nohomo) returns: %ld\n", search(&list, "you"));
    printf("Searching for 'nonexistent_name' returns: %ld\n", search(&list, "nonexistent_name"));

    free_arraylist(&list);  

    return 0;
}


// ==============
// IMPLEMENTATION
// ==============

contact* new_contact(int phone_num, char* name) {
    contact* c = (contact*) malloc(sizeof(contact));
    check_null_ptr(c, "new_contact", "malloc");
    c->phone_num = phone_num;
    strcpy(c->name, name);
    
    return c;
}

void free_contact(contact* c) {
    free(c);
    c = NULL; // free(NULL) has no effect and is safe, this protects in case we double free 'c'
}


arraylist new_arraylist(int size) {
    arraylist list;
    list.size = size;
    list.idx = 0;
    list.arr = (contact**) malloc(sizeof(contact*) * size);
    check_null_ptr(list.arr, "new_arraylist_stack", "malloc");

    for (int i = 0; i < size; i++) 
        list.arr[i] = (contact*) malloc(sizeof(contact));
         
    return list;
}


void free_arraylist(arraylist* list) {
    for (size_t i = 0; i < list->idx; i++) 
        free_contact(list->arr[i]);
    free(list->arr);
}


bool push(arraylist* list, contact* c) {
    if (list->size == list->idx - 1) {
        int newsize = list->size * 2;
        list->arr = (contact**) realloc(list->arr, sizeof(contact) * newsize);
        check_null_ptr(list->arr, "push", "realloc");
        list->size = newsize;
        printf("[realloc: list->arr copied into new dynamic array of double the size]\n");
    }

    list->arr[list->idx++] = c;
    return true;
}


bool pop(arraylist* list) {
    if (list->size == 0 || list->idx == 0) return false;

    free_contact(list->arr[list->idx]); 
    list->idx--;

    if (list->idx < list->size / 2) {
        size_t newsize = list->size / 2;
        list->arr = (contact**) realloc(list->arr, sizeof(contact) * newsize);
        check_null_ptr(list->arr, "pop", "realloc");
    }
    return true;
}


bool remove_idx(arraylist* list, size_t idx) {
    if (list->idx < idx) {
        printf("remove from arraylist: index %ld out of bounds\n", idx);
        return false;
    } else if (list->idx == idx) {
        pop(list);
        return true;
    }

    contact** buff = (contact**) malloc(sizeof(contact) * list->idx - 1); 
    check_null_ptr(buff, "remove_idx", "malloc");

    // copy list->arr into buff, except for idx
    size_t buff_idx = 0, arr_idx = 0;
    while (arr_idx < list->idx) {
        if (arr_idx == idx) {
            arr_idx++; 
            continue;
        }
        buff[buff_idx++] = list->arr[arr_idx++];
    }

    free_contact(list->arr[idx]);   
    free(list->arr);        
    list->arr = buff;
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


int64_t search(arraylist* list, char* name) {
    for (size_t i = 0; i < list->idx; i++)
        if (strcmp(name, list->arr[i]->name) == 0)
            return (int) i;
    return -1;
}


void print_list(arraylist* list) {
    for (size_t i = 0; i < list->idx; i++) // use idx as list.length 
        printf("%ld, %s\n", list->arr[i]->phone_num, list->arr[i]->name);
}


void check_null_ptr(void* ptr, char* func_name, char* operation) {
    if (ptr == NULL) {
        fprintf(stderr, "%s: %s fail, exitting 'gracefully'\n", func_name, operation);
        exit(EXIT_FAILURE);
    }
}