/* 
 * File:   TestEpoll.cpp
 * Author: Administrator
 *
 * Created on 2014-1-28, 11:10:10
 */

#include <stdlib.h>
#include <iostream>
#include "../TcpSock.h"
#include "../Parser.h"
#include "../Epoll.h"
/*
 * Simple C++ Test Suite
 */
class TestParser:public CommLib::ProtocolParser
{
    public:
        virtual bool parse(char* buff, int len)
        {
            char *buffnew =new char[ len +1 ];
            memcpy( buffnew ,buff,len );
            buffnew[ len ] = 0;
            
            std::cout<< buffnew <<std::endl;
        }
};

void test1() {
    std::cout << "TestEpoll test 1" << std::endl;
    
    CommLib::Epoll2 ep;
    TestParser* parser = new TestParser();
    
    CommLib::SockAddr addr( 2080,"0.0.0.0",CommLib::Sock::IP );
    
    CommLib::TcpServSock ssock( parser ,&ep  );
    ssock.Bind( addr );
    ssock.Listen( 10 );
    ssock.RegistEpoll();
    
    while( 1 )
    {
            ep.Schedule( 10 );
    }
}

void test2() {
    std::cout << "TestEpoll test 2" << std::endl;
//    std::cout << "%TEST_FAILED% time=0 testname=test2 (TestEpoll) message=error message sample" << std::endl;
}

int main(int argc, char** argv) {
    std::cout << "%SUITE_STARTING% TestEpoll" << std::endl;
    std::cout << "%SUITE_STARTED%" << std::endl;

    std::cout << "%TEST_STARTED% test1 (TestEpoll)" << std::endl;
    test1();
    std::cout << "%TEST_FINISHED% time=0 test1 (TestEpoll)" << std::endl;

    std::cout << "%TEST_STARTED% test2 (TestEpoll)\n" << std::endl;
    test2();
    std::cout << "%TEST_FINISHED% time=0 test2 (TestEpoll)" << std::endl;

    std::cout << "%SUITE_FINISHED% time=0" << std::endl;

    return (EXIT_SUCCESS);
}

