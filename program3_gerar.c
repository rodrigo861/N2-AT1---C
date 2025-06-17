#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TOTAL_LEITURAS 2000

typedef enum { CONJ_Z, CONJ_Q, TEXTO, BINARIO } TipoSensor;

typedef struct {
    char id[50];
    TipoSensor tipo;
} Sensor;

long gerar_timestamp(long inicio, long fim) {
    return inicio + rand() % (fim - inicio + 1);
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        printf("Uso: %s <inicio_timestamp> <fim_timestamp> <sensor1_id>:<tipo> <sensor2_id>:<tipo> ...\n", argv[0]);
        return 1;
    }

    long inicio = atol(argv[1]);
    long fim = atol(argv[2]);

    if (inicio >= fim) {
        printf("Timestamp inicial deve ser menor que o final.\n");
        return 1;
    }

    int total_sensores = argc - 3;
    Sensor* sensores = malloc(total_sensores * sizeof(Sensor));

    for (int i = 0; i < total_sensores; i++) {
        char* token = strtok(argv[i + 3], ":");
        strcpy(sensores[i].id, token);

        token = strtok(NULL, ":");
        if (strcmp(token, "CONJ_Z") == 0) sensores[i].tipo = CONJ_Z;
        else if (strcmp(token, "CONJ_Q") == 0) sensores[i].tipo = CONJ_Q;
        else if (strcmp(token, "TEXTO") == 0) sensores[i].tipo = TEXTO;
        else if (strcmp(token, "BINARIO") == 0) sensores[i].tipo = BINARIO;
        else {
            printf("Tipo desconhecido: %s\n", token);
            free(sensores);
            return 1;
        }
    }

    srand(time(NULL));

    FILE* saida = fopen("dados_teste.txt", "w");
    if (!saida) {
        printf("Erro ao criar arquivo de saída.\n");
        free(sensores);
        return 1;
    }

    char textos[][20] = {"Alfa", "Beta", "Gamma", "Delta", "Epsilon"};

    for (int i = 0; i < total_sensores; i++) {
        for (int j = 0; j < TOTAL_LEITURAS; j++) {
            long timestamp = gerar_timestamp(inicio, fim);
            switch (sensores[i].tipo) {
                case CONJ_Z:
                    fprintf(saida, "%ld %s %d\n", timestamp, sensores[i].id, rand() % 100);
                    break;
                case CONJ_Q:
                    fprintf(saida, "%ld %s %.2f\n", timestamp, sensores[i].id, (rand() % 10000) / 100.0);
                    break;
                case TEXTO:
                    fprintf(saida, "%ld %s %s\n", timestamp, sensores[i].id, textos[rand() % 5]);
                    break;
                case BINARIO:
                    fprintf(saida, "%ld %s %s\n", timestamp, sensores[i].id, (rand() % 2) ? "true" : "false");
                    break;
            }
        }
    }

    fclose(saida);
    free(sensores);

    printf("Arquivo dados_teste.txt gerado com sucesso.\n");
    return 0;
}
