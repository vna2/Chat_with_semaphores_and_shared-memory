#include "helping_functions.hpp"
#include "semaphores_sheared_mem.hpp"
#include "md5/md5.h"
using namespace std;

int ENC(char* read_shared_mem_key_file,int recive_shared_mem_size_file,char* write_shared_mem_key_file,int write_shared_mem_size_file,char* write_semaphore,char* read_semaphore);
int ENC_job(int ENC_type,char* read_shared_mem_key_file,int read_shared_mem_size_file);
int main(int argc, char const *argv[]){
    //cout << endl << CHAN_semaphore_p1_key_file << " "<< ENC2_semaphore_p1_key_file <<endl;
    int resend_flag=0;

    int sem_ENC2_p1_id = get_semaphore_id_from_file(ENC2_semaphore_p1_key_file);
    int sem_ENC2_p2_id = get_semaphore_id_from_file(ENC2_semaphore_p2_key_file);
    int sem_ENC2_p3_id = get_semaphore_id_from_file(ENC2_semaphore_p3_key_file);
    int sem_ENC2_p4_id = get_semaphore_id_from_file(ENC2_semaphore_p4_key_file);
    int sem_CHAN_resend_p1_id = get_semaphore_id_from_file(CHAN_semaphore_resend_p1_key_file);
    int sem_ENC2_resend_p1_id = get_semaphore_id_from_file(ENC2_semaphore_resend_p1_key_file);

    int sem_CHAN_p2_id = get_semaphore_id_from_file(CHAN_semaphore_p2_key_file);
    int sem_CHAN_p3_id = get_semaphore_id_from_file(CHAN_semaphore_p3_key_file);
    int sem_p2_p2_id = get_semaphore_id_from_file(P2_semaphore_p2_key_file);

    while(1){
        cout<<"\n\n\n\n\n\n";
        cout<<"\n\n\n\n\n\n";
    #if DEBUG >= 1
        cout << "THIS IS P1 MESSAGE\n";
    #endif
    #if DEBUG >= 1
        printf("~ENC2 %d CHAN ,%d\n", getpid(),sem_ENC2_p2_id);
    #endif
    semaphore_wait(sem_ENC2_p2_id);
    if(ENC_job(2,CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file)==-1){
        #if DEBUG >= 2
        cout<< "RESEND MESSAGE \n";
        #endif
        // #if DEBUG >= 1
        //     printf("~ ENC2 %d releasing Resend %d\n", getpid(),sem_ENC2_p1_id);
        // #endif
        // semaphore_signal(sem_ENC2_p1_id);
        ENC(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file,CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file,CHAN_semaphore_p1_key_file,ENC2_semaphore_p1_key_file);
        #if DEBUG >= 1
        printf("~ ENC2 %d releasing Resend %d\n", getpid(),sem_CHAN_p2_id);
        #endif
        semaphore_signal(sem_CHAN_p2_id);
        // #if DEBUG >= 1
        //     printf("~ ENC2 %d releasing Resend %d\n", getpid(),sem_CHAN_resend_p1_id);
        // #endif
        // semaphore_signal(sem_CHAN_resend_p1_id);
        #if DEBUG >= 1
            printf("~ENC2 %d waiting Resend MESSAGE BACK chan ,%d\n", getpid(),sem_ENC2_resend_p1_id);
        #endif
        semaphore_wait(sem_ENC2_resend_p1_id);

        ENC(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file,ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file,P2_semaphore_p1_key_file,ENC2_semaphore_p1_key_file);
        resend_flag=1;
        #if DEBUG >= 1
            printf("~ ENC2 %d releasing %d\n", getpid(),sem_p2_p2_id);
        #endif
        semaphore_signal(sem_p2_p2_id);

    }
    if(resend_flag==0)
        ENC(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file,ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file,P2_semaphore_p1_key_file,ENC2_semaphore_p1_key_file);
        else

        resend_flag=0;


    #if DEBUG >= 1
        printf("~ ENC2 %d releasing %d\n", getpid(),sem_p2_p2_id);
    #endif
    semaphore_signal(sem_p2_p2_id);
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
    #if DEBUG >= 2
        cout << "THIS IS P2 MESSAGE\n";
    #endif
    semaphore_wait(sem_ENC2_p3_id);
    #if DEBUG >= 1
        printf("~CHAN %d waiting p3 %d\n", getpid(),sem_ENC2_p3_id);
    #endif
    ENC(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file,CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file,CHAN_semaphore_p1_key_file,ENC2_semaphore_p1_key_file);
    semaphore_signal(sem_CHAN_p3_id);
    #if DEBUG >= 1
        printf("~ CHAN %d releasing p2 %d\n", getpid(),sem_CHAN_p3_id);
    #endif

    #if DEBUG >= 1
        printf("~ENC2 %d waiting message back ,%d\n", getpid(),sem_ENC2_p4_id);
    #endif
    semaphore_wait(sem_ENC2_p4_id);
    ENC(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file,ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file,P2_semaphore_p1_key_file,ENC2_semaphore_p1_key_file);
    #if DEBUG >= 1
        printf("~ CHAN %d releasing p2 %d\n", getpid(),sem_p2_p2_id);
    #endif
    semaphore_signal(sem_p2_p2_id);

    cout<<"\n\n\n\n\n\n";
    cout<<"\n\n\n\n\n\n";

    }
    return 0;
}

int ENC(char* read_shared_mem_key_file,int read_shared_mem_size_file,char* write_shared_mem_key_file,int write_shared_mem_size_file,char* write_semaphore,char* read_semaphore ){
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


    //~~~~~~~~~~~~~~~~~READ FROM~~~~~~~~~~~~~~~~~~//
    message* mess = new message(shared_memory_read->message_arrey);
    mess->flag_checksum=shared_memory_read->flag_checksum;
    shared_memory_read->checksum[33]='\0';
    cout << "shared_memory_read->checksum "<<shared_memory_read->checksum<<endl;
    strcpy(mess->checksum,shared_memory_read->checksum);

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





    //~~~~~~~~~~~~~~~~~WRITE~~~~~~~~~~~~~~~~~~//

    //~~~~~~~~~~~~~~~~~~~memory~~~~~~~~~~~~~~~~~~~~~~~//
    /* Attach the shared memory segment. */
    int mem_seg_write_id=get_memory_id_from_file(write_shared_mem_key_file,write_shared_mem_size_file);
    message* shared_memory_write = (message*) shmat(mem_seg_write_id, NULL, 0);
    if(shared_memory_write==(void*)-1)die("shared memory ENC-write");


    strcpy(shared_memory_write->message_arrey,mess->message_arrey);
    strcpy(shared_memory_write->checksum,mess->checksum);
    shared_memory_write->checksum[33]='\0';
    shared_memory_write->flag_checksum= mess->flag_checksum;
    #if DEBUG >= 1
        printf ("\t- shared memory write_ENC"); shared_memory_write->print();
    #endif

    /* Detach the shared memory segment. */
    shmdt(shared_memory_write);
    #if DEBUG>= 1
        cout<<"\t"<<getpid()<<" detached memory ENC_CHAN\n";
    #endif
    semaphore_signal(sem_write_id);
    #if DEBUG >= 1
        printf("~ write ENC %d releasing %d\n", getpid(),sem_write_id);
    #endif
    //if
    return 0;

}

int resend_message(char* read_shared_mem_key_file,int read_shared_mem_size_file,char* write_shared_mem_key_file,int write_shared_mem_size_file,char* write_semaphore,char* read_semaphore ){
    //~~~~~~~~~~~~~~~~~~~memory~~~~~~~~~~~~~~~~~~~~~~~//
    /* Attach the shared memory segment. */
    int mem_seg_id=get_memory_id_from_file(read_shared_mem_key_file,read_shared_mem_size_file);
    message* shared_memory_read = (message*) shmat(mem_seg_id, NULL, 0);
    if(shared_memory_read==(void*)-1)die("shared memory ENC-write");



/* Detach the shared memory segment. */
    shmdt(shared_memory_read);
    #if DEBUG>= 2
        cout<<"\t"<<getpid()<<" detached memory ENC_CHAN\n";
    #endif
    return 0;
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
        cout << "ENC TYPE 1\n";
        strcpy(shared_memory_read->checksum,checksum);
    }
    else if(ENC_type==2){
        cout<< ".c_str: "<< md5(mess->message_arrey).c_str()<< " mess checksum: "<<mess->checksum<<endl;
        if(strcmp(md5(mess->message_arrey).c_str(),mess->checksum)!=0){
            #if DEBUG>= 2
            cout << "ENC TYPE 1";
            #endif
            #if DEBUG>= 2
            cout<<" different checksum try resend\n";
            #endif
            shared_memory_read->flag_checksum=1;
            resend_flag=-1;
        }
    }
    return resend_flag;
}
