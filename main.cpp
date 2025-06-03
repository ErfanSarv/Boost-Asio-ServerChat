#include "boost/asio.hpp"
#include <iostream>
#include "Server.h"



namespace as=boost::asio ;
int main(){

as::io_context io;

int port=8080 ;

std::cout<<"                 *<_>*"<<std::endl;
std::cout<<"Welcome \npress <Delete> button for clients list \nType <Close> and then client id for deleting " <<std::endl<<std::endl;



Server server1(io,port);






io.run();
}




