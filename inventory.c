
#include <stdio.h>

#define MAX 100
#define FILE_NAME "inventory.txt"

struct Product {
    int id;
    char name[50];
    int quantity;
    float price;
};

int findProduct(struct Product p[], int count, int id) {
    for (int i = 0; i < count; i++) {
        if (p[i].id == id)
            return i;
    }
    return -1;
}

void saveProducts(struct Product p[], int count) {
    FILE *fp = fopen(FILE_NAME, "w");

    if (fp == NULL) {
        printf("Error saving file!\n");
        return;
    }

    for (int i = 0; i < count; i++) {
        fprintf(fp, "%d|%s|%d|%.2f\n",
                p[i].id, p[i].name,
                p[i].quantity, p[i].price);
    }

    fclose(fp);
}

int loadProducts(struct Product p[]) {
    FILE *fp = fopen(FILE_NAME, "r");

    if (fp == NULL)
        return 0;

    int count = 0;

    while (count < MAX &&
           fscanf(fp, "%d|%49[^|]|%d|%f",
                  &p[count].id,
                  p[count].name,
                  &p[count].quantity,
                  &p[count].price) == 4) {
        count++;
    }

    fclose(fp);
    return count;
}

void addProduct(struct Product p[], int *count) {
    if (*count >= MAX) {
        printf("Inventory is full!\n");
        return;
    }

    int id;

    printf("Enter Product ID: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID!\n");
        return;
    }

    if (findProduct(p, *count, id) != -1) {
        printf("Product ID already exists!\n");
        return;
    }

    p[*count].id = id;

    printf("Enter Product Name: ");
    scanf(" %49[^\n]", p[*count].name);

    printf("Enter Quantity: ");
    if (scanf("%d", &p[*count].quantity) != 1 ||
        p[*count].quantity < 0) {
        printf("Invalid quantity!\n");
        return;
    }

    printf("Enter Price: ");
    if (scanf("%f", &p[*count].price) != 1 ||
        p[*count].price < 0) {
        printf("Invalid price!\n");
        return;
    }

    (*count)++;
    saveProducts(p, *count);

    printf("Product added successfully!\n");
}

void viewProducts(struct Product p[], int count) {
    if (count == 0) {
        printf("No products found.\n");
        return;
    }

    printf("\n===== INVENTORY =====\n");

    for (int i = 0; i < count; i++) {
        printf("\nID: %d\n", p[i].id);
        printf("Name: %s\n", p[i].name);
        printf("Quantity: %d\n", p[i].quantity);
        printf("Price: Rs. %.2f\n", p[i].price);
        printf("Stock Value: Rs. %.2f\n",
               p[i].quantity * p[i].price);
    }
}

void searchProduct(struct Product p[], int count) {
    int id;

    printf("Enter Product ID: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID!\n");
        return;
    }

    int index = findProduct(p, count, id);

    if (index == -1) {
        printf("Product not found.\n");
        return;
    }

    printf("Product: %s\n", p[index].name);
    printf("Quantity: %d\n", p[index].quantity);
    printf("Price: Rs. %.2f\n", p[index].price);
}

void updateStock(struct Product p[], int count) {
    int id, quantity;

    printf("Enter Product ID: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID!\n");
        return;
    }

    int index = findProduct(p, count, id);

    if (index == -1) {
        printf("Product not found.\n");
        return;
    }

    printf("Enter New Quantity: ");
    if (scanf("%d", &quantity) != 1 || quantity < 0) {
        printf("Invalid quantity!\n");
        return;
    }

    p[index].quantity = quantity;
    saveProducts(p, count);

    printf("Stock updated successfully!\n");
}

void sellProduct(struct Product p[], int count) {
    int id, quantity;

    printf("Enter Product ID: ");
    if (scanf("%d", &id) != 1) {
        printf("Invalid ID!\n");
        return;
    }

    int index = findProduct(p, count, id);

    if (index == -1) {
        printf("Product not found.\n");
        return;
    }

    printf("Enter Quantity to Sell: ");
    if (scanf("%d", &quantity) != 1 || quantity <= 0) {
        printf("Enter a valid positive quantity!\n");
        return;
    }

    if (quantity > p[index].quantity) {
        printf("Insufficient stock!\n");
        return;
    }

    p[index].quantity -= quantity;

    printf("Sale successful!\n");
    printf("Total Amount: Rs. %.2f\n",
           quantity * p[index].price);
    printf("Remaining Stock: %d\n", p[index].quantity);

    saveProducts(p, count);
}

void lowStockReport(struct Product p[], int count) {
    int found = 0;

    printf("\n===== LOW STOCK REPORT =====\n");

    for (int i = 0; i < count; i++) {
        if (p[i].quantity < 5) {
            printf("%s - Only %d units left\n",
                   p[i].name, p[i].quantity);
            found = 1;
        }
    }

    if (!found)
        printf("No low-stock products.\n");
}

int main() {
    struct Product products[MAX];
    int count = loadProducts(products);
    int choice;

    while (1) {
        printf("\n===== INVENTORY MANAGEMENT =====\n");
        printf("1. Add Product\n");
        printf("2. View Products\n");
        printf("3. Search Product\n");
        printf("4. Update Stock\n");
        printf("5. Sell Product\n");
        printf("6. Low Stock Report\n");
        printf("7. Exit\n");
        printf("Enter Choice: ");

        if (scanf("%d", &choice) != 1) {
            printf("Invalid input!\n");
            break;
        }

        switch (choice) {
            case 1: addProduct(products, &count); break;
            case 2: viewProducts(products, count); break;
            case 3: searchProduct(products, count); break;
            case 4: updateStock(products, count); break;
            case 5: sellProduct(products, count); break;
            case 6: lowStockReport(products, count); break;
            case 7:
                printf("Inventory System Closed.\n");
                return 0;
            default:
                printf("Invalid choice!\n");
        }
    }

    return 0;
}
