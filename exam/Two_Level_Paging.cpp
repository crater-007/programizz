#include <iostream>
#include <iomanip>
#include <cstdint>
#include <cstddef>

using namespace std;

const int TABLE_SIZE = 1024;
const int PAGE_OFFSET_BITS = 12;

struct AddressParts {
    uint32_t directoryIndex;
    uint32_t tableIndex;
    uint32_t offset;
};

struct PageTable {
    int frames[TABLE_SIZE];

    PageTable() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            frames[i] = -1;
        }
    }
};

struct PageDirectory {
    PageTable* tables[TABLE_SIZE];

    PageDirectory() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            tables[i] = nullptr;
        }
    }

    ~PageDirectory() {
        for (int i = 0; i < TABLE_SIZE; i++) {
            delete tables[i];
        }
    }
};

int next_free_frame = 0;

AddressParts parse_address(uint32_t virtual_address) {
    AddressParts parts;

    parts.directoryIndex = (virtual_address >> 22) & 0x3FF;
    parts.tableIndex = (virtual_address >> 12) & 0x3FF;
    parts.offset = virtual_address & 0xFFF;

    return parts;
}

uint32_t translate(uint32_t virtual_address, PageDirectory& directory) {
    AddressParts parts = parse_address(virtual_address);

    cout << "\nVirtual Address: 0x"
         << hex << uppercase << setw(8) << setfill('0')
         << virtual_address << dec << endl;

    cout << "Page Directory Index: " << parts.directoryIndex << endl;
    cout << "Page Table Index: " << parts.tableIndex << endl;

    cout << "Offset: 0x"
         << hex << uppercase << setw(3) << setfill('0')
         << parts.offset << dec << endl;

    if (directory.tables[parts.directoryIndex] == nullptr) {
        cout << "Page Fault: Allocating new Page Table" << endl;
        directory.tables[parts.directoryIndex] = new PageTable;
    }

    PageTable* pageTable = directory.tables[parts.directoryIndex];

    if (pageTable->frames[parts.tableIndex] == -1) {
        cout << "Page Fault: Allocating new Physical Frame" << endl;
        pageTable->frames[parts.tableIndex] = next_free_frame;
        next_free_frame++;
    } else {
        cout << "Page already mapped. Using existing Physical Frame." << endl;
    }

    uint32_t frameNumber = static_cast<uint32_t>(pageTable->frames[parts.tableIndex]);
    uint32_t physicalAddress = (frameNumber << PAGE_OFFSET_BITS) | parts.offset;

    cout << "Physical Frame Number: " << frameNumber << endl;
    cout << "Physical Address: 0x"
         << hex << uppercase << setw(8) << setfill('0')
         << physicalAddress << dec << endl;

    return physicalAddress;
}

int main() {
    PageDirectory directory;

    uint32_t addresses[] = {
        0x00000000,
        0x00000004,
        0x00400000,
        0xFFFFF000
    };

    int addressCount = sizeof(addresses) / sizeof(addresses[0]);

    cout << "Two-Level Paging Simulation" << endl;

    for (int i = 0; i < addressCount; i++) {
        translate(addresses[i], directory);
    }

    int allocatedPageTables = 0;

    for (int i = 0; i < TABLE_SIZE; i++) {
        if (directory.tables[i] != nullptr) {
            allocatedPageTables++;
        }
    }

    size_t pageTableMemory = static_cast<size_t>(allocatedPageTables) * sizeof(PageTable);
    size_t directoryMemory = sizeof(directory);
    size_t totalPagingMemory = pageTableMemory + directoryMemory;

    cout << "\nExecution Analysis" << endl;
    cout << "Allocated Page Tables: " << allocatedPageTables << endl;
    cout << "Allocated Physical Frames: " << next_free_frame << endl;
    cout << "Memory used by Page Tables: " << pageTableMemory << " bytes" << endl;
    cout << "Memory used by Page Directory: " << directoryMemory << " bytes" << endl;
    cout << "Total memory used by paging structures: "
         << totalPagingMemory << " bytes" << endl;
    cout << "Single-level page table memory: 4194304 bytes" << endl;

    return 0;
}
