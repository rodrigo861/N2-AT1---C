#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

typedef struct {
    long timestamp;
    char id_sensor[50];
    char valor[100];
} Leitura;

long converter_para_timestamp(const char* data, const char* hora) {
    struct tm t = {0};
    int dia, mes, ano, hora_d, minuto, segundo;
    if (sscanf(data, "%d/%d/%d", &dia, &mes, &ano) != 3) {
        printf("Erro: Formato de data inválido. Use DD/MM/AAAA\n");
        return -1;
    }
    if (sscanf(hora, "%d:%d:%d", &hora_d, &minuto, &segundo) != 3) {
        printf("Erro: Formato de hora inválido. Use HH:MM:SS\n");
        return -1;
    }

    t.tm_mday = dia;
    t.tm_mon = mes - 1;
    t.tm_year = ano - 1900;
    t.tm_hour = hora_d;
    t.tm_min = minuto;
    t.tm_sec = segundo;

    return mktime(&t);
}

int busca_binaria_decrescente(Leitura* leituras, int tamanho, long alvo) {
    int esquerda = 0, direita = tamanho - 1;
    int melhor_indice = -1;
    long menor_diferenca = LONG_MAX;

    while (esquerda <= direita) {
        int meio = esquerda + (direita - esquerda) / 2;
        long diff = labs(leituras[meio].timestamp - alvo);

        if (diff < menor_diferenca) {
            menor_diferenca = diff;
            melhor_indice = meio;
        }

        if (leituras[meio].timestamp < alvo) {
            direita = meio - 1;
        } else if (leituras[meio].timestamp > alvo) {
            esquerda = meio + 1;
        } else {
            return meio;
        }
    }
    return melhor_indice;
}

int main(int argc, char* argv[]) {
    if (argc < 4) {
        printf("Uso: %s <nome_sensor> <DD/MM/AAAA> <HH:MM:SS>\n", argv[0]);
        return 1;
    }

    const char* nome_sensor = argv[1];
    const char* data = argv[2];
    const char* hora = argv[3];

    long timestamp_alvo = converter_para_timestamp(data, hora);
    if (timestamp_alvo == -1) return 1;

    char nome_arquivo[100];
    sprintf(nome_arquivo, "%s.txt", nome_sensor);

    FILE* arq = fopen(nome_arquivo, "r");
    if (!arq) {
        printf("Erro ao abrir o arquivo %s\n", nome_arquivo);
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

    int indice = busca_binaria_decrescente(leituras, total, timestamp_alvo);

    if (indice >= 0) {
        printf("Leitura mais próxima:\n");
        printf("Timestamp: %ld\n", leituras[indice].timestamp);
        printf("Sensor: %s\n", leituras[indice].id_sensor);
        printf("Valor: %s\n", leituras[indice].valor);
    } else {
        printf("Nenhuma leitura encontrada.\n");
    }

    free(leituras);
    return 0;
}
