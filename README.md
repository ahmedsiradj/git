# Producer-Consumer Program with Processes

This program demonstrates the producer-consumer problem using **processes** instead of threads. It simulates one producer and one consumer using shared memory and named semaphores for synchronization. The buffer has a size of 4.

## Program Workflow

### 1. Setup
   - **Shared Memory**:
     - A shared buffer array of size 4 is created, along with a counter `count` that tracks the number of items in the buffer.
   - **Semaphores**:
     - `empty_slots`: Tracks the available slots in the buffer (starts at 4 since the buffer is empty initially).
     - `filled_slots`: Tracks the number of items in the buffer (starts at 0 since no items are in the buffer initially).
     - `mutex`: Ensures exclusive access to the buffer for either the producer or consumer at any given time.

### 2. Creating Processes
   - The program uses `fork()` to create two separate processes:
     - **Producer Process**: Handles producing items.
     - **Consumer Process**: Handles consuming items.

### 3. Producer Process Workflow
   - The producer attempts to produce 10 items in a loop.
   - For each item:
     - **Waits for an Empty Slot**: Ensures there’s space in the buffer using `sem_wait(empty_slots)`. If the buffer is full, the producer waits.
     - **Locks the Buffer**: Acquires a mutex lock with `sem_wait(mutex)` to gain exclusive access.
     - **Adds Item to Buffer**: The item is added to the buffer, and `count` is incremented.
     - **Prints Buffer Status**: Displays the produced item and current buffer contents.
     - **Releases the Lock**: Unlocks the buffer using `sem_post(mutex)` so the consumer can access it.
     - **Signals a Filled Slot**: Uses `sem_post(filled_slots)` to signal the consumer that a new item is available.
     - **Waits for a Second**: Pauses for 1 second to simulate production time.

### 4. Consumer Process Workflow
   - The consumer attempts to consume 10 items in a loop.
   - For each item:
     - **Waits for a Filled Slot**: Ensures there’s an item in the buffer using `sem_wait(filled_slots)`. If the buffer is empty, the consumer waits.
     - **Locks the Buffer**: Acquires a mutex lock with `sem_wait(mutex)` to gain exclusive access.
     - **Removes Item from Buffer**: The item is removed from the buffer, and `count` is decremented.
     - **Prints Buffer Status**: Displays the consumed item and current buffer contents.
     - **Releases the Lock**: Unlocks the buffer using `sem_post(mutex)` so the producer can access it.
     - **Signals an Empty Slot**: Uses `sem_post(empty_slots)` to signal the producer that a slot is now available.
     - **Waits for Two Seconds**: Pauses for 2 seconds to simulate consumption time.

### 5. Ending and Cleanup
   - After producing and consuming 10 items each, both processes finish.
   - The parent process (`main`) waits for the child process (producer) to finish.
   - All shared resources (shared memory and semaphores) are released and cleaned up.

## Compilation and Execution

To compile and run the program, use the following commands:

```bash
gcc -o producer_consumer producer_consumer.c -pthread
./producer_consumer
