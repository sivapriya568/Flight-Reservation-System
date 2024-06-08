#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Structure to represent a seat
struct Seat {
    int seatNumber;
    int isBooked;
    char passengerName[50];
    struct Seat *next;
};

// Function to initialize seats using linked list
struct Seat *initializeSeats(int totalSeats) {
    struct Seat *head = NULL;

    for (int i = totalSeats; i > 0; i--) {
        struct Seat *newSeat = (struct Seat *)malloc(sizeof(struct Seat));

        if (newSeat == NULL) {
            printf("Memory allocation error\n");
            exit(EXIT_FAILURE);
        }

        newSeat->seatNumber = i;
        newSeat->isBooked = 0;
        strcpy(newSeat->passengerName, "");
        newSeat->next = head;
        head = newSeat;
    }

    return head;
}

// Function to display available seats
void displayAvailableSeats(struct Seat *head) {
    printf("Available Seats:\n");

    while (head != NULL) {
        if (!head->isBooked) {
            printf("Seat %d\n", head->seatNumber);
        }
        head = head->next;
    }
}

// Function to book a seat
void bookSeat(struct Seat *head, int seatNumber, const char *passengerName) {
    struct Seat *current = head;

    while (current != NULL && current->seatNumber != seatNumber) {
        current = current->next;
    }

    if (current != NULL && !current->isBooked) {
        current->isBooked = 1;
        strcpy(current->passengerName, passengerName);
        printf("Seat %d booked for %s\n", seatNumber, passengerName);
    } else {
        printf("Seat is already booked or does not exist\n");
    }
}

// Function to cancel a booked seat
/*void cancelSeat(struct Seat *head, int seatNumber) {
    struct Seat *current = head;

    while (current != NULL && current->seatNumber != seatNumber) {
        current = current->next;
    }

    if (current != NULL && current->isBooked) {
        current->isBooked = 0;
        strcpy(current->passengerName, "");
        printf("Booking for Seat %d cancelled\n", seatNumber);
    } else {
        printf("Seat %d is not booked or does not exist\n", seatNumber);
    }
}*/
void cancelSeat(struct Seat **headPtr, int seatNumber) {
    struct Seat *current = *headPtr;
    struct Seat *prev = NULL;

    while (current != NULL && current->seatNumber != seatNumber) {
        prev = current;
        current = current->next;
    }

    if (current != NULL && current->isBooked) {
        current->isBooked = 0;
        strcpy(current->passengerName, "");
        printf("Booking for Seat %d cancelled\n", seatNumber);

        // Remove the canceled seat from the linked list
        /*if (prev == NULL) {
            // The canceled seat is the head of the list
            *headPtr = current->next;
        } else {
            prev->next = current->next;
        }/

        free(current); // Free the memory of the canceled seat*/
    } else {
        printf("Seat is not booked or does not exist\n");
    }
}


// Function to save seat information to a text file
void saveSeatsToFile(struct Seat *head, const char *filename) {
    FILE *file = fopen(filename, "w");
    if (file != NULL) {
        while (head != NULL) {
            fprintf(file, "%d %d %s\n", head->seatNumber, head->isBooked, head->passengerName);
            head = head->next;
        }
        fclose(file);
    } else {
        printf("Error opening file for writing\n");
    }
}

// Function to load seat information from a text file
struct Seat *loadSeatsFromFile(const char *filename) {
    struct Seat *head = NULL;
    struct Seat *current = NULL;

    FILE *file = fopen(filename, "r");
    if (file != NULL) {
        int seatNumber, isBooked;
        char passengerName[50];

        while (fscanf(file, "%d %d %s", &seatNumber, &isBooked, passengerName) == 3) {
            struct Seat *newSeat = (struct Seat *)malloc(sizeof(struct Seat));
            if (newSeat == NULL) {
                printf("Memory allocation error\n");
                exit(EXIT_FAILURE);
            }

            newSeat->seatNumber = seatNumber;
            newSeat->isBooked = isBooked;
            strcpy(newSeat->passengerName, passengerName);
            newSeat->next = NULL;

            if (head == NULL) {
                head = newSeat;
                current = head;
            } else {
                current->next = newSeat;
                current = newSeat;
            }
        }
        fclose(file);
    } else {
        printf("Error opening file for reading\n");
    }

    return head;
}

// Function to free memory allocated for seats
void freeSeats(struct Seat *head) {
    struct Seat *current = head;
    struct Seat *next;

    while (current != NULL) {
        next = current->next;
        free(current);
        current = next;
    }
}

int main() {
    const int totalSeats = 10;
	printf("Total no.of seats in flight:%d",totalSeats);
    // Load seats from file at the beginning of the program
    struct Seat *head = initializeSeats(totalSeats);
    int choice;

    while (1) {
        // Switch case for booking, cancellation, and display
        printf("\n1. Display Available Seats\n");
        printf("2. Book Seat\n");
        printf("3. Cancel Seat\n");
        printf("0. Exit\n");

        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                displayAvailableSeats(head);
                break;

            case 2: {
                int seatNumber;
                char passengerName[50];
                printf("Enter seat number: ");
                scanf("%d", &seatNumber);
                printf("Enter passenger name: ");
                scanf("%s", passengerName);
                bookSeat(head, seatNumber, passengerName);
                saveSeatsToFile(head, "seats.txt");
                //freeSeats(head);
                //head = loadSeatsFromFile("seats.txt");
                break;
            }

            case 3: {
    			int seatNumber;
    			printf("Enter seat number to cancel: ");
    			scanf("%d", &seatNumber);
    			cancelSeat(&head, seatNumber);
    			saveSeatsToFile(head, "seats.txt");
    			break;
			}


            case 0:
                // Free allocated memory and exit
                freeSeats(head);
                printf("Thank you for using\n");
                return 0;

            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }

        // Load seats immediately after saving
        // freeSeats(head);
        // head = loadSeatsFromFile("seats.txt");
        // printf("Seats successfully loaded from 'seats.txt'\n");
    }

    return 0;
}
