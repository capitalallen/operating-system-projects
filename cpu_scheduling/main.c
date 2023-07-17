#include "oslabs.h"
#include <stdlib.h>
#include <stdio.h>


int main(void) {
    struct PCB ready_queue[QUEUEMAX];
    int queue_cnt = 0;
    /**
     * ppScheduler
     * 
    */
    struct PCB current_process = {0,0,0,0,0,0,0};
    int timestamp = 11;

    for(int i = 0; i < 6; i++) {
        int process_priority = i;
        printf("new_priority: %d\n", process_priority);
        struct PCB new_process = {i, timestamp, rand(), 0, 0, 0, process_priority};
        current_process = handle_process_arrival_pp(ready_queue, &queue_cnt, current_process, new_process, timestamp);
        timestamp++;
    }
    while(queue_cnt > 0) {
        current_process = handle_process_completion_pp(ready_queue, &queue_cnt, timestamp);
        printf("out_priority: %d\n", current_process.process_priority);
        
        timestamp++;
    }
}