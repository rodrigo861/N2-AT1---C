#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>
#include <locale.h>

#ifdef _WIN32
#include <windows.h>
#endif

typedef struct {
    long timestamp;
    char id_sensor[50];
    char valor[100];
} Leitura;

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

int validar_data(int ano, int mes, int dia, int hora, int minuto, int segundo) {
    if (ano < 1970 || mes < 1 || mes > 12 || dia < 1 || dia > 31 ||
        hora < 0 || hora > 23 || minuto < 0 || minuto > 59 || segundo < 0 || segundo > 59) {
        return 0;
    }

    if ((mes == 4 || mes == 6 || mes == 9 || mes == 11) && dia > 30) return 0;
    if (mes == 2) {
        int bissexto = (ano % 4 == 0 && (ano % 100 != 0 || ano % 400 == 0));
        if (dia > (bissexto ? 29 : 28)) return 0;
    }

    return 1;
}

long converter_para_timestamp(const char* data, const char* hora) {
    int ano, mes, dia, h, m, s;
    if (sscanf(data, "%d-%d-%d", &ano, &mes, &dia) != 3 ||
        sscanf(hora, "%d:%d:%d", &h, &m, &s) != 3) {
        fprintf(stderr, "Formato inválido. Use: AAAA-MM-DD HH:MM:SS\n");
        return -1;
    }

    if (!validar_data(ano, mes, dia, h, m, s)) {
        fprintf(stderr, "Data ou hora inválida.\n");
        return -1;
    }

    struct tm tm_data = {0};
    tm_data.tm_year = ano - 1900;
    tm_data.tm_mon  = mes - 1;
    tm_data.tm_mday = dia;
    tm_data.tm_hour = h;
    tm_data.tm_min  = m;
    tm_data.tm_sec  = s;
    tm_data.tm_isdst = -1;

    return mktime(&tm_data);
}

int main(int argc, char* argv[]) {
    // Suporte UTF-8 e português no Windows
    setlocale(LC_ALL, "Portuguese");

#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    if (argc != 4) {
        printf("Uso: %s <sensor> <AAAA-MM-DD> <HH:MM:SS>\n", argv[0]);
        return 1;
    }

    const char* sensor = argv[1];
    const char* data = argv[2];
    const char* hora = argv[3];

    long timestamp_alvo = converter_para_timestamp(data, hora);
    if (timestamp_alvo == -1) return 1;

    char nome_arquivo[100];
    sprintf(nome_arquivo, "%s.txt", sensor);

    FILE* arq = fopen(nome_arquivo, "r");
    if (!arq) {
        printf("Erro ao abrir o arquivo %s\n", nome_arquivo);
        return 1;
    }

    Leitura* leituras = malloc(sizeof(Leitura) * 1000);
    int capacidade = 1000, total = 0;
    char linha[256];

    while (fgets(linha, sizeof(linha), arq)) {
        if (total >= capacidade) {
            capacidade *= 2;
            leituras = realloc(leituras, capacidade * sizeof(Leitura));
        }
        sscanf(linha, "%ld %s %[^\n]", &leituras[total].timestamp,
               leituras[total].id_sensor, leituras[total].valor);
        total++;
    }
    fclose(arq);

    if (total == 0) {
        printf("Arquivo %s está vazio.\n", nome_arquivo);
        free(leituras);
        return 1;
    }

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
