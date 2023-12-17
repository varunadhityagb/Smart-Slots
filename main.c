#include <stdio.h>
#include <stdbool.h>

typedef struct ParkingSlot {
    int slotNumber;
    bool isOccupied;
} ParkingSlot;

ParkingSlot basement1[100];
ParkingSlot basement2[100];

void parkVehicle() //have to add the parameters
{
    printf("Welcome to the mall\n\n");
}

void retrieveVehicle() //have to add the parameters
{
    printf("Thank you.\n\n");
}

void main()
{
    bool loop = true;
    do {
        printf("\033[1m\033[5m\033[32mWelcome to Smart Slots !!\033[0m\n");
        printf("1. Park\n");
        printf("2. Retrieve Car from Parking\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        int choice;
        scanf("%d", &choice);
        switch (choice) {
            case 1:
                parkVehicle();
                break;
            case 2:
                retrieveVehicle();
                break;
            case 3:
                loop = false;
                break;
            default:
                printf("Invalid choice\n");
                loop = false;
                break;
        }
    } while (loop);
    
}   