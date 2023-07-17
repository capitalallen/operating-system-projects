#include "oslabs.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>


struct MEMORY_BLOCK allocate_memory(bool perfect_match, bool found, int memory_map_index, int request_size, int process_id, int *map_cnt, struct MEMORY_BLOCK memory_map[MAPMAX], struct MEMORY_BLOCK temp_memory_block,  struct MEMORY_BLOCK allocated_memory){
    // if perfect match, allocate and return; 
    if (perfect_match){
        memory_map[memory_map_index].segment_size=request_size;
        memory_map[memory_map_index].process_id=process_id;
        return memory_map[memory_map_index];
    } else if (found){ 
        temp_memory_block = memory_map[memory_map_index];
        allocated_memory.start_address = temp_memory_block.start_address;
        allocated_memory.end_address = allocated_memory.start_address + request_size - 1;
        allocated_memory.process_id = process_id;
        allocated_memory.segment_size = request_size;
        *map_cnt = *map_cnt + 1;

        struct MEMORY_BLOCK prev_block = memory_map[memory_map_index + 1]; 
        memory_map[memory_map_index + 1].start_address = allocated_memory.end_address + 1;
        memory_map[memory_map_index + 1].end_address = memory_map[memory_map_index].end_address;
        memory_map[memory_map_index + 1].process_id = memory_map[memory_map_index].process_id;
        memory_map[memory_map_index + 1].segment_size = memory_map[memory_map_index].segment_size - request_size;
        memory_map[memory_map_index] = allocated_memory;

        for (int i = memory_map_index+2; i <= *map_cnt; i++){
            struct MEMORY_BLOCK temp = memory_map[i]; 
            memory_map[i]=prev_block; 
            prev_block=temp; 
        }
    } 
    return allocated_memory;
}
/**
 *
 * This method allocates memory according to the Best Fit scheme.
 * The method is given the process id of the requesting process,
 * size of the memory being requested, and the memory map.
 */

struct MEMORY_BLOCK best_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt, int process_id) {
    struct MEMORY_BLOCK temp_memory_block, allocated_memory;
    allocated_memory.end_address = 0;
    allocated_memory.start_address = 0;
    allocated_memory.process_id = 0;
    allocated_memory.segment_size = 0;

    bool found = false;
    bool perfect_match=false; 
    int memory_map_index = 0, best_fit_segmant = 0;

    for (int i = 0; i <= *map_cnt - 1; i++){
        // if request size is same as free block 
        if (request_size == memory_map[i].segment_size && memory_map[i].process_id==0){
            found = true;
            perfect_match=true; 
            memory_map_index = i;
            best_fit_segmant = request_size;
            break;
        }
        // if request size is larger, find smallest free block that is larger than requested
        else if ((memory_map[i].segment_size > request_size) && (memory_map[i].process_id == 0)){
            if (found == false){
                found = true;
                memory_map_index = i;
                best_fit_segmant = memory_map[i].segment_size;
            } else {
                if (best_fit_segmant > memory_map[i].segment_size)
                {
                    memory_map_index = i;
                    best_fit_segmant = memory_map[i].segment_size;
                }
            }
        }
    }
    return allocate_memory(perfect_match,found,memory_map_index,request_size,process_id,map_cnt,memory_map,temp_memory_block,allocated_memory);
}

/*
 * This method allocates memory according to the First Fit scheme.
 * The method is given the process id of the requesting process, size of the memory being requested, and the memory map.
 */
struct MEMORY_BLOCK first_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt, int process_id) {
    struct MEMORY_BLOCK temp_memory_block, allocated_memory;
    allocated_memory.end_address = 0;
    allocated_memory.start_address = 0;
    allocated_memory.process_id = 0;
    allocated_memory.segment_size = 0;
    int memory_map_index = 0, best_fit_segmant = 0;

    // iterate through memory map, find first that is fit -> found -> True 
    // if request size == free block -> perfect_match=True 
    bool found=false, perfect_match=false; 
    for (int i = 0; i <= *map_cnt - 1; i++) {
        if (request_size<=memory_map[i].segment_size && memory_map[i].process_id==0){
            found=true; 
            if (request_size==memory_map[i].segment_size){
                perfect_match=true;
                best_fit_segmant=request_size;
            } else {
                best_fit_segmant=memory_map[i].segment_size;
            }
            memory_map_index=i; 
        }
        if (found){
            break; 
        }
    }
    return allocate_memory(perfect_match,found,memory_map_index,request_size,process_id,map_cnt,memory_map,temp_memory_block,allocated_memory);
}


struct MEMORY_BLOCK worst_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt, int process_id)
{
    struct MEMORY_BLOCK temp_memory_block, allocated_memory;
    allocated_memory.end_address = 0;
    allocated_memory.start_address = 0;
    allocated_memory.process_id = 0;
    allocated_memory.segment_size = 0;

    bool found = false;
    bool perfect_match=false; 
    int memory_map_index = 0, worst_fit_segmant = 0;
    for (int i = 0; i <= *map_cnt - 1; i++){
        // if request size is larger, find smallest free block that is larger than requested
        if ((memory_map[i].segment_size >= request_size) && (memory_map[i].process_id == 0)){
            if (found == false){
                found = true;
                memory_map_index = i;
                worst_fit_segmant = memory_map[i].segment_size;
            } else {
                if (worst_fit_segmant < memory_map[i].segment_size){
                    memory_map_index = i;
                    worst_fit_segmant = memory_map[i].segment_size;
                }
            }
        }
    }
    if (request_size==worst_fit_segmant){
        perfect_match=true;
    }
    return allocate_memory(perfect_match,found,memory_map_index,request_size,process_id,map_cnt,memory_map,temp_memory_block,allocated_memory);
}

/*
 * This method allocates memory according to the Next Fit scheme.
 * The method is given the process id of the requesting process, size of the memory requested, the memory map,
 * and the address of the last block allocated.
 */
struct MEMORY_BLOCK next_fit_allocate(int request_size, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt, int process_id, int last_address) {
    struct MEMORY_BLOCK temp_memory_block, allocated_memory;
    allocated_memory.end_address = 0;
    allocated_memory.start_address = 0;
    allocated_memory.process_id = 0;
    allocated_memory.segment_size = 0;

    bool found = false;
    bool perfect_match=false; 
    int memory_map_index = 0, next_fit_segmant = 0;

    // iterate through memory map 
        // curr.start_address>=last_address && curr.process_id==0 
            // found -> true; break 
    for (int i = 0; i <= *map_cnt - 1; i++){
        // if request size is same as free block 
        if (last_address <= memory_map[i].start_address && memory_map[i].process_id==0 && request_size<=memory_map[i].segment_size){
            found = true;
            memory_map_index = i;
            next_fit_segmant = memory_map[i].segment_size;
            break; 
        }
    }
    // check if it's a perfect match if found==true 
    if (found && request_size==next_fit_segmant){
        perfect_match=true; 
    }
    return allocate_memory(perfect_match,found,memory_map_index,request_size,process_id,map_cnt,memory_map,temp_memory_block,allocated_memory);
}

void release_memory(struct MEMORY_BLOCK freed_block, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt) {
    bool flag = false;
    if ((*map_cnt == 1) && (memory_map[0].end_address == 0) && (memory_map[0].start_address == 0) && (memory_map[0].process_id == 0) && (memory_map[0].segment_size == 0))
        return;
    else {
        for (int i = 0; i < *map_cnt; i++) {
            if ((freed_block.start_address == memory_map[i].start_address) && (freed_block.end_address == memory_map[i].end_address) && (freed_block.process_id == memory_map[i].process_id)) {
                memory_map[i].process_id = 0;
                if (i > 0) {
                    if (memory_map[i - 1].process_id == 0) {
                        memory_map[i - 1].end_address = freed_block.end_address;
                        memory_map[i - 1].segment_size = memory_map[i - 1].segment_size + freed_block.segment_size;
                        for (int index = i; index <= *map_cnt; index++) {
                            memory_map[index] = memory_map[index + 1];
                        }
                        *map_cnt = *map_cnt - 1;
                        flag = true;
                    }
                }
                if (i < *map_cnt - 1) {
                    if (flag == false) {
                        i = i + 1;
                    }
                    if (memory_map[i].process_id == 0) {
                        memory_map[i].start_address = memory_map[i - 1].start_address;
                        memory_map[i].segment_size = memory_map[i].end_address - memory_map[i].start_address + 1;
                        for (int index = i; index <= *map_cnt; index++) {
                            memory_map[index - 1] = memory_map[index];
                        }
                        *map_cnt = *map_cnt - 1;
                    }
                }
                break;
            }
        }
    }
}
