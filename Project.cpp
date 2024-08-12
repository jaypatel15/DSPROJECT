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

void user_menu(HashTable* ht) {
    int choice;
    char country[30];
    double weight;
    unsigned long index;

    
    Parcel* cheapest = NULL, * most_expensive = NULL;
    Parcel* lightest = NULL, * heaviest = NULL;
    double total_weight = 0.0, total_value = 0.0;

    do {
        printf("\nMenu:\n");
        printf("1. Display all parcels for a country\n");
        printf("2. Display parcels heavier/lighter than a specific weight\n");
        printf("3. Display total load and valuation for a country\n");
        printf("4. Display cheapest and most expensive parcels\n");
        printf("5. Display lightest and heaviest parcels\n");
        printf("6. Exit\n");
        printf("Enter your choice: ");
        if (scanf("%d", &choice) != 1) {
            printf("Invalid input. Please enter a valid number.\n");
            while (getchar() != '\n');
            continue;
        }

        switch (choice) {
        case 1:
            printf("Enter country: ");
            if (scanf("%s", country) != 1) {
                printf("Invalid input.\n");
                while (getchar() != '\n');
                continue;
            }
            to_lowercase(country); 
            index = hash_function(country);
            display_all_parcels(ht->table[index]);
            break;
        case 2:
            printf("Enter country: ");
            if (scanf("%s", country) != 1) {
                printf("Invalid input.\n");
                while (getchar() != '\n'); 
                continue;
            }
            to_lowercase(country); 
            printf("Enter weight: ");
            if (scanf("%lf", &weight) != 1) {
                printf("Invalid input. Please enter a valid weight.\n");
                while (getchar() != '\n'); 
                continue;
            }

            int choice_weight;
            printf("Enter 1 to display parcels heavier than the weight, or 2 for lighter: ");
            if (scanf("%d", &choice_weight) != 1) {
                printf("Invalid input. Please enter 1 or 2.\n");
                while (getchar() != '\n'); 
                continue;
            }

            index = hash_function(country);
            if (choice_weight == 1) {
                display_parcels_by_weight(ht->table[index], weight, 1);  
            }
            else if (choice_weight == 2) {
                display_parcels_by_weight(ht->table[index], weight, 0);  
            }
            else {
                printf("Invalid choice.\n");
            }
            break;
        case 3:
            printf("Enter country: ");
            if (scanf("%s", country) != 1) {
                printf("Invalid input.\n");
                while (getchar() != '\n'); 
                continue;
            }
            to_lowercase(country); 
            index = hash_function(country);
            total_weight = 0.0;  
            total_value = 0.0;
            display_total_load_and_valuation(ht->table[index], &total_weight, &total_value);
            printf("Total weight: %.2f, Total value: %.2f\n", total_weight, total_value);
            break;
        case 4:
            printf("Enter country: ");
            if (scanf("%s", country) != 1) {
                printf("Invalid input.\n");
                while (getchar() != '\n'); 
                continue;
            }
            to_lowercase(country); 
            index = hash_function(country);
            cheapest = NULL;  
            most_expensive = NULL;
            find_cheapest_and_most_expensive(ht->table[index], &cheapest, &most_expensive);
            if (cheapest != NULL && most_expensive != NULL) {
                printf("Cheapest parcel: Country: %s, Weight: %.2f, Value: %.2f\n", cheapest->country, cheapest->weight, cheapest->value);
                printf("Most expensive parcel: Country: %s, Weight: %.2f, Value: %.2f\n", most_expensive->country, most_expensive->weight, most_expensive->value);
            }
            else {
                printf("No parcels found.\n");
            }
            break;
        case 5:
            printf("Enter country: ");
            if (scanf("%s", country) != 1) {
                printf("Invalid input.\n");
                while (getchar() != '\n'); 
                continue;
            }
            to_lowercase(country); 
            index = hash_function(country);
            lightest = NULL;  
            heaviest = NULL;
            find_lightest_and_heaviest(ht->table[index], &lightest, &heaviest);
            if (lightest != NULL && heaviest != NULL) {
                printf("Lightest parcel: Country: %s, Weight: %.2f, Value: %.2f\n", lightest->country, lightest->weight, lightest->value);
                printf("Heaviest parcel: Country: %s, Weight: %.2f, Value: %.2f\n", heaviest->country, heaviest->weight, heaviest->value);
            }
            else {
                printf("No parcels found.\n");
            }
            break;
        case 6:
            printf("Exiting...\n");
            break;
        default:
            printf("Invalid choice. Please try again.\n");
            while (getchar() != '\n'); 
        }
    } while (choice != 6);
}

void free_tree(Parcel* root) {
    if (root != NULL) {
        free_tree(root->left);
        free_tree(root->right);
        free(root->country);
        free(root);
    }
}


void free_hash_table(HashTable* ht) {
    for (int i = 0; i < 127; i++) {
        if (ht->table[i] != NULL) {
            free_tree(ht->table[i]);
        }
    }
}

int main() {
    HashTable ht;
    for (int i = 0; i < 127; i++) {
        ht.table[i] = NULL;
    }
    load_data(&ht, "couriers.txt");
    user_menu(&ht);
    free_hash_table(&ht);

    return 0;
}

void to_lowercase(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}
