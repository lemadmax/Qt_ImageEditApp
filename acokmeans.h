#ifndef ACOKMEANS_H
#define ACOKMEANS_H

#include<ctime>
#include<algorithm>
#include "kmeans.h"

#include "mainwindow.h"

class AcoKmeans
{
public:
    AcoKmeans(int k, int m, VVV rgb, Point dimension, double spatial, double w1, double w2, double w3, double Q, double karma, double evap, double alpha, double beta);

    VVV getRGBArray();
    double getFitness();
    double getJe();
    double getDmax();
    double getDmin();

private:
    VVV transform(vector<vector<int>> center, vector<vector<int>> label);
    double distance(vector<int> center, int x, int y);
    bool issimilar(vector<vector<int>> center, vector<vector<int>> ncenter);
    double calculateCenDis(vector<vector<int>> center, int x, int y);
    double calculateDis(vector<vector<int>> center, int x, int y, int e);

private:
    int k;
    int m;
    double evap = 0.1;
    int Q = 1;
    double karma;
    double alpha, beta;

    vector<vector<vector<double>>> pheromone;
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

#endif // ACOKMEANS_H
