#include <iostream>
#include <vector>
#include <algorithm>
#include <iomanip>
using namespace std;

struct Process {
    int pid;
    int arrivalTime;
    int burstTime;
    int completionTime;
    int turnaroundTime;
    int waitingTime;
};

int main() {
    int n;

    cout << "Enter number of processes: ";
    cin >> n;

    if (n <= 0) {
        cout << "Number of processes must be greater than 0.\n";
        return 0;
    }

    vector<Process> p(n);

    for (int i = 0; i < n; i++) {
        p[i].pid = i + 1;

        cout << "\nEnter arrival time of P" << p[i].pid << ": ";
        cin >> p[i].arrivalTime;

        cout << "Enter burst time of P" << p[i].pid << ": ";
        cin >> p[i].burstTime;

        if (p[i].arrivalTime < 0 || p[i].burstTime <= 0) {
            cout << "Arrival time cannot be negative and burst time must be positive.\n";
            return 0;
        }
    }

    // FCFS executes processes in increasing order of arrival time.
    // stable_sort keeps the input order when arrival times are equal.
    stable_sort(p.begin(), p.end(), [](const Process &a, const Process &b) {
        return a.arrivalTime < b.arrivalTime;
    });

    int currentTime = 0;
    double totalWaitingTime = 0;
    double totalTurnaroundTime = 0;

    for (int i = 0; i < n; i++) {
        // If the CPU is idle, move time to the next process's arrival.
        if (currentTime < p[i].arrivalTime) {
            currentTime = p[i].arrivalTime;
        }

        p[i].completionTime = currentTime + p[i].burstTime;
        p[i].turnaroundTime = p[i].completionTime - p[i].arrivalTime;
        p[i].waitingTime = p[i].turnaroundTime - p[i].burstTime;

        currentTime = p[i].completionTime;

        totalWaitingTime += p[i].waitingTime;
        totalTurnaroundTime += p[i].turnaroundTime;
    }

    cout << "\nFCFS Scheduling Results\n";
    cout << "-----------------------------------------------------------------\n";
    cout << left << setw(10) << "Process"
         << setw(12) << "AT"
         << setw(12) << "BT"
         << setw(12) << "CT"
         << setw(12) << "TAT"
         << setw(12) << "WT" << '\n';
    cout << "-----------------------------------------------------------------\n";

    for (int i = 0; i < n; i++) {
        cout << left << setw(10) << ("P" + to_string(p[i].pid))
             << setw(12) << p[i].arrivalTime
             << setw(12) << p[i].burstTime
             << setw(12) << p[i].completionTime
             << setw(12) << p[i].turnaroundTime
             << setw(12) << p[i].waitingTime << '\n';
    }

    cout << "-----------------------------------------------------------------\n";
    cout << fixed << setprecision(2);
    cout << "Average Waiting Time: " << totalWaitingTime / n << '\n';
    cout << "Average Turnaround Time: " << totalTurnaroundTime / n << '\n';

    // Display the execution order (Gantt chart sequence).
    cout << "\nExecution order (FCFS): ";
    for (int i = 0; i < n; i++) {
        cout << "P" << p[i].pid;
        if (i != n - 1) {
            cout << " -> ";
        }
    }
    cout << '\n';

    return 0;
}
