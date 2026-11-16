#include <iostream>
#include <vector>
#include <string>

using namespace std;

bool findSafeSequence(const vector<vector<int>>& allocation,
                      const vector<vector<int>>& need,
                      const vector<int>& available,
                      vector<int>& safeSequence) {
    int n = allocation.size();
    int m = available.size();

    vector<int> work = available;
    vector<bool> finished(n, false);
    safeSequence.clear();

    while (safeSequence.size() < n) {
        bool found = false;

        for (int i = 0; i < n; i++) {
            if (finished[i]) {
                continue;
            }

            bool canFinish = true;

            for (int j = 0; j < m; j++) {
                if (need[i][j] > work[j]) {
                    canFinish = false;
                    break;
                }
            }

            if (canFinish) {
                for (int j = 0; j < m; j++) {
                    work[j] += allocation[i][j];
                }

                finished[i] = true;
                safeSequence.push_back(i);
                found = true;
            }
        }

        if (!found) {
            return false;
        }
    }

    return true;
}

void displaySequence(const vector<int>& sequence) {
    for (size_t i = 0; i < sequence.size(); i++) {
        cout << "P" << sequence[i];

        if (i + 1 < sequence.size()) {
            cout << " -> ";
        }
    }

    cout << endl;
}

int main() {
    int n, m;

    cout << "Enter number of processes: ";
    cin >> n;

    cout << "Enter number of resource types: ";
    cin >> m;

    if (n <= 0 || m <= 0) {
        cout << "Invalid number of processes or resources." << endl;
        return 0;
    }

    vector<vector<int>> allocation(n, vector<int>(m));
    vector<vector<int>> maximum(n, vector<int>(m));
    vector<vector<int>> need(n, vector<int>(m));
    vector<int> available(m);

    cout << "\nEnter the Allocation Matrix:" << endl;
    for (int i = 0; i < n; i++) {
        cout << "Process P" << i << ": ";
        for (int j = 0; j < m; j++) {
            cin >> allocation[i][j];
        }
    }

    cout << "\nEnter the Maximum Demand Matrix:" << endl;
    for (int i = 0; i < n; i++) {
        cout << "Process P" << i << ": ";
        for (int j = 0; j < m; j++) {
            cin >> maximum[i][j];
        }
    }

    cout << "\nEnter the Available Resources:" << endl;
    for (int j = 0; j < m; j++) {
        cin >> available[j];
    }

    bool valid = true;

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            if (allocation[i][j] < 0 || maximum[i][j] < allocation[i][j]) {
                valid = false;
            }

            need[i][j] = maximum[i][j] - allocation[i][j];
        }
    }

    for (int j = 0; j < m; j++) {
        if (available[j] < 0) {
            valid = false;
        }
    }

    if (!valid) {
        cout << "\nInvalid input. Maximum demand must be at least the allocation." << endl;
        return 0;
    }

    cout << "\nNeed Matrix:" << endl;
    for (int i = 0; i < n; i++) {
        for (int j = 0; j < m; j++) {
            cout << need[i][j] << " ";
        }
        cout << endl;
    }

    vector<int> safeSequence;

    if (findSafeSequence(allocation, need, available, safeSequence)) {
        cout << "\nThe system is in a SAFE state." << endl;
        cout << "Safe sequence: ";
        displaySequence(safeSequence);
    } else {
        cout << "\nThe system is in an UNSAFE state." << endl;
    }

    char choice;
    cout << "\nDo you want to make a resource request? (y/n): ";
    cin >> choice;

    if (choice == 'y' || choice == 'Y') {
        int process;
        vector<int> request(m);

        cout << "Enter process number (0 to " << n - 1 << "): ";
        cin >> process;

        if (process < 0 || process >= n) {
            cout << "Invalid process number." << endl;
            return 0;
        }

        cout << "Enter the request vector: ";
        for (int j = 0; j < m; j++) {
            cin >> request[j];
        }

        bool requestValid = true;

        for (int j = 0; j < m; j++) {
            if (request[j] < 0 || request[j] > need[process][j]) {
                requestValid = false;
            }

            if (request[j] > available[j]) {
                requestValid = false;
            }
        }

        if (!requestValid) {
            cout << "Request cannot be granted because it exceeds the need or available resources." << endl;
            return 0;
        }

        for (int j = 0; j < m; j++) {
            available[j] -= request[j];
            allocation[process][j] += request[j];
            need[process][j] -= request[j];
        }

        if (findSafeSequence(allocation, need, available, safeSequence)) {
            cout << "Request granted. The system remains in a SAFE state." << endl;
            cout << "New safe sequence: ";
            displaySequence(safeSequence);
        } else {
            for (int j = 0; j < m; j++) {
                available[j] += request[j];
                allocation[process][j] -= request[j];
                need[process][j] += request[j];
            }

            cout << "Request denied. It would make the system unsafe." << endl;
        }
    }

    return 0;
}
