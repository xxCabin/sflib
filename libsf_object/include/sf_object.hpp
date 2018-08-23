/*
 * sf_object 基础对象
 * 提供了信号---槽的通信机制、AOP编程
 */

#pragma once

#include "sf_object.h"

namespace skyfire
{

    template<typename _VectorType, typename _FuncType>
    int sf_object::__sf_bind_helper(std::recursive_mutex &mu, _VectorType &vec, _FuncType func, bool mul_thread) {
        std::lock_guard<std::recursive_mutex> lck(mu);
        int bind_id = rand();
        while(std::find_if(vec.begin(),vec.end(),[=](auto p){
            return std::get<2>(p) == bind_id;
        }) != vec.end())
        {
            bind_id = rand();
        }
        vec.push_back(std::make_tuple(func, mul_thread, bind_id));
        return bind_id;
    }

    inline sf_object::~sf_object() {
        __p_msg_queue__->remove_msg(this);
    }

    template<typename _VectorType>
    void sf_object::__sf_aop_unbind_helper(std::recursive_mutex &mu, _VectorType &vec, int bind_id) {
        std::lock_guard<std::recursive_mutex> lck(mu);
        vec.erase(std::remove_if(vec.begin(),vec.end(),[=](auto p){
            return std::get<1>(p) == bind_id;
        }), vec.end());
    }

    template<typename _VectorType, typename _FuncType>
    int sf_object::__sf_aop_after_add_helper(std::recursive_mutex &mu, _VectorType &vec, _FuncType func) {
        std::lock_guard<std::recursive_mutex> lck(mu);
        int bind_id = rand();
        while(std::find_if(vec.begin(),vec.end(),[=](auto p){
            return std::get<1>(p) == bind_id;
        }) != vec.end())
        {
            bind_id = rand();
        }
        vec.push_back(std::make_tuple(func, bind_id));
        return bind_id;
    }

    template<typename _VectorType, typename _FuncType>
    int sf_object::__sf_aop_before_add_helper(std::recursive_mutex &mu, _VectorType &vec, _FuncType func) {
        std::lock_guard<std::recursive_mutex> lck(mu);
        int bind_id = rand();
        while(std::find_if(vec.begin(),vec.end(),[=](auto p){
            return std::get<1>(p) == bind_id;
        }) != vec.end())
        {
            bind_id = rand();
        }
        vec.insert(vec.begin() ,std::make_tuple(func, bind_id));
        return bind_id;
    }

    template<typename _VectorType>
    void sf_object::__sf_signal_unbind_helper(std::recursive_mutex &mu, _VectorType &vec, int bind_id) {
        std::lock_guard<std::recursive_mutex> lck(mu);
        vec.erase(std::remove_if(vec.begin(),vec.end(),[=](auto p){
            return std::get<2>(p) == bind_id;
        }), vec.end());
    }

}
