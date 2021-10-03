algo = int(input()) # read which algorithm
proc_count = int(input()) # read no of processes
arrival_time = input().split()
arrival_time = [ int(i) for i in arrival_time ] # arrival times
burst_time = input().split()
burst_time = [ int(i) for i in burst_time ] # burst times

# initialize frequently used objects
ready_queue = [0] * proc_count
completion_time = [-1] * proc_count
start_time = [-1] * proc_count
response_time = [0] * proc_count
waiting_time = [0] * proc_count
turnaround_time = [0] * proc_count
remaining_time = burst_time[:] # [:] is required to copy the list otherwise it references the same list

# some error handling
if (algo == 4):
    nice_value = input().split()
    nice_value = [ int(i) for i in nice_value ]
    if (len(nice_value) != proc_count):
        print("Invalid length")
        exit(1)
elif (algo == 5):
    time_quantum = int(input())
if (len(arrival_time) != proc_count or len(burst_time) != proc_count):
    print("Invalid length")
    exit(1)

# FCFS
if (algo == 1):
    ready_queue = sorted(range(proc_count), key = lambda x: arrival_time[x]) # first sort based on arrival times
    t = 0
    while len(ready_queue) > 0:
        if t >= arrival_time[ready_queue[0]]: # when there are items to schedule
            p = ready_queue.pop(0) # p is now the running process
            start_time[p] = t
            completion_time[p] = start_time[p] + burst_time[p]
            response_time[p] = start_time[p] - arrival_time[p]
            turnaround_time[p] = completion_time[p] - arrival_time[p]
            waiting_time[p] = turnaround_time[p] - burst_time[p]
            t = completion_time[p] - 1 # skip to the time when p completes
        t = t + 1

# SJF
elif (algo == 2):
    t = 0
    ready_queue = sorted(range(proc_count), key=lambda x: arrival_time[x])
    while len(ready_queue) > 0:
        if t >= arrival_time[ready_queue[0]]:
            count = 0
            for i in ready_queue: # check how many processes are ready to be queued
                if t >= arrival_time[i]:
                    count = count + 1
            ready_queue[:count] = sorted(ready_queue[:count], key=lambda x: burst_time[x]) # sort available processes based on burst times
            p = ready_queue.pop(0)
            start_time[p] = t
            completion_time[p] = start_time[p] + burst_time[p]
            response_time[p] = start_time[p] - arrival_time[p]
            turnaround_time[p] = completion_time[p] - arrival_time[p]
            waiting_time[p] = turnaround_time[p] - burst_time[p]
            t = completion_time[p] - 1
        t = t + 1

# SRTF
elif (algo == 3):
    t = 0
    p = 0
    ready_queue = sorted(range(proc_count), key=lambda x: arrival_time[x])
    while len(ready_queue) > 0:
        count = 0
        for i in ready_queue:
            if t >= arrival_time[i]:
                count = count + 1
        if t in arrival_time or t == completion_time[p] and count != 0: # only preempt when something new occurs
            if t < completion_time[p]: # save the state of preemted process and add it back to ready queue
                ready_queue.insert(0, p)
                remaining_time[p] = completion_time[p] - t
            count = 0
            for i in ready_queue:
                if t >= arrival_time[i]:
                    count = count + 1
            ready_queue[:count] = sorted(ready_queue[:count], key=lambda x: remaining_time[x]) # sort ready processes based on remaining burst time
            p = ready_queue.pop(0)
            if start_time[p] == -1: # only if being processed 1st time
                start_time[p] = t
            completion_time[p] = t + remaining_time[p]
            response_time[p] = start_time[p] - arrival_time[p]
            turnaround_time[p] = completion_time[p] - arrival_time[p]
            waiting_time[p] = turnaround_time[p] - burst_time[p]
        t = t + 1
        
# priority preemprive scheduling
# same as SRTF but second sorting is based on priority rather than remaining time
elif (algo == 4):
    t = 0
    p = 0
    ready_queue = sorted(range(proc_count), key=lambda x: arrival_time[x])
    while len(ready_queue) > 0:
        count = 0
        for i in ready_queue:
            if t >= arrival_time[i]:
                count = count + 1
        if t in arrival_time or t == completion_time[p] and count != 0:
            if t < completion_time[p]:
                ready_queue.insert(0, p)
                remaining_time[p] = completion_time[p] - t
            count = 0
            for i in ready_queue:
                if t >= arrival_time[i]:
                    count = count + 1
            ready_queue[:count] = sorted(ready_queue[:count], key=lambda x: nice_value[x])
            p = ready_queue.pop(0)
            if start_time[p] == -1:
                start_time[p] = t
            completion_time[p] = t + remaining_time[p]
            response_time[p] = start_time[p] - arrival_time[p]
            turnaround_time[p] = completion_time[p] - arrival_time[p]
            waiting_time[p] = turnaround_time[p] - burst_time[p]
        t = t + 1        
        
# RR
elif algo == 5:
    ready_queue = [] # ready queue concept is used differnetly than previous ones
    t = min(arrival_time)
    p = 0
    timer = time_quantum
    while len(ready_queue) > 0 or t <= max(arrival_time):
        for i, at in enumerate(arrival_time): # as soon as a process arrives, append it to ready queue
            if t == at:
                ready_queue.append(i)

        if (t >= completion_time[p] or timer >= time_quantum) and len(ready_queue) > 0: # preemnt when time quantum expires or process burst time ends
            if t < completion_time[p]: # add preempted process at the end on queue
                ready_queue.append(p)
                remaining_time[p] = completion_time[p] - t
            p = ready_queue.pop(0)
            if start_time[p] == -1:
                start_time[p] = t
            completion_time[p] = t + remaining_time[p]
            response_time[p] = start_time[p] - arrival_time[p]
            turnaround_time[p] = completion_time[p] - arrival_time[p]
            waiting_time[p] = turnaround_time[p] - burst_time[p]
            timer = 0 # reset the time quantum timer
        t = t + 1
        timer = timer + 1

throughput = float(proc_count / max(completion_time))

print(*waiting_time, sep=' ')
print(*response_time, sep=' ')
print(*turnaround_time, sep=' ')
print("%.4f"%throughput)