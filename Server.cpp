/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Server.cpp
 * Author: Adam & David
 * 
 * Created on Piatok, 2017, decembra 8, 15:39
 */

#include "Server.h"

/*typedef struct commonData
{
    Mapa* mapa;
    pthread_mutex_t* mutex;
    int* newsockfd;
    volatile int pointsServer;
    volatile int pointsClient;
} COMMON_DATA;*/

typedef void * (*THREADFUNCPTR)(void *);

Server::Server() {
}

Server::Server(int portNum)
{
    this->createServer(portNum);
}

Server::Server(const Server& orig) {
}

Server::~Server() {
    delete this->map_;
    delete this->playerClient_;
    delete this->playerServer_;
}

int Server::createServer(int portNum)
{
    int sockfd_1, sockfd_2, newsockfd_clientPlay, newsockfd_toClient;
    socklen_t cli_len_1, cli_len_2;
    struct sockaddr_in serv_addr_1, cli_addr_1;
    struct sockaddr_in serv_addr_2, cli_addr_2;
  
    bzero((char*)&serv_addr_1, sizeof(serv_addr_1));
    serv_addr_1.sin_family = AF_INET;
    serv_addr_1.sin_addr.s_addr = INADDR_ANY;
    serv_addr_1.sin_port = htons(portNum); // nasa zmena
    
    bzero((char*)&serv_addr_2, sizeof(serv_addr_2));
    serv_addr_2.sin_family = AF_INET;
    serv_addr_2.sin_addr.s_addr = INADDR_ANY;
    serv_addr_2.sin_port = htons(portNum + 2); // nasa zmena, druhy socket bude na porte o 2 viac ako prvy

    sockfd_1 = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd_1 < 0)
    {
            perror("Error creating socket for client play");
            return 1;
    }
    sockfd_2 = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd_2 < 0)
    {
            perror("Error creating socket for server to client");
            return 1;
    }

    if (bind(sockfd_1, (struct sockaddr*)&serv_addr_1, sizeof(serv_addr_1)) < 0)
    {
             perror("Error binding socket address to socket for client play");
             return 2;
    }
    if (bind(sockfd_2, (struct sockaddr*)&serv_addr_2, sizeof(serv_addr_2)) < 0)
    {
             perror("Error binding socket address to socket for server to client");
             return 2;
    }
    
    listen(sockfd_1, 5);
    cli_len_1 = sizeof(cli_addr_1);
    listen(sockfd_2, 5);
    cli_len_2 = sizeof(cli_addr_2);
    
    printf("\033c");
    cout << "Cakam na pripojenie klienta..." << endl;
            
    newsockfd_clientPlay = accept(sockfd_1, (struct sockaddr*)&cli_addr_1, &cli_len_1);
    if (newsockfd_clientPlay < 0)
    {
            perror("ERROR on accept socket for client play");
            return 3;
    }
    newsockfd_toClient = accept(sockfd_2, (struct sockaddr*)&cli_addr_2, &cli_len_2);
    if (newsockfd_toClient < 0)
    {
            perror("ERROR on accept socket for server to client");
            return 3;
    }
    
    cout << "Klient pripojeny. Hra zacina" << endl;
    sleep(1);
    
    pthread_t serverThread;
    pthread_t clientThread;
    
    pthread_mutex_t mutex;
    pthread_mutex_init(&mutex, NULL);
    
    /*COMMON_DATA commonData;
    commonData.mapa = new Mapa();
    commonData.mutex = &mutex;
    commonData.newsockfd_clientPlay = &newsockfd_clientPlay;
    commonData.newsockfd_toClient = &newsockfd_toClient;
    commonData.pointsServer = 0;
    commonData.pointsClient = 0;
    
    commonData.mapa->makeMap();*/
    
    this->map_ = new Map();
    this->mutex_ = &mutex;
    this->newsockfd_clientPlay_ = &newsockfd_clientPlay;
    this->newsockfd_toClient_ = &newsockfd_toClient;
    this->pointsServer_ = 5;
    this->pointsClient_ = 5;
    
    
    this->map_->makeMap();
    
    pthread_create(&serverThread, NULL, (THREADFUNCPTR)&Server::serverPlay, this);
    pthread_create(&clientThread, NULL, (THREADFUNCPTR)&Server::clientPlay, this);
    
    pthread_join(clientThread, NULL);
    pthread_join(serverThread, NULL);
    
    if (this->pointsServer_ == 0)
    {
        cout << "\nYOU LOST!" << endl;
    }
    else // pointsClient_ == 0
    {
        cout << "\nYOU WIN!" << endl;
    }
    
    pthread_mutex_destroy(&mutex);
    
    
    close((*this->newsockfd_clientPlay_));
    close(sockfd_1);
    close((*this->newsockfd_toClient_));
    close(sockfd_2);

    return 0;
}

void* Server::serverPlay()
{
    //COMMON_DATA* cData = (COMMON_DATA*) data;

    int n;
    //pthread_mutex_lock(this->mutex_);
    int mapSize = this->map_->getSizeX() * this->map_->getSizeY() + 300;
    //pthread_mutex_unlock(this->mutex_);
    
    char bufferMapSize[50];
    //funkcia na hodenie intu do char[] lebo to potrebuje socket
    snprintf(bufferMapSize, sizeof(bufferMapSize), "%d", mapSize);
    n = write((*this->newsockfd_toClient_), bufferMapSize, strlen(bufferMapSize));
    if (n < 0)
    {
        perror("Error writing to socket from serverPlay to client map size");
        return (void*)5;
    }
    bzero(bufferMapSize, 50);
    
    char bufferFromServer[mapSize];
    bzero(bufferFromServer, mapSize);
    
    this->playerServer_ = new Player(10, 11, 'A');
    
    pthread_mutex_lock(this->mutex_);
    this->map_->placePlayer(this->playerServer_);
    printf("\033c");
    cout << this->map_->toString() << endl;
    cout <<    "Pocet zivotov hraca A:  " << this->pointsServer_
            << "                       "
            << "Pocet zivotov hraca B:  " << this->pointsClient_ << endl;
    pthread_mutex_unlock(this->mutex_);
    
    while(this->pointsServer_ > 0 && this->pointsClient_ > 0)
    {
        if (this->playerServer_->getch()) 
        {
            this->playerServer_->setActionAccordingToGetchNum(); // nastavi akciu hracovi na zaklade toho, ktoru sipku stlacil, alebo medzernik pre vystrel
            
            //pthread_mutex_lock(this->mutex_);
            
            if (this->playerServer_->shootPressed())
            {
                pthread_mutex_lock(this->mutex_);
                if (this->map_->firstShootSecond(this->playerServer_, this->playerClient_))
                {
                    
                    this->pointsClient_--;
                    
                    stringstream toClient;
                    toClient << this->map_->toString() << "\n"
                                << "Pocet zivotov hraca A:  " << this->pointsServer_
                                << "                       "
                                << "Pocet zivotov hraca B:  " << this->pointsClient_ << endl;
                    printf("\033c"); // clearne obrazovku
                    cout << toClient.str() << endl;
                    
                    strcpy(bufferFromServer, toClient.str().c_str()); 
                    n = write((*this->newsockfd_toClient_), bufferFromServer, strlen(bufferFromServer));
                    if (n < 0)
                    {
                        perror("Error writing to socket from serverPlay to client map");
                        return (void*)5;
                    }
                    bzero(bufferFromServer, mapSize);
                    //toClient << "";
                }
                pthread_mutex_unlock(this->mutex_);
            } 
            else if (this->map_->playerCanMoveInDirection(this->playerServer_))
            {
                pthread_mutex_lock(this->mutex_);
                this->map_->removePlayer(this->playerServer_);
                this->playerServer_->move();
                this->map_->placePlayer(this->playerServer_);
                
                stringstream toClient;
                toClient << this->map_->toString() << "\n"
                            << "Pocet zivotov hraca A:  " << this->pointsServer_
                            << "                       "
                            << "Pocet zivotov hraca B:  " << this->pointsClient_ << endl;
                
                
                printf("\033c"); // clearne obrazovku
                cout << toClient.str() << endl;
                pthread_mutex_unlock(this->mutex_);
                
                strcpy(bufferFromServer, toClient.str().c_str()); 
                n = write((*this->newsockfd_toClient_), bufferFromServer, strlen(bufferFromServer));
                if (n < 0)
                {
                    perror("Error writing to socket from serverPlay to client map");
                    return (void*)5;
                }
                bzero(bufferFromServer, mapSize);
                //toClient << "";
            }
        }
    }
    //bzero(bufferFromServer, mapSize);
    char bufferFinisher[50];
    bzero(bufferFinisher, 50);
    string f;
    if (this->pointsServer_ == 0)
    {
        f = "C"; //67 vyhral client
    } 
    else
    {
        f = "S"; //83 vyhral server
    }
    strcpy(bufferFinisher, f.c_str());
    n = write((*this->newsockfd_toClient_), bufferFinisher, strlen(bufferFinisher));
    if (n < 0)
    {
        perror("Error writing to socket in server to client after direction map");
        return (void*)5;
    }
    return NULL;
}

void* Server::clientPlay()
{
    //COMMON_DATA* cData = (COMMON_DATA*) data;
    this->playerClient_ = new Player(5, 11, 'B');
    
    printf("\033c");
    pthread_mutex_lock(this->mutex_);
    this->map_->placePlayer(this->playerClient_);
    cout << this->map_->toString() << endl;
    cout <<    "Pocet zivotov hraca A:  " << this->pointsServer_
            << "                       "
            << "Pocet zivotov hraca B:  " << this->pointsClient_ << endl;
    pthread_mutex_unlock(this->mutex_);
    
    char bufferToServer[50];
    //bzero(bufferToServer, 256);
    int n;
    //pthread_mutex_lock(this->mutex_);
    int mapSize = this->map_->getSizeX() * this->map_->getSizeY() + 300;
    //pthread_mutex_unlock(this->mutex_);
    
    char bufferFromServer[mapSize];
    bzero(bufferFromServer, mapSize);
    
    char bufferMapSize[50];
    //funkcia na hodenie intu do char[] lebo to potrebuje socket
    snprintf(bufferMapSize, sizeof(bufferMapSize), "%d", mapSize);
    
    // da klientovi vediet velkost buffera pre prenos "mapy"
    n = write((*this->newsockfd_clientPlay_), bufferMapSize, strlen(bufferMapSize));
    bzero(bufferMapSize, 50);
    if (n < 0)
    {
        perror("Error writing to socket");
        return (void*)5;
    }
    
    pthread_mutex_lock(this->mutex_);
    stringstream toClient;
    toClient << this->map_->toString() << "\n"
                << "Pocet zivotov hraca A:  " << this->pointsServer_
                << "                       "
                << "Pocet zivotov hraca B:  " << this->pointsClient_ << endl;
    pthread_mutex_unlock(this->mutex_);
    strcpy(bufferFromServer, toClient.str().c_str()); 
    // posle klientovi nech si vykresli prvu mapu
    n = write((*this->newsockfd_clientPlay_), bufferFromServer, strlen(bufferFromServer));
    //bzero(bufferFromServer, mapSize);
    //toClient << "";
    
    if (n < 0)
    {
        perror("Error writing to socket");
        return (void*)5;
    }
    
    while (true)
    {
        bzero(bufferToServer, 50);
        n = read((*this->newsockfd_clientPlay_), bufferToServer, 49);
        if (n < 0)
        {
            perror("Error reading from socket in server from client direction");
            return (void*)4;
        }
        
        if (this->pointsServer_ > 0 && this->pointsClient_ > 0)
        {
            int getchNum = atoi(bufferToServer);
            this->playerClient_->setAction(getchNum);

            if (this->playerClient_->shootPressed())
            {
                pthread_mutex_lock(this->mutex_);
                if (this->map_->firstShootSecond(this->playerClient_, this->playerServer_))
                {
                    this->pointsServer_--;

                    printf("\033c"); // clearne obrazovku
                    stringstream toClient;
                    toClient << this->map_->toString() << "\n"
                            << "Pocet zivotov hraca A:  " << this->pointsServer_
                            << "                       "
                            << "Pocet zivotov hraca B:  " << this->pointsClient_ << endl;
                    cout << toClient.str() << endl;

                    bzero(bufferFromServer, mapSize);
                    //funckia na hodenie stringu do char[] kvoli socketu
                    strcpy(bufferFromServer, toClient.str().c_str()); 
                    //toClient << "";
                }
                pthread_mutex_unlock(this->mutex_);
            } 
            else if (this->map_->playerCanMoveInDirection(this->playerClient_)) // signalizuje, ci sa hrac moze pohnut v smere, ktory ma nastaveny
            {
                pthread_mutex_lock(this->mutex_);
                this->map_->removePlayer(this->playerClient_);
                this->playerClient_->move();
                //system("cls");
                printf("\033c"); // clearne obrazovku
                this->map_->placePlayer(this->playerClient_);
                stringstream toClient;
                toClient << this->map_->toString() << "\n"
                        << "Pocet zivotov hraca A:  " << this->pointsServer_
                        << "                       "
                        << "Pocet zivotov hraca B:  " << this->pointsClient_ << endl;
                cout << toClient.str() << endl;
                pthread_mutex_unlock(this->mutex_);

                bzero(bufferFromServer, mapSize);
                //funckia na hodenie stringu do char[] kvoli socketu
                strcpy(bufferFromServer, toClient.str().c_str()); 
                //toClient << "";
            }
            //bzero(bufferFromServer, mapSize);
            n = write((*this->newsockfd_clientPlay_), bufferFromServer, strlen(bufferFromServer));
            if (n < 0)
            {
                perror("Error writing to socket in server to client after direction map");
                return (void*)5;
            }
        }
        else
        {
            break;
        }
    }  
    char bufferFinisher[50];
    bzero(bufferFinisher, 50);
    string f;
    if (this->pointsServer_ == 0)
    {
        f = "C"; //67 vyhral client
    } 
    else
    {
        f = "S"; //83 vyhral server
    }
    strcpy(bufferFinisher, f.c_str());
    n = write((*this->newsockfd_clientPlay_), bufferFinisher, strlen(bufferFinisher));
    if (n < 0)
    {
        perror("Error writing to socket in server to client after direction map");
        return (void*)5;
    }
    return NULL; 
}