The program implements an Flight Management System in C, providing functionalities 
to manage ticket bookings, view bookings, cancel bookings, and update user details. It 
employs structured programming with the help of user-defined structures, file handling 
for data persistence, and a menu-driven interface. 
Key Components 
1. Data Structures: 
o user_details: A structure to store individual passenger details such as 
name, age, phone number, source, destination, preferred time slot, and 
f
 light time. 
o Arrays:  
▪ all_users[MAX_USERS]: Stores details of all booked passengers. 
▪ seats[5][5][2]: Maintains seat availability for each route and time 
slot. 
2. File Handling: 
o Persistent storage is implemented using bookings.dat. The program saves 
and loads user data and seat availability at the beginning and end of the 
program. 
3. City and Time Management: 
o The program supports 5 cities and two time slots (Morning and Evening). 
Predefined arrays (timeM and timeE) handle flight timings. 
Functions Overview 
1. InitializeSeats: 
o Initializes the seats array to the maximum capacity for valid source
destination routes. 
seats[i][j][MORNING] = MAX_SEATS; 
seats[i][j][EVENING] = MAX_SEATS; 
o Ensures routes within the same city are disabled. 
2. SaveData and LoadData: 
o Save and retrieve user and seat data from a file using binary file handling. 
o Ensures data is not lost between program runs. 
3. Bookseats: 
o Handles seat booking logic by verifying availability and updating the seats 
array: 
seats[source][destination][time_slot] -= bseats; 
o Prompts users for the number of seats they want to book and ensures 
they do not exceed available capacity. 
4. Book: 
o Primary booking function:  
▪ Prompts for source, destination, time slot, and preferred flight 
time. 
▪ Calls Bookseats to confirm seat availability. 
▪ Collects user details and stores them in the all_users array. 
5. Cancel: 
o Cancels an existing booking by searching for a matching name and phone 
number. 
o Restores the seats to the seats array: 
seats[source][destination][time_slot]++; 
o Removes the user from all_users. 
6. View: 
o Provides two ways to view bookings:  
▪ By passenger name and phone number. 
▪ By specific route and time slot. 
7. UpdateUserDetails: 
o Allows updating the passenger's age and phone number: 
scanf("%d", &all_users[i].Age); 
scanf("%s", all_users[i].Ph); 
o Ensures user records remain up to date. 
8. main: 
o Implements a menu-driven interface, providing seamless access to all 
features. 
o Loads data at the start and saves data before exiting. 
Unique Features 
1. Robust Seat Management: 
o Seat availability is dynamically updated for each route and time slot. 
o Ensures no overbooking and maintains integrity with file storage. 
2. File Persistence: 
o Uses fwrite and fread for efficient binary file storage, allowing recovery of 
data between runs. 
3. Route Validations: 
o Prevents booking or viewing for invalid source-destination pairs. 
if (destination == source) { 
printf("Invalid destination choice.\n"); 
return; 
} 
4. User-Friendly Interface: 
o Includes options for booking, viewing, canceling, and updating details. 
o Provides clear prompts and error messages. 
Highlighted Code Snippets 
1. Seat Deduction During Booking: 
seats[source][destination][time_slot] -= bseats; 
printf("Seats booked successfully! Remaining seats: %d\n", 
seats[source][destination][time_slot]); 
o Ensures seats are deducted only after validation. 
2. Restoring Seats During Cancellation: 
seats[source][destination][time_slot]++; 
printf("Seats successfully restored. Remaining seats: %d\n", 
seats[source][destination][time_slot]); 
3. File Handling: 
FILE *fp = fopen("bookings.dat", "wb"); 
fwrite(&user_count, sizeof(int), 1, fp); 
fwrite(all_users, sizeof(user_details), user_count, fp); 
fwrite(seats, sizeof(seats), 1, fp); 
fclose(fp); 
4. Menu Navigation: 
while (1) { 
printf("Airline Reservation System Menu:\n"); 
printf("1. Book Ticket\n2. View Booking\n3. Cancel Booking\n4. Update User 
Details\n5. Exit\n"); 
scanf("%d", &choice); 
switch (choice) { 
case 1: Book(); break; 
case 2: View(); break; 
case 3: Cancel(); break; 
case 4: UpdateUserDetails(); break; 
case 5: SaveData(); return 0; 
default: printf("Invalid choice! Please try again.\n"); 
} 
} 
Conclusion 
This program effectively manages airline ticket reservations with a focus on data 
persistence, user validation, and efficient seat allocation. The use of structured 
programming and clear separation of concerns makes it both modular and easy to 
extend.
