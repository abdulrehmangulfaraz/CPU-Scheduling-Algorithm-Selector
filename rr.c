#include "scheduler.h"
#include <stdbool.h>

// Simple Queue Implementation for RR
int queue[100];
int front = -1, rear = -1;

void enqueue(int pid_index) {
    if (front == -1) front = 0;
    rear++;
    queue[rear] = pid_index;
}

int dequeue() {
    if (front == -1 || front > rear) return -1;
    return queue[front++];
}

bool is_empty() {
    return (front == -1 || front > rear);
}

// Function to check if a process is already in queue (to avoid duplicates)

void calculate_rr(Process p[], int n, int quantum) {
    printf("\n--- Executing Round Robin (Quantum: %d) ---\n", quantum);

    // Reset Queue for this run
    front = -1; 
    rear = -1;
    
    // Status array to track if a process is currently in the queue
    int in_queue[100] = {0}; 
    
    // Reset internal state
    int current_time = 0;
    int completed = 0;
        
    // Initial loading of queue
    for(int i=0; i<n; i++) {
        // If it arrives at 0, add it
        if(p[i].arrival_time <= current_time && p[i].remaining_time > 0) {
            enqueue(i);
            in_queue[i] = 1;
        }
    }

    // Loop until all processes are completed
    while(completed < n) {
        int idx = dequeue();
        
        if (idx == -1) {
            // Queue is empty, but processes are not done. 
            // This means we have a gap in arrival times (CPU Idle).
            current_time++;
            
            // Check if anyone arrived now
            for(int i=0; i<n; i++) {
                if(p[i].arrival_time <= current_time && p[i].remaining_time > 0 && in_queue[i] == 0) {
                    enqueue(i);
                    in_queue[i] = 1;
                }
            }
            continue;
        }

        // We have a process to run
        // Determine how long to run it
        int time_slice = 0;
        if (p[idx].remaining_time > quantum) {
            time_slice = quantum;
        } else {
            time_slice = p[idx].remaining_time;
        }

        // Execute
        p[idx].remaining_time -= time_slice;
        current_time += time_slice;

        // CRITICAL STEP: Check for new arrivals BEFORE re-enqueueing current process
        for(int i=0; i<n; i++) {
            if(p[i].arrival_time <= current_time && 
               p[i].remaining_time > 0 && 
               in_queue[i] == 0 && 
               i != idx) { // Don't add self yet
                
                enqueue(i);
                in_queue[i] = 1;
            }
        }

        // If process is finished
        if (p[idx].remaining_time == 0) {
            p[idx].completion_time = current_time;
            p[idx].turnaround_time = p[idx].completion_time - p[idx].arrival_time;
            p[idx].waiting_time = p[idx].turnaround_time - p[idx].burst_time;
            completed++;
            // Don't enqueue again
        } else {
            // Not finished, put back in queue
            enqueue(idx);
        }
    }

    // Print Results
    printf("PID\tArrival\tBurst\tWaiting\tTurnaround\n");
    float total_wt = 0, total_tat = 0;
    for (int i = 0; i < n; i++) {
        printf("%d\t%d\t%d\t%d\t%d\n", 
               p[i].pid, p[i].arrival_time, p[i].burst_time, 
               p[i].waiting_time, p[i].turnaround_time);
        total_wt += p[i].waiting_time;
        total_tat += p[i].turnaround_time;
    }
    printf("Average Waiting Time: %.2f\n", total_wt / n);
    printf("Average Turnaround Time: %.2f\n", total_tat / n);
}