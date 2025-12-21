#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdio.h>
#include <stdlib.h>

// Represents a single process
typedef struct {
    int pid;            // Process ID
    int arrival_time;   // Arrival Time
    int burst_time;     // CPU Burst Time
    int priority;       // Priority (lower value might indicate higher priority, we'll define this later)
    
    // Execution metrics
    int remaining_time; // For preemptive algorithms (like RR)
    int waiting_time;
    int turnaround_time;
    int completion_time;
    int start_time;     // Useful for analysis
} Process;

// Function Prototypes for the Algorithms (to be implemented later)
void calculate_fcfs(Process p[], int n);
void calculate_sjf(Process p[], int n);
void calculate_priority(Process p[], int n);
void calculate_rr(Process p[], int n, int quantum);

// Helper function to reset metrics between algorithms
// This is crucial because we will run different algorithms on the SAME dataset
void reset_metrics(Process p[], int n);

#endif