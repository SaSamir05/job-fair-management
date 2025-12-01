#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_JOBS 100
#define MAX_APPLICATIONS 500

typedef struct {
    int id;
    char title[100];
    char company[100];
    char location[100];
    float salary;
    char requirements[200];
    char description[300];
    int vacancies;
} Job;

typedef struct {
    int id;
    char name[100];
    char email[100];
    char phone[20];
    int job_id;
    char status[20];
} Application;

// Function prototypes
bool admin_login();
void create_job();
void view_jobs();
void view_applications();
void delete_job();

int main() {
    printf("\n=== ADMIN PANEL ===\n");
    
    if (!admin_login()) {
        printf("Invalid credentials!\n");
        return 1;
    }
    
    int choice;
    do {
        printf("\n1. Create Job\n");
        printf("2. View All Jobs\n");
        printf("3. View Applications\n");
        printf("4. Delete Job\n");
        printf("5. Logout\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();
        
        switch(choice) {
            case 1:
                create_job();
                break;
            case 2:
                view_jobs();
                break;
            case 3:
                view_applications();
                break;
            case 4:
                delete_job();
                break;
            case 5:
                printf("Logging out...\n");
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 5);
    
    return 0;
}

bool admin_login() {
    char username[50], password[50];
    printf("Username: ");
    fgets(username, 50, stdin);
    username[strcspn(username, "\n")] = 0;
    
    printf("Password: ");
    fgets(password, 50, stdin);
    password[strcspn(password, "\n")] = 0;
    
    // Simple authentication (username: admin, password: admin123)
    return (strcmp(username, "admin") == 0 && strcmp(password, "admin123") == 0);
}

void create_job() {
    Job job;
    static int job_id = 1;
    
    printf("\n=== CREATE NEW JOB ===\n");
    job.id = job_id++;
    
    printf("Job Title: ");
    fgets(job.title, 100, stdin);
    job.title[strcspn(job.title, "\n")] = 0;
    
    printf("Company: ");
    fgets(job.company, 100, stdin);
    job.company[strcspn(job.company, "\n")] = 0;
    
    printf("Location: ");
    fgets(job.location, 100, stdin);
    job.location[strcspn(job.location, "\n")] = 0;
    
    printf("Salary: ");
    scanf("%f", &job.salary);
    getchar();
    
    printf("Requirements: ");
    fgets(job.requirements, 200, stdin);
    job.requirements[strcspn(job.requirements, "\n")] = 0;
    
    printf("Description: ");
    fgets(job.description, 300, stdin);
    job.description[strcspn(job.description, "\n")] = 0;
    
    printf("Vacancies: ");
    scanf("%d", &job.vacancies);
    getchar();
    
    // Save to file
    FILE *fp = fopen("job_data.txt", "a");
    if (fp != NULL) {
        fprintf(fp, "%d|%s|%s|%s|%.2f|%s|%s|%d\n", 
                job.id, job.title, job.company, job.location, 
                job.salary, job.requirements, job.description, job.vacancies);
        fclose(fp);
        printf("Job created successfully!\n");
    } else {
        printf("Error saving job!\n");
    }
}

void view_jobs() {
    printf("\n=== ALL JOBS ===\n");
    FILE *fp = fopen("job_data.txt", "r");
    if (fp == NULL) {
        printf("No jobs found!\n");
        return;
    }
    
    Job job;
    char line[500];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d|%[^|]|%[^|]|%[^|]|%f|%[^|]|%[^|]|%d", 
               &job.id, job.title, job.company, job.location, 
               &job.salary, job.requirements, job.description, &job.vacancies);
        
        printf("ID: %d\n", job.id);
        printf("Title: %s\n", job.title);
        printf("Company: %s\n", job.company);
        printf("Location: %s\n", job.location);
        printf("Salary: $%.2f\n", job.salary);
        printf("Vacancies: %d\n", job.vacancies);
        printf("Requirements: %s\n", job.requirements);
        printf("Description: %s\n", job.description);
        printf("------------------------\n");
    }
    fclose(fp);
}

void view_applications() {
    printf("\n=== ALL APPLICATIONS ===\n");
    FILE *fp = fopen("applications.txt", "r");
    if (fp == NULL) {
        printf("No applications found!\n");
        return;
    }
    
    Application app;
    char line[500];
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d|%[^|]|%[^|]|%[^|]|%d|%s", 
               &app.id, app.name, app.email, app.phone, &app.job_id, app.status);
        
        printf("Application ID: %d\n", app.id);
        printf("Name: %s\n", app.name);
        printf("Email: %s\n", app.email);
        printf("Phone: %s\n", app.phone);
        printf("Job ID: %d\n", app.job_id);
        printf("Status: %s\n", app.status);
        printf("------------------------\n");
    }
    fclose(fp);
}

void delete_job() {
    int job_id;
    printf("Enter Job ID to delete: ");
    scanf("%d", &job_id);
    getchar();
    
    FILE *fp = fopen("job_data.txt", "r");
    FILE *temp = fopen("temp.txt", "w");
    
    if (fp == NULL || temp == NULL) {
        printf("Error opening files!\n");
        return;
    }
    
    Job job;
    char line[500];
    bool found = false;
    
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d|%[^|]|%[^|]|%[^|]|%f|%[^|]|%[^|]|%d", 
               &job.id, job.title, job.company, job.location, 
               &job.salary, job.requirements, job.description, &job.vacancies);
        
        if (job.id != job_id) {
            fprintf(temp, "%s", line);
        } else {
            found = true;
        }
    }
    
    fclose(fp);
    fclose(temp);
    
    if (found) {
        remove("job_data.txt");
        rename("temp.txt", "job_data.txt");
        printf("Job deleted successfully!\n");
    } else {
        remove("temp.txt");
        printf("Job not found!\n");
    }
}