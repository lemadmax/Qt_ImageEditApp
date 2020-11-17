#include "mypba.h"

myPba::myPba(int C, int D, VVV rgb, Point dimension, double spatial, double w1, double w2, double w3)
{
    srand((unsigned)time(NULL));
    this->C = C;
    this->D = D;
    this->rgb = rgb;
    this->dimension = dimension;
    this->ifspatial = spatial;
    this->w1 = w1;
    this->w2 = w2;
    this->w3 = w3;
    V = 100;
    S = C / 2 * D * 2;
    RS = S;
    L = S / C + C;
    RL = L;
    X = C / 3;
    Y = C / 3 + (C % 3 > 0 ? 1 : 0);
//    X = L / 3;
//    Y = L / 3 + (L % 3 > 0 ? 1 : 0);
    Z = 1;
    E = max(1, S / 2);
    R = max(1, ((V / 2) / 100) * E);
    G = C - X - Y;
    Q = 0.95;
}

void myPba::calculateFit(Field &center) {
    vector<vector<int>> label;
    label.resize(dimension.x);
    for(int i = 0; i < dimension.x; i++) {
        label[i].resize(dimension.y);
    }
    double sumd[100];
    memset(sumd, 0, sizeof(double) * 100);
    int cnt[100];
    memset(cnt, 0, sizeof(int) * 100);
    double Je = 0;
    double dmax = 0;
    double dmin = DBL_MAX;
//        cout << 8 << endl;
//        for(int i = 0; i < c; i++) {
//            cout << i << "th center: ";
//            cout << center.x[i] << ' ';
//            cout << center.y[i] << ' ';
//            cout << center.r[i] << ' ';
//            cout << center.g[i] << ' ';
//            cout << center.b[i] << ' ' << endl;
//        }
//        cout << "dimen: " << dimension.x << ' ' << dimension.y << endl;
//        cout << "c = " << c << endl;
//        cout << "spatial: " << ifspatial << endl;
    for(int i = 0; i < dimension.x; i++) {
        for(int j = 0; j < dimension.y; j++) {
            double mindis = 999999;
            for(int t = 0; t < C; t++) {
                double dis = 0;
                dis = calculateDis(i, j, center, t);
                if(dis < mindis) {
                    mindis = dis;
                    label[i][j] = t;
                }
            }
            sumd[label[i][j]] += mindis;
            cnt[label[i][j]]++;
        }
    }
    for(int i = 0; i < C; i++) {
        if(cnt[i] > 0) {
            sumd[i] = sumd[i] / cnt[i];
            dmax = sumd[i] > dmax ? sumd[i] : dmax;
            Je += sumd[i];
        }
        double sum1 = 0;
        for(int j = 0; j < C; j++) {
            if(i != j) {
                sum1 += calculateCenDis(i, j, center);
            }
        }
        sum1 = sum1 / (C - 1);
        dmin = sum1 < dmin ? sum1 : dmin;
    }
    Je = Je / C;
    center.Je = Je;
    center.dmax = dmax;
    center.dmin = dmin;
//    double temp = pow(255, 2);
//    double maxdis = sqrt(temp * 5);
    center.fitness = w1 * Je + w2 * dmax - (w3 * dmin);
//    center.fitness = Je;
    center.label = label;
}

double myPba::calculateDis(int x, int y, Field center, int e) {
    double deltadx = ifspatial * ((x - center.x[e]) * 255) / dimension.x;
    double deltady = ifspatial * ((y - center.y[e]) * 255) / dimension.y;
    double deltar = rgb[1][x][y] - center.r[e];
    double deltag = rgb[2][x][y] - center.g[e];
    double deltab = rgb[3][x][y] - center.b[e];
    double dis = sqrt(pow(deltadx, 2) + pow(deltady, 2) + pow(deltar, 2) + pow(deltag, 2) + pow(deltab, 2));
    return dis;
}

double myPba::calculateDisDen(int x, int y, Field center, int e) {
    double deltar = rgb[1][x][y] - center.r[e];
    double deltag = rgb[2][x][y] - center.g[e];
    double deltab = rgb[3][x][y] - center.b[e];
    double dis = sqrt(pow(deltar, 2) + pow(deltag, 2) + pow(deltab, 2));
    return dis;
}

double myPba::calculateCenDis(int x, int y, Field center) {
    double deltadx = ifspatial * ((center.x[x] - center.x[y]) * 255) / dimension.x;
    double deltady = ifspatial * ((center.x[x] - center.y[y]) * 255) / dimension.y;
    double deltar = center.r[x] - center.r[y];
    double deltag = center.g[x] - center.g[y];
    double deltab = center.b[x] - center.b[y];
    double dis = sqrt(pow(deltadx, 2) + pow(deltady, 2) + pow(deltar, 2) + pow(deltag, 2) + pow(deltab, 2));
    return dis;
}

double myPba::calculateCenDisDen(int x, int y, Field center) {
    double deltar = center.r[x] - center.r[y];
    double deltag = center.g[x] - center.g[y];
    double deltab = center.b[x] - center.b[y];
    double dis = sqrt(pow(deltar, 2) + pow(deltag, 2) + pow(deltab, 2));
    return dis;
}

VVV myPba::transform(Field center, vector<vector<int>> label) {
    VVV newrgb = rgb;
    for(int i = 0; i < dimension.x; i++) {
        for(int j = 0; j < dimension.y; j++) {
            newrgb[1][i][j] = center.r[label[i][j]];
            newrgb[2][i][j] = center.g[label[i][j]];
            newrgb[3][i][j] = center.b[label[i][j]];
        }
    }
    return newrgb;
}


VVV myPba::getRGBArray() {

    while(V > 0 && RS >= C) {
        for(int i = 0; i < L - RL + 1; i++) {
            for(int j = 0; j < RL * C; j+=C) {
                Field center;
                for(int t = 0; t < C; t++) {
                    center.x[t] = rand() % dimension.x;
                    center.y[t] = rand() % dimension.y;
                    center.r[t] = rgb[1][center.x[t]][center.y[t]];
                    center.g[t] = rgb[2][center.x[t]][center.y[t]];
                    center.b[t] = rgb[3][center.x[t]][center.y[t]];
                }
                calculateFit(center);
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
            }
        }
        for(int i = 0; i < X; i++) {
            for(int j = 0; j < E; j++) {
                double W = (rand() % (j + 2)) * Q * (rand() % 2 == 0 ? -1 : 1);
                if(W == 0) {
                    W = Q - (E / 100) * (rand() % 2 == 0 ? -1 : 1);
                }
                if((double)(rand() % 100) / 100.0 <= (1 - Q)) {
                    W = rand() % 256;
                }
                Field fPollen;
                for(int t = 0; t < C; t++) {
                    fPollen.x[t] = comb[i].x[t] + (W * (dimension.x - 1) / 255);
                    if(fPollen.x[t] >= dimension.x) fPollen.x[t] = dimension.x - 1;
                    if(fPollen.x[t] < 0) fPollen.x[t] = 0;
                    fPollen.y[t] = comb[i].y[t] + (W * (dimension.y - 1) / 255);
                    if(fPollen.y[t] >= dimension.x) fPollen.y[t] = dimension.y - 1;
                    if(fPollen.y[t] < 0) fPollen.y[t] = 0;
                    fPollen.r[t] = comb[i].r[t] + W;
                    if(fPollen.r[t] > 255) fPollen.r[t] = 255;
                    if(fPollen.r[t] < 0) fPollen.r[t] = 0;
                    fPollen.g[t] = comb[i].g[t] + W;
                    if(fPollen.g[t] > 255) fPollen.g[t] = 255;
                    if(fPollen.g[t] < 0) fPollen.g[t] = 0;
                    fPollen.b[t] = comb[i].b[t] + W;
                    if(fPollen.b[t] > 255) fPollen.b[t] = 255;
                    if(fPollen.b[t] < 0) fPollen.b[t] = 0;
                }
                calculateFit(fPollen);
                if(fPollen.fitness < comb[i].fitness) {
                    comb[i] = fPollen;
                }
            }
        }

        for(int i = X; i < Y + X; i++) {
            for(int j = 0; j < R; j++) {
                double W = (rand() % (j + 2)) * Q * (rand() % 2 == 0 ? -1 : 1);
                if(W == 0) {
                    W = Q - (R / 100) * (rand() % 2 == 0 ? -1 : 1);
                }
                if((double)(rand() % 100) / 100.0 <= (Q * D / 100)) {
                    W = rand() % 256;
                }
                Field fPollen;
                for(int t = 0; t < C; t++) {
                    fPollen.x[t] = comb[i].x[t] + (W * (dimension.x - 1) / 255);
                    if(fPollen.x[t] >= dimension.x) fPollen.x[t] = dimension.x - 1;
                    if(fPollen.x[t] < 0) fPollen.x[t] = 0;
                    fPollen.y[t] = comb[i].y[t] + (W * (dimension.y - 1) / 255);
                    if(fPollen.y[t] >= dimension.x) fPollen.y[t] = dimension.y - 1;
                    if(fPollen.y[t] < 0) fPollen.y[t] = 0;
                    fPollen.r[t] = comb[i].r[t] + W;
                    if(fPollen.r[t] > 255) fPollen.r[t] = 255;
                    if(fPollen.r[t] < 0) fPollen.r[t] = 0;
                    fPollen.g[t] = comb[i].g[t] + W;
                    if(fPollen.g[t] > 255) fPollen.g[t] = 255;
                    if(fPollen.g[t] < 0) fPollen.g[t] = 0;
                    fPollen.b[t] = comb[i].b[t] + W;
                    if(fPollen.b[t] > 255) fPollen.b[t] = 255;
                    if(fPollen.b[t] < 0) fPollen.b[t] = 0;
                }
                calculateFit(fPollen);
                if(fPollen.fitness < comb[i].fitness) {
                    comb[i] = fPollen;
                }
            }
        }

        for(int i = X + Y; i < Y + X + Z; i++) {
            for(int j = 0; j < G; j++) {
                double W = (rand() % (j + 2)) * Q * (rand() % 2 == 0 ? -1 : 1);
                if(W == 0) {
                    W = Q - (G / 100) * (rand() % 2 == 0 ? -1 : 1);
                }
                if((double)(rand() % 100) / 100.0 < Q) {
                    W = rand() % 256;
                }
                Field fPollen;
                for(int t = 0; t < C; t++) {
                    fPollen.x[t] = comb[i].x[t] + (W * (dimension.x - 1) / 255);
                    if(fPollen.x[t] >= dimension.x) fPollen.x[t] = dimension.x - 1;
                    if(fPollen.x[t] < 0) fPollen.x[t] = 0;
                    fPollen.y[t] = comb[i].y[t] + (W * (dimension.y - 1) / 255);
                    if(fPollen.y[t] >= dimension.x) fPollen.y[t] = dimension.y - 1;
                    if(fPollen.y[t] < 0) fPollen.y[t] = 0;
                    fPollen.r[t] = comb[i].r[t] + W;
                    if(fPollen.r[t] > 255) fPollen.r[t] = 255;
                    if(fPollen.r[t] < 0) fPollen.r[t] = 0;
                    fPollen.g[t] = comb[i].g[t] + W;
                    if(fPollen.g[t] > 255) fPollen.g[t] = 255;
                    if(fPollen.g[t] < 0) fPollen.g[t] = 0;
                    fPollen.b[t] = comb[i].b[t] + W;
                    if(fPollen.b[t] > 255) fPollen.b[t] = 255;
                    if(fPollen.b[t] < 0) fPollen.b[t] = 0;
                }
                calculateFit(fPollen);
                if(fPollen.fitness < comb[i].fitness) {
                    comb[i] = fPollen;
                }
            }
        }
        sort(comb.begin(), comb.end());

        V = (V - D) > 0 ? (V - D) : 0;
        RS = RS / 3 * 3 * V / 100;
        RL = RS / C;
        E = max(1, RS / 2);
        R = max(1, (V / 2 / 100) * E);
        G = C - X - Y;

//        for(int i = 0; i < L; i++) {
//            for(int j = 0; j < C; j++) {
//                cout << i << "th comb: ";
//                cout << comb[i].x[j] << ' ';
//                cout << comb[i].y[j] << ' ';
//                cout << comb[i].r[j] << ' ';
//                cout << comb[i].g[j] << ' ';
//                cout << comb[i].b[j] << ' ' << endl;
//            }
//        }
        for(int i = 0; i < RL; i++) {
            cout << comb[i].fitness << ' ';
        }
        cout << endl;

    }
    Je = comb[0].Je;
    dmax = comb[0].dmax;
    dmin = comb[0].dmin;
    fitness = comb[0].fitness;

    VVV newrgb = transform(comb[0], comb[0].label);
    return newrgb;
}

vector<vector<int>> myPba::getLabel() {
    return comb[0].label;
}

double myPba::getJe() {
    return Je;
}
double myPba::getDmax() {
    return dmax;
}
double myPba::getDmin() {
    return dmin;
}
double myPba::getFitness() {
    return fitness;
}
