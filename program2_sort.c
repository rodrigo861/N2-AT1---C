#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    long timestamp;
    char sensor_id[50];
    char value[100];
} Reading;

int compare_desc(const void* a, const void* b) {
    Reading* r1 = (Reading*)a;
    Reading* r2 = (Reading*)b;
    if (r2->timestamp > r1->timestamp) return 1;
    if (r2->timestamp < r1->timestamp) return -1;
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Uso: %s arquivo de entrada\n", argv[0]);
        return 1;
    }

    FILE* input_file = fopen(argv[1], "r");
    if (!input_file) {
        printf("Erro ao abrir o arquivo de entrada.\nn");
        return 1;
    }

    Reading* readings = malloc(sizeof(Reading) * 1000);
    int capacity = 1000;
    int count = 0;
    char line[256];

    while (fgets(line, sizeof(line), input_file)) {
        if (count >= capacity) {
            capacity *= 2;
            readings = realloc(readings, capacity * sizeof(Reading));
        }
        sscanf(line, "%ld %s %[^\n]", &readings[count].timestamp, readings[count].sensor_id, readings[count].value);
        count++;
    }
    fclose(input_file);

    qsort(readings, count, sizeof(Reading), compare_desc);

    FILE* output_file = fopen("decrescente.txt", "w");
    if (!output_file) {
        printf("Erro ao criar arquivo decrescente.txt.\n");
        free(readings);
        return 1;
    }

    for (int i = 0; i < count; i++) {
        fprintf(output_file, "%ld %s %s\n", readings[i].timestamp, readings[i].sensor_id, readings[i].value);
    }
    fclose(output_file);

    for (int i = 0; i < count; i++) {
        char sensor_filename[100];
        sprintf(sensor_filename, "%s.txt", readings[i].sensor_id);

        FILE* sensor_file = fopen(sensor_filename, "a");
        if (sensor_file) {
            fprintf(sensor_file, "%ld %s %s\n", readings[i].timestamp, readings[i].sensor_id, readings[i].value);
            fclose(sensor_file);
        } else {
            printf("Erro ao criar arquivo para sensor %s\n", readings[i].sensor_id);
        }
    }

    free(readings);
    printf("Arquivos decrescente com sucesso.\n");
    return 0;
}
