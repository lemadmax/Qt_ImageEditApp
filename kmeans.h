#ifndef KMEANS_H
#define KMEANS_H
#include"mainwindow.h"

class Kmeans
{
public:
    Kmeans(int k, double ifspatial, VVV rgb, Point dimension, double w1, double w2, double w3);

    VVV getRGBArray();
    vector<vector<int>> runkMeans(vector<vector<int>> centerIn);

    void setcenter(vector<vector<int>> cent);

    double getJe();
    double getDmax();
    double getDmin();
    double getFittness();
    vector<vector<int>> getLabel();
    vector<vector<int>> getCenter();
    vector<vector<int>> assignPixels();

private:

    double calculateDis(int x, int y, int e);

    double calculateCenDis(int x, int y);

    double calculateDisDen(int x, int y, int e);

    bool issimilar();

    VVV setnewRGBArray(vector<vector<int>> label);

private:
    int k;
    double ifspatial;
    VVV rgb;
    Point dimension;
    vector<vector<int>> center;
    vector<vector<int>> ncenter;
    vector<vector<int>> Label;
    double w1, w2, w3;

    double Je;
    double dmax;
    double dmin;
    double fittness;

};

#endif // KMEANS_H
