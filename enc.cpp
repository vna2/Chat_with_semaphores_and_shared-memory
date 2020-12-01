#include "helping_functions.hpp"
#include "semaphores_sheared_mem.hpp"
#include "md5/md5.h"
using namespace std;

int ENC(char* read_shared_mem_key_file,int recive_shared_mem_size_file,char* write_shared_mem_key_file,int write_shared_mem_size_file,char* write_semaphore,char* read_semaphore);
int resend_message(char* shared_mem_key_file,int shared_mem_size_file);
int ENC_job(int ENC_type,char* read_shared_mem_key_file,int read_shared_mem_size_file);

int main(int argc, char const *argv[]){
    int sem_ENC_resend_p1_id = get_semaphore_id_from_file(ENC_semaphore_resend_p1_key_file);
    int sem_ENC_p1_id = get_semaphore_id_from_file(ENC_semaphore_p1_key_file);
    int sem_ENC_p2_id = get_semaphore_id_from_file(ENC_semaphore_p2_key_file);
    int sem_ENC_p3_id = get_semaphore_id_from_file(ENC_semaphore_p3_key_file);
    int sem_ENC_p4_id = get_semaphore_id_from_file(ENC_semaphore_p4_key_file);
    int sem_p_resend_p1_id = get_semaphore_id_from_file(P_semaphore_resend_p1_key_file);

    int sem_CHAN_resend_p1_id = get_semaphore_id_from_file(CHAN_semaphore_resend_p1_key_file);

    int sem_p_p2_id  = get_semaphore_id_from_file(P_semaphore_p2_key_file);
    int sem_p_p3_id  = get_semaphore_id_from_file(P_semaphore_p3_key_file);
    int sem_CHAN_p4_id = get_semaphore_id_from_file(CHAN_semaphore_p4_key_file);
    int sem_CHAN_p2_id = get_semaphore_id_from_file(CHAN_semaphore_p2_key_file);
    int sem_CHAN_p3_id = get_semaphore_id_from_file(CHAN_semaphore_p3_key_file);

    int resend_flag=0;

    while(1){
        cout<<"\n\n\n\n\n\n";
        cout<<"\n\n\n\n\n\n";
    #if DEBUG >= 2
        cout << "THIS IS P1 MESSAGE\n";
    #endif
    #if DEBUG >= 1
        printf("~ENC %d waiting P ,%d\n", getpid(),sem_ENC_p2_id);
    #endif
    semaphore_wait(sem_ENC_p2_id);
    ENC_job(1,P_ENC_shared_mem_key_file,P_ENC_shared_mem_size_file);
    ENC(P_ENC_shared_mem_key_file,P_ENC_shared_mem_size_file,ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file,CHAN_semaphore_p1_key_file,ENC_semaphore_p1_key_file);
    #if DEBUG >= 1
        printf("~ %d releasing CHAN p2 %d\n", getpid(),sem_CHAN_p2_id);
    #endif
    semaphore_signal(sem_CHAN_p2_id);
    #if DEBUG >= 1
        printf("~ENC %d waiting message back ,%d\n", getpid(),sem_ENC_p2_id);
    #endif
    semaphore_wait(sem_ENC_p2_id);
    if(resend_message(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file)==1){
        #if DEBUG >= 2
        cout<< "RESEND MESSAGE \n";
        #endif

        resend_flag=1;

        ENC(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file,P_ENC_shared_mem_key_file,P_ENC_shared_mem_size_file,P_semaphore_p1_key_file,ENC_semaphore_p1_key_file);
        semaphore_signal(sem_p_p2_id);
        #if DEBUG >= 1
            printf("~ ENC %d releasing Resend %d\n", getpid(),sem_p_p2_id);
        #endif

        #if DEBUG >= 1
            printf("~ENC %d waiting recend MESSAGE BACK P ,%d\n", getpid(),sem_ENC_resend_p1_id);
        #endif
        semaphore_wait(sem_ENC_resend_p1_id);
        ENC(P_ENC_shared_mem_key_file,P_ENC_shared_mem_size_file,ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file,CHAN_semaphore_p1_key_file,ENC_semaphore_p1_key_file);
        semaphore_signal(sem_CHAN_resend_p1_id);
        #if DEBUG >= 1
            printf("~ ENC %d releasing Resend %d\n", getpid(),sem_CHAN_resend_p1_id);
        #endif

    }
    cout<<"\n\n\n\n\n\n";
    if(resend_flag==1){
        #if DEBUG >= 1
            printf("~ENC %d waiting message back ,%d\n", getpid(),sem_ENC_p2_id);
        #endif
        semaphore_wait(sem_ENC_p2_id);
        resend_flag=0;
    }
    ENC_job(1,ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file);
    ENC(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file,P_ENC_shared_mem_key_file,P_ENC_shared_mem_size_file,P_semaphore_p1_key_file,ENC_semaphore_p1_key_file);
    semaphore_signal(sem_p_p2_id);
    #if DEBUG >= 1
        printf("~ p %d releasing %d\n", getpid(),sem_p_p2_id);
    #endif


    cout<<"\n\n\n\n\n\n";
    cout<<"\n\n\n\n\n\n";
    #if DEBUG >= 2
        cout << "THIS IS P2 MESSAGE\n";
    #endif
    #if DEBUG >= 1
        printf("~ENC1 %d waiting p3 %d\n", getpid(),sem_ENC_p3_id);
    #endif
    semaphore_wait(sem_ENC_p3_id);
    if(ENC_job(2,ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file)==-1){
        #if DEBUG >= 2
        cout<< "RESEND MESSAGE \n";
        #endif
        ENC(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file,CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file,CHAN_semaphore_p1_key_file,ENC_semaphore_p1_key_file);
        #if DEBUG >= 1
        printf("~ ENC2 %d releasing Resend %d\n", getpid(),sem_CHAN_p4_id);
        #endif
        semaphore_signal(sem_CHAN_p4_id);
        #if DEBUG >= 1
            printf("~ENC2 %d waiting Resend MESSAGE BACK chan ,%d\n", getpid(),sem_ENC_resend_p1_id);
        #endif
        semaphore_wait(sem_ENC_resend_p1_id);
        ENC(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file,P_ENC_shared_mem_key_file,P_ENC_shared_mem_size_file,P_semaphore_p1_key_file,ENC_semaphore_p1_key_file);
        #if DEBUG >= 1
            printf("~ ENC2 %d releasing %d\n", getpid(),sem_p_p3_id);
        #endif
        resend_flag=1;
        semaphore_signal(sem_p_p3_id);
    }
    if(resend_flag==0)
        ENC(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file,P_ENC_shared_mem_key_file,P_ENC_shared_mem_size_file,P_semaphore_p1_key_file,ENC_semaphore_p1_key_file);
    else
        resend_flag=0;

    semaphore_signal(sem_p_p3_id);
    #if DEBUG >= 1
        printf("~ p1 %d releasing p3 %d\n", getpid(),sem_p_p3_id);
    #endif
    #if DEBUG >= 1
        printf("~ENC %d waiting message back ,%d\n", getpid(),sem_ENC_p4_id);
    #endif
    semaphore_wait(sem_ENC_p4_id);
    ENC(P_ENC_shared_mem_key_file,P_ENC_shared_mem_size_file,ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file,CHAN_semaphore_p1_key_file,ENC_semaphore_p1_key_file);
    semaphore_signal(sem_CHAN_p4_id);
    #if DEBUG >= 1
        printf("~ CHAN %d releasing p2 %d\n", getpid(),sem_CHAN_p4_id);
    #endif
    cout<<"\n\n\n\n\n\n";
    cout<<"\n\n\n\n\n\n";
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
    strcpy(mess->checksum,shared_memory_read->checksum);
    shared_memory_read->checksum[33]='\0';
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
        cout << "ENC TYPE 1";
        strcpy(shared_memory_read->checksum,checksum);
    }
    else if(ENC_type==2){
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
