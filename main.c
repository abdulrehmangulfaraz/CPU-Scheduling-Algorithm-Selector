#include "scheduler.h"

// Implementation of reset_metrics
// This function must exist so the linker can find it!
void reset_metrics(Process p[], int n) {
    for (int i = 0; i < n; i++) {
        p[i].waiting_time = 0;
        p[i].turnaround_time = 0;
        p[i].completion_time = 0;
        p[i].start_time = 0;
        p[i].remaining_time = p[i].burst_time; // Reset remaining time to full burst
    }
}

int main() {
    printf("--- CPU Scheduling Algorithm Selector ---\n");

    // Test Data: 3 Processes
    // P1: Arrives at 0, Burst 10
    // P2: Arrives at 1, Burst 5
    // P3: Arrives at 2, Burst 8
    Process test_procs[3] = {
        {1, 0, 10, 1, 10, 0, 0, 0, 0},
        {2, 1, 5, 2, 5, 0, 0, 0, 0},
        {3, 2, 8, 3, 8, 0, 0, 0, 0}
    };
    int n = 3;

    // Run FCFS
    reset_metrics(test_procs, n); 
    calculate_fcfs(test_procs, n);
    calculate_sjf(test_procs, n);
    int quantums[] = {2, 4, 8};
    for (int i = 0; i < 3; i++) {
        reset_metrics(test_procs, n);
        calculate_rr(test_procs, n, quantums[i]);
    }

    return 0;
}