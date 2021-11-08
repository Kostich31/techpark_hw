#include "gtest/gtest.h"

extern "C" {
#include "workers.h"
}

TEST(data, Creating){
    workers_data data;
    size_t size = 10;
    ASSERT_EQ(create(&data, size), 0);
    ASSERT_EQ(data.size, 0);
    ASSERT_EQ(data.capacity, size);
    free_data(&data);
}

TEST(data, IncorrectCreating){
    ASSERT_EQ(create(NULL, 1), -1);
}

TEST(data, Insert){
    workers_data data;
    size_t size = 1;
    ASSERT_EQ(create(&data, size), 0);
    Workers worker = {
        "Programmer",
        "Pypkin",
        "Vasya",
        30, 1, 5, 50000
    };
    ASSERT_EQ(insert(&data,worker), 0);
    ASSERT_EQ(data.size, 1);
    ASSERT_EQ(worker.age, data.w_data[0].age);
    ASSERT_EQ(worker.is_male, data.w_data[0].is_male);
    ASSERT_EQ(worker.salary, data.w_data[0].salary);
    ASSERT_EQ(worker.experience, data.w_data[0].experience);
    ASSERT_STREQ(worker.position, data.w_data[0].position);
    ASSERT_STREQ(worker.first_name, data.w_data[0].first_name);
    ASSERT_STREQ(worker.second_name, data.w_data[0].second_name);
    free_data(&data);
}

TEST(data, IncorrectInsert){
    Workers worker;
    ASSERT_EQ(insert(NULL, worker), -1);
}

TEST(data, ResizesingInsert){
    workers_data data;
    size_t capacity_old = 1;
    ASSERT_EQ(create(&data, capacity_old), 0);
    Workers worker = {
        "Programmer",
        "Pypkin",
        "Vasya",
        30, 1, 5, 50000
    };
    insert(&data,worker);
    size_t size_old = data.size;
    insert(&data,worker);
    ASSERT_EQ(data.capacity, capacity_old*2);
    ASSERT_EQ(data.size, size_old+1);
    free_data(&data);
}

TEST(data, Spliting){
    workers_data data;
    size_t capacity = 1;
    create(&data, capacity);
    Workers worker = {
        "Programmer",
        "Pypkin",
        "Vasya",
        30, 1, 5, 50000
    };
    insert(&data,worker);
    insert(&data,worker);
    insert(&data,worker);
    insert(&data,worker);
    workers_data result;
    create(&result, capacity);
    ASSERT_EQ(split(&data, &result, 1, 2),0);
    ASSERT_EQ(data.w_data[0].age, result.w_data[1].age);
    ASSERT_EQ(result.size, 2);
    ASSERT_EQ(free_data(&data), 0);
    ASSERT_EQ(free_data(&result), 0);
}

TEST(data, IncorrectSpliting){
    workers_data data;
    create(&data,1);
    ASSERT_EQ(split(NULL,&data, 0,1),-1);
    ASSERT_EQ(split(&data,NULL, 0,1),-1);
    free_data(&data);
}

TEST(data, Free){
    workers_data data;
    create(&data, 1);
    ASSERT_EQ(free_data(&data),0);
    ASSERT_EQ(data.w_data, nullptr);
    ASSERT_EQ(data.size, 0);
    ASSERT_EQ(data.capacity, 0);
}

TEST(data, IncorrectFree){
    ASSERT_EQ(free_data(NULL),-1);
}

TEST(read, FileReading){
    workers_data data;
    create(&data,1);
    ASSERT_EQ(read_from_file(const_cast<char*>("../tests/test10k.bin"),&data),0);
    ASSERT_EQ(data.size, 10000);
    ASSERT_EQ(free_data(&data), 0);
}

TEST(read, IncorrectFileOpening){
    ASSERT_EQ(read_from_file(const_cast<char*>("../tests/test10k.bin"), NULL),-1);
    workers_data data;
    create(&data, 1);
    ASSERT_EQ(read_from_file(const_cast<char*>(""),&data),-1);
    free_data(&data);
}

TEST(data, SortSecondName){
    workers_data data;
    size_t capacity = 1;
    create(&data, capacity);
    Workers worker1 = {
        "Programmer",
        "Barov",
        "Vasya",
        30, 1, 5, 50000
    };
    Workers worker2 = {
        "Programmer",
        "Arov",
        "Vasya",
        30, 1, 5, 50000
    };
    insert(&data, worker1);
    insert(&data, worker2);
    ASSERT_EQ(sort_by_second_name(&data), 0);
    ASSERT_STREQ(data.w_data[0].second_name, "Arov");
    free_data(&data);
}

TEST(data, FindMaxAge){
    workers_data data;
    size_t capacity = 1;
    create(&data, capacity);
    Workers worker1 = {
        "Programmer",
        "Barov",
        "Vasya",
        50, 1, 5, 50000
    };
    Workers worker2 = {
        "Programmer",
        "Arov",
        "Vasya",
        30, 1, 5, 50000
    };
    insert(&data, worker1);
    insert(&data, worker2);
    Workers result;
    ASSERT_EQ(find_max_age_by_position(&data,"Programmer", &result), 0);
    ASSERT_EQ(result.age, 50);
    free_data(&data);
}

TEST(data, IncorrectFindMaxAge){
    workers_data data;
    size_t capacity = 1;
    create(&data, capacity);
    Workers worker1 = {
        "Programmer",
        "Barov",
        "Vasya",
        50, 1, 5, 50000
    };
    Workers worker2 = {
        "Programmer",
        "Arov",
        "Vasya",
        30, 1, 5, 50000
    };
    insert(&data, worker1);
    insert(&data, worker2);
    Workers result;
    ASSERT_EQ(find_max_age_by_position(NULL,"Programmer", &result), -1);
    ASSERT_EQ(find_max_age_by_position(&data,NULL, &result), -1);
    ASSERT_EQ(find_max_age_by_position(&data,NULL, NULL), -1);
    free_data(&data);
}

TEST(data, FindMinAge){
    workers_data data;
    size_t capacity = 1;
    create(&data, capacity);
    Workers worker1 = {
        "Programmer",
        "Barov",
        "Vasya",
        50, 1, 5, 50000
    };
    Workers worker2 = {
        "Programmer",
        "Arov",
        "Vasya",
        30, 1, 5, 50000
    };
    insert(&data, worker1);
    insert(&data, worker2);
    Workers result;
    ASSERT_EQ(find_min_age_by_position(&data,"Programmer", &result), 0);
    ASSERT_EQ(result.age, 30);
    free_data(&data);
}

TEST(data, IncorrectFindMinAge){
    workers_data data;
    size_t capacity = 1;
    create(&data, capacity);
    Workers worker1 = {
        "Programmer",
        "Barov",
        "Vasya",
        50, 1, 5, 50000
    };
    Workers worker2 = {
        "Programmer",
        "Arov",
        "Vasya",
        30, 1, 5, 50000
    };
    insert(&data, worker1);
    insert(&data, worker2);
    Workers result;
    ASSERT_EQ(find_min_age_by_position(NULL,"Programmer", &result), -1);
    ASSERT_EQ(find_min_age_by_position(&data,NULL, &result), -1);
    ASSERT_EQ(find_min_age_by_position(&data,NULL, NULL), -1);
    free_data(&data);
}

TEST(data, FindByPosition){
    workers_data data;
    size_t capacity = 1;
    create(&data, capacity);
    Workers worker = {
        "Programmer",
        "Barov",
        "Vasya",
        50, 1, 5, 50000
    };
    insert(&data, worker);
    ASSERT_EQ(find_by_position(&data, "Programmer"), 1);
    ASSERT_EQ(find_by_position(&data, "QA-engineer"), 0);

    free_data(&data);
}

TEST(data, IncorrectFindByPosition){
    workers_data data;
    size_t capacity = 1;
    create(&data, capacity);
    Workers worker = {
        "Programmer",
        "Barov",
        "Vasya",
        50, 1, 5, 50000
    };
    insert(&data, worker);
    ASSERT_EQ(find_by_position(&data, NULL), -1);
    ASSERT_EQ(find_by_position(NULL, "Programmer"), -1);

    free_data(&data);
}
int main(int argc, char** argv) {
    chdir("/");
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}