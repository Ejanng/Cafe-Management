#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdlib.h>
#include <unistd.h>

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

    // Function to flush the input buffer
    void flushInputBuffer() {
        int c;
        while ((c = getchar()) != '\n' && c != EOF);
    }

    // Function to display loading animation
    void loadingAnimation() {
        printf("Loading ");
        fflush(stdout);
        for (int i = 0; i < 3; i++) {
            printf(".");
            fflush(stdout);
            usleep(500000); // Sleep for 0.5 seconds
        }
        printf("\n");
    }


void createMenuFile() {
    // Open the file for writing
    FILE *file = fopen(MENU_FILE, "w");
    if (!file) {
        // If the file cannot be opened, print an error message and return
        perror("Error creating menu file");
        return;
    }
    fclose(file);
}

void createSalesFile() {
    // Open the file for writing
    FILE *file = fopen(SALES_FILE, "w");
    if (!file) {
        // If the file cannot be opened, print an error message and return
        perror("Error creating menu file");
        return;
    }
    fclose(file);
}
    // --------------------------------------------
    // --            Load Function               --
    // --------------------------------------------

void loadMenu(struct Item items[], int *numItems) {
    // Open the file for reading
    FILE *file = fopen(MENU_FILE, "r");
    if (!file) {
        // If the file cannot be opened, print an error message and return
        perror("Error opening menu file");
        void createMenuFile();
        return;
    }

    // Read the items from the file
    *numItems = 0;
    loadingAnimation(); // Display loading animation
    while (fscanf(file, "%s %s %s %f %d\n", items[*numItems].code, items[*numItems].name,
            items[*numItems].temperature, &items[*numItems].price, &items[*numItems].quantitySold) == 5) {
        printf("\n\nLoading... \nLoaded item: %s %s %s %.2f %d\n", items[*numItems].code, items[*numItems].name,
            items[*numItems].temperature, items[*numItems].price, items[*numItems].quantitySold);
            fflush(stdout);
            usleep(10000); // Sleep for 0.5 seconds
            system("cls");
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
        void createMenuFile();
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
    printf("\n--------------------------------------------\n");
    printf("--             Adding Item...             -- \n");
    printf("--------------------------------------------\n");
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
    printf("\n--------------------------------------------\n");
    printf("--            Removing Item...            -- \n");
    printf("--------------------------------------------\n");
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
    printf("\n--------------------------------------------\n");
    printf("--            Editing Item...             -- \n");
    printf("--------------------------------------------\n");
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
    printf("\n--------------------------------------------\n");
    printf("--          Displaying Menu...            -- \n");
    printf("--------------------------------------------\n");
    char command[256];
    snprintf(command, sizeof(command), "start cmd /c \"type %s && pause\"", MENU_FILE);
    system(command);
}

    // --------------------------------------------
    // --          Log Transaction               --
    // --------------------------------------------

void logTransaction(struct Item item, int quantity, char size, float totalPrice);

    // --------------------------------------------
    // --        Calculate Change                --
    // --------------------------------------------

float calculateChange(float total, float payment);

    // --------------------------------------------
    // --           Sell Function                --
    // --------------------------------------------

void sellItems(struct Item items[], int numItems) {
    // Sell items to customers
    float total = 0.0;
    char code[10];
    float payment;

    printf("\n--------------------------------------------\n");
    printf("--             Selling Item...            -- \n");
    printf("--------------------------------------------\n");
    // Enter the item code and quantity
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

        // Enter the quantity of the item
        int quantity;
        printf("Enter quantity: ");
        scanf("%d", &quantity);

        float price = selectedItem->price;
        float totalPrice = 0.0;

        // Determine if the item is a drink or food based on the code
        if (selectedItem->code[0] != 'S') {  // Assuming food codes start with 'S'
            // For drinks, ask for size
            char size;
            printf("Enter size (S, M, L): ");
            scanf(" %c", &size);
            size = toupper(size);

            // Adjust price based on size
            if (size == 'M') {
                price += 20.0;
            } else if (size == 'L') {
                price += 30.0;
            }
            totalPrice = price * quantity;

            // Log the transaction with size
            logTransaction(*selectedItem, quantity, size, totalPrice);
        } else {
            // For food, no size adjustment
            totalPrice = price * quantity;

            // Log the transaction without size
            logTransaction(*selectedItem, quantity, '-', totalPrice);
        }

        total += totalPrice;
        selectedItem->quantitySold += quantity;

        // Print the order details
        printf("Added %d of %s to the order. Subtotal: %.2f\n", quantity, selectedItem->name, totalPrice);
    }

    // Print the total amount to pay
    printf("\n\nTotal amount to pay: %.2f\n\n", total);
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
    // --        Determine the Top 3             --
    // --------------------------------------------

void determineTop3(struct Item items[], int numItems, char top3[][50]);

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
    printf("Total Sales: %.2f\n\n", totalSales);

    // Print the total sales
    printf("\n--------------------------------------------\n");
    printf("--              Total Sales               -- \n");
    printf("--------------------------------------------\n\n");
    // Determine the top 3 best-selling
    char top3[3][50] = {"", "", ""};
    determineTop3(items, numItems, top3);

    printf("Top 3 best-sales:\n");
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
    // Create a copy of the items array
    struct Item sortedItems[MAX_ITEMS];
    for (int i = 0; i < numItems; i++) {
        sortedItems[i] = items[i];
    }

    // Sort the copied array by quantitySold
    for (int i = 0; i < numItems - 1; i++) {
        for (int j = 0; j < numItems - i - 1; j++) {
            if (sortedItems[j].quantitySold < sortedItems[j + 1].quantitySold) {
                // Swap the items
                struct Item temp = sortedItems[j];
                sortedItems[j] = sortedItems[j + 1];
                sortedItems[j + 1] = temp;
            }
        }
    }

    // Copy the top 3 best-selling items to the top3 array
    for (int i = 0; i < 3 && i < numItems; i++) {
        snprintf(top3[i], sizeof(top3[i]), "%s %s %s", sortedItems[i].code, sortedItems[i].name, sortedItems[i].temperature);
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
        void createSalesFile();
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
        // Check if input is valid
        if (scanf("%d", &choice) != 1) {
            // Clear invalid input from the buffer
            flushInputBuffer();
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        
        switch (choice) {
            case 1:
                // Add a new item to the menu
                system("cls");
                addItem(items, &numItems);
                break;
            case 2:
                // Remove an item from the menu
                system("cls");
                removeItem(items, &numItems);
                break;
            case 3:
                // Edit an item in the menu
                system("cls");
                editItem(items, numItems);
                break;
            case 4:
                // Display the menu in the command prompt
                system("cls");
                displayMenuInCmd();
                break;
            case 5:
                // Sell items to customers
                system("cls");
                sellItems(items, numItems);
                saveMenu(items, numItems);
                break;
            case 6:
                // Display the total sales and the top 3 best-selling drinks
                system("cls");
                displayTotalSales(items, numItems);
                break;
            case 7:
                // Save the menu to the file and exit the program
                system("cls");
                saveMenu(items, numItems);
                printf("\n--------------------------------------------\n");
                printf("--       Thank you for Ordering!!!        -- \n");
                printf("--------------------------------------------\n");
                exit(0);
            default:
                // Print an error message for an invalid choice
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
