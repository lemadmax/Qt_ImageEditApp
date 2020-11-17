#ifndef MYBA_H
#define MYBA_H

#include<ctime>
#include<algorithm>

#include"mainwindow.h"

using namespace std;

#define VVV vector<vector<vector<int>>>

class BeesAlgorithm{

public:
    BeesAlgorithm(int c, int n, int m,
           int elit, int nep, int nsp,
           double ngh, int imax,
           VVV rgb, Point d, double spatial,
                  double w1, double w2, double w3) {
            this->c = c;
            this->n = n;
            this->m = m;
            this->elit = elit;
            this->nep = nep;
            this->nsp = nsp;
            this->ngh = ngh;
            this->imax = imax;
            this->rgb = rgb;
            this->dimension = d;
            this->ifspatial = spatial;
            this->w1 = w1;
            this->w2 = w2;
            this->w3 = w3;
            srand((unsigned)time(NULL));
        }

private:
    int c;
    int n;
    int m;
    int elit;
    int nep;
    int nsp;
    double ngh;
    int imax;
    double ifspatial;

    double Je = 0;
    double dmax = 0;
    double dmin = 0;
    double fittness = 0;

    VVV rgb;
    Point dimension;
    double w1, w2, w3;

    struct Field {
        int x[100], y[100];
        int r[100], g[100], b[100];
        double fittness = 0;
        double Je = 0;
        double dmax = 0;
        double dmin = 0;
        vector<vector<int>> label;

        bool operator < (const Field &a) const {
            return fittness < a.fittness;
        }
    };

    vector<Field> comb;

private:

    void calculateFit(Field &center) {
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
                for(int t = 0; t < c; t++) {
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
        for(int i = 0; i < c; i++) {
            if(cnt[i] > 0) {
                sumd[i] = sumd[i] / cnt[i];
                dmax = sumd[i] > dmax ? sumd[i] : dmax;
                Je += sumd[i];
            }
            double sum1 = 0;
            for(int j = 0; j < c; j++) {
                if(i != j) {
                    sum1 += calculateCenDis(i, j, center);
                }
            }
            sum1 = sum1 / (c - 1);
            dmin = sum1 < dmin ? sum1 : dmin;
        }
        Je = Je / c;
        center.Je = Je;
        center.dmax = dmax;
        center.dmin = dmin;
//        double temp = pow(255, 2);
//        double maxdis = sqrt(temp * 5);
        center.fittness = w1 * Je + w2 * dmax - w3 * dmin;
//        center.fittness = Je;
        center.label = label;
    }

    double calculateDis(int x, int y, Field center, int e) {
        double deltadx = ifspatial * ((x - center.x[e]) * 255) / dimension.x;
        double deltady = ifspatial * ((y - center.y[e]) * 255) / dimension.y;
        double deltar = rgb[1][x][y] - center.r[e];
        double deltag = rgb[2][x][y] - center.g[e];
        double deltab = rgb[3][x][y] - center.b[e];
        double dis = sqrt(pow(deltadx, 2) + pow(deltady, 2) + pow(deltar, 2) + pow(deltag, 2) + pow(deltab, 2));
        return dis;
    }

    double calculateDisDen(int x, int y, Field center, int e) {
        double deltar = rgb[1][x][y] - center.r[e];
        double deltag = rgb[2][x][y] - center.g[e];
        double deltab = rgb[3][x][y] - center.b[e];
        double dis = sqrt(pow(deltar, 2) + pow(deltag, 2) + pow(deltab, 2));
        return dis;
    }

    double calculateCenDis(int x, int y, Field center) {
        double deltadx = ifspatial * ((center.x[x] - center.x[y]) * 255) / dimension.x;
        double deltady = ifspatial * ((center.x[x] - center.y[y]) * 255) / dimension.y;
        double deltar = center.r[x] - center.r[y];
        double deltag = center.g[x] - center.g[y];
        double deltab = center.b[x] - center.b[y];
        double dis = sqrt(pow(deltadx, 2) + pow(deltady, 2) + pow(deltar, 2) + pow(deltag, 2) + pow(deltab, 2));
        return dis;
    }

    double calculateCenDisDen(int x, int y, Field center) {
        double deltar = center.r[x] - center.r[y];
        double deltag = center.g[x] - center.g[y];
        double deltab = center.b[x] - center.b[y];
        double dis = sqrt(pow(deltar, 2) + pow(deltag, 2) + pow(deltab, 2));
        return dis;
    }

    VVV transform(Field center, vector<vector<int>> label) {
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

public:

    VVV getRGBArray() {
//        Field center;
//        for(int i = 0; i < c; i++) {
//            center.x[i] = rand() % dimension.x;
//            center.y[i] = rand() % dimension.y;
//            center.r[i] = rgb[1][center.x[i]][center.y[i]];
//            center.g[i] = rgb[2][center.x[i]][center.y[i]];
//            center.b[i] = rgb[3][center.x[i]][center.y[i]];
//            cout << i << "th center: ";
//            cout << center.x[i] << ' ';
//            cout << center.y[i] << ' ';
//            cout << center.r[i] << ' ';
//            cout << center.g[i] << ' ';
//            cout << center.b[i] << ' ' << endl;
//        }
        for(int i = 0; i < n; i++) {
            Field center;
            for(int j = 0; j < c; j++) {
                center.x[j] = rand() % dimension.x;
                center.y[j] = rand() % dimension.y;
                center.r[j] = rgb[1][center.x[j]][center.y[j]];
                center.g[j] = rgb[2][center.x[j]][center.y[j]];
                center.b[j] = rgb[3][center.x[j]][center.y[j]];
            }

            calculateFit(center);
            if(comb.size() < m) {
                comb.push_back(center);
                sort(comb.begin(), comb.end());
            }
            else {
                if(center.fittness < comb[m - 1].fittness) {
                    comb[m - 1] = center;
                    sort(comb.begin(), comb.end());
                }
            }
//            for(int j = 0; j < comb.size(); j++) {
//                cout << comb[j].fittness << ' ';
//            }
//            cout << endl;
        }
        cout << "start" << endl;
        while(imax--) {
            for(int i = 0; i < elit; i++) {
                for(int j = 0; j < nep; j++) {
                    Field nei;
                    for(int t = 0; t < c; t++) {
                        nei.x[t] = comb[i].x[t] + (rand() % dimension.x) * ngh * (rand() % 2 == 0 ? -1 : 1);
                        if(nei.x[t] >= dimension.x) nei.x[t] = dimension.x - 1;
                        if(nei.x[t] < 0) nei.x[t] = 0;
                        nei.y[t] = comb[i].y[t] + (rand() % dimension.y) * ngh * (rand() % 2 == 0 ? -1 : 1);
                        if(nei.y[t] >= dimension.y) nei.y[t] = dimension.y - 1;
                        if(nei.y[t] < 0) nei.y[t] = 0;
                        nei.r[t] = comb[i].r[t] + (rand() % 256) * ngh * (rand() % 2 == 0 ? -1 : 1);
                        if(nei.r[t] >= 256) nei.r[t] = 255;
                        if(nei.r[t] < 0) nei.r[t] = 0;
                        nei.g[t] = comb[i].g[t] + (rand() % 256) * ngh * (rand() % 2 == 0 ? -1 : 1);
                        if(nei.g[t] >= 256) nei.g[t] = 255;
                        if(nei.g[t] < 0) nei.g[t] = 0;
                        nei.b[t] = comb[i].b[t] + (rand() % 256) * ngh * (rand() % 2 == 0 ? -1 : 1);
                        if(nei.b[t] >= 256) nei.b[t] = 255;
                        if(nei.b[t] < 0) nei.b[t] = 0;
                    }
                    calculateFit(nei);
                    if(nei.fittness < comb[i].fittness) {
                        comb[i] = nei;
                    }
                }
            }
            for(int i = elit; i < m; i++) {
                for(int j = 0; j < nsp; j++) {
                    Field nei;
                    for(int t = 0; t < c; t++) {
                        nei.x[t] = comb[i].x[t] + (rand() % dimension.x) * ngh * (rand() % 2 == 0 ? -1 : 1);
                        if(nei.x[t] >= dimension.x) nei.x[t] = dimension.x;
                        if(nei.x[t] < 0) nei.x[t] = 0;
                        nei.y[t] = comb[i].y[t] + (rand() % dimension.y) * ngh * (rand() % 2 == 0 ? -1 : 1);
                        if(nei.y[t] >= dimension.y) nei.y[t] = dimension.y;
                        if(nei.y[t] < 0) nei.y[t] = 0;
                        nei.r[t] = comb[i].r[t] + (rand() % 256) * ngh * (rand() % 2 == 0 ? -1 : 1);
                        if(nei.r[t] >= 256) nei.r[t] = 255;
                        if(nei.r[t] < 0) nei.r[t] = 0;
                        nei.g[t] = comb[i].g[t] + (rand() % 256) * ngh * (rand() % 2 == 0 ? -1 : 1);
                        if(nei.g[t] >= 256) nei.g[t] = 255;
                        if(nei.g[t] < 0) nei.g[t] = 0;
                        nei.b[t] = comb[i].b[t] + (rand() % 256) * ngh * (rand() % 2 == 0 ? -1 : 1);
                        if(nei.b[t] >= 256) nei.b[t] = 255;
                        if(nei.b[t] < 0) nei.b[t] = 0;
                    }
                    calculateFit(nei);
                    if(nei.fittness < comb[i].fittness)
                        comb[i] = nei;
                }
            }
            sort(comb.begin(), comb.end());
            for(int j = 0; j < comb.size(); j++) {
                cout << comb[j].fittness << ' ';
            }
            cout << endl;
        }
        Je = comb[0].Je;
        dmax = comb[0].dmax;
        dmin = comb[0].dmin;
        fittness = comb[0].fittness;
        VVV newrgb = transform(comb[0], comb[0].label);
        return newrgb;
    }

    vector<vector<int>> getLabel() {
        return comb[0].label;
    }

    double getJe() {
        return Je;
    }
    double getDmax() {
        return dmax;
    }
    double getDmin() {
        return dmin;
    }
    double getFitness() {
        return fittness;
    }


};


#endif // MYBA_H
