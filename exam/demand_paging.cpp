#include <iostream>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
using namespace std;

struct Request {
    int page;
    char operation;
};

struct Frame {
    int page = -1;
    int reference = 0;
    int modify = 0;
};

void printSecondChance(const vector<Frame>& frames, int page) {
    cout << "After page fault for " << page << ": ";
    for (int i = 0; i < (int)frames.size(); i++) {
        if (frames[i].page == -1) cout << "[ - | R:0 ] ";
        else cout << "[ " << frames[i].page << " | R:" << frames[i].reference << " ] ";
    }
    cout << '\n';
}

void printEnhanced(const vector<Frame>& frames, int page) {
    cout << "After page fault for " << page << ": ";
    for (int i = 0; i < (int)frames.size(); i++) {
        if (frames[i].page == -1) cout << "[ - | R:0 M:0 ] ";
        else cout << "[ " << frames[i].page << " | R:" << frames[i].reference
                  << " M:" << frames[i].modify << " ] ";
    }
    cout << '\n';
}

void simulateSecondChance(int capacity, const vector<Request>& requests) {
    vector<Frame> frames(capacity);
    int hand = 0;
    int hits = 0;
    int faults = 0;

    cout << "\nSECOND CHANCE ALGORITHM\n";
    for (const auto& request : requests) {
        int position = -1;
        for (int i = 0; i < capacity; i++) {
            if (frames[i].page == request.page) {
                position = i;
                break;
            }
        }

        if (position != -1) {
            hits++;
            frames[position].reference = 1;
            continue;
        }

        faults++;
        while (frames[hand].page != -1 && frames[hand].reference == 1) {
            frames[hand].reference = 0;
            hand = (hand + 1) % capacity;
        }

        frames[hand].page = request.page;
        frames[hand].reference = 1;
        frames[hand].modify = 0;
        hand = (hand + 1) % capacity;

        printSecondChance(frames, request.page);
    }

    cout << "Total Page Hits: " << hits << '\n';
    cout << "Total Page Faults: " << faults << '\n';
    cout << "Pages Written to Disk: 0\n";
}

int chooseEnhancedVictim(vector<Frame>& frames, int& hand) {
    int capacity = (int)frames.size();

    for (int i = 0; i < capacity; i++) {
        int index = (hand + i) % capacity;
        if (frames[index].reference == 0 && frames[index].modify == 0)
            return index;
    }

    for (int i = 0; i < capacity; i++) {
        int index = (hand + i) % capacity;
        if (frames[index].reference == 1)
            frames[index].reference = 0;
    }

    for (int i = 0; i < capacity; i++) {
        int index = (hand + i) % capacity;
        if (frames[index].reference == 0 && frames[index].modify == 1)
            return index;
    }

    return hand;
}

void simulateEnhancedSecondChance(int capacity, const vector<Request>& requests) {
    vector<Frame> frames(capacity);
    int hand = 0;
    int hits = 0;
    int faults = 0;
    int diskWrites = 0;

    cout << "\nENHANCED SECOND CHANCE ALGORITHM\n";
    for (const auto& request : requests) {
        int position = -1;
        for (int i = 0; i < capacity; i++) {
            if (frames[i].page == request.page) {
                position = i;
                break;
            }
        }

        if (position != -1) {
            hits++;
            frames[position].reference = 1;
            if (request.operation == 'W')
                frames[position].modify = 1;
            continue;
        }

        faults++;
        int target = -1;
        for (int i = 0; i < capacity; i++) {
            if (frames[i].page == -1) {
                target = i;
                break;
            }
        }

        if (target != -1) {
            target = hand;
            frames[target].page = request.page;
            frames[target].reference = 1;
            frames[target].modify = (request.operation == 'W');
            hand = (hand + 1) % capacity;
        } else {
            target = chooseEnhancedVictim(frames, hand);
            if (frames[target].modify == 1)
                diskWrites++;

            frames[target].page = request.page;
            frames[target].reference = 1;
            frames[target].modify = (request.operation == 'W');
            hand = (target + 1) % capacity;
        }

        printEnhanced(frames, request.page);
    }

    cout << "Total Page Hits: " << hits << '\n';
    cout << "Total Page Faults: " << faults << '\n';
    cout << "Pages Written to Disk: " << diskWrites << '\n';
}

int main() {
    int capacity;
    if (!(cin >> capacity) || capacity <= 0) {
        cout << "Invalid number of frames.\n";
        return 1;
    }

    string line;
    getline(cin, line);
    getline(cin, line);

    vector<Request> requests;
    string token;
    stringstream input(line);

    while (input >> token) {
        size_t separator = token.find(':');
        if (separator == string::npos) {
            cout << "Invalid request format.\n";
            return 1;
        }

        try {
            int page = stoi(token.substr(0, separator));
            string operationText = token.substr(separator + 1);
            if (page < 0 || operationText.size() != 1 ||
                (operationText[0] != 'R' && operationText[0] != 'W')) {
                cout << "Invalid request format.\n";
                return 1;
            }
            requests.push_back({page, operationText[0]});
        } catch (...) {
            cout << "Invalid request format.\n";
            return 1;
        }
    }

    if (requests.empty()) {
        cout << "No page requests were provided.\n";
        return 1;
    }

    cout << "Number of Frames: " << capacity << '\n';
    cout << "Reference String: ";
    for (const auto& request : requests)
        cout << request.page << ':' << request.operation << ' ';
    cout << '\n';

    simulateSecondChance(capacity, requests);
    simulateEnhancedSecondChance(capacity, requests);

    cout << "\nCOMPARATIVE REPORT\n";
    cout << "Second Chance treats every request as a read operation.\n";
    cout << "Enhanced Second Chance uses both reference and modify bits.\n";
    cout << "The page fault counts and disk writes above show the result for this reference string.\n";

    return 0;
}
