#include "ruby/ruby.h"
#include "ruby/encoding.h"
#include "library.h"

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/mman.h>
#include <unistd.h>

#define SIZE 20

struct DataItem {
    int data;
    int key;
};

struct DataItem **hashArray;
void *dataArea;

struct DataItem* nullItem;

void* create_shared_memory(size_t size) {
    int protection = PROT_READ | PROT_WRITE;
    int visibility = MAP_SHARED | MAP_ANONYMOUS;
    return mmap(NULL, size, protection, visibility, -1, 0);
}


int hashCode(int key) {
    return key % SIZE;
}

void insert(int key,int data) {
    //get the hash
    int hashIndex = hashCode(key);

    //move in array until an empty or deleted cell
    while(hashArray[hashIndex] != NULL && hashArray[hashIndex]->key != -1) {
        //go to next cell
        ++hashIndex;

        //wrap around the table
        hashIndex %= SIZE;
    }

    struct DataItem *item = (struct DataItem*) (dataArea + sizeof(struct DataItem) * hashIndex);
    item->data = data;
    item->key = key;

    hashArray[hashIndex] = item;
}

void display() {
    int i = 0;

    for(i = 0; i<SIZE; i++) {

        if(hashArray[i] != NULL)
            printf(" (%d,%d)",hashArray[i]->key,hashArray[i]->data);
        else
            printf(" nil ");
    }

    printf("\n");
}

VALUE rb_init() {
    hashArray = (struct DataItem**)create_shared_memory(sizeof(void *) * SIZE);
    dataArea = create_shared_memory(sizeof(struct DataItem) * SIZE);
    return Qnil;
}

VALUE rb_insert(VALUE self, VALUE key, VALUE val) {
    int key_in = NUM2INT(key);
    int val_in = NUM2INT(val);
    insert(key_in, val_in);
    return Qnil;
}

VALUE rb_display() {
    display();
    return Qnil;
}

void Init_extension(void) {
    VALUE CFromRubyExample = rb_define_module("CacheRb");
    VALUE NativeHelpers = rb_define_class_under(CFromRubyExample, "NativeHelpers", rb_cObject);

    rb_define_singleton_method(NativeHelpers, "init", rb_init, 0);
    rb_define_singleton_method(NativeHelpers, "insert", rb_insert, 2);
    rb_define_singleton_method(NativeHelpers, "display", rb_display, 0);
}
