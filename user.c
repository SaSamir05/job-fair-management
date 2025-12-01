#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#define MAX_JOBS 100

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
void browse_jobs();
void apply_job();
void search_jobs();
void filter_by_salary();

int main() {
    printf("\n=== USER PANEL ===\n");
    
    int choice;
    do {
        printf("\n1. Browse All Jobs\n");
        printf("2. Search Jobs\n");
        printf("3. Filter by Salary Range\n");
        printf("4. Apply for Job\n");
        printf("5. Exit\n");
        printf("Enter choice: ");
        scanf("%d", &choice);
        getchar();
        
        switch(choice) {
            case 1:
                browse_jobs();
                break;
            case 2:
                search_jobs();
                break;
            case 3:
                filter_by_salary();
                break;
            case 4:
                apply_job();
                break;
            case 5:
                printf("Thank you for using Job Fair System!\n");
                break;
            default:
                printf("Invalid choice!\n");
        }
    } while (choice != 5);
    
    return 0;
}

void browse_jobs() {
    printf("\n=== AVAILABLE JOBS ===\n");
    FILE *fp = fopen("job_data.txt", "r");
    if (fp == NULL) {
        printf("No jobs available!\n");
        return;
    }
    
    Job job;
    char line[500];
    int count = 0;
    
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d|%[^|]|%[^|]|%[^|]|%f|%[^|]|%[^|]|%d", 
               &job.id, job.title, job.company, job.location, 
               &job.salary, job.requirements, job.description, &job.vacancies);
        
        if (job.vacancies > 0) {
            printf("ID: %d | Title: %s | Company: %s | Salary: $%.2f | Location: %s\n", 
                   job.id, job.title, job.company, job.salary, job.location);
            count++;
        }
    }
    
    if (count == 0) {
        printf("No jobs available currently!\n");
    }
    
    fclose(fp);
}

void search_jobs() {
    char keyword[100];
    printf("Enter search keyword (job title or company): ");
    fgets(keyword, 100, stdin);
    keyword[strcspn(keyword, "\n")] = 0;
    
    printf("\n=== SEARCH RESULTS ===\n");
    FILE *fp = fopen("job_data.txt", "r");
    if (fp == NULL) {
        printf("No jobs available!\n");
        return;
    }
    
    Job job;
    char line[500];
    bool found = false;
    
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d|%[^|]|%[^|]|%[^|]|%f|%[^|]|%[^|]|%d", 
               &job.id, job.title, job.company, job.location, 
               &job.salary, job.requirements, job.description, &job.vacancies);
        
        if ((strstr(job.title, keyword) || strstr(job.company, keyword)) && job.vacancies > 0) {
            printf("ID: %d | Title: %s | Company: %s | Salary: $%.2f\n", 
                   job.id, job.title, job.company, job.salary);
            found = true;
        }
    }
    
    if (!found) {
        printf("No jobs found matching your search!\n");
    }
    
    fclose(fp);
}

void filter_by_salary() {
    float min_salary, max_salary;
    printf("Enter minimum salary: ");
    scanf("%f", &min_salary);
    printf("Enter maximum salary: ");
    scanf("%f", &max_salary);
    getchar();
    
    printf("\n=== JOBS IN SALARY RANGE ===\n");
    FILE *fp = fopen("job_data.txt", "r");
    if (fp == NULL) {
        printf("No jobs available!\n");
        return;
    }
    
    Job job;
    char line[500];
    bool found = false;
    
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d|%[^|]|%[^|]|%[^|]|%f|%[^|]|%[^|]|%d", 
               &job.id, job.title, job.company, job.location, 
               &job.salary, job.requirements, job.description, &job.vacancies);
        
        if (job.salary >= min_salary && job.salary <= max_salary && job.vacancies > 0) {
            printf("ID: %d | Title: %s | Company: %s | Salary: $%.2f\n", 
                   job.id, job.title, job.company, job.salary);
            found = true;
        }
    }
    
    if (!found) {
        printf("No jobs found in this salary range!\n");
    }
    
    fclose(fp);
}

void apply_job() {
    int job_id;
    printf("Enter Job ID to apply: ");
    scanf("%d", &job_id);
    getchar();
    
    // Check if job exists and has vacancies
    FILE *fp = fopen("job_data.txt", "r");
    if (fp == NULL) {
        printf("No jobs available!\n");
        return;
    }
    
    Job job;
    char line[500];
    bool job_found = false;
    
    while (fgets(line, sizeof(line), fp)) {
        sscanf(line, "%d|%[^|]|%[^|]|%[^|]|%f|%[^|]|%[^|]|%d", 
               &job.id, job.title, job.company, job.location, 
               &job.salary, job.requirements, job.description, &job.vacancies);
        
        if (job.id == job_id && job.vacancies > 0) {
            job_found = true;
            break;
        }
    }
    fclose(fp);
    
    if (!job_found) {
        printf("Job not found or no vacancies available!\n");
        return;
    }
    
    Application app;
    static int app_id = 1;
    app.id = app_id++;
    app.job_id = job_id;
    strcpy(app.status, "Pending");
    
    printf("Enter your name: ");
    fgets(app.name, 100, stdin);
    app.name[strcspn(app.name, "\n")] = 0;
    
    printf("Enter your email: ");
    fgets(app.email, 100, stdin);
    app.email[strcspn(app.email, "\n")] = 0;
    
    printf("Enter your phone: ");
    fgets(app.phone, 20, stdin);
    app.phone[strcspn(app.phone, "\n")] = 0;
    
    // Save application
    FILE *app_file = fopen("applications.txt", "a");
    if (app_file != NULL) {
        fprintf(app_file, "%d|%s|%s|%s|%d|%s\n", 
                app.id, app.name, app.email, app.phone, app.job_id, app.status);
        fclose(app_file);
        printf("Application submitted successfully!\n");
    } else {
        printf("Error saving application!\n");
    }
}