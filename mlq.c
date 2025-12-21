#include "scheduler.h"
#include <stdbool.h>

void calculate_mlq(Process p[], int n) {
    printf("\n--- Executing Multi-Level Queue (MLQ) ---\n");
    printf("Queue 1 (Priority 1): Round Robin (Q=4)\n");
    printf("Queue 2 (Priority 2): FCFS\n");

    int current_time = 0;
    int completed = 0;
    int rr_quantum = 4;
    int rr_timer = 0;
    int last_process_id = -1; 

    printf("PID\tQ_Level\tArrival\tBurst\tWaiting\tTurnaround\n");

    while (completed < n) {
        int idx = -1;
        bool q1_active = false;
        
        // --- STRATEGY: Check Queue 1 (High Priority) First ---
        
        // 1. Continue current Q1 process if valid
        if (last_process_id != -1 && p[last_process_id].priority == 1 && 
            p[last_process_id].remaining_time > 0 && rr_timer < rr_quantum) {
            
            idx = last_process_id;
            q1_active = true;
        } 
        else {
            // 2. Find next available Q1 process (Round Robin scan)
            int start_search = (last_process_id == -1) ? 0 : (last_process_id + 1) % n;
            
            for (int i = 0; i < n; i++) {
                int curr = (start_search + i) % n;
                if (p[curr].priority == 1 && p[curr].remaining_time > 0 && p[curr].arrival_time <= current_time) {
                    idx = curr;
                    q1_active = true;
                    rr_timer = 0; 
                    break;
                }
            }
        }

        // --- STRATEGY: Check Queue 2 (Low Priority) Only if Q1 is Empty ---
        if (!q1_active) {
            int min_arrival = 100000;
            
            for (int i = 0; i < n; i++) {
                // Check for Priority != 1 (User processes)
                if (p[i].priority != 1 && p[i].remaining_time > 0 && p[i].arrival_time <= current_time) {
                    if (p[i].arrival_time < min_arrival) {
                        min_arrival = p[i].arrival_time;
                        idx = i;
                    }
                }
            }
        }

        // --- EXECUTION PHASE ---
        if (idx != -1) {
            p[idx].remaining_time--;
            current_time++;
            
            if (p[idx].priority == 1) {
                rr_timer++;
            }
            
            last_process_id = idx;

            if (p[idx].remaining_time == 0) {
                p[idx].completion_time = current_time;
                p[idx].turnaround_time = p[idx].completion_time - p[idx].arrival_time;
                p[idx].waiting_time = p[idx].turnaround_time - p[idx].burst_time;
                completed++;
                
                printf("%d\tQ%d\t%d\t%d\t%d\t%d\n", 
                       p[idx].pid, p[idx].priority, p[idx].arrival_time, 
                       p[idx].burst_time, p[idx].waiting_time, p[idx].turnaround_time);
                
                if (p[idx].priority == 1) rr_timer = 0;
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