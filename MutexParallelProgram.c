
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <pthread.h>
#include <stdbool.h>

//define the node
struct list_node_s{
    int data;
    struct list_node_s* next;
};


int thread_count;

//thread function
void *program(void* run);

//initializing mutex lock
pthread_mutex_t mutex;


//method definitions

int Insert(int value, struct list_node_s** head_pp);

int Member(int value, struct list_node_s* head_p);

int Delete (int value, struct list_node_s** head_pp);

void GenerateLinkedList(int n);

int GenerateRandomNumber();

double Execute();

int calculateN(double mean, double std);

void Free_list(struct list_node_s** head_pp);

//head pointer definition
struct list_node_s* head_p = NULL;


double mean;
double std;

//input argument definition
int n;
int m;
int b;
float m_member;
float m_insert;
float m_delete;

//Operation counts to be executed per each operation type
int numOfMembers;
int numOfInserts;
int numOfDeletions;

int main(int argc, char *argv[]) {

    //obtaining inputs from the command line arguments
    n =  (int) strtol(argv[1], NULL, 10);
    m =  (int) strtol(argv[2], NULL, 10);
    m_member = strtof(argv[3], NULL);
    m_insert= strtof(argv[4], NULL);
    m_delete = strtof(argv[5], NULL);
    b=(int) strtol(argv[6],NULL,10);
    thread_count = (int) strtol(argv[7], NULL, 10);

    numOfMembers = m * m_member ;
    numOfInserts = m * m_insert ;
    numOfDeletions = m * m_delete ;

    int next_b=b;
    bool first_time=true;

    while(first_time ||!(b-next_b>=0 && b-next_b<5)) {

        first_time=false;
        int a;
        double mean_sum = 0;
        double s_deviation_sum = 0;
        b=next_b;

        //loop for executing the experiment for a given sample size
        for( a = 0; a < next_b; a = a + 1 ) {
            double time_taken = Execute();
            mean_sum = mean_sum + time_taken;
            s_deviation_sum = s_deviation_sum + time_taken*time_taken;
        }

        mean = mean_sum/b;
        std = sqrt(s_deviation_sum/ b -mean*mean);

        next_b = calculateN(mean,std);
        if(next_b==1 || next_b==0){
            next_b=2;
        }

    }

    printf("Final Mean---- %lf\n",mean);
    printf("Final STD---- %lf\n",std);



    return 0;
}

//thread function implementation
void *program(void* thread_id) {


    //access the thread id
    int threadId = (int) thread_id;

    int member_count_per_thread=0;
    int insert_count_per_thread=0;
    int delete_count_per_thread=0;

    int operations_per_thread=0;

    //allocating total operation counters for each thread using thread id
    if(numOfMembers %  thread_count==0 || numOfMembers%thread_count<= threadId){
        member_count_per_thread = numOfMembers/thread_count;
    }else if(numOfMembers%thread_count > threadId){
        member_count_per_thread = (numOfMembers/thread_count) +1;
    }

    if(numOfInserts %thread_count==0 || numOfInserts%thread_count<= threadId){
        insert_count_per_thread = numOfInserts/thread_count;
    }else if(numOfInserts%thread_count > threadId){
        insert_count_per_thread = (numOfInserts/thread_count) +1;
    }

    if(numOfDeletions%thread_count==0 || numOfDeletions%thread_count<= threadId){
        delete_count_per_thread = numOfDeletions/thread_count;
    }else if(numOfDeletions%thread_count > threadId){
        delete_count_per_thread = (numOfDeletions/thread_count) +1;
    }

    operations_per_thread= member_count_per_thread + insert_count_per_thread + delete_count_per_thread;

    srand(time(0));

    //counters to keep track of operations
    int member_counter=0;
    int insert_counter =0;
    int delete_counter=0;
    int operations_per_thread_counter=0;

    while (operations_per_thread_counter  != operations_per_thread) {

        // generate random number for the operation
        int number = rand() % 65536;

        //read lock for the member operation
        int operation_index = GenerateRandomNumber();

        if (operation_index == 1 & member_counter != member_count_per_thread) {

            //mutex lock for insert operation
            pthread_mutex_lock(&mutex);
            int result = Member(number, head_p);
            pthread_mutex_unlock(&mutex);

            member_counter += 1;
            operations_per_thread_counter += 1;

        } else if (operation_index == 2 & insert_counter != insert_count_per_thread) {

            //mutex lock for insert operation
            pthread_mutex_lock(&mutex);
            int result = Insert(number, &head_p);
            pthread_mutex_unlock(&mutex);

            operations_per_thread_counter += 1;
            insert_counter += 1;
        } else if (operation_index == 3 & delete_counter != delete_count_per_thread) {

            //mutex lock for insert operation
            pthread_mutex_lock(&mutex);
            int result = Delete(number, &head_p);
            pthread_mutex_unlock(&mutex);

            operations_per_thread_counter += 1;
            delete_counter += 1;

        }
    }

    return NULL;
}

//Member function implementation for linked list
int Member(int value, struct list_node_s* head_p) {
    struct list_node_s* curr_p = head_p;

    while(curr_p != NULL && curr_p->data < value)
        curr_p = curr_p->next;
    if(curr_p == NULL || curr_p->data >value){
        return 0;
    } else {
        return 1;
    }
}

//delete function implementation for linked list
int Delete (int value, struct list_node_s** head_pp) {
    struct list_node_s* curr_p = *head_pp;
    struct list_node_s* pred_p =NULL;

    while(curr_p != NULL && curr_p->data < value){
        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if(curr_p !=NULL && curr_p->data ==value){
        if(pred_p == NULL){
            *head_pp = curr_p->next;
            free(curr_p);
        } else {
            pred_p-> next = curr_p->next;
            free(curr_p);
        }
        return 1;
    } else {
        return 0;
    }
}

//insert function implementation for linked list
int Insert (int value, struct list_node_s** head_pp) {
    struct  list_node_s* curr_p = *head_pp;
    struct  list_node_s* pred_p =NULL;
    struct  list_node_s* temp_p;

    while(curr_p != NULL &&  curr_p->data < value){

        pred_p = curr_p;
        curr_p = curr_p->next;
    }

    if(curr_p == NULL || curr_p->data > value){

        temp_p = malloc(sizeof(struct list_node_s));
        temp_p->data = value;
        temp_p->next = curr_p;
        if(pred_p == NULL){
            *head_pp = temp_p;}
        else
            pred_p->next = temp_p;
        return 1;
    } else {
        return 0;
    }
}

//generate a link list with size n
void GenerateLinkedList(int n){

    long counter=0;
    while(counter<n){
        int num = rand() % 65536;
        int result = Insert(num,&head_p);
        if(result==1){
            counter+=1;
        }
    }
}

//random number from 1-3 will be returned
int GenerateRandomNumber(){
    int lower = 1;
    int upper = 3;
    int num = (rand() % (upper - lower + 1)) + lower;
    return num;

}

int  Is_empty(struct list_node_s* head_p) {
    if (head_p == NULL)
        return 1;
    else
        return 0;
}

//remove the linked list
void Free_list(struct list_node_s** head_pp) {
    struct list_node_s* curr_p;
    struct list_node_s* succ_p;

    if (Is_empty(*head_pp)) return;
    curr_p = *head_pp;
    succ_p = curr_p->next;

    while (succ_p != NULL) {
        free(curr_p);
        curr_p = succ_p;
        succ_p = curr_p->next;
    }
    free(curr_p);
    *head_pp = NULL;
}


//calculate the sample size
int calculateN(double mean, double std) {
    double answer = (100* 1.96 * std)/(5*mean);
    int n_new = (int) (answer*answer);
    return n_new;
}


double Execute(){
    GenerateLinkedList(n);

    //time initialization
    clock_t t;


    //initialize mutex lock
    pthread_mutex_init(&mutex, NULL);
    int thread;
    pthread_t* thread_handles;
    thread_handles = malloc( thread_count * sizeof(pthread_t) );

    //start the timer
    t = clock();

    //thread creation
    for(thread =0 ; thread< thread_count; thread++){
        pthread_create(&thread_handles[thread], NULL, program, (void*) thread);

    }

    //join threads
    for(thread =0 ; thread< thread_count; thread++){
        pthread_join(thread_handles[thread], NULL);
    }

    //get the time taken for a operation
    t = clock() - t;
    double time_taken = ((double) t) / CLOCKS_PER_SEC;

    //destroy the mutex lock
    pthread_mutex_destroy(&mutex);
    Free_list(&head_p);
    free(thread_handles);

    return time_taken;

}
