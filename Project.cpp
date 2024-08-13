/*
* FILE : project.cpp
* PROJECT : SENG1050 - project
* PROGRAMMER : Prachi Patel, Jay Patel
* FIRST VERSION : 2024-08-12
* DESCRIPTION : This program manages parcel tracking using a hash table and binary search tree, enabling queries and display of parcel details based on country, weight, and value.
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#pragma warning(disable : 4996)

// Structure for each parcel
typedef struct Parcel {
    char* country;
    double weight;
    double value;
    struct Parcel* left, * right;
} Parcel;

// Hash table with 127 buckets, each holding the root of a BST
typedef struct HashTable {
    Parcel* table[127];
} HashTable;



/*
 * Function: hash_function()
 *
 * Description: Generates a hash value using the djb2 algorithm.
 *
 * Parameters: const char* str : The input string to hash.
 *
 * Return Value: An unsigned long representing the hash value.
 */

unsigned long hash_function(const char* str) {
    unsigned long hash = 5381;
    int c;
    while ((c = *str++)) {
        hash = ((hash << 5) + hash) + c;
    }
    return hash % 127;
}



/*
 * Function: create_node()
 *
 * Description: Creates a new Parcel node with the given country, weight, and value.
 *
 * Parameters: const char* country : The country of the parcel.
 *             double weight : The weight of the parcel.
 *             double value : The value of the parcel.
 *
 * Return Value: A pointer to the newly created Parcel node.
 */

Parcel* create_node(const char* country, double weight, double value) {
    Parcel* newNode = (Parcel*)malloc(sizeof(Parcel));
    if (newNode == NULL) {
        fprintf(stderr, "Memory allocation failed for new node.\n");
        exit(EXIT_FAILURE);
    }

    // Ensure the country name does not exceed 20 characters
    size_t len = strlen(country);
    if (len > 20) {
        len = 20;  // Limit to 20 characters
    }

    newNode->country = (char*)malloc(len + 1);
    if (newNode->country == NULL) {
        fprintf(stderr, "Memory allocation failed for country name.\n");
        free(newNode);
        exit(EXIT_FAILURE);
    }
    strncpy(newNode->country, country, len);
    newNode->country[len] = '\0';  // Null-terminate the string

    newNode->weight = weight;
    newNode->value = value;
    newNode->left = newNode->right = NULL;
    return newNode;
}


/*
 * Function: insert()
 *
 * Description: Inserts a new Parcel node into the BST based on the weight.
 *
 * Parameters: Parcel* root : The root of the BST.
 *             const char* country : The country of the parcel.
 *             double weight : The weight of the parcel.
 *             double value : The value of the parcel.
 *
 * Return Value: A pointer to the root of the BST after insertion.
 */

Parcel* insert(Parcel* root, const char* country, double weight, double value) {
    if (root == NULL) return create_node(country, weight, value);
    if (weight < root->weight)
        root->left = insert(root->left, country, weight, value);
    else
        root->right = insert(root->right, country, weight, value);
    return root;
}

/*
 * Function: in_order()
 *
 * Description: Performs an in-order traversal of the BST and displays parcel details.
 *
 * Parameters: Parcel* root : The root of the BST.
 *
 * Return Value: None
 */

void in_order(Parcel* root) {
    if (root != NULL) {
        in_order(root->left);
        printf("Country: %s, Weight: %.2f, Value: %.2f\n", root->country, root->weight, root->value);
        in_order(root->right);
    }
}

/*
 * Function: insert_into_hash_table()
 *
 * Description: Inserts a new parcel into the hash table using the country as the key.
 *
 * Parameters: HashTable* ht : The hash table.
 *             const char* country : The country of the parcel.
 *             double weight : The weight of the parcel.
 *             double value : The value of the parcel.
 *
 * Return Value: None
 */

void insert_into_hash_table(HashTable* ht, const char* country, double weight, double value) {
    unsigned long index = hash_function(country);
    ht->table[index] = insert(ht->table[index], country, weight, value);
}
/*
 * Function: load_data()
 *
 * Description: Loads parcel data from a file and inserts it into the hash table.
 *
 * Parameters: HashTable* ht : The hash table.
 *             const char* filename : The name of the file containing parcel data.
 *
 * Return Value: None
 */
void load_data(HashTable* ht, const char* filename) {
    FILE* file = fopen(filename, "r");
    if (!file) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }

    char buffer[100];  // Buffer to hold each line
    char country[30];
    double weight = 0.0, value = 0.0;
    char* token;

    while (fgets(buffer, sizeof(buffer), file)) {
        // Remove newline character if present
        buffer[strcspn(buffer, "\n")] = '\0';

        // Extract the country
        token = strtok(buffer, ",");
        if (token != NULL) {
            strncpy(country, token, sizeof(country) - 1);
            country[sizeof(country) - 1] = '\0';
            to_lowercase(country);  // Normalize to lowercase before inserting into the hash table
        }

        // Extract the weight
        token = strtok(NULL, ",");
        if (token != NULL) {
            weight = atof(token);

            // Validate weight range
            if (weight < 100 || weight > 50000) {
                fprintf(stderr, "Invalid weight: %.2f. Skipping entry.\n", weight);
                continue;
            }
        }

        // Extract the value
        token = strtok(NULL, ",");
        if (token != NULL) {
            value = atof(token);

            // Validate value range
            if (value < 10 || value > 2000) {
                fprintf(stderr, "Invalid value: %.2f. Skipping entry.\n", value);
                continue;
            }
        }

        // Insert the parsed values into the hash table
        insert_into_hash_table(ht, country, weight, value);
    }

    fclose(file);
}




/*
 * Function: display_all_parcels()
 *
 * Description: Displays all parcels stored in the BST for a specific country.
 *
 * Parameters: Parcel* root : The root of the BST.
 *
 * Return Value: None
 */

void display_all_parcels(Parcel* root) {
    if (root != NULL) {
        in_order(root);
    }
    else {
        printf("No parcels found.\n");
    }
}

/*
 * Function: display_parcels_by_weight()
 *
 * Description: Displays parcels in the BST that are either heavier or lighter than a specific weight.
 *
 * Parameters: Parcel* root : The root of the BST.
 *             double weight : The reference weight for comparison.
 *             int higher : A flag indicating whether to display parcels heavier (1) or lighter (0) than the reference weight.
 *
 * Return Value: None
 */

void display_parcels_by_weight(Parcel* root, double weight, int higher) {
    if (root == NULL) return;

    display_parcels_by_weight(root->left, weight, higher);

    if ((higher && root->weight > weight) || (!higher && root->weight < weight)) {
        printf("Country: %s, Weight: %.2f, Value: %.2f\n", root->country, root->weight, root->value);
    }

    display_parcels_by_weight(root->right, weight, higher);
}

/*
 * Function: display_total_load_and_valuation()
 *
 * Description: Calculates and displays the total weight and value of all parcels for a specific country.
 *
 * Parameters: Parcel* root : The root of the BST.
 *             double* total_weight : Pointer to the total weight variable.
 *             double* total_value : Pointer to the total value variable.
 *
 * Return Value: None
 */

void display_total_load_and_valuation(Parcel* root, double* total_weight, double* total_value) {
    if (root != NULL) {
        *total_weight += root->weight;
        *total_value += root->value;
        display_total_load_and_valuation(root->left, total_weight, total_value);
        display_total_load_and_valuation(root->right, total_weight, total_value);
    }
}

/*
 * Function: find_cheapest_and_most_expensive()
 *
 * Description: Finds and displays the cheapest and most expensive parcels in the BST.
 *
 * Parameters: Parcel* root : The root of the BST.
 *             Parcel** cheapest : Pointer to the Parcel pointer that will hold the cheapest parcel.
 *             Parcel** most_expensive : Pointer to the Parcel pointer that will hold the most expensive parcel.
 *
 * Return Value: None
 */

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

/*
 * Function: find_lightest_and_heaviest()
 *
 * Description: Finds and displays the lightest and heaviest parcels in the BST.
 *
 * Parameters: Parcel* root : The root of the BST.
 *             Parcel** lightest : Pointer to the Parcel pointer that will hold the lightest parcel.
 *             Parcel** heaviest : Pointer to the Parcel pointer that will hold the heaviest parcel.
 *
 * Return Value: None
 */

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

/*
 * Function: user_menu()
 *
 * Description: Provides a user interface to interact with the parcel management system.
 *
 * Parameters: HashTable* ht : The hash table containing all parcels.
 *
 * Return Value: None
 */

void user_menu(HashTable* ht) {
    int choice;
    char country[30];
    double weight;
    unsigned long index;

    // Declare these variables before the switch statement
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
            while (getchar() != '\n'); // clear the input buffer
            continue;
        }

        switch (choice) {
        case 1:
            printf("Enter country: ");
            if (scanf("%s", country) != 1) {
                printf("Invalid input.\n");
                while (getchar() != '\n'); // clear the input buffer
                continue;
            }
            to_lowercase(country); // Convert to lowercase
            index = hash_function(country);
            display_all_parcels(ht->table[index]);
            break;
        case 2:
            printf("Enter country: ");
            if (scanf("%s", country) != 1) {
                printf("Invalid input.\n");
                while (getchar() != '\n'); // clear the input buffer
                continue;
            }
            to_lowercase(country); // Convert to lowercase
            printf("Enter weight: ");
            if (scanf("%lf", &weight) != 1) {
                printf("Invalid input. Please enter a valid weight.\n");
                while (getchar() != '\n'); // clear the input buffer
                continue;
            }

            int choice_weight;
            printf("Enter 1 to display parcels heavier than the weight, or 2 for lighter: ");
            if (scanf("%d", &choice_weight) != 1) {
                printf("Invalid input. Please enter 1 or 2.\n");
                while (getchar() != '\n'); // clear the input buffer
                continue;
            }

            index = hash_function(country);
            if (choice_weight == 1) {
                display_parcels_by_weight(ht->table[index], weight, 1);  // Heavier
            }
            else if (choice_weight == 2) {
                display_parcels_by_weight(ht->table[index], weight, 0);  // Lighter
            }
            else {
                printf("Invalid choice.\n");
            }
            break;
        case 3:
            printf("Enter country: ");
            if (scanf("%s", country) != 1) {
                printf("Invalid input.\n");
                while (getchar() != '\n'); // clear the input buffer
                continue;
            }
            to_lowercase(country); // Convert to lowercase
            index = hash_function(country);
            total_weight = 0.0;  // Reinitialize variables
            total_value = 0.0;
            display_total_load_and_valuation(ht->table[index], &total_weight, &total_value);
            printf("Total weight: %.2f, Total value: %.2f\n", total_weight, total_value);
            break;
        case 4:
            printf("Enter country: ");
            if (scanf("%s", country) != 1) {
                printf("Invalid input.\n");
                while (getchar() != '\n'); // clear the input buffer
                continue;
            }
            to_lowercase(country); // Convert to lowercase
            index = hash_function(country);
            cheapest = NULL;  // Reinitialize variables
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
                while (getchar() != '\n'); // clear the input buffer
                continue;
            }
            to_lowercase(country); // Convert to lowercase
            index = hash_function(country);
            lightest = NULL;  // Reinitialize variables
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
            while (getchar() != '\n'); // clear the input buffer
        }
    } while (choice != 6);
}


/*
 * Function: free_tree()
 *
 * Description: Frees all nodes in the BST recursively.
 *
 * Parameters: Parcel* root : The root of the BST.
 *
 * Return Value: None
 */

void free_tree(Parcel* root) {
    if (root != NULL) {
        free_tree(root->left);
        free_tree(root->right);
        free(root->country);
        free(root);
    }
}

/*
 * Function: free_hash_table()
 *
 * Description: Frees all BSTs stored in the hash table.
 *
 * Parameters: HashTable* ht : The hash table containing all parcels.
 *
 * Return Value: None
 */

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

    // Load data from file
    load_data(&ht, "couriers.txt");

    // Display user menu
    user_menu(&ht);

    // Clean up memory
    free_hash_table(&ht);

    return 0;
}

/*
 * Function: to_lowercase()
 *
 * Description: Converts a string to lowercase.
 *
 * Parameters: char* str : The string to be converted.
 *
 * Return Value: None
 */

void to_lowercase(char* str) {
    for (int i = 0; str[i]; i++) {
        str[i] = tolower(str[i]);
    }
}
