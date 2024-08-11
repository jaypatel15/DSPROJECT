#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#pragma warning(disable : 4996)

// Structure for each parcel
typedef struct Parcel {
    char* country;    // Dynamically allocated
    double weight;
    double value;
    struct Parcel* left, * right;
} Parcel;

// Hash table with 127 buckets, each holding the root of a BST
typedef struct HashTable {
    Parcel* table[127];
} HashTable;


