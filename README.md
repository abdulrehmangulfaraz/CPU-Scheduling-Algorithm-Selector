# CPU Scheduling Algorithm Selector

## Project Overview
This tool simulates various CPU scheduling algorithms (FCFS, SJF, RR, Priority, MLQ, MLFQ) to determine the optimal strategy for a given set of processes.

## Analysis & Decision Criteria
The "Selector" determines the best algorithm based on **Average Waiting Time**, as this is the primary metric for system efficiency.

### Trade-off Observations
1.  **FCFS:** Performs well for uniform batch jobs but suffers from the "Convoy Effect" if a long CPU-bound process arrives first.
2.  **SJF (Shortest Job First):** Consistently provides the mathematically minimum average waiting time. However, it requires knowing the burst time in advance, which is not always possible in real systems.
3.  **Round Robin:** The best choice for **Fairness** and **Response Time**. It prevents starvation but often has a higher average Turnaround Time compared to SJF.
4.  **MLFQ:** The most robust for general-purpose operating systems. It dynamically penalizes CPU-heavy processes and prioritizes I/O-bound (interactive) processes without needing to know burst times beforehand.

## Complexity Assessment
This project addresses the **Complex Computing Problem (CCP)** attributes:
* **Depth of Analysis:** Solved the trade-off between fairness (RR) and efficiency (SJF) by implementing MLFQ.
* **Modular Design:** Separated logic into `algorithms/` modules and a core `scheduler.h` interface.