/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Map.h
 * Author: Adam & David
 *
 * Created on Utorok, 2017, novembra 28, 21:28
 */

#ifndef MAP_H
#define MAP_H

#include <iostream>
#include <string>
#include <fstream>
#include "Player.h"

using namespace std;

class Map
{
public:
	Map(/*const int, const int*/);
        Map(const Map&);
	~Map();

	string makeMap();
	string toString();
	bool placePlayer(Player *);
	bool removePlayer(Player *);
	int index(int, int) const;
	bool playerCanMoveInDirection(Player *);
        int getSizeX();
        int getSizeY();
        
        bool firstShootSecond(Player *, Player *);
        bool isWall(int, int);
        bool isPlayer(int, int, Player *);

private:
	char *map_;
	int size_x_;
	int size_y_;
	static const char WALL_CHAR_ = '#';
	static const char EMPTY_PLACE_ = ' ';
};


#endif /* MAP_H */

