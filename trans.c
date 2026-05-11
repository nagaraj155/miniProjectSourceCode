#include <stdio.h>
#include <stdlib.h>

// Structure definition
struct clientData {
    unsigned int acctNum;
    char lastName[15];
    char firstName[10];
    double balance;
};

// Function prototypes
unsigned int enterChoice(void);
void textFile(FILE *readPtr);
void updateRecord(FILE *fPtr);
void newRecord(FILE *fPtr);
void deleteRecord(FILE *fPtr);
void initializeFile(FILE *fPtr);

int main() {
    FILE *cfPtr;
    unsigned int choice;

    // Open file (create if not exists)
    cfPtr = fopen("credit.dat", "rb+");

    if (cfPtr == NULL) {
        cfPtr = fopen("credit.dat", "wb+");

        if (cfPtr == NULL) {
            printf("File could not be created.\n");
            exit(1);
        }

        initializeFile(cfPtr);
    }

    // Menu loop
    while ((choice = enterChoice()) != 5) {
        switch (choice) {
            case 1:
                textFile(cfPtr);
                break;
            case 2:
                updateRecord(cfPtr);
                break;
            case 3:
                newRecord(cfPtr);
                break;
            case 4:
                deleteRecord(cfPtr);
                break;
            default:
                printf("Invalid choice\n");
        }
    }

    fclose(cfPtr);
    return 0;
}

// Initialize file with 100 empty records
void initializeFile(FILE *fPtr) {
    struct clientData blank = {0, "", "", 0.0};

    for (int i = 0; i < 100; i++) {
        fwrite(&blank, sizeof(struct clientData), 1, fPtr);
    }
}

// Convert binary file to text file
void textFile(FILE *readPtr) {
    FILE *writePtr = fopen("accounts.txt", "w");

    if (writePtr == NULL) {
        printf("File could not be opened.\n");
        return;
    }

    rewind(readPtr);

    struct clientData client;

    fprintf(writePtr, "%-6s%-16s%-11s%10s\n",
            "Acct", "Last Name", "First Name", "Balance");

    while (fread(&client, sizeof(struct clientData), 1, readPtr) == 1) {
        if (client.acctNum != 0) {
            fprintf(writePtr, "%-6d%-16s%-11s%10.2f\n",
                    client.acctNum,
                    client.lastName,
                    client.firstName,
                    client.balance);
        }
    }

    fclose(writePtr);
    printf("accounts.txt created successfully.\n");
}

// Update account
void updateRecord(FILE *fPtr) {
    unsigned int account;
    double transaction;
    struct clientData client;

    printf("Enter account number (1-100): ");
    scanf("%u", &account);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0) {
        printf("Account not found.\n");
        return;
    }

    printf("Current Balance: %.2f\n", client.balance);
    printf("Enter amount (+ deposit / - withdraw): ");
    scanf("%lf", &transaction);

    client.balance += transaction;

    fseek(fPtr, -sizeof(struct clientData), SEEK_CUR);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Updated Balance: %.2f\n", client.balance);
}

// Add new record
void newRecord(FILE *fPtr) {
    unsigned int account;
    struct clientData client;

    printf("Enter new account number (1-100): ");
    scanf("%u", &account);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum != 0) {
        printf("Account already exists.\n");
        return;
    }

    printf("Enter LastName FirstName Balance: ");
    scanf("%14s %9s %lf",
          client.lastName,
          client.firstName,
          &client.balance);

    client.acctNum = account;

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&client, sizeof(struct clientData), 1, fPtr);

    printf("Account created successfully.\n");
}

// Delete record
void deleteRecord(FILE *fPtr) {
    unsigned int account;
    struct clientData client;
    struct clientData blank = {0, "", "", 0.0};

    printf("Enter account number to delete: ");
    scanf("%u", &account);

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fread(&client, sizeof(struct clientData), 1, fPtr);

    if (client.acctNum == 0) {
        printf("Account does not exist.\n");
        return;
    }

    fseek(fPtr, (account - 1) * sizeof(struct clientData), SEEK_SET);
    fwrite(&blank, sizeof(struct clientData), 1, fPtr);

    printf("Account deleted successfully.\n");
}

// Menu
unsigned int enterChoice(void) {
    unsigned int choice;

    printf("\n===== BANK MENU =====\n");
    printf("1 - Export to text file\n");
    printf("2 - Update account\n");
    printf("3 - Add new account\n");
    printf("4 - Delete account\n");
    printf("5 - Exit\n");
    printf("Enter choice: ");

    scanf("%u", &choice);
    return choice;
}