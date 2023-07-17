void release_memory(struct MEMORY_BLOCK freed_block, struct MEMORY_BLOCK memory_map[MAPMAX], int *map_cnt)
{
    bool flag = false;
    if ((*map_cnt == 1) && (memory_map[0].end_address == 0) &&
        (memory_map[0].start_address == 0) && (memory_map[0].process_id == 0) &&
        (memory_map[0].segment_size == 0))
        return;
    else
    {
        for (int i = 0; i < *map_cnt; i++)
        {
            if ((freed_block.start_address == memory_map[i].start_address) && (freed_block.end_address == memory_map[i].end_address) &&
                (freed_block.process_id == memory_map[i].process_id))
            {
                memory_map[i].process_id = 0;
                if (i > 0)
                {
                    if (memory_map[i - 1].process_id == 0)
                    {
                        memory_map[i - 1].end_address =
                            freed_block.end_address;
                        memory_map[i - 1].segment_size = memory_map[i1].segment_size + freed_block.segment_size;
                        for (int index = i; index <= *map_cnt;
                             index++)
                        {
                            memory_map[index] = memory_map[index +
                                                           1];
                        }
                        *map_cnt = *map_cnt - 1;
                        flag = true;
                    }
                }
                if (i < *map_cnt - 1)
                {
                    if (flag == false)
                    {
                        i = i + 1;
                    }
                    if (memory_map[i].process_id == 0)
                    {
                        memory_map[i].start_address = memory_map[i1].start_address;
                        memory_map[i].segment_size =
                            memory_map[i].end_address - memory_map[i].start_address + 1;
                        for (int index = i; index <= *map_cnt;
                             index++)
                        {
                            memory_map[index - 1] =
                                memory_map[index];
                        }
                        *map_cnt = *map_cnt - 1;
                    }
                }
                break;
            }
        }
    }
}


    struct MEMORY_BLOCK curr_block, memory_allocation;
    memory_allocation.end_address = 0;
    memory_allocation.start_address = 0;
    memory_allocation.process_id = 0;
    memory_allocation.segment_size = 0;
    bool found = false;
    int memory_table_index = 0, best_fit_segmant = 0;
    for (int i = 0; i <= *map_cnt - 1; i++) {
        if (request_size <= memory_map[i].segment_size)
        {
            found = true;
            memory_table_index = i;
            best_fit_segmant = request_size;
            break;
        }
    }
    if (found == true)
    {
        curr_block = memory_map[memory_table_index];
        memory_allocation.start_address = curr_block.start_address;
        memory_allocation.end_address = memory_allocation.start_address + request_size - 1;
        memory_allocation.process_id = process_id;
        memory_allocation.segment_size = request_size;
        *map_cnt = *map_cnt + 1;
        for (int i = memory_table_index; i <= *map_cnt; i++)
        {
            memory_map[memory_table_index + 1] = memory_map[i];
        }
        memory_map[memory_table_index + 1].start_address = memory_allocation.end_address + 1;
        memory_map[memory_table_index + 1].end_address = memory_map[memory_table_index].end_address;
        memory_map[memory_table_index + 1].process_id = memory_map[memory_table_index].process_id;
        memory_map[memory_table_index + 1].segment_size = memory_map[memory_table_index].segment_size - request_size;
        memory_map[memory_table_index] = memory_allocation;
    }
    return memory_allocation;