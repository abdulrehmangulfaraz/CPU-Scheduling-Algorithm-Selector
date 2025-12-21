#include "scheduler.h"
#include <limits.h> // For INT_MAX

void calculate_sjf(Process p[], int n) {
    printf("\n--- Executing SJF (Non-Preemptive) ---\n");

    int current_time = 0;
    int completed = 0;
    int is_completed[100] = {0}; // Track which processes are done (assuming max 100 processes)
    
    float total_waiting_time = 0;
    float total_tat = 0;

    printf("PID\tArrival\tBurst\tWaiting\tTurnaround\n");

    while (completed != n) {
        int idx = -1;
        int min_burst = INT_MAX;

        // Find the process with minimum burst time among those that have arrived
        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && is_completed[i] == 0) {
                if (p[i].burst_time < min_burst) {
                    min_burst = p[i].burst_time;
                    idx = i;
                }
                // Tie-breaking: If burst times are same, pick the one that arrived first (FCFS rule)
                if (p[i].burst_time == min_burst) {
                    if (p[i].arrival_time < p[idx].arrival_time) {
                        idx = i;
                    }
                }
            }
        }

        if (idx != -1) {
            // Process found
            p[idx].start_time = current_time;
            p[idx].completion_time = p[idx].start_time + p[idx].burst_time;
            p[idx].turnaround_time = p[idx].completion_time - p[idx].arrival_time;
            p[idx].waiting_time = p[idx].turnaround_time - p[idx].burst_time;

            // Update totals
            total_waiting_time += p[idx].waiting_time;
            total_tat += p[idx].turnaround_time;

            // Mark as completed
            is_completed[idx] = 1;
            completed++;
            current_time = p[idx].completion_time;

            printf("%d\t%d\t%d\t%d\t%d\n", 
                   p[idx].pid, p[idx].arrival_time, p[idx].burst_time, 
                   p[idx].waiting_time, p[idx].turnaround_time);
        } else {
            // No process arrived yet; CPU idle
            current_time++;
        }
    }

    printf("\nAverage Waiting Time: %.2f", total_waiting_time / n);
    printf("\nAverage Turnaround Time: %.2f\n", total_tat / n);
}