#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct {
    long timestamp;
    char id_sensor[50];
    char valor[100];
} Leitura;

int comparar_desc(const void* a, const void* b) {
    Leitura* l1 = (Leitura*)a;
    Leitura* l2 = (Leitura*)b;
    if (l2->timestamp > l1->timestamp) return 1;
    if (l2->timestamp < l1->timestamp) return -1;
    return 0;
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        printf("Uso: %s <arquivo_entrada>\n", argv[0]);
        return 1;
    }

    FILE* arq = fopen(argv[1], "r");
    if (!arq) {
        printf("Erro ao abrir o arquivo de entrada.\n");
        return 1;
    }

    Leitura* leituras = malloc(sizeof(Leitura) * 1000);
    int capacidade = 1000;
    int total = 0;
    char linha[256];

    while (fgets(linha, sizeof(linha), arq)) {
        if (total >= capacidade) {
            capacidade *= 2;
            leituras = realloc(leituras, capacidade * sizeof(Leitura));
        }
        sscanf(linha, "%ld %s %[^\n]", &leituras[total].timestamp, leituras[total].id_sensor, leituras[total].valor);
        total++;
    }
    fclose(arq);

    qsort(leituras, total, sizeof(Leitura), comparar_desc);

    FILE* saida = fopen("decrescente.txt", "w");
    if (!saida) {
        printf("Erro ao criar arquivo decrescente.txt.\n");
        free(leituras);
        return 1;
    }

    for (int i = 0; i < total; i++) {
        fprintf(saida, "%ld %s %s\n", leituras[i].timestamp, leituras[i].id_sensor, leituras[i].valor);
    }
    fclose(saida);

    for (int i = 0; i < total; i++) {
        char nome_arquivo[100];
        sprintf(nome_arquivo, "%s.txt", leituras[i].id_sensor);

        FILE* sensor_arq = fopen(nome_arquivo, "a"); 
        if (sensor_arq) {
            fprintf(sensor_arq, "%ld %s %s\n", leituras[i].timestamp, leituras[i].id_sensor, leituras[i].valor);
            fclose(sensor_arq);
        } else {
            printf("Erro ao criar arquivo para sensor %s\n", leituras[i].id_sensor);
        }
    }

    free(leituras);
    printf("Arquivos decrescente com sucesso.\n");
    return 0;
}
