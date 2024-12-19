#include <stdio.h>
#include <string.h>

#define MAX_SEATS 20          // Maximum number of seats per route per time slot
#define MAX_USERS 20          // Maximum number of users
#define MORNING 0             // Constant for morning time slot
#define EVENING 1             // Constant for evening time slot

char *city[] = {"Delhi", "Bombay", "Kolkata", "Jaipur", "Chennai"};   // List of cities
int timeM[] = {6, 8, 10, 11};    // Morning flight times
int timeE[] = {1, 3, 5, 8};     // Evening flight times

// Struct to store user details for booking
typedef struct user_details {
    char Name[50];              // User's full name
    int Age;                    // User's age
    char Ph[15];                // User's phone number
    int source;                 // Source city index (0-4)
    int destination;            // Destination city index (0-4)
    char time[10];              // Time of flight ("Morning" or "Evening")
    int flight_time;            // Flight time (from the list of times)
} user_details;

user_details all_users[MAX_USERS];  // Array to store all users
int user_count = 0;                  // Current count of users

int seats[5][5][2];                  // 3D array to store seat availability (cities x cities x timeslot)

void InitializeSeats() {
    // Initialize the seats for all routes and time slots
    for (int i = 0; i < 5; i++) {
        for (int j = 0; j < 5; j++) {
            if (i != j) {
                seats[i][j][MORNING] = MAX_SEATS; // Available seats in the morning
                seats[i][j][EVENING] = MAX_SEATS; // Available seats in the evening
            } else {
                seats[i][j][MORNING] = 0; // No route to the same city
                seats[i][j][EVENING] = 0;
            }
        }
    }
}

// Function to save data (user details and seats availability) to a file
void SaveData() {
    FILE *fp = fopen("bookings.dat", "wb");
    if (fp) {
        fwrite(&user_count, sizeof(int), 1, fp);   // Write user count
        fwrite(all_users, sizeof(user_details), user_count, fp);  // Write all users data
        fwrite(seats, sizeof(seats), 1, fp);  // Write updated seats data
        fclose(fp);
    }
}

// Function to load data (user details and seats availability) from a file
void LoadData() {
    FILE *fp = fopen("bookings.dat", "rb");
    if (fp) {
        fread(&user_count, sizeof(int), 1, fp);  // Read user count
        fread(all_users, sizeof(user_details), user_count, fp);  // Read all users data
        fread(seats, sizeof(seats), 1, fp);  // Read seats data
        fclose(fp);
    } else {
        InitializeSeats();  // Initialize the seats if no file exists
    }
}

// Function to book seats on a particular route and time slot
int Bookseats(int source, int destination, int time_slot) {
    int bseats;

    printf("Available seats for this route: %d\n", seats[source][destination][time_slot]);
    printf("Enter the number of seats you want to book: ");
    scanf("%d", &bseats);

    // Check if enough seats are available
    if (bseats > seats[source][destination][time_slot]) {
        printf("Not enough seats available! Only %d seats left.\n", seats[source][destination][time_slot]);
        return 0;
    }

    // Deduct booked seats from the available seats
    seats[source][destination][time_slot] -= bseats;
    printf("Seats booked successfully! Remaining seats for this route: %d\n", seats[source][destination][time_slot]);
    return bseats;
}

// Function to cancel an existing booking
void Cancel() {
    char name[50], phone[15];
    int found = 0;

    // Get user input for booking cancellation
    printf("Enter the full name for cancellation: ");
    getchar();  // Clear newline character
    fgets(name, sizeof(name), stdin);
    name[strcspn(name, "\n")] = 0;  // Remove trailing newline
    printf("Enter the phone number for cancellation: ");
    scanf("%s", phone);

    // Search for the user and confirm cancellation
    for (int i = 0; i < user_count; i++) {
        if (strcmp(all_users[i].Name, name) == 0 && strcmp(all_users[i].Ph, phone) == 0) {
            int source = all_users[i].source;
            int destination = all_users[i].destination;
            int time_slot = (strcmp(all_users[i].time, "Morning") == 0) ? MORNING : EVENING;

            // Show booking details and confirm cancellation
            printf("Booking found for %s:\n", name);
            printf("Name: %s, Age: %d, Phone: %s, From: %s, To: %s, Time: %s %d\n",
                   all_users[i].Name, all_users[i].Age, all_users[i].Ph,
                   city[source], city[destination],
                   all_users[i].time, all_users[i].flight_time);

            printf("Are you sure you want to cancel this booking? (1 for Yes, 0 for No): ");
            int confirm;
            scanf("%d", &confirm);

            if (confirm == 1) {
                // If confirmed, add the seats back to availability
                seats[source][destination][time_slot]++;
                // Remove the user from the array
                for (int j = i; j < user_count - 1; j++) {
                    all_users[j] = all_users[j + 1];
                }
                user_count--;
                printf("Booking successfully canceled. Remaining seats for this route: %d\n", seats[source][destination][time_slot]);
                SaveData();  // Save updated data
                found = 1;
                break;
            } else {
                printf("Cancellation aborted.\n");
                found = 1;
                break;
            }
        }
    }

    if (!found) {
        printf("No booking found with the name %s and phone number %s.\n", name, phone);
    }
}

// Function to handle the booking process
void Book() {
    int source, destination, number, seats_to_book;
    char time[10];

    // Get source city
    printf("From:\n");
    for (int i = 0; i < 5; i++) {
        printf("%d. %s\n", i + 1, city[i]);
    }

    printf("Enter your choice (1-5): ");
    scanf("%d", &source);

    // Validate source input
    if (source < 1 || source > 5) {
        printf("Invalid source choice.\n");
        return;
    }

    // Get destination city
    printf("To:\n");
    for (int i = 0; i < 5; i++) {
        if (i != source - 1) {
            printf("%d. %s\n", i + 1, city[i]);
        }
    }

    printf("Enter your choice (1-5, excluding %d): ", source);
    scanf("%d", &destination);

    // Validate destination input
    if (destination < 1 || destination > 5 || destination == source) {
        printf("Invalid destination choice.\n");
        return;
    }

    // Get time slot choice
    printf("Time slot selection: 1 for Morning, 2 for Evening: ");
    int time_slot_choice;
    scanf("%d", &time_slot_choice);

    // Set time based on choice
    if (time_slot_choice == 1) {
        strcpy(time, "Morning");
    } else if (time_slot_choice == 2) {
        strcpy(time, "Evening");
    } else {
        printf("Invalid time slot selection.\n");
        return;
    }

    int time_slot = (strcmp(time, "Morning") == 0) ? MORNING : EVENING;

    // Show preferred timings based on selected time slot
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
    printf("\nYour preferred time is: ");
    scanf("%d", &number);

    // Call Bookseats function to book seats
    seats_to_book = Bookseats(source - 1, destination - 1, time_slot);
    for (int i = 0; i < seats_to_book; i++) {
        if (user_count < MAX_USERS) {
            user_details user;
            printf("Enter details for passenger %d:\n", i + 1);
            printf("Full Name: ");
            getchar();  // Clear newline
            fgets(user.Name, sizeof(user.Name), stdin);
            user.Name[strcspn(user.Name, "\n")] = 0;  // Remove trailing newline
            printf("Age: ");
            scanf("%d", &user.Age);
            printf("Phone number: ");
            scanf("%s", user.Ph);
            user.source = source - 1;
            user.destination = destination - 1;
            strcpy(user.time, time);
            user.flight_time = number;
            all_users[user_count] = user;
            user_count++;
            SaveData();
        }
    }
}

int main() {
    int choice;
    LoadData();  // Load data from file

    while (1) {
        printf("\nMenu:\n");
        printf("1. Book Ticket\n");
        printf("2. Cancel Booking\n");
        printf("3. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        if (choice == 1) {
            Book();
        } else if (choice == 2) {
            Cancel();
        } else if (choice == 3) {
            printf("Exiting program.\n");
            break;
        } else {
            printf("Invalid choice. Please try again.\n");
        }
    }

    return 0;
}
