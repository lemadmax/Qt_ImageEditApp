#ifndef MYPBA_H
#define MYPBA_H

#include<ctime>
#include<algorithm>

#include "mainwindow.h"

class myPba
{
public:
    myPba(int C, int D, VVV rgb, Point dimension, double spatial, double w1, double w2, double w3);

    VVV getRGBArray();

    vector<vector<int>> getLabel();
    double getJe();
    double getDmax();
    double getDmin();
    double getFitness();

private:

    struct Field{
          int x[100];
          int y[100];
          int r[100];
          int g[100];
          int b[100];
          double Je = 0;
          double dmax = 0;
          double dmin = 0;
          double fitness = 0;
          vector<vector<int>> label;
          bool operator < (const Field &a) const {
              return fitness < a.fitness;
          }
    };

    void calculateFit(Field &center);

    double calculateDis(int x, int y, Field center, int e);

    double calculateDisDen(int x, int y, Field center, int e);

    double calculateCenDis(int x, int y, Field center);

    double calculateCenDisDen(int x, int y, Field center);

    VVV transform(Field center, vector<vector<int>> label);

private:
    vector<Field> comb;

    int C;
    int D;
    int V;
    int S;
    int RS;
    int L;
    int RL;
    int X;
    int Y;
    int Z;
    int E;
    int R;
    int G;
    double Q;
    double ifspatial;

    int disw = 50;

    VVV rgb;
    Point dimension;
    double w1, w2, w3;

    double Je = 0;
    double dmax = 0;
    double dmin = 0;
    double fitness = 0;
};

#endif // MYPBA_H
