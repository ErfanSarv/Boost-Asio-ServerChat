#ifndef SERVER_H
#define SERVER_H
#include "boost/asio.hpp"
#include <memory>
#include <vector>
#include <thread>
#include <string>
#include <map>
#include <shared_mutex>

namespace as=boost::asio ;

class Server
{


public:
    Server(boost::asio::io_context &io,int &port);

private:
        void Accepting(boost::asio::io_context &io) ;

        void Reading(std::shared_ptr<as::ip::tcp::socket> socketPtr,std::shared_ptr<int> socketId);

        void Sending(std::shared_ptr<std::vector<char>> messagePtr , std::shared_ptr<as::ip::tcp::socket> socketPtr);

        void Delete(std::shared_ptr<as::ip::tcp::socket> socketPtr,std::shared_ptr<int> socketId);


        void Controlling(); //Control for server ADMIN

        void Admin();

std::shared_mutex mutex;
std::map<int , std::shared_ptr<as::ip::tcp::socket>> socketMap ;
int id = 0 ;


as::ip::tcp::socket socket ;
as::ip::tcp::acceptor acceptor ;
};

#endif // SERVER_H
















