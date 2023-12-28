#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>

// Define the ParkingSlot struct
typedef struct {
    int slotNumber;
    bool isOccupied;
} ParkingSlot;

// Define the ParkingInfo struct
typedef struct {
    ParkingSlot parkingSlot;
    char vehicleNumber[10];
    char vehicleType[20];
    char timeIn[10];
    char timeOut[10];
    char date[10];
} ParkingInfo;

// Declare arrays for parking slots and parking info
ParkingInfo parkingInfo[200];

// Declare pointers for the next available info
int infoPointer = 0;
bool isFileOK = true;

// Base file name
char file_name[] = "parkingInfo_";

// Function to get the current date
char *getDate() {
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char *date = malloc(10);
    sprintf(date, "%d_%d_%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);
    return date;
}

// Function to check the file
void *fileChecker(void *vargp) {
    // Buffer for reading lines from the file
    char buffer[1024];

    // Variables for counting rows and columns in the file
    int row = 0;
    int col = 0;

    // Get the current time
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);

    // Get date
    char *file_date = getDate();

    // Append the file date to the file name
    strcat(file_name, file_date);

    // Append the file extension
    strcat(file_name, ".csv");

    // Check if the file is OK
    if (isFileOK){
        // Open the file for reading
        FILE *fp = fopen(file_name, "r");

        // If the file doesn't exist, create it
        if (fp == NULL) {
            fp = fopen(file_name, "w");
            fprintf(fp, "Slot_Number,Vehicle_Number,Vehicle_Type,Date,Time_In,Time_Out\n");
            fclose(fp);
        }

        // Read lines from the file
        while (fgets(buffer, 1024, fp)) {
            col = 0;
            row++;

            // Check the header row
            if (row == 1) {
                char *value = strtok(buffer, ", ");

                // Check each column in the header row
                while (value) {
                    if (col == 0) {
                        if (strcmp(value, "Slot_Number") != 0) {
                            isFileOK = false;
                            break;
                        }
                    } else if (col == 1) {
                        if (strcmp(value, "Vehicle_Number") != 0) {
                            isFileOK = false;
                            break;
                        }
                    } else if (col == 2) {
                        if (strcmp(value, "Vehicle_Type") != 0) {
                            isFileOK = false;
                            break;
                        }
                    } else if (col == 3) {
                        if (strcmp(value, "Date") != 0) {
                            isFileOK = false;
                            break;
                        }
                    } else if (col == 4) {
                        if (strcmp(value, "Time_In") != 0) {
                            isFileOK = false;
                            break;
                        }
                    } else if (col == 5) {
                        if (strcmp(value, "Time_Out") != 0) {
                            isFileOK = false;
                            break;
                        }
                    }

                    value = strtok(NULL, ", ");
                    col++;
                }
            }
        }

        // Close the file
        fclose(fp);
    } else {
        // If the file is not OK, create a new file
        FILE *fp = fopen(file_name, "w");
        fprintf(fp, "Slot_Number,Vehicle_Number,Vehicle_Type,Date,Time_In,Time_Out\n");
        fclose(fp);
    }
    pthread_exit(NULL);
}

void *fetchPointer(void *arg) {
    //ParkingInfo *pa = (ParkingInfo *)arg;   
    for (int i = 0; i < 200; i++) {
        if (parkingInfo[i].parkingSlot.isOccupied == false) {
            infoPointer = i;
            break;
        }
    }
    pthread_exit(NULL);
}

// Function to search for a vehicle by slot number
int searchVehicle(ParkingInfo *arr, int x) {
    if (arr[x-1].parkingSlot.isOccupied == true) {
        return x-1;
    } else return -1;
    }

// Function to handle invalid input
void invalidInput() {
    printf("\033[1m\033[5m\033[31mInvalid input. Please enter an integer.\033[0m\n");
    sleep(2);
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

// Function to calculate the time difference
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

// Function to park a vehicle
void parkVehicle(ParkingInfo *parkingInfo) 
{
    // Get the current date
    pthread_t thread3;
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char *date = malloc(10);
    sprintf(date, "%d/%d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

    // Prompt the user to enter the vehicle number and vehicle type
    printf("Enter the vehicle number: ");
    char vehicleNumber[10];
    scanf("%s", vehicleNumber);
    printf("Enter the vehicle type: ");
    char vehicleType[20];
    scanf("%s", vehicleType);

    // Store the vehicle information in the parking info array
    parkingInfo[infoPointer].parkingSlot.slotNumber = infoPointer + 1;
    parkingInfo[infoPointer].parkingSlot.isOccupied = true;
    strcpy(parkingInfo[infoPointer].vehicleNumber, vehicleNumber);
    strcpy(parkingInfo[infoPointer].vehicleType, vehicleType);
    strcpy(parkingInfo[infoPointer].timeIn, getTime());
    strcpy(parkingInfo[infoPointer].timeOut, "00:00");
    strcpy(parkingInfo[infoPointer].date, date);
    printf("Park your car in slot %d\n", infoPointer + 1);
    pthread_create(&thread3, NULL, fetchPointer, NULL);
    printf("%s\n", parkingInfo[0].timeIn);

}

void retrieveVehicle(int num) //have to add the parameters
{   
    pthread_t thread4;
    float  fee;
    int pos = searchVehicle(parkingInfo, num);
    if (pos != -1) {
        strcpy(parkingInfo[pos].timeOut, getTime());
        int time = calculateTime(parkingInfo[pos].timeIn);
        printf("Your vehiclea was parked in %d\n", pos + 1);
        printf("Your car was parked for %d minutes\n", time);
        
        //calculate the fee
        if (strcmp(parkingInfo[pos].vehicleType, "Car") == 0 || strcmp(parkingInfo[pos].vehicleType, "car") == 0) {
            fee = time * 0.5;
        } else if (strcmp(parkingInfo[pos].vehicleType, "Bus") == 0 || strcmp(parkingInfo[pos].vehicleType, "bus") == 0) {
            fee = time * 1.0;
        }
        // Open the file for appending
        FILE *fp = fopen(file_name, "a");
        fprintf(fp, "%d,%s,%s,%s,%s,%s\n", parkingInfo[pos].parkingSlot.slotNumber, parkingInfo[pos].vehicleNumber, parkingInfo[pos].vehicleType, parkingInfo[pos].date, parkingInfo[pos].timeIn, parkingInfo[pos].timeOut);
        fclose(fp);

        // Clear the parking info
        parkingInfo[pos].parkingSlot.isOccupied = false;
        strcpy(parkingInfo[pos].vehicleNumber, "");
        strcpy(parkingInfo[pos].vehicleType, "");
        strcpy(parkingInfo[pos].timeIn, "");
        strcpy(parkingInfo[pos].timeOut, "");
        strcpy(parkingInfo[pos].date, "");
        pthread_create(&thread4, NULL, fetchPointer, NULL);
        
    } else {
        printf("No vehicle found.\n");
    }
}

//function for splash screen
void splash() {
    printf("\033[32m\033[5m__        __   _                            _          ____                       _     ____  _       _   \033[0m\n");    
    printf("\033[32m\033[5m\\ \\      / /__| | ___ ___  _ __ ___   ___  | |_ ___   / ___| _ __ ___   __ _ _ __| |_  / ___|| | ___ | |_ ___ \033[0m\n");
    printf("\033[32m\033[5m \\ \\ /\\ / / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\ | __/ _ \\  \\___ \\| '_ ` _ \\ / _` | '__| __| \\___ \\| |/ _ \\| __/ __|\033[0m\n");
    printf("\033[32m\033[5m  \\ V  V /  __/ | (_| (_) | | | | | |  __/ | || (_) |  ___) | | | | | | (_| | |  | |_   ___) | | (_) | |_\\__ \\ \033[0m\n");
    printf("\033[32m\033[5m   \\_/\\_/ \\___|_|\\___\\___/|_| |_| |_|\\___|  \\__\\___/  |____/|_| |_| |_|\\__,_|_|   \\__| |____/|_|\\___/ \\__|___/\033[0m\n");                                                                     
}
// Main function
void main()
{
    // Declare a boolean variable to control the loop
    bool loop = true;
    
    pthread_t thread, thread2;
    pthread_create(&thread, NULL, fileChecker, NULL);

    // Start of the loop
    do {
        splash();
        sleep(2);

        // Print the menu options
        printf("1. Park\n");
        printf("2. Retrieve Car from Parking\n");
        printf("3. Temp\n");
        printf("4. Exit\n");

        // Prompt the user to enter their choice
        printf("Enter your choice: ");

        // Declare a variable to hold the user's choice
        int choice;

        // Read the user's choice and check if it's an integer
        int result = scanf("%d", &choice);
        if (result < 1) {
            // If the input is not an integer, print an error message
            // if not integer then check whether it is a sting or a float
            
            invalidInput();
            // Clear the input buffer
            while(getchar() != '\n');
        } else {
            // Process the user's choice
            if (choice == 1) {
                printf("\033[2J\033[1;1H");
                // If the choice is 1, park the vehicle
                parkVehicle(parkingInfo);
            } else if (choice == 2) {
                printf("\033[2J\033[1;1H");
                // If the choice is 2, retrieve the vehicle
                printf("Enter the slot number: ");
                int slotNumber;
                scanf("%d", &slotNumber);
                retrieveVehicle(slotNumber);
            } else if (choice == 3) {
                printf("\033[2J\033[1;1H");
            } else if (choice == 4){
                printf("\033[2J\033[1;1H");
                // If the choice is 4, exit the loop
                loop = false;
            } else if (choice == 5) {
                printf("\033[2J\033[1;1H");
                // If the choice is 5, exit the loop
                splash();
                sleep(1);
                char ch[10];
                scanf("%s", ch);
                if (strcmp(ch, "exit") == 0) {
                    printf("Welcome Admin!!\n");
                } 
            }else {
                // If the choice is not 1, 2, 3, or 4, print an error message
                printf("Invalid choice. Enter 1 or 2 or 3 or 4\n");
            }
        }
    } while (loop); // End of the loop   
}
