#include "helping_functions.hpp"
#include "semaphores_sheared_mem.hpp"
#include "md5/md5.h"
using namespace std;

int ENC(char* read_shared_mem_key_file,int recive_shared_mem_size_file,char* write_shared_mem_key_file,int write_shared_mem_size_file,char* write_semaphore,char* read_semaphore);

int main(int argc, char const *argv[]){
    //cout << endl << CHAN_semaphore_p1_key_file << " "<< ENC2_semaphore_p1_key_file <<endl;
    int sem_ENC2_p2_id = get_semaphore_id_from_file(ENC2_semaphore_p2_key_file);
    int sem_CHAN_p2_id = get_semaphore_id_from_file(CHAN_semaphore_p2_key_file);
    int sem_ENC2_p3_id = get_semaphore_id_from_file(ENC2_semaphore_p3_key_file);
    int sem_CHAN_p3_id = get_semaphore_id_from_file(CHAN_semaphore_p3_key_file);
    int sem_P2_p3_id = get_semaphore_id_from_file(P2_semaphore_p3_key_file);




    while(1){
    #if DEBUG >= 1
        cout << "THIS IS P1 MESSAGE";
    #endif
    ENC(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file,ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file,P2_semaphore_p1_key_file,ENC2_semaphore_p1_key_file);
    cout<<"\n\n\n\n\n\n";
    #if DEBUG >= 1
        printf("~ENC2 %d waiting message back p2 %d\n", getpid(),sem_ENC2_p2_id);
    #endif
    semaphore_wait(sem_ENC2_p2_id);
    ENC(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file,CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file,CHAN_semaphore_p1_key_file,ENC2_semaphore_p1_key_file);
    semaphore_signal(sem_CHAN_p2_id);
    #if DEBUG >= 1
        printf("~ CHAN %d releasing p2 %d\n", getpid(),sem_CHAN_p2_id);
    #endif


    cout<<"\n\n\n\n\n\n";
    cout<<"\n\n\n\n\n\n";
    #if DEBUG >= 1
        cout << "THIS IS P2 MESSAGE\n";
    #endif
    #if DEBUG >= 1
        printf("~ENC2 %d waiting p3 %d\n", getpid(),sem_ENC2_p3_id);
    #endif
    semaphore_wait(sem_ENC2_p3_id);
    ENC(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file,CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file,CHAN_semaphore_p1_key_file,ENC2_semaphore_p1_key_file);
    semaphore_signal(sem_CHAN_p3_id);
    #if DEBUG >= 1
        printf("~ CHAN %d releasing p2 %d\n", getpid(),sem_CHAN_p3_id);
    #endif
    // #if DEBUG >= 1
    //     printf("~ENC2 %d waiting message back ,%d\n", getpid(),sem_ENC2_p2_id);
    // #endif
    // semaphore_wait(sem_ENC2_p2_id);


    }
    return 0;
}

int ENC(char* read_shared_mem_key_file,int read_shared_mem_size_file,char* write_shared_mem_key_file,int write_shared_mem_size_file,char* write_semaphore,char* read_semaphore ){
    int resend_flag=0;
    #if DEBUG >= 1
    cout<<"- ENC born now ID "<<getpid()<<" with parent  "<<getppid()<<endl;
    #endif
    //~~~~~~~~~~~~~~~~~~~semaphore~~~~~~~~~~~~~~~~~~~~~~~//
    int sem_read_id  = get_semaphore_id_from_file(read_semaphore);
    int sem_write_id  = get_semaphore_id_from_file(write_semaphore);
    cout << endl<< read_semaphore <<endl;

    #if DEBUG >= 1
    printf("~ENC %d waiting ,%d\n", getpid(),sem_read_id);
    #endif
    semaphore_wait(sem_read_id);

    //~~~~~~~~~~~~~~~~~~~memory~~~~~~~~~~~~~~~~~~~~~~~//
    /* Attach the shared memory segment. */
    int mem_seg_read_id=get_memory_id_from_file(read_shared_mem_key_file,read_shared_mem_size_file);
    message* shared_memory_read = (message*) shmat(mem_seg_read_id, NULL, 0);
    if(shared_memory_read==(void*)-1)die("shared memory ENC-read");


    //~~~~~~~~~~~~~~~~~READ FROM P~~~~~~~~~~~~~~~~~~//
    message* mess = new message(shared_memory_read->message_arrey);
    mess->flag_checksum=shared_memory_read->flag_checksum;
    #if DEBUG >= 1
        printf("~ read ENC %d \n", getpid());
        printf ("\t- shared memory read_ENC"); shared_memory_read->print();
    #endif

    /* Detach the shared memory segment. */
    shmdt(shared_memory_read);
    #if DEBUG>= 2
        cout<<"\t"<<getpid()<<" detached memory read ENC\n";
    #endif

    //~~~~~~~~~~~~~~~~~~~~ENC-JOB~~~~~~~~~~~~~~~~~~~~~//

    char* checksum = new char[1000];
    strcpy(checksum,md5(mess->message_arrey).c_str());
    if(mess->flag_checksum==-1){
        cout << "FIRST IF \n";
        strcpy(mess->checksum,checksum);
        mess->flag_checksum=0;

    }
    else if(strcmp(md5(mess->message_arrey).c_str(),mess->checksum)!=0){
        #if DEBUG>= 2
        cout<<" different checksum try resend\n";
        #endif
        cout << "SECOND IF \n";
        resend_flag=-1;
    }
    else if(mess->flag_checksum==0){
        cout << "THIRD IF \n";
        strcpy(mess->checksum,checksum);
    }




    //~~~~~~~~~~~~~~~~~WRITE~~~~~~~~~~~~~~~~~~//

    //~~~~~~~~~~~~~~~~~~~memory~~~~~~~~~~~~~~~~~~~~~~~//
    /* Attach the shared memory segment. */
    int mem_seg_write_id=get_memory_id_from_file(write_shared_mem_key_file,write_shared_mem_size_file);
    message* shared_memory_write = (message*) shmat(mem_seg_write_id, NULL, 0);
    if(shared_memory_write==(void*)-1)die("shared memory ENC-write");


    strcpy(shared_memory_write->message_arrey,mess->message_arrey);
    strcpy(shared_memory_write->checksum,mess->checksum);
    shared_memory_write->flag_checksum= mess->flag_checksum;
    #if DEBUG >= 1
        printf ("\t- shared memory write_ENC"); shared_memory_write->print();
    #endif

    /* Detach the shared memory segment. */
    shmdt(shared_memory_write);
    #if DEBUG>= 2
        cout<<"\t"<<getpid()<<" detached memory ENC_CHAN\n";
    #endif
    semaphore_signal(sem_write_id);
    #if DEBUG >= 1
        printf("~ write ENC %d releasing %d\n", getpid(),sem_write_id);
    #endif
    //if
    return resend_flag;


}
