#include "psokmeans.h"

psokmeans::psokmeans(int C, int S, int tmax, VVV rgb, Point dimension, double spatial, double w1, double w2, double w3, double w, double c1, double c2)
{
    srand((unsigned)time(NULL));
    this->C = C;
    this->S = S;
    this->tmax = tmax;
    this->rgb = rgb;
    this->dimension = dimension;
    this->spatial = spatial;
    this->w1 = w1;
    this->w2 = w2;
    this->w3 = w3;
    this->w = w;
    this->c1 = c1;
    this->c2 = c2;
}

VVV psokmeans::getRGBArray() {
    vector<vector<int>> label;
    label.resize(dimension.x);
    for(int i = 0; i < dimension.x; i++)
        label[i].resize(dimension.y);

    VVV particals;
    VVV velo;
    velo.resize(S);
    for(int i = 0; i < S; i++) {
        velo[i].resize(C);
        for(int j = 0; j < C; j++) {
            velo[i][j].resize(5);
            for(int t = 0; t < 5; t++)
                velo[i][j][t] = 0;
        }
    }

    VVV pb;
    vector<double> fit;
    fit.resize(S);

    pb.resize(S);
    for(int i = 0; i < S; i++) {
        pb[i].resize(C);
        for(int j = 0; j < C; j++) {
            pb[i][j].resize(5);
        }
    }

    vector<vector<int>> gbest;
    gbest.resize(C);
    for(int i = 0; i < C; i++) {
        gbest[i].resize(5);
    }
    double gbfit = DBL_MAX;

    particals.resize(S);
    for(int i = 0; i < S; i++) {
        particals[i].resize(C);
        for(int j = 0; j < C; j++) {
            particals[i][j].resize(5);
        }
    }

    for(int i = 0; i < S; i++) {
        Kmeans kmeans(C, spatial, rgb, dimension, w1, w2, w3);
        vector<vector<int>> tcen;
        tcen.resize(C);
        for(int j = 0; j < C; j++) {
            tcen[j].push_back(rand() % dimension.x);
            tcen[j].push_back(rand() % dimension.y);
            tcen[j].push_back(rgb[1][tcen[j][0]][tcen[j][1]]);
            tcen[j].push_back(rgb[2][tcen[j][0]][tcen[j][1]]);
            tcen[j].push_back(rgb[3][tcen[j][0]][tcen[j][1]]);
        }
        kmeans.runkMeans(tcen);
        tcen = kmeans.getCenter();
        pb[i] = tcen;
        fit[i] = kmeans.getFittness();
        if(fit[i] < gbfit) {
            gbfit = fit[i];
            gbest = tcen;
            Je = kmeans.getJe();
            dmax = kmeans.getDmax();
            dmin = kmeans.getDmin();
            fitness = gbfit;
            label = kmeans.assignPixels();
        }
        particals[i] = tcen;
    }
    cout << tmax << endl;
    while(tmax--) {
        for(int i = 0; i < S; i++) {
            cout << i << endl;
            cout << Je << " " << dmax << " " << dmin << ' ' << fitness << endl;
            Kmeans kmeans(C, spatial, rgb, dimension, w1, w2, w3);
            for(int j = 0; j < C; j++) {
                for(int t = 0; t < 5; t++) {
                    particals[i][j][t] += velo[i][j][t];
                    int tv = velo[i][j][t];
                    velo[i][j][t] = w * tv + c1 * ((rand() % 100) / (double)101) * (pb[i][j][t] - particals[i][j][t])
                            + c2 * ((rand() % 100) / (double)101) * (gbest[j][t] - particals[i][j][t]);
                    cout << velo[i][j][t] << ' ';
                }
                cout << endl;
            }
            kmeans.setcenter(particals[i]);
            kmeans.assignPixels();
            particals[i] = kmeans.getCenter();
            double tfit = kmeans.getFittness();
            if(tfit < fit[i]) {
                pb[i] = particals[i];
                fit[i] = tfit;
            }
            if(tfit < gbfit) {
                gbest = particals[i];
                gbfit = tfit;
                Je = kmeans.getJe();
                dmax = kmeans.getDmax();
                dmin = kmeans.getDmin();
                fitness = gbfit;
                cout << fitness << endl;
                label = kmeans.assignPixels();
            }
            w = w - ((w - 0.49) / tmax);
        }
    }

    return transform(gbest, label);
}


VVV psokmeans::transform(vector<vector<int>> center, vector<vector<int>> label) {
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


double psokmeans::getFitness(){
    return fitness;
}
double psokmeans::getJe(){
    return Je;
}
double psokmeans::getDmax(){
    return dmax;
}
double psokmeans::getDmin(){
    return dmin;
}
