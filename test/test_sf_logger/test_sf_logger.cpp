
/**
* @version 1.0.0
* @author skyfire
* @mail skyfireitdiy@hotmail.com
* @see http://github.com/skyfireitdiy/sflib
* @file test_sf_logger.cpp

* sflib第一版本发布
* 版本号1.0.0
* 发布日期：2018-10-22
*/

#include <iostream>
// 1.SF_LOGGER_STANDALONE宏表示单独使用sf_logger，不必依赖其他sflib库
#define SF_LOGGER_STANDALONE
// 2.SF_DEBUG表示启用sf_debug宏
#define SF_DEBUG
#include "sf_logger.hpp"

using namespace skyfire;


int main()
{
    // 3.获取单例对象
    auto logger = sf_logger::get_instance();
    // 4.添加日志文件，SF_WARN_LEVEL以上的日志级别将会打印到此文件中
    logger->add_level_file(SF_WARN_LEVEL, "runlog.log");
    // 5.SF_INFO_LEVEL以上的日志级别将会打印到标准输出流
    logger->add_level_stream(SF_INFO_LEVEL, &std::cout);
    // 6.日志打印
    sf_error("hello", "world");
    sf_warn("this is warn");
    sf_error("hello", "world");
    sf_warn("this is warn");
    getchar();
    g_logger->stop_logger();
    getchar();
}