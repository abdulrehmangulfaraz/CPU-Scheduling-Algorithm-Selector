#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    // --- INPUTS ---
    int pid;            // Identification
    int arrival_time;   // Scheduling order realism
    int burst_time;     // Core scheduling metric
    int priority;       // Priority-based decisions
    int queue_id;       // Initial Queue ID (For MLQ)
    
    // --- OUTPUTS / TRACKING ---
    int remaining_time; // Execution tracking
    int waiting_time;
    int turnaround_time;
    int completion_time;
    int start_time;
} Process;

// Function Prototypes
void calculate_fcfs(Process p[], int n);
void calculate_sjf(Process p[], int n);
void calculate_priority(Process p[], int n);
void calculate_rr(Process p[], int n, int quantum);
void calculate_mlq(Process p[], int n);
void calculate_mlfq(Process p[], int n);

void reset_metrics(Process p[], int n);

#endif