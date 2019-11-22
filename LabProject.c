/* 
Create a multithread program consisting of one producer
(random times) and two consumers.
A producer produces a product and stops after 50 products are completed.
Each consumer consumes a product if any exists.
* Consumers should compete for products
* Each consumer obtains a fair number of products (Peterson Algorithm)
Producer and consumers run concurrently
Print each step and the number of products consumed by the consumer.
*/
#if 0
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/wait.h>

#define Produced 100

int totalProduced = 0;
int count = 0;
int first = 0;
int second = 0; 
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER; // Setting Up MUTEX


void* Producers(void* arg){
	while(count < Produced){
		//printf("Producers has produced %d out of %d\n", count + 1, Produced);
		pthread_mutex_lock(&mutex1); // Critical Section
		totalProduced++;
		count++;
		pthread_mutex_unlock(&mutex1); // End of Critical Section
		sleep(1);
		
	}
	pthread_exit(0); // Exit Thread.
}

void Taken(int which){
	if (which == 1){
		//printf("First Consumer Took it\n");
		first++;
	}
	if (which == 2){
		//printf("Second Consumer Took it\n");
		second++;
	}
	totalProduced--;
}

void* firstConsumer(void* arg){
	while(count != Produced || totalProduced != 0){
		pthread_mutex_lock(&mutex1); // Critical Section
		if(totalProduced!=0){
			Taken(1);
		}
		pthread_mutex_unlock(&mutex1); // End of Critical Section
		//printf("First Consumer is Waiting\n");
		//sleep(1);
	}
	pthread_exit(0);
}

void* secondConsumer(void* arg){
	while(count != Produced || totalProduced != 0){
		pthread_mutex_lock(&mutex1); // Critical Section
		if(totalProduced!=0){
			Taken(2);
		}
		pthread_mutex_unlock(&mutex1); // End of Critical Section
		//printf("Second Consumer is Waiting\n");
		//sleep(1);
	}
	pthread_exit(0);
}

int main(){
	pthread_t Pro;
	pthread_t Con1;
	pthread_t Con2;
	pthread_mutex_init(&mutex1,NULL); // Initializing Mutex

    pthread_create(&Pro, NULL, Producers, NULL); 
    pthread_create(&Con1, NULL, firstConsumer, NULL); 
	pthread_create(&Con2, NULL, secondConsumer, NULL); 
	pthread_join(Pro,NULL);
	pthread_join(Con1,NULL);
	pthread_join(Con2,NULL);
	printf("Consumer 1: %d\n", first);
	printf("Consumer 2: %d\n", second);
	printf("count %d\n",count);
   	pthread_mutex_destroy(&mutex1); // Destroying Mutex
   	return 0;
}

#else

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>
#include <stdbool.h>
#include <sys/wait.h>

#define Produced 100000

int totalProduced = 0;
int count = 0;
int first = 0;
int second = 0; 
char turn = 'i';
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER; // Setting Up MUTEX


void* Producers(void* arg){
	while(count < Produced){
		//printf("Producers has produced %d out of %d\n", count + 1, Produced);
		pthread_mutex_lock(&mutex1); // Critical Section
		totalProduced++;
		count++;
		pthread_mutex_unlock(&mutex1); // End of Critical Section
		sleep(1);
	}
	pthread_exit(0); // Exit Thread.
}

void Taken(int which){
	if (which == 1){
		//printf("First Consumer Took it\n");
		first++;
	}
	if (which == 2){
		//printf("Second Consumer Took it\n");
		second++;
	}
	totalProduced--;
}

void* firstConsumer(void* arg){
	while(count != Produced || totalProduced != 0){
		pthread_mutex_lock(&mutex1); // Critical Section
		if(totalProduced!= 0 && turn == 'i'){
			Taken(1);
			turn = 'j';
			// flagj = true;
			// flagi = false;
		}
		pthread_mutex_unlock(&mutex1); // End of Critical Section
		//printf("First Consumer is Waiting\n");
		//sleep(1);
	}
	pthread_exit(0);
}

void* secondConsumer(void* arg){
	while(count != Produced || totalProduced != 0){
		pthread_mutex_lock(&mutex1); // Critical Section
		if(totalProduced!= 0 && turn == 'j'){
			Taken(2);
			turn = 'i';
		}
		pthread_mutex_unlock(&mutex1); // End of Critical Section
		//printf("Second Consumer is Waiting\n");
		//sleep(1);
	}
	pthread_exit(0);
}

int main(){
	pthread_t Pro;
	pthread_t Con1;
	pthread_t Con2;
	pthread_mutex_init(&mutex1,NULL); // Initializing Mutex

    pthread_create(&Pro, NULL, Producers, NULL); 
    pthread_create(&Con1, NULL, firstConsumer, NULL); 
	pthread_create(&Con2, NULL, secondConsumer, NULL); 
	pthread_join(Pro,NULL);
	pthread_join(Con1,NULL);
	pthread_join(Con2,NULL);
	printf("Consumer 1: %d\n", first);
	printf("Consumer 2: %d\n", second);
	printf("count %d\n",count);
   	pthread_mutex_destroy(&mutex1); // Destroying Mutex
   	return 0;
}

#endif