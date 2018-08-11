/*
#include <glog/logging.h>
#include <iostream>
#include <gtest/gtest.h>


class MyInfoLogger : public google::base::Logger {
public:
    virtual void Write(bool force_flush,
            time_t timestamp,
            const char* message,
            int message_len) {
        std::cout << "MyInfoLogger::Write "
            << "|" << force_flush
            << "|" << timestamp
            << "|" << message
            << "|" << message_len << std::endl;
    }

    virtual void Flush() {
        std::cout << "MyInfoLogger::Flush" << std::endl;
    }

    virtual google::uint32 LogSize() {
        return 0;
    }
};//MyInfoLogger


TEST(glog, log) {
    std::string argc = "demo";
    google::InitGoogleLogging(argc.c_str());

    MyInfoLogger my_info_logger;
    google::base::SetLogger(google::GLOG_INFO, &my_info_logger);
    google::base::SetLogger(google::GLOG_FATAL, &my_info_logger);

    LOG(INFO) << "LOG(INFO)";
    LOG(WARNING) << "LOG(WARNING)";
}



*/