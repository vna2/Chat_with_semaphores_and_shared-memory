#include "helping_functions.hpp"
#include "semaphores_sheared_mem.hpp"
#include "md5/md5.h"
using namespace std;

int P(char* read_shared_mem_key_file,int read_shared_mem_size_file,char* write_shared_mem_key_file, int write_shared_mem_size_file,char* write_semaphore,char* read_semaphore);
int ENC(char* read_shared_mem_key_file,int recive_shared_mem_size_file,char* write_shared_mem_key_file,int write_shared_mem_size_file,char* write_semaphore,char* read_semaphore);
int CHAN(char* read_shared_mem_key_file, int read_shared_mem_size_file,char* write_shared_mem_key_file, int write_shared_mem_size_file,char* write_semaphore,char* read_semaphore);
int flag_resend=0;
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



//~~~~~~~~~~~~~~~~Generate_keys~~~~~~~~~~~~~~~~~~~~~~//
    key_t P_ENC_shared_mem_key =ftok("main.o",1);
    key_t P_ENC_shared_mem_size =ftok("main.o",2);
    key_t ENC_CHAN_shared_mem_key =ftok("main.o",3);
    key_t ENC_CHAN_shared_mem_size =ftok("main.o",4);
    key_t CHAN_ENC_shared_mem_key =ftok("main.o",5);
    key_t CHAN_ENC_shared_mem_size =ftok("main.o",6);
    key_t ENC_P2_shared_mem_key =ftok("main.o",7);
    key_t ENC_P2_shared_mem_size =ftok("main.o",8);
    key_t P_shared_mem_key =ftok("main.o",9);
    key_t P_shared_mem_size =ftok("main.o",10);

    key_t P_semaphore_read_key =ftok("main.o",11);
    key_t P_semaphore_p2_key =ftok("main.o",12);
    key_t ENC_semaphore_p1_key =ftok("main.o",13);
    key_t ENC_semaphore_p2_key =ftok("main.o",14);
    key_t CHAN_semaphore_p1_key =ftok("main.o",15);
    key_t CHAN_semaphore_p2_key =ftok("main.o",16);


//~~~~~~~~~~~~~~~~Shared_memory~~~~~~~~~~~~~~~~~~~~~~//
    //Shared_memory for P
    generate_memory_segment(P_shared_mem_key,P_shared_mem_size,P_shared_mem_key_file);
    int P_mem_id=get_memory_id_from_file(P_shared_mem_key_file,P_shared_mem_size);
    char* mem_P = (char*) shmat(P_mem_id, NULL, 0);
    if(mem_P==(void*)-1)die("shared memory main");
    #if DEBUG >= 2
        printf ("!! shared mem_P attached at main and initialized !!\n");
    #endif

    //Shared_memory for P -> ENC
    generate_memory_segment(P_ENC_shared_mem_key,P_ENC_shared_mem_size,P_ENC_shared_mem_key_file);
    int P_ENC_mem_id=get_memory_id_from_file(P_ENC_shared_mem_key_file,P_ENC_shared_mem_size);
    char* mem_P_ENC = (char*) shmat(P_ENC_mem_id, NULL, 0);
    if(mem_P_ENC==(void*)-1)die("shared memory main");
    #if DEBUG >= 2
        printf ("!! shared mem_P_ENC attached at main and initialized !!\n");
    #endif

    //Shared_memory for ENC -> CHAN
    generate_memory_segment(ENC_CHAN_shared_mem_key,ENC_CHAN_shared_mem_size,ENC_CHAN_shared_mem_key_file);
    int ENC_CHAN_mem_id=get_memory_id_from_file(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size);
    char* mem_ENC_CHAN = (char*) shmat(ENC_CHAN_mem_id, NULL, 0);
    if(mem_ENC_CHAN==(void*)-1)die("shared memory main");
    #if DEBUG >= 2
        printf ("!! shared mem_ENC_CHAN attached at main and initialized !!\n");
    #endif

    //Shared_memory for CHAN -> ENC
    generate_memory_segment(CHAN_ENC_shared_mem_key,CHAN_ENC_shared_mem_size,CHAN_ENC_shared_mem_key_file);
    int CHAN_ENC_mem_id=get_memory_id_from_file(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size);
    char* mem_CHAN_ENC = (char*) shmat(CHAN_ENC_mem_id, NULL, 0);
    if(mem_CHAN_ENC==(void*)-1)die("shared memory main");
    #if DEBUG >= 2
        printf ("!! shared mem_CHAN_ENC attached at main and initialized !!\n");
    #endif

    //Shared_memory for ENC-> P
    generate_memory_segment(ENC_P2_shared_mem_key,ENC_P2_shared_mem_size,ENC_P2_shared_mem_key_file);
    int ENC_P_mem_id=get_memory_id_from_file(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size);
    char* mem_ENC_P = (char*) shmat(ENC_P_mem_id, NULL, 0);
    if(mem_ENC_P==(void*)-1)die("shared memory main");
    #if DEBUG >= 2
        printf ("!! shared mem_ENC_P attached at main and initialized !!\n");
    #endif



//~~~~~~~~~~~~~~~~Semaphore~~~~~~~~~~~~~~~~~~~~~~~~~//
    int sem_P_write=generate_semaphore(P_semaphore_p2_key,P_semaphore_p2_key_file);
    initialise_semaphore(sem_P_write);
    int sem_P_read=generate_semaphore(P_semaphore_read_key, P_semaphore_p1_key_file);
    initialise_semaphore(sem_P_read);

    int sem_ENC_read=generate_semaphore(ENC_semaphore_p1_key, ENC_semaphore_p1_key_file);
    initialise_semaphore(sem_ENC_read);
    int sem_ENC_write=generate_semaphore(ENC_semaphore_p2_key, ENC_semaphore_p2_key_file);
    initialise_semaphore(sem_ENC_write);

    int sem_CHAN_read=generate_semaphore(CHAN_semaphore_p1_key, CHAN_semaphore_p1_key_file);
    initialise_semaphore(sem_CHAN_read);
    int sem_CHAN_write=generate_semaphore(CHAN_semaphore_p2_key, CHAN_semaphore_p2_key_file);
    initialise_semaphore(sem_CHAN_write);

//~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~//
    //int sem__id  = get_semaphore_id_from_file(P_semaphore_p2_key_file);
    int sem_write_id = get_semaphore_id_from_file(P_semaphore_p1_key_file);

    for (size_t i = 0; i <1; i++) {
        //sleep(2);
        cout << "\n\n\n\n\n\n\n\n\n\n\n\n";
        //~~~~~~~~~~~~~~~~~~~memory~~~~~~~~~~~~~~~~~~~~~~~//
        /* Attach the shared memory segment. */
        int mem_seg_id=get_memory_id_from_file(P_shared_mem_key_file,P_shared_mem_size_file);
        message* shared_memory = (message*) shmat(mem_seg_id, NULL, 0);
        if(shared_memory==(void*)-1)die("shared memory P");
        #if DEBUG >= 2
            printf ("! shared memory attached at address %p\n", shared_memory);
        #endif
        strcpy(shared_memory->message_arrey,temp[i]);
        //~~~~~~~~~~~~~~~~~~~clears~~~~~~~~~~~~~~~~~~~~~~~//
        /* Detach the shared memory segment. */
        shmdt(shared_memory);
        #if DEBUG>= 2
        cout<<"\t"<<getpid()<<" detached memory P-ENC\n";
        #endif
        semaphore_signal(sem_write_id);
        #if DEBUG >= 1
            printf("~ write_P %d releasing\n", getpid());
        #endif




        //~~~~~~~~~~~~P~~~~~~~~~~//
        pid_t processID_P;
        #if DEBUG >= 1
            cout<<"!! starting P... !!\n";
        #endif
        processID_P=fork();
        if(processID_P==-1) die("fork failed");
        else if (processID_P==0){
                P(P_shared_mem_key_file,P_shared_mem_size_file,P_ENC_shared_mem_key_file,P_ENC_shared_mem_size_file ,ENC_semaphore_p1_key_file,P_semaphore_p1_key_file);
            exit(0);
        }

        sleep(2);
        cout << "\n\n\n\n\n\n\n\n\n\n\n\n";


        //~~~~~~~~~~~~ENC~~~~~~~~~~//
        pid_t processID_ENC;
        #if DEBUG >= 1
           cout<<"!! starting ENC... !!\n";
        #endif
        processID_ENC=fork();
        if(processID_ENC==-1) die("fork failed");
        else if (processID_ENC==0){
           ENC(P_ENC_shared_mem_key_file,P_ENC_shared_mem_size_file,ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file,CHAN_semaphore_p1_key_file,ENC_semaphore_p1_key_file);
           exit(0);
        }


        sleep(2);
        cout << "\n\n\n\n\n\n\n\n\n\n\n\n";

        //~~~~~~~~~~~~CHAN~~~~~~~~~~//
        pid_t processID_CHAN;
        #if DEBUG >= 1
            cout<<"!! starting CHAN... !!\n";
        #endif
        processID_CHAN=fork();
        if(processID_CHAN==-1) die("fork failed");
        else if (processID_CHAN==0){
            CHAN(ENC_CHAN_shared_mem_key_file,ENC_CHAN_shared_mem_size_file,CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file,ENC_semaphore_p1_key_file,CHAN_semaphore_p1_key_file);
            exit(0);
        }

         sleep(2);
         cout << "\n\n\n\n\n\n\n\n\n\n\n\n";

        //~~~~~~~~~~~~ENC2~~~~~~~~~~//
        pid_t processID_ENC2;
        #if DEBUG >= 1
           cout<<"!! starting ENC... !!\n";
        #endif
        processID_ENC2=fork();
        if(processID_ENC==-1) die("fork failed");
        else if (processID_ENC2==0){
           ENC(CHAN_ENC_shared_mem_key_file,CHAN_ENC_shared_mem_size_file,ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file,CHAN_semaphore_p1_key_file,ENC_semaphore_p1_key_file);
           exit(0);
        }

        sleep(2);
        cout << "\n\n\n\n\n\n\n\n\n\n\n\n";

       //~~~~~~~~~~~~P2~~~~~~~~~~//
       // pid_t processID_P2;
       // #if DEBUG >= 1
       //    cout<<"!! starting P2... !!\n";
       // #endif
       // processID_P2=fork();
       // if(processID_ENC==-1) die("fork failed");
       // else if (processID_P2==0){
       //    ENC(ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file,ENC_P2_shared_mem_key_file,ENC_P2_shared_mem_size_file,ENC_semaphore_p1_key_file,P_semaphore_p2_key_file);
       //    exit(0);
       // }


    }




    while(wait(&status) > 0);
    #if DEBUG >= 1
    cout<<"! ALL children finished"<<endl;
    #endif

    return 0;
}

int P(char* read_shared_mem_key_file,int read_shared_mem_size_file,char* write_shared_mem_key_file, int write_shared_mem_size_file,char* write_semaphore,char* read_semaphore){
    #if DEBUG >= 1
        cout<<"- P born now ID "<<getpid()<<" with parent  "<<getppid()<<endl;
    #endif
    //~~~~~~~~~~~~~~~~~~~semaphore~~~~~~~~~~~~~~~~~~~~~~~//
    int sem_read_id  = get_semaphore_id_from_file(read_semaphore);
    int sem_write_id = get_semaphore_id_from_file(write_semaphore);

    #if DEBUG >= 1
        printf("~ read P %d waiting\n", getpid());
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
    #if DEBUG >= 1
        //printf("~ read P->ENC %d waiting\n", getpid());
    #endif

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
    printf("~ write P %d releasing\n", getpid());
    #endif

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
    printf("~ENC %d waiting\n", getpid());
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




    //~~~~~~~~~~~~~~~~~WRITE TOO CHAN~~~~~~~~~~~~~~~~~~//

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
        printf("~ write ENC %d releasing\n", getpid());
    #endif
    //if
    return resend_flag;


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
    printf("~ read ENC %d waiting\n", getpid());
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
        strcpy(mess->message_arrey,"aaaaa");

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
    #if DEBUG >= 1
        printf("~ write ENC %d releasing\n", getpid());
    #endif

}
