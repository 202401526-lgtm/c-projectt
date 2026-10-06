#include <stdio.h>

#define MAX_PATIENTS 100
#define NUM_SLOTS 5

typedef struct {
    int id;
    char name[50];
    int age;
    char gender[10];
} Patient;

typedef struct {
    char time[30];
    int patientID;
} Reservation;

/* Function prototypes */
int findPatient(Patient patients[], int count, int id);
int hasReservation(Reservation slots[], int patientID);
int adminLogin(void);

void adminMode(Patient patients[], int *count, Reservation slots[]);
void userMode(Patient patients[], int count, Reservation slots[]);

void addPatient(Patient patients[], int *count);
void editPatient(Patient patients[], int count);
void reserveSlot(Patient patients[], int count, Reservation slots[]);
void cancelReservation(Patient patients[], int count, Reservation slots[]);

void viewPatient(Patient patients[], int count);
void viewReservations(Reservation slots[]);

int main(void)
{
    Patient patients[MAX_PATIENTS];
    int patientCount = 0;
    int choice;

    Reservation slots[NUM_SLOTS] = {
        {"2:00 PM - 2:30 PM", -1},
        {"2:30 PM - 3:00 PM", -1},
        {"3:00 PM - 3:30 PM", -1},
        {"4:00 PM - 4:30 PM", -1},
        {"4:30 PM - 5:00 PM", -1}
    };

    while (1) {

        printf("\n====================================\n");
        printf("      CLINIC MANAGEMENT SYSTEM\n");
        printf("====================================\n");

        printf("1. Admin Mode\n");
        printf("2. User Mode\n");
        printf("3. Exit\n");

        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {
            while (getchar() != '\n');
            printf("Invalid input.\n");
            continue;
        }

        if (choice == 1) {

            if (!adminLogin())
                return 0;

            adminMode(patients, &patientCount, slots);
        }

        else if (choice == 2) {

            userMode(patients, patientCount, slots);
        }

        else if (choice == 3) {

            printf("Goodbye!\n");
            return 0;
        }

        else {

            printf("Invalid choice.\n");
        }
    }
}


/* ================= ADMIN LOGIN ================= */

int adminLogin(void)
{
    int password;
    int attempts;

    printf("\n========== ADMIN LOGIN ==========\n");

    for (attempts = 1; attempts <= 3; attempts++) {

        printf("Enter password: ");

        if (scanf("%d", &password) != 1) {

            while (getchar() != '\n');

            printf("Invalid password.\n");
            continue;
        }

        if (password == 1234) {

            printf("Login successful!\n");
            return 1;
        }

        printf("Incorrect password. Attempts left: %d\n",
               3 - attempts);
    }

    printf("Wrong password 3 times. System closed.\n");

    return 0;
}


/* ================= ADMIN MODE ================= */

void adminMode(Patient patients[], int *count, Reservation slots[])
{
    int choice;

    while (1) {

        printf("\n========== ADMIN MODE ==========\n");

        printf("1. Add new patient record\n");
        printf("2. Edit patient record\n");
        printf("3. Reserve a slot with the doctor\n");
        printf("4. Cancel reservation\n");
        printf("5. Return to main menu\n");

        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {

            while (getchar() != '\n');

            printf("Invalid choice.\n");
            continue;
        }

        switch (choice) {

            case 1:
                addPatient(patients, count);
                break;

            case 2:
                editPatient(patients, *count);
                break;

            case 3:
                reserveSlot(patients, *count, slots);
                break;

            case 4:
                cancelReservation(patients, *count, slots);
                break;

            case 5:
                return;

            default:
                printf("Invalid choice.\n");
        }
    }
}


/* ================= USER MODE ================= */

void userMode(Patient patients[], int count, Reservation slots[])
{
    int choice;

    while (1) {

        printf("\n========== USER MODE ==========\n");

        printf("1. View patient record\n");
        printf("2. View today's reservations\n");
        printf("3. Return to main menu\n");

        printf("Enter choice: ");

        if (scanf("%d", &choice) != 1) {

            while (getchar() != '\n');

            printf("Invalid choice.\n");
            continue;
        }

        switch (choice) {

            case 1:
                viewPatient(patients, count);
                break;

            case 2:
                viewReservations(slots);
                break;

            case 3:
                return;

            default:
                printf("Invalid choice.\n");
        }
    }
}


/* ================= ADD PATIENT ================= */

void addPatient(Patient patients[], int *count)
{
    int id;

    if (*count >= MAX_PATIENTS) {

        printf("Patient database is full.\n");
        return;
    }

    while (1) {

        printf("Enter patient ID: ");

        if (scanf("%d", &id) != 1) {

            while (getchar() != '\n');

            printf("Invalid ID.\n");
            continue;
        }

        if (findPatient(patients, *count, id) == -1)
            break;

        printf("ID already exists. Enter another ID.\n");
    }

    patients[*count].id = id;

    printf("Enter patient name (one word): ");
    scanf("%49s", patients[*count].name);

    printf("Enter patient age: ");

    while (scanf("%d", &patients[*count].age) != 1) {

        while (getchar() != '\n');

        printf("Invalid age. Enter again: ");
    }

    printf("Enter patient gender: ");
    scanf("%9s", patients[*count].gender);

    (*count)++;

    printf("Patient added successfully.\n");
}


/* ================= EDIT PATIENT ================= */

void editPatient(Patient patients[], int count)
{
    int id;
    int index;

    if (count == 0) {

        printf("No patients found.\n");
        return;
    }

    printf("Enter patient ID: ");

    if (scanf("%d", &id) != 1) {

        while (getchar() != '\n');

        printf("Invalid ID.\n");
        return;
    }

    index = findPatient(patients, count, id);

    if (index == -1) {

        printf("Incorrect ID.\n");
        return;
    }

    printf("Enter new patient name (one word): ");
    scanf("%49s", patients[index].name);

    printf("Enter new age: ");

    while (scanf("%d", &patients[index].age) != 1) {

        while (getchar() != '\n');

        printf("Invalid age. Enter again: ");
    }

    printf("Enter new gender: ");
    scanf("%9s", patients[index].gender);

    printf("Patient record updated successfully.\n");
}


/* ================= RESERVE SLOT ================= */

void reserveSlot(Patient patients[], int count, Reservation slots[])
{
    int patientID;
    int slot;
    int i;

    if (count == 0) {

        printf("No patients found. Add a patient first.\n");
        return;
    }

    printf("\nAvailable slots:\n");

    for (i = 0; i < NUM_SLOTS; i++) {

        if (slots[i].patientID == -1)
            printf("%d. %s\n", i + 1, slots[i].time);
    }

    printf("Enter patient ID: ");

    if (scanf("%d", &patientID) != 1) {

        while (getchar() != '\n');

        printf("Invalid ID.\n");
        return;
    }

    if (findPatient(patients, count, patientID) == -1) {

        printf("Incorrect ID.\n");
        return;
    }

    if (hasReservation(slots, patientID)) {

        printf("This patient already has a reservation.\n");
        return;
    }

    printf("Enter desired slot number: ");

    if (scanf("%d", &slot) != 1) {

        while (getchar() != '\n');

        printf("Invalid slot.\n");
        return;
    }

    if (slot < 1 || slot > NUM_SLOTS) {

        printf("Invalid slot number.\n");
        return;
    }

    if (slots[slot - 1].patientID != -1) {

        printf("This slot is already reserved.\n");
        return;
    }

    slots[slot - 1].patientID = patientID;

    printf("Reservation successful.\n");
}


/* ================= CANCEL RESERVATION ================= */

void cancelReservation(Patient patients[], int count, Reservation slots[])
{
    int patientID;
    int i;

    if (count == 0) {

        printf("No patients found.\n");
        return;
    }

    printf("Enter patient ID: ");

    if (scanf("%d", &patientID) != 1) {

        while (getchar() != '\n');

        printf("Invalid ID.\n");
        return;
    }

    if (findPatient(patients, count, patientID) == -1) {

        printf("Incorrect ID.\n");
        return;
    }

    for (i = 0; i < NUM_SLOTS; i++) {

        if (slots[i].patientID == patientID) {

            slots[i].patientID = -1;

            printf("Reservation cancelled successfully.\n");
            printf("Slot %s is available again.\n",
                   slots[i].time);

            return;
        }
    }

    printf("This patient has no reservation.\n");
}


/* ================= VIEW PATIENT ================= */

void viewPatient(Patient patients[], int count)
{
    int id;
    int index;

    if (count == 0) {

        printf("No patients found.\n");
        return;
    }

    printf("Enter patient ID: ");

    if (scanf("%d", &id) != 1) {

        while (getchar() != '\n');

        printf("Invalid ID.\n");
        return;
    }

    index = findPatient(patients, count, id);

    if (index == -1) {

        printf("Incorrect ID.\n");
        return;
    }

    printf("\nPatient Information\n");

    printf("ID: %d\n", patients[index].id);
    printf("Name: %s\n", patients[index].name);
    printf("Age: %d\n", patients[index].age);
    printf("Gender: %s\n", patients[index].gender);
}


/* ================= VIEW RESERVATIONS ================= */

void viewReservations(Reservation slots[])
{
    int i;

    printf("\n========== TODAY'S RESERVATIONS ==========\n");

    for (i = 0; i < NUM_SLOTS; i++) {

        printf("%d. %s --> ",
               i + 1,
               slots[i].time);

        if (slots[i].patientID == -1)
            printf("Available\n");

        else
            printf("Patient ID: %d\n",
                   slots[i].patientID);
    }
}


/* ================= FIND PATIENT ================= */

int findPatient(Patient patients[], int count, int id)
{
    int i;

    for (i = 0; i < count; i++) {

        if (patients[i].id == id)
            return i;
    }

    return -1;
}


/* ================= CHECK RESERVATION ================= */

int hasReservation(Reservation slots[], int patientID)
{
    int i;

    for (i = 0; i < NUM_SLOTS; i++) {

        if (slots[i].patientID == patientID)
            return 1;
    }

    return 0;
}
