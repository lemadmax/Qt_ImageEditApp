#include "gakmeans.h"

GaKmeans::GaKmeans(int k, int population, VVV rgb, Point dimension, double spatial, double w1, double w2, double w3, double pc, double pm)
{
    srand((unsigned)time(NULL));
    this->k = k;
    this->population = population;
    this->rgb = rgb;
    this->dimension = dimension;
    this->pc = pc;
    this->pm = pm;
    this->w1 = w1;
    this->w2 = w2;
    this->w3 = w3;
    this->spatial = spatial;
}

VVV GaKmeans::getRGBArray(){

    Kmeans kmeans(k, spatial, rgb, dimension, w1, w2, w3);
    VVV strs(population, vector<vector<int>>(k, vector<int>(5)));
    for(int i = 0; i < population; i++) {
        for(int j = 0; j < k; j++) {
            strs[i][j][0] = rand() % dimension.x;
            strs[i][j][1] = rand() % dimension.y;
            strs[i][j][2] = rgb[1][strs[i][j][0]][strs[i][j][1]];
            strs[i][j][3] = rgb[2][strs[i][j][0]][strs[i][j][1]];
            strs[i][j][4] = rgb[3][strs[i][j][0]][strs[i][j][1]];
        }
        label = kmeans.runkMeans(strs[i]);
        strs[i] = kmeans.getCenter();
    }

    vector<vector<int>> bestLabel;
    vector<vector<int>> bestCen;
    fitness = DBL_MAX;
    int iteration = maxIt;
    while(iteration--) {
        for(int i = 0; i < population; i++) {
            int randi = rand() % population;
            double randp = double(rand() % 100) / 100.0;
            int randj = rand() % k;
            if(randp < pc) {
                vector<int> temp = strs[i][randj];
                strs[i][randj] = strs[randi][randj];
                strs[randi][randj] = temp;
            }
        }
        for(int i = 0; i < population; i++) {
            double randp = double(rand() % 100) / 100.0;
            int randj = rand() % k;
            if(randp < pm) {
                for(int j = 0; j < 5; j++) {
                    strs[i][randj][j] -= 10;
                }
            }
            else {
                for(int j = 0; j < 5; j++) {
                    strs[i][randj][j] += 10;
                }
            }
        }
        vector<double> fits(population);
        for(int i = 0; i < population; i++) {
            label = kmeans.runkMeans(strs[i]);
            strs[i] = kmeans.getCenter();
            fits[i] = kmeans.getFittness();
            if(fits[i] < fitness) {
                fitness = fits[i];
                Je = kmeans.getJe();
                dmax = kmeans.getDmax();
                dmin = kmeans.getDmin();
                bestLabel = label;
                bestCen = strs[i];
            }
        }
        for(int i = 0; i < population; i++) {
            cout << fits[i] << ' ';
        }
        cout << endl;
        for(int i = 0; i < population; i+=2) {
            if(fits[i] > fits[i + 1]) {
                strs[i] = genRandCen();
                label = kmeans.runkMeans(strs[i]);
                strs[i] = kmeans.getCenter();
            }
            else {
                strs[i + 1] = genRandCen();
                label = kmeans.runkMeans(strs[i + 1]);
                strs[i + 1] = kmeans.getCenter();
            }
        }
    }

    return transform(bestCen, bestLabel);
}

vector<vector<int>> GaKmeans::genRandCen() {
    vector<vector<int>> cen(k, vector<int>(5));
    for(int i = 0; i < k; i++) {
        cen[i][0] = rand() % dimension.x;
        cen[i][1] = rand() % dimension.y;
        cen[i][2] = rgb[1][cen[i][0]][cen[i][1]];
        cen[i][3] = rgb[2][cen[i][0]][cen[i][1]];
        cen[i][4] = rgb[3][cen[i][0]][cen[i][1]];
    }
    return cen;
}

VVV GaKmeans::transform(vector<vector<int>> center, vector<vector<int>> label) {
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

double GaKmeans::getFitness(){
    return fitness;
}
double GaKmeans::getJe(){
    return Je;
}
double GaKmeans::getDmax(){
    return dmax;
}
double GaKmeans::getDmin(){
    return dmin;
}
