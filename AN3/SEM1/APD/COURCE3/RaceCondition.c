/*
apare atunci cand 2 sau mai multe threaduri modifica aceeasi variabila sharuita, in
acelasi timp, fara control

int x = 0

//Threadul1
x++;

//Threadul2
x++;

astfel finalul poate fi 1 sau 2

*/

#include<stdio.h>
#include<pthread.h>

int count = 0;

void* increment(void* arg){
    for(int i = 0; i < 1000000; i++){
        count++;
        // ambele threaduri modifica simultan counterul si atunci 
        // ambele threduri acceseaza si modifica variabila. trebuiea 2000000 sa fie
    }

    return NULL;
}

int main(void){
    pthread_t t1;
    pthread_t t2;

    pthread_create(&t1,NULL,increment,NULL);
    pthread_create(&t2,NULL,increment,NULL);
    
    pthread_join(t1,NULL);
    pthread_join(t2,NULL);

    printf("Rezultatul final: %d\n",count);
    return 0;
}