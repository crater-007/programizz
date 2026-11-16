#include <iostream>
#include <cmath>
#include <iomanip>
#include <cstdint>

using namespace std;

int main() {
    int LAS_bits, PAS_bits, Page_Size_bytes;

    cout << "Enter Logical Address bits: ";
    cin >> LAS_bits;

    cout << "Enter Physical Address bits: ";
    cin >> PAS_bits;

    cout << "Enter Page Size (in bytes): ";
    cin >> Page_Size_bytes;

    if (!cin || LAS_bits <= 0 || PAS_bits <= 0 || Page_Size_bytes <= 0 ||
        (Page_Size_bytes & (Page_Size_bytes - 1)) != 0) {
        cout << "Invalid input. Enter positive address bit values and a page size that is a power of 2." << endl;
        return 1;
    }

    int offsetBits = static_cast<int>(log2(Page_Size_bytes));

    if (offsetBits > LAS_bits || offsetBits > PAS_bits || LAS_bits >= 64 || PAS_bits >= 64) {
        cout << "Invalid input. Address sizes must be below 64 bits and large enough for the page offset." << endl;
        return 1;
    }

    uint64_t logicalSpace = 1ULL << LAS_bits;
    uint64_t physicalSpace = 1ULL << PAS_bits;
    uint64_t totalPages = 1ULL << (LAS_bits - offsetBits);
    uint64_t totalFrames = 1ULL << (PAS_bits - offsetBits);

    int pageNumberBits = LAS_bits - offsetBits;
    int frameNumberBits = PAS_bits - offsetBits;

    cout << "\n--- Paging Architecture Report ---\n";

    cout << "Logical Address Space : ";
    if (logicalSpace >= (1ULL << 30)) {
        cout << logicalSpace / (1ULL << 20) << " MB (" << logicalSpace / (1ULL << 30) << " GB)";
    } else if (logicalSpace >= (1ULL << 20)) {
        cout << logicalSpace / (1ULL << 20) << " MB";
    } else {
        cout << logicalSpace / 1024 << " KB";
    }

    cout << "\nPhysical Address Space: ";
    if (physicalSpace >= (1ULL << 30)) {
        cout << physicalSpace / (1ULL << 20) << " MB (" << physicalSpace / (1ULL << 30) << " GB)";
    } else if (physicalSpace >= (1ULL << 20)) {
        cout << physicalSpace / (1ULL << 20) << " MB";
    } else {
        cout << physicalSpace / 1024 << " KB";
    }

    cout << "\nPage / Frame Size : " << Page_Size_bytes << " Bytes";
    cout << "\nOffset bits (d) : " << offsetBits << " bits";
    cout << "\nPage Number bits (p) : " << pageNumberBits << " bits";
    cout << "\nFrame Number bits (f) : " << frameNumberBits << " bits";
    cout << "\nTotal Number of Pages : " << totalPages;
    cout << "\nTotal Number of Frames: " << totalFrames << endl;

    return 0;
}
