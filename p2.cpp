#include "helping_functions.hpp"
#include "semaphores_sheared_mem.hpp"
#include "md5/md5.h"

int P(char* read_shared_mem_key_file,int read_shared_mem_size_file,char* write_shared_mem_key_file, int write_shared_mem_size_file,char* write_semaphore,char* read_semaphore);
int resend_message(char* shared_mem_key_file,int shared_mem_size_file);

int main(int argc, char const *argv[]) {
    int resend_flag=0;

    char* temp[5];
    int status =0;
    for (size_t i = 0; i < 5; i++) {
        temp[i] = new char;
    }
    strcpy(temp[0], "P2-hola bitch");
    strcpy(temp[1], "P2-hola bitch1");//
    strcpy(temp[2], "P2-hola bitch2");
    strcpy(temp[3], "TERM");

    int sem_ENC2_p2_id = get_semaphore_id_from_file(ENC2_semaphore_p2_key_file);
    int sem_p2_p1_id = get_semaphore_id_from_file(P2_semaphore_p1_key_file);
    int sem_p2_p2_id = get_semaphore_id_from_file(P2_semaphore_p2_key_file);
    int sem_p2_p4_id = get_semaphore_id_from_file(P2_semaphore_p4_key_file);
    int sem_p1_p4_id = get_semaphore_id_from_file(P_semaphore_p4_key_file);

    int sem_ENC2_p3_id = get_semaphore_id_from_file(ENC2_semaphore_p3_key_file);
    int sem_ENC2_resend_p1_id = get_semaphore_id_from_file(ENC2_semaphore_resend_p1_key_file);

    #if DEBUG >= 1
    printf("~P %d waiting from p2 %d\n", getpid(),sem_p2_p2_id);
    #endif
    semaphore_wait(sem_p2_p2_id);
    cout<<"\n\n";
    cout << "P1 message\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    if(if_term(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file)==1){
        cout << "Exit program\n";
        delete_semaphores_and_sheared_mem();
        return 0;
    }
    cout << "Message arived : ";print_message_sh_mem(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file); cout <<" with checksum: ";print_message_checksum(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file);
    P(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file,ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file,ENC2_semaphore_p1_key_file,P2_semaphore_p1_key_file);
    cout << "Message Send : ";print_message_sh_mem(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file); cout <<" with checksum: ";print_message_checksum(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file);

    semaphore_signal(sem_ENC2_p2_id);
    #if DEBUG >= 1
        printf("~ ENC2 %d releasing %d\n", getpid(),sem_ENC2_p2_id);
    #endif

    #if DEBUG >= 1
    printf("~P %d waiting from P1 to start sending message %d\n", getpid(),sem_p2_p4_id);
    #endif
    semaphore_wait(sem_p2_p4_id);
    for (size_t i = 0; i <4; i++) {
        cout<<"\n\n";
        cout << "P2 message\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        int mem_seg_id=get_memory_id_from_file(P2_shared_mem_key_file,P2_shared_mem_size_file);
        message* shared_memory = (message*) shmat(mem_seg_id, NULL, 0);
        if(shared_memory==(void*)-1)die("shared memory P");
        #if DEBUG >= 2
            printf ("! shared memory attached at address %p\n", shared_memory);
        #endif
        strcpy(shared_memory->message_arrey,temp[i]);
        //~~~~~~~~~~~~~~~~~~~clears~~~~~~~~~~~~~~~~~~~~~~~//
        /* Detach the shared memory segment. */
        shmdt(shared_memory);
        #if DEBUG>= 1
            cout<<"\t"<<getpid()<<" detached memory P-ENC\n";
        #endif
        semaphore_signal(sem_p2_p1_id);
        #if DEBUG >= 1
            printf("~ P2 %d releasing p1 %d\n", getpid(),sem_p2_p1_id);
        #endif
        P(P2_shared_mem_key_file,P2_shared_mem_size_file,ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file,ENC2_semaphore_p1_key_file,P2_semaphore_p1_key_file);
        cout << "Message send: ";print_message_sh_mem(P2_shared_mem_key_file,P_shared_mem_size_file);
        semaphore_signal(sem_ENC2_p3_id);
        #if DEBUG >= 1
            printf("~ P2 %d releasing p3 %d\n", getpid(),sem_ENC2_p3_id);
        #endif
        if(strcmp(temp[i],"TERM")==0){
            cout<< "Exit program\n";
            return 1;
        }
        cout << "Waiting message back\n";
        #if DEBUG >= 1
            printf("~P %d waiting message back p2 %d\n", getpid(),sem_p2_p2_id);
        #endif
        semaphore_wait(sem_p2_p2_id);
        if(resend_message(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file)==1){
            resend_flag=1;
            mem_seg_id=get_memory_id_from_file(P2_shared_mem_key_file,P2_shared_mem_size_file);
            message* shared_memory_resend = (message*) shmat(mem_seg_id, NULL, 0);
            if(shared_memory_resend==(void*)-1)die("shared memory P");
            #if DEBUG >= 2
                printf ("! shared memory attached at address %p\n", shared_memory_resend);
            #endif
            strcpy(shared_memory_resend->message_arrey,temp[i]);
            //~~~~~~~~~~~~~~~~~~~clears~~~~~~~~~~~~~~~~~~~~~~~//
            /* Detach the shared memory segment. */
            shmdt(shared_memory_resend);
            #if DEBUG>= 2
                cout<<"\t"<<getpid()<<" detached memory P-ENC\n";
            #endif
            cout<< "Wrong Checksum resend message\n Message arived: ";print_message_sh_mem(P2_shared_mem_key_file,P2_shared_mem_size_file);cout <<" with checksum: ";print_message_checksum(P2_shared_mem_key_file,P2_shared_mem_size_file);
            P(P2_shared_mem_key_file,P2_shared_mem_size_file,ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file,ENC2_semaphore_p1_key_file,P2_semaphore_p1_key_file);
            cout << "Message send: ";print_message_sh_mem(P2_shared_mem_key_file,P2_shared_mem_size_file);cout <<" with checksum: ";print_message_checksum(P2_shared_mem_key_file,P2_shared_mem_size_file);



            semaphore_signal(sem_ENC2_resend_p1_id);
            #if DEBUG >= 1
                printf("~ CHAN %d releasing Resend %d\n", getpid(),sem_ENC2_resend_p1_id);
            #endif

        }
        if(resend_flag==1){
            cout << "Waiting message back\n";
            #if DEBUG >= 1
                printf("~P %d waiting message back ,%d\n", getpid(),sem_p2_p2_id);
            #endif
            semaphore_wait(sem_p2_p2_id);
            resend_flag=0;
        }
        cout << "Message arived: ";print_message_sh_mem(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file); cout <<" with checksum: ";print_message_checksum(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file);
        P(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file,ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file,ENC2_semaphore_p1_key_file,P2_semaphore_p1_key_file);
        cout << "Message send: ";print_message_sh_mem(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file);cout <<" with checksum: ";print_message_checksum(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file);

        #if DEBUG >= 1
        printf("~ P2 %d releasing for statring P1 send message p4 %d\n", getpid(),sem_p1_p4_id);
        #endif
        semaphore_signal(sem_p1_p4_id);
        #if DEBUG >= 1
            printf("~P2 %d waiting ENC2 ,%d\n", getpid(),sem_p2_p2_id);
        #endif
        semaphore_wait(sem_p2_p2_id);
        if(if_term(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file)==1){
            cout << "Exit program deleting memmory and semaphores" <<endl;
            delete_semaphores_and_sheared_mem();
            return 0;
        }
        cout<<"\n\n";
        cout << "P1 message\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
        cout << "Message arived: ";print_message_sh_mem(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file); cout <<" with checksum: ";print_message_checksum(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file);
        P(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file,ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file,ENC2_semaphore_p1_key_file,P2_semaphore_p1_key_file);
        cout << "Message send: ";print_message_sh_mem(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file);cout <<" with checksum: ";print_message_checksum(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file);

        cout << "Message Read from P1\n";
        semaphore_signal(sem_ENC2_p2_id);
        #if DEBUG >= 1
            printf("~ P2 %d releasing p2 %d\n", getpid(),sem_ENC2_p2_id);
        #endif
        cout << "Waiting message from P1\n";
        #if DEBUG >= 1
        printf("~P %d waiting from P1 to start sending message %d\n", getpid(),sem_p2_p4_id);
        #endif
        semaphore_wait(sem_p2_p4_id);

    }


}

int P(char* read_shared_mem_key_file,int read_shared_mem_size_file,char* write_shared_mem_key_file, int write_shared_mem_size_file,char* write_semaphore,char* read_semaphore){
    #if DEBUG >= 2
        cout<<"- P2 born now ID "<<getpid()<<endl;
    #endif
    //~~~~~~~~~~~~~~~~~~~semaphore~~~~~~~~~~~~~~~~~~~~~~~//
    int sem_read_id  = get_semaphore_id_from_file(read_semaphore);
    int sem_write_id = get_semaphore_id_from_file(write_semaphore);

    #if DEBUG >= 2
        printf("~ENC %d waiting ,%d\n", getpid(),sem_read_id);
    #endif
    semaphore_wait(sem_read_id);

    //~~~~~~~~~~~~~~~~~~~memory~~~~~~~~~~~~~~~~~~~~~~~//
    /* Attach the shared memory segment. */
    int mem_seg_id_read=get_memory_id_from_file(read_shared_mem_key_file,read_shared_mem_size_file);
    message* shared_memory_read = (message*) shmat(mem_seg_id_read, NULL, 0);
    if(shared_memory_read==(void*)-1)die("shared memory P");
    #if DEBUG >= 1
        printf ("! shared memory attached at address %p\n", shared_memory_read);
    #endif




    //~~~~~~~~~~~~~~~~~~~~~~~~~READ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

    message* mess = new message(shared_memory_read->message_arrey);
    #if DEBUG >= 0
        printf ("\t- shared memory read_P"); mess->print();
    #endif


    //~~~~~~~~~~~~~~~~~~~~~~~~~WRITE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//


    /* Attach the shared memory segment. */
    int mem_seg_id_write=get_memory_id_from_file(write_shared_mem_key_file,write_shared_mem_size_file);
    message* shared_memory_write = (message*) shmat(mem_seg_id_write, NULL, 0);
    if(shared_memory_write==(void*)-1)die("shared memory P");
    #if DEBUG >= 1
        printf ("! shared memory attached at address %p\n", shared_memory_write);
    #endif

    strcpy(shared_memory_write->message_arrey,shared_memory_read->message_arrey);
    shared_memory_write->flag_checksum=mess->flag_checksum;
    #if DEBUG >= 0
        printf ("\t- shared memory write_P"); shared_memory_write->print();
    #endif

    //~~~~~~~~~~~~~~~~~~~clears~~~~~~~~~~~~~~~~~~~~~~~//
    /* Detach the shared memory segment. */
    shmdt(shared_memory_read);
    #if DEBUG>= 2
    cout<<"\t"<<getpid()<<" detached memory P\n";
    #endif
    shmdt(shared_memory_write);
    #if DEBUG>= 2
    cout<<"\t"<<getpid()<<" detached memory P-ENC\n";
    #endif

    semaphore_signal(sem_write_id);
    #if DEBUG >= 1
    printf("~ write P %d releasing %d\n", getpid(),sem_write_id);
    #endif

    return 0;
}
