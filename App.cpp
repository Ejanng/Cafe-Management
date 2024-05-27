#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <unistd.h>

#define MENU_FILE "menu.txt"
#define SALES_FILE "sales.txt"
#define MAX_ITEMS 100

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

void flushInputBuffer() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

void loadingAnimation() {
    printf("Loading ");
    fflush(stdout);
    for (int i = 0; i < 3; i++) {
        printf(".");
        fflush(stdout);
        usleep(500000);
    }
    printf("\n");
}

void createMenuFile() {
    FILE *file = fopen(MENU_FILE, "w");
    if (!file) {
        perror("Error creating menu file");
        return;
    }
    fclose(file);
}

void createSalesFile() {
    FILE *file = fopen(SALES_FILE, "w");
    if (!file) {
        perror("Error creating sales file");
        return;
    }
    fclose(file);
}

void loadMenu(struct Item items[], int *numItems) {
    FILE *file = fopen(MENU_FILE, "r");
    if (!file) {
        perror("Error opening menu file");
        createMenuFile();
        return;
    }

    *numItems = 0;
    loadingAnimation();
    while (fscanf(file, "%[^,], %[^,], %[^,], %f, %d\n", items[*numItems].code, items[*numItems].name,
            items[*numItems].temperature, &items[*numItems].price, &items[*numItems].quantitySold) == 5) {
        printf("\n\nLoading... \nLoaded item: %s %s %s %.2f %d\n", items[*numItems].code, items[*numItems].name,
            items[*numItems].temperature, items[*numItems].price, items[*numItems].quantitySold);
            fflush(stdout);
            usleep(10000);
            system("cls");
        (*numItems)++;
        if (*numItems >= MAX_ITEMS) {
            printf("Maximum number of items exceeded.\n");
            break;
        }
    }

    fclose(file);
}

void saveMenu(struct Item items[], int numItems) {
    FILE *file = fopen(MENU_FILE, "w");
    if (!file) {
        perror("Error opening menu file");
        createMenuFile();
        return;
    }

    for (int i = 0; i < numItems; i++) {
        fprintf(file, "%s, %s, %s, %.2f, %d\n", items[i].code, items[i].name, items[i].temperature, items[i].price, items[i].quantitySold);
    }

    fclose(file);
}

void updateSales(struct Sale sales[], int numSales, const char *oldCode, const char *newCode, const char *newName, float newPrice) {
    for (int i = 0; i < numSales; i++) {
        if (strcmp(sales[i].code, oldCode) == 0) {
            strcpy(sales[i].code, newCode);
            strcpy(sales[i].name, newName);
            sales[i].total = newPrice * sales[i].quantity;
        }
    }
}

void readSales(const char *filename, struct Sale sales[], int *numSales) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Could not open file");
        createSalesFile();
        return;
    }

    *numSales = 0;
    while (fscanf(file, "Code: %[^,], Name: %[^,], Size: %c, Quantity: %d, Total: %f\n",
                  sales[*numSales].code,
                  sales[*numSales].name,
                  &sales[*numSales].size,
                  &sales[*numSales].quantity,
                  &sales[*numSales].total) == 5) {
        (*numSales)++;
        if (*numSales >= MAX_ITEMS) {
            break; // To avoid overflow
        }
    }

    fclose(file);
}

void readMenu(const char *filename, struct Item items[], int *numItems) {
    FILE *file = fopen(filename, "r");
    if (!file) {
        perror("Could not open file");
        exit(EXIT_FAILURE);
    }

    *numItems = 0;
    while (fscanf(file, "%[^,], %[^,], %[^,], %f, %d\n",
                  items[*numItems].code,
                  items[*numItems].name,
                  items[*numItems].temperature,
                  &items[*numItems].price,
                  &items[*numItems].quantitySold) == 5) {
        (*numItems)++;
        if (*numItems >= MAX_ITEMS) {
            break; // To avoid overflow
        }
    }

    fclose(file);
}

void removeItem(struct Item items[], int *numItems) {
    char code[10];
    printf("\n--------------------------------------------\n");
    printf("--            Removing Item...            -- \n");
    printf("--------------------------------------------\n");
    printf("Enter code of item to remove: ");
    scanf("%s", code);

    if (isdigit(code[0])) {
        printf("Invalid code: Item code cannot start with a number. Please enter a valid code.\n");
        return;
    }

    int index = -1;
    for (int i = 0; i < *numItems; i++) {
        if (strcmp(items[i].code, code) == 0) {
            index = i;
            break;
        }
    }

    if (index == -1) {
        printf("Item not found.\n");
        return;
    }

    for (int i = index; i < *numItems - 1; i++) {
        items[i] = items[i + 1];
    }

    (*numItems)--;
    saveMenu(items, *numItems);
    printf("Item removed successfully!\n");
    usleep(5000000);
    system("cls");
}

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
        usleep(5000000);
        system("cls");
        return;
    }

    // Ignore the code if it is '#'
    if (strcmp(newItem.code, "#") != 0) {
        // Check if the code is already taken
        for (int i = 0; i < *numItems; i++) {
            if (strcmp(items[i].code, newItem.code) == 0) {
                printf("Error: Code is already taken. Please enter a different code.\n");
                usleep(5000000);
                system("cls");
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
    usleep(5000000);
    system("cls");
}

void editItem(struct Item items[], int numItems) {
    char code[10];
    printf("\n--------------------------------------------\n");
    printf("--            Editing Item...             -- \n");
    printf("--------------------------------------------\n");
    printf("Enter code of item to edit: ");
    scanf("%s", code);

    struct Item *selectedItem = NULL;
    for (int i = 0; i < numItems; i++) {
        if (strcmp(items[i].code, code) == 0) {
            selectedItem = &items[i];
            break;
        }
    }

    if (selectedItem == NULL) {
        printf("Item not found.\n");
        usleep(5000000);
        system("cls");
        return;
    }

    int choice;
    while (1) {
        printf("What do you want to edit?\n1. Name\n2. Temperature\n3. Price\n4. Done\nChoice: ");
        if (scanf("%d", &choice) != 1) {
            flushInputBuffer();
            printf("Invalid input. Please enter a number.\n");
            continue;
        }
        flushInputBuffer();

        switch (choice) {
            case 1:
                printf("Enter new name: ");
                scanf(" %[^\n]s", selectedItem->name);
                break;
            case 2:
                printf("Enter new temperature: ");
                scanf("%s", selectedItem->temperature);
                break;
            case 3:
                printf("Enter new price: ");
                scanf("%f", &selectedItem->price);
                break;
            case 4:
                saveMenu(items, numItems);
                printf("Item edited successfully!\n");
                usleep(5000000);
                system("cls");
                return;
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }
}

void displayMenuInCmd() {
    printf("\n--------------------------------------------\n");
    printf("--          Displaying Menu...            -- \n");
    printf("--------------------------------------------\n");
    char command[256];
    snprintf(command, sizeof(command), "start cmd /c \"type %s && pause\"", MENU_FILE);
    system(command);
}

void logTransaction(struct Item item, int quantity, char size, float totalPrice);

float calculateChange(float total, float payment) {
    return payment - total;
}

void sellItems(struct Item items[], int numItems) {
    float total = 0.0;
    char code[10];
    float payment;

    printf("\n--------------------------------------------\n");
    printf("--             Selling Item...            -- \n");
    printf("--------------------------------------------\n");
    while (1) {
        printf("\n**************\nEnter item code (or 'EX' to finish): ");
        scanf("%s", code);

        if (strcmp(code, "EX") == 0) {
            break;
        }

        if (isdigit(code[0])) {
            printf("Invalid item code: %s. Item code cannot start with a number. Please try again.\n", code);
            continue;
        }

        struct Item *selectedItem = NULL;
        for (int i = 0; i < numItems; i++) {
            if (strcmp(items[i].code, code) == 0) {
                selectedItem = &items[i];
                break;
            }
        }

        if (selectedItem == NULL) {
            printf("Invalid item code: %s. Please try again.\n", code);
            continue;
        }

        int quantity;
        printf("Enter quantity: ");
        scanf("%d", &quantity);

        float price = selectedItem->price;
        float totalPrice = 0.0;

        if (selectedItem->code[0] != 'S') {
            char size;
            printf("Enter size (S, M, L): ");
            scanf(" %c", &size);
            size = toupper(size);

            if (size == 'M') {
                price += 20.0;
            } else if (size == 'L') {
                price += 30.0;
            }
            totalPrice = price * quantity;
            logTransaction(*selectedItem, quantity, size, totalPrice);
        } else {
            totalPrice = price * quantity;
            logTransaction(*selectedItem, quantity, '-', totalPrice);
        }

        total += totalPrice;
        selectedItem->quantitySold += quantity;
        printf("Added %d of %s to the order. Subtotal: %.2f\n", quantity, selectedItem->name, totalPrice);
    }

    printf("\n\nTotal amount to pay: %.2f\n\n", total);
    while (1) {
        printf("Enter payment amount: ");
        int result = scanf("%f", &payment);

        // Check if input is valid
        if (result != 1) {
            flushInputBuffer(); // Clear the invalid input from the buffer
            printf("Invalid input. Please enter a number.\n");
            continue;
        }

        if (payment < total) {
            printf("Insufficient payment. Please enter an amount of at least %.2f.\n", total);
        } else {
            float change = calculateChange(total, payment);
            printf("Change: %.2f\n", change);
            break;
        }
    }

    usleep(5000000);
    system("cls");
}

void determineTop3(struct Item items[], int numItems, char top3[][50]);

float calculateTotalSales(struct Sale sales[], int numSales) {
    float totalSales = 0.0;

    for (int i = 0; i < numSales; i++) {
        totalSales += sales[i].total;
    }

    return totalSales;
}

void displayTotalSales(struct Item items[], int numItems) {
    struct Sale sales[MAX_ITEMS];
    int numSales;

    readSales("sales.txt", sales, &numSales);

    float totalSales = calculateTotalSales(sales, numSales);

    printf("\n--------------------------------------------\n");
    printf("--              Total Sales               -- \n");
    printf("--------------------------------------------\n\n");
    printf("Total Sales: %.2f\n\n", totalSales);

    char top3[3][50];
    determineTop3(items, numItems, top3);

    printf("Top 3 Best Selling Items:\n");
    for (int i = 0; i < 3; i++) {
        if (top3[i][0] != '\0') {
            printf("%d. %s\n", i + 1, top3[i]);
        }
    }

    printf("\nSales History:\n");
    for (int i = 0; i < numSales; i++) {
        printf("Code: %s, Name: %s, Size: %c, Quantity: %d, Total: %.2f\n",
               sales[i].code, sales[i].name, sales[i].size, sales[i].quantity, sales[i].total);
    }

    printf("\n**************\nEnter any key to Exit!\n");
    char exit;
    scanf("%s", &exit);
}

void determineTop3(struct Item items[], int numItems, char top3[][50]) {
    struct Item sortedItems[MAX_ITEMS];
    int actualItems = 0;

    // Copy items to sortedItems and count items with quantitySold > 0
    for (int i = 0; i < numItems; i++) {
        if (items[i].quantitySold > 0) {
            sortedItems[actualItems++] = items[i];
        }
    }

    // Sort the items by quantitySold in descending order
    for (int i = 0; i < actualItems - 1; i++) {
        for (int j = 0; j < actualItems - i - 1; j++) {
            if (sortedItems[j].quantitySold < sortedItems[j + 1].quantitySold) {
                struct Item temp = sortedItems[j];
                sortedItems[j] = sortedItems[j + 1];
                sortedItems[j + 1] = temp;
            }
        }
    }

    // Fill the top3 array with the top 3 best-selling items
    for (int i = 0; i < 3 && i < actualItems; i++) {
        snprintf(top3[i], 50, "%s %s %s", sortedItems[i].code, sortedItems[i].name, sortedItems[i].temperature);
    }

    // If there are fewer than 3 items sold, ensure remaining top3 slots are empty
    for (int i = actualItems; i < 3; i++) {
        top3[i][0] = '\0'; // Set to empty string
    }
}

void logTransaction(struct Item item, int quantity, char size, float totalPrice) {
    FILE *file = fopen(SALES_FILE, "a");
    if (!file) {
        perror("Error opening sales file");
        createSalesFile();
        return;
    }

    fprintf(file, "Code: %s, Name: %s, Size: %c, Quantity: %d, Total: %.2f\n", item.code, item.name, size, quantity, totalPrice);

    fclose(file);
}

int main() {
    struct Item items[100];
    int numItems = 0;
    loadMenu(items, &numItems);


    while (1) {
        int choice;
        char input[100];  // Buffer for reading input
        char *endptr;     // To check for extra characters


        printf("Welcome to the cashier system!\n");
        printf("\n1. Add item\n2. Remove item\n3. Edit item\n4. Display menu\n5. Sell items\n6. Display total sales\n7. Exit\nEnter your choice: ");
        if (fgets(input, sizeof(input), stdin) != NULL) {
            // Remove the newline character if it's present
            input[strcspn(input, "\n")] = '\0';

            // Convert input to integer and check for extra characters
            choice = strtol(input, &endptr, 10);

            // Check if there were any extra characters after the number
            if (*endptr != '\0') {
                printf("Invalid input. Please enter a number.\n");
                continue;
            }
        } else {
            printf("Error reading input. Please try again.\n");
            continue;
        }


        switch (choice) {
            case 1:
                system("cls");
                addItem(items, &numItems);
                break;
            case 2:
                system("cls");
                removeItem(items, &numItems);
                break;
            case 3:
                system("cls");
                editItem(items, numItems);
                break;
            case 4:
                system("cls");
                loadMenu(items, &numItems);
                system("cls");
                displayMenuInCmd();
                break;
            case 5:
                system("cls");
                sellItems(items, numItems);
                saveMenu(items, numItems);
                break;
            case 6:
                system("cls");
                readMenu("menu.txt", items, &numItems);
                displayTotalSales(items, numItems);
                break;
            case 7:
                system("cls");
                saveMenu(items, numItems);
                printf("\n--------------------------------------------\n");
                printf("--       Thank you for Ordering!!!        -- \n");
                printf("--------------------------------------------\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
        system("cls");
    }

    return 0;
}
