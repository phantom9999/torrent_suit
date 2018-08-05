#include <string>
#include <iostream>
#include <gtest/gtest.h>
#include <boost/algorithm/string.hpp>


TEST(minihttp, strutil) {
    using std::string;
    using std::cout;
    using std::endl;
    string data = "asdfQWE";
    boost::algorithm::to_lower(data);
    cout << data << endl;
}



