#include "tmeans.h"

tmeans::tmeans(int k, double T, int width, int height, VVV rgb, double spatial, double w1, double w2, double w3) {
    this->k = k;
    this->t = T;
    this->width = width;
    this->height = height;
    this->rgb = rgb;
    this->spatial = spatial;
    this->w1 = w1;
    this->w2 = w2;
    this->w3 = w3;
    dimension = rgb.size();
    accuracy = 0;
    Je = 0;
    dmax = 0;
    dmin = 0;
    fitness = 0;
    label.resize(width);
    for(int i = 0; i < width; i++)
        label[i].resize(height);
    for(int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
            label[i][j] = -1;
        }
    }
}


vector<vector<int>> tmeans::getLabel() {
    vector<int> center;
    vector<int> ncenter;
    vector<int> maxfeature;
    maxfeature.resize(dimension + 2);
    int maxfvalue = 0;
    for(int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
            int currentvalue = 0;
            for(int e = 0; e < dimension; e++) {
                currentvalue += rgb[e][i][j];
            }
            if(currentvalue > maxfvalue) {
                maxfvalue = currentvalue;
                maxfeature[0] = i;
                maxfeature[1] = j;
                for(int e = 0; e < dimension; e++) {
                    maxfeature[e + 2] = rgb[e][i][j];
                }
            }
        }
    }
    for(int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
            vector<int> currentPixel;
            currentPixel.resize(dimension + 2);
            currentPixel[0] = i;
            currentPixel[1] = j;
            for(int e = 0; e < dimension; e++) {
                currentPixel[e + 2] = rgb[e][i][j];
            }
            double dis = calcuateDis(currentPixel, maxfeature);
            if(dis < t) {
                label[i][j] = 0;
            }
        }
    }
    center.assign(maxfeature.begin(), maxfeature.end());
    for(int i =0; i < center.size(); i++) {
        cout << center[i] << ' ';
    }
    cout << endl;

    ncenter = calNewCenter(center);

    for(int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
            if(label[i][j] == -1) {
                label[i][j] = 1;
            }
        }
    }

    return label;
}

vector<int> tmeans::calNewCenter(vector<int> center) {
    vector<int> ncenter;
    int  count = 0;

    ncenter.resize(center.size());

    for(int i = 0; i < width; i++) {
        for(int j = 0; j < height; j++) {
            if(label[i][j] == 0) {
                for(int e = 0; e < center.size(); e++) {
                    ncenter[e] += center[e];

                }
                count ++;
            }
        }
    }
    for(int i = 0; i < center.size(); i++) {
        ncenter[i] = ncenter[i] / count;
    }

    return ncenter;
}

bool tmeans::terminateCrit(vector<int> center, vector<int> ncenter) {

    for(int i = 0; i < center.size(); i++) {

    }

    return true;
}

double tmeans::getAccuracy() {
    return accuracy;
}


double tmeans::getJe() {
    return Je;
}


double tmeans::getDmax() {
    return dmax;
}


double tmeans::getDmin() {
    return dmin;
}

double tmeans::getFitness() {
    return fitness;
}


double tmeans::calcuateDis(vector<int> a, vector<int> b) {
    double dis = 0;
    dis += spatial * pow(((a[0] - b[0]) * 255 )/ width, 2);
    dis += spatial * pow(((a[1] - b[1]) * 255 )/ height, 2);
    for(int i = 2; i < dimension; i++) {
        dis += pow((a[i] - b[i]), 2);
    }
    dis = sqrt(dis);
    return dis;
}
