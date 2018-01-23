/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Client.h
 * Author: Adam & David
 *
 * Created on Piatok, 2017, decembra 8, 16:00
 */

#ifndef CLIENT_H
#define CLIENT_H

#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include <iostream>

#include <pthread.h>

#include <termios.h>

using namespace std;

class Client {
public:
    Client();
    Client(int);
    Client(const Client& orig);
    virtual ~Client();
    
private:
    int getchNumber_;
    static const int UP_ = 65;
    static const int DOWN_ = 66;
    static const int RIGHT_ = 67;
    static const int LEFT_ = 68;
    static const int SHOOT_ = 32;
    static const int BAD_GETCH1_ = 27; // bulharska konstanta LINUX
    static const int BAD_GETCH2_ = 91; //BULHARSKA LINUX
    
    pthread_mutex_t* mutexCout_;
    int* sockfd_client_;
    int* sockfd_fromServer_;
    int mapSize_;
    
    int createClient(int);
    int _getch();
    bool getchFunction();
    bool isGetchOK(int);
    void* clientPlay();
    void* fromServer();
    
};

#endif /* CLIENT_H */

