#include "pbakmeans.h"

PbaKmeans::PbaKmeans(int C, int D, VVV rgb, Point dimension, double spatial, double w1, double w2, double w3)
{
    srand((unsigned)time(NULL));
    this->C = C;
    this->D = D;
    this->rgb = rgb;
    this->dimension = dimension;
    this->spatial = spatial;
    this->w1 = w1;
    this->w2 = w2;
    this->w3 = w3;
    S = D * C;
    RS = S;
    L = S / C;
    RL = L;
    E = max(1, S / 2);
    R = max(1, E * V / 200);
    X = RL / 3;
    Y = RL / 3 + RL % 3;
    Z = RL - X - Y;
    Q = 0.95;
    G = Z;
}

VVV PbaKmeans::getRGBArray() {

    VVV newrgb = rgb;

    while(V > 0 && RS >= C) {

        for(int i = 0; i < L - RL + 1; i++) {
            for(int j = 0; j < RL; j++) {
                Field center;
                Kmeans kmeans(C, spatial, rgb, dimension, w1, w2, w3);
                vector<vector<int>> kcen;
                kcen.resize(C);
                for(int t = 0; t < C; t++) {
                    center.x[t] = rand() % dimension.x;
                    kcen[t].push_back(center.x[t]);
                    center.y[t] = rand() % dimension.y;
                    kcen[t].push_back(center.y[t]);
                    center.r[t] = rgb[1][center.x[t]][center.y[t]];
                    kcen[t].push_back(center.r[t]);
                    center.g[t] = rgb[2][center.x[t]][center.y[t]];
                    kcen[t].push_back(center.g[t]);
                    center.b[t] = rgb[3][center.x[t]][center.y[t]];
                    kcen[t].push_back(center.b[t]);
                }

                center.label = kmeans.runkMeans(kcen);
                center.center = kmeans.getCenter();
                center.Je = kmeans.getJe();
                center.dmax = kmeans.getDmax();
                center.dmin = kmeans.getDmin();
                center.fitness = kmeans.getFittness();
//                cout << j << endl;
//                cout << RL << endl;
//                cout << "start1" << endl;
                if(comb.size() < RL) {
                    comb.push_back(center);
                    sort(comb.begin(), comb.end());
                }
                else {
                    if(center.fitness < comb[RL - 1].fitness) {
                        comb[RL - 1] = center;
                        sort(comb.begin(), comb.end());

                    }
                }
//                cout << "end1" << endl;
            }
        }
//        cout << comb.size() << endl;
//        cout << "start" << endl;
        for(int i = 0; i < X; i++) {
            for(int j = 0; j < E; j++) {
                double W = (rand() % 256) * (1 - Q) * (j + 1) *  (rand() % 2 == 0 ? -1 : 1);
                if(((double)(rand() % 100)) / 100.0 < (1 - Q)) {
                    W = rand() % 256 * (rand() % 2 == 0 ? -1 : 1);
                }
                Field fPollen;
                Kmeans kmeans(C, spatial, rgb, dimension, w1, w2, w3);
                vector<vector<int>> kcen;
                kcen.resize(C);
                for(int t = 0; t < C; t++) {
                    fPollen.x[t] = comb[i].x[t] + (W * (dimension.x - 1) / 255);
                    if(fPollen.x[t] >= dimension.x) fPollen.x[t] = dimension.x - 1;
                    if(fPollen.x[t] < 0) fPollen.x[t] = 0;
                    kcen[t].push_back(fPollen.x[t]);
                    fPollen.y[t] = comb[i].y[t] + (W * (dimension.y - 1) / 255);
                    if(fPollen.y[t] >= dimension.x) fPollen.y[t] = dimension.y - 1;
                    if(fPollen.y[t] < 0) fPollen.y[t] = 0;
                    kcen[t].push_back(fPollen.y[t]);
                    fPollen.r[t] = comb[i].r[t] + W;
                    if(fPollen.r[t] > 255) fPollen.r[t] = 255;
                    if(fPollen.r[t] < 0) fPollen.r[t] = 0;
                    kcen[t].push_back(fPollen.r[t]);
                    fPollen.g[t] = comb[i].g[t] + W;
                    if(fPollen.g[t] > 255) fPollen.g[t] = 255;
                    if(fPollen.g[t] < 0) fPollen.g[t] = 0;
                    kcen[t].push_back(fPollen.g[t]);
                    fPollen.b[t] = comb[i].b[t] + W;
                    if(fPollen.b[t] > 255) fPollen.b[t] = 255;
                    if(fPollen.b[t] < 0) fPollen.b[t] = 0;
                    kcen[t].push_back(fPollen.b[t]);
                }
                fPollen.label = kmeans.runkMeans(kcen);
                fPollen.center = kmeans.getCenter();
                fPollen.Je = kmeans.getJe();
                fPollen.dmax = kmeans.getDmax();
                fPollen.dmin = kmeans.getDmin();
                fPollen.fitness = kmeans.getFittness();
//                cout << i << endl;
                if(fPollen.fitness < comb[i].fitness) {
                    comb[i] = fPollen;
                }
            }
        }

        for(int i = X; i < X + Y; i++) {
            for(int j = 0; j < R; j++) {
                double W = (rand() % 256) * (1 - Q) * (j + 1) *  (rand() % 2 == 0 ? -1 : 1);
                if(((double)(rand() % 100)) / 100.0 < (Q * D / 100)) {
                    W = rand() % 256 * (rand() % 2 == 0 ? -1 : 1);
                }
                Field fPollen;
                Kmeans kmeans(C, spatial, rgb, dimension, w1, w2, w3);
                vector<vector<int>> kcen;
                kcen.resize(C);
                for(int t = 0; t < C; t++) {
                    fPollen.x[t] = comb[i].x[t] + (W * (dimension.x - 1) / 255);
                    if(fPollen.x[t] >= dimension.x) fPollen.x[t] = dimension.x - 1;
                    if(fPollen.x[t] < 0) fPollen.x[t] = 0;
                    kcen[t].push_back(fPollen.x[t]);
                    fPollen.y[t] = comb[i].y[t] + (W * (dimension.y - 1) / 255);
                    if(fPollen.y[t] >= dimension.x) fPollen.y[t] = dimension.y - 1;
                    if(fPollen.y[t] < 0) fPollen.y[t] = 0;
                    kcen[t].push_back(fPollen.y[t]);
                    fPollen.r[t] = comb[i].r[t] + W;
                    if(fPollen.r[t] > 255) fPollen.r[t] = 255;
                    if(fPollen.r[t] < 0) fPollen.r[t] = 0;
                    kcen[t].push_back(fPollen.r[t]);
                    fPollen.g[t] = comb[i].g[t] + W;
                    if(fPollen.g[t] > 255) fPollen.g[t] = 255;
                    if(fPollen.g[t] < 0) fPollen.g[t] = 0;
                    kcen[t].push_back(fPollen.g[t]);
                    fPollen.b[t] = comb[i].b[t] + W;
                    if(fPollen.b[t] > 255) fPollen.b[t] = 255;
                    if(fPollen.b[t] < 0) fPollen.b[t] = 0;
                    kcen[t].push_back(fPollen.b[t]);
                }
                fPollen.label = kmeans.runkMeans(kcen);
                fPollen.center = kmeans.getCenter();
                fPollen.Je = kmeans.getJe();
                fPollen.dmax = kmeans.getDmax();
                fPollen.dmin = kmeans.getDmin();
                fPollen.fitness = kmeans.getFittness();
//                cout << i << endl;
                if(fPollen.fitness < comb[i].fitness) {
                    comb[i] = fPollen;
                }
            }
        }

        for(int i = X + Y; i < X + Y + Z; i++) {
            for(int j = 0; j < E; j++) {
                double W = (rand() % 256) * (1 - Q) * (j + 1) *  (rand() % 2 == 0 ? -1 : 1);
                if(((double)(rand() % 100)) / 100.0 < Q) {
                    W = rand() % 256 * (rand() % 2 == 0 ? -1 : 1);
                }
                Field fPollen;
                Kmeans kmeans(C, spatial, rgb, dimension, w1, w2, w3);
                vector<vector<int>> kcen;
                kcen.resize(C);
                for(int t = 0; t < C; t++) {
                    fPollen.x[t] = comb[i].x[t] + (W * (dimension.x - 1) / 255);
                    if(fPollen.x[t] >= dimension.x) fPollen.x[t] = dimension.x - 1;
                    if(fPollen.x[t] < 0) fPollen.x[t] = 0;
                    kcen[t].push_back(fPollen.x[t]);
                    fPollen.y[t] = comb[i].y[t] + (W * (dimension.y - 1) / 255);
                    if(fPollen.y[t] >= dimension.x) fPollen.y[t] = dimension.y - 1;
                    if(fPollen.y[t] < 0) fPollen.y[t] = 0;
                    kcen[t].push_back(fPollen.y[t]);
                    fPollen.r[t] = comb[i].r[t] + W;
                    if(fPollen.r[t] > 255) fPollen.r[t] = 255;
                    if(fPollen.r[t] < 0) fPollen.r[t] = 0;
                    kcen[t].push_back(fPollen.r[t]);
                    fPollen.g[t] = comb[i].g[t] + W;
                    if(fPollen.g[t] > 255) fPollen.g[t] = 255;
                    if(fPollen.g[t] < 0) fPollen.g[t] = 0;
                    kcen[t].push_back(fPollen.g[t]);
                    fPollen.b[t] = comb[i].b[t] + W;
                    if(fPollen.b[t] > 255) fPollen.b[t] = 255;
                    if(fPollen.b[t] < 0) fPollen.b[t] = 0;
                    kcen[t].push_back(fPollen.b[t]);
                }
                fPollen.label = kmeans.runkMeans(kcen);
                fPollen.center = kmeans.getCenter();
                fPollen.Je = kmeans.getJe();
                fPollen.dmax = kmeans.getDmax();
                fPollen.dmin = kmeans.getDmin();
                fPollen.fitness = kmeans.getFittness();
//                cout << i << endl;
                if(fPollen.fitness < comb[i].fitness) {
                    comb[i] = fPollen;
                }
            }
        }
        sort(comb.begin(), comb.end());

        cout << "RS = " << RS << endl;
        cout << "RL = " << RL << endl;
        for(int i = 0; i < RL; i++) {
            cout << comb[i].fitness << ' ';
        }
        cout << endl;

        V = (V - D) > 0 ? (V - D) : 0;
        RS = RS * V / 100;
        RL = RS / C;
        E = max(1, RS / 2);
        R = max(1, E * V / 200);
        X = RL / 3;
        Y = RL / 3 + RL % 3;
        Z = RL - X - Y;
        G = Z;

    }

    Je = comb[0].Je;
    dmax = comb[0].dmax;
    dmin = comb[0].dmin;
    fitness = comb[0].fitness;
    return transform(comb[0].center, comb[0].label);
}

VVV PbaKmeans::transform(vector<vector<int>> center, vector<vector<int>> label) {
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

vector<vector<int>> PbaKmeans::getLabel() {
    return comb[0].label;
}

double PbaKmeans::getFitness(){
    return fitness;
}
double PbaKmeans::getJe(){
    return Je;
}
double PbaKmeans::getDmax(){
    return dmax;
}
double PbaKmeans::getDmin(){
    return dmin;
}
