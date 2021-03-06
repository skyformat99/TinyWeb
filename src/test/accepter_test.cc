/*
*Author:GeneralSandman
*Code:https://github.com/GeneralSandman/TinyWeb
*E-mail:generalsandman@163.com
*Web:www.generalsandman.cn
*/

/*---XXX---
*
****************************************
*
*/

#include <tiny_core/accepter.h>
#include <tiny_core/eventloop.h>
#include <tiny_core/netaddress.h>
#include <tiny_base/api.h>

#include <iostream>
#include <unistd.h>
#include <boost/bind.hpp>

using namespace std;

int number = 0;

EventLoop *g_loop = nullptr;

void ConnectionCallback(int connectfd, const NetAddress &peer)
{
    string info = to_string(++number);
    cout << "new Connection:" << peer.getIpPort() << endl;
    cout << "write to:" << peer.getIpPort() << endl;
    writeString(connectfd, info);
    close(connectfd);
}

void fun1()
{
    cout << "invoke per second\n";
}

void timeout()
{
    cout << "Timeout!\n";
    g_loop->quit();
}

int main()
{
    g_loop = new EventLoop();
    g_loop->runEvery(1, boost::bind(fun1));
    g_loop->runAfter(200, boost::bind(timeout));

    NetAddress server("127.0.0.1:9898");
    Accepter accept(g_loop, server,createNoBlockSocket());
    accept.setConnectionCallback(ConnectionCallback);
    accept.listen();

    g_loop->loop();
    delete g_loop;
    return 0;
}