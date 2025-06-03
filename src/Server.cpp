#include "Server.h"
#include "boost/asio.hpp"
#include <iostream>
#include <vector>
#include <memory>
#include <thread>
#include <chrono>
#include <shared_mutex>
#include <windows.h>

namespace as=boost::asio ;  ///Finsihed Now go for get keyboard for showing all of the connected clients !

Server::Server(boost::asio::io_context &io,int &port):socket(io),acceptor(io,as::ip::tcp::endpoint(as::ip::tcp::v4(),port)){



Accepting(io);


std::thread ControlThread([this](){Controlling();});
ControlThread.detach();


std::thread AdminThread([this](){Admin();});
AdminThread.detach();



}

//std::shared_mutex sharedmutex ; //SharedMutex
//std::map<std::shared_ptr<as::ip::tcp::socket>,int> socketMap ;

void Server::Accepting(as::io_context &io){

acceptor.set_option(boost::asio::socket_base::reuse_address(false));

auto socketPtr = std::make_shared<as::ip::tcp::socket>(io) ;
auto socketId = std::make_shared<int>(id) ;

acceptor.async_accept(*socketPtr , [this,socketPtr,&io,socketId](boost::system::error_code ec){

                      if(!ec){std::cout<<"+1Client"<<std::endl;


                      std::unique_lock<std::shared_mutex> lock(mutex); //Changing the map !
                        socketMap.emplace(id,socketPtr);
                      lock.unlock();



                        Reading(socketPtr,socketId); //Read message from client !



                        id=id+1 ;

                            Accepting(io); //New client

                      }else{std::cout<<"err in async_Accept() : "<<ec.message();}

                      });




}

void Server::Reading(std::shared_ptr<as::ip::tcp::socket> socketPtr,std::shared_ptr<int> socketId){


auto messagePtr = std::make_shared<std::vector<char>>(1024);

socketPtr->async_read_some(as::buffer(*messagePtr),[this,messagePtr,socketPtr,socketId](boost::system::error_code ec,size_t bytes){

               if(!ec){
                for(int i=0;i<bytes;i++){
                    std::cout<<(*messagePtr)[i]; //write the message from client !
                }

                Sending(messagePtr,socketPtr); //send the message to other clients !

                Reading(socketPtr,socketId); //again ! take new message from this client !
               }else{
                        Delete(socketPtr,socketId); //this else mean this client is disconnected !
               }

               });



}

void Server::Sending(std::shared_ptr<std::vector<char>> messagePtr , std::shared_ptr<as::ip::tcp::socket> socketPtr){

std::shared_lock<std::shared_mutex> lock(mutex);  //read from map !

for(auto c : socketMap){
    if(c.second == socketPtr){continue;}



    as::async_write(*c.second , as::buffer(*messagePtr),[this](boost::system::error_code ec,size_t bytes){

                    if(!ec){

                        std::cout<<"sended ! "  ;

                    }else{
                        std::cout<<"err in Sending() : "<<ec.message()<<std::endl;
                    }


                    });


}



}


void Server::Delete(std::shared_ptr<as::ip::tcp::socket> socketPtr,std::shared_ptr<int> socketId){

std::unique_lock<std::shared_mutex> locky(mutex); // shared mutex for changing the map

    socketMap.erase(*socketId);

    locky.unlock(); //unlock



if(socketMap.size()==0){id=0; } //Reset The id // You can Delete This ! ;D
}



void Server::Controlling(){

while (true) {






    if(auto a=GetAsyncKeyState(VK_DELETE)& 0x8000){
        std::cout<<"******LIST OF CLIENTS******"<<std::endl;

        std::shared_lock<std::shared_mutex>lock(mutex);
        for(auto c:socketMap){
            std::cout<<"The id : "<<c.first << "  the socket Name : "<<c.second<<std::endl;

        }
        Sleep(300);
    }
    Sleep(50);

}




}



void Server::Admin(){std::string message ;
                     int idsocket ;
while(true){
while(true){



    std::getline(std::cin,message);


    if(message=="Close"){goto BreakingBad;} // delete the client  process !
    else{
        //Admin Writing !
    }


}
BreakingBad:


    std::cin>>idsocket ;

    std::unique_lock<std::shared_mutex> lockylock(mutex);


    auto closingsocket = socketMap.find(idsocket) ;

    closingsocket->second->set_option(as::socket_base::linger(true,0)); //Close the client socket

    socketMap.erase(idsocket);

    std::cout<<"Client Closed"<<std::endl;

    lockylock.unlock();

}

}














