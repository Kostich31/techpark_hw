#include "consistent_search.h"
#include "parallel_search.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <dlfcn.h>

int main(){
    workers_data data;
    if(create(&data, ARRAY_DEFAULT_SIZE) == -1) return -1;

    char* file_name = "/home/travis/build/Kostich31techpark_hw/tests/test10k.bin";
    if(read_from_file(file_name, &data) == -1) return -1;

    workers_data search_result;
    if(create(&search_result, ARRAY_DEFAULT_SIZE) == -1) return -1;
    //Расчет времени выполнения на обычным алгоритме
    clock_t begin = clock();
    int result = search(&data, &search_result);
    clock_t end = clock();
    
    if(result){
        return 1;
    }

    printf("static lib time: %f\n", (double)(end-begin));

    if(free_data(&search_result) == -1) return -1;
    if(create(&search_result, ARRAY_DEFAULT_SIZE) == -1) return -1;
    
    void* library;
    int(*searchf)(workers_data* input, workers_data* output);
    library = dlopen("libworkers_dynamic.so", RTLD_LAZY);
    if(!library){
        fprintf(stderr, "Error while loading dynamic lib: %s\n", dlerror());
        return 1;
    } 
    *(int**)(&searchf) = dlsym(library, "search");

    //Расчет времени выполнения на распараллелльном алгоритме
    begin = clock();
    int result_parallel = (*searchf)(&data, &search_result);
    end = clock();

    if(result_parallel){
        dlclose(library);
        return 1;
    }
    dlclose(library);
    printf("dynamic lib time: %f\n", (double)(end-begin));
    
    if(free_data(&search_result) == -1) return -1;
    if(free_data(&data) == -1) return -1;

    return 0;
}