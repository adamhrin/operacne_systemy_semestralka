/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Client.cpp
 * Author: Adam & David
 * 
 * Created on Piatok, 2017, decembra 8, 16:00
 */

#include "Client.h"

/*typedef struct commonData
{
    pthread_mutex_t* mutexCout;
    int* sockfd_client;
    int* sockfd_server;
    
    //pointer na funkciu, ktoru chcem volat z metody ktora bezi na threade
    //a kedze ta metoda je staticka musim mat vsetko k comu pristupujem v spolocnej strukture
    //(zo statickej metody neviem pristupit k this)
    bool (*getchFn)(void); 
    int getchNumberCD;
} COMMON_DATA;*/

typedef void * (*THREADFUNCPTR)(void *);

Client::Client() {
}

Client::Client(int portNum)
{
    this->createClient(portNum);
}

Client::Client(const Client& orig) {
}

Client::~Client() {
}

int Client::createClient(int portNum)
{
    int sockfd_client, sockfd_fromServer;
    struct sockaddr_in serv_addr1, serv_addr2;
    struct hostent* server1;
    struct hostent* server2;

    
    server1 = gethostbyname("localhost"); // nasa zmena
    server2 = gethostbyname("localhost");

    if (server1 == NULL)
    {
        fprintf(stderr, "Error, no such host server1 from client\n");
        return 2;
    }
    
    if (server2 == NULL)
    {
        fprintf(stderr, "Error, no such host server2 from client\n");
        return 2;
    }
    
    bzero((char*)&serv_addr1, sizeof(serv_addr1));
    bzero((char*)&serv_addr2, sizeof(serv_addr2));
    
    serv_addr1.sin_family = AF_INET;
    serv_addr2.sin_family = AF_INET;
    
    bcopy(
            (char*)server1->h_addr,
            (char*)&serv_addr1.sin_addr.s_addr,
            server1->h_length
    );
    
    bcopy(
            (char*)server2->h_addr,
            (char*)&serv_addr2.sin_addr.s_addr,
            server2->h_length
    );
    
    serv_addr1.sin_port = htons(portNum); // nasa zmena
    serv_addr2.sin_port = htons(portNum + 2); // nasa zmena, druhy socket bude na porte o 2 viac ako prvy
    
    sockfd_client = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd_client < 0)
    {
        perror("Error creating socket for client");
        return 3;
    }
    
    sockfd_fromServer = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd_fromServer < 0)
    {
        perror("Error creating socket for client from server");
        return 3;
    }

    if (connect(sockfd_client, (struct sockaddr*)&serv_addr1, sizeof(serv_addr1)) < 0)
    {
        perror("Error connecting to socket for client");
        return 4;
    }
    if (connect(sockfd_fromServer, (struct sockaddr*)&serv_addr2, sizeof(serv_addr2)) < 0)
    {
        perror("Error connecting to socket for client from server");
        return 4;
    }
    //cout << "Este nezbehlo readnutie mapSizu" << endl;
    
    /*char buffer[50];
    int n = read(this->sockfd_client_, buffer, 49);
    this->mapSize_ = atoi(buffer);
    if (n < 0)
    {
        perror("Error reading from socket in client from server map size");
        return 6;
    }*/
    
    //cout << "velkost mapy" + this->mapSize_ << endl;
    //cout << "Zbehlo readnutie mapSizu" << endl;
    
    pthread_t clientThread;
    pthread_t fromServerThread;
    
    pthread_mutex_t mutexCout;
    pthread_mutex_init(&mutexCout, NULL);
    
    this->mutexCout_ = &mutexCout;
    this->sockfd_client_ = &sockfd_client;
    this->sockfd_fromServer_ = &sockfd_fromServer;
    //void* returnValue = 0;
    
    pthread_create(&clientThread, NULL, (THREADFUNCPTR)&Client::clientPlay, this);
    pthread_create(&fromServerThread, NULL, (THREADFUNCPTR)&Client::fromServer, this);
    
    pthread_join(fromServerThread, NULL);
    pthread_join(clientThread, NULL);
    
    /*if (*returnValue == (int)67)
    {
        cout << "YOU WIN!" << endl;
    } else if (*returnValue == (int)83)
    {
        cout << "YOU LOST!" << endl;
    }*/
    
    pthread_mutex_destroy(this->mutexCout_);
    
    close((*this->sockfd_fromServer_));
    close((*this->sockfd_client_));
    return 0;
}

void* Client::fromServer()
{
    int n;
    //char bufferFromServer[this->mapSize_];
    char bufferMapSize[50];
    n = read((*this->sockfd_fromServer_), bufferMapSize, 49);
    if (n < 0)
    {
        perror("Error reading from socket in client fromServer mapSize");
        return (void*)6;
    }
    
    int mapSize = atoi(bufferMapSize);
    char bufferFromServer[mapSize]; // 3963
    bzero(bufferMapSize, 50);
    while (true)
    {
        bzero(bufferFromServer, mapSize);
        n = read((*this->sockfd_fromServer_), bufferFromServer, mapSize - 1);
        if (n < 0)
        {
            perror("Error reading from socket in client from server ONLY map");
            return (void*)6;
        }
        if (bufferFromServer[0] == 67) // 67 = 'C' => vyhral klient
        {
            cout << "\nYOU WIN!" << endl;
            return NULL;
        }
        else if (bufferFromServer[0] == 83) // 83 = 'S' => vyhral server
        {
            cout << "\nYOU LOST!" << endl;
            return NULL;
        }
        
        pthread_mutex_lock(this->mutexCout_);
        printf("\033c");
        cout << bufferFromServer << endl;
        pthread_mutex_unlock(this->mutexCout_);
    }
}

void* Client::clientPlay()
{
    //COMMON_DATA* cData = (COMMON_DATA*) data;
    int n;
    char bufferToServer[50];
    
    char buffer[50];
    n = read((*this->sockfd_client_), buffer, 49);
    
    if (n < 0)
    {
        perror("Error reading from socket in client from server map size");
        return (void*)6;
    }
    this->mapSize_ = atoi(buffer);
    char bufferFromServer[this->mapSize_];
    
    n = read((*this->sockfd_client_), bufferFromServer, this->mapSize_ - 1);
    if (n < 0)
    {
        perror("Error reading from socket in client play from server first map");
        return (void*)6;
    }
    pthread_mutex_lock(this->mutexCout_);
    printf("\033c");
    cout << bufferFromServer << endl;
    pthread_mutex_unlock(this->mutexCout_);
    
    while(true)
    {
	bzero(bufferToServer, 50);
        if (this->getchFunction())
        {
            //funkcia na hodenie intu do char[] lebo to potrebuje socket
            snprintf(bufferToServer, sizeof(bufferToServer), "%d", this->getchNumber_);
            // napise na socket smer klienta
            n = write((*this->sockfd_client_), bufferToServer, strlen(bufferToServer));
            if (n < 0)
            {
                perror("Error writing to socket from client to server direction");
                return (void*)5;
            }
            
            bzero(bufferFromServer, this->mapSize_);
            n = read((*this->sockfd_client_), bufferFromServer, this->mapSize_ - 1);// tu mi to stoji :)
            if (n < 0)
            {
                perror("Error reading from socket in client from server map");
                return (void*)6;
            }
            if (bufferFromServer[0] == 67) // 67 = 'C' => vyhral klient
            {
                cout << "\nYOU WIN!" << endl;
                return NULL;
            }
            else if (bufferFromServer[0] == 83) // 83 = 'S' => vyhral server
            {
                cout << "\nYOU LOST!" << endl;
                return NULL;
            }
            
            pthread_mutex_lock(this->mutexCout_);
            printf("\033c");
            cout << bufferFromServer << endl;
            pthread_mutex_unlock(this->mutexCout_);
        }
    }
}

int Client::_getch()
{
    char buf = 0;
    struct termios old = { 0 };
    if (tcgetattr(0, &old) < 0)
            perror("tcsetattr()");
    old.c_lflag &= ~ICANON;
    old.c_lflag &= ~ECHO;
    old.c_cc[VMIN] = 1;
    old.c_cc[VTIME] = 0;
    if (tcsetattr(0, TCSANOW, &old) < 0)
            perror("tcsetattr ICANON");
    if (read(0, &buf, 1) < 0)
            perror("read()");
    old.c_lflag |= ICANON;
    old.c_lflag |= ECHO;
    if (tcsetattr(0, TCSADRAIN, &old) < 0)
            perror("tcsetattr ~ICANON");

    return (int)buf;
}

bool Client::getchFunction() 
{
    int getch = this->_getch();
    if (getch != Client::BAD_GETCH1_)
    {
        if (getch != Client::BAD_GETCH2_) 
        {
            if(this->isGetchOK(getch))
            {
                this->getchNumber_ = getch;
                return true;
            }
        }
    }
    return false;
}

/**
 * 
 * @param getch
 * @return vrati true ak je getchNumber pripustny (iba tie klavesy ktore maju funkcionalitu)
 */
bool Client::isGetchOK(int getch)
{
    return getch == Client::UP_ || getch == Client::DOWN_ || getch == Client::RIGHT_ || getch == Client::LEFT_ || getch == Client::SHOOT_;
}
