Dependencies:
    shell - /bin/sh
    basic GNU Linux utilities(grep, cut, ...)
    bc - basic calculator
    awk
    
Instructions:
    "cd" into the directory
    "sudo chmod a+x 19XJ1A0576.sh"
    "./19XJ1A0576.sh" for mode 1
    "./19XJ1A0576.sh <read_rate> <printout_rate>" for mode 2
    printout_rate must be a whole multiple of read_rate
    
Output format(no args):

[processor type]
[kernel version]
[total memory configured in kB]
[amount of time since the system was last booted in secondss]

Output format(2 args)(1 time per <printout_rate>):

[Percentage CPU time spent in user mode]
[Percentage CPU time spent in kernel mode]
[Percentage CPU time spent in idle mode]
[amount of availabe memory in kB]
[percentage of availabe memory]
[sectors read per second]
[sectors written per second]
[number of context switches]
[number of processes born]
--------------------------------------------------------------------