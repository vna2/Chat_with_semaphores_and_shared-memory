#ifndef SEMAPHORE_SHARED_MEM
#define SEMAPHORE_SHARED_MEM

#define P_shared_mem_key_file "key/P-shared.mem.key"
#define P_shared_mem_size_file sizeof(message*)
#define P_ENC_shared_mem_key_file "key/P-ENC-shared.mem.key"
#define P_ENC_shared_mem_size_file sizeof(message*)
#define ENC_CHAN_shared_mem_key_file "key/ENC-CHAN-shared.mem.key"
#define ENC_CHAN_shared_mem_size_file sizeof(message*)
#define CHAN_ENC_shared_mem_key_file "key/CHAN-ENC-shared.mem.key"
#define CHAN_ENC_shared_mem_size_file sizeof(message*)
#define ENC_P2_shared_mem_key_file "key/ENC-P-shared.mem.key"
#define ENC_P2_shared_mem_size_file sizeof(message*)
#define P2_shared_mem_key_file "key/P2-shared.mem.key"
#define P2_shared_mem_size_file sizeof(message*)

#define P_semaphore_p1_key_file "key/p_p1.semaphore.key"
#define P_semaphore_p2_key_file "key/p_p2.semaphore.key"
#define P_semaphore_p3_key_file "key/p_p3.semaphore.key"
#define P_semaphore_p4_key_file "key/p_p4.semaphore.key"
#define P_semaphore_resend_p1_key_file "key/p_resend_p1.semaphore.key"

#define ENC_semaphore_p1_key_file "key/enc_p1.semaphore.key"
#define ENC_semaphore_p2_key_file "key/enc_p2.semaphore.key"
#define ENC_semaphore_p3_key_file "key/enc_p3.semaphore.key"
#define ENC_semaphore_p4_key_file "key/enc_p4.semaphore.key"
#define ENC_semaphore_resend_p1_key_file "key/enc_resend_p1.semaphore.key"


#define CHAN_semaphore_p1_key_file "key/CHAN_p1.semaphore.key"
#define CHAN_semaphore_p2_key_file "key/CHAN_p2.semaphore.key"
#define CHAN_semaphore_p3_key_file "key/CHAN_p3.semaphore.key"
#define CHAN_semaphore_p4_key_file "key/CHAN_p4.semaphore.key"
#define CHAN_semaphore_resend_p1_key_file "key/CHAN_resend_p1.semaphore.key"


#define ENC2_semaphore_p1_key_file "key/encc_p1.semaphore.key"
#define ENC2_semaphore_p2_key_file "key/encc_p2.semaphore.key"
#define ENC2_semaphore_p3_key_file "key/encc_p3.semaphore.key"
#define ENC2_semaphore_p4_key_file "key/encc_p4.semaphore.key"
#define ENC2_semaphore_resend_p1_key_file "key/encc_resend_p1.semaphore.key"


#define P2_semaphore_p1_key_file "key/p2_p1.semaphore.key"
#define P2_semaphore_p2_key_file "key/p2_p2.semaphore.key"
#define P2_semaphore_p3_key_file "key/p2_p3.semaphore.key"
#define P2_semaphore_p4_key_file "key/p2_p4.semaphore.key"
#define P2_semaphore_resend_p1_key_file "key/p2_resend_p1.semaphore.key"




#include "helping_functions.hpp"
void die(char er[1000]);


int generate_memory_segment(key_t mem_key, size_t mem_sz, char* file_name){
    int mem_seg_id,mem_fd;
    /* Allocate a shared memory id. */
    mem_seg_id = shmget (mem_key, mem_sz, IPC_CREAT | IPC_EXCL | S_IRUSR | S_IWUSR);
    if(mem_seg_id < 0)die("can not allocate memory segment");
    #if DEBUG>= 2
        else cout<<"! alocated memory with id "<<mem_seg_id<<endl;
        cout<<"! making shared memory file and writing key\n";
    #endif
    /*creating file for memory key*/
    mem_fd = open(file_name, O_WRONLY | O_TRUNC | O_EXCL | O_CREAT, 0644);
    if (mem_fd < 0)die("Could not open mem.id");
    /*writing memory id*/
    if (write(mem_fd, &mem_key, sizeof(key_t)) < 0) die("Could not write key to file");
    close(mem_fd);
    return mem_seg_id;
}

int get_memory_id_from_file(char* file_name, int mem_sz){
    int mem_fd,mem_seg_id;
    key_t mem_key;
    /*Recover mem_key from file*/
    mem_fd = open(file_name, O_RDONLY);
    if (mem_fd < 0) die("Could not open memory file for reading");
    if (read(mem_fd, &mem_key, sizeof(int)) != sizeof(int)) die("Error reading the memory key");
    close(mem_fd);
    /*optain mem id*/
    mem_seg_id=shmget(mem_key, mem_sz, 0666);
    #if DEBUG>= 2
        cout<<"- opened file and got memory id "<<mem_seg_id<<endl;
    #endif
    return mem_seg_id;
}

void clear_mem(char* mem_file, int mem_sz){
    /* Deallocate the shared memory segment. */
    if(shmctl (get_memory_id_from_file(mem_file,mem_sz), IPC_RMID, 0) < 0)die("can not dealocate memory");
    #if DEBUG >= 2
        cout<<"!! deleted memory !!\n";
    #endif
    //removing memory file
    if (unlink(mem_file) < 0) die("Could not unlink memory file ");
    #if DEBUG >= 2
        else printf("!! unlinked memory file !!\n");
    #endif
}


////semaphore////
int generate_semaphore(key_t sem_key, char* file_name, unsigned int num=0){
    char final_file_name[50]="\0";
    if(num!=0)
        sprintf(final_file_name,"%u",num);
    strcat(final_file_name,file_name);

    int sem_fd,sem_id;
    /*make file for semaphore key*/
    #if DEBUG >= 2
        cout<<"!! making shemaphore file and writing key!!\n";
    #endif
    sem_fd = open(final_file_name, O_WRONLY | O_TRUNC | O_EXCL | O_CREAT, 0644);
    if (sem_fd < 0)die("Could not open sem.key");
    /*write the semaphore key*/
    if (write(sem_fd, &sem_key, sizeof(key_t)) < 0) die("Could not write key to file");
    close(sem_fd);
    /*creating shemapgore*/
    sem_id = semget(sem_key, 1, IPC_CREAT | IPC_EXCL | 0600);
    if (sem_id < 0) die("Could not create sem");
    #if DEBUG >= 2
        else  cout<<"!! created semaphore!! "<<sem_id << endl;
    #endif

    return sem_id;
}

int get_semaphore_id_from_file(char* file_name, unsigned int num=0){
    char final_file_name[50]="\0";
    if(num!=0)
        sprintf(final_file_name,"%u",num);
    strcat(final_file_name,file_name);

    int sem_fd, sem_id;
    key_t sem_key;
    /*Recover sem_key from file*/
    sem_fd = open(final_file_name, O_RDONLY);
    if (sem_fd < 0) die("Could not open sem key for reading");
    if (read(sem_fd, &sem_key, sizeof(key_t)) != sizeof(key_t)) die("Error reading the semaphore key");
    close(sem_fd);
    // Now obtain the (hopefully) existing sem
    sem_id = semget(sem_key, 0, 0);
    if (sem_id < 0) die("Could not obtain semaphore");
    #if DEBUG >= 2
    cout<<"- opened file and got semaphore "<<sem_id<<endl;
    #endif
    return sem_id;
}

void initialise_semaphore(int sem_id){
    /*seting shemaphore to 0*/
    if(semctl(sem_id, 0 , SETVAL, 0) < 0) die("Could not set value of semaphore");
    #if DEBUG >= 2
        else cout<<" and set to 1\n";
    #endif
}

static int semaphore_wait(int sem_id){
    #if DEBUG  >= 3
        cout<<"!! semaphore "<<sem_id<<"waiting!!\n";
    #endif
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = -1; /* P() */
    sem_b.sem_flg = 0;
    if (semop(sem_id, &sem_b, 1) == -1) {
        fprintf(stderr, "semaphore_p failed\n");
        return(0);
    }
    return(1);
}

static int semaphore_signal(int sem_id){
    #if DEBUG >= 3
        cout<<"!! semaphore "<<sem_id<<"releasing !! \n";
    #endif
    struct sembuf sem_b;
    sem_b.sem_num = 0;
    sem_b.sem_op = 1; /* V() */
    sem_b.sem_flg = 0;
    if (semop(sem_id, &sem_b, 1) == -1) {
        fprintf(stderr, "semaphore_v failed\n");
        return(0);
    }
    return(1);
}

void clear_sem(char* sem_file, unsigned int num=0){
    char final_file_name[50]="\0";
    if(num!=0)
        sprintf(final_file_name,"%u",num);
    strcat(final_file_name,sem_file);

    /*deleting semaphore*/
    if (semctl(get_semaphore_id_from_file(final_file_name), 0, IPC_RMID) < 0)die("Could not delete semaphore");
    #if DEBUG >= 2
        else cout<<"!! deleted semaphore!!\n";
    #endif
    //removing semaphore file
    if (unlink(final_file_name) < 0) die("Could not unlink key file");
    #if DEBUG >= 2
        else printf("!! unlinked semaphore file!!\n");
    #endif
}

void initialized_all_shared_memmory_semaphores(){

    //~~~~~~~~~~~~~~~~Generate_keys~~~~~~~~~~~~~~~~~~~~~~//
        key_t P_ENC_shared_mem_key =ftok("p.o",1);
        key_t P_ENC_shared_mem_size =ftok("p.o",2);
        key_t ENC_CHAN_shared_mem_key =ftok("p.o",3);
        key_t ENC_CHAN_shared_mem_size =ftok("p.o",4);
        key_t CHAN_ENC_shared_mem_key =ftok("p.o",5);
        key_t CHAN_ENC_shared_mem_size =ftok("p.o",6);
        key_t ENC_P2_shared_mem_key =ftok("p.o",7);
        key_t ENC_P2_shared_mem_size =ftok("p.o",8);
        key_t P_shared_mem_key =ftok("p.o",9);
        key_t P_shared_mem_size =ftok("p.o",10);
        key_t P2_shared_mem_key =ftok("p.o",11);
        key_t P2_shared_mem_size =ftok("p.o",12);

        key_t P_semaphore_p1_key =ftok("p.o",13);
        key_t P_semaphore_p2_key =ftok("p.o",14);
        key_t P_semaphore_p3_key =ftok("p.o",15);
        key_t P_semaphore_p4_key =ftok("p.o",32);
        key_t P_semaphore_resend_p1_key =ftok("p.o",33);

        key_t ENC_semaphore_p1_key =ftok("p.o",16);
        key_t ENC_semaphore_p2_key =ftok("p.o",17);
        key_t ENC_semaphore_p3_key =ftok("p.o",18);
        key_t ENC_semaphore_p4_key =ftok("p.o",19);
        key_t ENC_semaphore_resend_p1_key =ftok("p.o",34);

        key_t CHAN_semaphore_p1_key =ftok("p.o",20);
        key_t CHAN_semaphore_p2_key =ftok("p.o",21);
        key_t CHAN_semaphore_p3_key =ftok("p.o",22);
        key_t CHAN_semaphore_p4_key =ftok("p.o",23);
        key_t CHAN_semaphore_resend_p1_key =ftok("p.o",35);


        key_t ENC2_semaphore_p1_key=ftok("p.o",24);
        key_t ENC2_semaphore_p2_key =ftok("p.o",25);
        key_t ENC2_semaphore_p3_key =ftok("p.o",26);
        key_t ENC2_semaphore_p4_key =ftok("p.o",27);
        key_t ENC2_semaphore_resend_p1_key =ftok("p.o",36);


        key_t P2_semaphore_p1_key =ftok("p.o",28);
        key_t P2_semaphore_p2_key =ftok("p.o",29);
        key_t P2_semaphore_p3_key =ftok("p.o",30);
        key_t P2_semaphore_p4_key =ftok("p.o",31);



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

        //Shared_memory for P2
        generate_memory_segment(P2_shared_mem_key,P2_shared_mem_size,P2_shared_mem_key_file);
        int P2_mem_id=get_memory_id_from_file(P2_shared_mem_key_file,P2_shared_mem_size);
        char* mem_P2 = (char*) shmat(P2_mem_id, NULL, 0);
        if(mem_P==(void*)-1)die("shared memory main");
        #if DEBUG >= 2
            printf ("!! shared mem_P attached at main and initialized !!\n");
        #endif


    //~~~~~~~~~~~~~~~~Semaphore~~~~~~~~~~~~~~~~~~~~~~~~~//
        int sem_P_p1=generate_semaphore(P_semaphore_p1_key,P_semaphore_p1_key_file);
        initialise_semaphore(sem_P_p1);
        int sem_P_p2=generate_semaphore(P_semaphore_p2_key, P_semaphore_p2_key_file);
        initialise_semaphore(sem_P_p2);
        int sem_P_p3=generate_semaphore(P_semaphore_p3_key, P_semaphore_p3_key_file);
        initialise_semaphore(sem_P_p3);
        int sem_P_p4=generate_semaphore(P_semaphore_p4_key, P_semaphore_p4_key_file);
        initialise_semaphore(sem_P_p4);
        int sem_resend_P_p1=generate_semaphore(P_semaphore_resend_p1_key, P_semaphore_resend_p1_key_file);
        initialise_semaphore(sem_resend_P_p1);


        int sem_ENC_p1=generate_semaphore(ENC_semaphore_p1_key, ENC_semaphore_p1_key_file);
        initialise_semaphore(sem_ENC_p1);
        int sem_ENC_p2=generate_semaphore(ENC_semaphore_p2_key, ENC_semaphore_p2_key_file);
        initialise_semaphore(sem_ENC_p2);
        int sem_ENC_p3=generate_semaphore(ENC_semaphore_p3_key, ENC_semaphore_p3_key_file);
        initialise_semaphore(sem_ENC_p3);
        int sem_ENC_p4=generate_semaphore(ENC_semaphore_p4_key, ENC_semaphore_p4_key_file);
        initialise_semaphore(sem_ENC_p4);
        int sem_resend_ENC_p1=generate_semaphore(ENC_semaphore_resend_p1_key, ENC_semaphore_resend_p1_key_file);
        initialise_semaphore(sem_resend_ENC_p1);

        int sem_CHAN_p1=generate_semaphore(CHAN_semaphore_p1_key, CHAN_semaphore_p1_key_file);
        initialise_semaphore(sem_CHAN_p1);
        int sem_CHAN_p2=generate_semaphore(CHAN_semaphore_p2_key, CHAN_semaphore_p2_key_file);
        initialise_semaphore(sem_CHAN_p2);
        int sem_CHAN_p3=generate_semaphore(CHAN_semaphore_p3_key, CHAN_semaphore_p3_key_file);
        initialise_semaphore(sem_CHAN_p3);
        int sem_CHAN_p4=generate_semaphore(CHAN_semaphore_p4_key, CHAN_semaphore_p4_key_file);
        initialise_semaphore(sem_CHAN_p4);
        int sem_resend_CHAN_p1=generate_semaphore(CHAN_semaphore_resend_p1_key, CHAN_semaphore_resend_p1_key_file);
        initialise_semaphore(sem_resend_CHAN_p1);

        int sem_ENC2_p1=generate_semaphore(ENC2_semaphore_p1_key, ENC2_semaphore_p1_key_file);
        initialise_semaphore(sem_ENC2_p1);
        int sem_ENC2_p2=generate_semaphore(ENC2_semaphore_p2_key, ENC2_semaphore_p2_key_file);
        initialise_semaphore(sem_ENC2_p2);
        int sem_ENC2_p3=generate_semaphore(ENC2_semaphore_p3_key, ENC2_semaphore_p3_key_file);
        initialise_semaphore(sem_ENC2_p3);
        int sem_ENC2_p4=generate_semaphore(ENC2_semaphore_p4_key, ENC2_semaphore_p4_key_file);
        initialise_semaphore(sem_ENC2_p4);
        int sem_resend_ENC2_p1=generate_semaphore(ENC2_semaphore_resend_p1_key, ENC2_semaphore_resend_p1_key_file);
        initialise_semaphore(sem_resend_ENC2_p1);


        int sem_P2_p1=generate_semaphore(P2_semaphore_p1_key,P2_semaphore_p1_key_file);
        initialise_semaphore(sem_P2_p1);
        int sem_P2_p2=generate_semaphore(P2_semaphore_p2_key, P2_semaphore_p2_key_file);
        initialise_semaphore(sem_P2_p2);
        int sem_P2_p3=generate_semaphore(P2_semaphore_p3_key, P2_semaphore_p3_key_file);
        initialise_semaphore(sem_P2_p3);
        int sem_P2_p4=generate_semaphore(P2_semaphore_p4_key, P2_semaphore_p4_key_file);
        initialise_semaphore(sem_P2_p4);

}




void die(char er[1000]){//for handling errors
    int fd = open("error.txt", O_WRONLY | O_TRUNC | O_EXCL | O_CREAT, 0644);
    write(fd, er, strlen(er));
    close(fd);
    perror(er);
    exit(1);
}



#endif
