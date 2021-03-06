
/**
* @version 1.0.0
* @author skyfire
* @mail skyfireitdiy@hotmail.com
* @see http://github.com/skyfireitdiy/sflib
* @file sf_rpc_server.hpp

* sflib第一版本发布
* 版本号1.0.0
* 发布日期：2018-10-22
*/

/*
 * sf_rpc_server RPC服务器
 */

#pragma once

#include "sf_rpc_server.h"
#include "sf_rpc_utils.h"

namespace skyfire {

    
    template<typename _Type>
    void sf_rpc_server::__send_back(SOCKET sock, int id_code, _Type data) {
        sf_rpc_res_context_t res;
        res.call_id = id_code;
        res.ret = sf_serialize_binary(data);
        __tcp_server__->send(sock, RPC_RES_TYPE, sf_serialize_binary(res));
    }

    
    void sf_rpc_server::__on_data_coming(SOCKET sock, const sf_pkg_header_t &header, const byte_array &data) {
        if(header.type != RPC_REQ_TYPE)
        {
            return;
        }
        std::string id;
        byte_array param;
        sf_rpc_req_context_t req;
        sf_deserialize_binary(data, req, 0);
        for (auto &p : __func__vec__) {
            p(sock, req);
        }
    }

    
    template<typename _Func>
    void sf_rpc_server::reg_rpc_func(const std::string &id, _Func func) {
        if constexpr (std::is_bind_expression<_Func>::value) {
            static_assert(!sf_check_param_reference<_Func>::value, "Param can't be reference");
            static_assert(!sf_check_param_pointer<_Func>::value, "Param can't be pointer");
            static_assert(!sf_check_return_reference<_Func>::value, "Return can't be reference");
            static_assert(!sf_check_return_pointer<_Func>::value, "Return can't be pointer");


            using _Ret = typename sf_function_type_helper<_Func>::return_type;
            using _Param = typename sf_function_type_helper<_Func>::param_type;

            auto f = [=](SOCKET s, const sf_rpc_req_context_t& req) {
                if (req.func_id == id) {
                    _Param param;
                    sf_deserialize_binary(req.params, param, 0);
                    _Ret ret = sf_invoke(func, param);
                    __send_back(s, req.call_id, ret);
                }
            };
            __func__vec__.push_back(f);

        } else {
            static_assert(!sf_check_param_reference<decltype(std::function(func))>::value,
                          "Param can't be reference");
            static_assert(!sf_check_param_pointer<decltype(std::function(func))>::value, "Param can't be pointer");
            static_assert(!sf_check_return_reference<decltype(std::function(func))>::value,
                          "Return can't be reference");
            static_assert(!sf_check_return_pointer<decltype(std::function(func))>::value,
                          "Return can't be pointer");


            using _Ret = typename sf_function_type_helper<decltype(std::function(func))>::return_type;
            using _Param = typename sf_function_type_helper<decltype(std::function(func))>::param_type;

            auto f = [=](SOCKET s, const sf_rpc_req_context_t& req ) {
                if (req.func_id == id) {
                    _Param param;
                    sf_deserialize_binary(req.params, param, 0);
                    if constexpr (std::is_same<_Ret, void>::value) {
                        sf_invoke(func, param);
                        __send_back(s, req.call_id, '\0');
                    } else {
                        _Ret ret = sf_invoke(func, param);
                        __send_back(s, req.call_id, ret);
                    }
                }
            };
            __func__vec__.push_back(f);
        }

    }

    
    std::shared_ptr<sf_rpc_server> sf_rpc_server::make_server() {
        return std::shared_ptr<sf_rpc_server>(new sf_rpc_server);
    }

    
    sf_rpc_server::sf_rpc_server() {
        sf_bind_signal(sf_rpc_server::__tcp_server__,
                       data_coming,
                       [=](SOCKET sock, const sf_pkg_header_t &header, const byte_array &data) {
                           __on_data_coming(sock, header, data);
                       },
                       true);
        sf_bind_signal(sf_rpc_server::__tcp_server__,new_connection,[=](SOCKET sock){
            client_connected(sock);
        },true);
        sf_bind_signal(sf_rpc_server::__tcp_server__,closed,[=](SOCKET sock){
            client_disconnected(sock);
        },true);
    }

    
    bool sf_rpc_server::listen(const std::string &ip, unsigned short port) {
        return __tcp_server__->listen(ip, port);
    }

    
    void sf_rpc_server::close() {
        __tcp_server__->close();
    }
}
