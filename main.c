#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define NUM_CHAIRS 3

pthread_mutex_t mutex;
sem_t studentsSem;
sem_t taSem;

int numStudents;
int servedStudents = 0;
int waitingStudents = 0;

void *ta_thread(void *arg) {
    while (servedStudents < numStudents) {
        sem_wait(&studentsSem); // Wait for a student to arrive

        pthread_mutex_lock(&mutex); //mutex lock so that the Critical section is protected

        waitingStudents--; //decrement waiting students since TA is now helping
        printf("TA is helping a student. Students served: %d\n", servedStudents);

        sem_post(&taSem); // Notify the student that the TA is available

        pthread_mutex_unlock(&mutex); //unlock CS

        // Simulate helping the student
        printf("TA is helping a student...\n");
        sleep(rand() % 5 + 1); // Sleep for a random time

        printf("TA has finished helping a student.\n");

        servedStudents++;
    }

    printf("All students have been served. TA is going home.\n");

    return NULL;
}

void *student_thread(void *arg) {
    int student_id = *(int *)arg;

    // Simulate programming
    printf("Student %d is programming...\n", student_id);
    sleep(rand() % 5 + 1); // Sleep for a random time

    pthread_mutex_lock(&mutex); //lock CS, waitingStudents is a variable we want to be careful with

    if (servedStudents < numStudents) { //not all students have been served yet
        if (waitingStudents < NUM_CHAIRS) {
            waitingStudents++; //increment waiting chairs since now a new student is waiting
            printf("Student %d is waiting in the hallway. Waiting students: %d\n",
                   student_id, waitingStudents);

            pthread_mutex_unlock(&mutex); //unlock CS

            sem_post(&studentsSem); // Notify the TA that a student is waiting (signal in semaphores)

            sem_wait(&taSem); // Wait for the TA to be available (wait on TA sem, that will indicate whether TA is currently helping)

            printf("Student %d is getting help from the TA...\n", student_id);
            sleep(rand() % 5 + 1); // Sleep for a random time

            printf("Student %d has finished getting help from the TA.\n", student_id);
        } else {
            pthread_mutex_unlock(&mutex);
            printf("Student %d will come back later. No chairs available.\n",
                   student_id);
            sleep(rand() % 5 + 1); // Sleep for a random time before trying again
        }
    } else {
        pthread_mutex_unlock(&mutex);
        printf("Student %d has completed all assignments and is leaving.\n",
               student_id);
    }

    return NULL;
}

int main() {
    printf("Enter the number of students: ");
    scanf("%d", &numStudents);

    pthread_t ta;
    pthread_t students[numStudents];
    int student_ids[numStudents];

    // Initialize mutex and semaphores
    pthread_mutex_init(&mutex, NULL);
    sem_init(&studentsSem, 0, 0);
    sem_init(&taSem, 0, 0);

    // Create TA thread
    pthread_create(&ta, NULL, ta_thread, NULL);

    // Create student threads
    for (int i = 0; i < numStudents; i++) {
        student_ids[i] = i + 1;
        pthread_create(&students[i], NULL, student_thread, &student_ids[i]);
    }

    // Join student threads
    for (int i = 0; i < numStudents; i++) {
        pthread_join(students[i], NULL);
    }

    // Cancel TA thread
    pthread_cancel(ta);

    // Join TA thread
    pthread_join(ta, NULL);

    // Clean up mutex and semaphores
    pthread_mutex_destroy(&mutex);
    sem_destroy(&studentsSem);
    sem_destroy(&taSem);

    return 0;
}
