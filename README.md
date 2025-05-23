SENG1050 - Data Structures Project Checklist
1. Programming / Functionality
- Implement a hash table with 127 buckets.
- Each bucket should contain a pointer to the root of a binary search tree (BST).
- Dynamically allocate memory for the country names.
- Implement the djb2 hash function to generate unique hash values using country names.
- Load data from couriers.txt into the hash table and BST.
- Ensure that each node in the BST represents a parcel and is placed based on the parcel's weight.
- Implement error checking and handle any errors appropriately.
- Clean up all allocated memory before exiting the program.
2. Prototype
- Create the user menu with the following options:
- Enter country name and display all parcel details.
-  Enter country and weight pair, display parcels with weight higher/lower than the entered weight.
-  Display the total parcel load and valuation for the country.
-  Display the cheapest and most expensive parcel details for a country.
-  Display the lightest and heaviest parcel for the country.
-  Exit the application.
- Ensure that no unnecessary code duplication occurs.
3. Value Constraints
- Ensure that the destination country name does not exceed 20 characters.
- Validate the weight (100-50000 grams) and valuation ($10-$2000) before inserting data into the hash tabl- Ensure no duplicate weight exists for the same destination.
4. Commenting
- Adhere to the SET Coding Standards for commenting.
- Provide clear comments explaining the purpose of each function and important blocks of code.
- Comment on memory allocation and deallocation processes.
5. Checklist
- Complete the requirements checklist in PDF form as checklist.pdf.
- Include specific assignment requirements, relevant lecture requirements, and SET Coding Standards in th6. File Naming
- Name the source file as project.cpp.
- Name the checklist file as checklist.pdf.
- Use the provided sample data file couriers.txt.
