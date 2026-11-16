#include <iostream>
#include <vector>
#include <iomanip>

using namespace std;

struct Process {
    int size;
};

struct Block {
    int size;
    int remaining;
};

void runAllocation(const vector<int>& blockSizes, const vector<int>& processSizes, int method) {
    vector<Block> blocks;

    for (int size : blockSizes) {
        blocks.push_back({size, size});
    }

    string algorithm;

    if (method == 0) {
        algorithm = "First Fit";
    } else if (method == 1) {
        algorithm = "Best Fit";
    } else {
        algorithm = "Worst Fit";
    }

    cout << "\n" << algorithm << " Allocation\n";
    cout << left << setw(15) << "Process No."
         << setw(15) << "Process Size"
         << setw(15) << "Block No."
         << "Remaining Space" << '\n';

    cout << string(60, '-') << '\n';

    for (int i = 0; i < static_cast<int>(processSizes.size()); ++i) {
        int selectedBlock = -1;

        for (int j = 0; j < static_cast<int>(blocks.size()); ++j) {
            if (blocks[j].remaining < processSizes[i]) {
                continue;
            }

            if (selectedBlock == -1) {
                selectedBlock = j;
            } else if (method == 1 &&
                       blocks[j].remaining < blocks[selectedBlock].remaining) {
                selectedBlock = j;
            } else if (method == 2 &&
                       blocks[j].remaining > blocks[selectedBlock].remaining) {
                selectedBlock = j;
            }

            if (method == 0) {
                break;
            }
        }

        cout << left << setw(15) << i + 1
             << setw(15) << processSizes[i];

        if (selectedBlock == -1) {
            cout << setw(15) << "Not allocated"
                 << "N/A" << '\n';
        } else {
            blocks[selectedBlock].remaining -= processSizes[i];

            cout << setw(15) << selectedBlock + 1
                 << blocks[selectedBlock].remaining << '\n';
        }
    }
}

int main() {
    int blockCount;
    cout << "Enter number of memory blocks: ";
    cin >> blockCount;

    if (blockCount <= 0) {
        cout << "Invalid number of memory blocks." << endl;
        return 1;
    }

    vector<int> blockSizes(blockCount);

    cout << "Enter sizes of memory blocks:\n";
    for (int i = 0; i < blockCount; ++i) {
        cin >> blockSizes[i];

        if (blockSizes[i] <= 0) {
            cout << "Block sizes must be positive." << endl;
            return 1;
        }
    }

    int processCount;
    cout << "Enter number of processes: ";
    cin >> processCount;

    if (processCount <= 0) {
        cout << "Invalid number of processes." << endl;
        return 1;
    }

    vector<int> processSizes(processCount);

    cout << "Enter memory required by each process:\n";
    for (int i = 0; i < processCount; ++i) {
        cin >> processSizes[i];

        if (processSizes[i] <= 0) {
            cout << "Process sizes must be positive." << endl;
            return 1;
        }
    }

    runAllocation(blockSizes, processSizes, 0);
    runAllocation(blockSizes, processSizes, 1);
    runAllocation(blockSizes, processSizes, 2);

    return 0;
}
