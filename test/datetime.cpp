#include <iostream>
#include <gtest/gtest.h>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>

#include "unistd.h"

TEST(datetime, format){
    pthread_t tid = pthread_self();
    printf("now tid is %p \n", tid);
}