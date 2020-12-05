#include "helping_functions.hpp"
#include "semaphores_sheared_mem.hpp"
#include "md5/md5.h"
using namespace std;

int ENC(char* read_shared_mem_key_file,int recive_shared_mem_size_file,char* write_shared_mem_key_file,int write_shared_mem_size_file,char* write_semaphore,char* read_semaphore);
int ENC_job(int ENC_type,char* read_shared_mem_key_file,int read_shared_mem_size_file);
int resend_message(char* shared_mem_key_file,int shared_mem_size_file);

int main(int argc, char const *argv[]){
    //cout << endl << CHAN_semaphore_p1_key_file << " "<< ENC2_semaphore_p1_key_file <<endl;
    int resend_flag=0;

    int sem_ENC2_p2_id = get_semaphore_id_from_file(ENC2_semaphore_p2_key_file);
    int sem_CHAN_resend_p1_id = get_semaphore_id_from_file(CHAN_semaphore_resend_p1_key_file);
    int sem_ENC2_resend_p1_id = get_semaphore_id_from_file(ENC2_semaphore_resend_p1_key_file);

    int sem_CHAN_p2_id = get_semaphore_id_from_file(CHAN_semaphore_p2_key_file);
    int sem_p2_p2_id = get_semaphore_id_from_file(P2_semaphore_p2_key_file);

    while(1){
        cout<<"\n\n";
        cout << "P1 message\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    #if DEBUG >= 1
        printf("~ENC2 %d CHAN ,%d\n", getpid(),sem_ENC2_p2_id);
    #endif
    semaphore_wait(sem_ENC2_p2_id);
    if(ENC_job(2,CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file)==-1){
        resend_flag=1;
        cout << " Wrong Checksum resend message\n Message arived: ";print_message_sh_mem(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file);cout <<" with checksum: ";print_message_checksum(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file);
        ENC_job(1,CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file);
        ENC(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file,CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file,CHAN_semaphore_p1_key_file,ENC2_semaphore_p1_key_file);
        cout << " Message Send: ";print_message_sh_mem(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file);cout <<" with checksum: ";print_message_checksum(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file);

        #if DEBUG >= 1
        printf("~ ENC2 %d releasing Resend %d\n", getpid(),sem_CHAN_p2_id);
        #endif
        semaphore_signal(sem_CHAN_p2_id);
        cout << "Waiting message back\n";
        #if DEBUG >= 1
            printf("~ENC2 %d waiting Resend MESSAGE BACK chan ,%d\n", getpid(),sem_ENC2_resend_p1_id);
        #endif
        semaphore_wait(sem_ENC2_resend_p1_id);

        cout << "Message arived: ";print_message_sh_mem(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file); cout <<" with checksum: ";print_message_checksum(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file);
        ENC_job(1,CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file);
        ENC(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file,ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file,P2_semaphore_p1_key_file,ENC2_semaphore_p1_key_file);
        cout << "Message send: ";print_message_sh_mem(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file); cout <<" with checksum: ";print_message_checksum(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file);
        #if DEBUG >= 1
            printf("~ ENC2 %d releasing %d\n", getpid(),sem_p2_p2_id);
        #endif
        semaphore_signal(sem_p2_p2_id);

    }
    if(resend_flag==0){
        cout << "Message arived: ";print_message_sh_mem(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file); cout <<" with checksum: ";print_message_checksum(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file);
        ENC_job(1,CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file);
        ENC(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file,ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file,P2_semaphore_p1_key_file,ENC2_semaphore_p1_key_file);
        cout << "Message send: ";print_message_sh_mem(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file); cout <<" with checksum: ";print_message_checksum(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file);
    }
    else
        resend_flag=0;

    #if DEBUG >= 1
        printf("~ ENC2 %d releasing %d\n", getpid(),sem_p2_p2_id);
    #endif
    semaphore_signal(sem_p2_p2_id);
    if(if_term(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file)==1){
        cout << "Exit program\n";
        return 0;
    }
    cout << "Waiting message back\n";
    #if DEBUG >= 1
        printf("~ENC2 %d waiting message back p2 %d\n", getpid(),sem_ENC2_p2_id);
    #endif
    semaphore_wait(sem_ENC2_p2_id);
    cout << "Message arived: ";print_message_sh_mem(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file); cout <<" with checksum: ";print_message_checksum(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file);
    ENC_job(1,ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file);
    ENC(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file,CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file,CHAN_semaphore_p1_key_file,ENC2_semaphore_p1_key_file);
    cout << "Message send: ";print_message_sh_mem(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file); cout <<" with checksum: ";print_message_checksum(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file);

    semaphore_signal(sem_CHAN_p2_id);
    #if DEBUG >= 1
        printf("~ CHAN %d releasing p2 %d\n", getpid(),sem_CHAN_p2_id);
    #endif

    cout<<"\n\n";
    cout << "P2 message\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    #if DEBUG >= 1
        printf("~CHAN %d waiting p3 %d\n", getpid(),sem_ENC2_p2_id);
    #endif
    semaphore_wait(sem_ENC2_p2_id);
    cout << "Message arived: ";print_message_sh_mem(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file); cout <<" with checksum: ";print_message_checksum(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file);
    ENC_job(1,ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file);
    ENC(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file,CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file,CHAN_semaphore_p1_key_file,ENC2_semaphore_p1_key_file);
    cout << "Message send: ";print_message_sh_mem(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file); cout <<" with checksum: ";print_message_checksum(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file);
    semaphore_signal(sem_CHAN_p2_id);
    #if DEBUG >= 1
        printf("~ CHAN %d releasing p2 %d\n", getpid(),sem_CHAN_p2_id);
    #endif
    if(if_term(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file)==1){
        cout <<"Exit programm \n";
        return 0;
    }
    cout << "Waiting message back\n";
    #if DEBUG >= 1
        printf("~ENC2 %d waiting message back ,%d\n", getpid(),sem_ENC2_p2_id);
    #endif
    semaphore_wait(sem_ENC2_p2_id);
    if(resend_message(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file)==1){
        resend_flag=1;
        #if DEBUG >= 2

        #endif
        cout<< "Resend message \n";
        cout << "Message arived: ";print_message_sh_mem(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file); cout <<" with checksum: ";print_message_checksum(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file);
        ENC_job(1,CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file);
        ENC(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file,ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file,P2_semaphore_p1_key_file,ENC2_semaphore_p1_key_file);
        cout << "Message send: ";print_message_sh_mem(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file); cout <<" with checksum: ";print_message_checksum(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file);

        semaphore_signal(sem_p2_p2_id);
        #if DEBUG >= 1
            printf("~ ENC %d releasing Resend %d\n", getpid(),sem_p2_p2_id);
        #endif
        cout << "Waiting message back\n";
        #if DEBUG >= 1
            printf("~ENC %d waiting recend MESSAGE BACK P ,%d\n", getpid(),sem_ENC2_resend_p1_id);
        #endif
        semaphore_wait(sem_ENC2_resend_p1_id);
        cout << "Message arived: ";print_message_sh_mem(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file); cout <<" with checksum: ";print_message_checksum(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file);
        ENC_job(1,ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file);
        ENC(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file,CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file,CHAN_semaphore_p1_key_file,ENC2_semaphore_p1_key_file);
        cout << "Message send: ";print_message_sh_mem(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file); cout <<" with checksum: ";print_message_checksum(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file);

        semaphore_signal(sem_CHAN_resend_p1_id);
        #if DEBUG >= 1
            printf("~ ENC %d releasing Resend %d\n", getpid(),sem_CHAN_resend_p1_id);
        #endif


    }
    if(resend_flag==1){
        cout << "Waiting message back\n";
        #if DEBUG >= 1
            printf("~ENC %d waiting message back ,%d\n", getpid(),sem_ENC2_p2_id);
        #endif
        semaphore_wait(sem_ENC2_p2_id);
        resend_flag=0;
    }
    cout << "Message arived: ";print_message_sh_mem(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file); cout <<" with checksum: ";print_message_checksum(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file);
    ENC_job(1,CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file);
    ENC(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file,ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file,P2_semaphore_p1_key_file,ENC2_semaphore_p1_key_file);
    cout << "Message send: ";print_message_sh_mem(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file); cout <<" with checksum: ";print_message_checksum(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file);

    #if DEBUG >= 1
        printf("~ CHAN %d releasing p2 %d\n", getpid(),sem_p2_p2_id);
    #endif
    semaphore_signal(sem_p2_p2_id);



    }
    return 0;
}

int ENC(char* read_shared_mem_key_file,int read_shared_mem_size_file,char* write_shared_mem_key_file,int write_shared_mem_size_file,char* write_semaphore,char* read_semaphore ){
    #if DEBUG >= 3
    cout<<"- ENC2 born now ID "<<getpid()<<endl;
    #endif
    //~~~~~~~~~~~~~~~~~~~semaphore~~~~~~~~~~~~~~~~~~~~~~~//
    int sem_read_id  = get_semaphore_id_from_file(read_semaphore);
    int sem_write_id  = get_semaphore_id_from_file(write_semaphore);

    #if DEBUG >= 1
    printf("~ENC2 %d waiting ,%d\n", getpid(),sem_read_id);
    #endif
    semaphore_wait(sem_read_id);

    //~~~~~~~~~~~~~~~~~~~memory~~~~~~~~~~~~~~~~~~~~~~~//
    /* Attach the shared memory segment. */
    int mem_seg_read_id=get_memory_id_from_file(read_shared_mem_key_file,read_shared_mem_size_file);
    message* shared_memory_read = (message*) shmat(mem_seg_read_id, NULL, 0);
    if(shared_memory_read==(void*)-1)die("shared memory ENC-read");
    #if DEBUG >= 2
        printf ("! shared memory attached at address %p\n", shared_memory_read);
    #endif

    //~~~~~~~~~~~~~~~~~READ FROM~~~~~~~~~~~~~~~~~~//
    message* mess = new message(shared_memory_read->message_arrey);
    mess->flag_checksum=shared_memory_read->flag_checksum;
    shared_memory_read->checksum[33]='\0';
    strcpy(mess->checksum,shared_memory_read->checksum);

    #if DEBUG >=3
        printf ("\t- shared memory read_ENC2"); shared_memory_read->print();
    #endif

    /* Detach the shared memory segment. */
    shmdt(shared_memory_read);
    #if DEBUG>= 2
        cout<<"\t"<<getpid()<<" detached memory read ENC\n";
    #endif







    //~~~~~~~~~~~~~~~~~WRITE~~~~~~~~~~~~~~~~~~//

    //~~~~~~~~~~~~~~~~~~~memory~~~~~~~~~~~~~~~~~~~~~~~//
    /* Attach the shared memory segment. */
    int mem_seg_write_id=get_memory_id_from_file(write_shared_mem_key_file,write_shared_mem_size_file);
    message* shared_memory_write = (message*) shmat(mem_seg_write_id, NULL, 0);
    if(shared_memory_write==(void*)-1)die("shared memory ENC-write");
    #if DEBUG >= 2
        printf ("! shared memory attached at address %p\n", shared_memory_write);
    #endif

    strcpy(shared_memory_write->message_arrey,mess->message_arrey);
    strcpy(shared_memory_write->checksum,mess->checksum);
    shared_memory_write->checksum[33]='\0';
    shared_memory_write->flag_checksum= mess->flag_checksum;
    #if DEBUG >= 3
        printf ("\t- shared memory write_ENC2"); shared_memory_write->print();
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
    return 0;

}
