#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define MAX_TASKS 100
#define MAX_DATE 20
#define MAX_TIME 10

struct Task {
    char description[100];
    int completed;
    int priority;  // Priority of the task
    char dueDateTime[MAX_DATE + MAX_TIME];  // Combine date and time into one field
};

struct Task tasks[MAX_TASKS];
int taskCount = 0;

void addTask(char description[], char dueDateTime[], int priority) {
    if (taskCount < MAX_TASKS) {
        strcpy(tasks[taskCount].description, description);
        strcpy(tasks[taskCount].dueDateTime, dueDateTime);
        tasks[taskCount].completed = 0;
        tasks[taskCount].priority = priority;
        taskCount++;
        printf("Task added successfully.\n");
    } else {
        printf("Task limit reached. Cannot add more tasks.\n");
    }
}

void markComplete(int taskIndex) {
    if (taskIndex >= 0 && taskIndex < taskCount) {
        tasks[taskIndex].completed = 1;
        printf("Task marked as complete. Reminder: %s\n", tasks[taskIndex].dueDateTime);
    } else {
        printf("Invalid task index.\n");
    }
}

void displayTasks() {
    if (taskCount == 0) {
        printf("No tasks available.\n");
    } else {
        printf("Tasks:\n");
        for (int i = 0; i < taskCount; i++) {
            printf("%d. %s - Due: %s - Priority: %d - %s\n", i + 1, tasks[i].description, tasks[i].dueDateTime, tasks[i].priority, tasks[i].completed ? "Completed" : "Incomplete");
        }
    }
}

void displayCalendar() {
    printf("\nCalendar:\n");

    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].completed == 0) {
            printf("Task: %s - Due: %s - Priority: %d\n", tasks[i].description, tasks[i].dueDateTime, tasks[i].priority);
        }
    }
}

void displayProgress() {
    int completedCount = 0;

    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].completed == 1) {
            completedCount++;
        }
    }

    if (taskCount > 0) {
        float completionPercentage = (float)completedCount / taskCount * 100;
        printf("\nProgress: %.2f%% of tasks completed.\n", completionPercentage);
    } else {
        printf("\nProgress: No tasks available.\n");
    }
}

void displayPriorityHistogram() {
    printf("\nPriority Histogram:\n");

    int priorityCounts[5] = {0};  // Array to store the count of tasks for each priority

    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].completed == 0) {
            priorityCounts[tasks[i].priority - 1]++;
        }
    }

    for (int i = 0; i < 5; i++) {
        printf("%d: ", i + 1);
        for (int j = 0; j < priorityCounts[i]; j++) {
            printf("#");
        }
        printf("\n");
    }
}

void checkOverdueTasks() {
    time_t t = time(NULL);
    struct tm *now = localtime(&t);

    for (int i = 0; i < taskCount; i++) {
        if (tasks[i].completed == 0) {
            struct tm dueDateTime;
            sscanf(tasks[i].dueDateTime, "%d-%d-%d %d:%d", &dueDateTime.tm_year, &dueDateTime.tm_mon, &dueDateTime.tm_mday, &dueDateTime.tm_hour, &dueDateTime.tm_min);
            dueDateTime.tm_year -= 1900;
            dueDateTime.tm_mon -= 1;

            if (mktime(&dueDateTime) < mktime(now)) {
                printf("Reminder: Task '%s' is overdue!\n", tasks[i].description);
            }
        }
    }
}

int main() {
    checkOverdueTasks();

    int choice, taskIndex, priority;
    char description[100], dueDateTime[MAX_DATE + MAX_TIME];

    do {
        printf("\n1. Add Task\n2. Mark Task as Complete\n3. Display Tasks\n4. Display Calendar\n5. Display Progress\n6. Display Priority Histogram\n7. Exit\n");
        printf("Enter your choice: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter task description: ");
                scanf(" %99[^\n]", description);
                printf("Enter due date and time (YYYY-MM-DD HH:MM): ");
                scanf(" %19[^\n]", dueDateTime);
                printf("Enter task priority (1-5, where 5 is the highest): ");
                scanf("%d", &priority);

                // Validate priority
                if (priority < 1 || priority > 5) {
                    printf("Invalid priority. Setting priority to 1 (lowest).\n");
                    priority = 1;
                }

                addTask(description, dueDateTime, priority);
                break;
            case 2:
                printf("Enter task index to mark as complete: ");
                if (scanf("%d", &taskIndex) != 1 || taskIndex < 1 || taskIndex > taskCount) {
                    printf("Invalid input. Please enter a valid task index.\n");
                    while (getchar() != '\n');
                } else {
                    markComplete(taskIndex - 1);
                }
                break;
            case 3:
                displayTasks();
                break;
            case 4:
                displayCalendar();
                break;
            case 5:
                displayProgress();
                break;
            case 6:
                displayPriorityHistogram();
                break;
            case 7:
                printf("Exiting the program.\n");
                break;
            default:
                printf("Invalid choice. Please enter a valid option.\n");
        }

    } while (choice != 7);

    return 0;
}
