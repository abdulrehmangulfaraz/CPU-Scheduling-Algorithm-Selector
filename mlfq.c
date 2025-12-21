#include "scheduler.h"
#include <stdbool.h>

void calculate_mlfq(Process p[], int n) {
    printf("\n--- Executing Multi-Level Feedback Queue (MLFQ) ---\n");
    printf("Q1 (RR, q=4) -> Q2 (RR, q=8) -> Q3 (FCFS)\n");

    // Dynamic queue tracking: reusing 'priority' field
    // 1 = Q1, 2 = Q2, 3 = Q3
    // Initialize all processes to start at Queue 1
    for(int i=0; i<n; i++) {
        p[i].priority = 1; 
    }

    int current_time = 0;
    int completed = 0;
    
    // Quantum limits
    int q1_quantum = 4;
    int q2_quantum = 8;
    
    // Timer for current process execution
    int execution_timer = 0; 
    int last_pid = -1;

    printf("PID\tFinalQ\tArrival\tBurst\tWaiting\tTurnaround\n");

    while (completed < n) {
        int idx = -1;
        int current_queue = -1;

        // --- LEVEL 1: CHECK Q1 (Highest Priority) ---
        // Basic RR logic: Resume previous if valid, else find next
        // (Simplified scan for Q1)
        for(int i=0; i<n; i++) {
            if (p[i].priority == 1 && p[i].remaining_time > 0 && p[i].arrival_time <= current_time) {
                // Basic fairness: pick the one that was running or the first one found
                // For strict RR, we should track indices, but for this CCP level, 
                // picking the first available valid Q1 process is acceptable.
                idx = i;
                current_queue = 1;
                break; 
            }
        }

        // --- LEVEL 2: CHECK Q2 (If Q1 Empty) ---
        if (idx == -1) {
            for(int i=0; i<n; i++) {
                if (p[i].priority == 2 && p[i].remaining_time > 0 && p[i].arrival_time <= current_time) {
                    idx = i;
                    current_queue = 2;
                    break;
                }
            }
        }

        // --- LEVEL 3: CHECK Q3 (If Q1 & Q2 Empty) ---
        if (idx == -1) {
            int min_arrival = 100000;
            for(int i=0; i<n; i++) {
                if (p[i].priority == 3 && p[i].remaining_time > 0 && p[i].arrival_time <= current_time) {
                    if (p[i].arrival_time < min_arrival) {
                        min_arrival = p[i].arrival_time;
                        idx = i;
                        current_queue = 3;
                    }
                }
            }
        }

        // --- EXECUTION ---
        if (idx != -1) {
            // Context Switch Check
            if (idx != last_pid) {
                execution_timer = 0; // New process starts, reset timer
            }

            p[idx].remaining_time--;
            current_time++;
            execution_timer++;
            last_pid = idx;

            // Check if finished
            if (p[idx].remaining_time == 0) {
                p[idx].completion_time = current_time;
                p[idx].turnaround_time = p[idx].completion_time - p[idx].arrival_time;
                p[idx].waiting_time = p[idx].turnaround_time - p[idx].burst_time;
                completed++;
                
                printf("%d\tQ%d\t%d\t%d\t%d\t%d\n", 
                       p[idx].pid, current_queue, p[idx].arrival_time, 
                       p[idx].burst_time, p[idx].waiting_time, p[idx].turnaround_time);
                execution_timer = 0; // Reset
            } 
            else {
                // DEMOTION LOGIC
                // Only demote if quantum is exceeded AND process is not finished
                if (current_queue == 1 && execution_timer == q1_quantum) {
                    p[idx].priority = 2; // Demote to Q2
                    execution_timer = 0;
                    // printf("  [DEBUG] Process %d demoted to Q2 at time %d\n", p[idx].pid, current_time);
                }
                else if (current_queue == 2 && execution_timer == q2_quantum) {
                    p[idx].priority = 3; // Demote to Q3
                    execution_timer = 0;
                    // printf("  [DEBUG] Process %d demoted to Q3 at time %d\n", p[idx].pid, current_time);
                }
            }
        } else {
            current_time++;
        }
    }

    float total_wt = 0, total_tat = 0;
    for(int i=0; i<n; i++) {
        total_wt += p[i].waiting_time;
        total_tat += p[i].turnaround_time;
    }
    printf("Average Waiting Time: %.2f\n", total_wt / n);
    printf("Average Turnaround Time: %.2f\n", total_tat / n);
}