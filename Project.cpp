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


unsigned long hash_function(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % 127;
}




Parcel* create_node(const char* country, double weight, double value) {
    Parcel* newNode = (Parcel*)malloc(sizeof(Parcel));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed for new node.\n");
        exit(EXIT_FAILURE);
    }


    size_t len = strlen(country);
    if (len > 20) {
        len = 20;
    }

    newNode->country = (char*)malloc(len + 1);
    if (newNode->country == NULL) {
        fprintf(stderr, "Memory allocation failed for country name.\n");
        free(newNode);
        exit(EXIT_FAILURE);
    }
    strncpy(newNode->country, country, len);
    newNode->country[len] = '\0';

    newNode->weight = weight;
    newNode->value = value;
    newNode->left = newNode->right = NULL;
    return newNode;
}



Parcel* insert(Parcel* root, const char* country, double weight, double value) {
    if (root == NULL) return create_node(country, weight, value);
    if (weight < root->weight)
        root->left = insert(root->left, country, weight, value);
    else
        root->right = insert(root->right, country, weight, value);
    return root;
}


void in_order(Parcel* root) {
    if (root != NULL) {
        in_order(root->left);
        printf("Country: %s, Weight: %.2f, Value: %.2f\n", root->country, root->weight, root->value);
        in_order(root->right);
    }
}


void insert_into_hash_table(HashTable* ht, const char* country, double weight, double value) {
    unsigned long index = hash_function(country);
    ht->table[index] = insert(ht->table[index], country, weight, value);
}

void load_data(HashTable* ht, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char buffer[100];
    char country[30];
    double weight = 0.0, value = 0.0;
    char* token;

    while (fgets(buffer, sizeof(buffer), file)) {

        buffer[strcspn(buffer, "\n")] = '\0';


        token = strtok(buffer, ",");
        if (token != NULL) {
            strncpy(country, token, sizeof(country) - 1);
            country[sizeof(country) - 1] = '\0';
            to_lowercase(country);
        }


        token = strtok(NULL, ",");
        if (token != NULL) {
            weight = atof(token);

            // Validate weight range
            if (weight < 100 || weight > 50000) {
                fprintf(stderr, "Invalid weight: %.2f. Skipping entry.\n", weight);
                continue;
            }
        }


        token = strtok(NULL, ",");

      if (token != NULL)
       {
                value = atof(token);

              
                if (value < 10 || value > 2000)
                {
                fprintf(stderr, "Invalid value: %.2f. Skipping entry.\n", value);
                continue;
                }
      }

        insert_into_hash_table(ht, country, weight, value);
    }

    fclose(file);
}
<<<<<<< HEAD



void display_all_parcels(Parcel* root) {
    if (root != NULL) {
        in_order(root);
    }
    else {
        printf("No parcels found.\n");
    }
}


void display_parcels_by_weight(Parcel* root, double weight, int higher) {
    if (root == NULL) return;

    display_parcels_by_weight(root->left, weight, higher);

    if ((higher && root->weight > weight) || (!higher && root->weight < weight)) {
        printf("Country: %s, Weight: %.2f, Value: %.2f\n", root->country, root->weight, root->value);
    }

    display_parcels_by_weight(root->right, weight, higher);
}


void display_total_load_and_valuation(Parcel* root, double* total_weight, double* total_value) {
    if (root != NULL) {
        *total_weight += root->weight;
        *total_value += root->value;
        display_total_load_and_valuation(root->left, total_weight, total_value);
        display_total_load_and_valuation(root->right, total_weight, total_value);
    }
}


void find_cheapest_and_most_expensive(Parcel* root, Parcel** cheapest, Parcel** most_expensive) {
    if (root != NULL) {
        if (*cheapest == NULL || root->value < (*cheapest)->value) {
            *cheapest = root;
        }
        if (*most_expensive == NULL || root->value > (*most_expensive)->value) {
            *most_expensive = root;
        }
        find_cheapest_and_most_expensive(root->left, cheapest, most_expensive);
        find_cheapest_and_most_expensive(root->right, cheapest, most_expensive);
    }
}


void find_lightest_and_heaviest(Parcel* root, Parcel** lightest, Parcel** heaviest) {
    if (root != NULL) {
        if (*lightest == NULL || root->weight < (*lightest)->weight) {
            *lightest = root;
        }
        if (*heaviest == NULL || root->weight > (*heaviest)->weight) {
            *heaviest = root;
        }
        find_lightest_and_heaviest(root->left, lightest, heaviest);
        find_lightest_and_heaviest(root->right, lightest, heaviest);
    }
}
=======
>>>>>>> 7
