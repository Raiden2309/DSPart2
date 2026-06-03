//Name: Ivy Ling Ke Lin
//TP No: TP076996

#ifndef ROBOTNAVIGATION_HPP
#define ROBOTNAVIGATION_HPP

#include <iostream>
#include <string>

using namespace std;

struct MoveNode
{
    string move;
    MoveNode* next;
};

class RobotNavigationModule
{
private:
    MoveNode* top;

public:
    RobotNavigationModule();

    ~RobotNavigationModule();

    bool isEmpty();

    void recordMove(string move);

    string popMove();

    void displayNavigationLog();

    void returnToStart();

    void obstacleDetected();
};

#endif