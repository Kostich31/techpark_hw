#pragma once
#include "workers.h"

//Структура для информации о должностях, с которыми должен работать процесс
typedef struct {
    size_t positions_per_proc;
    size_t proc_idx;
    size_t processes_number;
    size_t positions_number;
} proc_info;

size_t calculate_proc_positions_count(proc_info proc);
int search(workers_data *data, workers_data *output_data);
