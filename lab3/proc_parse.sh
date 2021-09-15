#!/bin/sh

if [ $# -eq 0 ] # if no args
then
    grep "model name" -m 1 /proc/cpuinfo
    echo "kernel version: " `uname -r`
    grep "MemTotal" -m 1 /proc/meminfo
    echo "system uptime since boot: " `cut -d " " -f 1 /proc/uptime` " seconds" # 1st number fom /proc/uptime
    exit
fi

if [ $(($2%$1)) -ne 0 ]
then
    echo Enter printout_rate as a whole multiple of read_rate
    exit
fi

# required lated for calulating average
user_perc=0
kernel_perc=0
idle_perc=0
available_mem_perc=0
sectors_read_delta=0
sectors_written_delta=0
context_switch_delta=0
processes_delta=0
# number of samples availabe to average out of
samples=$(echo $2/$1 | bc -l)
i=$1 # timer in seconds

# first read to calulate delta
total_time_start=$(grep "cpu" -m 1 /proc/stat)
total_time_start=${total_time_start[@]:5} # set variable to itself with an offset of 5 characters
total_time_start=$((${total_time_start// /+})) # replace all spaces with +
user_time_start=`awk 'NR==1 {print $2}' /proc/stat` # read 2nd arg from line 1
kernel_time_start=`awk 'NR==1 {print $4}' /proc/stat`
idle_time_start=`awk 'NR==1 {print $5}' /proc/stat`
sectors_read_start=$(awk 'NR==1 {print $6}' /proc/diskstats)
sectors_written_start=$(awk 'NR==1 {print $10}' /proc/diskstats)
context_switch_start=$(grep "ctxt" -m 1 /proc/stat | cut -d " " -f 2)
processes_start=$(grep "processes" -m 1 /proc/stat | cut -d " " -f 2)

sleep $1

while true
do
    # read cpustats
    total_time=$(grep "cpu" -m 1 /proc/stat)
    total_time=${total_time[@]:5}
    total_time=$((${total_time// /+}))
    user_time=`awk 'NR==1 {print $2}' /proc/stat`
    kernel_time=`awk 'NR==1 {print $4}' /proc/stat`
    idle_time=`awk 'NR==1 {print $5}' /proc/stat`
    # read meminfo
    total_mem=$(awk 'NR==1 {print $2}' /proc/meminfo)
    available_mem=$(awk 'NR==3 {print $2}' /proc/meminfo)
    # read diskstats
    sectors_read=$(awk 'NR==1 {print $6}' /proc/diskstats)
    sectors_written=$(awk 'NR==1 {print $10}' /proc/diskstats)
    # read context switches
    context_switch=$(grep "ctxt" -m 1 /proc/stat | cut -d " " -f 2)
    # read processes
    processes=$(grep "processes" -m 1 /proc/stat | cut -d " " -f 2)
    
    # cpu time diff
    total_delta=$(($total_time-$total_time_start))
    user_delta=$(($user_time-$user_time_start))
    kernel_delta=$(($kernel_time-$kernel_time_start))
    idle_delta=$(($idle_time-$idle_time_start))
    # disk time diff
    sectors_read_delta=$(($sectors_read_delta+$sectors_read-$sectors_read_start))
    sectors_written_delta=$(($sectors_written_delta+$sectors_written-$sectors_written_start))
    # context switch time diff
    context_switch_delta=$(($context_switch_delta+$context_switch-$context_switch_start))
    # processes time diff
    processes_delta=$(($processes_delta+$processes-$processes_start))
    
    # cpu percentage
    user_perc=$(echo $user_delta/$total_delta*100+$user_perc | bc -l)
    kernel_perc=$(echo $kernel_delta/$total_delta*100+$kernel_perc | bc -l)
    idle_perc=$(echo $idle_delta/$total_delta*100+$idle_perc | bc -l)
    # memory percentage
    available_mem_perc=$(echo $available_mem/$total_mem*100+$available_mem_perc | bc -l)
    
    # reset start times
    total_time_start=$total_time
    user_time_start=$user_time
    kernel_time_start=$kernel_time
    idle_time_start=$idle_time
    sectors_read_start=$sectors_read
    sectors_written_start=$sectors_written
    context_switch_start=$context_switch
    processes_start=$processes
    
    # when its time to print
    if [ $i -ge $2 ]
    then
        i=0
        echo -n "Percentage CPU in user mode  : "; echo $user_perc/$samples | bc -l
        echo -n "Percentage CPU in kernel mode: "; echo $kernel_perc/$samples | bc -l
        echo -n "Percentage CPU in idle mode  : "; echo $idle_perc/$samples | bc -l
        echo -n "Amount of memory available   : "; echo $available_mem kB
        echo -n "Percentage memory available  : "; echo $available_mem_perc/$samples | bc -l
        echo -n "Sectors read per second      : "; echo $sectors_read_delta/$samples | bc -l
        echo -n "Sectors written per second   : "; echo $sectors_written_delta/$samples | bc -l
        echo -n "Context switches per second  : "; echo $context_switch_delta/$samples | bc -l
        echo -n "Processes born per second    : "; echo $processes_delta/$samples | bc -l
        echo --------------------------------------------------------------------
        
        # reset average calculators
        user_perc=0
        kernel_perc=0
        idle_perc=0
        available_mem_perc=0
        sectors_read_delta=0
        sectors_written_delta=0
        context_switch_delta=0
        processes_delta=0
    fi
    
    sleep $1
    i=$(($i+$1))
done