#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <numeric>
#include <algorithm>

using namespace std;

vector<vector<int> > max_resources;
vector<vector<int> > allocated_resources;
vector<int> total_resources;
vector<vector<int> > need_resources;
vector<vector<int> > safe_sequence_all;

int resource_count;
int process_count;

void initializeData() {
    ifstream num("input.txt");
    int temp;

    // read number of resources and processes
    num >> resource_count;
    num >> process_count;
    
    // initialize total resources
    for (int i = 0; i < resource_count; i++) {
        num >> temp;
        total_resources.push_back(temp);
    }

    // initialize the 2D vectors now that we have the dimensions
    max_resources.resize(process_count, vector<int>(resource_count, -1));
    allocated_resources.resize(process_count, vector<int>(resource_count, -1));
    need_resources.resize(process_count, vector<int>(resource_count, -1));

    // fill the max resources matrix
    for (int i = 0; i < process_count; i++) {
        for (int j = 0; j < resource_count; j++) {
            num >> max_resources[i][j];
        }
    }

    // fill the allocated resources matrix
    for (int i = 0; i < process_count; i++) {
        for (int j = 0; j < resource_count; j++) {
            num >> allocated_resources[i][j];
        }
    }

    // fill the need resources matrix
    for (int i = 0; i < process_count; i++) {
        for (int j = 0; j < resource_count; j++) {
            need_resources[i][j] = max_resources[i][j] - allocated_resources[i][j];
        }
    }
}

bool canFinish(int process, vector<int> available_resources) {
    for (int i = 0; i < resource_count; i++) {
        if (need_resources[process][i] > available_resources[i])
            return false;
    }

    return true;
}

void calculateSafeSequence(vector<int> available_resources, vector<int> safe_sequence, vector<bool> finished) {
    for (int i = 0; i < process_count; i++) {
        if (!finished[i] && canFinish(i, available_resources)) {
            // update available resources since process finished
            for (int j = 0; j < resource_count; j++)
            {
                available_resources[j] += allocated_resources[i][j];
            }
            finished[i] = true;
            
            // recursively call the function to print lexicographically
            safe_sequence.push_back(i);
            calculateSafeSequence(available_resources, safe_sequence, finished);
            safe_sequence.pop_back();
            
            finished[i] = false;
            for (int j = 0; j < resource_count; j++)
            {
                available_resources[j] -= allocated_resources[i][j];
            }
        }
    }
    
    if (safe_sequence.size() == process_count) {
        safe_sequence_all.push_back(safe_sequence);
    }

}

int main(int argc, char *argv[]) {
    initializeData();
    
    vector<int> available_resources(resource_count);
    for (int i = 0; i < resource_count; i++) {
        int used = 0;
        for (int j = 0; j < process_count; j++) {
            used += allocated_resources[j][i];
        }
        available_resources[i] = total_resources[i] - used;
    }
    
    vector<int> safe_sequence;
    vector<bool> finished(process_count, false);
    calculateSafeSequence(available_resources, safe_sequence, finished);
    
    cout << safe_sequence_all.size() << endl;
    for (int i = 0; i < safe_sequence_all.size(); i++) {
        for (int j = 0; j < process_count; j++) {
            cout << safe_sequence_all[i][j] << " ";
        }
        cout << endl;
    }

    return 0;
}