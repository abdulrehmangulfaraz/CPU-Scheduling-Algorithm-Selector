#include "scheduler.h"

void reset_metrics(Process p[], int n) {
    for (int i = 0; i < n; i++) {
        p[i].waiting_time = 0;
        p[i].turnaround_time = 0;
        p[i].completion_time = 0;
        p[i].remaining_time = p[i].burst_time; // Reset remaining time to full burst
    }
}

int main() {
    printf("--- CPU Scheduling Algorithm Selector ---\n");
    printf("Initializing process structures...\n");

    // Test Data (We will make this dynamic later)
    Process test_procs[3] = {
        {1, 0, 10, 1, 10, 0, 0, 0, 0}, // PID 1, Arr 0, Burst 10
        {2, 1, 5, 2, 5, 0, 0, 0, 0},   // PID 2, Arr 1, Burst 5
        {3, 2, 8, 3, 8, 0, 0, 0, 0}    // PID 3, Arr 2, Burst 8
    };

    printf("Build successful. Core structure defined.\n");
    return 0;
}