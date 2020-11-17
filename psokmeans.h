#ifndef PSOKMEANS_H
#define PSOKMEANS_H


#include<ctime>
#include<algorithm>
#include "kmeans.h"

#include "mainwindow.h"

class psokmeans
{
public:
    psokmeans(int C, int S, int tmax, VVV rgb, Point dimension, double spatial, double w1, double w2, double w3, double w, double c1, double c2);

    VVV getRGBArray();
    double getFitness();
    double getJe();
    double getDmax();
    double getDmin();

private:
    VVV transform(vector<vector<int>> center, vector<vector<int>> label);

private:
    int C;
    int S;
    int tmax;

    VVV rgb;
    Point dimension;
    double spatial;
    double w1, w2, w3;
    double w, c1, c2;

    double Je = 0;
    double dmax = 0;
    double dmin = 0;
    double fitness = 0;
};

#endif // PSOKMEANS_H
