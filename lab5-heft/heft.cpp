#include <iostream>
#include <vector>
#include <fstream>

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
    int outdegree;
    float rank;
    int processor_exec;
    int st;
    int ft;
    vector<Processor> processors;
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
        }
    }
}

int main(int argc, char *argv[]) {
    initializeData();
    
    // for (int i = 0; i < job_count; i++)
    // {
    //     for (int j = 0; j < job_count; j++)
    //     {
    //         cout << communication_cost_dag[i][j] << " ";
    //     }
    //     cout << endl;
    // }

    return 0;
}