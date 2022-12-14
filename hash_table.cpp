/*
 Student Name: Brandon Goddard

This file provides definition for the structs and functions declared in the
header file. It also contains helper functions that are not accessible from
outside of the file.

FOR FULL CREDIT, BE SURE TO TRY MULTIPLE TEST CASES and DOCUMENT YOUR CODE.

===================================
Naming conventions in this file:
===================================
1. All struct names use camel case where the first letter is capitalized.
  e.g. "HashTable", or "HashTableEntry"

2. Variable names with a preceding underscore "_" will not be called directly.
  e.g. "_HashTable", "_HashTableEntry"

  Recall that in C, we have to type "struct" together with the name of the struct
  in order to initialize a new variable. To avoid this, in hash_table.h
  we use typedef to provide new "nicknames" for "struct _HashTable" and
  "struct _HashTableEntry". As a result, we can create new struct variables
  by just using:
    - "HashTable myNewTable;"
     or
    - "HashTableEntry myNewHashTableEntry;"

  The preceding underscore "_" simply provides a distinction between the names
  of the actual struct defition and the "nicknames" that we use to initialize
  new structs.
  [See Hidden Definitions section for more information.]

3. Functions, their local variables and arguments are named with camel case, where
  the first letter is lower-case.
  e.g. "createHashTable" is a function. One of its arguments is "numBuckets".
       It also has a local variable called "newTable".

4. The name of a struct member is divided by using underscores "_". This serves
  as a distinction between function local variables and struct members.
  e.g. "num_buckets" is a member of "HashTable".

*/

/****************************************************************************
* Include the Public Interface
*
* By including the public interface at the top of the file, the compiler can
* enforce that the function declarations in the the header are not in
* conflict with the definitions in the file. This is not a guarantee of
* correctness, but it is better than nothing!
***************************************************************************/
#include "hash_table.h"


/****************************************************************************
* Include other private dependencies
*
* These other modules are used in the implementation of the hash table module,
* but are not required by users of the hash table.
***************************************************************************/
#include <stdlib.h>   // For malloc and free
#include <stdio.h>    // For printf


/****************************************************************************
* Hidden Definitions
*
* These definitions are not available outside of this file. However, because
* the are forward declared in hash_table.h, the type names are
* available everywhere and user code can hold pointers to these structs.
***************************************************************************/
/**
 * This structure represents an a hash table.
 * Use "HashTable" instead when you are creating a new variable. [See top comments]
 */

struct _HashTable {
  /** The array of pointers to the head of a singly linked list, whose nodes
      are HashTableEntry objects */
  HashTableEntry** buckets;
  /** The hash function pointer */
  HashFunction hash;
  /** The number of buckets in the hash table */
  unsigned int num_buckets;
};

/**
 * This structure represents a hash table entry.
 * Use "HashTableEntry" instead when you are creating a new variable. [See top comments]
 */
struct _HashTableEntry {
  /** The key for the hash table entry */
  unsigned int key;
  /** The value associated with this hash table entry */
  void* value;
  /**
  * A pointer pointing to the next hash table entry
  * NULL means there is no next entry (i.e. this is the tail)
  */
  HashTableEntry* next;
};


/****************************************************************************
* Private Functions
*
* These functions are not available outside of this file, since they are not
* declared in hash_table.h.
***************************************************************************/
/**
* createHashTableEntry
*
* Helper function that creates a hash table entry by allocating memory for it on
* the heap. It initializes the entry with key and value, initialize pointer to
* the next entry as NULL, and return the pointer to this hash table entry.
*
* @param key The key corresponds to the hash table entry
* @param value The value stored in the hash table entry
* @return The pointer to the hash table entry
*/
static HashTableEntry* createHashTableEntry(unsigned int key, void* value) {
    // alloc space for the new entry struct
    HashTableEntry* newEntry = (HashTableEntry*)malloc(sizeof(HashTableEntry));
    // init key, value, and next appropriately
    newEntry->key = key;
    newEntry->value = value;
    newEntry->next = NULL;
    // return the pointer to this created entry
    return newEntry;
}

/**
* findItem
*
* Helper function that checks whether there exists the hash table entry that
* contains a specific key.
*
* @param hashTable The pointer to the hash table.
* @param key The key corresponds to the hash table entry
* @return The pointer to the hash table entry, or NULL if key does not exist
*/
static HashTableEntry* findItem(HashTable* hashTable, unsigned int key) {
    // find the corresponding index of key in the hashTable w/ hashfunction ptr
    unsigned int ind = hashTable->hash(key);
    // go through linked list(s) to see if key exists
    HashTableEntry* p = hashTable->buckets[ind];
    // test if bucket is empty
    while (p) {
        // if we find a matching key
        if ((p->key)==key)
            return p;
        // if we reach the end of the bucket & find no matching key
        p = p->next;
    }
    // the key does not exist
    p = NULL;
    return p;
}

/****************************************************************************
* Public Interface Functions
*
* These functions implement the public interface as specified in the header
* file, and make use of the private functions and hidden definitions in the
* above sections.
****************************************************************************/
// The createHashTable is provided for you as a starting point.
HashTable* createHashTable(HashFunction hashFunction, unsigned int numBuckets) {
  // The hash table has to contain at least one bucket. Exit gracefully if
  // this condition is not met.
  if (numBuckets==0) {
    printf("Hash table has to contain at least 1 bucket...\n");
    exit(1);
  }
  // Allocate memory for the new HashTable struct on heap.
  HashTable* newTable = (HashTable*)malloc(sizeof(HashTable));
  // Initialize the components of the new HashTable struct.
  newTable->hash = hashFunction;
  newTable->num_buckets = numBuckets;
  newTable->buckets = (HashTableEntry**)malloc(numBuckets*sizeof(HashTableEntry*));
  // As the new buckets are empty, init each bucket as NULL.
  unsigned int i;
  for (i=0; i<numBuckets; ++i) {
    newTable->buckets[i] = NULL;
  }
  // Return the new HashTable struct.
  return newTable;
}

void destroyHashTable(HashTable* hashTable) {
    unsigned int ind;
    for (ind=0;ind<(hashTable->num_buckets);ind++) {
        HashTableEntry* p = hashTable->buckets[ind];
        // if bucket is empty, move along
        if (!p) {
            continue;
        }
        while(hashTable->buckets[ind]) {
            // store next to make next entry the first in the list
            HashTableEntry* nex = hashTable->buckets[ind]->next;
            // free the first entry and its value
            free(hashTable->buckets[ind]->value);
            free(hashTable->buckets[ind]);
            // set new first entry in bucket
            hashTable->buckets[ind] = nex;
        }
    }
    // lastly, free the hash table
    free(hashTable->buckets);
    free(hashTable);
}

void* insertItem(HashTable* hashTable, unsigned int key, void* value) {
    HashTableEntry* p = findItem(hashTable, key);
    // if we found the entry
    if (p) {
        // store and return old value ptr
        void* old = p->value;
        // free the old value IFF it is not the same value given
        if (old!=value) {
            free(p->value);
            p->value = value;
            // return old value when replaced
            return old;
        }
        // return NULL if old and value are the same, no overwrite takes place
        return NULL;
    } else {
        // find the right bucket & create a new entry
        HashTableEntry* new2 = createHashTableEntry(key, value);
        unsigned int ind = hashTable->hash(key);
        // insert in front of bucket (test if bucket is empty first)
        HashTableEntry* p2 = hashTable->buckets[ind];
        if (p2) {
            new2->next = p2;
        }
        hashTable->buckets[ind] = new2;
        return NULL;
    }
}

void* getItem(HashTable* hashTable, unsigned int key) {
    HashTableEntry* p = findItem(hashTable, key);
    // if findItem() returns null, the key was not found
    // otherwise, return the correct value ptr
    if (p) {
        return p->value;
    } else {
        return NULL;
    }
}

void* removeItem(HashTable* hashTable, unsigned int key) {
    HashTableEntry* p = findItem(hashTable, key);
    if (p) {
        unsigned int ind = hashTable->hash(key);
        HashTableEntry* p2 = hashTable->buckets[ind];
        // if the entry is at the front of the list
        if ((hashTable->buckets[ind]->key)==key) {
            // free the entry ptr & make correct connections
            void* val = p->value;
            free(hashTable->buckets[ind]);
            hashTable->buckets[ind] = p->next;
            return val;
        }
        // if entry is not in front of list
        while (1) {
            if ((p2->next->key)==key) {
                // free the entry ptr & make correct connections
                void* val = p2->next->value;
                HashTableEntry* nex = p2->next->next;
                free(p2->next);
                p2->next = nex;
                return val;
            }
            // continue down the list until we find the key
            p2 = p2->next;
        }
    } else {
        return NULL;
    }
}

void deleteItem(HashTable* hashTable, unsigned int key) {
    HashTableEntry* p = findItem(hashTable, key);
    if (p) {
        unsigned int ind = hashTable->hash(key);
        HashTableEntry* p2 = hashTable->buckets[ind];
        // if the entry is at the front of the list
        if ((hashTable->buckets[ind]->key)==key) {
            // free the entry & make correct connections
            free(hashTable->buckets[ind]->value);
            free(hashTable->buckets[ind]);
            hashTable->buckets[ind] = p2->next;
        }
        // if entry is not in front of list
        while (p2->next) {
            if ((p2->next->key)==key) {
                // free the entry & make correct connections
                HashTableEntry* nex = p2->next->next;
                free(p2->next->value);
                free(p2->next);
                p2->next = nex;
                break;
            }
            // continue down the list until we find the key
            p2 = p2->next;
        }
    }
}