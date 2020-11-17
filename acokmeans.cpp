#include "acokmeans.h"

AcoKmeans::AcoKmeans(int k, int m, VVV rgb, Point dimension, double spatial, double w1, double w2, double w3, double Q, double karma, double evap, double alpha, double beta)
{

    srand((unsigned)time(NULL));
    this->k = k;
    this->m = m;
    this->rgb = rgb;
    this->dimension = dimension;
    this->spatial = spatial;
    this->w1 = w1;
    this->w2 = w2;
    this->w3 = w3;
    this->Q = Q;
    this->karma = karma;
    this->evap = evap;
    this->alpha = alpha;
    this->beta = beta;
    pheromone = vector<vector<vector<double>>>(k, vector<vector<double>>(dimension.x, vector<double>(dimension.y)));
    for(int i = 0; i < pheromone.size(); i++) {
        for(int j = 0; j < pheromone[i].size(); j++) {
            for(int e = 0; e < pheromone[i][j].size(); e++) {
                pheromone[i][j][e] = 1;
            }
        }
    }
}

VVV AcoKmeans::getRGBArray() {
    cout << "start" << endl;
    VVV newrgb = rgb;
    vector<vector<int>> bestCenter(k, vector<int>(5));
    vector<vector<int>> bestLabel(dimension.x, vector<int>(dimension.y));
    double bestJe = 0;
    double bestDmax = 0;
    double bestDmin = 0;
    double bestFit = DBL_MAX;
    int exitCri = m;
    bool firstWave = true;
    while(exitCri--) {
        for(int i = 0; i < m; i++) {
            cout << "ant: " << i << endl;
            vector<vector<int>> center(k, vector<int>(5));
            if(!firstWave) {
                center = bestCenter;
            }
            else {
                for(int j = 0; j < k; j++) {
                    center[j][0] = rand() % dimension.x;
                    center[j][1] = rand() % dimension.y;
                    center[j][2] = rgb[1][center[j][0]][center[j][1]];
                    center[j][3] = rgb[2][center[j][0]][center[j][1]];
                    center[j][4] = rgb[3][center[j][0]][center[j][1]];
                }
            }
            vector<vector<int>> label(dimension.x, vector<int>(dimension.y));
            int maxIteration = 1000;
            while(maxIteration--) {
                vector<vector<double>> sump(k, vector<double>(5));
                vector<int> cntp(k);

                vector<int> sumd(k);
                double tJe = 0;
                double tdmax = 0;
                double tdmin = DBL_MAX;

                for(int j = 0; j < k; j++) {
                    double sum1 = 0;
                    for(int e = 0; e < k; e++) {
                        if(j != e) {
                            sum1 += calculateCenDis(center, j, e);
                        }
                    }
                    sum1 = sum1 / (k - 1);
                    tdmin = sum1 < tdmin ? sum1 : tdmin;
                }

                for(int j = 0; j < dimension.x; j++) {
                    for(int e = 0; e < dimension.y; e++) {
                        vector<double> temp(k);
                        double sum = 0;
                        for(int l = 0; l < k; l++) {
                            temp[l] = pow(pheromone[l][j][e], alpha) * pow((karma / distance(center[l], j, e)), beta);
                            sum += temp[l];
                        }
                        double randnum = double(rand() % 100) / 100.0;
                        double p = 0;
                        for(int l = 0; l < k; l++) {
                            p += temp[l] / sum;
                            if(p > randnum) {
                                label[j][e] = l;
                                sump[l][0] += j;
                                sump[l][1] += e;
                                sump[l][2] += rgb[1][j][e];
                                sump[l][3] += rgb[2][j][e];
                                sump[l][4] += rgb[3][j][e];
                                cntp[l]++;
                                sumd[l] += calculateDis(center, j, e, l);
                                break;
                            }
                        }
                    }
                }
                vector<vector<int>> ncenter(k, vector<int>(5));
                for(int j = 0; j < k; j++) {
                    if(cntp[j] > 0) sumd[j] = sumd[j] / cntp[j];
                    tdmax = sumd[j] > tdmax ? sumd[j] : tdmax;
                    tJe += sumd[j];
                    for(int e = 0; e < 5; e++) {
                        if(cntp[j] == 0) {
                            ncenter[j][e] = center[j][e];
                        }
                        else ncenter[j][e] = sump[j][e] / cntp[j];
                    }
                }
                tJe = tJe / k;
                double tfitness = w1 * tJe + w2 * tdmax - (w3 * tdmin);
                if(issimilar(center, ncenter)) {
                    if(tfitness < bestFit) {
                        bestCenter = center;
                        bestLabel = label;
                        bestJe = tJe;
                        bestDmax = tdmax;
                        bestDmin = tdmin;
                        bestFit = tfitness;
    //                    cout << "Je: " << tJe << endl;
    //                    cout << "dmax: " << tdmax << endl;
    //                    cout << "dmin: " << tdmin << endl;
    //                    cout << "fit: " << bestFit << endl;
                    }
                    break;
                }
                else {
                    center = ncenter;
                }
            }

            firstWave = false;
        }
        //*************************
        //update pheromone level
        //*************************
        double minCenDis = DBL_MAX;
        for(int i = 0; i < k; i++) {
            for(int j = i + 1; j < k; j++) {
                double td = calculateCenDis(bestCenter, i, j);
                if(td < minCenDis) minCenDis = td;
            }
        }
        double deltaTao = Q * minCenDis;
        vector<int> cnt(k);
        vector<double> Idis(k);
        vector<double> Edis(k);
        for(int i = 0; i < dimension.x; i++) {
            for(int j = 0; j < dimension.y; j++) {
                double id = 0;
                id += pow(rgb[1][i][j] - bestCenter[bestLabel[i][j]][2], 2);
                id += pow(rgb[2][i][j] - bestCenter[bestLabel[i][j]][3], 2);
                id += pow(rgb[3][i][j] - bestCenter[bestLabel[i][j]][4], 2);
                id = sqrt(id);
                Idis[bestLabel[i][j]] += id;
                double ed = 0;
                ed += pow(i - bestCenter[bestLabel[i][j]][0], 2);
                ed += pow(j - bestCenter[bestLabel[i][j]][1], 2);
                ed = sqrt(ed);
                Edis[bestLabel[i][j]] += ed;
                cnt[bestLabel[i][j]]++;
            }
        }
//        cout << deltaTao / ((Idis[0] / cnt[0]) * (Edis[0] / cnt[0])) << endl;
        for(int i = 0; i < dimension.x; i++) {
            for(int j = 0; j < dimension.y; j++) {
                for(int e = 0; e < k; e++) {
                    pheromone[e][i][j] = pheromone[e][i][j] * (1 - evap);
                    if(bestLabel[i][j] == e) {
                        pheromone[e][i][j] += deltaTao / ((Idis[e] / cnt[e]) * (Edis[e] / cnt[e]));
                    }
                }
            }
        }
    }
    cout << "end" << endl;
    Kmeans kmeans(k, spatial, rgb, dimension, w1, w2, w3);
    label = kmeans.runkMeans(bestCenter);
    Je = kmeans.getJe();
    dmax = kmeans.getDmax();
    dmin = kmeans.getDmin();
    fitness = kmeans.getFittness();
    return transform(bestCenter, label);
}

double AcoKmeans::calculateCenDis(vector<vector<int>> center, int x, int y) {
    double deltadx = spatial * (((center[x][0] - center[y][0]) * 255) / dimension.x);
    double deltady = spatial * (((center[x][1] - center[y][1]) * 255) / dimension.y);
    double deltar = center[x][2] - center[y][2];
    double deltag = center[x][3] - center[y][3];
    double deltab = center[x][4] - center[y][4];
    double dis = sqrt(pow(deltadx, 2) + pow(deltady, 2) + pow(deltar, 2) + pow(deltag, 2) + pow(deltab, 2));
    return dis;
}

double AcoKmeans::calculateDis(vector<vector<int>> center, int x, int y, int e) {
    double deltadx = spatial * (((x - center[e][0]) * 255) / dimension.x);
    double deltady = spatial * (((y - center[e][1]) * 255) / dimension.y);
    double deltar = rgb[1][x][y] - center[e][2];
    double deltag = rgb[2][x][y] - center[e][3];
    double deltab = rgb[3][x][y] - center[e][4];
    double dis = sqrt(pow(deltadx, 2) + pow(deltady, 2) + pow(deltar, 2) + pow(deltag, 2) + pow(deltab, 2));
    return dis;
}

bool AcoKmeans::issimilar(vector<vector<int>> center, vector<vector<int>> ncenter) {
    for(int i = 0 ; i < k; i++) {
        for(int j = 0; j < 5; j++) {
            int temp = center[i][j] - ncenter[i][j];
            if(temp > 3 || temp < -3)
                return false;
        }
    }
    return true;
}

double AcoKmeans::distance(vector<int> center, int x, int y) {
    double intensity = 0;
    for(int i = 1; i < 4; i++) {
        intensity += pow(rgb[i][x][y] - center[i + 1], 2);
    }
    double euclidean = 0;
    euclidean = pow(x - center[0], 2) + pow(y - center[1], 2);
    return sqrt(intensity) * sqrt(euclidean);
}

VVV AcoKmeans::transform(vector<vector<int>> center, vector<vector<int>> label) {
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

double AcoKmeans::getFitness(){
    return fitness;
}
double AcoKmeans::getJe(){
    return Je;
}
double AcoKmeans::getDmax(){
    return dmax;
}
double AcoKmeans::getDmin(){
    return dmin;
}
