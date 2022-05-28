#include <iostream>
#include <algorithm>
#include <stdio.h>
#include <cstdlib>
#include <vector>
#include "MazeTriversal.h"
using namespace std;

int GRID_SIZE = 17;
float ALPHA = 0.01;
int R_GOAL = 0;
int R_HIT_WALL = -5;
int R_STEP = -1;
int GOAL[2] = {GRID_SIZE-1,GRID_SIZE-1};
int EPISODE_LENGTH_MAX = 400;
//Action 0: up 1: down 2: left 3: right 4: stay
int Actions[5][2] = {{-1,0},{1,0},{0,-1},{0,1},{0,0}};
string ActionNames[5] = {"up", "down", "left", "right", "stay"};

int** createMaze(int gridSize)
{
    //Creating Empty Wall
    int** Maze;
    Maze = new int*[gridSize];
    for (int i = 0; i < gridSize; i++)
    {
        Maze[i] = new int[gridSize];

        for (int j = 0; j < gridSize; j++)
        {
            Maze[i][j] = 0;
        }
    }

    //Horizontal Wall
    for (int i = 0 ; i < 12; i++)
    {
        Maze[3][i] = 1;
    }

    for (int i = 10 ; i < gridSize; i++)
    {
        Maze[5][i] = 1;
    }

    for (int i = 3 ; i < 12; i++)
    {
        Maze[8][i] = 1;
    }

    for (int i = 4 ; i < gridSize; i++)
    {
        Maze[11][i] = 1;
    }

    //Vertical Wall
    for (int i = 8 ; i < 12; i++)
    {
        Maze[i][2] = 2;
    } 

    Maze[GOAL[0]][GOAL[1]]= 4;
    return Maze;
}

void showMaze(int** Maze)
{
    for (int i = 0 ; i < GRID_SIZE ; i++)
    {
        for (int j = 0 ; j < GRID_SIZE ; j++)
        {
            cout << Maze[i][j];
        }
        cout<< "\n";
    }
    cout << "\n";
    return;
}

bool HitWall(int curr[], int action, int** Maze)
{
    int filler1 = curr[0]+Actions[action][0];
    int filler2 = curr[1]+Actions[action][1];
    int s_new[2] = {filler1,filler2};
    if (*min_element(begin(s_new),end(s_new)) < 0 ||  *max_element(begin(s_new),end(s_new))  >  (GRID_SIZE-1))
    {
        return true;
    }
    if ( (Maze[curr[0]][curr[1]] == 1 && action == 1) ||  (Maze[curr[0]][curr[1]] == 3 && action == 3) || (Maze[s_new[0]][s_new[1]] == 1 && action == 0) || (Maze[s_new[0]][s_new[1]] == 2 && action == 2)) 
    {
        return true;
    }
    
    return false;
}

int Reward(int state[],int action, int** Maze)
{
    if (state == GOAL)
    {
        return R_GOAL;
    }
    else if (HitWall(state,action,Maze) == true)
    {
        return R_HIT_WALL;
    }
    else
    {
        return R_STEP;
    }
}

int* s_next_calc(int state[], int action)
{
    int* s_new = new int[2];
    int filler1 = state[0]+Actions[action][0];
    int filler2 = state[1]+Actions[action][1];
    s_new[0]= filler1;
    s_new[1]= filler2;
    return s_new;
}


struct stepStruct {
    int* state;
    int reward, action;
};

typedef struct stepStruct Struct;

Struct step(int state[2], int action, int** Maze)
{
    Struct s;
    vector<int> action_list;
    action_list.push_back(action);
    int action_taken;
    int* s_next = new int[2];
    for (int i = 0 ; i< 5; i++)
    {
        if (i != action)
        {
            action_list.push_back(i);
        }
    }
    int random = rand() % 100;
    if (random <= (1-ALPHA)*100)
    {
        action_taken = action_list[0];
    }
    else
    {
        int random2 = rand()%4;
        action_taken = action_list[random2+1];
    }

    int r = Reward(state,action_taken,Maze);
    if (r == R_HIT_WALL || r == R_GOAL)
    {
        for (int i = 0 ; i < 2 ; i++)
        {
            s_next[i] = state[i];
        }
    }
    else
    {
        int* filler = s_next_calc(state,action_taken);
        for (int i = 0 ; i < 2 ; i++)
        {
            s_next[i] = filler[i];
        }     
    }
    s.state = s_next;
    s.reward = r;
    s.action = action_taken;
    return s;
}

int choose_action_epsilon(float*** Q, int* state, float epsilon)
{
    int action;
    if (int random = rand() % 100 < epsilon*100)
    {
        action = rand()%5;
        return action;
    }
    int stateAction[5];
    for (int i = 0; i < 5 ; i++)
    {
        stateAction[i]= Q[state[0]][state[1]][i];
    }
    return distance(stateAction, max_element(begin(stateAction),end(stateAction)));
}

float*** createQTable()
{
    float*** Q;
    Q = new float**[GRID_SIZE];
    for (int i = 0 ; i < GRID_SIZE; i++)
    {
        Q[i] = new float*[GRID_SIZE];
        for (int j = 0; j < GRID_SIZE; j++)
        {
            Q[i][j] = new float[5];
            for (int k = 0 ; k < 5; k++)
            {
                Q[i][j][k] = 0;
            }
        }
    }
    return Q;
}

float*** q_learning(float gamma, int episodes, float epsilon, int** Maze)
{
    //int** policy;
    float*** Q = createQTable();
    Q[GRID_SIZE-1][GRID_SIZE-1][4] = 1;
    int sum_steps = 0;
    for (int i = 0; i < episodes; i++)
    {
        int state[2] = {rand()%17, rand()%17};
        //int state[2] = {0,0};

        while (Maze[state[0]][state[1]] == 1 )
        {
            state[0] = rand()%17;
            state[1] = rand()%17;
        }
        int r_sum = 0;
        int j = 1;
        while ( (state[0] != GOAL[0] || state[1] != GOAL[1]) && j < EPISODE_LENGTH_MAX)
        {
            int action = choose_action_epsilon(Q,state,epsilon);
            Struct s = step(state,action,Maze);

            float max_Q_next = *max_element(Q[s.state[0]][s.state[1]], Q[s.state[0]][s.state[1]]+5);
            Q[state[0]][state[1]][s.action] += s.reward + gamma * max_Q_next -Q[state[0]][state[1]][s.action];
            state[0] = s.state[0];
            state[1] = s.state[1];
            //r_sum += s.reward;
            j+=1;
        }
        sum_steps += j;
    }
    return Q;
}

int** generatePolicy(float*** Q)
{
    int** policy = new int* [GRID_SIZE];
    for (int i = 0 ; i < GRID_SIZE ; i++)
    {
        policy[i] = new int [GRID_SIZE];
        for (int j = 0 ; j < GRID_SIZE ; j++)
        {

            policy[i][j] = distance(Q[i][j], max_element(Q[i][j],Q[i][j]+5));
        }
    }
    return policy;
}


void showPolicy(int** Maze)
{
    for (int i = 0 ; i < GRID_SIZE ; i++)
    {
        for (int j = 0 ; j < GRID_SIZE ; j++)
        {
            if (Maze[i][j] == 0)
            {
                cout << "^";
            }
            if (Maze[i][j] == 1)
            {
                cout << "v";
            }            
            if (Maze[i][j] == 2)
            {
                cout << "<";
            }            
            if (Maze[i][j] == 3)
            {
                cout << ">";
            }
            if (Maze[i][j] == 4)
            {
                cout << "o";
            }
        }
        cout<< "\n";
    }
    cout << "\n";
    return;
}