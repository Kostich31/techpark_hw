#include <stdio.h>
#include <stdlib.h>
#include "consistent_search.h"

int search(workers_data* data, workers_data* output_data){
    // Собираем все профессиии из базы данных
    workers_data workers_positions;
    if(create(&workers_positions, ARRAY_DEFAULT_SIZE) == -1) return -1;
    for(size_t i = 0; i < data->size; ++i){
        int find = find_by_position(&workers_positions, data->w_data[i].position);
        if(find == 0){
            if(insert(&workers_positions, data->w_data[i]) == -1) return -1;
            else if(find == -1) return -1;
        }
    }
    // Находим самого возрастного сотрудника
    for(size_t i = 0; i < workers_positions.size; ++i){
        Workers max_age_worker;
        if(find_max_age_by_position(data, workers_positions.w_data[i].position, &max_age_worker)!= 0){
            return -1;
        }
        if(insert(output_data, max_age_worker) == -1) return -1;

    }
    // Находим самого молодого сотрудника
    for(size_t i = 0; i < workers_positions.size; ++i){
        Workers min_age_worker;
        if(find_min_age_by_position(data, workers_positions.w_data[i].position, &min_age_worker)!= 0){
            return -1;
        }
        if(insert(output_data, min_age_worker) == -1) return -1;
        
    }

    //Сортируем полученные данные
    if(sort_by_second_name(output_data) == -1) return -1;

    if (free_data(&workers_positions) == -1) return -1;
    
    return 0;
}
