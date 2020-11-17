#include "kmeans.h"

Kmeans::Kmeans(int k, double ifspatial, VVV rgb, Point dimension, double w1, double w2, double w3)
{
    this->k = k;
    this->ifspatial = ifspatial;
    this->rgb = rgb;
    this->dimension = dimension;
    this->w1 = w1;
    this->w2 = w2;
    this->w3 = w3;

    Je = 0;
    dmax = 0;
    dmin = 0;

    center.resize(k);
    for(int i = 0; i < k; i++)
        center[i].resize(5);
    ncenter = center;
    srand((unsigned)time(NULL));
}

VVV Kmeans::getRGBArray() {
    VVV newrgb = rgb;
    for(int i = 0; i < k; i++) {
        center[i][0] = rand() % dimension.x;
        center[i][1] = rand() % dimension.y;
        center[i][2] = rgb[1][center[i][0]][center[i][1]];
        center[i][3] = rgb[2][center[i][0]][center[i][1]];
        center[i][4] = rgb[3][center[i][0]][center[i][1]];
    }
    int count = 1000;
    while(count--) {
//        cout << "center: ";
//        for(int i = 0; i < k; i++) {
//            cout << "center" << i << "= ";
//            for(int j = 2; j < 5; j++) {
//                cout << center[i][j] << ' ';
//            }
//            cout << endl;
//        }
        Label = assignPixels();
        if(issimilar()) {
//            cout << "converge" << endl;
            newrgb = setnewRGBArray(Label);
            break;
        }
        center = ncenter;
    }
    return newrgb;
}

vector<vector<int>> Kmeans::runkMeans(vector<vector<int>> centerIn) {
    center = centerIn;
    int count = 1000;
    vector<vector<int>> label;
    while(count--) {
//        cout << "center: ";
//        for(int i = 0; i < k; i++) {
//            cout << "center" << i << "= ";
//            for(int j = 2; j < 5; j++) {
//                cout << center[i][j] << ' ';
//            }
//            cout << endl;
//        }
        label = assignPixels();
        if(issimilar()) {
//            cout << "converge" << endl;
            return label;
        }
        center = ncenter;
    }
    return label;
}

vector<vector<int>> Kmeans::assignPixels() {

    vector<vector<int>> label;
    label.resize(dimension.x);
    for(int i = 0; i < dimension.x; i++)
        label[i].resize(dimension.y);

    int sumx[100];
    memset(sumx, 0, sizeof(int) * 100);
    int sumy[100];
    memset(sumy, 0, sizeof(int) * 100);
    int sumr[100];
    memset(sumr, 0, sizeof(int) * 100);
    int sumg[100];
    memset(sumg, 0, sizeof(int) * 100);
    int sumb[100];
    memset(sumb, 0, sizeof(int) * 100);
    int cnt[100];
    memset(cnt, 0, sizeof(int) * 100);

    double sumd[100];
    memset(sumd, 0, sizeof(double)*100);
    Je = 0;
    dmax = 0;
    dmin = DBL_MAX;



    for(int i = 0; i < k; i++) {
        double sum1 = 0;
        for(int j = 0; j < k; j++) {
            if(i != j) {
                sum1 += calculateCenDis(i, j);
            }
        }
        sum1 = sum1 / (k - 1);
        dmin = sum1 < dmin ? sum1 : dmin;
    }

    for(int i = 0; i < dimension.x; i++) {
        for(int j =0 ; j < dimension.y; j++) {
            double mindis = 9999999;
            for(int e = 0; e < k; e++) {
                double dis = 0;
                dis = calculateDis(i, j, e);
                if(dis < mindis) {
                    mindis = dis;
                    label[i][j] = e;
                }
            }
            sumd[label[i][j]] += mindis;
            sumx[label[i][j]] += i;
            sumy[label[i][j]] += j;
            sumr[label[i][j]] += rgb[1][i][j];
            sumg[label[i][j]] += rgb[2][i][j];
            sumb[label[i][j]] += rgb[3][i][j];
            cnt[label[i][j]]++;
        }
    }
    for(int i = 0; i < k; i++) {
        if(cnt[i] > 0) {
            sumd[i] = sumd[i] / cnt[i];
            dmax = sumd[i] > dmax ? sumd[i] : dmax;
            Je += sumd[i];
            ncenter[i][0] = sumx[i] / cnt[i];
            ncenter[i][1] = sumy[i] / cnt[i];
            ncenter[i][2] = sumr[i] / cnt[i];
            ncenter[i][3] = sumg[i] / cnt[i];
            ncenter[i][4] = sumb[i] / cnt[i];
        }
    }
    Je = Je / k;
//    double temp = pow(255, 2);
//    double maxdis = sqrt(temp * 5);
    fittness = w1 * Je + w2 * dmax - (w3 * dmin);
//    fittness = Je;
    return label;
}

double Kmeans::calculateCenDis(int x, int y) {
    double deltadx = ifspatial * (((center[x][0] - center[y][0]) * 255) / dimension.x);
    double deltady = ifspatial * (((center[x][1] - center[y][1]) * 255) / dimension.y);
    double deltar = center[x][2] - center[y][2];
    double deltag = center[x][3] - center[y][3];
    double deltab = center[x][4] - center[y][4];
    double dis = sqrt(pow(deltadx, 2) + pow(deltady, 2) + pow(deltar, 2) + pow(deltag, 2) + pow(deltab, 2));
//    double r1 = center[x][2];
//    double g1 = center[x][3];
//    double b1 = center[x][4];
//    double r2 = center[y][2];
//    double g2 = center[y][3];
//    double b2 = center[y][4];
//    double min1 = min(r1, g1) < min(g1, b1) ? min(r1, g1) : min(g1, b1);
//    double min2 = min(r2, g2) < min(g2, b2) ? min(r2, g2) : min(g2, b2);
//    r1 = r1 - min1;
//    g1 = g1 - min1;
//    b1 = b1 - min1;
//    r2 = r2 - min2;
//    g2 = g2 - min2;
//    b2 = b2 - min2;
//    double dis1 = sqrt(pow((r1 - r2), 2) + pow((g1 - g2), 2) + pow((b1 - b2), 2) + pow(deltadx, 2) + pow(deltady, 2));
    return dis;
}

double Kmeans::calculateDis(int x, int y, int e) {
    double deltadx = ifspatial * (((x - center[e][0]) * 255) / dimension.x);
    double deltady = ifspatial * (((y - center[e][1]) * 255) / dimension.y);
    double deltar = rgb[1][x][y] - center[e][2];
    double deltag = rgb[2][x][y] - center[e][3];
    double deltab = rgb[3][x][y] - center[e][4];
    double dis = sqrt(pow(deltadx, 2) + pow(deltady, 2) + pow(deltar, 2) + pow(deltag, 2) + pow(deltab, 2));
//    double r1 = rgb[1][x][y];
//    double g1 = rgb[2][x][y];
//    double b1 = rgb[3][x][y];
//    double r2 = center[e][2];
//    double g2 = center[e][3];
//    double b2 = center[e][4];
//    double min1 = min(r1, g1) < min(g1, b1) ? min(r1, g1) : min(g1, b1);
//    double min2 = min(r2, g2) < min(g2, b2) ? min(r2, g2) : min(g2, b2);
//    r1 = r1 - min1;
//    g1 = g1 - min1;
//    b1 = b1 - min1;
//    r2 = r2 - min2;
//    g2 = g2 - min2;
//    b2 = b2 - min2;
//    double dis1 = sqrt(pow((r1 - r2), 2) + pow((g1 - g2), 2) + pow((b1 - b2), 2) + pow(deltadx, 2) + pow(deltady, 2));
    return dis;
}

double Kmeans::calculateDisDen(int x, int y, int e) {
    double deltar = rgb[1][x][y] - center[e][2];
    double deltag = rgb[2][x][y] - center[e][3];
    double deltab = rgb[3][x][y] - center[e][4];
    double dis = sqrt(pow(deltar, 2) + pow(deltag, 2) + pow(deltab, 2));
    return dis;
}

bool Kmeans::issimilar() {
    for(int i = 0 ; i < k; i++) {
        for(int j = 0; j < 5; j++) {
            int temp = center[i][j] - ncenter[i][j];
            if(temp > 3 || temp < -3)
                return false;
        }
    }
    return true;
}

VVV Kmeans::setnewRGBArray(vector<vector<int>> label){
    VVV newrgb = rgb;
    for(int i = 0; i < dimension.x; i++) {
        for(int j = 0; j < dimension.y; j++) {
            newrgb[1][i][j] = ncenter[label[i][j]][2];
            newrgb[2][i][j] = ncenter[label[i][j]][3];
            newrgb[3][i][j] = ncenter[label[i][j]][4];
        }
    }
    return newrgb;
}

void Kmeans::setcenter(vector<vector<int>> cent) {
    center = cent;
}

vector<vector<int>> Kmeans::getLabel() {
    return Label;
}

double Kmeans::getJe() {
    return Je;
}
double Kmeans::getDmax() {
    return dmax;
}
double Kmeans::getDmin() {
    return dmin;
}
double Kmeans::getFittness() {
    return fittness;
}
vector<vector<int>> Kmeans::getCenter() {
    return center;
}
