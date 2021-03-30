/*
Implement deadlock detection algorithm that is applicable to a system with resources
having multiple instances. The algorithm employs several time-varying data structures,
* Available: A vector of length m indicates the number of available resources of each type.
* Allocation: An n × m matrix defines the number of resources of each type currently allocated to each process.
* Request: An n × m matrix indicates the current request of each process.
If Request[i][j] equals k, then process Pi is requesting k more instances of resource type Rj .
You need to print the safe sequence if the system is deadlock free, otherwise print a suitable
message.
*/

#include<stdio.h>
#include<stdlib.h>
#define complete 1
#define incomplete 0
#define True 1
#define False 0

void printSafeSequence(unsigned int *available, unsigned int **allocation, unsigned int **request, 
                       unsigned int num_resources, unsigned int num_processes){
    unsigned int *state = (unsigned int*)calloc(num_processes, sizeof(unsigned int)); // Holds the state of completion of all the processes
    unsigned int *safeSequence = (unsigned int*)malloc(num_processes * sizeof(unsigned int));
    unsigned int proc_number = 0, deadlock, feasible, i, j;
    while(proc_number < num_processes){
        deadlock = True; // Assume a deadlock has occurred
        for(i = 0; i < num_processes; i++){
            if(state[i] == incomplete){ // Ignore processes that have been completed
                feasible = True;
                for(j = 0; j < num_resources; j++)
                    if(request[i][j] > available[j]){
                        feasible = False;  // Servicing this process is infeasible as the requested resources are unavailable
                        break;
                    } 
                if(feasible){ // ith process can be allowed to run to termination as its requested resources are available
                    safeSequence[proc_number++] = i;
                    deadlock = False;
                    state[i] = complete;
                    for(j = 0; j < num_resources; j++) available[j] += allocation[i][j]; // Adding the released resources to the available resource pool
                }
            }
        }
        if(deadlock) break;
    }
    if(deadlock) printf("A deadlock will occur\n"); 
    else{
        printf("Available resource vector after all processes have been completed is: ");
        for(i = 0; i < num_resources; i++) printf("%u ", available[i]);
        printf("\nOne of the safe sequences is: ");
        for(i = 0; i < num_processes; i++) printf("Process %u -> ", safeSequence[i]);
        printf("\b\b\b  \n");
    } 
    free(state);
    free(safeSequence);
}

int main(){
    unsigned int num_processes, num_resources, i, j;
    printf("Enter the number of processes: "); scanf("%u", &num_processes);
    printf("Enter the number of types of resources: "); scanf("%u", &num_resources);
    unsigned int *available = (unsigned int*)malloc(sizeof(unsigned int) * num_resources);
    unsigned int **allocation = (unsigned int**)malloc(sizeof(unsigned int*) * num_processes);
    unsigned int **request = (unsigned int**)malloc(sizeof(unsigned int*) * num_processes); 
    for(i = 0; i < num_processes; i++){
        allocation[i] = (unsigned int*)malloc(sizeof(unsigned int) * num_resources);
        request[i] = (unsigned int*)malloc(sizeof(unsigned int) * num_resources);
    }
    printf("Enter the total instances available of each type of resource (the available vector) separated by spaces: ");
    for(i = 0; i < num_resources; i++) scanf("%u", &available[i]);
    printf("Enter the allocation matrix:\n");
    for(i = 0; i < num_processes; i++)
        for(j = 0; j < num_resources; j++)
            scanf("%u", &allocation[i][j]);
    printf("Enter the request matrix:\n");
    for(i = 0; i < num_processes; i++)
        for(j = 0; j < num_resources; j++)
            scanf("%u", &request[i][j]);
    printSafeSequence(available, allocation, request, num_resources, num_processes);
    for(i = 0; i < num_processes; i++){
        free(allocation[i]);
        free(request[i]);
    }
    free(allocation);
    free(available);
    free(request);
    return 0;
}
