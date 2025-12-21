#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int pid;
    int arrival_time;
    int burst_time;
    int priority;
    
    // Execution metrics
    int remaining_time;
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