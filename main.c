#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>

// Define the ParkingSlot struct
typedef struct {
    int slotNumber;
    bool isOccupied;
} ParkingSlot;

// Define the ParkingInfo struct
typedef struct {
    int slotNumber;
    char vehicleNumber[10];
    char vehicleType[20];
    char timeIn[10];
    char timeOut[10];
    char date[10];
} ParkingInfo;

// Declare arrays for parking slots and parking info
ParkingSlot basement1[100];
ParkingSlot basement2[100];
ParkingInfo parkingInfo[200];

// Declare pointers for the next available slot and info
int slotPointer = 0;
int infoPointer = 0;

// Function to increase the size of the parkingInfo array
void incrementArrSize(ParkingInfo *arr) {
    int size = sizeof(arr) / sizeof(arr[0]);
    size++;
    ParkingInfo *temp = realloc(arr, size * sizeof(ParkingInfo));
    arr = temp;
}

// Function to search for a vehicle by slot number
void searchVehicle(ParkingInfo *arr, int x) {
    printf("%d\n", x);
    for (int i = 0; i < infoPointer; i++) {
        if (arr[i].slotNumber == x) {
            printf("Vehicle Number: %s\n", arr[i].vehicleNumber);
            printf("Vehicle Type: %s\n", arr[i].vehicleType);
            printf("Time In: %s\n", arr[i].timeIn);
            printf("Time Out: %s\n", arr[i].timeOut);
            printf("Date: %s\n", arr[i].date);
            return;
        }
    }
}

// Function to handle invalid input
void invalidInput() {
    printf("\033[1m\033[5m\033[31mInvalid input. Please enter an integer.\033[0m\n");
    sleep(3);
    printf("\033[2J\033[1;1H");
}

// Function to get the current time
char *getTime() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char *time = malloc(10);
    sprintf(time, "%d:%d", tm.tm_hour, tm.tm_min);
    return time;
}

int calculateTime(char *timeIn)
{
    int timeInHours, timeInMinutes, timeOutHours, timeOutMinutes;
    char *timeOut = getTime();

    //Parse the timeIn and timeOut to get the hours and minutes
    sscanf(timeIn, "%d:%d", &timeInHours, &timeInMinutes);
    sscanf(timeOut, "%d:%d", &timeOutHours, &timeOutMinutes);

    //Convert the timeIn and timeOut to minutes
    int t1 = timeInHours * 60 + timeInMinutes;
    int t2 = timeOutHours * 60 + timeOutMinutes;

    //Calculate the difference
    int diff = t2 - t1;
    return diff;
}


void parkVehicle(ParkingInfo *parkingInfo) //have to add the parameters
{
    
}

void retrieveVehicle() //have to add the parameters
{   
    int num;
    scanf("%d", &num);
    searchVehicle(parkingInfo, num);
}

// Main function
void main()
{
    // Declare a boolean variable to control the loop
    bool loop = true;
    // Start of the loop
    do {
        // Print the welcome message with some formatting
        printf("\033[1m\033[5m\033[32mWelcome to Smart Slots !!\033[0m\n"); // Used ANSI escape codes to format the text

        // Print the menu options
        printf("1. Park\n");
        printf("2. Retrieve Car from Parking\n");
        printf("3. Get time\n");
        printf("4. Exit\n");

        // Prompt the user to enter their choice
        printf("Enter your choice: ");

        // Declare a variable to hold the user's choice
        int choice;

        // Read the user's choice and check if it's an integer
        int result = scanf("%d", &choice);
        if (result < 1) {
            // If the input is not an integer, print an error message
            inalidInput();
            // Clear the input buffer
            while(getchar() != '\n');
        } else {
            // Process the user's choice
            if (choice == 1) {
                // If the choice is 1, park the vehicle
                parkVehicle(parkingInfo);
            } else if (choice == 2) {
                // If the choice is 2, retrieve the vehicle
                retrieveVehicle();
            } else if (choice == 3) {
                // If the choice is 3, print the current time
                int t2 = calculateTime("22:38");
                printf("Time: %d\n", t2);
            } else if (choice == 4) {
                // If the choice is 4, exit the loop
                loop = false;
            } else {
                // If the choice is not 1, 2, 3, or 4, print an error message
                printf("Invalid choice\n");
            }
        }
    } while (loop); // End of the loop
}
