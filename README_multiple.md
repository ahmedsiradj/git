# Producer-Consumer Problem Simulation with Multiple Producers and Consumers

This project simulates the classic producer-consumer problem, implementing multiple producer and consumer processes that share a buffer of fixed size. The synchronization between the processes is achieved using semaphores to prevent race conditions and ensure that resources are used in a mutually exclusive manner.

### Key Components
- **Buffer**: Shared between producers and consumers, with a fixed size (`BUFFER_SIZE`), representing the total capacity.
- **Semaphores**:
  - `empty_slots`: Tracks the available empty slots in the buffer.
  - `filled_slots`: Tracks the occupied slots in the buffer.
  - `mutex`: Ensures that only one process accesses the buffer at a time.
- **Multiple Producers & Consumers**: 
  - Two producers and two consumers simulate concurrent data production and consumption.
  - Each producer creates items, and each consumer removes items from the shared buffer.

## Code Structure

### Constants
- `BUFFER_SIZE`: Defines the fixed capacity of the buffer.
- `NUM_PRODUCERS` and `NUM_CONSUMERS`: Define the number of producer and consumer processes created.

### Global Variables
- `buffer`: An array shared between processes to hold the produced items.
- `count`: Tracks the number of items currently in the buffer.

### Semaphores
- **`empty_slots`**: Initialized to `BUFFER_SIZE`, indicating the number of empty slots. Each producer must check this semaphore to ensure space is available in the buffer.
- **`filled_slots`**: Initialized to 0, tracking filled slots. Each consumer checks this semaphore before attempting to remove an item from the buffer.
- **`mutex`**: Ensures exclusive access to the buffer for either a producer or a consumer at a time.

### Functions

#### `producer(int id)`
Simulates a producer process, identified by an `id`.
- **Workflow**:
  1. Generates an item.
  2. Checks `empty_slots` (blocks if no slots are empty).
     - **Prints** when checking for empty slots and locking the buffer.
  3. Locks `mutex` to gain exclusive buffer access.
  4. Adds the item to the buffer, then increments `count`.
  5. Releases `mutex` and signals `filled_slots`.
  6. Sleeps to simulate time taken for production.

#### `consumer(int id)`
Simulates a consumer process, identified by an `id`.
- **Workflow**:
  1. Checks `filled_slots` (blocks if no slots are filled).
     - **Prints** when checking for filled slots and locking the buffer.
  2. Locks `mutex` for exclusive buffer access.
  3. Removes an item from the buffer, then decrements `count`.
  4. Releases `mutex` and signals `empty_slots`.
  5. Sleeps to simulate time taken for consumption.

### Main Program (`main`)
1. Initializes shared memory and semaphores.
2. Spawns `NUM_PRODUCERS` producer processes.
3. Spawns `NUM_CONSUMERS` consumer processes.
4. Waits for all processes to finish.
5. Cleans up semaphores and shared memory.

### Example Output

The code includes print statements that show each producer and consumer's actions in real-time. Key messages:
- **Producer Messages**:
  - `"Producer <id>: Checking for empty slot..."` — Indicates producer checks for space.
  - `"Producer <id>: Trying to lock buffer..."` — Indicates producer tries to access buffer.
- **Consumer Messages**:
  - `"Consumer <id>: Checking for filled slot..."` — Indicates consumer checks for items.
  - `"Consumer <id>: Trying to lock buffer..."` — Indicates consumer tries to access buffer.

These messages help to track when each producer or consumer is blocked, waiting, or actively working with the buffer.

## Compilation and Execution

To compile and run the program, use the following commands:

```bash
gcc producer_consumer.c -o producer_consumer -pthread
./producer_consumer
