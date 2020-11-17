#ifndef PBAKMEANS_H
#define PBAKMEANS_H

#include<ctime>
#include<algorithm>
#include "kmeans.h"

#include "mainwindow.h"

class PbaKmeans
{
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
          vector<vector<int>> center;
          bool operator < (const Field &a) const {
              return fitness < a.fitness;
          }
    };

public:
    PbaKmeans(int C, int D, VVV rgb, Point dimension, double spatial, double w1, double w2, double w3);

    VVV getRGBArray();
    vector<vector<int>> getLabel();
    double getFitness();
    double getJe();
    double getDmax();
    double getDmin();

private:
    VVV transform(vector<vector<int>> center, vector<vector<int>> label);

private:
    int C;
    int D;

    int V = 100;
    int S;
    int RS;
    int L;
    int RL;
    int E;
    int R;
    int G;
    int X;
    int Y;
    int Z;
    double Q;

    VVV rgb;
    Point dimension;
    double spatial;
    double w1, w2, w3;

    double Je = 0;
    double dmax = 0;
    double dmin = 0;
    double fitness = 0;
    vector<Field> comb;
};

#endif // PBAKMEANS_H
