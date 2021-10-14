#include <stdio.h>
#include <stdlib.h>
#include "schedule.h"

int main() {
    short hours = 0, minutes = 0;
    char destination_station[STATION_NAME_LENGTH] = "";
    printf("Please enter time and destination station in format HH:MM STATION_NAME:\n");
    scanf("%hd:%hd %20s", &hours, &minutes, destination_station);

    common_schedule* schedule = schedule_test_data();
    train_schedule *target_train = find_train_schedule(hours, minutes, destination_station, schedule);
    if (target_train != NULL) {
        printf("You need train number %hd at %02hd:%02hd\n",
               target_train->train_number,
               target_train->stops[0].departure_time / MINUTES_IN_HOUR,
               target_train->stops[0].departure_time % MINUTES_IN_HOUR);
    } else {
        printf("NOT FOUND\n");
    }
    
    for(int i = 0; i < schedule->trains_count ; i++){
        free(&schedule->trains[i].stops[0]);
    }
    free(schedule->trains);
    free(schedule);
    return 0;
}
