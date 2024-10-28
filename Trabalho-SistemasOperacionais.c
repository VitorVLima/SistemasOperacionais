#include <stdio.h>
#include <pthread.h>
#include <unistd.h>

#define NUM_FILOSOFOS 5
#define MAX_FILOSOFOS_COMENDO 2 // Número máximo de filósofos que podem comer ao mesmo tempo

pthread_mutex_t talheres[NUM_FILOSOFOS]; // Talheres
pthread_mutex_t mutex_acesso; // Para controlar o acesso ao número de filósofos comendo
int cont_filosofo_comendo = 0; // Contador de filósofos comendo

void* atividade_filosofos(void* arg) {
    int id = *(int*)arg;

    while (1) {
        // O filósofo está pensando
        printf("Filósofo %d está pensando.\n", id);
        sleep(5); // O filósofo pensa por 5 segundos

        // Tenta comer
        pthread_mutex_lock(&mutex_acesso);
        while (cont_filosofo_comendo >= MAX_FILOSOFOS_COMENDO) {
            pthread_mutex_unlock(&mutex_acesso);
            sleep(1); // Espera um segundo antes de tentar novamente
            pthread_mutex_lock(&mutex_acesso);
        }
        cont_filosofo_comendo++;
        pthread_mutex_unlock(&mutex_acesso);

        // O filósofo pega os talheres
        pthread_mutex_lock(&talheres[id]); // Pega o talher à esquerda
        printf("Filósofo %d pegou o talher à esquerda (%d).\n", id, id);
        
        pthread_mutex_lock(&talheres[(id + 1) % NUM_FILOSOFOS]); // Pega o talher à direita
        printf("Filósofo %d pegou o talher à direita (%d).\n", id, (id + 1) % NUM_FILOSOFOS);

        // O filósofo come
        printf("Filósofo %d está comendo.\n", id);
        sleep(1); // O filósofo come por 1 segundo

        // O filósofo solta os talheres
        pthread_mutex_unlock(&talheres[id]);
        pthread_mutex_unlock(&talheres[(id + 1) % NUM_FILOSOFOS]);
        printf("Filósofo %d soltou os talheres da esquerda (%d) e da direita (%d).\n", id, id, (id + 1) % NUM_FILOSOFOS);

        // Atualiza o número de filósofos comendo
        pthread_mutex_lock(&mutex_acesso);
        cont_filosofo_comendo--;
        pthread_mutex_unlock(&mutex_acesso);
    }

    return NULL;
}

int main() {
    pthread_t filosofos[NUM_FILOSOFOS];
    int ids[NUM_FILOSOFOS];

    // Inicializa os mutexes
    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_mutex_init(&talheres[i], NULL);
    }
    pthread_mutex_init(&mutex_acesso, NULL);

    // Cria as threads dos filósofos
    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        ids[i] = i;
        pthread_create(&filosofos[i], NULL, atividade_filosofos, (void*)&ids[i]);
    }


    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_join(filosofos[i], NULL);
    }

    
    for (int i = 0; i < NUM_FILOSOFOS; i++) {
        pthread_mutex_destroy(&talheres[i]);
    }
    pthread_mutex_destroy(&mutex_acesso);

    return 0;
}


