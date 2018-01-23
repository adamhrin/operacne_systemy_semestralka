/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Player.h
 * Author: Adam & David
 *
 * Created on Utorok, 2017, novembra 28, 21:31
 */

#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <string>
//WINDOWS #include <conio.h> 

#include <unistd.h>
#include <termios.h>
#include <stdio.h>
#include <stdlib.h>

using namespace std;

class Player
{

public:
        Player();
        Player(const Player&);
	Player(int x, int y, const char);
	~Player();

	Player * getPlayer();
	int getPos_x();
	int getPos_y();
	char getCharOfPlayer();

	int _getch();
	bool getch();
        
	void move();
	void getCoordinatesAfterMove(int&, int&);
	void setActionAccordingToGetchNum();
        void setAction(int);
        bool shootPressed();
        
        string getDirection();
        //Action getAction(); //vratit hodnotu enumu

private:
	char player_;
	int pos_x_;
	int pos_y_;
	enum Action { MOVE, SHOOT } action_;
	int getchNumber_;
        enum Direction { UP, DOWN, RIGHT, LEFT } direction_;
        

	static const int UP_ = 65;
	static const int DOWN_ = 66;
	static const int RIGHT_ = 67;
	static const int LEFT_ = 68;
        static const int SHOOT_ = 32;
	static const int BAD_GETCH1_ = 27; // bulharska konstanta LINUX
	static const int BAD_GETCH2_ = 91; //BULHARSKA LINUX
        
        bool isGetchOK(int);
};


#endif /* PLAYER_H */

