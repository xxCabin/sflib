
/**
* @version 1.0.0
* @author skyfire
* @mail skyfireitdiy@hotmail.com
* @see http://github.com/skyfireitdiy/sflib
* @file sf_eventloop.h

* sflib第一版本发布
* 版本号1.0.0
* 发布日期：2018-10-22
*/

/*
 * sf_eventloop 事件循环
 */

#pragma once

#include <condition_variable>
#include <mutex>
#include <atomic>

#include "sf_single_instance.hpp"
#include "sf_msg_queue.hpp"
#include "sf_nocopy.h"

namespace skyfire
{
    /**
     *  @brief 消息循环
     */
    class sf_eventloop : sf_nocopy<>
    {
    private:
        sf_msg_queue* __p_msg_queue__ = sf_msg_queue::get_instance();
        std::atomic<int> running__ { 0 };
    public:
        /**
         * @brief sf_eventloop 构造一个事件循环对象
         */
        sf_eventloop();

        /**
         * @brief exec 执行事件循环
         */
        void exec();

        /**
         * @brief clear 清空事件
         */
        void clear();

        /**
         * @brief wake 激活事件（通常用于退出）
         */
        void wake();

        /**
         * @brief quit 退出事件循环
         */
        void quit();
    };


}
