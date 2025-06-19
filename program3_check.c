#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <limits.h>

#ifdef _WIN32
#include <windows.h>
#endif

typedef struct {
    long timestamp;
    char sensor_name[50];
    char value[100];
} SensorRecord;

int binary_search_descending(SensorRecord* records, int size, long target) {
    int left = 0, right = size - 1;
    int closest_index = -1;
    long smallest_diff = LONG_MAX;

    while (left <= right) {
        int mid = left + (right - left) / 2;
        long diff = labs(records[mid].timestamp - target);

        if (diff < smallest_diff) {
            smallest_diff = diff;
            closest_index = mid;
        }

        if (records[mid].timestamp < target) {
            right = mid - 1;
        } else if (records[mid].timestamp > target) {
            left = mid + 1;
        } else {
            return mid;
        }
    }
    return closest_index;
}

int validate_datetime(int day, int month, int year, int hour, int minute, int second) {
    if (year < 1970 || month < 1 || month > 12 || day < 1 || day > 31 ||
        hour < 0 || hour > 23 || minute < 0 || minute > 59 || second < 0 || second > 59) {
        return 0;
    }

    if ((month == 4 || month == 6 || month == 9 || month == 11) && day > 30) return 0;
    if (month == 2) {
        int is_leap_year = (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0));
        if (day > (is_leap_year ? 29 : 28)) return 0;
    }

    return 1;
}

long convert_to_timestamp(const char* date_str, const char* time_str) {
    int day, month, year, hour, minute, second;

    if (sscanf(date_str, "%d/%d/%d", &day, &month, &year) != 3 ||
        sscanf(time_str, "%d:%d:%d", &hour, &minute, &second) != 3) {
        fprintf(stderr, "Data invalida. Use: DD/MM/AAAA HH:MM:SS\n");
        return -1;
    }

    if (!validate_datetime(day, month, year, hour, minute, second)) {
        fprintf(stderr, "Data ou hora invalida.\n");
        return -1;
    }

    struct tm datetime = {0};
    datetime.tm_year = year - 1900;
    datetime.tm_mon  = month - 1;
    datetime.tm_mday = day;
    datetime.tm_hour = hour;
    datetime.tm_min  = minute;
    datetime.tm_sec  = second;
    datetime.tm_isdst = -1;

    return mktime(&datetime);
}

int main(int argc, char* argv[]) {
#ifdef _WIN32
    SetConsoleOutputCP(CP_UTF8);
#endif

    if (argc != 4) {
        printf("Uso: %s (sensor) (dia/mes/ano) (hh:mm:ss)\n", argv[0]);
        return 1;
    }

    const char* sensor_name = argv[1];
    const char* date_str = argv[2];
    const char* time_str = argv[3];

    long target_timestamp = convert_to_timestamp(date_str, time_str);
    if (target_timestamp == -1) return 1;

    char filename[100];
    snprintf(filename, sizeof(filename), "%s.txt", sensor_name);

    FILE* sensor_file = fopen(filename, "r");
    if (!sensor_file) {
        printf("Erro ao abrir o arquivo %s\n", filename);
        return 1;
    }

    SensorRecord* records = malloc(sizeof(SensorRecord) * 1000);
    int capacity = 1000, count = 0;
    char line[256];

    while (fgets(line, sizeof(line), sensor_file)) {
        if (count >= capacity) {
            capacity *= 2;
            records = realloc(records, capacity * sizeof(SensorRecord));
        }
        sscanf(line, "%ld %s %[^\n]", &records[count].timestamp,
               records[count].sensor_name, records[count].value);
        count++;
    }
    fclose(sensor_file);

    if (count == 0) {
        printf("Arquivo %s vazio ou nao encontrado.\n", filename);
        free(records);
        return 1;
    }

    int index = binary_search_descending(records, count, target_timestamp);
    if (index >= 0) {
        printf("\nLEITURA APROXIMADA:\n");
        printf("TIMESTAMP:%ld || SENSOR:%s || VALOR:%s\n", records[index].timestamp, records[index].sensor_name, records[index].value);
    } else {
        printf("LEITURA NAO ENCONTRADA.\n");
    }

    free(records);
    return 0;
}
