#include "helping_functions.hpp"
#include "semaphores_sheared_mem.hpp"
#include "md5/md5.h"
#include <stdlib.h>
#include<time.h>
int CHAN(int type,char* read_shared_mem_key_file, int read_shared_mem_size_file,char* write_shared_mem_key_file, int write_shared_mem_size_file,char* write_semaphore,char* read_semaphore);
int resend_message(char* shared_mem_key_file,int shared_mem_size_file);
int rand_number=0;
int resend_flag=0;

int main(int argc, char const *argv[]) {

    int sem_CHAN_resend_p1_id = get_semaphore_id_from_file(CHAN_semaphore_resend_p1_key_file);
    int sem_ENC_resend_p1_id = get_semaphore_id_from_file(ENC_semaphore_resend_p1_key_file);
    int sem_ENC2_resend_p1_id = get_semaphore_id_from_file(ENC2_semaphore_resend_p1_key_file);

    int sem_CHAN_p2_id = get_semaphore_id_from_file(CHAN_semaphore_p2_key_file);
    int sem_CHAN_p4_id = get_semaphore_id_from_file(CHAN_semaphore_p4_key_file);
    int sem_CHAN_p3_id = get_semaphore_id_from_file(CHAN_semaphore_p3_key_file);
    int sem_ENC_p2_id = get_semaphore_id_from_file(ENC_semaphore_p2_key_file);
    int sem_ENC_p3_id = get_semaphore_id_from_file(ENC_semaphore_p3_key_file);
    int sem_ENC2_p2_id = get_semaphore_id_from_file(ENC2_semaphore_p2_key_file);
    int sem_ENC2_p4_id = get_semaphore_id_from_file(ENC2_semaphore_p4_key_file);



    while(1){
        cout<<"\n\n";
        cout << "P1 message\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";

    #if DEBUG >= 1
        printf("~CHAN %d waiting enc ,%d\n", getpid(),sem_CHAN_p2_id);
    #endif
    semaphore_wait(sem_CHAN_p2_id);
    cout << "Message arived: ";print_message_sh_mem(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file); cout <<" with checksum: ";print_message_checksum(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file);
    CHAN(0,ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file,CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file,ENC2_semaphore_p1_key_file,CHAN_semaphore_p1_key_file);
    cout << "Message send: ";print_message_sh_mem(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file); cout <<" with checksum: ";print_message_checksum(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file);

    #if DEBUG >= 1
        printf("~ %d releasing enc2 p2 %d\n", getpid(),sem_ENC2_p2_id);
    #endif
    semaphore_signal(sem_ENC2_p2_id);
    if(if_term(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file)==1){
        cout << "Exit program \n";
        return 0;
    }

    cout << "Waiting message back\n";
    #if DEBUG >= 1
        printf("~CHAN %d waiting message back ,%d\n", getpid(),sem_CHAN_p2_id);
    #endif
    semaphore_wait(sem_CHAN_p2_id);
    if(resend_message(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file)==1){
        resend_flag=1;
        cout<< "Resend message \n";

        cout << "Message arived: ";print_message_sh_mem(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file); cout <<" with checksum: ";print_message_checksum(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file);
        CHAN(0,CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file,ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file,ENC_semaphore_p1_key_file,CHAN_semaphore_p1_key_file);
        cout << "Message send: ";print_message_sh_mem(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file); cout <<" with checksum: ";print_message_checksum(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file);

        #if DEBUG >= 1
        printf("~ CHAN %d releasing Resend %d\n", getpid(),sem_ENC_p2_id);
        #endif
        semaphore_signal(sem_ENC_p2_id);
        cout << "Waiting message back\n";
        #if DEBUG >= 1
            printf("~CHAN %d waiting Resend MESSAGE BACK enc2 ,%d\n", getpid(),sem_CHAN_resend_p1_id);
        #endif
        semaphore_wait(sem_CHAN_resend_p1_id);
        cout << "Message arived: ";print_message_sh_mem(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file); cout <<" with checksum: ";print_message_checksum(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file);
        CHAN(0,ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file,CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file,ENC2_semaphore_p1_key_file,CHAN_semaphore_p1_key_file);
        cout << "Message send: ";print_message_sh_mem(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file); cout <<" with checksum: ";print_message_checksum(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file);
        semaphore_signal(sem_ENC2_resend_p1_id);
        #if DEBUG >= 1
            printf("~ CHAN %d releasing Resend %d\n", getpid(),sem_ENC2_resend_p1_id);
        #endif

    }
    if(resend_flag==1){
        cout << "Waiting message back\n";
        #if DEBUG >= 1
            printf("~CHAN %d waiting message back ,%d\n", getpid(),sem_CHAN_p2_id);
        #endif
        semaphore_wait(sem_CHAN_p2_id);
        resend_flag=0;
    }
    cout << "Message arived: ";print_message_sh_mem(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file); cout <<" with checksum: ";print_message_checksum(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file);
    CHAN(0,CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file,ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file,ENC_semaphore_p1_key_file,CHAN_semaphore_p1_key_file);
    cout << "Message send: ";print_message_sh_mem(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file); cout <<" with checksum: ";print_message_checksum(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file);

    semaphore_signal(sem_ENC_p2_id);
    #if DEBUG >= 1
        printf("~ ENC1 %d releasing p2 %d\n", getpid(),sem_ENC_p2_id);
    #endif


    cout<<"\n\n";
    cout << "P2 message\n~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~\n";
    #if DEBUG >= 1
        printf("~ENC2 %d waiting p3 %d\n", getpid(),sem_CHAN_p3_id);
    #endif
    semaphore_wait(sem_CHAN_p3_id);
    cout << "Message arived: ";print_message_sh_mem(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file); cout <<" with checksum: ";print_message_checksum(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file);
    CHAN(1,CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file,ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file,ENC_semaphore_p1_key_file,CHAN_semaphore_p1_key_file);
    cout << "Message send: ";print_message_sh_mem(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file); cout <<" with checksum: ";print_message_checksum(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file);

    semaphore_signal(sem_ENC_p3_id);
    #if DEBUG >= 1
        printf("~ ENC1 %d releasing p3 %d\n", getpid(),sem_ENC_p3_id);
    #endif
    if(if_term(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file)==1){
        cout << "Exit program\n";
        return 0;
    }
    cout << "Waiting message back\n";
    #if DEBUG >= 1
        printf("~CHAN %d waiting message back ,%d\n", getpid(),sem_CHAN_p4_id);
    #endif
    semaphore_wait(sem_CHAN_p4_id);
    if(resend_message(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file)==1){
        cout<< "Resend message \n";
        resend_flag=1;
        cout << "Message arived: ";print_message_sh_mem(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file); cout <<" with checksum: ";print_message_checksum(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file);
        CHAN(0,ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file,CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file,ENC2_semaphore_p1_key_file,CHAN_semaphore_p1_key_file);
        cout << "Message send: ";print_message_sh_mem(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file); cout <<" with checksum: ";print_message_checksum(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file);
        semaphore_signal(sem_ENC2_p4_id);
        #if DEBUG >= 1
        printf("~ CHAN %d releasing Resend %d\n", getpid(),sem_ENC2_p4_id);
        #endif
        cout << "Waiting message back\n";
        #if DEBUG >= 1
            printf("~CHAN %d waiting Resend MESSAGE BACK enc2 ,%d\n", getpid(),sem_CHAN_resend_p1_id);
        #endif
        semaphore_wait(sem_CHAN_resend_p1_id);
        cout << "Message arived: ";print_message_sh_mem(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file); cout <<" with checksum: ";print_message_checksum(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file);
        CHAN(0,CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file,ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file,ENC_semaphore_p1_key_file,CHAN_semaphore_p1_key_file);
        cout << "Message send: ";print_message_sh_mem(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file); cout <<" with checksum: ";print_message_checksum(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file);
        semaphore_signal(sem_ENC_resend_p1_id);
        #if DEBUG >= 1
            printf("~ CHAN %d releasing Resend %d\n", getpid(),sem_ENC_resend_p1_id);
        #endif

    }
    if(resend_flag==1){
        cout << "Waiting message back\n";
        #if DEBUG >= 1
            printf("~CHAN %d waiting message back ,%d\n", getpid(),sem_CHAN_p4_id);
        #endif
        semaphore_wait(sem_CHAN_p4_id);
        resend_flag=0;
    }
    cout << "Message arived: ";print_message_sh_mem(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file); cout <<" with checksum: ";print_message_checksum(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file);
    CHAN(0,ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file,CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file,ENC2_semaphore_p1_key_file,CHAN_semaphore_p1_key_file);
    cout << "Message send: ";print_message_sh_mem(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file); cout <<" with checksum: ";print_message_checksum(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file);

    semaphore_signal(sem_ENC2_p4_id);
    #if DEBUG >= 1
        printf("~ ENC2 %d releasing p4 %d\n", getpid(),sem_ENC2_p4_id);
    #endif

}

    return 0;
}

int CHAN(int type,char* read_shared_mem_key_file, int read_shared_mem_size_file,char* write_shared_mem_key_file, int write_shared_mem_size_file,char* write_semaphore,char* read_semaphore){
    #if DEBUG >= 3
    cout<<"- CHAN born now ID "<< getpid()<<endl;
    #endif

    //~~~~~~~~~~~~~~~~~~~semaphore~~~~~~~~~~~~~~~~~~~~~~~//
    int sem_read_id  = get_semaphore_id_from_file(read_semaphore);
    int sem_write_id  = get_semaphore_id_from_file(write_semaphore);

    //~~~~~~~~~~~~~~~~~READ~~~~~~~~~~~~~~~~~~//
    #if DEBUG >= 1
    printf("~CHAN %d waiting ,%d\n", getpid(),sem_read_id);
    #endif
    semaphore_wait(sem_read_id);
    //~~~~~~~~~~~~~~~~~~~~memory~~~~~~~~~~~~~~~~~~~~~~~//
    /* Attach the shared memory segment. */
    int mem_seg_read_id=get_memory_id_from_file(read_shared_mem_key_file,read_shared_mem_size_file);
    message* shared_memory_read = (message*) shmat(mem_seg_read_id, NULL, 0);
    if(shared_memory_read==(void*)-1)die("shared memory CHAN-read");
    #if DEBUG >= 2
        printf ("! shared memory attached at address %p\n", shared_memory_read);
    #endif

    message* mess = new message(shared_memory_read->message_arrey);

    strcpy(mess->checksum,shared_memory_read->checksum);
    shared_memory_read->checksum[33]='\0';
    mess->flag_checksum= shared_memory_read->flag_checksum;
    #if DEBUG >= 0
        printf ("\t- shared memory read_ENC"); mess->print();
    #endif
    /* Detach the shared memory segment. */
    shmdt(shared_memory_read);
    #if DEBUG>= 2
        cout<<"\t"<<getpid()<<" detached memory ENC_CHAN\n";
    #endif

    //~~~~~~~~~~~~~~~CHANGE ON CHANEL~~~~~~~~~~~~~~~~//
    if(type==1){
        if(strcmp(mess->message_arrey,"TERM")==0){
            #if DEBUG>= 3
            cout << "\nTERM\n";
            #endif
        }else{
            srand(time(NULL));
            int rand_num=(rand() % 3);
            if(rand_num==2 && rand_number!=3){
                strcpy(mess->message_arrey,"CHANGED_MESSAGE");
                rand_number=rand_num;
            }else{
                rand_number=rand_num;
            }
        }

    }
    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

    //~~~~~~~~~~~~~~WRITE CHAN~~~~~~~~~~~~~~//

    int mem_seg_write_id=get_memory_id_from_file(write_shared_mem_key_file,write_shared_mem_size_file);
    message* shared_memory_write = (message*) shmat(mem_seg_write_id, NULL, 0);
    if(shared_memory_write==(void*)-1)die("shared memory CHAN-write");
    #if DEBUG >= 2
        printf ("! shared memory attached at address %p\n", shared_memory_write);
    #endif

    strcpy(shared_memory_write->message_arrey,mess->message_arrey);
    strcpy(shared_memory_write->checksum,mess->checksum);
    shared_memory_write->checksum[33]='\0';
    shared_memory_write->flag_checksum = mess->flag_checksum;
    #if DEBUG >= 0
        printf ("\t- shared memory write_CHAN"); shared_memory_write->print();
    #endif

    /* Detach the shared memory segment. */
    shmdt(shared_memory_write);
    #if DEBUG>= 2
        cout<<"\t"<<getpid()<<" detached memory CHAN_ENC\n";
    #endif
    semaphore_signal(sem_write_id);
    #if DEBUG >= 1
        printf("~ write CHAN %d releasing %d\n", getpid(),sem_write_id);
    #endif

}
