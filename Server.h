/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Server.h
 * Author: Adam & David
 *
 * Created on Piatok, 2017, decembra 8, 15:39
 */

#ifndef SERVER_H
#define SERVER_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include <cstring>
#include <sstream>

#include <pthread.h>

#include "Map.h"

using namespace std;

class Server {
public:
    Server();
    Server(int portNum);
    Server(const Server& orig);
    virtual ~Server();
    
    int createServer(int);
    
private:
    Map* map_;
    pthread_mutex_t* mutex_;
    int* newsockfd_clientPlay_;
    int* newsockfd_toClient_;
    volatile int pointsServer_;
    volatile int pointsClient_;
    Player* playerServer_;
    Player* playerClient_;
    
    void* serverPlay();
    void* clientPlay();
};

#endif /* SERVER_H */

