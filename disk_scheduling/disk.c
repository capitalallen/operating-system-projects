#include <stdio.h>
#include "oslabs.h"
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>

bool is_null_RCB(struct RCB current_request) {
    if ((current_request.address == 0) && (current_request.arrival_timestamp == 0) && (current_request.cylinder == 0) && (current_request.process_id == 0) &&
        (current_request.request_id == 0)) {
        return true;
    } else {
        return false;
    }
}

/*
 * handle_request_arrival_fcfs
 * implements the logic to handle the arrival of a new IO request in a First-Come-First-Served Scheduler.
 * @param request_queue:
 * @param queue_cnt
 * @param current_request
 * @param new_request
 * @param timestamp
 * @returns the RCB of the newly-arriving request if the disk is free (indicated by the third parameter being a NULLRCB),
 * otherwise, it returns the RCB of the currently-serviced request after adding the newly-arriving request to the request queue.
 */
struct RCB handle_request_arrival_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp) {
    if (is_null_RCB(current_request)) {
        return new_request;
    } else {
        if (*queue_cnt <= QUEUEMAX) {
            request_queue[*queue_cnt] = new_request;
            *queue_cnt += 1;
            return current_request;
        } else {
            return current_request;
        }
    }
}

/*
 * This method implements the logic to handle the completion of servicing an IO request in a First-Come-First-Served Scheduler.
 * @param request_queue: the request queue (an array of RCB structs).
 * @param queue_cnt: The number of items in the request queue
 * @returns trequest_queue, queue_cnt, RCB
 */
struct RCB handle_request_completion_fcfs(struct RCB request_queue[QUEUEMAX], int *queue_cnt)
{
    if (*queue_cnt <= 0) {
        struct RCB null_RCB = {0, 0, 0, 0, 0};
        return null_RCB;
    } else {
        struct RCB next_RCB;
        int temp_index = 0;
        int temp_arrival = request_queue[0].arrival_timestamp;
        for (int index = 1; index < *queue_cnt; index++) {
            if (temp_arrival > request_queue[index].arrival_timestamp) {
                temp_arrival = request_queue[index].arrival_timestamp;
                temp_index = index;
            }
        }
        next_RCB = request_queue[temp_index];
        for (int index = temp_index; index < *queue_cnt; index++) {
            request_queue[index] = request_queue[index + 1];
        }
        *queue_cnt -= 1;
        return next_RCB;
    }
}

/*
 * This method implements the logic to handle the arrival of a new IO request in a Shortest-Seek-First
 * (also known as Shortest-Seek-Time-First) Scheduler.
 * @param request_queue: the request queue (an array of RCB structs).
 * @param queue_cnt: The number of items in the request queue
 * @param current_request
 * @param new_request
 * @param timestamp
 * @returns trequest_queue, queue_cnt, RCB
 */
struct RCB handle_request_arrival_sstf(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp)
{
    if (is_null_RCB(current_request)) {
        return new_request;
    }
    else {
        if (*queue_cnt <= QUEUEMAX) {
            request_queue[*queue_cnt] = new_request;
            *queue_cnt += 1;
            return current_request;
        }
        else {
            return current_request;
        }
    }
}

/*
 * This method implements the logic to handle the completion of servicing an IO request in a Shortest-Seek-Time-First Scheduler.
 * @param request_queue: the request queue (an array of RCB structs).
 * @param queue_cnt: The number of items in the request queue
 * @param current_cylinder: the current cylinder.
 * @returns trequest_queue, queue_cnt, RCB
 */
struct RCB handle_request_completion_sstf(struct RCB request_queue[QUEUEMAX], int *queue_cnt, int current_cylinder)
{
    if (*queue_cnt <= 0) {
        struct RCB null_RCB = {0, 0, 0, 0, 0};
        return null_RCB;
    } else {
        struct RCB next_RCB;
        int temp_index = 0, temp_cylinder = 0, temp_arrivel = 0;
        temp_cylinder = abs(current_cylinder - request_queue[0].cylinder);
        temp_arrivel = request_queue[0].arrival_timestamp;
        for (int index = 1; index < *queue_cnt; index++) {
            if (temp_cylinder == abs(current_cylinder - request_queue[index].cylinder)) {
                if (temp_arrivel > request_queue[index].arrival_timestamp) {
                    temp_cylinder = abs(current_cylinder - request_queue[index].cylinder);
                    temp_arrivel = request_queue[index].arrival_timestamp;
                    temp_index = index;
                }
            } else if (temp_cylinder > abs(current_cylinder - request_queue[index].cylinder)) {
                temp_cylinder = abs(current_cylinder - request_queue[index].cylinder);
                temp_arrivel = request_queue[index].arrival_timestamp;
                temp_index = index;
            }
        }
        next_RCB = request_queue[temp_index];
        for (int index = temp_index; index < *queue_cnt; index++) {
            request_queue[index] = request_queue[index + 1];
        }
        *queue_cnt -= 1;
        return next_RCB;
    }
}

struct RCB handle_request_arrival_look(struct RCB request_queue[QUEUEMAX], int *queue_cnt, struct RCB current_request, struct RCB new_request, int timestamp) {
    if (is_null_RCB(current_request)) {
        return new_request;
    } else {
        if (*queue_cnt <= QUEUEMAX) {
            request_queue[*queue_cnt] = new_request;
            *queue_cnt += 1;
            return current_request;
        } else {
            return current_request;
        }
    }
}

/*
 * This method implements the logic to handle the completion of servicing an IO request in a LOOK Scheduler.
 * @param request_queue: the request queue (an array of RCB structs).
 * @param queue_cnt: The number of items in the request queue
 * @param current_cylinder: the current cylinder.
 * @param scan_direction: the scan direction.
 * @returns trequest_queue, queue_cnt, RCB
 */
struct RCB handle_request_completion_look(struct RCB request_queue[QUEUEMAX], int *queue_cnt, int current_cylinder, int scan_direction){
    if (*queue_cnt <= 0){
        struct RCB null_RCB = {0, 0, 0, 0, 0};
        return null_RCB;
    } 
    struct RCB next_RCB;
    int res_index = 0, res_cylinder = 0, res_arrival = 2147483647;
    bool found=false;
    // if there is same cylinder - pick earliest arrival time 
    for (int i = 0; i < *queue_cnt; i++) {
        if (current_cylinder == request_queue[i].cylinder) {
            found=true;
            if (res_arrival>request_queue[i].arrival_timestamp){
                res_index=i; 
                res_arrival=request_queue[i].arrival_timestamp;
            }
        }
    }
    if (found){
        next_RCB = request_queue[res_index];
        for (int i = res_index; i < *queue_cnt; i++) {
            request_queue[i] = request_queue[i + 1];
        }
        *queue_cnt -= 1;
        return next_RCB;
    }
    res_index = 0;
    res_cylinder = 2147483647;
    bool is_larger=false;
    int diff=2147483647;
    for (int i = 0; i < *queue_cnt; i++) {
        if (current_cylinder<request_queue[i].cylinder){
            is_larger=true;
            break;
        }
    }
    for (int i = 0; i < *queue_cnt; i++) {
        if (scan_direction==1){
            // pick closest to current 
            if (is_larger){
                if (request_queue[i].cylinder>current_cylinder){
                    int new_diff=abs(request_queue[i].cylinder-current_cylinder);
                    if (new_diff<abs(res_cylinder-current_cylinder)){
                        res_index=i;
                        res_cylinder=request_queue[i].cylinder;
                    }
                }
            } else {
                if (request_queue[i].cylinder<=current_cylinder){
                    int new_diff=abs(request_queue[i].cylinder-current_cylinder);
                    if (new_diff<abs(res_cylinder-current_cylinder)){
                        res_index=i;
                        res_cylinder=request_queue[i].cylinder;
                    }
                }
            }
        } else {
            // pick closest to current 
            if (!is_larger){
                if (request_queue[i].cylinder<current_cylinder){
                    int new_diff=abs(request_queue[i].cylinder-current_cylinder);
                    if (new_diff<abs(res_cylinder-current_cylinder)){
                        res_index=i;
                        res_cylinder=request_queue[i].cylinder;
                    }
                }
            } else {
                if (request_queue[i].cylinder>=current_cylinder){
                    int new_diff=abs(request_queue[i].cylinder-current_cylinder);
                    if (new_diff<abs(res_cylinder-current_cylinder)){
                        res_index=i;
                        res_cylinder=request_queue[i].cylinder;
                    }
                }
            }
        }
    }
    next_RCB = request_queue[res_index];
    for (int index = res_index; index < *queue_cnt; index++) {
        request_queue[index] = request_queue[index + 1];
    }
    *queue_cnt -= 1;
    return next_RCB;
    
}

