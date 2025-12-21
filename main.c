#include "scheduler.h"

// Implementation of reset_metrics
void reset_metrics(Process p[], int n) {
    for (int i = 0; i < n; i++) {
        p[i].waiting_time = 0;
        p[i].turnaround_time = 0;
        p[i].completion_time = 0;
        p[i].start_time = 0;
        p[i].remaining_time = p[i].burst_time;
    }
}

int main() {
    printf("--- CPU Scheduling Algorithm Selector ---\n");

    // Test Data: 3 Processes
    // P1: Arr 0, Burst 10, Prio 2 (Medium)
    // P2: Arr 1, Burst 5,  Prio 3 (Low Priority - Higher Number)
    // P3: Arr 2, Burst 8,  Prio 1 (High Priority - Lower Number)
    Process test_procs[3] = {
        {1, 0, 10, 2, 10, 0, 0, 0, 0}, 
        {2, 1, 5,  3, 5,  0, 0, 0, 0},   
        {3, 2, 8,  1, 8,  0, 0, 0, 0}    
    };
    int n = 3;

    // Run FCFS
    reset_metrics(test_procs, n);
    calculate_fcfs(test_procs, n);

    // Run SJF
    reset_metrics(test_procs, n);
    calculate_sjf(test_procs, n);

    // Run Priority
    reset_metrics(test_procs, n);
    calculate_priority(test_procs, n);

    // Run Round Robin
    int quantums[] = {2, 4}; // Reduced to 2 examples for brevity
    for (int i = 0; i < 2; i++) {
        reset_metrics(test_procs, n);
        calculate_rr(test_procs, n, quantums[i]);
    }

    return 0;
}