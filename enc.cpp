#include "helping_functions.hpp"
#include "semaphores_sheared_mem.hpp"
#include "md5/md5.h"
using namespace std;

int ENC(char* read_shared_mem_key_file,int recive_shared_mem_size_file,char* write_shared_mem_key_file,int write_shared_mem_size_file,char* write_semaphore,char* read_semaphore);
int resend_message(char* shared_mem_key_file,int shared_mem_size_file);
int ENC_job(int ENC_type,char* read_shared_mem_key_file,int read_shared_mem_size_file);
int main(int argc, char const *argv[]){
    int sem_ENC_resend_p1_id = get_semaphore_id_from_file(ENC_semaphore_resend_p1_key_file);
    int sem_ENC_p2_id = get_semaphore_id_from_file(ENC_semaphore_p2_key_file);
    int sem_ENC_p3_id = get_semaphore_id_from_file(ENC_semaphore_p3_key_file);
    int sem_ENC_p4_id = get_semaphore_id_from_file(ENC_semaphore_p4_key_file);

    int sem_CHAN_resend_p1_id = get_semaphore_id_from_file(CHAN_semaphore_resend_p1_key_file);

    int sem_p_p2_id  = get_semaphore_id_from_file(P_semaphore_p2_key_file);
    int sem_p_p3_id  = get_semaphore_id_from_file(P_semaphore_p3_key_file);
    int sem_CHAN_p4_id = get_semaphore_id_from_file(CHAN_semaphore_p4_key_file);
    int sem_CHAN_p2_id = get_semaphore_id_from_file(CHAN_semaphore_p2_key_file);

    int resend_flag=0;

    while(1){
        cout<<"\n\n";
    cout << "P1 message\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    #if DEBUG >= 1
        printf("~ENC %d waiting P ,%d\n", getpid(),sem_ENC_p2_id);
    #endif
    semaphore_wait(sem_ENC_p2_id);
    cout << "Message arived: ";print_message_sh_mem(P_ENC_shared_mem_key_file,P_ENC_shared_mem_size_file); cout <<" with checksum: ";print_message_checksum(P_ENC_shared_mem_key_file,P_ENC_shared_mem_size_file);
    ENC_job(1,P_ENC_shared_mem_key_file,P_ENC_shared_mem_size_file);
    ENC(P_ENC_shared_mem_key_file,P_ENC_shared_mem_size_file,ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file,CHAN_semaphore_p1_key_file,ENC_semaphore_p1_key_file);
    cout << "Message send ";print_message_sh_mem(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file);cout <<" with checksum: ";print_message_checksum(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file);

    semaphore_signal(sem_CHAN_p2_id);
    #if DEBUG >= 1
        printf("~ %d releasing CHAN p2 %d\n", getpid(),sem_CHAN_p2_id);
    #endif
    if(if_term(P_ENC_shared_mem_key_file,P_ENC_shared_mem_size_file)==1){
        return 0;
    }
    cout << "Waiting message back\n";
    #if DEBUG >= 1
        printf("~ENC %d waiting message back ,%d\n", getpid(),sem_ENC_p2_id);
    #endif
    semaphore_wait(sem_ENC_p2_id);
    if(resend_message(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file)==1){
        //RESEND MESSAGE
        cout<< "Resend message \n";
        resend_flag=1;
        cout << "Message arived: ";print_message_sh_mem(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file); cout <<" with checksum: ";print_message_checksum(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file);
        ENC_job(1,ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file);
        ENC(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file,P_ENC_shared_mem_key_file,P_ENC_shared_mem_size_file,P_semaphore_p1_key_file,ENC_semaphore_p1_key_file);
        cout << "Message send ";print_message_sh_mem(P_ENC_shared_mem_key_file,P_ENC_shared_mem_size_file);cout <<" with checksum: ";print_message_checksum(P_ENC_shared_mem_key_file,P_ENC_shared_mem_size_file);

        semaphore_signal(sem_p_p2_id);
        #if DEBUG >= 1
            printf("~ ENC %d releasing Resend %d\n", getpid(),sem_p_p2_id);
        #endif

        cout << "Waiting message back\n";
        #if DEBUG >= 1
            printf("~ENC %d waiting recend MESSAGE BACK P ,%d\n", getpid(),sem_ENC_resend_p1_id);
        #endif
        semaphore_wait(sem_ENC_resend_p1_id);
        cout << "Message arived ";print_message_sh_mem(P_ENC_shared_mem_key_file,P_ENC_shared_mem_size_file);cout <<" with checksum: ";print_message_checksum(P_ENC_shared_mem_key_file,P_ENC_shared_mem_size_file);
        ENC_job(1,P_ENC_shared_mem_key_file,P_ENC_shared_mem_size_file);
        ENC(P_ENC_shared_mem_key_file,P_ENC_shared_mem_size_file,ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file,CHAN_semaphore_p1_key_file,ENC_semaphore_p1_key_file);
        cout << "Message send ";print_message_sh_mem(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file);cout <<" with checksum: ";print_message_checksum(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file);

        semaphore_signal(sem_CHAN_resend_p1_id);
        #if DEBUG >= 1
            printf("~ ENC %d releasing Resend %d\n", getpid(),sem_CHAN_resend_p1_id);
        #endif

    }
    if(resend_flag==1){
        cout << "Waiting message back\n";
        #if DEBUG >= 1
            printf("~ENC %d waiting message back ,%d\n", getpid(),sem_ENC_p2_id);
        #endif
        semaphore_wait(sem_ENC_p2_id);
        resend_flag=0;
    }
    cout << "Message arived ";print_message_sh_mem(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file);cout <<" with checksum: ";print_message_checksum(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file);
    ENC_job(1,ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file);
    ENC(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file,P_ENC_shared_mem_key_file,P_ENC_shared_mem_size_file,P_semaphore_p1_key_file,ENC_semaphore_p1_key_file);
    cout << "Message send: ";print_message_sh_mem(P_ENC_shared_mem_key_file,P_ENC_shared_mem_size_file);cout <<" with checksum: ";print_message_checksum(P_ENC_shared_mem_key_file,P_ENC_shared_mem_size_file);
    semaphore_signal(sem_p_p2_id);
    #if DEBUG >= 1
        printf("~ p %d releasing %d\n", getpid(),sem_p_p2_id);
    #endif


    cout<<"\n\n";
    cout << "P2 message\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

    #if DEBUG >= 1
        printf("~ENC1 %d waiting p3 %d\n", getpid(),sem_ENC_p3_id);
    #endif
    semaphore_wait(sem_ENC_p3_id);
    if(ENC_job(2,ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file)==-1){
        //RESEND MESSAGE

        cout << " Wrong Checksum resend message\n Message arived: ";print_message_sh_mem(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file);cout <<" with checksum: ";print_message_checksum(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file);
        ENC_job(1,ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file);
        ENC(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file,CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file,CHAN_semaphore_p1_key_file,ENC_semaphore_p1_key_file);
        cout << "Message send: ";print_message_sh_mem(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file);cout <<" with checksum: ";print_message_checksum(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file);

        #if DEBUG >= 1
        printf("~ ENC2 %d releasing Resend %d\n", getpid(),sem_CHAN_p4_id);
        #endif
        semaphore_signal(sem_CHAN_p4_id);
        cout << "Waiting message back\n";
        #if DEBUG >= 1
            printf("~ENC2 %d waiting Resend MESSAGE BACK chan ,%d\n", getpid(),sem_ENC_resend_p1_id);
        #endif
        semaphore_wait(sem_ENC_resend_p1_id);
        cout << "Message arived: ";print_message_sh_mem(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file);cout <<" with checksum: ";print_message_checksum(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file);
        ENC_job(1,ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file);
        ENC(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file,P_ENC_shared_mem_key_file,P_ENC_shared_mem_size_file,P_semaphore_p1_key_file,ENC_semaphore_p1_key_file);
        cout << "Message send: ";print_message_sh_mem(P_ENC_shared_mem_key_file,P_ENC_shared_mem_size_file);cout <<" with checksum: ";print_message_checksum(P_ENC_shared_mem_key_file,P_ENC_shared_mem_size_file);

        resend_flag=1;

    }
    if(resend_flag==0){
        cout << "Message arived: ";print_message_sh_mem(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file);cout <<" with checksum: ";print_message_checksum(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file);
        ENC_job(1,ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file);
        ENC(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file,P_ENC_shared_mem_key_file,P_ENC_shared_mem_size_file,P_semaphore_p1_key_file,ENC_semaphore_p1_key_file);
        cout << "Message send: ";print_message_sh_mem(P_ENC_shared_mem_key_file,P_ENC_shared_mem_size_file);cout <<" with checksum: ";print_message_checksum(P_ENC_shared_mem_key_file,P_ENC_shared_mem_size_file);
    }
    else
        resend_flag=0;

    semaphore_signal(sem_p_p3_id);
    #if DEBUG >= 1
        printf("~ p1 %d releasing p3 %d\n", getpid(),sem_p_p3_id);
    #endif
    if(if_term(P_ENC_shared_mem_key_file,P_ENC_shared_mem_size_file)==1){
        cout << "Exit program\n";
        return 0;
    }
    cout << "Waiting message back\n";
    #if DEBUG >= 1
        printf("~ENC %d waiting message back ,%d\n", getpid(),sem_ENC_p4_id);
    #endif
    semaphore_wait(sem_ENC_p4_id);
    cout << "Message arived: ";print_message_sh_mem(P_ENC_shared_mem_key_file,P_ENC_shared_mem_size_file);cout <<" with checksum: ";print_message_checksum(P_ENC_shared_mem_key_file,P_ENC_shared_mem_size_file);
    ENC_job(1,P_ENC_shared_mem_key_file,P_ENC_shared_mem_size_file);
    ENC(P_ENC_shared_mem_key_file,P_ENC_shared_mem_size_file,ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file,CHAN_semaphore_p1_key_file,ENC_semaphore_p1_key_file);
    cout << "Message send: ";print_message_sh_mem(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file);cout <<" with checksum: ";print_message_checksum(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file);

    semaphore_signal(sem_CHAN_p4_id);
    #if DEBUG >= 1
        printf("~ CHAN %d releasing p2 %d\n", getpid(),sem_CHAN_p4_id);
    #endif

    }
    return 0;
}

int ENC(char* read_shared_mem_key_file,int read_shared_mem_size_file,char* write_shared_mem_key_file,int write_shared_mem_size_file,char* write_semaphore,char* read_semaphore ){
    int resend_flag=0;
    #if DEBUG >= 3
    cout<<"- ENC born now ID "<<getpid()<<endl;
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
    #if DEBUG >= 2
        printf ("! shared memory attached at address %p\n", shared_memory_read);
    #endif

    //~~~~~~~~~~~~~~~~~READ FROM P~~~~~~~~~~~~~~~~~~//
    message* mess = new message(shared_memory_read->message_arrey);
    strcpy(mess->checksum,shared_memory_read->checksum);
    shared_memory_read->checksum[33]='\0';
    mess->flag_checksum=shared_memory_read->flag_checksum;

    #if DEBUG >= 0
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
    #if DEBUG >= 2
        printf ("! shared memory attached at address %p\n", shared_memory_write);
    #endif

    strcpy(shared_memory_write->message_arrey,mess->message_arrey);
    strcpy(shared_memory_write->checksum,mess->checksum);
    shared_memory_write->checksum[33]='\0';
    shared_memory_write->flag_checksum= mess->flag_checksum;
    #if DEBUG >= 0
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
