#include "gtest/gtest.h"
#include <stdlib.h>
#include <stdio.h>
extern "C" {
#include "schedule.h"
}

TEST(schedule, CorrectTrainNumber1) {
    common_schedule *schedule = schedule_test_data();
    train_schedule *t = find_train_schedule(10, 00, "Partizanskaya", schedule);
    EXPECT_NE(t, nullptr);
    EXPECT_EQ(t->train_number, 10);
    for(int i = 0; i < schedule->trains_count ; i++){
        free(&schedule->trains[i].stops[0]);
    }
    free(schedule->trains);
    free(schedule);
    
}

TEST(schedule, CorrectTrainNumber2) {
    common_schedule *schedule = schedule_test_data();
    train_schedule *t = find_train_schedule(15, 00, "Semenovskaya", schedule);
    EXPECT_NE(t, nullptr);
    EXPECT_EQ(t->train_number, 1);
    for(int i = 0; i < schedule->trains_count ; i++){
        free(&schedule->trains[i].stops[0]);
    }
    free(schedule->trains);
    free(schedule);
}

TEST(schedule, CorrectTrainNumber3) {
    common_schedule *schedule = schedule_test_data();
    train_schedule *t = find_train_schedule(8, 30, "Pervomayskaya", schedule);
    EXPECT_NE(t, nullptr);
    EXPECT_EQ(t->train_number, 7);
    for(int i = 0; i < schedule->trains_count ; i++){
        free(&schedule->trains[i].stops[0]);
    }
    free(schedule->trains);
    free(schedule);
    
}

TEST(schedule, Midnight) {
    common_schedule *schedule = schedule_test_data();
    train_schedule *t = find_train_schedule(0, 0, "Semenovskaya", schedule);
    EXPECT_NE(t, nullptr);
    EXPECT_EQ(t->train_number, 1);
    for(int i = 0; i < schedule->trains_count ; i++){
        free(&schedule->trains[i].stops[0]);
    }
    free(schedule->trains);
    free(schedule);
    
}

TEST(schedule, NonCorrectStationName) {
    common_schedule *schedule = schedule_test_data();
    train_schedule *t = find_train_schedule(23, 59, "Schelkosskaya", schedule);
    EXPECT_EQ(t, nullptr);
    for(int i = 0; i < schedule->trains_count ; i++){
        free(&schedule->trains[i].stops[0]);
    }
    free(schedule->trains);
    free(schedule);
}

TEST(schedule, NonCorrectMinutes) {
    common_schedule *schedule = schedule_test_data();
    train_schedule *t = find_train_schedule(23, 60, "Schelkosskaya", schedule);
    EXPECT_EQ(t, nullptr);
    for(int i = 0; i < schedule->trains_count ; i++){
        free(&schedule->trains[i].stops[0]);
    }
    free(schedule->trains);
    free(schedule);
}

TEST(schedule, NonCorrectHours) {
    common_schedule *schedule = schedule_test_data();
    train_schedule *t = find_train_schedule(24, 20, "Schelkosskaya", schedule);
    EXPECT_EQ(t, nullptr);
    for(int i = 0; i < schedule->trains_count ; i++){
        free(&schedule->trains[i].stops[0]);
    }
    free(schedule->trains);
    free(schedule);
}

int main(int argc, char** argv) {

    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}