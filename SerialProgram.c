#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <sys/time.h>


//node definition
struct list_node_s{
    int data;
    struct list_node_s* next;
};


//method definitions

int Insert(int value, struct list_node_s** head_pp);

int Member(int value, struct list_node_s* head_p);

int Delete (int value, struct list_node_s** head_pp);

void GenerateLinkedList(int n);

int GenerateRandomNumber();

void Free_list(struct list_node_s** head_pp);

struct list_node_s* head_p = NULL;

//input argument definition
int n;
int m;
int b;
float m_member;
float m_insert;
float m_delete;

int main(int argc, char *argv[]) {

    //obtaining inputs from the command line arguments
    n =  (int) strtol(argv[1], NULL, 10);
    m =  (int) strtol(argv[2], NULL, 10);
    m_member = strtof(argv[3], NULL);
    m_insert= strtof(argv[4], NULL);
    m_delete = strtof(argv[5], NULL);
    b=(int) strtol(argv[6],NULL,10);

    srand(time(0));
    int a;
    double mean_sum = 0;
    double s_deviation_sum = 0;

    for( a = 0; a < b; a = a + 1 ) {

        GenerateLinkedList(n);

        //Total number operations for each operation type
        int numOfMembers = m * m_member ;
        int numOfInserts = m * m_insert ;
        int numOfDeletions = m * m_delete ;

        //counters to keep track of operations
        int member_counter=0;
        int insert_counter =0;
        int delete_counter=0;

        int operation_counter =0;


        //time initialization
        clock_t t;
        t = clock();

        while (operation_counter != m) {

            // generate random number for the operation
            int number = rand() % 65536;

            //read lock for the member operation
            int operation_index = GenerateRandomNumber();
            if (operation_index == 1 & member_counter != numOfMembers) {
                int result = Member(number, head_p);
                member_counter += 1;
                operation_counter += 1;

            } else if (operation_index == 2 & insert_counter != numOfInserts) {
                int result = Insert(number, &head_p);
                operation_counter += 1;
                insert_counter += 1;

            } else if (operation_index == 3 & delete_counter != numOfDeletions) {
                int result = Delete(number, &head_p);
                operation_counter += 1;
                delete_counter += 1;
            }
        }

        t = clock() - t;
        double time_taken = ((double) t) / CLOCKS_PER_SEC; // in seconds

        Free_list(&head_p);
        mean_sum = mean_sum + time_taken;
        s_deviation_sum = s_deviation_sum + time_taken*time_taken;


    }
    double mean = mean_sum/b;
    double std = sqrt(s_deviation_sum/ b -mean*mean);
    printf("mean-------- %lf\n",mean);
    printf("std -------- %lf\n", std);

    return 0;

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

//Delete function implementation for linked list
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

//Insert function implementation for linked list
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

