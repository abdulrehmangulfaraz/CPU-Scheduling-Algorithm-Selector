#include "scheduler.h"
#include <string.h>
#include <unistd.h> 
#include <fcntl.h> 

// --- HELPER FUNCTIONS ---

void reset_metrics(Process p[], int n) {
    for (int i = 0; i < n; i++) {
        p[i].waiting_time = 0;
        p[i].turnaround_time = 0;
        p[i].completion_time = 0;
        p[i].start_time = 0;
        p[i].remaining_time = p[i].burst_time;
    }
}

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

// --- DISPLAY FUNCTIONS ---

void print_input_data(Process p[], int n, char* title) {
    printf("\n>>> LOADED SCENARIO: %s\n", title);
    printf("----------------------------------------------------------\n");
    printf("PID | Arrival | Burst | Priority | Q_ID (MLQ) \n");
    printf("----------------------------------------------------------\n");
    for(int i=0; i<n; i++) {
        printf(" %-2d | %-7d | %-5d | %-8d | %-5d\n", 
               p[i].pid, p[i].arrival_time, p[i].burst_time, 
               p[i].priority, p[i].queue_id);
    }
    printf("----------------------------------------------------------\n");
}

void print_selector_decision() {
    printf("\n");
    printf("=============================================================\n");
    printf("                  FINAL ANALYSIS & SELECTION                 \n");
    printf("=============================================================\n");
    printf("%-25s | %-12s | %-12s\n", "Algorithm", "Avg Wait", "Avg TAT");
    printf("-------------------------------------------------------------\n");
    
    int best_idx = 0;
    for(int i=0; i<result_count; i++) {
        printf("%-25s | %-12.2f | %-12.2f\n", 
               results[i].name, 
               results[i].avg_waiting_time, 
               results[i].avg_turnaround_time);
        
        if (results[i].avg_waiting_time < results[best_idx].avg_waiting_time) {
            best_idx = i;
        }
    }
    printf("-------------------------------------------------------------\n");
    printf(">>> RECOMMENDED ALGORITHM: %s\n", results[best_idx].name);
    printf(">>> REASON: Lowest Average Waiting Time (%.2f)\n", results[best_idx].avg_waiting_time);
    printf("=============================================================\n");
}

// --- EXECUTION ENGINE (Silent) ---

void calculate_all_stats(Process p_orig[], int n) {
    Process p[20];
    result_count = 0; 

    // Redirect stdout to NULL to silence algorithms during calc
    int stdout_backup = dup(STDOUT_FILENO);
    int dev_null = open("/dev/null", O_WRONLY);
    dup2(dev_null, STDOUT_FILENO);

    // 1. FCFS
    for(int i=0; i<n; i++) p[i] = p_orig[i];
    reset_metrics(p, n);
    calculate_fcfs(p, n);
    save_result("FCFS", p, n);

    // 2. SJF
    for(int i=0; i<n; i++) p[i] = p_orig[i];
    reset_metrics(p, n);
    calculate_sjf(p, n);
    save_result("SJF", p, n);

    // 3. Priority
    for(int i=0; i<n; i++) p[i] = p_orig[i];
    reset_metrics(p, n);
    calculate_priority(p, n);
    save_result("Priority", p, n);

    // 4. MLQ
    for(int i=0; i<n; i++) p[i] = p_orig[i];
    reset_metrics(p, n);
    calculate_mlq(p, n);
    save_result("Multi-Level Queue", p, n);

    // 5. MLFQ
    for(int i=0; i<n; i++) p[i] = p_orig[i];
    reset_metrics(p, n);
    calculate_mlfq(p, n);
    save_result("MLFQ", p, n);

    // 6. Round Robin (Q=2, Q=4)
    int quantums[] = {2, 4};
    for(int q=0; q<2; q++) {
        for(int i=0; i<n; i++) p[i] = p_orig[i];
        reset_metrics(p, n);
        calculate_rr(p, n, quantums[q]);
        char buffer[50];
        sprintf(buffer, "Round Robin (Q=%d)", quantums[q]);
        save_result(buffer, p, n);
    }

    // Restore stdout
    fflush(stdout);
    dup2(stdout_backup, STDOUT_FILENO);
    close(dev_null);
    close(stdout_backup);
}

// --- SCENARIO LOADER ---
// Initializer format: {pid, arrival, burst, priority, queue_id}
void load_scenario(int choice, Process p[], int *n, char* name_buffer) {
    switch(choice) {
        case 1: 
            *n = 4; strcpy(name_buffer, "Standard Mix");
            // Standard: Q_ID 1 default
            p[0] = (Process){1, 0, 8, 2, 1}; p[1] = (Process){2, 1, 4, 1, 1};
            p[2] = (Process){3, 2, 9, 3, 1}; p[3] = (Process){4, 3, 5, 2, 1};
            break;
        case 2: 
            *n = 3; strcpy(name_buffer, "Convoy Effect");
            p[0] = (Process){1, 0, 100, 3, 1}; p[1] = (Process){2, 1, 1, 1, 1};
            p[2] = (Process){3, 2, 1, 1, 1};
            break;
        case 3: 
            *n = 3; strcpy(name_buffer, "Reverse Convoy");
            p[0] = (Process){1, 0, 2, 1, 1}; p[1] = (Process){2, 1, 3, 1, 1};
            p[2] = (Process){3, 2, 100, 3, 1};
            break;
        case 4: 
            *n = 5; strcpy(name_buffer, "Batch Arrival (t=0)");
            p[0] = (Process){1, 0, 10, 3, 1}; p[1] = (Process){2, 0, 1, 1, 1};
            p[2] = (Process){3, 0, 2, 3, 1};  p[3] = (Process){4, 0, 1, 4, 1};
            p[4] = (Process){5, 0, 5, 2, 1};
            break;
        case 5: 
            *n = 3; strcpy(name_buffer, "Priority Dominance");
            p[0] = (Process){1, 0, 10, 3, 1}; p[1] = (Process){2, 2, 4, 2, 1};
            p[2] = (Process){3, 4, 2, 1, 1};
            break;
        case 6: 
            *n = 4; strcpy(name_buffer, "Equal Load");
            p[0] = (Process){1, 0, 4, 1, 1}; p[1] = (Process){2, 1, 4, 1, 1};
            p[2] = (Process){3, 2, 4, 1, 1}; p[3] = (Process){4, 3, 4, 1, 1};
            break;
        case 7: 
            *n = 3; strcpy(name_buffer, "MLFQ Stress");
            // MLFQ ignores initial queue, starts everyone at 1
            p[0] = (Process){1, 0, 20, 1, 1}; p[1] = (Process){2, 4, 2, 1, 1};
            p[2] = (Process){3, 8, 4, 1, 1};
            break;
        case 8: 
            *n = 5; strcpy(name_buffer, "Starvation Risk");
            p[0] = (Process){1, 0, 20, 5, 1}; p[1] = (Process){2, 1, 2, 1, 1};
            p[2] = (Process){3, 2, 2, 1, 1};  p[3] = (Process){4, 3, 2, 1, 1};
            p[4] = (Process){5, 4, 2, 1, 1};
            break;
        case 9: 
            *n = 3; strcpy(name_buffer, "MLQ System vs User");
            // IMPORTANT: Here we set Q_ID explicitly!
            // P1(Sys) -> Q1, P2(User) -> Q2, P3(Sys) -> Q1
            p[0] = (Process){1, 0, 10, 1, 1}; 
            p[1] = (Process){2, 0, 5,  2, 2}; 
            p[2] = (Process){3, 2, 8,  1, 1};
            break;
        case 10: 
            *n = 5; strcpy(name_buffer, "Complex Random");
            p[0] = (Process){1, 0, 12, 3, 1}; p[1] = (Process){2, 3, 6, 2, 1};
            p[2] = (Process){3, 5, 3, 1, 1};  p[3] = (Process){4, 10, 8, 2, 1};
            p[4] = (Process){5, 12, 2, 1, 1};
            break;
    }
}

// --- MAIN MENU ---

int main() {
    Process master_procs[20];
    Process temp_procs[20];
    int n = 0;
    int choice;
    char scenario_name[100];

    while(1) {
        printf("\n=============================================\n");
        printf("   CPU SCHEDULING ALGORITHM SELECTOR (v3.0)  \n");
        printf("=============================================\n");
        printf("1. Run a Test Scenario (1-10)\n");
        printf("2. Custom Input\n");
        printf("3. Exit\n");
        printf("choice> ");
        scanf("%d", &choice);

        if (choice == 3) break;

        if (choice == 1) {
            printf("Enter Scenario # (1-10): ");
            int sc;
            scanf("%d", &sc);
            if(sc < 1 || sc > 10) { printf("Invalid.\n"); continue; }
            load_scenario(sc, master_procs, &n, scenario_name);
            print_input_data(master_procs, n, scenario_name);
        } 
        else if (choice == 2) {
            strcpy(scenario_name, "Custom User Input");
            printf("Enter number of processes: "); 
            scanf("%d", &n);
            printf("\n--- Enter Details ---\n");
            for(int i=0; i<n; i++) {
                master_procs[i].pid = i+1;
                printf("Process %d Arrival Time: ", i+1); 
                scanf("%d", &master_procs[i].arrival_time);
                printf("Process %d Burst Time:   ", i+1); 
                scanf("%d", &master_procs[i].burst_time);
                printf("Process %d Priority:     ", i+1); 
                scanf("%d", &master_procs[i].priority);
                printf("Process %d Queue ID (1/2): ", i+1); // Added Input
                scanf("%d", &master_procs[i].queue_id);
                
                master_procs[i].remaining_time = master_procs[i].burst_time;
            }
            print_input_data(master_procs, n, scenario_name);
        }

        printf("\nAnalyzing workload... Please wait.\n");
        calculate_all_stats(master_procs, n);
        print_selector_decision();

        while(1) {
            printf("\n-------------------------------------------------------------\n");
            printf("[ DRILL DOWN MENU ] View detailed execution logs:\n");
            printf("1. FCFS                     2. SJF\n");
            printf("3. Priority                 4. Multi-Level Queue (MLQ)\n");
            printf("5. Multi-Level Feedback     6. Round Robin (RR)\n");
            printf("0. << Back to Main Menu\n");
            printf("-------------------------------------------------------------\n");
            printf("Check Algorithm > ");
            
            int sub;
            scanf("%d", &sub);
            if(sub == 0) break;

            for(int i=0; i<n; i++) temp_procs[i] = master_procs[i];
            reset_metrics(temp_procs, n);

            if(sub==1) calculate_fcfs(temp_procs, n);
            else if(sub==2) calculate_sjf(temp_procs, n);
            else if(sub==3) calculate_priority(temp_procs, n);
            else if(sub==4) calculate_mlq(temp_procs, n);
            else if(sub==5) calculate_mlfq(temp_procs, n);
            else if(sub==6) {
                printf("Enter Quantum: ");
                int q; scanf("%d", &q);
                calculate_rr(temp_procs, n, q);
            }
        }
    }
    return 0;
}