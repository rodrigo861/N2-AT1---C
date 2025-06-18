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

long converter_data_hora_para_timestamp(const char* data, const char* hora) {
    struct tm tm_data = {0};
    if (sscanf(data, "%d/%d/%d", &tm_data.tm_mday, &tm_data.tm_mon, &tm_data.tm_year) != 3) {
        fprintf(stderr, "Formato de data invalido. Use DD/MM/AAAA\n");
        return -1;
    }
    if (sscanf(hora, "%d:%d:%d", &tm_data.tm_hour, &tm_data.tm_min, &tm_data.tm_sec) != 3) {
        fprintf(stderr, "Formato de hora invalido. Use HH:MM:SS\n");
        return -1;
    }

    tm_data.tm_year -= 1900;
    tm_data.tm_mon -= 1;
    tm_data.tm_isdst = -1;

    return mktime(&tm_data);
}

int main(int argc, char* argv[]) {
    if (argc < 6) {
        printf("Necessario usar: ./gerar <data_inicio> <hora_inicio> <data_fim> <hora_fim> <sensor1_id>:<tipo> ...\n", argv[0]);
        printf("Exemplo: ./gerar 14/06/2025 15:00:00 15/06/2025 18:33:20 temperatura:CONJ_Q umidade:CONJ_Z\n");
        return 1;
    }

    long inicio = converter_data_hora_para_timestamp(argv[1], argv[2]);
    long fim = converter_data_hora_para_timestamp(argv[3], argv[4]);

    if (inicio == -1 || fim == -1) return 1;
    if (inicio >= fim) {
        printf("Erro na data. A data inicial deve ser menor que a final .\n");
        return 1;
    }

    int total_sensores = argc - 5;
    Sensor* sensores = malloc(total_sensores * sizeof(Sensor));
    if (!sensores) {
        printf("Erro de alocaçao de memoria.\n");
        return 1;
    }

    for (int i = 0; i < total_sensores; i++) {
        char* entrada = argv[i + 5];
        char* separador = strchr(entrada, ':');

        if (!separador) {
            printf("Erro ao gerar sensor, '%s' formato incorreto. Use <nome>:<tipo>\n", entrada);
            free(sensores);
            return 1;
        }

        *separador = '\0';
        strcpy(sensores[i].id, entrada);
        char* tipo_str = separador + 1;

        if (strlen(sensores[i].id) == 0) {
            printf("Erro, nome do sensor esta vazio em '%s'\n", entrada);
            free(sensores);
            return 1;
        }

        if (strcmp(tipo_str, "CONJ_Z") == 0) sensores[i].tipo = CONJ_Z;
        else if (strcmp(tipo_str, "CONJ_Q") == 0) sensores[i].tipo = CONJ_Q;
        else if (strcmp(tipo_str, "TEXTO") == 0) sensores[i].tipo = TEXTO;
        else if (strcmp(tipo_str, "BINARIO") == 0) sensores[i].tipo = BINARIO;
        else {
            printf("Erro no tipo '%s' em '%s', desconhecido\n", tipo_str, entrada);
            free(sensores);
            return 1;
        }
    }

    srand(time(NULL));

    FILE* saida = fopen("sensores.txt", "w");
    if (!saida) {
        printf("Nao foi possivel criar o arquivo de saida.\n");
        free(sensores);
        return 1;
    }

    char textos[][20] = {"Norte", "Sul", "Leste", "Oeste", "Centro"};

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

    printf("Arquivo sensores.txt gerado com sucesso.\n");
    return 0;
}
