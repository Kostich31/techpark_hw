#pragma once
#include <stddef.h>

#define FIRST_NAME_SIZE 18
#define SECOND_NAME_SIZE 32
#define POSITION_SIZE 28
#define ARRAY_DEFAULT_SIZE 1

typedef struct{
    char position[POSITION_SIZE]; // 28
    char second_name [SECOND_NAME_SIZE]; // 32
    char first_name[FIRST_NAME_SIZE]; //18
    unsigned short age; //2
    unsigned short is_male; //2
    unsigned short experience; // 2
    unsigned int salary; //4
} Workers;

typedef struct{
    Workers* w_data;
    size_t size;
    size_t capacity;
} workers_data;

int create(workers_data* data, const size_t capacity);
int insert(workers_data* data, const Workers worker);
int free_data(workers_data* data);
int find_by_position(const workers_data* data, const char* target_pos);
int sort_by_second_name(workers_data* data);
int find_min_age_by_position(const workers_data* data, const char* target_pos, Workers* output_data);
int find_max_age_by_position(const workers_data* data, const char* target_pos, Workers* output_data);
int read_from_file(const char* file_name, workers_data* output_data);
int split(workers_data *input, workers_data *output, size_t start_index, size_t size);
