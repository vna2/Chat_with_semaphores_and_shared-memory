#ifndef HELPING_FUNCTION_INCLUDED
#define HELPING_FUNCTION_INCLUDED

#include <sys/types.h>
#include <sys/wait.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/ipc.h>
#include <sys/sem.h>

#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <string>

#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <string.h>
#include <limits>

#include "semaphores_sheared_mem.hpp"


#define BUFFER_MESSEGE_LENGTH  500
#define NUM_OF_MESSEGES 100
#define DEBUG 2

using namespace std;

class message{
  public:
    //int message_length ;
    char message_arrey[BUFFER_MESSEGE_LENGTH];
    char checksum[BUFFER_MESSEGE_LENGTH];
    int flag_checksum;//0 if not changed 1 if changed
    message(){}
    message(char* message){
        //this->message_length = strlen(message);
        strcpy(message_arrey,message);
        //cout << message_arrey << endl;
        #if DEBUG>= 3
            cout << "!! Constractor message end his job \n";
        #endif
    }
    void print(){
        cout << "\n\n ~~~~~~~~~~~~~~~message print~~~~~~~~~~~~~~~~~\n";
        cout << "\t message: "<< message_arrey << '\n' <<"\t checksum:  " << checksum << "\t flag checksum: "<< flag_checksum << endl << endl;

    }
};

// class queue{
//     int size, start, finish;
//     P table[NUM_OF_MESSEGES];
//  public:
//     queue(int a=MAX_N){
//         #if DEBUG>= 3
//             cout<<"\n\n message queue\n\n";
//         #endif
//         size=0;
//         start=0;
//         finish=0;
//     }
//     ~queue(){
//         free(table);
//     }
//     void push(P item){
//         table[finish]=item;
//         finish++;
//         size++;
//     }
//     P pop(){
//         if(size==0)
//             return NULL;
//         P tmp=table[start];
//         start++;
//         size--;
//         return tmp;
//     }
//     void print(){
//         cout<<"----list\n";
//         for(int i=start;i<finish;i++)
//             table[i].print();
//         cout<<"----\n";
//     }
//     bool isempty(){
//         return size==0;
//     }
//     int getsize(){
//         return size;
//     }
// };

#endif
