#include <iostream>
#include <vector>
#include <fstream>
#include <iomanip>
#include <numeric>
#include <algorithm>

using namespace std;

int job_count;
int processor_count;
vector<vector<int>> communication_cost_dag;

class Processor {
public:
    int computation_cost;
    int est;
    int eft;
};

class Job {
public:
    float computaion_avg;
    float rank;
    int processor_exec;
    int st;
    int ft;
    vector<Processor> processors;
    vector<int> parents;
};

vector<Job> jobs;

// initializes all the data from input.txt
void initializeData() {
    ifstream num("input.txt");
    int temp;

    // read number of jobs and processors
    num >> job_count;
    num >> processor_count;
    // fill the vectors with empty data
    for (int i = 0; i < job_count; i++) {
        Job *jb = new Job;
        jobs.push_back(*jb);
        for (int j = 0; j < processor_count; j++) {
            Processor *p = new Processor;
            jobs[i].processors.push_back(*p);
        }
    }

    // fill the computation costs of all jobs on all processors
    for (int i = 0; i < processor_count; i++) {
        for (int j = 0; j < job_count; j++) {
            num >> temp;
            jobs[j].processors[i].computation_cost = temp;
        }
    }
    
    // fill the communications directed acyclic graph adjacency matrix
    for (int i = 0; i < job_count; i++) {
        vector<int> v(job_count);
        communication_cost_dag.push_back(v);
        for (int j = 0; j < job_count; j++) {
            num >> temp;
            communication_cost_dag[i][j] = temp;
            if (communication_cost_dag[i][j] > 0) {
                jobs[j].parents.push_back(i);
            }
        }
    }
}
 
// calculate average computation costs of all jobs
void computationAvgCalculate() {
    for (int i = 0; i < job_count; i++) {
        jobs[i].computaion_avg = 0;
        for (int j = 0; j < processor_count; j++) {
            jobs[i].computaion_avg += jobs[i].processors[j].computation_cost;
        }
        jobs[i].computaion_avg = jobs[i].computaion_avg / processor_count;
    }
}

// for every node, recursively find the upper rank
float rankCalculate(int node) {
    float sub_rank = 0; // rank of the dependent task
    float temp_max = 0; // temp_max = maximum of(cost of communication between jobs + rank(i))

    for (int i = 0; i < job_count; i++) { // for every node
        if (communication_cost_dag[node][i] > 0) { // that has an incoming edge from main node
            sub_rank = rankCalculate(i); // calculate its upper rank
            if (sub_rank + communication_cost_dag[node][i] > temp_max) { // if the upper rank is max among others
                temp_max = sub_rank + communication_cost_dag[node][i]; // set this as the max
            }
        }
    }
    
    return temp_max + jobs[node].computaion_avg; // rank of the main node
}

// sort the jobs w.r.t upper ranks and return in a vector
vector<int> sortRank() {
    vector<int> index(job_count);
    iota(index.begin(), index.end(), 0);
    sort(index.begin(), index.end(), [](int a, int b) -> bool {
        return jobs[a].rank > jobs[b].rank;
    });
    
    return index;
}

// calculate earliest start and finish times of all jobs
void schedule(vector<int> rank_index_sorted) {
    vector<int> processor_available(processor_count, 0); // time at which the processor will be availabe, initially 0
    int processor_current = -1; // the processsor that was used most recently, useful for communication cost calculation
    int parent_job_delay = 0; // delay added to processor_available when a job is dependent on its parent and has to be executed only after it finishes
    int parent_node = -1;
    int parent_processor = -1;

    // first job schedule
    processor_current = min_element(jobs[0].processors.begin(), jobs[0].processors.end(), [](Processor &a, Processor &b) -> bool {
        return a.computation_cost < b.computation_cost;
    }) - jobs[0].processors.begin(); // min_element returns reference to the min element in vector, substracting it with reference of first element gives the index of minimum element
    jobs[0].st = 0;
    jobs[0].ft = jobs[0].processors[processor_current].computation_cost;
    processor_available[processor_current] = jobs[0].ft;
    jobs[0].processor_exec = processor_current;
    for (int i = 0; i < processor_count; i++) {
        jobs[0].processors[i].est = 0;
        jobs[0].processors[i].eft = jobs[0].processors[i].computation_cost;
    }

    // scheduling rest of the jobs
    for (int i = 1; i < job_count; i++) {
        int ii = rank_index_sorted[i]; // for making the code simpler
        if (communication_cost_dag[rank_index_sorted[i - 1]][ii] > 0) {
            parent_job_delay = jobs[rank_index_sorted[i - 1]].ft;
            parent_node = rank_index_sorted[i - 1];
            parent_processor = processor_current;
        }
        for (int j = 0; j < processor_count; j++) {
            if (j == parent_processor)
                jobs[ii].processors[j].est = max(parent_job_delay, processor_available[j]);
            else
                jobs[ii].processors[j].est = max(parent_job_delay + communication_cost_dag[rank_index_sorted[parent_node]][ii], processor_available[j]);
            jobs[ii].processors[j].eft = jobs[ii].processors[j].est + jobs[ii].processors[j].computation_cost;
        }
        // pick processor with lowest EFT
        processor_current = min_element(jobs[ii].processors.begin(), jobs[ii].processors.end(), [](Processor &a, Processor &b) -> bool {
            return a.eft < b.eft;
        }) - jobs[ii].processors.begin(); // min_element returns reference to the min element in vector, substracting it with reference of first element gives the index of minimum element
        jobs[ii].st = jobs[ii].processors[processor_current].est;
        jobs[ii].ft = jobs[ii].processors[processor_current].eft;
        jobs[ii].processor_exec = processor_current;
        processor_available[processor_current] = jobs[ii].ft;
    }
}

int main(int argc, char *argv[]) {
    initializeData();

    cout << "No. of tasks:" << job_count << endl;
    cout << "No. of processors:" << processor_count << endl;
    cout << "\nThe upward rank values:" << endl;

    computationAvgCalculate();
    for (int i = 0; i < job_count; i++) {
        jobs[i].rank = rankCalculate(i);
        cout << "Task " << i + 1 << ": " <<  fixed << setprecision(6) << jobs[i].rank << endl;
    }
    vector<int> rank_index_sorted = sortRank();
    
    cout << "\nThe order of tasks to be scheduled:" << endl;
    
    for (int i = 0; i < job_count; i++) {
        cout << rank_index_sorted[i] + 1 << " ";
    }
    cout << "\n\nEST and EFT on different processors" << endl;

    schedule(rank_index_sorted);
    
    for (int i = 0; i < job_count; i++) {
        cout << "Task: " << i + 1 << endl;
        for (int j = 0; j < processor_count; j++) {
            cout << "processor " << j + 1 << "||est: " << jobs[i].processors[j].est << " eft: " << jobs[i].processors[j].eft << " ||" << endl;
        }
        cout << endl;
    }
    

    return 0;
}