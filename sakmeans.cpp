#include "sakmeans.h"

SaKmeans::SaKmeans(int k, double E, VVV rgb, Point dimension, double spatial, double w1, double w2, double w3)
{
    srand((unsigned)time(NULL));
    this->k = k;
    this->E = E;
    this->rgb = rgb;
    this->dimension = dimension;
    this->w1 = w1;
    this->w2 = w2;
    this->w3 = w3;
    this->spatial = spatial;
}


VVV SaKmeans::getRGBArray(){

    double T = 500;
    vector<vector<int>> centers(k, vector<int>(5));
    for(int i = 0; i < k; i++) {
        centers[i][0] = rand() % dimension.x;
        centers[i][1] = rand() % dimension.y;
        centers[i][2] = rgb[1][centers[i][0]][centers[i][1]];
        centers[i][3] = rgb[2][centers[i][0]][centers[i][1]];
        centers[i][4] = rgb[3][centers[i][0]][centers[i][1]];
    }
    Kmeans kmeans(k, spatial, rgb, dimension, w1, w2, w3);
    label = kmeans.runkMeans(centers);
    centers = kmeans.getCenter();
    Je = kmeans.getJe();
    dmax = kmeans.getDmax();
    dmin = kmeans.getDmin();
    fitness = kmeans.getFittness();
    while(T) {
        int randi = rand() % k;
        int randn = rand() % dimension.x;
        vector<vector<int>> ncenters = centers;
        ncenters[randi][0] = (ncenters[randi][0] + randn) % dimension.x;
        ncenters[randi][1] = (ncenters[randi][1] + randn) % dimension.y;
        for(int i = 2; i < 5; i++) {
            ncenters[randi][i] = rgb[i - 1][ncenters[randi][0]][ncenters[randi][1]];
        }
        vector<vector<int>> nlabel = kmeans.runkMeans(ncenters);
        double randp = double(rand() % 100) / 100.0;
        double p = exp(-E*2/T);
        if(kmeans.getFittness() < fitness || randp < p) {
            centers = ncenters;
            Je = kmeans.getJe();
            dmax = kmeans.getDmax();
            dmin = kmeans.getDmin();
            fitness = kmeans.getFittness();
            label = nlabel;
        }
        T-=E;
    }

    return transform(centers, label);

}

VVV SaKmeans::transform(vector<vector<int>> center, vector<vector<int>> label) {
    VVV newrgb = rgb;
    for(int i = 0; i < dimension.x; i++) {
        for(int j = 0; j < dimension.y; j++) {
            newrgb[1][i][j] = center[label[i][j]][2];
            newrgb[2][i][j] = center[label[i][j]][3];
            newrgb[3][i][j] = center[label[i][j]][4];
        }
    }
    return newrgb;
}

double SaKmeans::getFitness(){
    return fitness;
}
double SaKmeans::getJe(){
    return Je;
}
double SaKmeans::getDmax(){
    return dmax;
}
double SaKmeans::getDmin(){
    return dmin;
}
