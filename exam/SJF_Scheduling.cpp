#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Process {
    int id;
    int arrivalTime;
    int burstTime;
    int waitingTime;
    int turnaroundTime;
    int completionTime;
    bool completed;
};

int main() {
    int n;
    cout << "Enter the number of processes: ";
    cin >> n;

    if (n <= 0) {
        cout << "Number of processes must be greater than zero.\n";
        return 0;
    }

    vector<Process> processes(n);

    for (int i = 0; i < n; i++) {
        processes[i].id = i + 1;
        processes[i].completed = false;
        cout << "Enter arrival time and burst time for P" << i + 1 << ": ";
        cin >> processes[i].arrivalTime >> processes[i].burstTime;

        if (processes[i].arrivalTime < 0 || processes[i].burstTime <= 0) {
            cout << "Arrival time cannot be negative and burst time must be positive.\n";
            return 0;
        }
    }

    int currentTime = 0;
    int completedCount = 0;
    double totalWaitingTime = 0;
    double totalTurnaroundTime = 0;

    while (completedCount < n) {
        int selected = -1;

        for (int i = 0; i < n; i++) {
            if (!processes[i].completed && processes[i].arrivalTime <= currentTime) {
                if (selected == -1 ||
                    processes[i].burstTime < processes[selected].burstTime ||
                    (processes[i].burstTime == processes[selected].burstTime &&
                     processes[i].arrivalTime < processes[selected].arrivalTime)) {
                    selected = i;
                }
            }
        }

        if (selected == -1) {
            currentTime++;
            continue;
        }

        processes[selected].waitingTime = currentTime - processes[selected].arrivalTime;
        currentTime += processes[selected].burstTime;
        processes[selected].completionTime = currentTime;
        processes[selected].turnaroundTime =
            processes[selected].completionTime - processes[selected].arrivalTime;
        processes[selected].completed = true;

        totalWaitingTime += processes[selected].waitingTime;
        totalTurnaroundTime += processes[selected].turnaroundTime;
        completedCount++;
    }

    cout << "\nProcess\tArrival\tBurst\tCompletion\tWaiting\tTurnaround\n";
    for (const Process& p : processes) {
        cout << "P" << p.id << "\t"
             << p.arrivalTime << "\t"
             << p.burstTime << "\t"
             << p.completionTime << "\t\t"
             << p.waitingTime << "\t"
             << p.turnaroundTime << "\n";
    }

    cout << fixed << setprecision(2);
    cout << "\nAverage waiting time: " << totalWaitingTime / n << "\n";
    cout << "Average turnaround time: " << totalTurnaroundTime / n << "\n";

    return 0;
}
