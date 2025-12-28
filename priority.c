#include "scheduler.h"
#include <limits.h>

void calculate_priority(Process p[], int n) {
    printf("\n--- Executing Priority Scheduling ---\n");
    printf("Note: Lower value = Higher Priority\n");

    int current_time = 0;
    int completed = 0;
    int is_completed[100] = {0};
    
    float total_waiting_time = 0;
    float total_tat = 0;

    printf("PID\tArrival\tBurst\tPriority\tWaiting\tTurnaround\n");

    while (completed != n) {
        int idx = -1;
        int min_priority = INT_MAX;

        // Find the process with HIGHEST PRIORITY (lowest value) among those that have arrived
        for (int i = 0; i < n; i++) {
            if (p[i].arrival_time <= current_time && is_completed[i] == 0) {
                if (p[i].priority < min_priority) {
                    min_priority = p[i].priority;
                    idx = i;
                }
                // Tie-breaking: FCFS (Arrival Time)
                if (p[i].priority == min_priority) {
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

            printf("%d\t%d\t%d\t%d\t\t%d\t%d\n", 
                   p[idx].pid, p[idx].arrival_time, p[idx].burst_time, p[idx].priority,
                   p[idx].waiting_time, p[idx].turnaround_time);
        } else {
            // CPU Idle
            current_time++;
        }
    }

    printf("\nAverage Waiting Time: %.2f", total_waiting_time / n);
    printf("\nAverage Turnaround Time: %.2f\n", total_tat / n);
}