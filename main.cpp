/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   main.cpp
 * Author: Adam & David
 *
 * Created on Utorok, 2017, novembra 28, 21:26
 */

//#include "heap_monitor.h"
#include "Game.h"
#include <stdio.h>
#include <iostream>

using namespace std;

int main(int argc, char* argv[])
{
    //initHeapMonitor(); /// detects memory leaks!
    
    if (argc < 3)
    {
        cout << "Malo parametrov" << endl;
        return -1;
    }
    
    Game* game = new Game(argv[1], atoi(argv[2]));
    game->play();
    
    delete game;
    
    return 0;
}

