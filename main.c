#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h>
#include <pthread.h>
#include <regex.h>
#include <dirent.h>
#include <sys/types.h>

// Define the ParkingSlot struct
typedef struct {
    int slotNumber;
    bool isOccupied;
} ParkingSlot;

// Define the ParkingInfo struct
typedef struct {
    ParkingSlot parkingSlot;
    char vehicleNumber[20];
    char vehicleType[20];
    char timeIn[10];
    char timeOut[10];
    char date[10];
} ParkingInfo;

// Declare arrays for parking slots and parking info
ParkingInfo parkingInfo[200];

// global variables
int infoPointer = 0;
bool isFileOK = true;
char file_name[] = "parkingInfo_";
pthread_t thread, thread2, thread3;


//function for welcome_splash screen
void welcome_splash() {
    printf("\033[32m\033[5m__        __   _                            _          ____                       _     ____  _       _   \033[0m\n");    
    printf("\033[32m\033[5m\\ \\      / /__| | ___ ___  _ __ ___   ___  | |_ ___   / ___| _ __ ___   __ _ _ __| |_  / ___|| | ___ | |_ ___ \033[0m\n");
    printf("\033[32m\033[5m \\ \\ /\\ / / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\ | __/ _ \\  \\___ \\| '_ ` _ \\ / _` | '__| __| \\___ \\| |/ _ \\| __/ __|\033[0m\n");
    printf("\033[32m\033[5m  \\ V  V /  __/ | (_| (_) | | | | | |  __/ | || (_) |  ___) | | | | | | (_| | |  | |_   ___) | | (_) | |_\\__ \\ \033[0m\n");
    printf("\033[32m\033[5m   \\_/\\_/ \\___|_|\\___\\___/|_| |_| |_|\\___|  \\__\\___/  |____/|_| |_| |_|\\__,_|_|   \\__| |____/|_|\\___/ \\__|___/\033[0m\n");                                                                     
}

//function for parking_splash screen
void parking_splash() {
    printf("\033[32m\033[5m ____            _    _             \033[0m\n");
    printf("\033[32m\033[5m|  _ \\ __ _ _ __| | _(_)_ __   __ _ \033[0m\n");
    printf("\033[32m\033[5m| |_) / _` | '__| |/ / | '_ \\ / _` |\033[0m\n");
    printf("\033[32m\033[5m|  __/ (_| | |  |   <| | | | | (_| |\033[0m\n");
    printf("\033[32m\033[5m|_|   \\__,_|_|  |_|\\_\\_|_| |_|\\__, |\033[0m\n");
    printf("\033[32m\033[5m                              |___/ \033[0m\n");                                                                  
}

//function for admin_splash screen
void admin_splash() {
    printf("\033[32m\033[5m__        __   _                               _       _           _         _ _ \033[0m\n");
    printf("\033[32m\033[5m\\ \\      / /__| | ___ ___  _ __ ___   ___     / \\   __| |_ __ ___ (_)_ __   | | |\033[0m\n");
    printf("\033[32m\033[5m \\ \\ /\\ / / _ \\ |/ __/ _ \\| '_ ` _ \\ / _ \\   / _ \\ / _` | '_ ` _ \\| | '_ \\  | | |\033[0m\n");
    printf("\033[32m\033[5m  \\ V  V /  __/ | (_| (_) | | | | | |  __/  / ___ \\ (_| | | | | | | | | | | |_|_|\033[0m\n");
    printf("\033[32m\033[5m   \\_/\\_/ \\___|_|\\___\\___/|_| |_| |_|\\___| /_/   \\_\\__,_|_| |_| |_|_|_| |_| (_|_)\033[0m\n");   
}

// function for payment_splash screen
void payment_splash() {
    printf("\033[32m\033[5m ____                                  _   \033[0m\n");
    printf("\033[32m\033[5m|  _ \\ __ _ _   _ _ __ ___   ___ _ __ | |_ \033[0m\n");
    printf("\033[32m\033[5m| |_) / _` | | | | '_ ` _ \\ / _ \\ '_ \\| __|\033[0m\n");
    printf("\033[32m\033[5m|  __/ (_| | |_| | | | | | |  __/ | | | |_ \033[0m\n");
    printf("\033[32m\033[5m|_|   \\__,_|\\__, |_| |_| |_|\\___|_| |_|\\__|\033[0m\n");
    printf("\033[32m\033[5m            |___/                          \033[0m\n");     
}

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
        FILE *fp = fopen(file_name, "r+");

        // If the file doesn't exist, create it
        if (fp == NULL) {
            fp = fopen(file_name, "w");
            fprintf(fp, "Slot_Number,Vehicle_Number,Vehicle_Type,Date,Time_In,Time_Out\n");
            fclose(fp);
        }

        // Read lines from the file
        char *fileData = fgets(buffer, 1024, fp);
        if (fileData == NULL) {
            fprintf(fp, "Slot_Number,Vehicle_Number,Vehicle_Type,Date,Time_In,Time_Out\n");
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
    sleep(1);
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
    regex_t regex1, regex2;
    int reti1, reti2;
    char msgbuf[100];

    reti1 = regcomp(&regex1, "^[A-Z]{2}[0-9]{1,2}(?:[A-Z])?(?:[A-Z]*)?[0-9]{4}$",REG_EXTENDED);
    if (reti1) {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }

    reti2 = regcomp(&regex2, "^[0-9]{2}BH[0-9]{4}[A-Z]{2}$", REG_EXTENDED);
    if (reti2) {
        fprintf(stderr, "Could not compile regex\n");
        exit(1);
    }

    // Get the current date
    time_t t = time(NULL);
    struct tm tm = *localtime(&t);
    char *date = malloc(10);
    sprintf(date, "%d/%d/%d", tm.tm_mday, tm.tm_mon + 1, tm.tm_year + 1900);

    // Prompt the user to enter the vehicle number and vehicle type
    printf("Enter the vehicle number: ");
    char vehicleNumber[20];
    scanf("%s", vehicleNumber);
    reti1 = regexec(&regex1, vehicleNumber, 0, NULL, 0);
    reti2 = regexec(&regex2, vehicleNumber, 0, NULL, 0);
    while ((reti1 == REG_NOMATCH) && (reti2 == REG_NOMATCH)) {
        printf("Invalid vehicle number. Enter again: ");
        scanf("%s", vehicleNumber);
        reti1 = regexec(&regex1, vehicleNumber, 0, NULL, 0);
        reti2 = regexec(&regex2, vehicleNumber, 0, NULL, 0);
    }

    // Free the memory allocated for the regex
    regfree(&regex1);
    regfree(&regex2);

    printf("Enter the vehicle type: ");
    char vehicleType[20];
    scanf("%s", vehicleType);
    if (strcmp(vehicleType, "Car") != 0 && strcmp(vehicleType, "car") != 0 && strcmp(vehicleType, "Bike") != 0 && strcmp(vehicleType, "bike") != 0) {
        printf("Invalid vehicle type. Enter Car or Bike : ");
        scanf("%s", vehicleType);
    }
    //vehicleNumber[strlen(vehicleNumber)-3] = '\0';
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
    printf("Press any key to continue...\n");
    getch();
}

// Function to retrieve a vehicle
void retrieveVehicle(int num) //have to add the parameters
{   
    pthread_t thread4;
    float  fee;
    int pos = searchVehicle(parkingInfo, num);
    if (pos != -1) {
        strcpy(parkingInfo[pos].timeOut, getTime());
        int time = calculateTime(parkingInfo[pos].timeIn);
        
        // Calculate the fee based on vehicle type and time parked
        if (strcmp(parkingInfo[pos].vehicleType, "Car") == 0 || strcmp(parkingInfo[pos].vehicleType, "car") == 0) {
            if (time <= 60) {
                fee = 30.0;
            } else if (60 > time > 120) {
                fee = 40.0;
            } else if (120 > time > 180) {
                fee = 50.0;
            } else {
                fee = 70.0;
            }
        } else if (strcmp(parkingInfo[pos].vehicleType, "Bike") == 0 || strcmp(parkingInfo[pos].vehicleType, "bike") == 0) {
            if (time <= 60) {
                fee = 20.0;
            } else if (60 > time > 120) {
                fee = 30.0;
            } else if (120 > time > 180) {
                fee = 40.0;
            } else {
                fee = 60.0;
            }
        }
        
        printf("\033[2J\033[1;1H");
        payment_splash();
        printf("Slot: %d\n", parkingInfo[pos].parkingSlot.slotNumber);
        printf("Vehicle Number: %s\n", parkingInfo[pos].vehicleNumber);
        printf("Vehicle Type: %s\n", parkingInfo[pos].vehicleType);
        printf("Time In: %s\n", parkingInfo[pos].timeIn);
        printf("Time Out: %s\n", parkingInfo[pos].timeOut);
        printf("Amount to be paid: \033[4m%.2f\033[0m\n", fee);
        
        printf("\nPay the amount to the cashier.\n");
        printf("Thank you for using our service.\n\n");
        printf("Press any key to continue...\n");
        getch();
        printf("\033[2J\033[1;1H");
        
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

//function for parkingHistory
void parkingHistory() {
    DIR *dr;
   struct dirent *en;
   char var1[25], var2[25], var3[25], var4[25], var5[25], var6[25];
   dr = opendir("."); //open all directory
   if (dr) {
      while ((en = readdir(dr)) != NULL) {
         {
            char filename[100];
            char *first = strtok(en->d_name, ".");
            char *second = strtok(NULL, ".");
            if(second != NULL){
               if(strcmp(second, "csv") == 0)
               {
                  char *first_part = strtok(en->d_name, "_");
                  char *second_part = strtok(NULL, "_");
                  char *third_part = strtok(NULL, "_");
                  char *fourth_part = strtok(NULL, "_");
                  printf("\n\033[1m\033[4mVehicles parked on %s-%s-%s\033[0m\n", second_part, third_part, fourth_part);
                  sprintf(filename, "%s_%s_%s_%s.csv", first_part, second_part, third_part, fourth_part);

                  FILE* fp = fopen(filename, "r");
               
                  if (!fp)
                     printf("Can't open file\n");

                  else {
                     char buffer[1024];

                     int row = 0;
                     printf("\n\033[1mSlot\t  RegN\t\tType\t  Date\t\tTime In\tTime Out\033[0m\n");
                     printf("---------------------------------------------------------------\n"); 
                     while (fgets(buffer,1024, fp)) {
                        row++;

                        // To avoid printing of column
                        // names in file can be changed
                        // according to need
                        if (row == 1)
                           continue;

                        // Splitting the data
                        char* value = strtok(buffer, ", ");

                        while (value) {
                           printf("%s\t", value);
                           value = strtok(NULL, ", ");
                        }

                        printf("\n");
                     }

                     // Close the file
                     fclose(fp);
                  }
               } 
            }
         }
      }
   }    
   closedir(dr); //close all directory
   printf("Press any key to continue...\n");
   getch();
}
    

//function for the admin menu
void admin()
{
    // Declare a boolean variable to control the loop
    bool loop = true;

    // Start of the loop
    do {
        printf("\033[2J\033[1;1H");
        admin_splash();
        // Print the menu options
        printf("\n\n1. View Parking Info\n");
        printf("2. View Parking History\n");
        printf("3. Exit\n");

        // Prompt the user to enter their choice
        printf("Enter your choice: ");

        // Declare a variable to hold the user's choice
        int choice;

        // Read the user's choice and check if it's an integer
        int result = scanf("%d", &choice);
        if (result < 1) {
            // If the input is not an integer, print an error message
            invalidInput();
            // Clear the input buffer
            while(getchar() != '\n');
        } else {
            // Process the user's choice
            if (choice == 1) {
                int parkCount=0;
                printf("\033[2J\033[1;1H");
                for (int i = 0; i < 200; i++) {
                    if (parkingInfo[i].parkingSlot.isOccupied == true) {
                        printf("%d\t\t%s\t\t%s\t\t%s\t%s\t%s\n", parkingInfo[i].parkingSlot.slotNumber, parkingInfo[i].vehicleNumber, parkingInfo[i].vehicleType, parkingInfo[i].date, parkingInfo[i].timeIn, parkingInfo[i].timeOut);
                        parkCount++;
                    }
                }
                printf("Vehile Count: %d\n", parkCount);
                printf("Press any key to continue...\n");
                getch();
            } else if (choice == 2) {
                printf("\033[2J\033[1;1H");
                parkingHistory();
            } else if (choice == 3) {
                printf("\033[2J\033[1;1H");
                loop = false;
            } else {
                // If the choice is not 1, 2, or 3, print an error message
                printf("Invalid choice. Enter 1 or 2 or 3\n");
            }
        }
    } while (loop); // End of the loop
}

// Main function
void main()
{
    // Code inside the block

    // Declare a boolean variable to control the loop
    bool loop = true;
    
    pthread_create(&thread, NULL, fileChecker, NULL);

    // Start of the loop
    do {
        printf("\033[2J\033[1;1H");
        welcome_splash();

        // Print the menu options
        printf("\n\n1. Park\n");
        printf("2. Retrieve Car from Parking\n");
        printf("3. Exit\n");

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
                parking_splash();
                parkVehicle(parkingInfo);
            } else if (choice == 2) {
                printf("\033[2J\033[1;1H");
                printf("Enter the slot number: ");
                int slotNumber;
                scanf("%d", &slotNumber);
                retrieveVehicle(slotNumber);
            } else if (choice == 3){
                printf("\033[2J\033[1;1H");
                loop = false;
            } else if (choice == 101) {
                printf("\033[2J\033[1;1H");
                welcome_splash();
                char ch[10];
                scanf("%s", ch);
                if (strcmp(ch, "aie") == 0) {
                    printf("\033[2J\033[1;1H");
                    admin();
                    printf("\033[2J\033[1;1H");
                } 
            }else {
                // If the choice is not 1, 2, 3, or 4, print an error message
                printf("Invalid choice. Enter 1 or 2 or 3 or 4\n");
            }
        }
    } while (loop); // End of the loop   
    pthread_join(thread, NULL);
    pthread_join(thread2, NULL);
    pthread_join(thread3, NULL);
}
