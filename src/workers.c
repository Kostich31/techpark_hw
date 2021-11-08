#include "workers.h"
#include <string.h>
#include <stdio.h>
#include <stdlib.h>

int create(workers_data* data, const size_t capacity){
    if(!data) return -1;

    data->w_data = malloc(capacity*sizeof(Workers));
    data->size = 0;
    data->capacity = capacity;

    return 0;
}

int insert(workers_data* data, const Workers worker){
    if(!data) return -1;

    if(data->size == data->capacity){
        data->capacity*=2;
        data->w_data = realloc(data->w_data, data->capacity*sizeof(Workers));
    }
    data->w_data[data->size++] = worker;
    return 0;
}

int free_data(workers_data* data){
    if(!data) return -1;

    free(data->w_data);
    data->w_data = NULL;
    data->capacity = data->size = 0;
    return 0;
}

int find_by_position(const workers_data* data, const char* target_pos){
    if(!data || !target_pos) return -1;

    for(size_t i = 0; i < data->size; ++i){
        if(strcmp(data->w_data[i].position, target_pos) == 0) return 1;
    }
    return 0;
}

int sort_by_second_name(workers_data* data){
    if(!data) return -1;

    Workers tmp;
    for(size_t i = 0; i < data->size; ++i){
        for(size_t j = i+1; j < data->size; ++j){
            if(strcmp(data->w_data[i].second_name,data->w_data[j].second_name) > 0){
                tmp = data->w_data[i];
                data->w_data[i] = data->w_data[j];
                data->w_data[j] = tmp;
            }
        }
    }
    return 0;
}

int find_min_age_by_position(const workers_data* data, const char* target_pos, Workers* output_data){
    if(!data || !target_pos || !output_data ) return -1;

    size_t min_idx = 0;
    unsigned short min_age = data->w_data[min_idx].age;
    for(size_t i = 1; i < data->size; ++i){
        if(strcmp(data->w_data[i].position, target_pos) == 0 && data->w_data[i].age < min_age){
            min_idx = i;
            min_age = data->w_data[min_idx].age;
        } 
    }
    *output_data = data->w_data[min_idx];
    return 0;
}

int find_max_age_by_position(const workers_data* data, const char* target_pos, Workers* output_data){
    if(!data || !target_pos || !output_data) return -1;

    size_t max_idx = 0;
    unsigned short max_age = data->w_data[max_idx].age;
    for(size_t i = 1; i < data->size; ++i){
        if(strcmp(data->w_data[i].position, target_pos) == 0 && data->w_data[i].age > max_age){
            max_idx = i;
            max_age = data->w_data[max_idx].age;
        } 
    }

    *output_data = data->w_data[max_idx];
    return 0;
}

int read_from_file(const char* file_name, workers_data* data){
    if(!file_name){
        fprintf(stderr, "Incorrect file name\n");
        return -1;
    }
    if(!data){
        fprintf(stderr, "Incorrecct data pointer\n");
        return -1;
    }
    FILE*f = fopen(file_name, "rb");
    if(!f){
        fprintf(stderr, "Failed to open file for read\n");
        return -1;
    }

    Workers w;
    while(fread(&w, sizeof(Workers), 1, f)==1){
        if(insert(data,w) == -1) return -1;
    }

    if(fclose(f)){
        fprintf(stderr, "Failed to close file\n");
        return -1;

    }
    return 0;
}

int split(workers_data *input_data, workers_data *output_data, size_t start_index, size_t size) {
    if (!input_data || !output_data) {
        return -1;
    }

    for (size_t i = start_index; i < start_index + size; i++) {
        if (insert(output_data, input_data->w_data[i]) == -1) {
            return -1;
        }
    }
    return 0;
}
