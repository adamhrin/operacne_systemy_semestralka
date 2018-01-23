/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Map.cpp
 * Author: Adam & David
 * 
 * Created on Utorok, 2017, novembra 28, 21:28
 */

#include "Map.h"


Map::Map()
{
	string toString;
	string line;
	ifstream mapFile("map.txt");
	if (mapFile.is_open())
	{
		int num_of_rows = 0;
		int num_of_cols = 0;

		while (getline(mapFile, line))
		{
			num_of_cols = line.length();
			num_of_rows++;
		}

		mapFile.close();

		this->size_x_ = num_of_rows;
		this->size_y_ = num_of_cols;
	}
	else
	{
		toString += "Unable to open file";
	}

	this->map_ = new char[this->size_x_ * this->size_y_];
}


Map::Map(const Map& orig) {
}

Map::~Map()
{
    delete this->map_;
}

string Map::makeMap() {
	string toString;
	string line;
	ifstream mapFile("map.txt");
	if (mapFile.is_open())
	{
		int i = 0;

		while (getline(mapFile, line))
		{
			for (int j = 0; j < this->size_y_; j++)
			{
				toString += this->map_[index(i, j)] = line.at(j);
			}

			toString += '\n';
			i++;
		}
		mapFile.close();
	}
	else
	{
		toString += "Unable to open file";
	}

	return toString;
}

string Map::toString()
{
	string toString;
	for (int i = 0; i < this->size_x_; i++)
	{
		for (int j = 0; j < this->size_y_; j++)
		{
			toString += this->map_[index(i, j)];
		}
		toString += '\n';
	}
	return toString;
}

bool Map::placePlayer(Player *player) // nemusim nic kontrolovat, lebo uz som skontroloval
{
	this->map_[index(player->getPos_x(), player->getPos_y())] = player->getCharOfPlayer();
	return true;
}

bool Map::removePlayer(Player *player) 
{
	if (player != NULL)
	{
		this->map_[index(player->getPos_x(), player->getPos_y())] = Map::EMPTY_PLACE_;
		return true;
	}
	return false;
}

int Map::index(int x, int y) const 
{ 
	return y + this->size_y_ * x; 
}

bool Map::playerCanMoveInDirection(Player * player)
{
	int x, y;
	player->getCoordinatesAfterMove(x, y); // vypocitam si dopredu kam by sa hrac posunutim vo svojom smere dostal 
	bool result = this->map_[index(x, y)] != Map::WALL_CHAR_; // zistim, ci tam, kde by sa chcel hrac dostat je wall_char a podla toho mu to dovolim/nedovolim
	return result;
}

int Map::getSizeX()
{
    return this->size_x_;
}

int Map::getSizeY()
{
    return this->size_y_;
}

bool Map::firstShootSecond(Player * player1, Player * player2)
{
    int bullet_x, bullet_y;
    bullet_x = player1->getPos_x();
    bullet_y = player1->getPos_y();
    
    if (player1->getDirection() == "UP")
    {
        bullet_x = player1->getPos_x() - 1;
        while (!isWall(bullet_x, bullet_y))
        {
            if (isPlayer(bullet_x, bullet_y, player2))
            {
                return true;
            }
            bullet_x--;
        }
    } else if (player1->getDirection() == "DOWN")
    {
        bullet_x = player1->getPos_x() + 1;
        while (!isWall(bullet_x, bullet_y)) 
        {
            if (isPlayer(bullet_x, bullet_y, player2))
            {
                return true;
            }
            bullet_x++;
        }
    } else if (player1->getDirection() == "RIGHT")
    {
        bullet_y = player1->getPos_y() + 1;
        while (!isWall(bullet_x, bullet_y)) 
        {
            if (isPlayer(bullet_x, bullet_y, player2))
            {
                return true;
            }
            bullet_y++;
        }
    } else if (player1->getDirection() == "LEFT")
    {
        bullet_y = player1->getPos_y() - 1;
        while (!isWall(bullet_x, bullet_y)) 
        {
            if (isPlayer(bullet_x, bullet_y, player2))
            {
                return true;
            }
            bullet_y--;
        }
    } else {
        cout << "NETRAFIL SI!" << endl;
        sleep(1000000);
        return false;
    }
}

bool Map::isWall(int x, int y)
{
    return this->map_[index(x, y)] == Map::WALL_CHAR_ ? true : false;
}

bool Map::isPlayer(int x, int y, Player * player)
{
    return this->map_[index(x, y)] == player->getCharOfPlayer() ? true : false;
}
