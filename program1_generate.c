#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define TOTAL_READINGS 2000

typedef enum { CONJ_Z, CONJ_Q, TEXTO, BINARIO } SensorType;

typedef struct {
    char id[50];
    SensorType type;
} Sensor;

long generate_random_timestamp(long start, long end) {
    return start + rand() % (end - start + 1);
}

long convert_to_timestamp(const char* date, const char* time) {
    struct tm datetime = {0};
    if (sscanf(date, "%d/%d/%d", &datetime.tm_mday, &datetime.tm_mon, &datetime.tm_year) != 3) {
        fprintf(stderr, "Formato de data invalido. Use DD/MM/AAAA\n");
        return -1;
    }
    if (sscanf(time, "%d:%d:%d", &datetime.tm_hour, &datetime.tm_min, &datetime.tm_sec) != 3) {
        fprintf(stderr, "Formato de hora invalido. Use HH:MM:SS\n");
        return -1;
    }

    datetime.tm_year -= 1900;
    datetime.tm_mon -= 1;
    datetime.tm_isdst = -1;

    return mktime(&datetime);
}

int main(int argc, char* argv[]) {
    if (argc < 6) {
        printf("Necessario usar: ./gerar <data_inicio> <hora_inicio> <data_fim> <hora_fim> <sensor1_id>:<tipo> ...\n\n");
        printf("Exemplo: ./gerar 14/06/2025 15:00:00 15/06/2025 18:33:20 temperatura:CONJ_Q umidade:CONJ_Z\n");
        return 1;
    }

    long start = convert_to_timestamp(argv[1], argv[2]);
    long end = convert_to_timestamp(argv[3], argv[4]);

    if (start == -1 || end == -1) return 1;
    if (start >= end) {
        printf("Erro na data. A data inicial deve ser menor que a final .\n");
        return 1;
    }

    int sensor_count = argc - 5;
    Sensor* sensors = malloc(sensor_count * sizeof(Sensor));
    if (!sensors) {
        printf("Erro de alocaçao de memoria.\n");
        return 1;
    }

    for (int i = 0; i < sensor_count; i++) {
        char* input = argv[i + 5];
        char* separator = strchr(input, ':');

        if (!separator) {
            printf("Erro ao gerar sensor, '%s' formato incorreto. Use <nome>:<tipo>\n", input);
            free(sensors);
            return 1;
        }

        *separator = '\0';
        strcpy(sensors[i].id, input);
        char* type_str = separator + 1;

        if (strlen(sensors[i].id) == 0) {
            printf("Erro, nome do sensor esta vazio em '%s'\n", input);
            free(sensors);
            return 1;
        }

        if (strcmp(type_str, "CONJ_Z") == 0) sensors[i].type = CONJ_Z;
        else if (strcmp(type_str, "CONJ_Q") == 0) sensors[i].type = CONJ_Q;
        else if (strcmp(type_str, "TEXTO") == 0) sensors[i].type = TEXTO;
        else if (strcmp(type_str, "BINARIO") == 0) sensors[i].type = BINARIO;
        else {
            printf("Erro no tipo '%s' em '%s', desconhecido\n", type_str, input);
            free(sensors);
            return 1;
        }
    }

    srand(time(NULL));

    FILE* output = fopen("sensors.txt", "w");
    if (!output) {
        printf("Nao foi possivel criar o arquivo de saida.\n");
        free(sensors);
        return 1;
    }

    char text_values[][20] = {"Norte", "Sul", "Leste", "Oeste", "Centro"};

    for (int i = 0; i < sensor_count; i++) {
        for (int j = 0; j < TOTAL_READINGS; j++) {
            long timestamp = generate_random_timestamp(start, end);
            switch (sensors[i].type) {
                case CONJ_Z:
                    fprintf(output, "%ld %s %d\n", timestamp, sensors[i].id, rand() % 100);
                    break;
                case CONJ_Q:
                    fprintf(output, "%ld %s %.2f\n", timestamp, sensors[i].id, (rand() % 10000) / 100.0);
                    break;
                case TEXTO:
                    fprintf(output, "%ld %s %s\n", timestamp, sensors[i].id, text_values[rand() % 5]);
                    break;
                case BINARIO:
                    fprintf(output, "%ld %s %s\n", timestamp, sensors[i].id, (rand() % 2) ? "true" : "false");
                    break;
            }
        }
    }

    fclose(output);
    free(sensors);

    printf("Arquivo sensors.txt gerado com sucesso.\n");
    return 0;
}
