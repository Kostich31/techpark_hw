#include <stdlib.h>
#include <string.h>
#include "schedule.h"

int convert_to_minutes(int hours, int minutes) 
{
    if (hours >= HOURS_IN_DAY || minutes >= MINUTES_IN_HOUR)
        return -1;
    return hours * MINUTES_IN_HOUR + minutes;
}

train_schedule *find_by_station_name(train_schedule *train_schedule, const char *destination_name) 
{
    if (train_schedule == NULL)
        return NULL;

    for (int i = 0; i < train_schedule->stops_count; i++) {
        if (strcmp(destination_name, train_schedule->stops[i].station_name) == 0) {
            return train_schedule;
        }
    }
    return NULL;
}

train_schedule *find_train_schedule(int hours, int minutes, const char *destination_name, common_schedule* schedule) {
    if (schedule == NULL)
        return NULL;

    int current_time = convert_to_minutes(hours, minutes);
    if (current_time < 0)
        return NULL;

    for (int i = 0; i < schedule->trains_count; i++) {
        train_schedule *train_schedule = &schedule->trains[i];
        if (train_schedule != NULL && current_time < train_schedule->stops[0].departure_time) {
            return find_by_station_name(train_schedule, destination_name);
        }
    }

    return find_by_station_name(&schedule->trains[0], destination_name);
}

common_schedule* schedule_test_data()
{
    common_schedule* schedule = (common_schedule *)(malloc(sizeof(common_schedule)));
    if (schedule == NULL){
        return NULL;
    }

    schedule->trains_count = TRAINS_COUNT;
    schedule->trains = (train_schedule *) malloc(TRAINS_COUNT * sizeof(train_schedule));

    train_stop stops[STOPS_COUNT] = 
    {
        {360, 360, "Schelkovskaya"},
        {365, 366, "Pervomayskaya"},
        {369, 370, "Izmailovskaya"},
        {372, 373, "Partizanskaya"},
        {375, 376, "Semenovskaya"}
    };

    for (int i = 0; i < TRAINS_COUNT; i++) {
        train_schedule train_schedule = {
                (short)(i + 1), STOPS_COUNT, (train_stop*)malloc(STOPS_COUNT * sizeof(train_stop))
        };

        for (int j = 0; j < STOPS_COUNT; j++) {
            train_stop stop = {
                    (short)(stops[j].arrival_time + i*30),
                    (short)(stops[j].departure_time + i*30)
            };
            strcpy(stop.station_name, stops[j].station_name);
            train_schedule.stops[j] = stop;
        }
        schedule->trains[i] = train_schedule;
    }
    return schedule;
}
