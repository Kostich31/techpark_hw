#include <stdlib.h>
#include <stdio.h>
#include <sys/sysinfo.h>
#include <sys/shm.h>
#include <wait.h>
#include <unistd.h>
#include "parallel_search.h"

//Функция, которая вычисляет количество должностей, которые должен охватить процесс
size_t calculate_proc_positions_count(proc_info proc) {
    // в последнем процессе обрабатываются оставшиеся профессии
    if (proc.proc_idx == proc.processes_number - 1) {
        return (proc.positions_number - proc.positions_per_proc*proc.proc_idx);
    } else {
        return proc.positions_per_proc;
    }

}

int search(workers_data *data, workers_data *output_data) {
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

    //Распараллеливаем алгоритм по количеству процессов
    //Каждый процесс будет отвечать за вычисление самых молодого и возрастного сотрудника в профессиях
    int processes_number = get_nprocs();
    int pid_arr[processes_number];
    // Считаю количество должностей в каждом процессе
    size_t positions_per_proc = workers_positions.size / processes_number;
    // Создаем shared memory, куда будут записаны самые молодые и возрастные сотрудники
    int shmid = shmget(IPC_PRIVATE, sizeof(Workers) * workers_positions.size*2, 0644);
    if (shmid == -1) {
        fprintf(stderr, "Failed to create shared memory\n");
        return -1;
    }

    //Работаем с количеством выданных процессов
    for (int proc_idx = 0; proc_idx < processes_number; proc_idx++) {
        int pid = fork();
        if (pid < 0) {
            fprintf(stderr, "Failed to fork child\n");
            return -1;
        } 
        else if (pid > 0) {
            // Заполняем массив pid для контроля их дальнейшего освобождения
            pid_arr[proc_idx] = pid;
        } 
        else if (pid == 0) {
            // Должности, с которыми работает текущий процесс
            workers_data proc_workers;
            if (create(&proc_workers, positions_per_proc) == -1) exit(1);

            // Считаем количество должностей для текущего процесса
            proc_info info = {
                    positions_per_proc,
                    proc_idx,
                    processes_number,
                    workers_positions.size
            };
            size_t proc_positions_count = calculate_proc_positions_count(info);
            // Отделяем нужную часть рабочих для рассчета процессом
            if (split(&workers_positions, &proc_workers, positions_per_proc * proc_idx, proc_positions_count) == -1 ) {
                return -1;
            }
            // Создаем массив найденных процессом сотрудников
            workers_data proc_result;
            if (create(&proc_result, proc_workers.size*2) == -1) exit(1);

            // Проходим по массиву выделенных процессу сотрудников
            for (size_t proc_positions_idx = 0; proc_positions_idx < proc_workers.size; proc_positions_idx++) {
                // Находим самого пожилого сотрудника
                Workers max_age_employee;
                if(find_max_age_by_position(data, proc_workers.w_data[proc_positions_idx].position, &max_age_employee) != 0) {
                    exit(2);
                }
                if (insert(&proc_result, max_age_employee) == -1) exit(3);

                // Находим самого молодого сотрудника
                Workers min_age_employee ;
                if (find_min_age_by_position(data, proc_workers.w_data[proc_positions_idx].position, &min_age_employee) != 0) {
                    exit(2);
                }
                if (insert(&proc_result, min_age_employee) == -1) exit(3);
            }

            // Записываем полученные данные в shared memory
            Workers *shm_workers_per_proc = (Workers*)shmat(shmid, NULL, 0);
            if (shm_workers_per_proc == (void *) -1) {
                fprintf(stderr, "Failed to attach shared memory\n");
                exit(4);
            }
            // Индекс относительно количества сотрудников в каждом процессе
            size_t shared_memory_start_idx = positions_per_proc * proc_idx*2;
            for (size_t i = 0; i < proc_result.size; i++) {
                shm_workers_per_proc[shared_memory_start_idx + i] = proc_result.w_data[i];
            }
            if (shmdt(shm_workers_per_proc) == -1) {
                fprintf(stderr, "Failed to detach shared memory\n");
                return -1;
            }

            if (free_data(&proc_result) == -1) exit(5);
            if (free_data(&proc_workers) == -1) exit(5);

            exit(0);
        }
    }

    // Ожидаем завершения всех процессов
    for (int proc_idx = 0; proc_idx < processes_number; proc_idx++) {
        int proc_status;
        waitpid(pid_arr[proc_idx], &proc_status, 0);
    }
    // Считываем данные из shared memory
    Workers *shm_workers = (Workers*)shmat(shmid, NULL, 0);
    if (shm_workers == (void *) -1) {
        fprintf(stderr, "Failed to attach shared memory\n");
        return -1;
    }
    for (size_t i = 0; i < workers_positions.size*2; i++) {
        insert(output_data, shm_workers[i]);
    }
    if (shmdt(shm_workers) == -1) {
        fprintf(stderr, "Failed to detach shared memory\n");
        return -1;
    }
    if (shmctl(shmid, IPC_RMID, 0) == -1) {
        fprintf(stderr, "Failed to destroy shared memory\n");
        return -1;
    }
    //Сортируем полученные данные
    if (sort_by_second_name(output_data) == -1) return-1;

    if (free_data(&workers_positions) == -1) return -1;

    return 0;
}
