#pragma once
#include <iostream>
#include <stdio.h>
#include <vector>
#include "MazeTriversal.cpp"
using namespace std;

extern int GRID_SIZE;
extern float ALPHA;
extern int R_GOAL;
extern int R_HIT_WALL;
extern int R_STEP;
extern int GOAL[2];
extern int EPISODE_LENGTH_MAX;
//Action 0: up 1: down 2: left 3: right 4: stay
extern int Actions[5][2];
extern string ActionNames[5];
int** createMaze(int gridSize);
void showMaze(int** Maze);
bool HitWall(int curr[], int action, int** Maze);
int Reward(int state[],int action, int** Maze);
int* s_next_calc(int state[], int action);
int choose_action_epsilon(float*** Q, int* state, float epsilon);
float*** createQTable();
float*** q_learning(float gamma, int episodes, float epsilon, int** Maze);
int** generatePolicy(float*** Q);
void showPolicy(int** Maze);
