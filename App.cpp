#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void loadMenu(struct Item items[], int *numItems) {
    FILE *file = fopen(MENU_FILE, "r");
    if (!file) {
        perror("Error opening menu file");
        return;
    }

    *numItems = 0;
    while (fscanf(file, "%s %s %s %f %d\n", items[*numItems].code, items[*numItems].name,
            items[*numItems].temperature, &items[*numItems].price, &items[*numItems].quantitySold) == 5) {
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
        return;
    }

    for (int i = 0; i < numItems; i++) {
        fprintf(file, "%s %s %s %.2f %d\n", items[i].code, items[i].name, items[i].temperature, items[i].price, items[i].quantitySold);
    }

    fclose(file);
}

void displayMenuInCmd() {
    char command[256];
    snprintf(command, sizeof(command), "start cmd /c \"type %s && pause\"", MENU_FILE);
    system(command);
}

void determineTop3(struct Sale sales[], int numSales, char top3[][50]) {
    struct Sale temp;
    for (int i = 0; i < numSales - 1; i++) {
        for (int j = 0; j < numSales - i - 1; j++) {
            if (sales[j].quantity < sales[j + 1].quantity) {
                temp = sales[j];
                sales[j] = sales[j + 1];
                sales[j + 1] = temp;
            }
        }
    }

    for (int i = 0; i < 3 && i < numSales; i++) {
        strcpy(top3[i], sales[i].name);
    }
}

void displayTotalSales(struct Item items[], int numItems) {
    float totalSales = 0.0;
    int numSales = 0;
    struct Sale sales[MAX_ITEMS];

    FILE *file = fopen(SALES_FILE, "r");
    if (!file) {
        perror("Error opening sales file");
        return;
    }

    while (fscanf(file, "Code: %s, Name: %[^,], Size: %c, Quantity: %d, Total: %f\n",
            sales[numSales].code, sales[numSales].name, &sales[numSales].size,
            &sales[numSales].quantity, &sales[numSales].total) == 5) {
        totalSales += sales[numSales].total;
        numSales++;
        if (numSales >= MAX_ITEMS) {
            printf("Maximum number of sales exceeded.\n");
            break;
        }
    }

    fclose(file);

    printf("Total Sales: %.2f\n", totalSales);

    char top3[3][50] = {"", "", ""};
    determineTop3(sales, numSales, top3);

    printf("Top 3 best-selling drinks:\n");
    for (int i = 0; i < 3 && strlen(top3[i]) > 0; i++) {
        printf("%d. %s\n", i + 1, top3[i]);
    }
}

void logTransaction(struct Item item, int quantity, char size, float totalPrice) {
    FILE *file = fopen(SALES_FILE, "a");
    if (!file) {
        perror("Error opening sales file");
        return;
    }

    fprintf(file, "Code: %s, Name: %s, Size: %c, Quantity: %d, Total: %.2f\n", item.code, item.name, size, quantity, totalPrice);

    fclose(file);
}

float calculateChange(float total, float payment) {
    return payment - total;
}

void sellItems(struct Item items[], int numItems) {
    float total = 0.0;
    char code[10];
    float payment;

    while (1) {
        printf("\n**************\nEnter item code (or 'EX' to finish): ");
        scanf("%s", code);

        if (strcmp(code, "EX") == 0) {
            break;
        }

        struct Item *selectedItem = NULL;
        for (int i = 0; i < numItems; i++) {
            if (strcmp(items[i].code, code) == 0) {
                selectedItem = &items[i];
                break;
            }
        }

        if (selectedItem == NULL) {
            printf("Invalid item code. Please try again.\n");
            continue;
        }

        int quantity;
        printf("Enter quantity: ");
        scanf("%d", &quantity);

        char size;
        printf("Enter size (S, M, L): ");
        scanf(" %c", &size);

        float price = selectedItem->price;
        if (size == 'M') {
            price += 20.0;
        } else if (size == 'L') {
            price += 30.0;
        }

        float totalPrice = price * quantity;
        total += totalPrice;

        logTransaction(*selectedItem, quantity, size, totalPrice);
        selectedItem->quantitySold += quantity;

        printf("Added %d of %s to the order. Subtotal: %.2f\n", quantity, selectedItem->name, totalPrice);
    }

    printf("Total amount to pay: %.2f\n", total);

    while (1) {
        printf("Enter payment amount: ");
        scanf("%f", &payment);

        if (payment < total) {
            printf("Insufficient payment. Please enter an amount of at least %.2f.\n", total);
        } else {
            float change = calculateChange(total, payment);
            printf("Change: %.2f\n", change);
            break;
        }
    }
}

void addItem(struct Item items[], int *numItems) {
    struct Item newItem;
    printf("Enter code: ");
    scanf("%s", newItem.code);
    printf("Enter name: ");
    scanf("%s", newItem.name);
    printf("Enter temperature: ");
    scanf("%s", newItem.temperature);
    printf("Enter price: ");
    scanf("%f", &newItem.price);
    newItem.quantitySold = 0; // Initialize quantitySold

    items[*numItems] = newItem;
    (*numItems)++;

    saveMenu(items, *numItems);
    printf("Item added successfully!\n");
}

void removeItem(struct Item items[], int *numItems) {
    char code[10];
    printf("Enter code of item to remove: ");
    scanf("%s", code);

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
}

void editItem(struct Item items[], int numItems) {
    char code[10];
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
        return;
    }

    printf("Enter new name: ");
    scanf("%s", selectedItem->name);
    printf("Enter new temperature: ");
    scanf("%s", selectedItem->temperature);
    printf("Enter new price: ");
    scanf("%f", &selectedItem->price);

    saveMenu(items, numItems);
    printf("Item edited successfully!\n");
}

int main() {
    struct Item items[100];
    int numItems = 0;
    loadMenu(items, &numItems);

    printf("Welcome to the cashier system!\n");

    while (1) {
        int choice;
        printf("\n1. Add item\n2. Remove item\n3. Edit item\n4. Display menu\n5. Sell items\n6. Display total sales\n7. Exit\nEnter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                addItem(items, &numItems);
                break;
            case 2:
                removeItem(items, &numItems);
                break;
            case 3:
                editItem(items, numItems);
                break;
            case 4:
                displayMenuInCmd();
                break;
            case 5:
                sellItems(items, numItems);
                break;
            case 6:
                displayTotalSales(items, numItems);
                break;
            case 7:
                saveMenu(items, numItems);
                printf("Goodbye!\n");
                exit(0);
            default:
                printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
