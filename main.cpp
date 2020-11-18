#include "helping_functions.hpp"
#include "semaphores_sheared_mem.hpp"
#include "md5/md5.h"
using namespace std;

int P1(char* P_message);
int ENC1();
int CHAN();

int main(int argc, char const *argv[]) {
    char* temp = new char;
    int status =0;
    strcpy(temp, "hola bitch");


//~~~~~~~~~~~~~~~~Generate_keys~~~~~~~~~~~~~~~~~~~~~~//
    key_t P1_ENC1_shared_mem_key =ftok("main.o",1);
    key_t P1_ENC1_shared_mem_size =ftok("main.o",2);
    key_t ENC1_CHAN_shared_mem_key =ftok("main.o",3);
    key_t ENC1_CHAN_shared_mem_size =ftok("main.o",4);
    key_t CHAN_ENC2_shared_mem_key =ftok("main.o",5);
    key_t CHAN_ENC2_shared_mem_size =ftok("main.o",6);
    // key_t ENC1_CHAN_shared_mem_key =ftok("main.o",7);
    // key_t ENC1_CHAN_shared_mem_size =ftok("main.o",8);

    key_t P1_semaphore_read_key =ftok("main.o",9);
    key_t P1_semaphore_write_key =ftok("main.o",10);
    key_t ENC1_semaphore_read_key =ftok("main.o",11);
    key_t ENC1_semaphore_write_key =ftok("main.o",12);



//~~~~~~~~~~~~~~~~Shared_memory~~~~~~~~~~~~~~~~~~~~~~//
    //Shared_memory for P1 -> ENC1
    generate_memory_segment(P1_ENC1_shared_mem_key,P1_ENC1_shared_mem_size,P1_ENC1_shared_mem_key_file);
    int P1_ENC1_mem_id=get_memory_id_from_file(P1_ENC1_shared_mem_key_file,P1_ENC1_shared_mem_size);
    char* mem_P1_ENC1 = (char*) shmat(P1_ENC1_mem_id, NULL, 0);
    if(mem_P1_ENC1==(void*)-1)die("shared memory main");
    #if DEBUG >= 2
        printf ("!! shared mem_P1_ENC1 attached at main and initialized !!\n");
    #endif

    //Shared_memory for ENC1 -> CHAN
    generate_memory_segment(ENC1_CHAN_shared_mem_key,ENC1_CHAN_shared_mem_size,ENC1_CHAN_shared_mem_key_file);
    int ENC1_CHAN_mem_id=get_memory_id_from_file(ENC1_CHAN_shared_mem_key_file,ENC1_CHAN_shared_mem_size);
    char* mem_ENC1_CHAN = (char*) shmat(ENC1_CHAN_mem_id, NULL, 0);
    if(mem_ENC1_CHAN==(void*)-1)die("shared memory main");
    #if DEBUG >= 2
        printf ("!! shared mem_ENC1_CHAN attached at main and initialized !!\n");
    #endif

    //Shared_memory for CHAN -> ENC2
    // generate_memory_segment(CHAN_ENC2_shared_mem_key,CHAN_ENC2_shared_mem_size,ENC1_CHAN_shared_mem_key_file);
    // int CHAN_ENC2_mem_id=get_memory_id_from_file(ENC1_CHAN_shared_mem_key_file,ENC1_CHAN_shared_mem_size);
    // char* mem_CHAN_ENC2 = (char*) shmat(ENC1_CHAN_mem_id, NULL, 0);
    // if(mem_ENC1_CHAN==(void*)-1)die("shared memory main");
    // #if DEBUG >= 2
    //     printf ("!! shared mem_ENC1_CHAN attached at main and initialized !!\n");
    // #endif


//~~~~~~~~~~~~~~~~Semaphore~~~~~~~~~~~~~~~~~~~~~~~~~//
    int sem_P1_write=generate_semaphore(P1_semaphore_write_key,P1_semaphore_write_key_file);
    initialise_semaphore(sem_P1_write);
    int sem_P1_read=generate_semaphore(P1_semaphore_read_key, P1_semaphore_read_key_file);
    initialise_semaphore(sem_P1_read);

    int sem_ENC1_read=generate_semaphore(ENC1_semaphore_read_key, ENC1_semaphore_read_key_file);
    initialise_semaphore(sem_ENC1_read);
    int sem_ENC1_write=generate_semaphore(ENC1_semaphore_write_key, ENC1_semaphore_write_key_file);
    initialise_semaphore(sem_ENC1_write);



//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    //~~~~~~~~~~~~P1~~~~~~~~~~//
    pid_t processID_P1;
    #if DEBUG >= 1
        cout<<"!! starting P... !!\n";
    #endif
    processID_P1=fork();
    if(processID_P1==-1) die("fork failed");
    else if (processID_P1==0){
        P1(temp);
        exit(0);
    }

    // sleep(5);
    // cout << "\n\n\n\n\n\n\n\n\n\n\n\n";


    //~~~~~~~~~~~~ENC1~~~~~~~~~~//
    pid_t processID_ENC1;
    #if DEBUG >= 1
        cout<<"!! starting ENC1... !!\n";
    #endif
    processID_ENC1=fork();
    if(processID_ENC1==-1) die("fork failed");
    else if (processID_ENC1==0){
        ENC1();
        exit(0);
    }

    // sleep(5);
    // cout << "\n\n\n\n\n\n\n\n\n\n\n\n";
    //
    // //~~~~~~~~~~~~CHAN~~~~~~~~~~//
    // pid_t processID_CHAN;
    // #if DEBUG >= 1
    //     cout<<"!! starting CHAN... !!\n";
    // #endif
    // processID_CHAN=fork();
    // if(processID_CHAN==-1) die("fork failed");
    // else if (processID_CHAN==0){
    //     CHAN();
    //     exit(0);
    // }








    while(wait(&status) > 0);
    #if DEBUG >= 1
    cout<<"! ALL children finished"<<endl;
    #endif

    return 0;
}

int P1(char* P_message){
    #if DEBUG >= 1
        cout<<"- P1 born now ID "<<getpid()<<" with parent  "<<getppid()<<endl;
    #endif

    //~~~~~~~~~~~~~~~~~~~memory~~~~~~~~~~~~~~~~~~~~~~~//
    /* Attach the shared memory segment. */
    int mem_seg_id=get_memory_id_from_file(P1_ENC1_shared_mem_key_file,P1_ENC1_shared_mem_size_file);
    char* shared_memory = (char*) shmat(mem_seg_id, NULL, 0);
    if(shared_memory==(void*)-1)die("shared memory P1");
    #if DEBUG >= 2
        printf ("! shared memory attached at address %p\n", shared_memory);
    #endif

    //~~~~~~~~~~~~~~~~~~~semaphore~~~~~~~~~~~~~~~~~~~~~~~//
    int sem_read_id  = get_semaphore_id_from_file(ENC1_semaphore_read_key_file);
    int sem_write_id = get_semaphore_id_from_file(P1_semaphore_write_key_file);
    //~~~~~~~~~~~~~~~~~~~~~~~~~SEND~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    #if DEBUG >= 1
        printf("~ send P1->ENC1 %d waiting\n", getpid());
    #endif
    semaphore_signal(sem_write_id);
    strcpy(shared_memory,P_message);
    #if DEBUG >= 1
        cout<<"~ preleasing sem_write(p1->enc1) has items\n";
    #endif
    semaphore_signal(sem_read_id);
    #if DEBUG >= 1
        printf("\tsend_P1_ENC1 shared memmory: %s \n",shared_memory);
        printf("~ send_P1_ENC1 %d releasing\n", getpid());
    #endif
    semaphore_wait(sem_write_id);

    //~~~~~~~~~~~~~~~~~~~clears~~~~~~~~~~~~~~~~~~~~~~~//
    /* Detach the shared memory segment. */
    shmdt(shared_memory);
    #if DEBUG>= 2
    cout<<"\t"<<getpid()<<" detached memory P1-ENC1\n";
    #endif
    //sleep(5);

    return 0;
}



int ENC1(){
    #if DEBUG >= 1
    cout<<"- ENC1 born now ID "<<getpid()<<" with parent  "<<getppid()<<endl;
    #endif


    //~~~~~~~~~~~~~~~~~~~memory~~~~~~~~~~~~~~~~~~~~~~~//
    /* Attach the shared memory segment. */
    int mem_seg_P1_ENC1_id=get_memory_id_from_file(P1_ENC1_shared_mem_key_file,P1_ENC1_shared_mem_size_file);
    char* shared_memory_P1_ENC1 = (char*) shmat(mem_seg_P1_ENC1_id, NULL, 0);
    if(shared_memory_P1_ENC1==(void*)-1)die("shared memory ENC1-read");

    //~~~~~~~~~~~~~~~~~~~semaphore~~~~~~~~~~~~~~~~~~~~~~~//
    int sem_read_id  = get_semaphore_id_from_file(ENC1_semaphore_read_key_file);
    int sem_write_id  = get_semaphore_id_from_file(ENC1_semaphore_write_key_file);

    //~~~~~~~~~~~~~~~~~READ FROM P1~~~~~~~~~~~~~~~~~~//
    semaphore_wait(sem_read_id);
    #if DEBUG >= 1
        printf("~ read P1->ENC1 %d waiting\n", getpid());
    #endif
    char* message_P1= new char ;
    strcpy(message_P1,shared_memory_P1_ENC1);
    semaphore_signal(sem_read_id);
    #if DEBUG >= 1
        printf("~ read P1->ENC1 %d releasing\n", getpid());
        printf ("\t- shared memory read_P1_ENC1: %s\n", message_P1);
    #endif

    /* Detach the shared memory segment. */
    shmdt(shared_memory_P1_ENC1);
    #if DEBUG>= 2
        cout<<"\t"<<getpid()<<" detached memory P1-ENC1\n";
    #endif

    //~~~~~~~~~~~~~~~~~~~~ENC1-JOB~~~~~~~~~~~~~~~~~~~~~//

    char* checksum = new char[50];
    strcpy(checksum,md5(message_P1).c_str());
    message* mess = new message(message_P1);
    strcpy(mess->checksum,checksum);
    mess->flag_checksum=0;

    //cout << checksum << endl <<endl;
    #if DEBUG >= 1
        cout << "\t Message value ENC: ";mess->print();
    #endif



    //~~~~~~~~~~~~~~~~~WRITE TOO CHAN~~~~~~~~~~~~~~~~~~//

    //~~~~~~~~~~~~~~~~~~~memory~~~~~~~~~~~~~~~~~~~~~~~//
    /* Attach the shared memory segment. */
    int mem_seg_ENC1_CHAN_id=get_memory_id_from_file(ENC1_CHAN_shared_mem_key_file,ENC1_CHAN_shared_mem_size_file);
    message* shared_memory_ENC1_CHAN = (message*) shmat(mem_seg_ENC1_CHAN_id, NULL, 0);
    if(shared_memory_ENC1_CHAN==(void*)-1)die("shared memory ENC1-write");

    semaphore_wait(sem_write_id);
    #if DEBUG >= 1
        printf("~ write ENC1->CHAN %d waiting\n", getpid());
    #endif
    strcpy(shared_memory_ENC1_CHAN->message_arrey,mess->message_arrey);
    strcpy(shared_memory_ENC1_CHAN->checksum,mess->checksum);
    shared_memory_ENC1_CHAN->flag_checksum= mess->flag_checksum;
    semaphore_signal(sem_write_id);
    #if DEBUG >= 1
        printf("~ write ENC1->CHAN %d releasing\n", getpid());
    #endif


    shared_memory_ENC1_CHAN->print();

    /* Detach the shared memory segment. */
    shmdt(shared_memory_ENC1_CHAN);
    #if DEBUG>= 2
        cout<<"\t"<<getpid()<<" detached memory ENC1_CHAN\n";
    #endif
    return 0;
}

int CHAN(){
    #if DEBUG >= 1
    cout<<"- CHAN born now ID "<<getpid()<<" with parent  "<<getppid()<<endl;
    #endif


    //~~~~~~~~~~~~~~~~~READ FROM ENC1~~~~~~~~~~~~~~~~~~//

    //~~~~~~~~~~~~~~~~~~~memory~~~~~~~~~~~~~~~~~~~~~~~//
    /* Attach the shared memory segment. */
    int mem_seg_ENC1_CHAN_id=get_memory_id_from_file(ENC1_CHAN_shared_mem_key_file,ENC1_CHAN_shared_mem_size_file);
    message* shared_memory_ENC1_CHAN = (message*) shmat(mem_seg_ENC1_CHAN_id, NULL, 0);
    if(shared_memory_ENC1_CHAN==(void*)-1)die("shared memory ENC1-read");
    #if DEBUG >= 1
        printf("~ read ENC1->CHAN %d waiting\n", getpid());
    #endif
    cout << "sheard mem ENC1-> CHAN: "<<shared_memory_ENC1_CHAN->message_arrey <<endl;
    message* mess = new message(shared_memory_ENC1_CHAN->message_arrey);
    strcpy(mess->checksum,shared_memory_ENC1_CHAN->checksum);
    mess->flag_checksum= shared_memory_ENC1_CHAN->flag_checksum;
    #if DEBUG >= 2
        cout << "\t Message value CHAN: ";mess->print();
    #endif

    //~~~~~~~~~~~~~~~CHANGE ON CHANEL~~~~~~~~~~~~~~~~//

    //~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//

    //~~~~~~~~~~~~~~WRITE CHAN TOO ENC2~~~~~~~~~~~~~~//





}
