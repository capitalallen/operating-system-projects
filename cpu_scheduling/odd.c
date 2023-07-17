#include "oslabs.h"
#include <stdlib.h>

struct PCB removeItem(struct PCB ready_queue[QUEUEMAX], int i, int *queue_cnt);


int isPCBEmpty(struct PCB process) {
    return process.process_id == 0 && process.arrival_timestamp == 0 && process.total_bursttime == 0 && process.execution_starttime == 0 && process.execution_endtime == 0 && process.process_priority == 0 && process.remaining_bursttime == 0;
}

void enqueue(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB process) {
    if(*queue_cnt == QUEUEMAX) return;
    ready_queue[*queue_cnt] = process;
    *queue_cnt += 1;
}

struct PCB getTopPriority(struct PCB ready_queue[QUEUEMAX], int *queue_cnt) {
    if(*queue_cnt == 0) {
        struct PCB null_pcb = {0,0,0,0,0,0,0};
        return null_pcb;
    }
    int minIndex = 0;
    for(int i = 1; i < *queue_cnt; i++) {
        struct PCB now = ready_queue[i];
        if(now.process_priority < ready_queue[minIndex].process_priority)
            minIndex = i;
    }
    *queue_cnt -= 1;
    
    return removeItem(ready_queue, minIndex, queue_cnt);
}

struct PCB getLowestBurstTime(struct PCB ready_queue[QUEUEMAX], int *queue_cnt) {
    if(*queue_cnt == 0) {
        struct PCB null_pcb = {0,0,0,0,0,0,0};
        return null_pcb;
    }
    int minIndex = 0;
    for(int i = 1; i < *queue_cnt; i++) {
        struct PCB now = ready_queue[i];
        if(now.remaining_bursttime < ready_queue[minIndex].remaining_bursttime)
            minIndex = i;
    }
    *queue_cnt -= 1;
    return removeItem(ready_queue, minIndex, queue_cnt);
}

struct PCB getEarlistTime(struct PCB ready_queue[QUEUEMAX], int *queue_cnt) {
    if(*queue_cnt == 0) {
        struct PCB null_pcb = {0,0,0,0,0,0,0};
        return null_pcb;
    }
    int minIndex = 0;
    for(int i = 1; i < *queue_cnt; i++) {
        struct PCB now = ready_queue[i];
        if(now.arrival_timestamp < ready_queue[minIndex].arrival_timestamp)
            minIndex = i;
    }
    *queue_cnt -= 1;
    return removeItem(ready_queue, minIndex, queue_cnt);
}

struct PCB removeItem(struct PCB ready_queue[QUEUEMAX], int i, int *queue_cnt) {
    //copy
    struct PCB p;
    p.process_id = ready_queue[i].process_id;
    p.arrival_timestamp = ready_queue[i].arrival_timestamp;
    p.total_bursttime = ready_queue[i].total_bursttime;
    p.execution_starttime = ready_queue[i].execution_starttime;
    p.execution_endtime = ready_queue[i].execution_endtime;
    p.remaining_bursttime = ready_queue[i].remaining_bursttime;
    p.process_priority = ready_queue[i].process_priority;
    //remove
    for(int j = i + 1; j < *queue_cnt + 1; j++) {
        ready_queue[j - 1].process_id = ready_queue[j].process_id;
        ready_queue[j - 1].arrival_timestamp = ready_queue[j].arrival_timestamp;
        ready_queue[j - 1].total_bursttime = ready_queue[j].total_bursttime;
        ready_queue[j - 1].execution_starttime = ready_queue[j].execution_starttime;
        ready_queue[j - 1].execution_endtime = ready_queue[j].execution_endtime;
        ready_queue[j - 1].remaining_bursttime = ready_queue[j].remaining_bursttime;
        ready_queue[j - 1].process_priority = ready_queue[j].process_priority;
    }
    return p;
}

/**
 *  handle the completion of execution of a process in a Priority-based Preemptive Scheduler.
 *  
 *  The method determines the process to execute next and returns its PCB.
*/

struct PCB handle_process_arrival_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp) {
    //if no current process
    if(isPCBEmpty(current_process)) {
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }

    if(current_process.process_priority > new_process.process_priority) {
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        current_process.remaining_bursttime -= (timestamp - current_process.execution_starttime);
        current_process.execution_starttime = 0;
        current_process.execution_endtime = 0;
        enqueue(ready_queue, queue_cnt, current_process);
        
        return new_process;
    }
    else {
        new_process.execution_starttime = 0;
        new_process.execution_endtime = 0;
        new_process.remaining_bursttime = new_process.total_bursttime;
        enqueue(ready_queue, queue_cnt, new_process);

        return current_process;
    }
}

struct PCB handle_process_completion_pp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp) {
    if(queue_cnt == 0) {
        struct PCB null_pcb = {0,0,0,0,0,0,0};
        return null_pcb;
    }
    struct PCB nextProcess = getTopPriority(ready_queue, queue_cnt);
    nextProcess.execution_starttime = timestamp;
    nextProcess.execution_endtime = timestamp + nextProcess.remaining_bursttime;
    return nextProcess;
}



/**
 * This method implements the logic to handle the arrival of a new process in a Round-Robin Scheduler.
 * 
*/
struct PCB handle_process_arrival_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp, int time_quantum) {
    //if current process is not empty 
    if(isPCBEmpty(current_process)) {
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + (new_process.total_bursttime > time_quantum ? time_quantum : new_process.total_bursttime);
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }
    new_process.execution_starttime = 0;
    new_process.execution_endtime = 0;
    new_process.remaining_bursttime = new_process.total_bursttime;
    enqueue(ready_queue, queue_cnt, new_process);

    return current_process;
}

struct PCB handle_process_completion_rr(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp, int time_quantum) {
    if(queue_cnt == 0) {
        struct PCB null_pcb = {0,0,0,0,0,0,0};
        return null_pcb;
    }
    struct PCB nextProcess = getEarlistTime(ready_queue, queue_cnt);
    nextProcess.execution_starttime = timestamp;
    nextProcess.execution_endtime = timestamp + (nextProcess.remaining_bursttime > time_quantum ? time_quantum : nextProcess.remaining_bursttime);
    return nextProcess;
}


/**
 * 
 * This method implements the logic to handle the arrival of a new process in a Shortest-Remaining-Time-Next Preemptive Scheduler.
*/

struct PCB handle_process_arrival_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, struct PCB current_process, struct PCB new_process, int timestamp) {
    //if no current process
    if(isPCBEmpty(current_process)) {
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        return new_process;
    }

    //new_process do not have shorter remain time
    if(current_process.remaining_bursttime <= new_process.total_bursttime) {
        new_process.execution_starttime = 0;
        new_process.execution_endtime = 0;
        new_process.remaining_bursttime = new_process.total_bursttime;
        enqueue(ready_queue, queue_cnt, new_process);
        return current_process;
    }
    else {
        //modify new process info
        new_process.execution_starttime = timestamp;
        new_process.execution_endtime = timestamp + new_process.total_bursttime;
        new_process.remaining_bursttime = new_process.total_bursttime;
        //modify current process info
        current_process.remaining_bursttime -= (timestamp - current_process.execution_starttime);
        current_process.execution_starttime = 0;
        current_process.execution_endtime = 0;
        enqueue(ready_queue, queue_cnt, current_process);
        return new_process;
    }
}

struct PCB handle_process_completion_srtp(struct PCB ready_queue[QUEUEMAX], int *queue_cnt, int timestamp) {
    if(queue_cnt == 0) {
        struct PCB null_pcb = {0,0,0,0,0,0,0};
        return null_pcb;
    }
    struct PCB nextProcess = getLowestBurstTime(ready_queue, queue_cnt);
    nextProcess.execution_starttime = timestamp;
    nextProcess.execution_endtime = timestamp + nextProcess.remaining_bursttime;
    return nextProcess;
}

