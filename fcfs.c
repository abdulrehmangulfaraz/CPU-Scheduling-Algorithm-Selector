#include "scheduler.h"

// Helper to sort by Arrival Time
void sort_by_arrival(Process p[], int n) {
    Process temp;
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (p[j].arrival_time > p[j+1].arrival_time) {
                temp = p[j];
                p[j] = p[j+1];
                p[j+1] = temp;
            }
        }
    }
}

void calculate_fcfs(Process p[], int n) {
    printf("\n--- Executing FCFS Algorithm ---\n");
    
    // 1. Sort processes by arrival time
    sort_by_arrival(p, n);

    int current_time = 0;
    float total_waiting_time = 0;
    float total_tat = 0;

    printf("PID\tArrival\tBurst\tWaiting\tTurnaround\n");

    for (int i = 0; i < n; i++) {
        // CPU remains idle if the next process hasn't arrived yet
        if (current_time < p[i].arrival_time) {
            current_time = p[i].arrival_time;
        }

        p[i].start_time = current_time;
        p[i].completion_time = current_time + p[i].burst_time;
        p[i].turnaround_time = p[i].completion_time - p[i].arrival_time;
        p[i].waiting_time = p[i].turnaround_time - p[i].burst_time;

        // Add to totals for averages
        total_waiting_time += p[i].waiting_time;
        total_tat += p[i].turnaround_time;

        // Advance time
        current_time = p[i].completion_time;

        // Print individual process result
        printf("%d\t%d\t%d\t%d\t%d\n", 
               p[i].pid, p[i].arrival_time, p[i].burst_time, 
               p[i].waiting_time, p[i].turnaround_time);
    }

    printf("\nAverage Waiting Time: %.2f", total_waiting_time / n);
    printf("\nAverage Turnaround Time: %.2f\n", total_tat / n);
}