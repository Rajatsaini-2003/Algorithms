#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;
struct Process {
    int id;
    int arrival_time;
    int burst_time;
    int waiting_time;
    int turnaround_time;
};
void fcfs(vector<Process>& processes) {
    int n = processes.size();
    int current_time = 0;
    for (int i = 0; i < n; i++) {
        if (current_time < processes[i].arrival_time) {
            current_time = processes[i].arrival_time;
        }
        processes[i].waiting_time = current_time - processes[i].arrival_time;
        current_time += processes[i].burst_time;
        processes[i].turnaround_time = processes[i].waiting_time + processes[i].burst_time;
    }
}
void sjn(vector<Process>& processes) {
    int n = processes.size();
    int current_time = 0;
    vector<bool> completed(n, false);
    for (int i = 0; i < n; i++) {
        int shortest_index = -1;
        for (int j = 0; j < n; j++) {
            if (!completed[j] && processes[j].arrival_time <= current_time &&
                (shortest_index == -1 || processes[j].burst_time < processes[shortest_index].burst_time)) {
                shortest_index = j;
            }
        }
        if (shortest_index != -1) {
            current_time = max(current_time, processes[shortest_index].arrival_time) + processes[shortest_index].burst_time;
            processes[shortest_index].waiting_time = current_time - processes[shortest_index].arrival_time - processes[shortest_index].burst_time;
            processes[shortest_index].turnaround_time = processes[shortest_index].waiting_time + processes[shortest_index].burst_time;
            completed[shortest_index] = true;
        } else {
            current_time++;
        }
    }
}
void round_robin(vector<Process>& processes, int quantum) {
    int n = processes.size();
    vector<int> remaining_burst_time(n);
    for (int i = 0; i < n; i++) {
        remaining_burst_time[i] = processes[i].burst_time;
    }

    int current_time = 0;
    bool done = false;
    while (!done) {
        done = true;
        for (int i = 0; i < n; i++) {
            if (remaining_burst_time[i] > 0) {
                done = false;
                if (remaining_burst_time[i] > quantum) {
                    current_time += quantum;
                    remaining_burst_time[i] -= quantum;
                } else {
                    current_time += remaining_burst_time[i];
                    processes[i].waiting_time = current_time - processes[i].burst_time - processes[i].arrival_time;
                    remaining_burst_time[i] = 0;
                    processes[i].turnaround_time = processes[i].waiting_time + processes[i].burst_time;
                }
            }
        }
    }
}
void print_results(const vector<Process>& processes) {
    for (const auto& p : processes) {
        cout << "Process " << p.id << ": Waiting Time = " << p.waiting_time << ", Turnaround Time = " << p.turnaround_time << endl;
    }
}
int main() {
    vector<Process> processes = {
        {1, 0, 10, 0, 0},
        {2, 1, 5, 0, 0},
        {3, 2, 8, 0, 0}
    };
    cout << "First-Come, First-Served (FCFS):" << endl;
    fcfs(processes);
    print_results(processes);
    for (auto& p : processes) {
        p.waiting_time = 0;
        p.turnaround_time = 0;
    }
    cout << "\nShortest Job Next (SJN):" << endl;
    sjn(processes);
    print_results(processes);
    for (auto& p : processes) {
        p.waiting_time = 0;
        p.turnaround_time = 0;
    }
    int quantum = 2;
    cout << "\nRound Robin (RR) with Quantum " << quantum << ":" << endl;
    round_robin(processes, quantum);
    print_results(processes);
    return 0;
}