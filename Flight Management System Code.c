#include <stdio.h>
#include <string.h>

#define MAX_SEATS 20  // Maximum number of seats available for each flight on any given route
#define MAX_USERS 20  // The maximum number of users the system can handle at any time
#define MORNING 0     // Identifier for morning flights
#define EVENING 1     // Identifier for evening flights

// A list of cities that can be selected as departure and destination points
char *city[] = {"Delhi", "Bombay", "Kolkata", "Jaipur", "Chennai"};
// Predefined times for morning flights
int timeM[] = {6, 8, 10, 11};
// Predefined times for evening flights
int timeE[] = {1, 3, 5, 8};

// A structure that holds details about the user's booking
typedef struct user_details {
    char Name[50];      // Name of the user making the booking
    int Age;            // User's age
    char Ph[15];        // User's phone number
    int source;         // Index of the source city (0-4)
    int destination;    // Index of the destination city (0-4)
    char time[10];      // Time of the flight ("Morning" or "Evening")
    int flight_time;    // The exact flight time (from the morning or evening list)
} user_details;

user_details all_users[MAX_USERS]; // Array to store user details
int user_count = 0;                // To track how many users have booked so far

// This is a 3D array that keeps track of available seats on each route for each time slot
int seats[5][5][2];     // 5 cities x 5 destinations x 2 time slots (Morning and Evening)

// Function to initialize the seats to their maximum available count when the program starts
void InitializeSeats() {
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (i != j) {
                seats[i][j][MORNING] = MAX_SEATS; // Set max seats for morning flights
                seats[i][j][EVENING] = MAX_SEATS; // Set max seats for evening flights
            } else {
                seats[i][j][MORNING] = 0; // No flights to the same city
                seats[i][j][EVENING] = 0;
            }
        }
    }
}

// This function is used to save all the data (bookings, seat availability) into a file
void SaveData() {
    FILE *fp = fopen("bookings.dat", "wb");
    if (fp) {
        fwrite(&user_count, sizeof(int), 1, fp);                    // Save the number of users who have made bookings
        fwrite(all_users, sizeof(user_details), user_count, fp);    // Save user booking details
        fwrite(seats, sizeof(seats), 1, fp);                        // Save the seat availability data
        fclose(fp);
    }
}

// This function loads previously saved data, such as user details and seat availability, from a file
void LoadData() {
    FILE *fp = fopen("bookings.dat", "rb");
    if (fp) {
        fread(&user_count, sizeof(int), 1, fp);                     // Load the number of users
        fread(all_users, sizeof(user_details), user_count, fp);     // Load the user details
        fread(seats, sizeof(seats), 1, fp);                         // Load seats data
        fclose(fp);
    } else {
        InitializeSeats();
    }
}

// This function is called when a user wants to book seats. It checks if seats are available and then updates the data.
int Bookseats(int source, int destination, int time_slot) {
    int bseats;

     // Display how many seats are available for this particular route and time slot
    printf("Available seats for this route: %d\n", seats[source][destination][time_slot]);
    printf("Enter the number of seats you want to book: ");
    scanf("%d", &bseats);

    // If the user wants to book more seats than are available, show an error
    if (bseats > seats[source][destination][time_slot]) {
        printf("Not enough seats available! Only %d seats left.\n", seats[source][destination][time_slot]);     
        return 0;       // Return 0 if there aren't enough seats
    }
    
     // If there are enough seats, deduct the booked seats from the available total
    seats[source][destination][time_slot] -= bseats; // Deduct seats
    printf("Seats booked successfully! Remaining seats for this route: %d\n", seats[source][destination][time_slot]);
    return bseats;   // Return the number of seats booked
}

// Function to cancel a booking based on the user's details (name and phone number)
void Cancel() {
    char name[50], phone[15];
    int found = 0;

    // Ask the user for the details of the booking they want to cancel
    printf("Enter the full name for cancellation: ");
    getchar();  // Clear newline
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;  // Remove any trailing newline character from the input
    printf("Enter the phone number for cancellation: ");
    scanf("%s", phone);

    
    // Look through the list of bookings to find the correct user
    for (int i = 0; i < user_count; i++) {
        if (strcmp(all_users[i].Name, name) == 0 && strcmp(all_users[i].Ph, phone) == 0) {
            int source = all_users[i].source;
            int destination = all_users[i].destination;
            int time_slot = (strcmp(all_users[i].time, "Morning") == 0) ? MORNING : EVENING;

            // If found, display the booking details and ask for confirmation to cancel
            printf("Booking found for %s:\n", name);
            printf("Name: %s, Age: %d, Phone: %s, From: %s, To: %s, Time: %s %d\n",
                   all_users[i].Name, all_users[i].Age, all_users[i].Ph,
                   city[source], city[destination],
                   all_users[i].time, all_users[i].flight_time);

            printf("Are you sure you want to cancel this booking? (1 for Yes, 0 for No): ");
            int confirm;
            scanf("%d", &confirm);

            // If the user confirms, cancel the booking and restore the seats
            if (confirm == 1) {
                seats[source][destination][time_slot]++; 
                // Remove the user from the list of bookings
                for (int j = i; j < user_count - 1; j++) {
                    all_users[j] = all_users[j + 1];
                }
                user_count--; // Decrease the user count
                printf("Booking successfully canceled. Remaining seats for this route: %d\n", seats[source][destination][time_slot]);
                SaveData();   // Save the updated data to the file
                found = 1;
                break;
            } else {
                printf("Cancellation aborted.\n");
                found = 1;
                break;
            }
        }
    }
    // If the booking is not found, inform the user
    if (!found) {
        printf("No booking found with the name %s and phone number %s.\n", name, phone);
    }
}

// This is the function that handles the entire booking process for a user
void Book() {
    int source, destination, number, seats_to_book;
    char time[10];

    printf("From:\n");
    for (int i = 0; i < 5; i++) {
        printf("%d. %s\n", i + 1, city[i]);
    }

    // Ask the user to select the source city for their flight
    printf("Enter your choice (1-5): ");
    scanf("%d", &source);

    // Ensure the selected source city is valid
    if (source < 1 || source > 5) {
        printf("Invalid source choice.\n");
        return;
    }

    printf("To:\n");
    for (int i = 0; i < 5; i++) {
        if (i != source - 1) {  // Prevent choosing the same city as source
            printf("%d. %s\n", i + 1, city[i]);
        }
    }

    // Ask the user to select the destination city
    printf("Enter your choice (1-5, excluding %d): ", source);
    scanf("%d", &destination);

    // Ensure the selected destination city is valid
    if (destination < 1 || destination > 5 || destination == source) {
        printf("Invalid destination choice.\n");    
        return;
    }

    // Ask the user to select the time slot for their flight
    printf("Time slot selection: 1 for Morning, 2 for Evening: ");
    int time_slot_choice;
    scanf("%d", &time_slot_choice);

    // Set the time of the flight based on user input
    if (time_slot_choice == 1) {
        strcpy(time, "Morning");
    } else if (time_slot_choice == 2) {
        strcpy(time, "Evening");
    } else {
        printf("Invalid time slot selection.\n");
        return;
    }

    int time_slot = (strcmp(time, "Morning") == 0) ? MORNING : EVENING;

    // Show the available times for the chosen time slot
    printf("Select preferred timing: ");
    if (time_slot == MORNING) {
        for (int i = 0; i < 4; i++) {
            printf("%d ", timeM[i]);
        }
    } else {
        for (int i = 0; i < 4; i++) {
            printf("%d ", timeE[i]);
        }
    }

    // Let the user select the time
    printf("\nYour preferred time is: ");
    scanf("%d", &number);

    // Book the seats if available
    seats_to_book = Bookseats(source - 1, destination - 1, time_slot);
    for (int i = 0; i < seats_to_book; i++) {
        if (user_count < MAX_USERS) {
            user_details user;
            printf("Enter details for passenger %d:\n", i + 1);
            printf("Full Name: ");
            getchar();  // Clear newline from previous input
            fgets(user.Name, sizeof(user.Name), stdin);
            user.Name[strcspn(user.Name, "\n")] = 0;  // Remove newline from name
            printf("Age: ");
            scanf("%d", &user.Age);
            printf("Phone Number: ");
            scanf("%s", user.Ph);

            user.source = source - 1;
            user.destination = destination - 1;
            strcpy(user.time, time);
            user.flight_time = number;

            all_users[user_count++] = user;     // Increment the user count
        } else {
            printf("User limit reached! Cannot store more users.\n");
            break;
        }
    }
    SaveData();     // Save updated booking data to file
}

//Function to View User details
void View() {
    printf("View Bookings Menu:\n");
    printf("1. View by Passenger Name and Phone\n"); //View Using Name and Phone Number
    printf("2. View by Route and Time\n"); // View Using Route and Timings
    printf("Enter your choice: ");
    int choice;
    scanf("%d", &choice);

    if (choice == 1) {
        char name[50], phone[15];
        int found = 0;

        printf("Enter the full name: ");
        getchar();  
        fgets(name, sizeof(name), stdin);
        name[strcspn(name, "\n")] = 0;  

        printf("Enter the phone number: ");
        scanf("%s", phone);

        for (int i = 0; i < user_count; i++) {
            if (strcasecmp(all_users[i].Name, name) == 0 && strcmp(all_users[i].Ph, phone) == 0) {
                printf("Booking found for %s:\n", name);
                printf("Name: %s, Age: %d, Phone: %s, From: %s, To: %s, Time: %s %d\n",
                       all_users[i].Name, all_users[i].Age, all_users[i].Ph,
                       city[all_users[i].source], city[all_users[i].destination],
                       all_users[i].time, all_users[i].flight_time);
                found = 1;
                break;
            }
        }

        if (!found) {
            printf("No booking found for this passenger.\n");
        }
    }
    else if (choice == 2) {
        int source, destination;
        printf("Enter the source city number: ");
        scanf("%d", &source);
        printf("Enter the destination city number: ");
        scanf("%d", &destination);

        if (source < 1 || destination < 1 || source > 5 || destination > 5 || source == destination) {
            printf("Invalid route!\n");
            return;
        }

        printf("Bookings for route %s to %s:\n", city[source - 1], city[destination - 1]);
        for (int i = 0; i < user_count; i++) {
            if (all_users[i].source == source - 1 && all_users[i].destination == destination - 1) {
                printf("Name: %s, Age: %d, Phone: %s, Time: %s %d\n", 
                       all_users[i].Name, all_users[i].Age, all_users[i].Ph, 
                       all_users[i].time, all_users[i].flight_time);
            }
        }
    }
}

void UpdateUserDetails() {
    char name[50], phone[15];
    int found = 0;

    printf("Enter the full name to update details: ");
    getchar();  // Clear newline
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;  // Remove trailing newline

    printf("Enter the phone number to update details: ");
    scanf("%s", phone);

    for (int i = 0; i < user_count; i++) {
        if (strcmp(all_users[i].Name, name) == 0 && strcmp(all_users[i].Ph, phone) == 0) {
            printf("Current details for %s:\n", name);
            printf("Name: %s, Age: %d, Phone: %s, From: %s, To: %s, Time: %s %d\n", 
                   all_users[i].Name, all_users[i].Age, all_users[i].Ph, 
                   city[all_users[i].source], city[all_users[i].destination], 
                   all_users[i].time, all_users[i].flight_time);
            
            printf("Enter new Age: ");
            scanf("%d", &all_users[i].Age);

            printf("Enter new Phone Number: ");
            scanf("%s", all_users[i].Ph);

            printf("User details updated successfully!\n");
            SaveData();
            found = 1;
            break;
        }
    }

    if (!found) {
        printf("No booking found for the given name and phone number.\n");
    }
}

// Main function to run the program's menu and interact with the user
int main() {
    LoadData();  // Load saved data from the file when the program starts
    int choice;

    // Infinite loop to allow the user to choose actions
    while (1) {
        printf("\nAirline Reservation System Menu:\n");
        printf("1. Book Ticket\n");
        printf("2. View Booking\n");
        printf("3. Cancel Booking\n");
        printf("4. Update User Details\n");
        printf("5. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                Book();
                break;
            case 2:
                View();
                break;
            case 3:
                Cancel();
                break;
            case 4:
                UpdateUserDetails();
                break;
            case 5:
                SaveData();
                printf("Exiting the program. Goodbye!\n");
                return 0;
            default:
                printf("Invalid choice! Please try again.\n");
        }
    }

return 0;
}
