
/**
* @version 1.0.0
* @author skyfire
* @mail skyfireitdiy@hotmail.com
* @see http://github.com/skyfireitdiy/sflib
* @file sf_tcp_client_win.h

* sflib第一版本发布
* 版本号1.0.0
* 发布日期：2018-10-22
*/

#pragma once

#include <winsock2.h>
#include <string>
#include <functional>
#include <memory>
#include <thread>
#include "sf_tcp_client_interface.h"

namespace skyfire
{
    class sf_tcp_client : public sf_tcp_client_interface
    {

    private:
        bool raw__ = false;
        bool inited__ = false;
        SOCKET sock__ = INVALID_SOCKET;
    public:

        SOCKET get_raw_socket() override;

        bool bind(const std::string& ip, unsigned short port) override;

        sf_tcp_client(bool raw = false);

        static std::shared_ptr <sf_tcp_client> make_client(bool raw = false);

        ~sf_tcp_client();

        bool connect_to_server(const std::string &ip, unsigned short port) override;


        bool send(int type, const byte_array &data) override;

        bool send(const byte_array & data) override;

        void close();
    };

}
