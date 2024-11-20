# Producer-Consumer Problem Simulation with One Producer and One Consumer

This project simulates the classic producer-consumer problem with a single producer and a single consumer that share a fixed-size buffer. Synchronization between the producer and consumer is managed with semaphores to prevent race conditions and ensure that they access the buffer mutually exclusively.

### Key Components
- **Buffer**: A shared buffer of fixed size (`BUFFER_SIZE`) where the producer stores items and the consumer retrieves them.
- **Semaphores**:
  - `empty_slots`: Tracks available empty slots in the buffer.
  - `filled_slots`: Tracks occupied slots in the buffer.
  - `mutex`: Ensures only one process accesses the buffer at a time.
- **One Producer & One Consumer**:
  - The producer creates items and adds them to the buffer.
  - The consumer removes items from the buffer.

## Code Structure

### Constants
- `BUFFER_SIZE`: Sets the capacity of the buffer. In this version, the buffer size is set to 4.

### Global Variables
- `buffer`: The shared buffer array where items are stored.
- `count`: Tracks the current number of items in the buffer.

### Semaphores
- **`empty_slots`**: Initialized to `BUFFER_SIZE`, indicating the number of empty slots. The producer must check this semaphore to ensure there is space before adding an item.
- **`filled_slots`**: Initialized to 0, representing filled slots. The consumer must check this semaphore to ensure there are items available before consuming.
- **`mutex`**: Controls exclusive access to the buffer, ensuring only one process accesses it at a time.

### Functions

#### `producer()`
Simulates the single producer process.
- **Workflow**:
  1. Generates an item (random number).
  2. Checks `empty_slots` (blocks if no slots are available).
     - **Prints** when checking for empty slots and locking the buffer.
  3. Locks `mutex` to gain exclusive access to the buffer.
  4. Adds the item to the buffer and increments `count`.
  5. Releases `mutex` and signals `filled_slots`.
  6. Sleeps to simulate the time taken for production.

#### `consumer()`
Simulates the single consumer process.
- **Workflow**:
  1. Checks `filled_slots` (blocks if no items are available).
     - **Prints** when checking for filled slots and locking the buffer.
  2. Locks `mutex` for exclusive buffer access.
  3. Removes an item from the buffer and decrements `count`.
  4. Releases `mutex` and signals `empty_slots`.
  5. Sleeps to simulate the time taken for consumption.

### Main Program (`main`)
1. Allocates shared memory and initializes semaphores.
2. Creates the producer and consumer processes.
3. Waits for both processes to finish.
4. Cleans up semaphores and shared memory.

### Example Output

The program includes print statements to indicate each step of the producer and consumer processes:
- **Producer Messages**:
  - `"Producer: Checking for empty slot..."` — Indicates producer is about to check for space and may block if no slots are available.
  - `"Producer: Trying to lock buffer..."` — Indicates producer tries to lock the buffer and may block if the consumer has locked it.
- **Consumer Messages**:
  - `"Consumer: Checking for filled slot..."` — Indicates consumer is about to check for an item and may block if no items are available.
  - `"Consumer: Trying to lock buffer..."` — Indicates consumer tries to lock the buffer and may block if the producer has locked it.

These messages help track when the producer or consumer is blocked, waiting, or accessing the buffer.

## Compilation and Execution

To compile and run the program, use the following commands:

```bash
gcc producer_consumer_one.c -o producer_consumer_one -pthread
./producer_consumer_one
