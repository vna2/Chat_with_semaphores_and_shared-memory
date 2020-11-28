#include "helping_functions.hpp"
#include "semaphores_sheared_mem.hpp"
#include "md5/md5.h"

int P(char* read_shared_mem_key_file,int read_shared_mem_size_file,char* write_shared_mem_key_file, int write_shared_mem_size_file,char* write_semaphore,char* read_semaphore);

int main(int argc, char const *argv[]) {
    char* temp[5];
    int status =0;
    for (size_t i = 0; i < 5; i++) {
        temp[i] = new char;
    }
    strcpy(temp[0], "hola bitch");
    strcpy(temp[1], "hola bitch1");
    strcpy(temp[2], "hola bitch2");
    strcpy(temp[3], "hola bitch3");
    {

    P(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file,ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file,ENC2_semaphore_p1_key_file,P2_semaphore_p1_key_file);
    releasing_all();
}


}

int P(char* read_shared_mem_key_file,int read_shared_mem_size_file,char* write_shared_mem_key_file, int write_shared_mem_size_file,char* write_semaphore,char* read_semaphore){
    #if DEBUG >= 1
        cout<<"- P born now ID "<<getpid()<<" with parent  "<<getppid()<<endl;
    #endif
    //~~~~~~~~~~~~~~~~~~~semaphore~~~~~~~~~~~~~~~~~~~~~~~//
    int sem_read_id  = get_semaphore_id_from_file(read_semaphore);
    int sem_write_id = get_semaphore_id_from_file(write_semaphore);

    #if DEBUG >= 1
        printf("~ENC %d waiting ,%d\n", getpid(),sem_read_id);
    #endif
    semaphore_wait(sem_read_id);

    //~~~~~~~~~~~~~~~~~~~memory~~~~~~~~~~~~~~~~~~~~~~~//
    /* Attach the shared memory segment. */
    int mem_seg_id_read=get_memory_id_from_file(read_shared_mem_key_file,read_shared_mem_size_file);
    message* shared_memory_read = (message*) shmat(mem_seg_id_read, NULL, 0);
    if(shared_memory_read==(void*)-1)die("shared memory P");
    #if DEBUG >= 2
        printf ("! shared memory attached at address %p\n", shared_memory_read);
    #endif


    /* Attach the shared memory segment. */
    int mem_seg_id_write=get_memory_id_from_file(write_shared_mem_key_file,write_shared_mem_size_file);
    message* shared_memory_write = (message*) shmat(mem_seg_id_write, NULL, 0);
    if(shared_memory_write==(void*)-1)die("shared memory P");
    #if DEBUG >= 2
        printf ("! shared memory attached at address %p\n", shared_memory_write);
    #endif


    //~~~~~~~~~~~~~~~~~~~~~~~~~READ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

    message* mess = new message(shared_memory_read->message_arrey);
    #if DEBUG >= 1
        printf ("\t- shared memory read_P"); mess->print();
    #endif


    //~~~~~~~~~~~~~~~~~~~~~~~~~WRITE~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//


    strcpy(shared_memory_write->message_arrey,shared_memory_read->message_arrey);
    shared_memory_write->flag_checksum=mess->flag_checksum;
    #if DEBUG >= 1
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
