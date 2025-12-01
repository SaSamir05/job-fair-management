#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Function prototypes
void admin_panel();
void user_panel();
void initialize_files();

int main() {
    printf("=== JOB FAIR MANAGEMENT SYSTEM ===\n");
    printf("1. Admin Panel\n");
    printf("2. User Panel\n");
    printf("3. Exit\n");
    
    int choice;
    printf("Enter your choice: ");
    scanf("%d", &choice);
    getchar(); // Clear buffer
    
    switch(choice) {
        case 1:
            admin_panel();
            break;
        case 2:
            user_panel();
            break;
        case 3:
            printf("Thank you for using Job Fair Management System!\n");
            exit(0);
        default:
            printf("Invalid choice!\n");
    }
    
    return 0;
}

void admin_panel() {
    system("gcc admin.c -o admin && ./admin");
}

void user_panel() {
    system("gcc user.c -o user && ./user");
}

void initialize_files() {
    // This will be called to ensure all necessary files exist
    system("touch job_data.txt applications.txt admin_credentials.txt");
}