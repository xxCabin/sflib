#include "sf_rpcserver.h"
#include <iostream>

using namespace std;
using namespace skyfire;

void print()
{
    cout<<"print"<<endl;
}

vector<int> add_one(vector<int> data)
{
    for(auto &p: data)
    {
        ++p;
    }
    return data;
}

void output(const char *str)
{
    cout<<str<<endl;
}

int main()
{
    cout<< sizeof(pkg_header_t) << endl;
    auto server = std::make_shared<sf_rpcserver<>>();
    server->reg_rpc_func("print", print);
    server->reg_rpc_func("add_one", add_one);
    cout<<server->listen("127.0.0.1",10001)<<endl;
    sf_eventloop eventloop;
    eventloop.exec();
}