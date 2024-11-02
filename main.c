#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <semaphore.h>
#include <fcntl.h>

#define BUFFER_SIZE 4

int *buffer;                // shared buffer
int *count;                 // shared count of items in buffer

// Semaphores for synchronization
sem_t *empty_slots;         // semaphore for empty slots in buffer
sem_t *filled_slots;        // semaphore for filled slots in buffer
sem_t *mutex;               // mutex for exclusive access to buffer

// Producer function
void producer() {
    int item, i = 0;
    while (i < 10) {  // Producing 10 items
        item = rand() % 100;  // produce an item

        sem_wait(empty_slots);         // wait for an empty slot
        sem_wait(mutex);               // lock buffer for exclusive access

        // Add item to buffer
        buffer[(*count)++] = item;
        printf("Producer produced: %d | Buffer: ", item);
        for (int j = 0; j < *count; j++) printf("%d ", buffer[j]);
        printf("\n");

        sem_post(mutex);               // unlock buffer
        sem_post(filled_slots);        // signal that there is a new filled slot

        i++;
        sleep(1);
    }
}

// Consumer function
void consumer() {
    int item, i = 0;
    while (i < 10) {  // Consuming 10 items
        sem_wait(filled_slots);        // wait for a filled slot
        sem_wait(mutex);               // lock buffer for exclusive access

        // Remove item from buffer
        item = buffer[--(*count)];
        printf("Consumer consumed: %d | Buffer: ", item);
        for (int j = 0; j < *count; j++) printf("%d ", buffer[j]);
        printf("\n");

        sem_post(mutex);               // unlock buffer
        sem_post(empty_slots);         // signal that there is a new empty slot

        i++;
        sleep(2);
    }
}

int main() {
    pid_t pid;

    // Allocate shared memory for buffer and count
    buffer = mmap(NULL, BUFFER_SIZE * sizeof(int), PROT_READ | PROT_WRITE,
                  MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    count = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE,
                 MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    *count = 0;  // initialize count

    // Initialize semaphores
    empty_slots = sem_open("/empty_slots", O_CREAT | O_EXCL, 0666, BUFFER_SIZE);
    filled_slots = sem_open("/filled_slots", O_CREAT | O_EXCL, 0666, 0);
    mutex = sem_open("/mutex", O_CREAT | O_EXCL, 0666, 1);

    // Fork to create producer and consumer processes
    pid = fork();

    if (pid == 0) {
        // Child process (producer)
        producer();
        exit(0);
    } else if (pid > 0) {
        // Parent process (consumer)
        consumer();
        
        // Wait for producer to finish
        wait(NULL);

        // Clean up semaphores and shared memory
        sem_unlink("/empty_slots");
        sem_unlink("/filled_slots");
        sem_unlink("/mutex");
        munmap(buffer, BUFFER_SIZE * sizeof(int));
        munmap(count, sizeof(int));
    } else {
        perror("Fork failed");
        exit(1);
    }

    return 0;
}
