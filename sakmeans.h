#ifndef SAKMEANS_H
#define SAKMEANS_H

#include<ctime>
#include<algorithm>
#include "kmeans.h"

#include "mainwindow.h"

class SaKmeans
{
public:
    SaKmeans(int k, double E, VVV rgb, Point dimension, double spatial, double w1, double w2, double w3);
    VVV getRGBArray();
    double getFitness();
    double getJe();
    double getDmax();
    double getDmin();

private:
    VVV transform(vector<vector<int>> center, vector<vector<int>> label);

private:
    int k;
    double E;

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

#endif // SAKMEANS_H
