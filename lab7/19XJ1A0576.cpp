#include <iostream>
#include <cstdlib>
#include <vector>
#include <ctime>

#define LOOPS 10000
#define EQULIBRIUM_TIME 9000
#define MEM_SIZE 1048576 // 1MB

using namespace std;

class Process {
    public:
    int start = -1; // starting location in the memory
    int size = -1; // size of the process
};

// keeps track of all free memory spaces in the memory
class MemoryHole {
    public:
    int start;
    int size;
};

vector<MemoryHole> memory_hole(0); // all free memory spaces aka holes
vector<Process> process_list(0); // list of all processes

// creates new processes and returns its size
int processSizeGenerator(int mode) {
    switch (mode) {
    case 1:
        return rand() % 1024 + 1; // generates random number between 1 and 1024 both inclusive
        break;
    case 2:
        return rand() % 100 + 1; // generates random number between 1 and 100 both inclusive
        break;
    case 3:
        return rand() % 501 + 500; // generates random number between 500 and 1000 both inclusive
        break;
    default:
        return 0;
        break;
    }
}

// inserts processes in process_list in ascending order
void processInsert(int process_size, int process_start) {
    printf("%d %d\n", process_start, process_size);
    Process *temp_process = new Process;
    temp_process->size = process_size;
    temp_process->start = process_start;

    if (process_list.size() == 0) {
        process_list.push_back(*temp_process);
        delete temp_process;
        return;
    }

    for (int i = 0; i < process_list.size(); i++) { // sort by start
        if (process_start < process_list[i].start) {
            process_list.insert(process_list.begin() + i, *temp_process);
            delete temp_process;
            return;
        }
    }

    process_list.push_back(*temp_process); // insert largest value at the end
    delete temp_process;
}

// adds process to the memory by removing/resizing holes and returns starting position of the process
int processAdd(int mode, int process_size) {
    int temp1 = MEM_SIZE;
    int temp2 = -1;

    switch (mode) {
    case 1: // best fit
        for (int i = 0; i < memory_hole.size(); i++) { // go through all holes
            if (memory_hole[i].size > process_size && memory_hole[i].size - process_size < temp1) { // if hole is better fitting than last one
                temp1 = memory_hole[i].size - process_size;
                temp2 = i;
            }
            else if (memory_hole[i].size == process_size) { // best fit hole
                temp1 = memory_hole[i].start;
                memory_hole.erase(memory_hole.begin() + i);
                return temp1;
            }
        }

        temp1 = memory_hole[temp2].start;
        memory_hole[temp2].start += process_size;
        memory_hole[temp2].size -= process_size;
        return temp1;
        break;

    case 2: // worst fit
        for (int i = 0; i < memory_hole.size(); i++) { // go through all holes
            if (memory_hole[i].size >= process_size && memory_hole[i].size - process_size > temp2) { // if hole is worst fitting than last one
                temp2 = memory_hole[i].size - process_size;
                temp1 = i;
            }
        }

        if (memory_hole[temp1].size == process_size) {
            temp2 = memory_hole[temp1].start;
            memory_hole.erase(memory_hole.begin() + temp1);
            return temp2;
        }

        temp2 = memory_hole[temp1].start;
        memory_hole[temp1].start += process_size;
        memory_hole[temp1].size -= process_size;
        return temp2;
        break;

    case 3: // first fit
        for (int i = 0; i < memory_hole.size(); i++) { // go through all holes
            if (memory_hole[i].size > process_size) { // if the hole is big enough, shrink it
                temp1 = memory_hole[i].start;
                memory_hole[i].start += process_size;
                memory_hole[i].size -= process_size;
                return temp1;
            }
            else if (memory_hole[i].size == process_size) { // if the hole is just big enough, remove it
                temp1 = memory_hole[i].start;
                memory_hole.erase(memory_hole.begin() + i);
                return temp1;
            }
        }
        break;

    default:
        break;
    }
    return -1;
}

// determines whether the new process can fit in memory
bool processCanFit(int process_size) {
    for (int i = 0; i < memory_hole.size(); i++) {
        if (process_size <= memory_hole[i].size) {
            return true;
        }
    }

    return false;
}

// inserts holes in memory_hole in ascending order
void holeInsert(int hole_size, int hole_start) {
    MemoryHole *temp_hole = new MemoryHole;
    temp_hole->start = hole_start;
    temp_hole->size = hole_size;

    if (memory_hole.size() == 0) {
        memory_hole.push_back(*temp_hole);
        delete temp_hole;
        return;
    }

    for (int i = 0; i < memory_hole.size(); i++) { // sort by start
        if (hole_start < memory_hole[i].start) {
            memory_hole.insert(memory_hole.begin() + i, *temp_hole);
            delete temp_hole;
            return;
        }
    }

    memory_hole.push_back(*temp_hole);
    delete temp_hole;
}

// removes a process from memory randomly
void processRemove() {
    int r = rand() % process_list.size(); // select a process at random
    bool touching_hole = false;

    for (int i = 0; i < memory_hole.size(); i++) {
        if (process_list[r].start == memory_hole[i].start + memory_hole[i].size) { // if there is a hole above the process
            touching_hole = true;
            if (process_list[r].start + process_list[r].size == memory_hole[i + 1].start) { // if there is a hole below also
                memory_hole[i].size += process_list[r].size + memory_hole[i + 1].size; // expand the upper hole till bottom hole ends
                memory_hole.erase(memory_hole.begin() + i + 1); // remove the bottom hole
            }
            else { // if there is no hole below the process
                memory_hole[i].size += process_list[r].size; // expand the hole till process ends
            }
        }

        else if (process_list[r].start + process_list[r].size == memory_hole[i].start) { // if there is a hole below the process
            touching_hole = true;
            memory_hole[i].start = process_list[r].start; // hole starts from where process started
            memory_hole[i].size += process_list[r].size; // hole size is increased
        }
    }
    if (!touching_hole) {
        printf("removing process\n");
        holeInsert(process_list[r].size, process_list[r].start);
    }
    process_list.erase(process_list.begin() + r); // remove the process
}

int main(int argc, char **argv) {
    srand(time(0)); // generate random seed. this changes every second

    holeInsert(MEM_SIZE, 0); // initialize empty memory as the first hole

    for (int i = 0; i < LOOPS; i++) { // run the simulation LOOPS number of times
        int process_size = processSizeGenerator(atoi(argv[1])); // generate a random process size
        while (!processCanFit(process_size)) { // check if the process can fit in existing holes, continue until it can
            processRemove(); // remove a process randolmly
        }
        int process_start = processAdd(atoi(argv[2]), process_size); // find out which hole to add the process to
        processInsert(process_size, process_start); // add the process to process_list
    }

    /* for (int i = 0; i < process_list.size(); i++) { */
    /*     printf("%d %d\n", process_list[i].start, process_list[i].size); */
    /* } */

    return 0;
}
