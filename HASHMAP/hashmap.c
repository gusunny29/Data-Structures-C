
#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>

// TODO: sudo apt install libbsd-dev
// This provides strlcpy
// See "man strlcpy"
#include <bsd/string.h>
#include <string.h>

#include "hashmap.h"


int
hash(char* key)
{
    // TODO: Produce an appropriate hash value.	
    long hash = 0;
    for(long ii = 0; key[ii]; ++ii){
	hash = hash * 97 + key[ii];
    }
    return hash;
}

hashmap*
make_hashmap_presize(int nn)
{
    hashmap* hh = calloc(1, sizeof(hashmap));
    // TODO: Allocate and initialize a hashmap with capacity 'nn'.
    // Double check "man calloc" to see what that function does.
    hh->size = 0;
    hh->capacity = nn;
    hh->data = calloc(nn, sizeof(hashmap_pair));

    return hh;
}

hashmap*
make_hashmap()
{
    return make_hashmap_presize(4);
}

void
free_hashmap(hashmap* hh)
{
    // TODO: Free all allocated data.
    free(hh->data);
    free(hh);
}

int
hashmap_has(hashmap* hh, char* kk)
{
	if (hashmap_get(hh,kk) == -1) {
		
		return 0; //we return 0 if it is not here
	}
	
    return 1;
}

int
hashmap_get(hashmap* hh, char* kk)
{
    // TODO: Return the value associated with the
    // key kk.
    // Note: return -1 for key not found.
    int hash_of_key  = hash(kk);
    int ii = hash_of_key % hh->capacity;

    if (hh->data[ii].used == true) {
	    for (int index = ii; index < hh->capacity; ++index) {
		if( hh->data[index].used == false) {
			return -1;
			} 

		if (strcmp(hh->data[index].key, kk) == 0){
			return hh->data[index].val;
			}
		}
	    return -1;
    } else {
	//slot is empty so return -1
	return -1;
    } 
}


void 
map_grow(hashmap* hh) 
{
	hashmap_pair* dd =  hh->data;
	long cap = hh->capacity;

	hh->capacity = hh->capacity*2;
	hh->data = calloc(hh->capacity, sizeof(hashmap_pair));
	hh->size = 0;

	for (int idx = 0; idx < cap; ++idx) {
		if( dd[idx].used == true ){
		hashmap_put(hh, dd[idx].key, dd[idx].val);
		}
	}

	free(dd);
}

void
hashmap_put(hashmap* hh, char* kk, int vv)
{
    // TODO: Insert the value 'vv' into the hashmap
    // for the key 'kk', replacing any existing value
    // for that key	
    // .
    
	
	//Check if it is already in hashmap
	if(hashmap_has(hh,kk) == 0){
		//ITS NOT IN HASHMAP
		//we are going to insert it so we check if we need to grow first
	
		float loadFactor = (float)hh->size/hh->capacity;
	
		if (loadFactor >= 0.5){
			map_grow(hh);
		}
		
		int hash_of_key = hash(kk);
		int ii = hash_of_key % hh->capacity;
			//LINEAR PROBING: scan forward in data array to find empty slot
			for (int index = ii; index < hh->capacity; ++index) {
				if (hh->data[index].used == false) {
				//we have found an empty slot
				ii = index;
				break;
				}
			}
		
	
	strcpy(hh->data[ii].key, kk);
	hh->data[ii].val = vv;
	hh->data[ii].used = true;
	hh->data[ii].tomb = false;
	hh->size = hh->size + 1;
	} else {
		//ITS IN HASHMAP
		int hash_of_key = hash(kk);
		int ii = hash_of_key % hh->capacity;
		for (int idx = ii; idx < hh->capacity; ++idx) {
			if (strcmp(hh->data[idx].key, kk)== 0) {
				hh->data[idx].val = vv;		
			}
		}
	}
}


void
hashmap_del(hashmap* hh, char* kk)
{
    // TODO: Remove any value associated with
    // this key in the map.
    int hash_of_key = hash(kk);
    int ii = hash_of_key % hh->capacity;

    for(int index = ii; index < hh->capacity; ++index) {
	if (strcmp(hh->data[index].key, kk) == 0) {
		hh->data[index].tomb = true;
	}
    }
}

hashmap_pair
hashmap_get_pair(hashmap* hh, int ii)
{
    // TODO: Get the {k,v} pair stored in index 'ii'.
    return hh->data[ii];
}


void
hashmap_dump(hashmap* hh)
{
    printf("== hashmap dump ==\n");
    // TODO: Print out all the keys and values currently
    // in the map, in storage order. Useful for debugging.
    
    for (int ii = 0; ii < hh->capacity; ++ii) {
	if(hh->data[ii].used == true && hh->data[ii].tomb == false) {
		printf("Key: %s, Value:%d , Size: %d \n", hh->data[ii].key, hh->data[ii].val, hh->size);
		}
	}
}
