#ifndef HELPING_FUNCTION_INCLUDED
#define HELPING_FUNCTION_INCLUDED

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <limits>

#include "semaphores_sheared_mem.hpp"
#include "md5/md5.h"


#define BUFFER_MESSEGE_LENGTH  500
#define NUM_OF_MESSEGES 100
#define DEBUG -1
using namespace std;

class message{
  public:
    //int message_length ;
    char message_arrey[BUFFER_MESSEGE_LENGTH];
    char checksum[BUFFER_MESSEGE_LENGTH];
    int flag_checksum;//0 if not changed 1 if changed
    message(){}
    message(char* message){
        //this->message_length = strlen(message);
        strcpy(message_arrey,message);
        flag_checksum=-1;
        //cout << message_arrey << endl;
        #if DEBUG>= 3
            cout << "!! Constractor message end his job \n";
        #endif
    }
    void print(){
        cout << "\n\n ~~~~~~~~~~~~~~~message print~~~~~~~~~~~~~~~~~\n";
        cout << "\t message: "<< message_arrey << '\n' <<"\t checksum:  " << checksum  << endl << endl;

    }
};

int if_term(char* shared_mem_key_file,int shared_mem_size_file){
    int term_flag=0;

    //~~~~~~~~~~~~~~~~~~~memory~~~~~~~~~~~~~~~~~~~~~~~//
    /* Attach the shared memory segment. */
    int mem_seg_id=get_memory_id_from_file(shared_mem_key_file,shared_mem_size_file);
    message* shared_memory = (message*) shmat(mem_seg_id, NULL, 0);
    if(shared_memory==(void*)-1)die("shared memory ENC-write");

    if(strcmp(shared_memory->message_arrey,"TERM")==0){
        term_flag=1;

    }

/* Detach the shared memory segment. */
    shmdt(shared_memory);
    #if DEBUG>= 2
        cout<<"\t"<<getpid()<<" detached memory ENC_CHAN\n";
    #endif
    return term_flag;
}

int resend_message(char* shared_mem_key_file,int shared_mem_size_file){
    int resend_flag=0;

    //~~~~~~~~~~~~~~~~~~~memory~~~~~~~~~~~~~~~~~~~~~~~//
    /* Attach the shared memory segment. */
    int mem_seg_id=get_memory_id_from_file(shared_mem_key_file,shared_mem_size_file);
    message* shared_memory = (message*) shmat(mem_seg_id, NULL, 0);
    if(shared_memory==(void*)-1)die("shared memory ENC-write");

    if(shared_memory->flag_checksum==1){
        resend_flag=1;

    }

/* Detach the shared memory segment. */
    shmdt(shared_memory);
    #if DEBUG>= 2
        cout<<"\t"<<getpid()<<" detached memory ENC_CHAN\n";
    #endif
    return resend_flag;
}

int ENC_job(int ENC_type,char* read_shared_mem_key_file,int read_shared_mem_size_file){
    int resend_flag=0;

    int mem_seg_read_id=get_memory_id_from_file(read_shared_mem_key_file,read_shared_mem_size_file);
    message* shared_memory_read = (message*) shmat(mem_seg_read_id, NULL, 0);
    if(shared_memory_read==(void*)-1)die("shared memory ENC-read");

    message* mess = new message(shared_memory_read->message_arrey);
    strcpy(mess->checksum,shared_memory_read->checksum);
    shared_memory_read->checksum[33]='\0';
    mess->flag_checksum=shared_memory_read->flag_checksum;

    char* checksum = new char[1000];
    strcpy(checksum,md5(mess->message_arrey).c_str());
    if(ENC_type==1){
        if(strcmp(shared_memory_read->message_arrey,"TERM")==0){
            #if DEBUG>= 3
            cout << "\nTERM\n";
            #endif
        }
        #if DEBUG>= 3
        cout << "ENC TYPE 1\n";
        #endif
        strcpy(shared_memory_read->checksum,checksum);
    }
    else if(ENC_type==2){
        if(strcmp(shared_memory_read->message_arrey,"TERM")==0){
            #if DEBUG>= 3
            cout << "\nTERM\n";
            #endif
        }
        else if(strcmp(md5(mess->message_arrey).c_str(),mess->checksum)!=0){
            #if DEBUG>= 3
            cout << "ENC TYPE 1";
            #endif
            #if DEBUG>= 3
            cout<<" different checksum try resend\n";
            #endif
            shared_memory_read->flag_checksum=1;
            resend_flag=-1;
        }
    }
    delete checksum;
    return resend_flag;
}

void print_message_sh_mem(char* shared_mem_key_file,int shared_mem_size_file){
    //~~~~~~~~~~~~~~~~~~~memory~~~~~~~~~~~~~~~~~~~~~~~//
    /* Attach the shared memory segment. */
    int mem_seg_id=get_memory_id_from_file(shared_mem_key_file,shared_mem_size_file);
    message* shared_memory = (message*) shmat(mem_seg_id, NULL, 0);
    if(shared_memory==(void*)-1)die("shared memory ENC-write");

    cout << shared_memory->message_arrey<<endl;

    /* Detach the shared memory segment. */
    shmdt(shared_memory);
    #if DEBUG>= 2
        cout<<"\t"<<getpid()<<" detached memory ENC_CHAN\n";
    #endif
}
void print_message_checksum(char* shared_mem_key_file,int shared_mem_size_file){
    //~~~~~~~~~~~~~~~~~~~memory~~~~~~~~~~~~~~~~~~~~~~~//
    /* Attach the shared memory segment. */
    int mem_seg_id=get_memory_id_from_file(shared_mem_key_file,shared_mem_size_file);
    message* shared_memory = (message*) shmat(mem_seg_id, NULL, 0);
    if(shared_memory==(void*)-1)die("shared memory ENC-write");

    cout << shared_memory->checksum<<endl;

    /* Detach the shared memory segment. */
    shmdt(shared_memory);
    #if DEBUG>= 2
        cout<<"\t"<<getpid()<<" detached memory ENC_CHAN\n";
    #endif
}

void delete_semaphores_and_sheared_mem(){
    int sem_p_p1_id = get_semaphore_id_from_file(P_semaphore_p1_key_file);
    int sem_p_p2_id = get_semaphore_id_from_file(P_semaphore_p2_key_file);
    int sem_p_p3_id = get_semaphore_id_from_file(P_semaphore_p3_key_file);
    int sem_p_p4_id = get_semaphore_id_from_file(P_semaphore_p4_key_file);

    int sem_ENC_resend_p1_id = get_semaphore_id_from_file(ENC_semaphore_resend_p1_key_file);
    int sem_ENC_p1_id = get_semaphore_id_from_file(ENC_semaphore_p1_key_file);
    int sem_ENC_p2_id = get_semaphore_id_from_file(ENC_semaphore_p2_key_file);
    int sem_ENC_p3_id = get_semaphore_id_from_file(ENC_semaphore_p3_key_file);
    int sem_ENC_p4_id = get_semaphore_id_from_file(ENC_semaphore_p4_key_file);

    int sem_CHAN_resend_p1_id = get_semaphore_id_from_file(CHAN_semaphore_resend_p1_key_file);
    int sem_CHAN_p1_id = get_semaphore_id_from_file(CHAN_semaphore_p1_key_file);
    int sem_CHAN_p2_id = get_semaphore_id_from_file(CHAN_semaphore_p2_key_file);
    int sem_CHAN_p4_id = get_semaphore_id_from_file(CHAN_semaphore_p4_key_file);
    int sem_CHAN_p3_id = get_semaphore_id_from_file(CHAN_semaphore_p3_key_file);

    int sem_ENC2_resend_p1_id = get_semaphore_id_from_file(ENC2_semaphore_resend_p1_key_file);
    int sem_ENC2_p1_id = get_semaphore_id_from_file(ENC2_semaphore_p1_key_file);
    int sem_ENC2_p2_id = get_semaphore_id_from_file(ENC2_semaphore_p2_key_file);
    int sem_ENC2_p3_id = get_semaphore_id_from_file(ENC2_semaphore_p3_key_file);
    int sem_ENC2_p4_id = get_semaphore_id_from_file(ENC2_semaphore_p4_key_file);

    int sem_p2_p1_id = get_semaphore_id_from_file(P2_semaphore_p1_key_file);
    int sem_p2_p2_id = get_semaphore_id_from_file(P2_semaphore_p2_key_file);
    int sem_p2_p4_id = get_semaphore_id_from_file(P2_semaphore_p4_key_file);


    clear_sem(P_semaphore_p1_key_file);
    clear_sem(P_semaphore_p2_key_file);
    clear_sem(P_semaphore_p3_key_file);
    clear_sem(P_semaphore_p4_key_file);

    clear_sem(ENC_semaphore_p1_key_file);
    clear_sem(ENC_semaphore_p2_key_file);
    clear_sem(ENC_semaphore_p3_key_file);
    clear_sem(ENC_semaphore_p4_key_file);
    clear_sem(ENC_semaphore_resend_p1_key_file);

    clear_sem(CHAN_semaphore_p1_key_file);
    clear_sem(CHAN_semaphore_p2_key_file);
    clear_sem(CHAN_semaphore_p3_key_file);
    clear_sem(CHAN_semaphore_p4_key_file);
    clear_sem(CHAN_semaphore_resend_p1_key_file);

    clear_sem(ENC2_semaphore_p1_key_file);
    clear_sem(ENC2_semaphore_p2_key_file);
    clear_sem(ENC2_semaphore_p3_key_file);
    clear_sem(ENC2_semaphore_p4_key_file);
    clear_sem(ENC2_semaphore_resend_p1_key_file);

    clear_sem(P2_semaphore_p1_key_file);
    clear_sem(P2_semaphore_p2_key_file);
    clear_sem(P2_semaphore_p4_key_file);

    clear_mem(P_shared_mem_key_file, P_shared_mem_size_file);
    clear_mem(P_ENC_shared_mem_key_file, P_ENC_shared_mem_size_file);
    clear_mem(ENC_CHAN_shared_mem_key_file, ENC_CHAN_shared_mem_size_file);
    clear_mem(CHAN_ENC_shared_mem_key_file, CHAN_ENC_shared_mem_size_file);
    clear_mem(ENC_P2_shared_mem_key_file, ENC_P2_shared_mem_size_file);
    clear_mem(P2_shared_mem_key_file, P2_shared_mem_size_file);

}

#endif
