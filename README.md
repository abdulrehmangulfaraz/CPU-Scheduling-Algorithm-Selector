# CPU Scheduling Algorithm Selector (v3.0)

## Project Overview
The **CPU Scheduling Algorithm Selector** is a C-based simulation and analysis tool developed to evaluate, compare, and recommend CPU scheduling algorithms based on process behavior and workload characteristics.

The system simulates multiple classical and advanced scheduling algorithms, computes key performance metrics such as **Average Waiting Time** and **Average Turnaround Time**, and automatically identifies the most efficient algorithm for a given scenario.

Users can either run **predefined real-world scenarios** or provide **custom process input** to analyze scheduling behavior under different operating system conditions.

---

## Key Features
- Supports **multiple CPU scheduling algorithms**
- Automatic **performance comparison and recommendation**
- Detailed **execution logs (Gantt-chart style)** for individual algorithms
- **10 built-in test scenarios** covering common OS scheduling problems
- Menu-driven, user-friendly console interface
- Modular and well-structured C implementation

---

## Supported Scheduling Algorithms
- **First Come First Served (FCFS)** — Non-preemptive
- **Shortest Job First (SJF)** — Non-preemptive
- **Priority Scheduling** — Non-preemptive
- **Round Robin (RR)** — Configurable Time Quantum
- **Multi-Level Queue (MLQ)** — Fixed priority queues
- **Multi-Level Feedback Queue (MLFQ)** — Dynamic priority adjustment

---

## Project Structure
```

├── main.c          // Program entry point and menu handling
├── scheduler.h     // Process structure and function declarations
├── fcfs.c          // FCFS scheduling implementation
├── sjf.c           // SJF scheduling implementation
├── priority.c      // Priority scheduling implementation
├── rr.c            // Round Robin scheduling implementation
├── mlq.c           // Multi-Level Queue scheduling logic
├── mlfq.c          // Multi-Level Feedback Queue scheduling logic
├── Makefile        // Build automation script

````

---

## Getting Started

### Prerequisites
- **GCC Compiler** (GNU Compiler Collection)
- **Make** (recommended for easier compilation)

---

### Compilation
Open a terminal in the project directory and run:

```bash
make
````

This will generate an executable named `scheduler` (or `scheduler.exe` on Windows).

To remove compiled object files:

```bash
make clean
```

---

### Running the Program

Execute the simulator using:

```bash
./scheduler
```

---

## Program Usage

Upon execution, the following menu is displayed:

```
=============================================
   CPU SCHEDULING ALGORITHM SELECTOR (v3.0)
=============================================
1. Run a Test Scenario (1-10)
2. Custom Input
3. Exit
```

---

## Test Scenarios (Option 1)

The simulator includes **10 predefined scenarios**, each designed to represent a specific scheduling behavior or edge case:

1. **Standard Mix**
   Balanced set of processes with varying arrival and burst times.

2. **Convoy Effect**
   A long process arrives first, causing shorter processes to wait (FCFS weakness).

3. **Reverse Convoy**
   Short processes arrive before a long one.

4. **Batch Arrival**
   All processes arrive at time 0.

5. **Priority Dominance**
   High-priority processes continuously block low-priority ones.

6. **Equal Load**
   Identical processes arriving sequentially.

7. **MLFQ Stress Test**
   Processes with varied burst times to test queue demotion behavior.

8. **Starvation Risk**
   Frequent arrival of high-priority processes.

9. **MLQ System vs User**
   Separate queues for System and User processes.

10. **Complex Random Mix**
    Mixed arrival times, burst durations, and priorities.

---

## Custom Input Mode (Option 2)

Users can manually define their own process set. For each process, the following parameters are required:

* **Arrival Time**
* **Burst Time**
* **Priority** (Lower value indicates higher priority)
* **Queue ID**

  * `1` → System Queue (MLQ)
  * `2` → User Queue (MLQ)

---

## Algorithm Configuration Details

| Algorithm       | Implementation Details                                                |
| --------------- | --------------------------------------------------------------------- |
| **FCFS**        | Sorted by Arrival Time, non-preemptive                                |
| **SJF**         | Shortest Burst Time first, tie-breaker: Arrival Time                  |
| **Priority**    | Lowest priority value selected first                                  |
| **Round Robin** | Time Quantum automatically tested with **2** and **4**                |
| **MLQ**         | Queue 1 (System): RR (Q=4), Queue 2 (User): FCFS, strict priority     |
| **MLFQ**        | Q1: RR (Q=4) → Q2: RR (Q=8) → Q3: FCFS, demotion after quantum expiry |

---

## Output and Analysis

* Calculates **Average Waiting Time**
* Calculates **Average Turnaround Time**
* Compares all algorithms under identical workloads
* Automatically **recommends the best-performing algorithm**
* Allows inspection of **individual algorithm execution logs**

---

## License

This project is open-source and intended strictly for **educational and academic use**.