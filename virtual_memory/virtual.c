#include "oslabs.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

/**
 * PTE is_valid, frame_number, arrival_timestamp, last_access_timestamp, reference_count
 *
 */
int process_page_access_fifo(struct PTE page_table[TABLEMAX], int *table_cnt, int page_number, int frame_pool[POOLMAX], int *frame_cnt, int current_timestamp) {
    // check if referenced page is already in memory
    // the page-table entry has the valid bit true
    // modify last_access_timestamp and reference_count
    // return frame number
    if (page_table[page_number].is_valid == true) {
        page_table[page_number].reference_count=page_table[page_number].reference_count+1;
        page_table[page_number].last_access_timestamp = current_timestamp;
        return page_table[page_number].frame_number;
    } else {
        // check if there are any free frames - the process frame pool is not empty
        if (*frame_cnt > 0){
            page_table[page_number].frame_number = frame_pool[*frame_cnt - 1];
            *frame_cnt -= 1;
            page_table[page_number].arrival_timestamp=current_timestamp;
            page_table[page_number].last_access_timestamp = current_timestamp;
            page_table[page_number].reference_count = 1;
            page_table[page_number].is_valid = true;
            return page_table[page_number].frame_number;
        } else {
            // find smallest arriavl_timestamp -> mark invalid and -1
            bool found = false;
            int smallest_i = 0;
            int smallest_frame = 0;
            int smallest_arrival_stemp = 2147483647;
            for (int i = 0; i < *table_cnt; i++){
                if (page_table[i].is_valid == true) {
                    if (found == false){
                        found = true;
                        smallest_i = i;
                        smallest_frame = page_table[i].frame_number;
                        smallest_arrival_stemp = page_table[i].arrival_timestamp;
                        found = true;
                    } else {
                        if (page_table[i].arrival_timestamp != -1 && smallest_arrival_stemp > page_table[i].arrival_timestamp){
                            smallest_i = i;
                            smallest_frame = page_table[i].frame_number;
                            smallest_arrival_stemp = page_table[i].arrival_timestamp;
                        }
                    }
                }
            }
            if (found == true){
                page_table[smallest_i].arrival_timestamp = -1;
                page_table[smallest_i].reference_count = -1;
                page_table[smallest_i].last_access_timestamp = -1;
                page_table[smallest_i].frame_number = -1;
                page_table[smallest_i].is_valid = false;

                page_table[page_number].frame_number = smallest_frame;
                page_table[page_number].arrival_timestamp = current_timestamp;
                page_table[page_number].is_valid = true;
                page_table[page_number].last_access_timestamp = current_timestamp;
                page_table[page_number].reference_count = 1;
            }
            return smallest_frame;
        }
    }
}
int count_page_faults_fifo(struct PTE page_table[TABLEMAX], int table_cnt, int refrence_string[REFERENCEMAX], int reference_cnt, int frame_pool[POOLMAX], int frame_cnt){
    int timestamp = 1, page_fault = 0;
    for (int i = 0; i < reference_cnt; i++) {
        timestamp=timestamp+1;
        int page_number=refrence_string[i];
        if (page_table[page_number].is_valid == true) {
            page_table[page_number].reference_count=page_table[page_number].reference_count+1;
            page_table[page_number].last_access_timestamp = timestamp;
        } else {
            page_fault += 1;
            // check if there are any free frames - the process frame pool is not empty
            if (frame_cnt > 0){
                page_table[page_number].frame_number = frame_pool[frame_cnt - 1];
                frame_cnt -= 1;
                page_table[page_number].arrival_timestamp=timestamp;
                page_table[page_number].last_access_timestamp = timestamp;
                page_table[page_number].reference_count = 1; // double check increment 
                page_table[page_number].is_valid = true;
                page_table[page_number].frame_number = frame_pool[frame_cnt - 1];
            } else {
                // find smallest arriavl_timestamp -> mark invalid and -1
                bool found = false;
                int smallest_i = 0;
                int smallest_frame = 0;
                int smallest_arrival_stemp = 2147483647;
                for (int i = 0; i < table_cnt; i++){
                    if (page_table[i].is_valid == true) {
                        if (found == false){
                            found = true;
                            smallest_i = i;
                            smallest_frame = page_table[i].frame_number;
                            smallest_arrival_stemp = page_table[i].arrival_timestamp;
                            found = true;
                        } else {
                            if (page_table[i].arrival_timestamp != -1 && smallest_arrival_stemp > page_table[i].arrival_timestamp){
                                smallest_i = i;
                                smallest_frame = page_table[i].frame_number;
                                smallest_arrival_stemp = page_table[i].arrival_timestamp;
                            }
                        }
                    }
                }
                if (found == true){
                    page_table[smallest_i].arrival_timestamp = -1;
                    page_table[smallest_i].reference_count = -1;
                    page_table[smallest_i].last_access_timestamp = -1;
                    page_table[smallest_i].frame_number = -1;
                    page_table[smallest_i].is_valid = false;

                    page_table[page_number].frame_number = smallest_frame;
                    page_table[page_number].arrival_timestamp = timestamp;
                    page_table[page_number].is_valid = true;
                    page_table[page_number].last_access_timestamp = timestamp;
                    page_table[page_number].reference_count = 1;
                }
            }

        }
    }
    return page_fault;
}

int process_page_access_lru(struct PTE page_table[TABLEMAX], int *table_cnt, int page_number, int frame_pool[POOLMAX], int *frame_cnt, int current_timestamp){
    // check if referenced page is already in memory
    // the page-table entry has the valid bit true
    // modify last_access_timestamp and reference_count
    // return frame number
    if (page_table[page_number].is_valid == true) {
        page_table[page_number].reference_count=page_table[page_number].reference_count+1;
        page_table[page_number].last_access_timestamp = current_timestamp;
        return page_table[page_number].frame_number;
    } else {
        // check if there are any free frames - the process frame pool is not empty
        if (*frame_cnt > 0){
            page_table[page_number].frame_number = frame_pool[*frame_cnt - 1];
            *frame_cnt -= 1;
            page_table[page_number].arrival_timestamp=current_timestamp;
            page_table[page_number].last_access_timestamp = current_timestamp;
            page_table[page_number].reference_count = 1;
            page_table[page_number].is_valid = true;
            return page_table[page_number].frame_number;
        } else {
            // find smallest arriavl_timestamp -> mark invalid and -1
            bool found = false;
            int smallest_i = 0;
            int smallest_frame = 0;
            int smallest_last_access_timestamp = 2147483647;
            for (int i = 0; i < *table_cnt; i++){
                if (page_table[i].is_valid == true) {
                    if (found == false){
                        found = true;
                        smallest_i = i;
                        smallest_frame = page_table[i].frame_number;
                        smallest_last_access_timestamp = page_table[i].last_access_timestamp;
                        found = true;
                    } else {
                        if (page_table[i].last_access_timestamp != -1 && smallest_last_access_timestamp > page_table[i].last_access_timestamp){
                            smallest_i = i;
                            smallest_frame = page_table[i].frame_number;
                            smallest_last_access_timestamp = page_table[i].last_access_timestamp;
                        }
                    }
                }
            }
            if (found == true){
                page_table[smallest_i].arrival_timestamp = -1;
                page_table[smallest_i].reference_count = -1;
                page_table[smallest_i].last_access_timestamp = -1;
                page_table[smallest_i].frame_number = -1;
                page_table[smallest_i].is_valid = false;

                page_table[page_number].frame_number = smallest_frame;
                page_table[page_number].arrival_timestamp = current_timestamp;
                page_table[page_number].is_valid = true;
                page_table[page_number].last_access_timestamp = current_timestamp;
                page_table[page_number].reference_count = 1;
            }
            return smallest_frame;
        }
    }
}

int count_page_faults_lru(struct PTE page_table[TABLEMAX], int table_cnt, int refrence_string[REFERENCEMAX], int reference_cnt, int frame_pool[POOLMAX], int frame_cnt)
{
    int timestamp = 1, page_fault = 0;
    for (int i = 0; i < reference_cnt; i++){
        timestamp = timestamp+1;
        if (page_table[refrence_string[i]].is_valid == true)
        {
            page_table[refrence_string[i]].last_access_timestamp = timestamp;
            page_table[refrence_string[i]].reference_count += 1;
        }
        else if ((page_table[refrence_string[i]].is_valid == false) && (frame_cnt > 0))
        {
            page_table[refrence_string[i]].frame_number = frame_pool[frame_cnt - 1];
            page_table[refrence_string[i]].last_access_timestamp = timestamp;
            page_table[refrence_string[i]].reference_count += 1;
            page_table[refrence_string[i]].is_valid = true;
            frame_cnt -= 1;
            page_fault += 1;
        }
        else if ((page_table[refrence_string[i]].is_valid == false) && (frame_cnt == 0))
        {
            page_fault += 1;
            bool flag_first = false;
            int temp_index = 0;
            int temp_frame = 0;
            int temp_last_arrival_stemp = 0;
            for (int index = 0; index < table_cnt; index++)
            {
                if ((page_table[index].is_valid == true) && (flag_first == false))
                {
                    temp_index = index;
                    temp_frame = page_table[index].frame_number;
                    temp_last_arrival_stemp = page_table[index].arrival_timestamp;
                    flag_first = true;
                }
                else if ((page_table[index].is_valid == true) && (temp_last_arrival_stemp > page_table[index].last_access_timestamp) && (flag_first == true))
                {
                    temp_last_arrival_stemp = page_table[index].last_access_timestamp;
                    temp_index = index;
                    temp_frame = page_table[index].frame_number;
                }
            }
            if (flag_first == true)
            {
                page_table[temp_index].arrival_timestamp = 0;
                page_table[temp_index].reference_count = 0;
                page_table[temp_index].last_access_timestamp = 0;
                page_table[temp_index].frame_number = -1;
                page_table[temp_index].is_valid = false;
                page_table[refrence_string[i]].frame_number = temp_frame;
                page_table[refrence_string[i]].arrival_timestamp = timestamp;
                page_table[refrence_string[i]].is_valid = true;
                page_table[refrence_string[i]].last_access_timestamp = timestamp;
                page_table[refrence_string[i]].reference_count = 1;
            }
        }
    }
    return page_fault;
}

int process_page_access_lfu(struct PTE page_table[TABLEMAX], int *table_cnt, int page_number, int frame_pool[POOLMAX], int *frame_cnt, int current_timestamp){
    // check if referenced page is already in memory
    // the page-table entry has the valid bit true
    // modify last_access_timestamp and reference_count
    // return frame number
    if (page_table[page_number].is_valid == true) {
        page_table[page_number].reference_count=page_table[page_number].reference_count+1;
        page_table[page_number].last_access_timestamp = current_timestamp;
        return page_table[page_number].frame_number;
    } else {
        // check if there are any free frames - the process frame pool is not empty
        if (*frame_cnt > 0){
            page_table[page_number].frame_number = frame_pool[*frame_cnt - 1];
            *frame_cnt -= 1;
            page_table[page_number].arrival_timestamp=current_timestamp;
            page_table[page_number].last_access_timestamp = current_timestamp;
            page_table[page_number].reference_count = 1;
            page_table[page_number].is_valid = true;
            return page_table[page_number].frame_number;
        } else {
            // find smallest reference unit -> mark invalid and -1
            bool found = false;
            int smallest_i = 0;
            int smallest_frame = 0;
            int smallest_arrival_timestamp = 2147483647;
            int smallest_reference_count=2147483647;
            for (int i = 0; i < *table_cnt; i++){
                if (page_table[i].is_valid == true) {
                    if (found == false){
                        found = true;
                        smallest_i = i;
                        smallest_frame = page_table[i].frame_number;
                        smallest_arrival_timestamp = page_table[i].arrival_timestamp;
                        smallest_reference_count=page_table[i].reference_count;
                        found = true;
                    } else {
                        if (page_table[i].arrival_timestamp != -1 && smallest_reference_count > page_table[i].reference_count){
                            smallest_i = i;
                            smallest_frame = page_table[i].frame_number;
                            smallest_arrival_timestamp = page_table[i].arrival_timestamp;
                            smallest_reference_count=page_table[i].reference_count;
                        } else if (page_table[i].arrival_timestamp != -1 && smallest_reference_count==page_table[i].reference_count){   
                            if (smallest_arrival_timestamp>page_table[i].arrival_timestamp){
                                smallest_i = i;
                                smallest_frame = page_table[i].frame_number;
                                smallest_arrival_timestamp = page_table[i].arrival_timestamp;
                                smallest_reference_count=page_table[i].reference_count;
                            }
                        }
                         
                    }
                }
            }
            if (found == true){
                page_table[smallest_i].arrival_timestamp = -1;
                page_table[smallest_i].reference_count = -1;
                page_table[smallest_i].last_access_timestamp = -1;
                page_table[smallest_i].frame_number = -1;
                page_table[smallest_i].is_valid = false;

                page_table[page_number].frame_number = smallest_frame;
                page_table[page_number].arrival_timestamp = current_timestamp;
                page_table[page_number].is_valid = true;
                page_table[page_number].last_access_timestamp = current_timestamp;
                page_table[page_number].reference_count = 1;
            }
            return smallest_frame;
        }
    }
}

int count_page_faults_lfu(struct PTE page_table[TABLEMAX], int table_cnt, int refrence_string[REFERENCEMAX], int reference_cnt, int frame_pool[POOLMAX], int frame_cnt)
{
    int timestamp = 1, page_fault = 0;
    for (int i = 0; i < reference_cnt; i++){
        timestamp += 1;
        if (page_table[refrence_string[i]].is_valid == true)
        {
            page_table[refrence_string[i]].last_access_timestamp = timestamp;
            page_table[refrence_string[i]].reference_count += 1;
        }
        else if ((page_table[refrence_string[i]].is_valid == false) && (frame_cnt > 0))
        {
            page_table[refrence_string[i]].frame_number = frame_pool[frame_cnt - 1];
            page_table[refrence_string[i]].last_access_timestamp = timestamp;
            page_table[refrence_string[i]].arrival_timestamp = timestamp;
            page_table[refrence_string[i]].reference_count += 1;
            page_table[refrence_string[i]].is_valid = true;
            frame_cnt -= 1;
            page_fault += 1;
        }
        else if ((page_table[refrence_string[i]].is_valid == false) && (frame_cnt == 0))
        {
            page_fault += 1;
            bool flag_first = false, flag_equal = false;
            int temp_index = 0;
            int temp_frame = 0;
            int temp_f_used = 0;
            int temp_arrivel_time = 0;
            for (int index = 0; index < table_cnt; index++)
            {
                if ((page_table[index].is_valid == true) && (flag_first == false))
                {
                    temp_index = index;
                    temp_frame = page_table[index].frame_number;
                    temp_f_used = page_table[index].reference_count;
                    temp_arrivel_time = page_table[index].arrival_timestamp;
                    flag_first = true;
                }
                else if ((page_table[index].is_valid == true) && (temp_f_used >= page_table[index].reference_count) && (flag_first == true))
                {
                    if (((temp_f_used == page_table[index].reference_count) && (temp_arrivel_time > page_table[index].arrival_timestamp)))
                    {
                        temp_f_used = page_table[index].reference_count;
                        temp_index = index;
                        temp_frame = page_table[index].frame_number;
                        temp_arrivel_time = page_table[index].arrival_timestamp;
                        flag_equal = true;
                    }
                    else if ((temp_f_used > page_table[index].reference_count) && (flag_equal == false))
                    {
                        temp_f_used = page_table[index].reference_count;
                        temp_index = index;
                        temp_frame = page_table[index].frame_number;
                        temp_arrivel_time = page_table[index].arrival_timestamp;
                    }
                }
            }
            if (flag_first == true)
            {
                page_table[temp_index].arrival_timestamp = 0;
                page_table[temp_index].reference_count = 0;
                page_table[temp_index].last_access_timestamp = 0;
                page_table[temp_index].frame_number = -1;
                page_table[temp_index].is_valid = false;
                page_table[refrence_string[i]].frame_number = temp_frame;
                page_table[refrence_string[i]].arrival_timestamp = timestamp;
                page_table[refrence_string[i]].is_valid = true;
                page_table[refrence_string[i]].last_access_timestamp = timestamp;
                page_table[refrence_string[i]].reference_count = 1;
            }
        }
    }
    return page_fault;
}


