/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Player.cpp
 * Author: Adam & David
 * 
 * Created on Utorok, 2017, novembra 28, 21:31
 */

#include "Player.h"

Player::Player() {
}

Player::Player(const Player& orig) {
}

Player::Player(int x, int y, const char pPlayer)
{
    this->player_ = pPlayer;
    this->pos_x_ = x;
    this->pos_y_ = y;
    this->direction_ = UP;
    this->action_ = MOVE;
    this->getchNumber_ = 0;
}


Player::~Player() {
}

Player* Player::getPlayer() {
    return this;
}

int Player::getPos_x() {
    return this->pos_x_;
}

int Player::getPos_y() {
    return this->pos_y_;
}

char Player::getCharOfPlayer() {
    return this->player_;
}


//LINUX
int Player::_getch() {
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

    //cout << (int)buf << endl;

    return (int)buf;
}


bool Player::getch()
{
    int getch = _getch();
    if (getch != Player::BAD_GETCH1_)
    {
        if (getch != Player::BAD_GETCH2_) 
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
bool Player::isGetchOK(int getch)
{
    return getch == Player::UP_ || getch == Player::DOWN_ || getch == Player::RIGHT_ || getch == Player::LEFT_ || getch == Player::SHOOT_;
}

void Player::move() {
    switch (this->direction_)
    {
    case UP:
            this->pos_x_--;
            break;
    case DOWN:
            this->pos_x_++;
            break;
    case RIGHT:
            this->pos_y_++; // na skoky o dva 2
            break;
    case LEFT:
            this->pos_y_--; // na skoky o dva 2
            break;
    default:
            break;
    }
}

void Player::getCoordinatesAfterMove(int & x, int & y)
{
    switch (this->direction_)
    {
    case UP:
            x = this->pos_x_ - 1;
            y = this->pos_y_;
            break;
    case DOWN:
            x = this->pos_x_ + 1;
            y = this->pos_y_;
            break;
    case RIGHT:
            x = this->pos_x_;
            y = this->pos_y_ + 1;
            break;
    case LEFT:
            x = this->pos_x_;
            y = this->pos_y_ - 1;
            break;
    default: // ked sa vystreli => nezmeni sa smer
            x = this->pos_x_;
            y = this->pos_y_;
            break;
    }
}

void Player::setActionAccordingToGetchNum()
{
    switch (this->getchNumber_)
    {
        case Player::UP_:
            this->action_ = MOVE;
            this->direction_ = UP;
            break;
        case Player::DOWN_:
            this->action_ = MOVE;
            this->direction_ = DOWN;
            break;
        case Player::RIGHT_:
            this->action_ = MOVE;
            this->direction_ = RIGHT;
            break;
        case Player::LEFT_:
            this->action_ = MOVE;
            this->direction_ = LEFT;
            break;
        case Player::SHOOT_:
            this->action_ = SHOOT;
            break;
        default:
            break;
    }
}

void Player::setAction(int getchNum)
{
    this->getchNumber_ = getchNum;
    this->setActionAccordingToGetchNum();
}

bool Player::shootPressed()
{
    if (this->action_ ==  SHOOT)
    {
        return true;
    }
    return false;
}

string Player::getDirection()
{
    string direction = "";
    switch(this->direction_)
    {
        case UP:
            direction = "UP";
            break;
        case DOWN:
            direction = "DOWN";
            break; 
        case RIGHT:
            direction = "RIGHT";
            break;
        case LEFT:
            direction = "LEFT";
            break;
        default:
            break;
    }
    return direction;
}

/*
Action Player::getAction()
{
    return this->action_;
}
*/