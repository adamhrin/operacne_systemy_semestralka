/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Game.cpp
 * Author: Adam & David
 * 
 * Created on Piatok, 2017, decembra 8, 15:41
 */

#include "Game.h"


using namespace std;

Game::Game() {
}

Game::Game(string side, int portNum)
{
    this->side_ = side;
    this->portNum_ = portNum;
}

Game::Game(const Game& orig) {
}

Game::~Game() {
    delete this->server_;
    delete this->client_;
}

void Game::play()
{
    if (this->side_ == "server")
    {
        this->server_ = new Server(this->portNum_);
    }
    else if (this->side_ == "client")
    {
        this->client_ = new Client(this->portNum_);
    }
}
