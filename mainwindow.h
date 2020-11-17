#ifndef MAINWINDOW_H
#define MAINWINDOW_H


#include <QMainWindow>
#include<QtGui>
#include<QLabel>
#include<opencv2/opencv.hpp>
#include<opencv2/core/core.hpp>
#include<opencv2/highgui/highgui.hpp>
#include<opencv2/imgproc/imgproc.hpp>
#include<opencv2/imgcodecs/imgcodecs.hpp>
#include<opencv2/ml/ml.hpp>
#include<QFileDialog>
#include<QAction>
#include<QMessageBox>
#include<QListWidgetItem>
#include<vector>
#include<QScrollArea>
#include<QGridLayout>
#include<iostream>
#include<mat.h>
#include<math.h>


#define VVV vector<vector<vector<int>>>

using namespace cv;
using namespace std;

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:

    void on_actionopen_triggered();

    void on_tabWidget_tabCloseRequested(int index);

    void on_actionGray_scale_triggered();

    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_actionRun_triggered();

    void on_actionZoom_in_triggered();

    void on_actionZoom_out_triggered();

    void on_actionsave_triggered();

    void on_actionedge_dectect_triggered();

    void on_actionMirror_triggered();

    void on_actionBA_triggered();

    void on_actionK_means_triggered();

    void on_actionPBA_triggered();

    void on_actionCV_K_means_triggered();

    void on_actionPBA_K_means_triggered();

    void on_actionAverage_triggered();

    void on_actionClear_triggered();

    void on_actionPSO_K_means_triggered();

    void on_actionGT_PBA_K_means_triggered();

    void on_actionCV_enhancement_triggered();

    void on_actionCV_enhance_histo_triggered();

    void on_actionCV_enhance_log_triggered();

    void on_actionGT_PBA_triggered();

    void on_actionGT_BA_triggered();

    void on_actionGT_CV_K_means_triggered();

    void on_actionGT_K_means_triggered();

    void on_actionSU_PBA_K_means_triggered();

    void on_actionMyEnhancement_triggered();

    void on_actionShow_bands_triggered();

    void on_actionReduce_Shadow_triggered();

    void on_actionForm_image_triggered();

    void on_actionT_means_triggered();

    void on_actionReduce_class_triggered();

    void on_actionOpen_hyperspectral_image_triggered();

    void on_actionACO_K_means_triggered();

    void on_actionGA_K_means_triggered();

    void on_actionSA_K_means_triggered();

protected:

    Point getImageDimension(QImage image);

    vector<vector<vector<int>>> imageToRGBArray(QImage imageIn);

    QImage RGBArrayToimage(vector<vector<vector<int>>> imageOut, QImage imageIn);

    QImage RGBArrayToimage_1(vector<vector<vector<int>>> RGBArray, Point d);

    VVV setRGBArrayFromLabel(vector<vector<int>> label);

private:

    QString getFileName();

    double getAccuracy(int c, Point d, vector<vector<int>> gt, vector<vector<int>> re);

    vector<vector<int>> getGtLabel(int c, QImage gt);

    void showAna();

    void showclass(int c, Point d, vector<vector<int>> re, vector<vector<int>> gt, vector<int> acc);

    void showImage(QImage *image);

    void grayScale();

    void edgeDectect();

    void mirror();

    void zoom(double zoomScale);

    void BA();

    void kMeans();

    void tMeans();

    void CV_kMeans();

    void CV_enhancement();

    void CV_enhance_histo();

    void CV_enhance_log();

    void myEnhance();

    void PBA();

    void PBAkMeans();

    void ACOkMeans();

    void GAkMeans();

    void SAkMeans();

    void SU_PBAkMeans();

    void GT_Kmeans();

    void GT_CV_Kmeans();

    void GTPBAkMeans();

    void GTPBA();

    void GTBA();

    void PSOkMeans();

    void setAnaTable(double Je, double dmax, double dmin, double fittness);

    void addAccuracyToAna(double Je, double dmax, double dmin, double fittness, double accruacy);

private:
    QImage gt;
    boolean ifgt = true;
    Ui::MainWindow *ui;
    int columnsNum = 3;
    int currentProcess = 0;
    double zoomInRate = 0.9;
    double zoomOutRate = 1.1;
    vector<QString> listItems;
    vector<int> tabImageNum;
    vector<QGridLayout*> la;
    vector<vector<QLabel*>> tabLabel;
};

#endif // MAINWINDOW_H
