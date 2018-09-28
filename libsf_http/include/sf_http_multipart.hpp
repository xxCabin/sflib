#include "sf_http_multipart.h"
#include "sf_http_request.hpp"
#include "sf_type.hpp"
#include "sf_http_utils.hpp"
#include "sf_utils.hpp"


namespace skyfire
{
    inline sf_http_multipart::sf_http_multipart(const std::string &boundary_str, const std::string& tmp_path) :
            boundary_str__(boundary_str), filename__(sf_path_join(tmp_path,sf_random::get_instance()->get_uuid_str()))
    {
    }

    inline sf_http_header sf_http_multipart::get_header() const
    {
        return header__;
    }

    inline bool sf_http_multipart::is_end() const
    {
        return end__;
    }

    inline bool sf_http_multipart::is_finished() const
    {
        return finish__;
    }

    inline std::string sf_http_multipart::get_filename() const
    {
        return filename__;
    }

    inline bool sf_http_multipart::append_data(const byte_array &data, byte_array &ret)
    {
        std::string new_boundary_str = "----" + boundary_str__;
        if(first_block)
        {
            auto tmp_data = to_string(data);
            if (tmp_data.find(new_boundary_str) != 0)
            {
                ret = data;
                return false;
            }
            std::string request_line;
            std::vector<std::string> header_lines;
            byte_array body;
            if (!sf_http_request::split_request(data, request_line, header_lines, body))
            {
                ret = data;
                return false;
            }
            if (!sf_http_request::parse_header(header_lines, header__))
            {
                ret = data;
                return false;
            }
            first_block = false;
            fp__ = std::make_shared<std::ofstream>(filename__,std::ios::binary | std::ios::out);
            // NOTE 暂时忽略掉打开失败的情况
            auto body_str = to_string(body);
            auto finish_pos = body_str.find(new_boundary_str);
            if(finish_pos == std::string::npos){
                fp__->write(body.data(),body.size());
                ret = byte_array();
                return true;
            }
            else
            {
                fp__->write(body.data(),finish_pos-2);// 有一个\r\n
                finish__ = true;
                fp__->close();
                if(finish_pos == body_str.find(new_boundary_str+"--"))
                {
                    end__ = true;
                    ret = byte_array();
                    return true;
                }
                else
                {
                    ret = {body.begin()+finish_pos,body.end()};
                    return true;
                }
            }
        }
        else
        {
            auto body = data;
            auto body_str = to_string(body);
            auto finish_pos = body_str.find(new_boundary_str);
            if(finish_pos == std::string::npos){
                fp__->write(body.data(),body.size());
                ret = byte_array();
                return true;
            }
            else
            {
                fp__->write(body.data(),finish_pos-2);// 有一个\r\n
                finish__ = true;
                fp__->close();
                if(finish_pos == body_str.find(new_boundary_str+"--"))
                {
                    end__ = true;
                    ret = byte_array();
                    return true;
                }
                else
                {
                    ret = {body.begin()+finish_pos,body.end()};
                    return true;
                }
            }
        }
    }
}