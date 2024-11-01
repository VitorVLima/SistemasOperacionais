#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

//Nome :  Vitor Vinicius Lima Silva

pthread_mutex_t talheres[5];
pthread_mutex_t mutex_acesso;
int cont_filosofo_comendo = 0;

void* atividade_filosofos(void* arg) {
    int id = *(int*)arg;
    int max_filosofos_comendo = 2; // Número máximo de filósofos que podem comer ao mesmo tempo
   
    while (1) {
        printf("O Filosofo %d iniciou o descanco\n", id);
        sleep(5);
        printf("O Filosofo %d finalizou o descanco\n", id);

        // Tenta comer e caso tenha menos de 2 comendo ele acessa os talheres
        pthread_mutex_lock(&mutex_acesso);
        while (cont_filosofo_comendo >= max_filosofos_comendo) {
            pthread_mutex_unlock(&mutex_acesso);
            sleep(1); // Espera um segundo antes de tentar novamente
            pthread_mutex_lock(&mutex_acesso);
        }
        cont_filosofo_comendo++;
        pthread_mutex_unlock(&mutex_acesso);

        // O filósofo pega os talheres e começou a comer
        pthread_mutex_lock(&talheres[id]);
        printf("Filósofo %d pegou o talher à esquerda (%d).\n", id, id);
        pthread_mutex_lock(&talheres[(id + 1) % 5]); 
        printf("Filósofo %d pegou o talher à direita (%d).\n", id, (id + 1) % 5);
        printf("Filósofo %d está comendo.\n", id);
        sleep(1);

        // O filósofo terminou de comer e solta os talheres
        pthread_mutex_unlock(&talheres[id]);
        pthread_mutex_unlock(&talheres[(id + 1) % 5]);
        printf("Filósofo %d soltou os talheres da esquerda (%d) e da direita (%d).\n", id, id, (id + 1) % 5);

        // Atualiza o número de filósofos comendo
        pthread_mutex_lock(&mutex_acesso);
        cont_filosofo_comendo--;
        pthread_mutex_unlock(&mutex_acesso);
    }
}

int main() {
    pthread_t filosofos[5];
    int ids[5];

    for (int i = 0; i < 5; i++) {
        pthread_mutex_init(&talheres[i], NULL);
    }
    pthread_mutex_init(&mutex_acesso, NULL);

    for (int i = 0; i < 5; i++) {
        ids[i] = i;
        pthread_create(&filosofos[i], NULL, atividade_filosofos, &ids[i]);
    }

    for (int i = 0; i < 5; i++) {
        pthread_join(filosofos[i], NULL);
    }
    
    for (int i = 0; i < 5; i++) {
        pthread_mutex_destroy(&talheres[i]);
    }
    pthread_mutex_destroy(&mutex_acesso);

    return 0;
}

