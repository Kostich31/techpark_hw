#define HOURS_IN_DAY 24
#define MINUTES_IN_HOUR 60
#define STATION_NAME_LENGTH 20
#define STOPS_COUNT 5
#define TRAINS_COUNT 10

typedef struct train_stop{
    int departure_time;
    int arrival_time;
    char station_name[STATION_NAME_LENGTH];
}train_stop;

typedef struct train_schedule{
    short train_number;
    short stops_count;
    train_stop *stops;
}train_schedule;

typedef struct common_schedule{
    short trains_count;
    train_schedule *trains;
}common_schedule;

int convert_to_minutes(int hours, int minutes);
train_schedule *find_train_schedule(int hours, int minutes, const char *destination_name, common_schedule* schedule);
train_schedule *find_by_station_name(train_schedule *train_schedule, const char *destination_name);
common_schedule* schedule_test_data();
