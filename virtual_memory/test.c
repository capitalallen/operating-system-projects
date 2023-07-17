    // check if referenced page is already in memory 
    // the page-table entry has the valid bit true 
    // modify last_access_timestamp and reference_count 
    // return frame number 
    if (page_table[page_number].is_valid == true) { 
        page_table[page_number].reference_count += 1;
        page_table[page_number].last_access_timestamp = current_timestamp;
        int res = page_table[page_number].frame_number;
        return res;
    }

    // check if there are any free frames - the process frame pool is not empty 
    if ((page_table[page_number].is_valid == false) && (*frame_cnt > 0)) {
        page_table[page_number].frame_number = frame_pool[*frame_cnt - 1];
        *frame_cnt -= 1;
        page_table[page_number].last_access_timestamp = current_timestamp;
        page_table[page_number].reference_count = 1;
        page_table[page_number].is_valid = true;
        int res = page_table[page_number].frame_number;
        return page_table[page_number].frame_number;
    } else { 
        // find smallest arriavl_timestamp -> mark invalid and -1 
        bool found = false;
        int smallest_i = 0;
        int smallest_frame = 0;
        int smallest_arrival_stemp = 10000;
        for (int i = 0; i < *table_cnt; i++)
        {
            if ((page_table[i].is_valid == true) && (found == false))
            {
                found=true; 
                smallest_i = i;
                smallest_i = page_table[i].frame_number;
                smallest_i = page_table[i].arrival_timestamp;
                found = true;
            } else if ((page_table[i].is_valid == true) && (page_table[i].arrival_timestamp!=-1) && (smallest_arrival_stemp > page_table[i].arrival_timestamp) && (found == true))
            {
                smallest_i = i;
                smallest_frame = page_table[i].frame_number;
                smallest_arrival_stemp = page_table[i].arrival_timestamp;
            }
        }
        if (found == true) {
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


// void printPte(struct PTE page_table[8])
// {
//     for (int i = 0; i < 8; i++)
//     {
//         printf("is_valid: %d, frame_number: %d, arrival_timestamp: %d, last_access_timestamp: %d, reference_count: %d\n",
//                page_table[i].is_valid, page_table[i].frame_number, page_table[i].arrival_timestamp, page_table[i].last_access_timestamp, page_table[i].reference_count);
//     }
// }
// int main()
// {
//     // PTE is_valid, frame_number, arrival_timestamp, last_access_timestamp, reference_count
//     struct PTE pte1 = {false, -1, -1, -1, -1};
//     struct PTE pte2 = {false, -1, -1, -1, -1};
//     struct PTE pte3 = {true, 10, 3, 3, 1};
//     struct PTE pte4 = {false, -1, -1, -1, -1};
//     struct PTE pte5 = {false, -1, -1, -1, -1};
//     struct PTE pte6 = {true, 20, 2, 4, 2};
//     struct PTE pte7 = {false, -1, -1, -1, -1};
//     struct PTE pte8 = {true, 30, 1, 1, 1};
//     struct PTE page_table[8] = {pte1, pte2, pte3, pte4, pte5, pte6, pte7, pte8};

//     int table_cnt = 8;
//     // int ref_string[11] = {0, 3, 2, 6, 3, 4, 5, 2, 4, 5, 6};
//     // int ref_cnt = 11;
//     int frame_pool[0] = {};
//     int frame_cnt = 0;

//     // int page_faults = count_page_faults_lfu(page_table, table_cnt, ref_string, ref_cnt, frame_pool, frame_cnt);
//     int page_faults = process_page_access_lfu(page_table, &table_cnt, 0, frame_pool, &frame_cnt, 12);
//     printPte(page_table);
//     printf("page faluts: %d\n", page_faults);
//     // process_page_access_fifo
//     return 0;
// }


    // int timestamp = 1, page_fault = 0;
    // for (int i = 0; i < reference_cnt; i++)
    // {
    //     timestamp += 1;
    //     if (page_table[refrence_string[i]].is_valid == true)
    //     {
    //         page_table[refrence_string[i]].last_access_timestamp = timestamp;
    //         page_table[refrence_string[i]].arrival_timestamp = timestamp;
    //         page_table[refrence_string[i]].reference_count += 1;
    //     }
    //     else if ((page_table[refrence_string[i]].is_valid == false) && (frame_cnt > 0))
    //     {
    //         page_table[refrence_string[i]].frame_number = frame_pool[frame_cnt - 1];
    //         page_table[refrence_string[i]].last_access_timestamp = timestamp;
    //         page_table[refrence_string[i]].arrival_timestamp = timestamp;
    //         page_table[refrence_string[i]].reference_count += 1;
    //         page_table[refrence_string[i]].is_valid = true;
    //         frame_cnt -= 1;
    //         page_fault += 1;
    //     }
    //     else if ((page_table[refrence_string[i]].is_valid == false) && (frame_cnt == 0))
    //     {
    //         page_fault += 1;
    //         bool flag_first = false;
    //         int temp_index = 0;
    //         int temp_frame = 0;
    //         int temp_arrival_timestamp = 2147483647;
    //         for (int index = 0; index < table_cnt; index++)
    //         {
    //             if ((page_table[index].is_valid == true) && (flag_first == false))
    //             {
    //                 temp_index = index;
    //                 temp_frame = page_table[index].frame_number;
    //                 temp_arrival_timestamp = page_table[index].arrival_timestamp;
    //                 flag_first = true;
    //             }
    //             else if ((page_table[index].is_valid == true) && (temp_arrival_timestamp > page_table[index].arrival_timestamp) && (flag_first == true))
    //             {
    //                 temp_arrival_timestamp = page_table[index].arrival_timestamp;
    //                 temp_index = index;
    //                 temp_frame = page_table[index].frame_number;
    //             }
    //         }
    //         if (flag_first == true)
    //         {
    //             page_table[temp_index].arrival_timestamp = 0;
    //             page_table[temp_index].reference_count = 0;
    //             page_table[temp_index].last_access_timestamp = 0;
    //             page_table[temp_index].frame_number = -1;
    //             page_table[temp_index].is_valid = false;
    //             page_table[refrence_string[i]].frame_number = temp_frame;
    //             page_table[refrence_string[i]].arrival_timestamp = timestamp;
    //             page_table[refrence_string[i]].is_valid = true;
    //             page_table[refrence_string[i]].last_access_timestamp = timestamp;
    //             page_table[refrence_string[i]].reference_count = 1;
    //         }
    //     }
    // }
    // return page_fault;