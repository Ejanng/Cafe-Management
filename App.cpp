#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

    // -------------------------------------------- 
    // --            Constants                   -- 
    // --------------------------------------------
    
#define MENU_FILE "menu.txt"
#define SALES_FILE "sales.txt"
#define MAX_ITEMS 100
    
    // -------------------------------------------- 
    // --            Function Prototypes         -- 
    // --------------------------------------------

struct Item {
    char code[10];
    char name[50];
    char temperature[10];
    float price;
    int quantitySold;
};

struct Sale {
    char code[10];
    char name[50];
    char size;
    int quantity;
    float total;
};

    // -------------------------------------------- 
    // --            Load Function               -- 
    // --------------------------------------------

void loadMenu(struct Item items[], int *numItems) {
    // Open the file for reading
    FILE *file = fopen(MENU_FILE, "r");
    if (!file) {
        // If the file cannot be opened, print an error message and return
        perror("Error opening menu file");
        return;
    }

    // Read the items from the file
    *numItems = 0;
    while (fscanf(file, "%s %s %s %f %d\n", items[*numItems].code, items[*numItems].name,
            items[*numItems].temperature, &items[*numItems].price, &items[*numItems].quantitySold) == 5) {
        // printf("Loaded item: %s %s %s %.2f %d\n", items[*numItems].code, items[*numItems].name,
        //     items[*numItems].temperature, items[*numItems].price, items[*numItems].quantitySold);
        (*numItems)++;
        // If the maximum number of items has been reached, stop reading
        if (*numItems >= MAX_ITEMS) {
            printf("Maximum number of items exceeded.\n");
            break;
        }
    }

    fclose(file);
}

    // -------------------------------------------- 
    // --            Save Function               -- 
    // --------------------------------------------

void saveMenu(struct Item items[], int numItems) {
    // Open the file for writing
    FILE *file = fopen(MENU_FILE, "w");
    // if the file cannot be opened, print an error message and return
    if (!file) {
        perror("Error opening menu file");
        return;
    }
     // Print the items to the file
    for (int i = 0; i < numItems; i++) {
        fprintf(file, "%s %s %s %.2f %d\n", items[i].code, items[i].name, items[i].temperature, items[i].price, items[i].quantitySold);
    }

    fclose(file);
}

    // -------------------------------------------- 
    // --             Add Function               -- 
    // -------------------------------------------- 

void addItem(struct Item items[], int *numItems) {
    // Add a new item to the menu
    struct Item newItem;
    printf("Enter code: ");
    scanf("%s", newItem.code);

    // Check if the code starts with a number
    if (isdigit(newItem.code[0])) {
        printf("Invalid code: Item code cannot start with a number. Please enter a valid code.\n");
        return;
    }

    // Ignore the code if it is '#'
    if (strcmp(newItem.code, "#") != 0) {
        // Check if the code is already taken
        for (int i = 0; i < *numItems; i++) {
            if (strcmp(items[i].code, newItem.code) == 0) {
                printf("Error: Code is already taken. Please enter a different code.\n");
                return;
            }
        }
    }

    // Enter the name, temperature, and price of the new item
    printf("Enter name: ");
    scanf("%s", newItem.name);
    printf("Enter temperature: ");
    scanf("%s", newItem.temperature);
    printf("Enter price: ");
    scanf("%f", &newItem.price);
    newItem.quantitySold = 0;

    // Add the new item to the menu
    items[*numItems] = newItem;
    (*numItems)++;

    // Save the menu to the file
    saveMenu(items, *numItems);
    printf("Item added successfully!\n");
}

    // -------------------------------------------- 
    // --           Remove Function              -- 
    // --------------------------------------------

void removeItem(struct Item items[], int *numItems) {
    // Remove an item from the menu
    char code[10];
    printf("Enter code of item to remove: ");
    scanf("%s", code);

    // Check if the code starts with a number
    if (isdigit(code[0])) {
        printf("Invalid code: Item code cannot start with a number. Please enter a valid code.\n");
        return;
    }

    // Find the index of the item with the given code
    int index = -1;
    for (int i = 0; i < *numItems; i++) {
        if (strcmp(items[i].code, code) == 0) {
            index = i;
            break;
        }
    }

    // If the item is not found, print an error message and return
    if (index == -1) {
        printf("Item not found.\n");
        return;
    }

    // Remove the item from the menu
    for (int i = index; i < *numItems - 1; i++) {
        items[i] = items[i + 1];
    }

    // Decrease the number of items
    (*numItems)--;
    saveMenu(items, *numItems);
    printf("Item removed successfully!\n");
}

    // -------------------------------------------- 
    // --          Edit Function                 -- 
    // --------------------------------------------

void editItem(struct Item items[], int numItems) {
    // Edit an item in the menu
    char code[10];
    printf("Enter code of item to edit: ");
    scanf("%s", code);

    // Check if the code starts with a number
    if (isdigit(code[0])) {
        printf("Invalid code: Item code cannot start with a number. Please enter a valid code.\n");
        return;
    }

    // Find the item with the given code
    struct Item *selectedItem = NULL;
    for (int i = 0; i < numItems; i++) {
        if (strcmp(items[i].code, code) == 0) {
            selectedItem = &items[i];
            break;
        }
    }

    // If the item is not found, print an error message and return
    if (selectedItem == NULL) {
        printf("Item not found.\n");
        return;
    }

    // Enter the new name, temperature, and price of the item
    printf("Enter new name: ");
    scanf("%s", selectedItem->name);
    printf("Enter new temperature: ");
    scanf("%s", selectedItem->temperature);
    printf("Enter new price: ");
    scanf("%f", &selectedItem->price);
    
    // Save the menu to the file
    saveMenu(items, numItems);
    printf("Item edited successfully!\n");
}

    // -------------------------------------------- 
    // --       Display Menu Function            -- 
    // --------------------------------------------

void displayMenuInCmd() {
    // Display the menu in the command prompt
    char command[256];
    snprintf(command, sizeof(command), "start cmd /c \"type %s && pause\"", MENU_FILE);
    system(command);
}

    // -------------------------------------------- 
    // --           Sell Function                -- 
    // --------------------------------------------

void sellItems(struct Item items[], int numItems) {
    // Sell items to customers
    float total = 0.0;
    char code[10];
    float payment;

    // Enter the item code, quantity, and size
    while (1) {
        printf("\n**************\nEnter item code (or 'EX' to finish): ");
        scanf("%s", code);
        
        // If the code is 'EX', finish the order
        if (strcmp(code, "EX") == 0) {
            break;
        }

        // Check if the code starts with a number
        if (isdigit(code[0])) {
            printf("Invalid item code: %s. Item code cannot start with a number. Please try again.\n", code);
            continue;
        }

        // Find the item with the given code
        struct Item *selectedItem = NULL;
        for (int i = 0; i < numItems; i++) {
            if (strcmp(items[i].code, code) == 0) {
                selectedItem = &items[i];
                break;
            }
        }

        // If the item is not found, print an error message and continue
        if (selectedItem == NULL) {
            printf("Invalid item code: %s. Please try again.\n", code);
            continue;
        }

        // Enter the quantity and size of the item
        int quantity;
        printf("Enter quantity: ");
        scanf("%d", &quantity);

        // Check if the quantity is valid
        char size;
        printf("Enter size (S, M, L): ");
        scanf(" %c", &size);
        size = toupper(size);
        
        // Check if the size is valid
        float price = selectedItem->price;
        if (size == 'M') {
            price += 20.0;
        } else if (size == 'L') {
            price += 30.0;
        }

        // Calculate the total price of the item
        float totalPrice = price * quantity;
        total += totalPrice;
        // Log the transaction
        logTransaction(*selectedItem, quantity, size, totalPrice);
        selectedItem->quantitySold += quantity;
        // Print the order details
        printf("Added %d of %s to the order. Subtotal: %.2f\n", quantity, selectedItem->name, totalPrice);
    }

    // Print the total amount to pay
    printf("Total amount to pay: %.2f\n", total);
    // Enter the payment amount
    while (1) {
        printf("Enter payment amount: ");
        scanf("%f", &payment);

        // Check if the payment is sufficient
        if (payment < total) {
            printf("Insufficient payment. Please enter an amount of at least %.2f.\n", total);
        } else {
            float change = calculateChange(total, payment);
            printf("Change: %.2f\n", change);
            break;
        }
    }
}

    // -------------------------------------------- 
    // --        Calculate Change                -- 
    // --------------------------------------------

float calculateChange(float total, float payment) {
    // Calculate the change to return to the customer
    return payment - total;
}

    // -------------------------------------------- 
    // --        Display Total Sales             -- 
    // --------------------------------------------

void displayTotalSales(struct Item items[], int numItems) {
    // Display the total sales and the top 3 best-selling drinks
    float totalSales = 0.0;

    for (int i = 0; i < numItems; i++) {
        // Calculate the total sales of each item
        totalSales += items[i].price * items[i].quantitySold;
    }

    // Print the total sales
    printf("Total Sales: %.2f\n", totalSales);
    // Determine the top 3 best-selling
    char top3[3][50] = {"", "", ""};
    determineTop3(items, numItems, top3);

    printf("Top 3 best-selling drinks:\n");
    for (int i = 0; i < 3 && strlen(top3[i]) > 0; i++) {
        // Print the top 3 best-selling
        printf("%d. %s\n", i + 1, top3[i]);
    }

    // Display the sales history
    printf("\nSales History:\n");
    FILE *salesFile = fopen(SALES_FILE, "r");
    // If the file cannot be opened, print an error message and return
    if (!salesFile) {
        perror("Error opening sales file");
        return;
    }

    // Read the sales history from the file
    char line[100];
    while (fgets(line, sizeof(line), salesFile)) {
        printf("%s", line);
    }

    fclose(salesFile);
}

    // -------------------------------------------- 
    // --        Determine the Top 3             -- 
    // --------------------------------------------

void determineTop3(struct Item items[], int numItems, char top3[][50]) {
    // Determine the top 3 best-selling
    for (int i = 0; i < numItems - 1; i++) {
        for (int j = 0; j < numItems - i - 1; j++) {
            if (items[j].quantitySold < items[j + 1].quantitySold) {
                // Swap the items
                struct Item temp = items[j];
                items[j] = items[j + 1];
                items[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < 3 && i < numItems; i++) {
        // Print the top 3 best-selling
        snprintf(top3[i], sizeof(top3[i]), "%s %s %s", items[i].code, items[i].name, items[i].temperature);
    }
}

    // -------------------------------------------- 
    // --          Log Transaction               -- 
    // --------------------------------------------

void logTransaction(struct Item item, int quantity, char size, float totalPrice) {
    // Log the transaction to the sales file
    FILE *file = fopen(SALES_FILE, "a");
    if (!file) {
        // If the file cannot be opened, print an error message and return
        perror("Error opening sales file");
        return;
    }

    // Print the transaction details to the file
    fprintf(file, "Code: %s, Name: %s, Size: %c, Quantity: %d, Total: %.2f\n", item.code, item.name, size, quantity, totalPrice);

    fclose(file);
}

    // -------------------------------------------- 
    // --           Main Function                -- 
    // --------------------------------------------

int main() {
    struct Item items[100];
    int numItems = 0;
    loadMenu(items, &numItems);

    printf("Welcome to the cashier system!\n");

    // Display the menu in the command prompt
    while (1) {
        int choice;
        printf("\n1. Add item\n2. Remove item\n3. Edit item\n4. Display menu\n5. Sell items\n6. Display total sales\n7. Exit\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                // Add a new item to the menu
                addItem(items, &numItems);
                break;
            case 2:
                // Remove an item from the menu
                removeItem(items, &numItems);
                break;
            case 3:
                // Edit an item in the menu
                editItem(items, numItems);
                break;
            case 4:
                // Display the menu in the command prompt
                displayMenuInCmd();
                break;
            case 5:
                // Sell items to customers
                sellItems(items, numItems);
                saveMenu(items, numItems);
                break;
            case 6:
                // Display the total sales and the top 3 best-selling drinks
                displayTotalSales(items, numItems);
                break;
            case 7:
                // Save the menu to the file and exit the program
                saveMenu(items, numItems);
                printf("Goodbye!\n");
                exit(0);
            default:
                // Print an error message for an invalid choice
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
