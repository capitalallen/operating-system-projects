
// int main (){
//     struct RCB null_RCB = {0,0,0,0,0};
//     struct RCB next_RCB_main;
//     struct RCB RCB_1 = {1,1,10,100,1};
//     struct RCB RCB_2 = {2,2,20,200,2};
//     struct RCB RCB_3 = {3,3,20,200,3};
//     struct RCB RCB_4 = {4,4,40,200,4};
//     struct RCB current_request = {0,0,0,0,0};
//     struct RCB request_queue[QUEUEMAX];
//     int x = 0, timestamp = 1;
//     int *queue = &x;

//     current_request = handle_request_arrival_fcfs(request_queue, queue, current_request, null_RCB, timestamp);
//     current_request = handle_request_arrival_fcfs(request_queue, queue, current_request, RCB_1, timestamp);
//     timestamp = 2;
//     current_request = handle_request_arrival_fcfs(request_queue, queue, current_request, RCB_2, timestamp);
//     timestamp = 3;
//     current_request = handle_request_arrival_fcfs(request_queue, queue, current_request, RCB_3, timestamp);
//     next_RCB_main = handle_request_completion_fcfs(request_queue, queue);



//     current_request = handle_request_arrival_look(request_queue, queue, current_request, RCB_1, timestamp);
//     current_request = handle_request_arrival_look(request_queue, queue, current_request, null_RCB, timestamp);
//     timestamp = 2;
//     current_request = handle_request_arrival_look(request_queue, queue, current_request, RCB_2, timestamp);
//     timestamp = 3;
//     current_request = handle_request_arrival_look(request_queue, queue, current_request, RCB_3, timestamp);
//     timestamp = 10;
//     current_request = handle_request_arrival_look(request_queue, queue, current_request, RCB_4, timestamp);
//     next_RCB_main = handle_request_completion_look(request_queue, queue, 20, 1);

//     return 0;
// }

void print(struct RCB request_queue[3])
{
    for (int i = 0; i < 8; i++)
    {
        printf("RID: %d, AT: %d, CYL: %d, ADDR: %d, PID: %d\n",
               request_queue[i].request_id, request_queue[i].arrival_timestamp, request_queue[i].cylinder, request_queue[i].address, request_queue[i].process_id);
    }
}


int main (){
    struct RCB null_RCB = {0,0,0,0,0};
    struct RCB next_RCB_main;
    struct RCB RCB_1 = {1,52,58,58,1};
    struct RCB RCB_2 = {1,52,58,58,2};
    struct RCB RCB_3 = {3,53,58,58,3};


    struct RCB current_request = {0,0,0,0,0};
    struct RCB request_queue[QUEUEMAX];
    request_queue[0]=RCB_1;
    request_queue[1]=RCB_2;
    request_queue[2]=RCB_3;
    int x = 3, timestamp = 1;
    int *queue = &x;

    next_RCB_main = handle_request_completion_look(request_queue, queue, 58, 1);
    print(request_queue);
    printf("------\n");
    printf("RID: %d, AT: %d, CYL: %d, ADDR: %d, PID: %d\n",
               next_RCB_main.request_id, next_RCB_main.arrival_timestamp, next_RCB_main.cylinder, next_RCB_main.address, next_RCB_main.process_id);
    return 0;
}
