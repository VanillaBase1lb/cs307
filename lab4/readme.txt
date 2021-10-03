Dependencies:
    python3 (>= 3.6)

Note:
    Tested on kernel 5.14.8 and python3.9.7
    Program assumes input will be given from stdin as mentioned in assignment pdf
    
Input format:
    The first line of the input represents the scheduling algorithm
    a. 1 for FCFS
    b. 2 for shortest-job first
    c. 3 for shortest remaining time first
    d. 4 for priority scheduling
    e. 5 for round-robin scheduling
    The second line of the input represents the number of processes
    The third line of the input contains the arrival time of the processes
    The fourth line represents the CPU burst times of the processes
    In the case of priority scheduling, the fifth line represents the nice value of each process.
    Lesser the nice value, the higher the priority of the process
    In the case of round-robin scheduling, the fifth line contains the time quantum.

Instructions:
    "python3 19XJ1A0576.py"
    Input format given above
    
Output format:
    The first line of the output is the waiting times of the processes separated by
    space.
    The second line of the output is the response times of the processes separated
    by space.
    The third line of the output is the turnaround times of the processes separated
    by space.
    The fourth line of the output is the throughput rounded upto 4 decimal places.