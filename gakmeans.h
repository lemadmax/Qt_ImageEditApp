#ifndef GAKMEANS_H
#define GAKMEANS_H

#include<ctime>
#include<algorithm>
#include "kmeans.h"

#include "mainwindow.h"

class GaKmeans
{
public:
    GaKmeans(int k, int population, VVV rgb, Point dimension, double spatial, double w1, double w2, double w3, double pc, double pm);
    VVV getRGBArray();
    double getFitness();
    double getJe();
    double getDmax();
    double getDmin();

private:
    VVV transform(vector<vector<int>> center, vector<vector<int>> label);
    vector<vector<int>> genRandCen();

private:
    int k;
    int population;
    int pc, pm;
    int maxIt = 10;

    VVV rgb;
    Point dimension;
    double spatial;
    double w1, w2, w3;

    vector<vector<int>> label;
    double Je = 0;
    double dmax = 0;
    double dmin = 0;
    double fitness = 0;
};

#endif // GAKMEANS_H
