#include "helping_functions.hpp"
#include "semaphores_sheared_mem.hpp"
#include "md5/md5.h"
int CHAN(char* read_shared_mem_key_file, int read_shared_mem_size_file,char* write_shared_mem_key_file, int write_shared_mem_size_file,char* write_semaphore,char* read_semaphore);

int main(int argc, char const *argv[]) {
    int sem_CHAN_p2_id = get_semaphore_id_from_file(CHAN_semaphore_p2_key_file);
    int sem_CHAN_p4_id = get_semaphore_id_from_file(CHAN_semaphore_p4_key_file);
    int sem_CHAN_p3_id = get_semaphore_id_from_file(CHAN_semaphore_p3_key_file);
    int sem_ENC_p2_id = get_semaphore_id_from_file(ENC_semaphore_p2_key_file);
    int sem_ENC_p3_id = get_semaphore_id_from_file(ENC_semaphore_p3_key_file);
    int sem_ENC2_p3_id = get_semaphore_id_from_file(ENC2_semaphore_p3_key_file);
    int sem_ENC2_p2_id = get_semaphore_id_from_file(ENC2_semaphore_p2_key_file);
    int sem_ENC2_p4_id = get_semaphore_id_from_file(ENC2_semaphore_p4_key_file);



    while(1){
    CHAN(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file,CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file,ENC2_semaphore_p1_key_file,CHAN_semaphore_p1_key_file);
    cout<<"\n\n\n\n\n\n";
    #if DEBUG >= 1
            printf("~CHAN %d waiting message back ,%d\n", getpid(),sem_CHAN_p2_id);
    #endif
    semaphore_wait(sem_CHAN_p2_id);

    CHAN(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file,ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file,ENC_semaphore_p1_key_file,CHAN_semaphore_p1_key_file);
    semaphore_signal(sem_ENC_p2_id);
    #if DEBUG >= 1
        printf("~ ENC1 %d releasing p2 %d\n", getpid(),sem_ENC_p2_id);
    #endif



    #if DEBUG >= 1
        cout << "THIS IS P2 MESSAGE\n";
    #endif
    #if DEBUG >= 1
        printf("~ENC2 %d waiting p3 %d\n", getpid(),sem_CHAN_p3_id);
    #endif
    semaphore_wait(sem_CHAN_p3_id);
    CHAN(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file,ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file,ENC_semaphore_p1_key_file,CHAN_semaphore_p1_key_file);
    semaphore_signal(sem_ENC_p3_id);
    #if DEBUG >= 1
        printf("~ ENC1 %d releasing p3 %d\n", getpid(),sem_ENC_p3_id);
    #endif
    #if DEBUG >= 1
        printf("~CHAN %d waiting message back ,%d\n", getpid(),sem_CHAN_p4_id);
    #endif
    semaphore_wait(sem_CHAN_p4_id);
    CHAN(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file,CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file,ENC2_semaphore_p1_key_file,CHAN_semaphore_p1_key_file);
    semaphore_signal(sem_ENC2_p4_id);
    #if DEBUG >= 1
        printf("~ ENC2 %d releasing p4 %d\n", getpid(),sem_ENC2_p4_id);
    #endif


    cout<<"\n\n\n\n\n\n";
    cout<<"\n\n\n\n\n\n";
}

    return 0;
}

int CHAN(char* read_shared_mem_key_file, int read_shared_mem_size_file,char* write_shared_mem_key_file, int write_shared_mem_size_file,char* write_semaphore,char* read_semaphore){
    #if DEBUG >= 1
    cout<<"- CHAN born now ID "<< getpid() << " with parent  "<< getppid()<<endl;
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

    message* mess = new message(shared_memory_read->message_arrey);

    strcpy(mess->checksum,shared_memory_read->checksum);
    mess->flag_checksum= shared_memory_read->flag_checksum;
    #if DEBUG >= 1
        printf ("\t- shared memory read_ENC"); mess->print();
    #endif
    /* Detach the shared memory segment. */
    shmdt(shared_memory_read);
    #if DEBUG>= 2
        cout<<"\t"<<getpid()<<" detached memory ENC_CHAN\n";
    #endif

    //~~~~~~~~~~~~~~~CHANGE ON CHANEL~~~~~~~~~~~~~~~~//
        //strcpy(mess->message_arrey,"aaaaa");

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

    //~~~~~~~~~~~~~~WRITE CHAN~~~~~~~~~~~~~~//

    int mem_seg_write_id=get_memory_id_from_file(write_shared_mem_key_file,write_shared_mem_size_file);
    message* shared_memory_write = (message*) shmat(mem_seg_write_id, NULL, 0);
    if(shared_memory_write==(void*)-1)die("shared memory CHAN-write");

    strcpy(shared_memory_write->message_arrey,mess->message_arrey);
    strcpy(shared_memory_write->checksum,mess->checksum);
    shared_memory_write->flag_checksum = mess->flag_checksum;
    #if DEBUG >= 1
        printf ("\t- shared memory write_CHAN"); shared_memory_write->print();
    #endif

    /* Detach the shared memory segment. */
    shmdt(shared_memory_write);
    #if DEBUG>= 2
        cout<<"\t"<<getpid()<<" detached memory CHAN_ENC\n";
    #endif
    semaphore_signal(sem_write_id);
    cout << sem_write_id <<endl;
    #if DEBUG >= 1
        printf("~ write CHAN %d releasing %d\n", getpid(),sem_write_id);
    #endif

}
