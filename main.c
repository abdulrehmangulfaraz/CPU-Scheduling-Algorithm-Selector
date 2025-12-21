#include "scheduler.h"
#include <string.h>

// Helper to reset process metrics between runs
void reset_metrics(Process p[], int n) {
    for (int i = 0; i < n; i++) {
        p[i].waiting_time = 0;
        p[i].turnaround_time = 0;
        p[i].completion_time = 0;
        p[i].start_time = 0;
        p[i].remaining_time = p[i].burst_time;
    }
}

// Structure to track results for comparison
typedef struct {
    char name[50];
    float avg_waiting_time;
    float avg_turnaround_time;
} AlgoResult;

AlgoResult results[10];
int result_count = 0;

void save_result(char* name, Process p[], int n) {
    float total_wt = 0;
    float total_tat = 0;
    for(int i=0; i<n; i++) {
        total_wt += p[i].waiting_time;
        total_tat += p[i].turnaround_time;
    }
    strcpy(results[result_count].name, name);
    results[result_count].avg_waiting_time = total_wt / n;
    results[result_count].avg_turnaround_time = total_tat / n;
    result_count++;
}

void print_selector_decision() {
    printf("\n=============================================\n");
    printf("       FINAL ANALYSIS & SELECTION            \n");
    printf("=============================================\n");
    printf("Algorithm\t\tAvg Wait\tAvg TAT\n");
    printf("---------------------------------------------\n");
    
    int best_idx = 0;
    for(int i=0; i<result_count; i++) {
        printf("%-20s\t%.2f\t\t%.2f\n", results[i].name, results[i].avg_waiting_time, results[i].avg_turnaround_time);
        
        // Selection Criteria: Minimal Average Waiting Time
        if (results[i].avg_waiting_time < results[best_idx].avg_waiting_time) {
            best_idx = i;
        }
    }
    
    printf("---------------------------------------------\n");
    printf("RECOMMENDED ALGORITHM: %s\n", results[best_idx].name);
    printf("REASON: Lowest Average Waiting Time (%.2f)\n", results[best_idx].avg_waiting_time);
    printf("=============================================\n");
}

int main() {
    printf("--- CPU Scheduling Algorithm Selector ---\n");

    // --- INPUT DATA (Change this for different scenarios) ---
    // Scenario: Mixed Burst Times
    Process test_procs[3] = {
        {1, 0, 10, 1, 10, 0, 0, 0, 0}, // P1: High Burst, High Priority (for MLQ)
        {2, 1, 5,  2, 5,  0, 0, 0, 0}, // P2: Med Burst, Low Priority
        {3, 2, 8,  1, 8,  0, 0, 0, 0}  // P3: Med Burst, High Priority
    };
    int n = 3;

    // 1. FCFS
    reset_metrics(test_procs, n);
    calculate_fcfs(test_procs, n);
    save_result("FCFS", test_procs, n);

    // 2. SJF
    reset_metrics(test_procs, n);
    calculate_sjf(test_procs, n);
    save_result("SJF (Non-Preemptive)", test_procs, n);

    // 3. Priority
    reset_metrics(test_procs, n);
    calculate_priority(test_procs, n);
    save_result("Priority Scheduling", test_procs, n);

    // 4. MLQ
    // Ensure priorities are set for MLQ demo (P1, P3 = Sys/1, P2 = User/2)
    test_procs[0].priority = 1; 
    test_procs[1].priority = 2; 
    test_procs[2].priority = 1; 
    reset_metrics(test_procs, n);
    calculate_mlq(test_procs, n);
    save_result("Multi-Level Queue", test_procs, n);

    // 5. MLFQ
    reset_metrics(test_procs, n);
    calculate_mlfq(test_procs, n);
    save_result("MLFQ", test_procs, n);

    // 6. Round Robin (Different Quantums)
    int quantums[] = {2, 4};
    for (int i = 0; i < 2; i++) {
        reset_metrics(test_procs, n);
        calculate_rr(test_procs, n, quantums[i]);
        char buffer[50];
        sprintf(buffer, "Round Robin (Q=%d)", quantums[i]);
        save_result(buffer, test_procs, n);
    }

    // FINAL OUTPUT
    print_selector_decision();

    return 0;
}