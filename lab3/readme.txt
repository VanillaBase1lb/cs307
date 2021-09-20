Dependencies:
    shell - /bin/sh
    basic GNU Linux utilities(grep, cut, ...)
    bc - basic calculator
    awk
    
Note:
    Tested on kernel 5.14.5
    Program assumes the presence of disk named "sda"
    printout_rate must be a whole multiple of read_rate
    
Instructions:
    "cd" into the directory
    "sudo chmod a+x 19XJ1A0576.sh"
    "./19XJ1A0576.sh" for mode 1
    "./19XJ1A0576.sh <read_rate> <printout_rate>" for mode 2
    
Output format(no args):

model name      : [processor type]
kernel version:  [kernel version]
MemTotal:       [total memory configured in kB] kB
system uptime since boot:  [amount of time since the system was last booted in seconds]  seconds

Output format(2 args)(1 time per <printout_rate>):

Percentage CPU in user mode  : [Percentage CPU time spent in user mode]
Percentage CPU in kernel mode: [Percentage CPU time spent in kernel mode]
Percentage CPU in idle mode  : [Percentage CPU time spent in idle mode]
Amount of memory available   : [amount of availabe memory in kB] kB
Percentage memory available  : [percentage of availabe memory]
Sectors read per second      : [sectors read per second]
Sectors written per second   : [sectors written per second]
Context switches per second  : [number of context switches]
Processes born per second    : [number of processes born]
--------------------------------------------------------------------