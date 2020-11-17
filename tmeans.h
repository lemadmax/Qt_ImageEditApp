#ifndef TMEANS_H
#define TMEANS_H
#include"mainwindow.h"

class tmeans
{
public:
    tmeans(int k, double T, int width, int height, VVV rgb, double spatial, double w1, double w2, double w3);

    vector<vector<int>> getLabel();
    double getAccuracy();
    double getJe();
    double getDmax();
    double getDmin();
    double getFitness();
    double getF1();

private:
    double calcuateDis(vector<int> a, vector<int> b);
    vector<int> calNewCenter(vector<int> center);
    bool terminateCrit(vector<int> center, vector<int> ncenter);

private:
    int k;
    int dimension;
    int width;
    int height;
    double t;
    VVV rgb;
    double spatial;
    double w1, w2, w3;

    vector<vector<int>> label;

    double Je;
    double dmax;
    double dmin;
    double fitness;
    double accuracy;
    double F1;
    vector<double> f1;
};

#endif // TMEANS_H
