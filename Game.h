/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Game.h
 * Author: Adam & David
 *
 * Created on Piatok, 2017, decembra 8, 15:41
 */

#ifndef GAME_H
#define GAME_H

#include "Map.h"
#include "Server.h"
#include "Client.h"
#include <stdio.h>
#include <iostream>

class Game {
public:
    Game();
    Game(string, int);
    Game(const Game& orig);
    virtual ~Game();
    
    void play();
private:
    string side_;
    int portNum_;
    Server* server_;
    Client* client_;
};

#endif /* GAME_H */

