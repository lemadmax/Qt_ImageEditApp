#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "myba.h"
#include "kmeans.h"
#include "mypba.h"
#include "pbakmeans.h"
#include "psokmeans.h"
#include "suedpbakmeans.h"
#include "tmeans.h"
#include "acokmeans.h"
#include "gakmeans.h"
#include "sakmeans.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    setWindowTitle("ImagePimp");
    ui->tabWidget->removeTab(0);
    ui->tabWidget->removeTab(0);
}

MainWindow::~MainWindow()
{
    delete ui;
}

//function that open an image
void MainWindow::on_actionopen_triggered()
{
    //name of path
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("select image"),
                                                    "",
                                                    tr("Images (*.png *.bmp *.jpg *.tif *.GIF)"));
    if(!fileName.isEmpty()) {
        QImage* image = new QImage;
        if(!(image->load(fileName))) {
            QMessageBox::information(this, "warning",
                                     "can't open image");
            return;
        }
        //get name of file
        /****************************************/
        QString name; //name of file
        for(int i = fileName.length(); i >= 0; i--) {
            if(fileName[i] == '/') break;
            name.insert(0, fileName[i]);
        }
        /****************************************/

        //the number of images in a tab
        //i is the row of listwidget
        tabImageNum.push_back(1);

        //add an index to the same image of different tab
        /****************************************/
        int cnt = 0;
        for(int i = 0; i < listItems.size(); i++) {
            if(fileName == listItems[i]) cnt++;
        }
        if(cnt) {
            QString temp = '(' + QString::number(cnt) + ')';
            int i = 0;
            for(i = name.length(); i >= 0; i--) {
                if(name[i] == '.') {
                    break;
                }
            }
            for(int j = 0; j < temp.length(); j++) {
                name.insert(i, temp[j]);
                i++;
            }
            for(int e = 0; e < ui->listWidget->count(); e++) {
                if(name == ui->listWidget->item(e)->text()) {
                    QString temp = '(' + QString::number(cnt) + ')';
                    int i = 0;
                    for(i = name.length(); i >= 0; i--) {
                        if(name[i] == '.') {
                            break;
                        }
                    }
                    for(int j = 0; j < temp.length(); j++) {
                        name.insert(i, temp[j]);
                        i++;
                    }
                    e = 0;
                }
            }
        }
        /*****************************************/

        //store the path of an image
        listItems.push_back(fileName);
        //add image name to listwidget
        ui->listWidget->addItem(name);

        //show image in a tab using scrollarea
        /****************************************/
        QScrollArea *scroll = new QScrollArea;
        scroll->setWidgetResizable(true);
        QWidget *wid = new QWidget;
        QGridLayout *layout = new QGridLayout;

        wid->setLayout(layout);
        scroll->setWidget(wid);

        QLabel *label = new QLabel(wid);
        label->setPixmap(QPixmap::fromImage(*image));
        label->setAlignment(Qt::AlignLeft);

        layout->addWidget(label);
        ui->tabWidget->addTab(scroll, name);
        ui->tabWidget->setCurrentWidget(scroll);
        /****************************************/

        //give each tab a layout
        la.push_back(layout);
        //save each label of a tab
        vector<QLabel*> v;
        v.push_back(label);
        tabLabel.push_back(v);
    }
}

//function that close an image by closing a tab
void MainWindow::on_tabWidget_tabCloseRequested(int index)
{
    QString name = ui->tabWidget->tabText(index);

    //find the name in listwidget and delete it
    /****************************************/
    for(int i = 0; i < listItems.size(); i++) {
        QListWidgetItem *item = new QListWidgetItem;
        item = ui->listWidget->item(i);
        if(item->text() == ui->tabWidget->tabText(index)){
            ui->listWidget->takeItem(i);
            //delete related arraies
            swap(tabImageNum[i], tabImageNum[tabImageNum.size() - 1]);
            swap(listItems[i], listItems[listItems.size() - 1]);
            swap(tabLabel[i], tabLabel[listItems.size() - 1]);
            swap(la[i], la[listItems.size() - 1]);
            la.pop_back();
            tabLabel.pop_back();
            listItems.pop_back();
            tabImageNum.pop_back();
            break;
        }
    }
    ui->tabWidget->removeTab(index);
    /****************************************/
}

//function that select grayscale as process method
void MainWindow::on_actionGray_scale_triggered()
{
    currentProcess = 1;

    //set table value
    /****************************************/
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setCurrentCell(0, 0);
    ui->label_3->setText("Grayscale");
    QTableWidgetItem *item = new QTableWidgetItem("No Parameter");
    ui->tableWidget->setItem(0, 0, item);
    //set table non editable
    for(int i = 0; i < ui->tableWidget->rowCount(); i++) {
        ui->tableWidget->item(i, 0)->setFlags(Qt::NoItemFlags);
    }
    /****************************************/
}

void MainWindow::on_actionedge_dectect_triggered()
{
    currentProcess = 2;
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setCurrentCell(0, 0);
    ui->label_3->setText("Edge dectect");
    QTableWidgetItem *item = new QTableWidgetItem("No Parameter");
    ui->tableWidget->setItem(0, 0, item);
    for(int i = 0; i < ui->tableWidget->rowCount(); i++) {
        ui->tableWidget->item(i, 0)->setFlags(Qt::NoItemFlags);
    }
}

void MainWindow::on_actionMirror_triggered()
{
    currentProcess = 3;
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setCurrentCell(0, 0);
    ui->label_3->setText("Mirror");
    QTableWidgetItem *item = new QTableWidgetItem("No Parameter");
    ui->tableWidget->setItem(0, 0, item);
    for(int i = 0; i < ui->tableWidget->rowCount(); i++) {
        ui->tableWidget->item(i, 0)->setFlags(Qt::NoItemFlags);
    }
}


void MainWindow::on_actionK_means_triggered()
{
    currentProcess = 5;
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(5);
    ui->tableWidget->setColumnCount(2);
    ui->label_3->setText("K-means");
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem("K"));
    ui->tableWidget->setItem(1, 0, new QTableWidgetItem("spatial(1/0)"));
    ui->tableWidget->setItem(2, 0, new QTableWidgetItem("w1"));
    ui->tableWidget->setItem(3, 0, new QTableWidgetItem("w2"));
    ui->tableWidget->setItem(4, 0, new QTableWidgetItem("w3"));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem("3"));
    ui->tableWidget->setItem(1, 1, new QTableWidgetItem("0"));
    ui->tableWidget->setItem(2, 1, new QTableWidgetItem("1"));
    ui->tableWidget->setItem(3, 1, new QTableWidgetItem("1.5"));
    ui->tableWidget->setItem(4, 1, new QTableWidgetItem("0.2"));
    for(int i = 0; i < ui->tableWidget->rowCount(); i++) {
        ui->tableWidget->item(i, 0)->setFlags(Qt::NoItemFlags);
    }
}

void MainWindow::on_actionT_means_triggered()
{
    currentProcess = 20;
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(6);
    ui->tableWidget->setColumnCount(2);
    ui->label_3->setText("T-means");
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem("K"));
    ui->tableWidget->setItem(1, 0, new QTableWidgetItem("T"));
    ui->tableWidget->setItem(2, 0, new QTableWidgetItem("spatial(1/0)"));
    ui->tableWidget->setItem(3, 0, new QTableWidgetItem("w1"));
    ui->tableWidget->setItem(4, 0, new QTableWidgetItem("w2"));
    ui->tableWidget->setItem(5, 0, new QTableWidgetItem("w3"));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem("3"));
    ui->tableWidget->setItem(1, 1, new QTableWidgetItem("60"));
    ui->tableWidget->setItem(2, 1, new QTableWidgetItem("0"));
    ui->tableWidget->setItem(3, 1, new QTableWidgetItem("1"));
    ui->tableWidget->setItem(4, 1, new QTableWidgetItem("1.5"));
    ui->tableWidget->setItem(5, 1, new QTableWidgetItem("0.2"));
    for(int i = 0; i < ui->tableWidget->rowCount(); i++) {
        ui->tableWidget->item(i, 0)->setFlags(Qt::NoItemFlags);
    }
}

void MainWindow::on_actionGT_K_means_triggered()
{
    currentProcess = 17;
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(5);
    ui->tableWidget->setColumnCount(2);
    ui->label_3->setText("GT_K-means");
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem("K"));
    ui->tableWidget->setItem(1, 0, new QTableWidgetItem("spatial(1/0)"));
    ui->tableWidget->setItem(2, 0, new QTableWidgetItem("w1"));
    ui->tableWidget->setItem(3, 0, new QTableWidgetItem("w2"));
    ui->tableWidget->setItem(4, 0, new QTableWidgetItem("w3"));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem("3"));
    ui->tableWidget->setItem(1, 1, new QTableWidgetItem("0"));
    ui->tableWidget->setItem(2, 1, new QTableWidgetItem("1"));
    ui->tableWidget->setItem(3, 1, new QTableWidgetItem("1.5"));
    ui->tableWidget->setItem(4, 1, new QTableWidgetItem("0.2"));
    for(int i = 0; i < ui->tableWidget->rowCount(); i++) {
        ui->tableWidget->item(i, 0)->setFlags(Qt::NoItemFlags);
    }
}

void MainWindow::on_actionCV_K_means_triggered()
{
    currentProcess = 7;
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(5);
    ui->tableWidget->setColumnCount(2);
    ui->label_3->setText("CV_K-means");
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem("K"));
    ui->tableWidget->setItem(1, 0, new QTableWidgetItem("spatial(1/0)"));
    ui->tableWidget->setItem(2, 0, new QTableWidgetItem("w1"));
    ui->tableWidget->setItem(3, 0, new QTableWidgetItem("w2"));
    ui->tableWidget->setItem(4, 0, new QTableWidgetItem("w3"));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem("3"));
    ui->tableWidget->setItem(1, 1, new QTableWidgetItem("0"));
    ui->tableWidget->setItem(2, 1, new QTableWidgetItem("1"));
    ui->tableWidget->setItem(3, 1, new QTableWidgetItem("1.5"));
    ui->tableWidget->setItem(4, 1, new QTableWidgetItem("0.2"));
    for(int i = 0; i < ui->tableWidget->rowCount(); i++) {
        ui->tableWidget->item(i, 0)->setFlags(Qt::NoItemFlags);
    }
}

void MainWindow::on_actionGT_CV_K_means_triggered()
{
    currentProcess = 16;
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(5);
    ui->tableWidget->setColumnCount(2);
    ui->label_3->setText("GT_CV_K-means");
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem("K"));
    ui->tableWidget->setItem(1, 0, new QTableWidgetItem("spatial(1/0)"));
    ui->tableWidget->setItem(2, 0, new QTableWidgetItem("w1"));
    ui->tableWidget->setItem(3, 0, new QTableWidgetItem("w2"));
    ui->tableWidget->setItem(4, 0, new QTableWidgetItem("w3"));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem("3"));
    ui->tableWidget->setItem(1, 1, new QTableWidgetItem("0"));
    ui->tableWidget->setItem(2, 1, new QTableWidgetItem("1"));
    ui->tableWidget->setItem(3, 1, new QTableWidgetItem("1.5"));
    ui->tableWidget->setItem(4, 1, new QTableWidgetItem("0.2"));
    for(int i = 0; i < ui->tableWidget->rowCount(); i++) {
        ui->tableWidget->item(i, 0)->setFlags(Qt::NoItemFlags);
    }
}


void MainWindow::on_actionCV_enhancement_triggered()
{
    currentProcess = 11;
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setCurrentCell(0, 0);
    ui->label_3->setText("enhencement");
    QTableWidgetItem *item = new QTableWidgetItem("No Parameter");
    ui->tableWidget->setItem(0, 0, item);
    for(int i = 0; i < ui->tableWidget->rowCount(); i++) {
        ui->tableWidget->item(i, 0)->setFlags(Qt::NoItemFlags);
    }
}

void MainWindow::on_actionMyEnhancement_triggered()
{
    currentProcess = 19;
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setCurrentCell(0, 0);
    ui->label_3->setText("My enhencement");
    QTableWidgetItem *item = new QTableWidgetItem("No Parameter");
    ui->tableWidget->setItem(0, 0, item);
    for(int i = 0; i < ui->tableWidget->rowCount(); i++) {
        ui->tableWidget->item(i, 0)->setFlags(Qt::NoItemFlags);
    }
}

void MainWindow::on_actionCV_enhance_histo_triggered()
{
    currentProcess = 12;
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setCurrentCell(0, 0);
    ui->label_3->setText("histo_enhencement");
    QTableWidgetItem *item = new QTableWidgetItem("No Parameter");
    ui->tableWidget->setItem(0, 0, item);
    for(int i = 0; i < ui->tableWidget->rowCount(); i++) {
        ui->tableWidget->item(i, 0)->setFlags(Qt::NoItemFlags);
    }
}

void MainWindow::on_actionCV_enhance_log_triggered()
{
    currentProcess = 13;
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(1);
    ui->tableWidget->setColumnCount(1);
    ui->tableWidget->setCurrentCell(0, 0);
    ui->label_3->setText("log_enhencement");
    QTableWidgetItem *item = new QTableWidgetItem("No Parameter");
    ui->tableWidget->setItem(0, 0, item);
    for(int i = 0; i < ui->tableWidget->rowCount(); i++) {
        ui->tableWidget->item(i, 0)->setFlags(Qt::NoItemFlags);
    }
}


void MainWindow::on_actionBA_triggered()
{
    currentProcess = 4;
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(12);
    ui->tableWidget->setColumnCount(2);
    ui->label_3->setText("BA");
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem("Centers(C)"));
    ui->tableWidget->setItem(1, 0, new QTableWidgetItem("Scouts(n)"));
    ui->tableWidget->setItem(2, 0, new QTableWidgetItem("Sites(m)"));
    ui->tableWidget->setItem(3, 0, new QTableWidgetItem("BestSites(e)"));
    ui->tableWidget->setItem(4, 0, new QTableWidgetItem("BeeForBest(nep)"));
    ui->tableWidget->setItem(5, 0, new QTableWidgetItem("BeeForOther(nsp)"));
    ui->tableWidget->setItem(6, 0, new QTableWidgetItem("Neighborhood(ngh)"));
    ui->tableWidget->setItem(7, 0, new QTableWidgetItem("imax"));
    ui->tableWidget->setItem(8, 0, new QTableWidgetItem("spatial"));
    ui->tableWidget->setItem(9, 0, new QTableWidgetItem("w1"));
    ui->tableWidget->setItem(10, 0, new QTableWidgetItem("w2"));
    ui->tableWidget->setItem(11, 0, new QTableWidgetItem("w3"));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem("3"));
    ui->tableWidget->setItem(1, 1, new QTableWidgetItem("50"));
    ui->tableWidget->setItem(2, 1, new QTableWidgetItem("10"));
    ui->tableWidget->setItem(3, 1, new QTableWidgetItem("3"));
    ui->tableWidget->setItem(4, 1, new QTableWidgetItem("20"));
    ui->tableWidget->setItem(5, 1, new QTableWidgetItem("10"));
    ui->tableWidget->setItem(6, 1, new QTableWidgetItem("0.2"));
    ui->tableWidget->setItem(7, 1, new QTableWidgetItem("3"));
    ui->tableWidget->setItem(8, 1, new QTableWidgetItem("0"));
    ui->tableWidget->setItem(9, 1, new QTableWidgetItem("1"));
    ui->tableWidget->setItem(10, 1, new QTableWidgetItem("1.5"));
    ui->tableWidget->setItem(11, 1, new QTableWidgetItem("0.2"));
    for(int i = 0; i < ui->tableWidget->rowCount(); i++) {
        ui->tableWidget->item(i, 0)->setFlags(Qt::NoItemFlags);
    }
}

void MainWindow::on_actionPBA_triggered()
{
    currentProcess = 6;
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(6);
    ui->tableWidget->setColumnCount(2);
    ui->label_3->setText("PBA");
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem("Centers(C)"));
    ui->tableWidget->setItem(1, 0, new QTableWidgetItem("Depletion(D)"));
    ui->tableWidget->setItem(2, 0, new QTableWidgetItem("spatial"));
    ui->tableWidget->setItem(3, 0, new QTableWidgetItem("w1"));
    ui->tableWidget->setItem(4, 0, new QTableWidgetItem("w2"));
    ui->tableWidget->setItem(5, 0, new QTableWidgetItem("w3"));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem("3"));
    ui->tableWidget->setItem(1, 1, new QTableWidgetItem("10"));
    ui->tableWidget->setItem(2, 1, new QTableWidgetItem("0"));
    ui->tableWidget->setItem(3, 1, new QTableWidgetItem("1"));
    ui->tableWidget->setItem(4, 1, new QTableWidgetItem("1.5"));
    ui->tableWidget->setItem(5, 1, new QTableWidgetItem("0.2"));
    for(int i = 0; i < ui->tableWidget->rowCount(); i++) {
        ui->tableWidget->item(i, 0)->setFlags(Qt::NoItemFlags);
    }
}

void MainWindow::on_actionPBA_K_means_triggered()
{
    currentProcess = 8;
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(6);
    ui->tableWidget->setColumnCount(2);
    ui->label_3->setText("PBA-K-means");
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem("Centers(C)"));
    ui->tableWidget->setItem(1, 0, new QTableWidgetItem("Depletion(D)"));
    ui->tableWidget->setItem(2, 0, new QTableWidgetItem("spatial"));
    ui->tableWidget->setItem(3, 0, new QTableWidgetItem("w1"));
    ui->tableWidget->setItem(4, 0, new QTableWidgetItem("w2"));
    ui->tableWidget->setItem(5, 0, new QTableWidgetItem("w3"));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem("3"));
    ui->tableWidget->setItem(1, 1, new QTableWidgetItem("10"));
    ui->tableWidget->setItem(2, 1, new QTableWidgetItem("0"));
    ui->tableWidget->setItem(3, 1, new QTableWidgetItem("1"));
    ui->tableWidget->setItem(4, 1, new QTableWidgetItem("1.5"));
    ui->tableWidget->setItem(5, 1, new QTableWidgetItem("0.2"));
    for(int i = 0; i < ui->tableWidget->rowCount(); i++) {
        ui->tableWidget->item(i, 0)->setFlags(Qt::NoItemFlags);
    }
}

void MainWindow::on_actionACO_K_means_triggered()
{
    currentProcess = 21;
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(11);
    ui->tableWidget->setColumnCount(2);
    ui->label_3->setText("ACO-K-means");
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem("Centers(K)"));
    ui->tableWidget->setItem(1, 0, new QTableWidgetItem("Ants(m)"));
    ui->tableWidget->setItem(2, 0, new QTableWidgetItem("spatial"));
    ui->tableWidget->setItem(3, 0, new QTableWidgetItem("w1"));
    ui->tableWidget->setItem(4, 0, new QTableWidgetItem("w2"));
    ui->tableWidget->setItem(5, 0, new QTableWidgetItem("w3"));
    ui->tableWidget->setItem(6, 0, new QTableWidgetItem("Q"));
    ui->tableWidget->setItem(7, 0, new QTableWidgetItem("karma"));
    ui->tableWidget->setItem(8, 0, new QTableWidgetItem("p"));
    ui->tableWidget->setItem(9, 0, new QTableWidgetItem("alpha"));
    ui->tableWidget->setItem(10, 0, new QTableWidgetItem("beta"));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem("3"));
    ui->tableWidget->setItem(1, 1, new QTableWidgetItem("10"));
    ui->tableWidget->setItem(2, 1, new QTableWidgetItem("0"));
    ui->tableWidget->setItem(3, 1, new QTableWidgetItem("1"));
    ui->tableWidget->setItem(4, 1, new QTableWidgetItem("1.5"));
    ui->tableWidget->setItem(5, 1, new QTableWidgetItem("0.2"));
    ui->tableWidget->setItem(6, 1, new QTableWidgetItem("100"));
    ui->tableWidget->setItem(7, 1, new QTableWidgetItem("100"));
    ui->tableWidget->setItem(8, 1, new QTableWidgetItem("0.1"));
    ui->tableWidget->setItem(9, 1, new QTableWidgetItem("1"));
    ui->tableWidget->setItem(10, 1, new QTableWidgetItem("1"));
    for(int i = 0; i < ui->tableWidget->rowCount(); i++) {
        ui->tableWidget->item(i, 0)->setFlags(Qt::NoItemFlags);
    }
}

void MainWindow::on_actionGA_K_means_triggered()
{
    currentProcess = 22;
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(8);
    ui->tableWidget->setColumnCount(2);
    ui->label_3->setText("GA-K-means");
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem("Centers(K)"));
    ui->tableWidget->setItem(1, 0, new QTableWidgetItem("population(P)"));
    ui->tableWidget->setItem(2, 0, new QTableWidgetItem("spatial"));
    ui->tableWidget->setItem(3, 0, new QTableWidgetItem("w1"));
    ui->tableWidget->setItem(4, 0, new QTableWidgetItem("w2"));
    ui->tableWidget->setItem(5, 0, new QTableWidgetItem("w3"));
    ui->tableWidget->setItem(6, 0, new QTableWidgetItem("pc"));
    ui->tableWidget->setItem(7, 0, new QTableWidgetItem("pm"));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem("3"));
    ui->tableWidget->setItem(1, 1, new QTableWidgetItem("20"));
    ui->tableWidget->setItem(2, 1, new QTableWidgetItem("0"));
    ui->tableWidget->setItem(3, 1, new QTableWidgetItem("1"));
    ui->tableWidget->setItem(4, 1, new QTableWidgetItem("1.5"));
    ui->tableWidget->setItem(5, 1, new QTableWidgetItem("0.2"));
    ui->tableWidget->setItem(6, 1, new QTableWidgetItem("0.4"));
    ui->tableWidget->setItem(7, 1, new QTableWidgetItem("0.4"));
    for(int i = 0; i < ui->tableWidget->rowCount(); i++) {
        ui->tableWidget->item(i, 0)->setFlags(Qt::NoItemFlags);
    }
}

void MainWindow::on_actionSA_K_means_triggered()
{
    currentProcess = 23;
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(6);
    ui->tableWidget->setColumnCount(2);
    ui->label_3->setText("SA-K-means");
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem("Centers(K)"));
    ui->tableWidget->setItem(1, 0, new QTableWidgetItem("DeltaE"));
    ui->tableWidget->setItem(2, 0, new QTableWidgetItem("spatial"));
    ui->tableWidget->setItem(3, 0, new QTableWidgetItem("w1"));
    ui->tableWidget->setItem(4, 0, new QTableWidgetItem("w2"));
    ui->tableWidget->setItem(5, 0, new QTableWidgetItem("w3"));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem("3"));
    ui->tableWidget->setItem(1, 1, new QTableWidgetItem("5"));
    ui->tableWidget->setItem(2, 1, new QTableWidgetItem("0"));
    ui->tableWidget->setItem(3, 1, new QTableWidgetItem("1"));
    ui->tableWidget->setItem(4, 1, new QTableWidgetItem("1.5"));
    ui->tableWidget->setItem(5, 1, new QTableWidgetItem("0.2"));
    for(int i = 0; i < ui->tableWidget->rowCount(); i++) {
        ui->tableWidget->item(i, 0)->setFlags(Qt::NoItemFlags);
    }
}




void MainWindow::on_actionGT_PBA_K_means_triggered()
{
    currentProcess = 10;
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(6);
    ui->tableWidget->setColumnCount(2);
    ui->label_3->setText("GT_PBA-K-means");
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem("Centers(C)"));
    ui->tableWidget->setItem(1, 0, new QTableWidgetItem("Depletion(D)"));
    ui->tableWidget->setItem(2, 0, new QTableWidgetItem("spatial"));
    ui->tableWidget->setItem(3, 0, new QTableWidgetItem("w1"));
    ui->tableWidget->setItem(4, 0, new QTableWidgetItem("w2"));
    ui->tableWidget->setItem(5, 0, new QTableWidgetItem("w3"));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem("3"));
    ui->tableWidget->setItem(1, 1, new QTableWidgetItem("10"));
    ui->tableWidget->setItem(2, 1, new QTableWidgetItem("0"));
    ui->tableWidget->setItem(3, 1, new QTableWidgetItem("1"));
    ui->tableWidget->setItem(4, 1, new QTableWidgetItem("1.5"));
    ui->tableWidget->setItem(5, 1, new QTableWidgetItem("0.2"));
    for(int i = 0; i < ui->tableWidget->rowCount(); i++) {
        ui->tableWidget->item(i, 0)->setFlags(Qt::NoItemFlags);
    }
}

void MainWindow::on_actionSU_PBA_K_means_triggered()
{
    currentProcess = 18;
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(6);
    ui->tableWidget->setColumnCount(2);
    ui->label_3->setText("SU_PBA-K-means");
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem("Centers(C)"));
    ui->tableWidget->setItem(1, 0, new QTableWidgetItem("Depletion(D)"));
    ui->tableWidget->setItem(2, 0, new QTableWidgetItem("spatial"));
    ui->tableWidget->setItem(3, 0, new QTableWidgetItem("w1"));
    ui->tableWidget->setItem(4, 0, new QTableWidgetItem("w2"));
    ui->tableWidget->setItem(5, 0, new QTableWidgetItem("w3"));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem("3"));
    ui->tableWidget->setItem(1, 1, new QTableWidgetItem("10"));
    ui->tableWidget->setItem(2, 1, new QTableWidgetItem("0"));
    ui->tableWidget->setItem(3, 1, new QTableWidgetItem("1"));
    ui->tableWidget->setItem(4, 1, new QTableWidgetItem("1.5"));
    ui->tableWidget->setItem(5, 1, new QTableWidgetItem("0.2"));
    for(int i = 0; i < ui->tableWidget->rowCount(); i++) {
        ui->tableWidget->item(i, 0)->setFlags(Qt::NoItemFlags);
    }
}

void MainWindow::on_actionGT_PBA_triggered()
{
    currentProcess = 14;
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(6);
    ui->tableWidget->setColumnCount(2);
    ui->label_3->setText("GTPBA");
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem("Centers(C)"));
    ui->tableWidget->setItem(1, 0, new QTableWidgetItem("Depletion(D)"));
    ui->tableWidget->setItem(2, 0, new QTableWidgetItem("spatial"));
    ui->tableWidget->setItem(3, 0, new QTableWidgetItem("w1"));
    ui->tableWidget->setItem(4, 0, new QTableWidgetItem("w2"));
    ui->tableWidget->setItem(5, 0, new QTableWidgetItem("w3"));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem("3"));
    ui->tableWidget->setItem(1, 1, new QTableWidgetItem("10"));
    ui->tableWidget->setItem(2, 1, new QTableWidgetItem("0"));
    ui->tableWidget->setItem(3, 1, new QTableWidgetItem("1"));
    ui->tableWidget->setItem(4, 1, new QTableWidgetItem("1.5"));
    ui->tableWidget->setItem(5, 1, new QTableWidgetItem("0.2"));
    for(int i = 0; i < ui->tableWidget->rowCount(); i++) {
        ui->tableWidget->item(i, 0)->setFlags(Qt::NoItemFlags);
    }
}

void MainWindow::on_actionGT_BA_triggered()
{
    currentProcess = 15;
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(12);
    ui->tableWidget->setColumnCount(2);
    ui->label_3->setText("GTBA");
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem("Centers(C)"));
    ui->tableWidget->setItem(1, 0, new QTableWidgetItem("Scouts(n)"));
    ui->tableWidget->setItem(2, 0, new QTableWidgetItem("Sites(m)"));
    ui->tableWidget->setItem(3, 0, new QTableWidgetItem("BestSites(e)"));
    ui->tableWidget->setItem(4, 0, new QTableWidgetItem("BeeForBest(nep)"));
    ui->tableWidget->setItem(5, 0, new QTableWidgetItem("BeeForOther(nsp)"));
    ui->tableWidget->setItem(6, 0, new QTableWidgetItem("Neighborhood(ngh)"));
    ui->tableWidget->setItem(7, 0, new QTableWidgetItem("imax"));
    ui->tableWidget->setItem(8, 0, new QTableWidgetItem("spatial"));
    ui->tableWidget->setItem(9, 0, new QTableWidgetItem("w1"));
    ui->tableWidget->setItem(10, 0, new QTableWidgetItem("w2"));
    ui->tableWidget->setItem(11, 0, new QTableWidgetItem("w3"));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem("3"));
    ui->tableWidget->setItem(1, 1, new QTableWidgetItem("50"));
    ui->tableWidget->setItem(2, 1, new QTableWidgetItem("10"));
    ui->tableWidget->setItem(3, 1, new QTableWidgetItem("3"));
    ui->tableWidget->setItem(4, 1, new QTableWidgetItem("20"));
    ui->tableWidget->setItem(5, 1, new QTableWidgetItem("10"));
    ui->tableWidget->setItem(6, 1, new QTableWidgetItem("0.2"));
    ui->tableWidget->setItem(7, 1, new QTableWidgetItem("3"));
    ui->tableWidget->setItem(8, 1, new QTableWidgetItem("0"));
    ui->tableWidget->setItem(9, 1, new QTableWidgetItem("1"));
    ui->tableWidget->setItem(10, 1, new QTableWidgetItem("1.5"));
    ui->tableWidget->setItem(11, 1, new QTableWidgetItem("0.2"));
    for(int i = 0; i < ui->tableWidget->rowCount(); i++) {
        ui->tableWidget->item(i, 0)->setFlags(Qt::NoItemFlags);
    }
}

void MainWindow::on_actionPSO_K_means_triggered()
{
    currentProcess = 9;
    ui->tableWidget->clear();
    ui->tableWidget->setRowCount(10);
    ui->tableWidget->setColumnCount(2);
    ui->label_3->setText("PSO-K-means");
    ui->tableWidget->setItem(0, 0, new QTableWidgetItem("Centers(C)"));
    ui->tableWidget->setItem(1, 0, new QTableWidgetItem("swarmNum"));
    ui->tableWidget->setItem(2, 0, new QTableWidgetItem("tmax"));
    ui->tableWidget->setItem(3, 0, new QTableWidgetItem("spatial"));
    ui->tableWidget->setItem(4, 0, new QTableWidgetItem("w1"));
    ui->tableWidget->setItem(5, 0, new QTableWidgetItem("w2"));
    ui->tableWidget->setItem(6, 0, new QTableWidgetItem("w3"));
    ui->tableWidget->setItem(7, 0, new QTableWidgetItem("w"));
    ui->tableWidget->setItem(8, 0, new QTableWidgetItem("c1"));
    ui->tableWidget->setItem(9, 0, new QTableWidgetItem("c2"));
    ui->tableWidget->setItem(0, 1, new QTableWidgetItem("3"));
    ui->tableWidget->setItem(1, 1, new QTableWidgetItem("10"));
    ui->tableWidget->setItem(2, 1, new QTableWidgetItem("20"));
    ui->tableWidget->setItem(3, 1, new QTableWidgetItem("0"));
    ui->tableWidget->setItem(4, 1, new QTableWidgetItem("1"));
    ui->tableWidget->setItem(5, 1, new QTableWidgetItem("1.5"));
    ui->tableWidget->setItem(6, 1, new QTableWidgetItem("0.2"));
    ui->tableWidget->setItem(7, 1, new QTableWidgetItem("0.9"));
    ui->tableWidget->setItem(8, 1, new QTableWidgetItem("1"));
    ui->tableWidget->setItem(9, 1, new QTableWidgetItem("1"));
    for(int i = 0; i < ui->tableWidget->rowCount(); i++) {
        ui->tableWidget->item(i, 0)->setFlags(Qt::NoItemFlags);
    }
}

//function that select an image by double click an item in listwidget
void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    //find corresponding tab by name
    for(int i = 0; i < ui->tabWidget->count(); i++) {
        if(item->text() == ui->tabWidget->tabText(i)) {
            ui->tabWidget->setCurrentIndex(i);
        }
    }
}

void MainWindow::on_actionAverage_triggered()
{
    double sumje = 0;
    double sumdmin = 0;
    double sumdmax = 0;
    double sumfit = 0;
    int len = ui->tableWidget1->rowCount();
    for(int i = 0; i < len; i++) {
        QString str = ui->tableWidget1->item(i, 1)->text();
        double je = str.toDouble();
        sumje += je;
        str = ui->tableWidget1->item(i, 2)->text();
        double dma = str.toDouble();
        sumdmax += dma;
        str = ui->tableWidget1->item(i, 3)->text();
        double dmi = str.toDouble();
        sumdmin += dmi;
        str = ui->tableWidget1->item(i, 4)->text();
        double fit = str.toDouble();
        sumfit += fit;
    }
    setAnaTable(sumje / len, sumdmax / len, sumdmin / len, sumfit / len);
}


void MainWindow::on_actionClear_triggered()
{
    ui->tableWidget1->setRowCount(0);
}

void MainWindow::on_actionShow_bands_triggered()
{
    QString fileName = getFileName();
    QImage imageIn;
    if(!imageIn.load(fileName)) {
        QMessageBox::information(this, "warnning", "can't open file");
        return;
    }
    VVV rgb = imageToRGBArray(imageIn);
    VVV newrgb = rgb;
    for(int i = 0; i < imageIn.width(); i++) {
        for(int j = 0; j < imageIn.height(); j++) {
            newrgb[1][i][j] = rgb[1][i][j];
            newrgb[2][i][j] = rgb[1][i][j];
            newrgb[3][i][j] = rgb[1][i][j];
        }
    }
    QImage imageOut = RGBArrayToimage(newrgb, imageIn);
    showImage(&imageOut);
    for(int i = 0; i < imageIn.width(); i++) {
        for(int j = 0; j < imageIn.height(); j++) {
            newrgb[2][i][j] = rgb[2][i][j];
            newrgb[1][i][j] = rgb[2][i][j];
            newrgb[3][i][j] = rgb[2][i][j];
        }
    }
    imageOut = RGBArrayToimage(newrgb, imageIn);
    showImage(&imageOut);
    for(int i = 0; i < imageIn.width(); i++) {
        for(int j = 0; j < imageIn.height(); j++) {
            newrgb[3][i][j] = rgb[3][i][j];
            newrgb[2][i][j] = rgb[3][i][j];
            newrgb[1][i][j] = rgb[3][i][j];
        }
    }
    imageOut = RGBArrayToimage(newrgb, imageIn);
    showImage(&imageOut);
}

void MainWindow::on_actionReduce_Shadow_triggered()
{
    QString fileName = getFileName();
    QImage imageIn;
    if(!imageIn.load(fileName)) {
        QMessageBox::information(this, "warnning", "can't open file");
        return;
    }
    VVV rgb = imageToRGBArray(imageIn);
    for(int i = 0; i < imageIn.width(); i++) {
        for(int j = 0; j < imageIn.height(); j++) {
            if(rgb[1][i][j] < 100 || rgb[2][i][j] < 100 || rgb[3][i][j] < 100) {
                rgb[1][i][j] += 100;
                rgb[2][i][j] += 100;
                rgb[3][i][j] += 100;
            }
        }
    }
    QImage imageOut = RGBArrayToimage(rgb, imageIn);
    showImage(&imageOut);
}

void MainWindow::on_actionReduce_class_triggered()
{
    QString GTfileName = QFileDialog::getOpenFileName(this,
                                                    tr("select image"),
                                                    "",
                                                    tr("Images (*.png *.bmp *.jpg *.tif *.GIF)"));
    if(!GTfileName.isEmpty()) {
        QImage image;
        if(!(image.load(GTfileName))) {
            QMessageBox::information(this, "warning",
                                     "can't open image");
            return;
        }
        QString fileName = getFileName();
        QImage imageIn;
        if(!imageIn.load(fileName)) {
            QMessageBox::information(this, "warnning", "can't open file");
            return;
        }
        VVV rgb = imageToRGBArray(imageIn);
        QString str = ui->tableWidget->item(0, 1)->text();
        int k = str.toInt();
        vector<vector<int>> gtLabel = getGtLabel(k, image);
        int centers[5] = {0};
        for(int i = 0; i < image.width(); i++) {
            for(int j = 0; j < image.height(); j++) {
                centers[gtLabel[i][j]] ++;
            }
        }
        int maxi = 0;
        int maxcnt = 0;
        for(int i = 0; i < k; i++) {
            if(centers[i] > maxcnt) {
                maxcnt = centers[i];
                maxi = i;
            }
        }
        int feature[3] = {0};
        for(int i = 0; i < image.width(); i++) {
            for(int j = 0; j < image.height(); j++) {
                if(gtLabel[i][j] == maxi) {
                    feature[0] += rgb[1][i][j];
                    feature[1] += rgb[2][i][j];
                    feature[2] += rgb[3][i][j];
                }
            }
        }
        feature[0] = rgb[1][30][30];
        feature[1] = rgb[2][30][30];
        feature[2] = rgb[3][30][30];
        for(int i = 0; i < image.width(); i++) {
            for(int j = 0; j < image.height(); j++) {
                if(gtLabel[i][j] == maxi) {
                    rgb[1][i][j] = feature[0];
                    rgb[2][i][j] = feature[1];
                    rgb[3][i][j] = feature[2];
                }
            }
        }
        for(int i = 400; i < image.width(); i++) {
            for(int j = 400; j < image.height(); j++) {
                rgb[1][i][j] = feature[0];
                rgb[2][i][j] = feature[1];
                rgb[3][i][j] = feature[2];
            }
        }
        QImage imageout = RGBArrayToimage(rgb, imageIn);
        showImage(&imageout);
    }
}


void MainWindow::on_actionForm_image_triggered()
{
    QString fileName1 = QFileDialog::getOpenFileName(this,
                                                     tr("select image"),
                                                     "",
                                                     tr("Images (*.png *.bmp *.jpg *.tif *.GIF)"));
    QImage imageIn1;
    if(!imageIn1.load(fileName1)) {
        QMessageBox::information(this, "warnning", "can't open file");
        return;
    }
    VVV rgb1 = imageToRGBArray(imageIn1);
    QString fileName2 = QFileDialog::getOpenFileName(this,
                                                     tr("select image"),
                                                     "",
                                                     tr("Images (*.png *.bmp *.jpg *.tif *.GIF)"));
    QImage imageIn2;
    if(!imageIn2.load(fileName2)) {
        QMessageBox::information(this, "warnning", "can't open file");
        return;
    }
    VVV rgb2 = imageToRGBArray(imageIn2);
    QString fileName3 = QFileDialog::getOpenFileName(this,
                                                     tr("select image"),
                                                     "",
                                                     tr("Images (*.png *.bmp *.jpg *.tif *.GIF)"));
    QImage imageIn3;
    if(!imageIn3.load(fileName3)) {
        QMessageBox::information(this, "warnning", "can't open file");
        return;
    }
    VVV rgb3 = imageToRGBArray(imageIn3);
    VVV newrgb = rgb1;
    for(int i = 0; i < imageIn1.width(); i++) {
        for(int j = 0; j < imageIn1.height(); j++) {
            newrgb[1][i][j] = rgb1[1][i][j];
            newrgb[2][i][j] = rgb2[1][i][j];
            newrgb[3][i][j] = rgb3[1][i][j];
        }
    }

    cout << rgb1[0][32][32] << ' ' << newrgb[1][32][32] << ' ' << newrgb[2][32][32] << ' ' << newrgb[3][32][32] << endl;
    QImage imageout1 = RGBArrayToimage(rgb1, imageIn1);
    showImage(&imageout1);
    showImage(&imageIn2);
    showImage(&imageIn3);
    QImage imageout = RGBArrayToimage(newrgb, imageIn1);
    showImage(&imageout);
}


//function of run
void MainWindow::on_actionRun_triggered()
{
    switch (currentProcess) {
    case 0:
        QMessageBox::information(this, "warnning", "no action");
        break;
    case 1:
        grayScale();
        break;
    case 2:
        edgeDectect();
        break;
    case 3:
        mirror();
        break;
    case 4:
        BA();
        break;
    case 5:
        kMeans();
        break;
    case 6:
        PBA();
        break;
    case 7:
        CV_kMeans();
        break;
    case 8:
        PBAkMeans();
        break;
    case 9:
        PSOkMeans();
        break;
    case 10:
        GTPBAkMeans();
        break;
    case 11:
        CV_enhancement();
        break;
    case 12:
        CV_enhance_histo();
        break;
    case 13:
        CV_enhance_log();
        break;
    case 14:
        for(int i = 0; i < 10; i++)
            GTPBA();
        break;
    case 15:
        for(int i = 0; i < 10; i++)
            GTBA();
        break;
    case 16:
        GT_CV_Kmeans();
        break;
    case 17:
        GT_Kmeans();
        break;
    case 18:
        SU_PBAkMeans();
        break;
    case 19:
        myEnhance();
        break;
    case 20:
        tMeans();
        break;
    case 21:
        ACOkMeans();
        break;
    case 22:
        GAkMeans();
        break;
    case 23:
        SAkMeans();
        break;
    default:
        QMessageBox::information(this, "warinning", "invalid");
    }
}

//get image path of current tab
QString MainWindow::getFileName() {
    QString fileName;
    //find the row of current image in listwidget
    for(int i = 0; i < ui->listWidget->count(); i++) {
        int index = ui->tabWidget->currentIndex();
        if(ui->listWidget->item(i)->text() == ui->tabWidget->tabText(index)) {
            //get path from listItem
            fileName = listItems[i];
            break;
        }
    }
    return fileName;
}

//function that show a processed image
void MainWindow::showImage(QImage *image) {
    QLabel *label = new QLabel(ui->tabWidget->currentWidget());
    label->setPixmap(QPixmap::fromImage(*image));
    for(int i = 0; i < ui->listWidget->count(); i++) {
        int index = ui->tabWidget->currentIndex();
        if(ui->listWidget->item(i)->text() == ui->tabWidget->tabText(index)) {
            //decide the location of new image
            int x = tabImageNum[i] % columnsNum;
            int y = tabImageNum[i] / columnsNum;
            //zoom to tab zoom rate
            label->setPixmap(label->pixmap()->scaled(tabLabel[i][0]->pixmap()->width(),
                    tabLabel[i][0]->pixmap()->height(), Qt::KeepAspectRatio, Qt::SmoothTransformation));
            //one more image in current tab
            tabImageNum[i]++;
            //add new image in the layout of this tab
            la[i]->addWidget(label, y, x);
            //save new image label
            tabLabel[i].push_back(label);
        }
    }
}

void MainWindow::showAna() {
    double sumje = 0;
    double sumdmin = 0;
    double sumdmax = 0;
    double sumfit = 0;
    int len = ui->tableWidget1->rowCount();
    for(int i = 0; i < ui->tableWidget1->rowCount(); i++) {
        QString str = ui->tableWidget->item(i, 1)->text();
        double je = str.toDouble();
        sumje += je;
        str = ui->tableWidget->item(i, 2)->text();
        double dmax = str.toDouble();
        sumdmax += dmax;
        str = ui->tableWidget->item(i, 3)->text();
        double dmin = str.toDouble();
        sumdmin += dmin;
        str = ui->tableWidget->item(i, 4)->text();
        double fit = str.toDouble();
        sumfit += fit;
    }
    setAnaTable(sumje / len, sumdmax / len, sumdmin / len, sumfit / len);
}

//grayscale
void MainWindow::grayScale()
{
    QString fileName = getFileName();
    QImage *image = new QImage;
    if(!image->load(fileName)) {
        QMessageBox::information(this, "warnning", "can't open file");
        return;
    }
    //store processed image
    QImage newImage;
    newImage = image->copy();

    //set the rgb value of a pixel to the average value
    for(int i = 0; i < image->width(); i++) {
        for(int j = 0; j < image->height(); j++) {
            QColor color = image->pixelColor(i, j);
            int average = (color.red() + color.green() + color.blue()) / 3;
            color.setRed(average);
            color.setGreen(average);
            color.setBlue(average);
            newImage.setPixelColor(i, j, color);
        }
    }
    showImage(&newImage);
}

void MainWindow::myEnhance() {
    QString fileName = getFileName();
    QImage *image = new QImage;
    if(!image->load(fileName)) {
        QMessageBox::information(this, "warnning", "can't open file");
        return;
    }
    //store processed image
    QImage newImage;
    newImage = image->copy();

    //set the rgb value of a pixel to the average value
    for(int i = 0; i < image->width(); i++) {
        for(int j = 0; j < image->height(); j++) {
            QColor color = image->pixelColor(i, j);
//            int minc = color.red() < color.green() ? color.red() : color.green();
//            minc = minc < color.blue() ? minc : color.blue();
//            color.setRed(color.red() + color.red() - minc);
//            color.setGreen(color.green() +color.green() - minc);
//            color.setBlue(color.blue() + color.blue() - minc);
//            newImage.setPixelColor(i, j, color);
            int maxc = color.red() > color.green() ? color.red() : color.green();
            maxc = maxc > color.blue() ? maxc : color.blue();
            color.setRed(color.red() == maxc ? 255 : maxc);
//            color.setGreen(color.green() == maxc ? maxc : 0);
//            color.setBlue(color.blue() == maxc ? maxc : 0);
//            if(color.red() == maxc) {
//                color.setGreen(color.green() - 50);
//                color.setBlue(color.blue() - 50);
//            }
            newImage.setPixelColor(i, j, color);
        }
    }
    showImage(&newImage);
}

void MainWindow::edgeDectect()
{
    QString fileName = getFileName();
    QImage *image = new QImage;
    if(!image->load(fileName)) {
        QMessageBox::information(this, "warnning", "cannot open file");
        return;
    }
    QImage newImage;
    newImage = image->copy();
    for(int i = 0; i < image->width(); i++) {
        for(int j = 0; j < image->height(); j++) {
            QColor color = image->pixelColor(i, j);
            int xr = 0;
            int yr = 0;
            int xg = 0;
            int yg = 0;
            int xb = 0;
            int yb = 0;
            if(i > 0 && i < image->width() - 1) {
                xr = image->pixelColor(i + 1, j).red() - image->pixelColor(i - 1, j).red();
                xg = image->pixelColor(i + 1, j).green() - image->pixelColor(i - 1, j).green();
                xb = image->pixelColor(i + 1, j).blue() - image->pixelColor(i - 1, j).blue();
            }
            if(j > 0 && i < image->height() - 1) {
                yr = image->pixelColor(i + 1, j).red() - image->pixelColor(i - 1, j).red();
                yg = image->pixelColor(i + 1, j).green() - image->pixelColor(i - 1, j).green();
                yb = image->pixelColor(i + 1, j).blue() - image->pixelColor(i - 1, j).blue();
            }
            color.setRed((int)sqrt(pow(xr, 2) + pow(yr, 2)));
            color.setGreen((int)sqrt(pow(xg, 2) + pow(yg, 2)));
            color.setBlue((int)sqrt(pow(xb, 2) + pow(yb, 2)));
            newImage.setPixelColor(i, j, color);
        }
    }
    showImage(&newImage);
}

void MainWindow::mirror()
{
    QString fileName = getFileName();
    QImage imageIn;
    if(!imageIn.load(fileName)) {
        QMessageBox::information(this, "warnning", "can't open file");
        return;
    }
    Point dimension = getImageDimension(imageIn);
    VVV rgb = imageToRGBArray(imageIn);
    for(int i = 0; i < dimension.y; i++) {
        for(int j = 0; j < dimension.x / 2; j++) {
            swap(rgb[0][j][i], rgb[0][dimension.x - j - 1][i]);
            swap(rgb[1][j][i], rgb[1][dimension.x - j - 1][i]);
            swap(rgb[2][j][i], rgb[2][dimension.x - j - 1][i]);
            swap(rgb[3][j][i], rgb[3][dimension.x - j - 1][i]);
        }
    }
    QImage imageOut = RGBArrayToimage(rgb, imageIn);
    showImage(&imageOut);
}

void MainWindow::zoom(double zoomScale) {
    for(int i = 0; i < ui->listWidget->count(); i++) {
        if(ui->listWidget->item(i)->text() == ui->tabWidget->tabText(ui->tabWidget->currentIndex())) {
            for(int j = 0; j < tabImageNum[i]; j++) {
                int w = tabLabel[i][j]->pixmap()->width();
                int h = tabLabel[i][j]->pixmap()->height();
                tabLabel[i][j]->setPixmap(tabLabel[i][j]->pixmap()->scaled((int)(w * zoomScale),
                                                                           (int)(h * zoomScale),
                                                                           Qt::KeepAspectRatio,
                                                                           Qt::SmoothTransformation));
            }

        }
    }
}

void MainWindow::on_actionZoom_in_triggered()
{
    zoom(zoomInRate);
}

void MainWindow::on_actionZoom_out_triggered()
{
    zoom(zoomOutRate);
}

void MainWindow::on_actionsave_triggered()
{
    for(int i = 0; i < ui->listWidget->count(); i++) {
        if(ui->listWidget->item(i)->text() == ui->tabWidget->tabText(ui->tabWidget->currentIndex())) {
            for(int j = 0; j < tabImageNum[i]; j++) {
                QImage out;
                out = tabLabel[i][j]->pixmap()->toImage();
                QString fileName = QFileDialog::getSaveFileName(this, "save " + QString::number(j + 1) + "th image",
                                                                "", tr("Images (*.png *.bmp *.jpg *.tif *.GIF)"));
                out.save(fileName);
            }
        }
    }
}

Point MainWindow::getImageDimension(QImage image)
{
    Point p;
    p.x = image.width();
    p.y = image.height();
    return p;
}

vector<vector<vector<int>>> MainWindow::imageToRGBArray(QImage imageIn)
{
    vector<vector<vector<int>>> RGBArray;
    //initialize vector space
    /********************************************/
    RGBArray.resize(4);
    for(int i = 0; i < 4; i++) {
        RGBArray[i].resize(imageIn.width());
        for(int j = 0; j < imageIn.width(); j++) {
            RGBArray[i][j].resize(imageIn.height());
        }
    }
    /********************************************/
    for(int i = 0; i < imageIn.width(); i++) {
        for(int j = 0; j < imageIn.height(); j++) {
            RGBArray[0][i][j] = imageIn.pixelColor(i, j).alpha();
            RGBArray[1][i][j] = imageIn.pixelColor(i, j).red();
            RGBArray[2][i][j] = imageIn.pixelColor(i, j).green();
            RGBArray[3][i][j] = imageIn.pixelColor(i, j).blue();
        }
    }
    return RGBArray;
}

QImage MainWindow::RGBArrayToimage(vector<vector<vector<int>>> RGBArray, QImage imageIn)
{
    QImage imageOut;
    imageOut = imageIn.copy();
    for(int i = 0; i < imageIn.width(); i++) {
        for(int j = 0; j < imageIn.height(); j++) {
            QColor color;
            color.setAlpha(RGBArray[0][i][j]);
            color.setRed(RGBArray[1][i][j]);
            color.setGreen(RGBArray[2][i][j]);
            color.setBlue(RGBArray[3][i][j]);
            imageOut.setPixelColor(i, j, color);
        }
    }
    return imageOut;
}

QImage MainWindow::RGBArrayToimage_1(vector<vector<vector<int>>> RGBArray, Point d)
{
    QImage imageOut(d.x, d.y, QImage::Format_RGB32);
    for(int i = 0; i < d.x; i++) {
        for(int j = 0; j < d.y; j++) {
            QColor color;
            color.setAlpha(RGBArray[0][i][j]);
            color.setRed(RGBArray[1][i][j]);
            color.setGreen(RGBArray[2][i][j]);
            color.setBlue(RGBArray[3][i][j]);
            imageOut.setPixelColor(i, j, color);
        }
    }
    return imageOut;
}

VVV MainWindow::setRGBArrayFromLabel(vector<vector<int>> label) {

    int w = label.size();
    int h = label[0].size();

    VVV rgb;
    rgb.resize(4);
    for(int i = 0; i < 4; i++) {
        rgb[i].resize(w);
        for(int j = 0; j < w; j++) {
            rgb[i][j].resize(h);
        }
    }
    int colors[5][3] = {
        {255, 0, 0},
        {0, 255, 0},
        {0, 0, 255},
        {255, 255, 0},
        {0, 255, 255}
    };
    int flag[100] = {0};
    int colorcnt = 0;
    for(int i = 0; i < w; i++) {
        for(int j = 0; j < h; j++) {
            if(flag[label[i][j]] == 0) {
                flag[label[i][j]] = colorcnt + 1;
                colorcnt++;
            }
            rgb[0][i][j] = 255;
            rgb[1][i][j] = colors[flag[label[i][j]]][0];
            rgb[2][i][j] = colors[flag[label[i][j]]][1];
            rgb[3][i][j] = colors[flag[label[i][j]]][2];

        }
    }
    return rgb;
}

void MainWindow:: setAnaTable(double Je, double dmax, double dmin, double fittness) {
    int row = ui->tableWidget1->rowCount();
    int col = ui->tableWidget->rowCount();
    int index = ui->tabWidget->currentIndex();
    QString name = ui->listWidget->item(index)->text();
    ui->tableWidget1->setColumnCount(6 + col);
    ui->tableWidget1->setRowCount(row + 1);
    ui->tableWidget1->setItem(row, 0, new QTableWidgetItem(name));
    ui->tableWidget1->setItem(row, 1, new QTableWidgetItem(QString::number(Je)));
    ui->tableWidget1->setItem(row, 2, new QTableWidgetItem(QString::number(dmax)));
    ui->tableWidget1->setItem(row, 3, new QTableWidgetItem(QString::number(dmin)));
    ui->tableWidget1->setHorizontalHeaderItem(4, new QTableWidgetItem("fitness"));
    ui->tableWidget1->setItem(row, 4, new QTableWidgetItem(QString::number(fittness)));
    ui->tableWidget1->setHorizontalHeaderItem(5, new QTableWidgetItem("type"));
    ui->tableWidget1->setItem(row, 5, new QTableWidgetItem(ui->label_3->text()));
    for(int i = 0; i < col; i++) {
        ui->tableWidget1->setHorizontalHeaderItem(i + 6, new QTableWidgetItem(ui->tableWidget->item(i, 0)->text()));
        ui->tableWidget1->setItem(row, i + 6, new QTableWidgetItem(ui->tableWidget->item(i, 1)->text()));
    }
}

void MainWindow::addAccuracyToAna(double Je, double dmax, double dmin, double fittness, double accuracy){
    int row = ui->tableWidget1->rowCount();
    int col = ui->tableWidget->rowCount();
    int index = ui->tabWidget->currentIndex();
    QString name = ui->listWidget->item(index)->text();
    ui->tableWidget1->setColumnCount(7 + col);
    ui->tableWidget1->setRowCount(row + 1);
    ui->tableWidget1->setItem(row, 0, new QTableWidgetItem(name));
    ui->tableWidget1->setItem(row, 1, new QTableWidgetItem(QString::number(Je)));
    ui->tableWidget1->setItem(row, 2, new QTableWidgetItem(QString::number(dmax)));
    ui->tableWidget1->setItem(row, 3, new QTableWidgetItem(QString::number(dmin)));
    ui->tableWidget1->setHorizontalHeaderItem(4, new QTableWidgetItem("fitness"));
    ui->tableWidget1->setItem(row, 4, new QTableWidgetItem(QString::number(fittness)));
    ui->tableWidget1->setHorizontalHeaderItem(5, new QTableWidgetItem("type"));
    ui->tableWidget1->setItem(row, 5, new QTableWidgetItem(ui->label_3->text()));
    ui->tableWidget1->setHorizontalHeaderItem(6, new QTableWidgetItem("accuracy"));
    ui->tableWidget1->setItem(row, 6, new QTableWidgetItem(QString::number(accuracy)));
    for(int i = 0; i < col; i++) {
        ui->tableWidget1->setHorizontalHeaderItem(i + 7, new QTableWidgetItem(ui->tableWidget->item(i, 0)->text()));
        ui->tableWidget1->setItem(row, i + 7, new QTableWidgetItem(ui->tableWidget->item(i, 1)->text()));
    }
}
double MainWindow::getAccuracy(int c, Point d, vector<vector<int>> re, vector<vector<int>> gt) {
    double res = 0;
    int match[50][50];
    memset(match, 0, sizeof(match));
    for(int i = 0; i < d.x; i++) {
        for(int j = 0; j < d.y; j++) {
            match[gt[i][j]][re[i][j]]++;
        }
    }
    vector<int> fflag;
    int ftotal = 0;
    for(int i = 0; i < c; i++) {
        vector<int> gtTore;
        vector<int> reTogt;
        for(int j = 0; j < c; j++) {
            gtTore.push_back(-1);
            reTogt.push_back(-1);
        }
        int total = 0;
        for(int j = i; j < i + c; j++) {
            int t1 = j % c;
            int cmaxi = 0;
            int in = 0;
            for(int e = 0; e < c; e++) {
                if(reTogt[e] == -1 && match[t1][e] > cmaxi) {
                    in = e;
                    cmaxi = match[t1][e];
                }
                cout << match[t1][e] << ' ';
            }
            cout << endl;
            reTogt[in] = t1;
            total += cmaxi;
        }
        cout << total << ' ' << ftotal << endl << endl;
        if(total > ftotal) {
            fflag.clear();
            for(int j = 0; j < c; j++)
                fflag.push_back(reTogt[j]);
            ftotal = total;
        }
    }
    //showclass(c, d, re, gt, fflag);
    double total = (double)d.x * d.y;
    res = (double)ftotal / total;
    cout << res << endl;
    return res;
}

void MainWindow::showclass(int c, Point d, vector<vector<int>> re, vector<vector<int>> gt, vector<int> acc) {
    vector<vector<vector<int>>> RGBArray;
    RGBArray.resize(4);
    for(int i = 0; i < 4; i++) {
        RGBArray[i].resize(d.x);
        for(int j = 0; j < d.x; j++) {
            RGBArray[i][j].resize(d.y);
        }
    }
    for(int i = 0; i < c; i++) {
        for(int j = 0; j < d.x; j++) {
            for(int e = 0; e < d.y; e++) {
                RGBArray[0][j][e] = 255;
                if(re[j][e] == i) {
                    RGBArray[1][j][e] = 255;
                    RGBArray[2][j][e] = 255;
                    RGBArray[3][j][e] = 255;
                }
                else {
                    RGBArray[1][j][e] = 0;
                    RGBArray[2][j][e] = 0;
                    RGBArray[3][j][e] = 0;
                }
            }
        }
        QImage imageOut = RGBArrayToimage_1(RGBArray, d);
        showImage(&imageOut);
        for(int j = 0; j < d.x; j++) {
            for(int e = 0; e < d.y; e++) {
                RGBArray[0][j][e] = 255;
                if(gt[j][e] == acc[i]) {
                    RGBArray[1][j][e] = 255;
                    RGBArray[2][j][e] = 255;
                    RGBArray[3][j][e] = 255;
                }
                else {
                    RGBArray[1][j][e] = 0;
                    RGBArray[2][j][e] = 0;
                    RGBArray[3][j][e] = 0;
                }
            }
        }
        QImage imageOut2 = RGBArrayToimage_1(RGBArray, d);
        showImage(&imageOut2);
    }
}

vector<vector<int>> MainWindow::getGtLabel(int c, QImage gt) {
    int wid = gt.width();
    int hei = gt.height();
    vector<vector<int>> label;
    label.resize(wid);
    for(int i = 0; i < wid; i++) {
        label[i].resize(hei);
    }
    VVV rgb = imageToRGBArray(gt);
    int ccnt = 1;
    vector<int> oc;
    oc.push_back(rgb[0][0][0]);
    oc.push_back(rgb[1][0][0]);
    oc.push_back(rgb[2][0][0]);
    oc.push_back(rgb[3][0][0]);
    vector<vector<int>> centers;
    centers.push_back(oc);

    showImage(&gt);
    for(int i = 0; i < wid; i++) {
        for(int j = 0; j < hei; j++) {
            int ci = -1;
            for(int e = 0; e < ccnt; e++) {
                if(rgb[0][i][j] == centers[e][0] &&
                        rgb[1][i][j] == centers[e][1] &&
                        rgb[2][i][j] == centers[e][2] &&
                        rgb[3][i][j] == centers[e][3]) {
                    ci = e;
                    break;
                }
            }
            if(ci == -1) {
                vector<int> tc ;
                tc.push_back(rgb[0][i][j]);
                tc.push_back(rgb[1][i][j]);
                tc.push_back(rgb[2][i][j]);
                tc.push_back(rgb[3][i][j]);
                centers.push_back(tc);
                ci = ccnt;
                ccnt++;
            }
            label[i][j] = ci;
        }
    }
    if (ccnt != c) {
        QMessageBox::information(this, "warnning", "the two cluster numbers are not the same");
    }
    return label;
}




/**********************************************

       BA, K-means, PBA-K-means


********************************************/




/*******************************************
 *
 *      Bees Algorithm
 *
 * ****************************************/
void MainWindow::BA()
{
    QString fileName = getFileName();
    QImage imageIn;
    if(!imageIn.load(fileName)) {
        QMessageBox::information(this, "warnning", "can't open file");
        return;
    }
    VVV rgb = imageToRGBArray(imageIn);
    Point dimension = getImageDimension(imageIn);
//    cout << imageIn. << endl;

    /**********************************************/
    //initialize parameters
    QString str = ui->tableWidget->item(0, 1)->text();
    int c = str.toInt();
    str = ui->tableWidget->item(1, 1)->text();
    int n = str.toInt();
    str = ui->tableWidget->item(2, 1)->text();
    int m = str.toInt();
    str = ui->tableWidget->item(3, 1)->text();
    int e = str.toInt();
    str = ui->tableWidget->item(4, 1)->text();
    int nep = str.toInt();
    str = ui->tableWidget->item(5, 1)->text();
    int nsp = str.toInt();
    str = ui->tableWidget->item(6, 1)->text();
    double ngh = str.toDouble();
    str = ui->tableWidget->item(7, 1)->text();
    int imax = str.toInt();
    str = ui->tableWidget->item(8, 1)->text();
    double spatial = str.toDouble();
    str = ui->tableWidget->item(9, 1)->text();
    double w1 = str.toDouble();
    str = ui->tableWidget->item(10, 1)->text();
    double w2 = str.toDouble();
    str = ui->tableWidget->item(11, 1)->text();
    double w3 = str.toDouble();
    /**********************************************/
    BeesAlgorithm bee(c, n, m, e, nep, nsp, ngh, imax, rgb, dimension, spatial, w1, w2, w3);
    VVV newrgb = bee.getRGBArray();
    QImage imageOut = RGBArrayToimage(newrgb, imageIn);
    showImage(&imageOut);
    setAnaTable(bee.getJe(), bee.getDmax(), bee.getDmin(), bee.getFitness());

}

void MainWindow::GTBA(){
    QString GTfileName = QFileDialog::getOpenFileName(this,
                                                    tr("select image"),
                                                    "",
                                                    tr("Images (*.png *.bmp *.jpg *.tif *.GIF)"));
    if(!GTfileName.isEmpty()) {
        QImage image;
        if(!(image.load(GTfileName))) {
            QMessageBox::information(this, "warning",
                                     "can't open image");
            return;
        }
        QString fileName = getFileName();
        QImage imageIn;
        if(!imageIn.load(fileName)) {
            QMessageBox::information(this, "warnning", "can't open file");
            return;
        }
        VVV rgb = imageToRGBArray(imageIn);
        Point dimension = getImageDimension(imageIn);
    //    cout << imageIn. << endl;

        /**********************************************/
        //initialize parameters
        QString str = ui->tableWidget->item(0, 1)->text();
        int c = str.toInt();
        str = ui->tableWidget->item(1, 1)->text();
        int n = str.toInt();
        str = ui->tableWidget->item(2, 1)->text();
        int m = str.toInt();
        str = ui->tableWidget->item(3, 1)->text();
        int e = str.toInt();
        str = ui->tableWidget->item(4, 1)->text();
        int nep = str.toInt();
        str = ui->tableWidget->item(5, 1)->text();
        int nsp = str.toInt();
        str = ui->tableWidget->item(6, 1)->text();
        double ngh = str.toDouble();
        str = ui->tableWidget->item(7, 1)->text();
        int imax = str.toInt();
        str = ui->tableWidget->item(8, 1)->text();
        double spatial = str.toDouble();
        str = ui->tableWidget->item(9, 1)->text();
        double w1 = str.toDouble();
        str = ui->tableWidget->item(10, 1)->text();
        double w2 = str.toDouble();
        str = ui->tableWidget->item(11, 1)->text();
        double w3 = str.toDouble();
        /**********************************************/
        BeesAlgorithm bee(c, n, m, e, nep, nsp, ngh, imax, rgb, dimension, spatial, w1, w2, w3);
        VVV newrgb = bee.getRGBArray();

        double accuracy = getAccuracy(c, dimension, bee.getLabel(), getGtLabel(c, image));

        QImage imageOut = RGBArrayToimage(newrgb, imageIn);
        showImage(&imageOut);
        addAccuracyToAna(bee.getJe(), bee.getDmax(), bee.getDmin(), bee.getFitness(), accuracy);
    }
}

/*********************************************/
/*********************************************/
/*********************************************/



/*******************************************
 *
 *      K-means
 *
 * ****************************************/

void MainWindow::kMeans() {
    QString fileName = getFileName();
    QImage imageIn;
    if(!imageIn.load(fileName)) {
        QMessageBox::information(this, "warnning", "can't open file");
        return;
    }
    VVV rgb = imageToRGBArray(imageIn);
    Point dimension = getImageDimension(imageIn);

    QString str = ui->tableWidget->item(0, 1)->text();
    int k = str.toInt();
    str = ui->tableWidget->item(1, 1)->text();
    double ifspatial = str.toDouble();
    str = ui->tableWidget->item(2, 1)->text();
    double w1 = str.toDouble();
    str = ui->tableWidget->item(3, 1)->text();
    double w2 = str.toDouble();
    str = ui->tableWidget->item(4, 1)->text();
    double w3 = str.toDouble();
    Kmeans kmean(k, ifspatial, rgb, dimension, w1, w2, w3);
    VVV newrgb = kmean.getRGBArray();
    double Je = kmean.getJe();
    double dmax = kmean.getDmax();
    double dmin = kmean.getDmin();
    double fittness = kmean.getFittness();
    QImage imageOut = RGBArrayToimage(newrgb, imageIn);
    showImage(&imageOut);
    setAnaTable(Je, dmax, dmin, fittness);

}

void MainWindow::GT_Kmeans() {
    QString GTfileName = QFileDialog::getOpenFileName(this,
                                                    tr("select image"),
                                                    "",
                                                    tr("Images (*.png *.bmp *.jpg *.tif *.GIF)"));
    if(!GTfileName.isEmpty()) {
        QImage image;
        if(!(image.load(GTfileName))) {
            QMessageBox::information(this, "warning",
                                     "can't open image");
            return;
        }
        QString fileName = getFileName();
        QImage imageIn;
        if(!imageIn.load(fileName)) {
            QMessageBox::information(this, "warnning", "can't open file");
            return;
        }
        VVV rgb = imageToRGBArray(imageIn);
        Point dimension = getImageDimension(imageIn);

        QString str = ui->tableWidget->item(0, 1)->text();
        int k = str.toInt();
        str = ui->tableWidget->item(1, 1)->text();
        double ifspatial = str.toDouble();
        str = ui->tableWidget->item(2, 1)->text();
        double w1 = str.toDouble();
        str = ui->tableWidget->item(3, 1)->text();
        double w2 = str.toDouble();
        str = ui->tableWidget->item(4, 1)->text();
        double w3 = str.toDouble();
        Kmeans kmean(k, ifspatial, rgb, dimension, w1, w2, w3);
        VVV newrgb = kmean.getRGBArray();

        double accuracy = getAccuracy(k, dimension, kmean.getLabel(), getGtLabel(k, image));

        double Je = kmean.getJe();
        double dmax = kmean.getDmax();
        double dmin = kmean.getDmin();
        double fittness = kmean.getFittness();
        QImage imageOut = RGBArrayToimage(newrgb, imageIn);
        showImage(&imageOut);
        addAccuracyToAna(Je, dmax, dmin, fittness, accuracy);
    }
}

void MainWindow::CV_kMeans() {
    QString str = ui->tableWidget->item(0, 1)->text();
    int k = str.toInt();

    QString fileName = getFileName();
    Mat srcImage = imread(fileName.toStdString());
        if (!srcImage.data)
        {
            printf("could not load image...\n");
            return;
        }
//        imshow("original", srcImage);

        //五个颜色，聚类之后的颜色随机从这里面选择
        Scalar colorTab[] = {
            Scalar(0,0,255),
            Scalar(0,255,0),
            Scalar(255,0,0),
            Scalar(0,255,255),
            Scalar(255,0,255)
        };

        int width = srcImage.cols;//图像的宽
        int height = srcImage.rows;//图像的高
        int channels = srcImage.channels();//图像的通道数

        //初始化一些定义
        int sampleCount = width*height;//所有的像素
        int clusterCount = k;//分类数
        Mat points(sampleCount, channels, CV_32F, Scalar(10));//points用来保存所有的数据
        Mat labels;//聚类后的标签
        Mat center(clusterCount, 1, points.type());//聚类后的类别的中心

        //将图像的RGB像素转到到样本数据
        int index;
        for (int i = 0; i < srcImage.rows; i++)
        {
            for (int j = 0; j < srcImage.cols; j++)
            {
                index = i*width + j;
                Vec3b bgr = srcImage.at<Vec3b>(i, j);
                //将图像中的每个通道的数据分别赋值给points的值
                points.at<float>(index, 0) = static_cast<int>(bgr[0]);
                points.at<float>(index, 1) = static_cast<int>(bgr[1]);
                points.at<float>(index, 2) = static_cast<int>(bgr[2]);
            }
        }
        //运行K-means算法
        //MAX_ITER也可以称为COUNT最大迭代次数，EPS最高精度,10表示最大的迭代次数，0.1表示结果的精确度
        TermCriteria criteria = TermCriteria(TermCriteria::EPS + TermCriteria::COUNT,10,0.1);
        kmeans(points, clusterCount, labels, criteria, 3, KMEANS_PP_CENTERS, center);

        //显示图像分割结果
        Mat result = Mat::zeros(srcImage.size(), srcImage.type());//创建一张结果图

        QImage imageIn;
        if(!imageIn.load(fileName)) {
            QMessageBox::information(this, "warnning", "can't open file");
            return;
        }
        VVV newrgb = imageToRGBArray(imageIn);
        for (int i = 0; i < srcImage.rows; i++)
        {
            for (int j = 0; j < srcImage.cols; j++)
            {
                index = i*width + j;
                int label = labels.at<int>(index);//每一个像素属于哪个标签
                result.at<Vec3b>(i, j)[0] = colorTab[label][0];//对结果图中的每一个通道进行赋值
                result.at<Vec3b>(i, j)[1] = colorTab[label][1];
                result.at<Vec3b>(i, j)[2] = colorTab[label][2];
                newrgb[1][j][i] = colorTab[label][2];
                newrgb[2][j][i] = colorTab[label][1];
                newrgb[3][j][i] = colorTab[label][0];
            }
        }
//        imshow("Kmeans", result);

//        waitKey(0);


        QImage imageOut = RGBArrayToimage(newrgb, imageIn);
        showImage(&imageOut);

}

void MainWindow::GT_CV_Kmeans() {
    QString str = ui->tableWidget->item(0, 1)->text();
    int k = str.toInt();

    QString fileName = getFileName();
    Mat srcImage = imread(fileName.toStdString());
        if (!srcImage.data)
        {
            printf("could not load image...\n");
            return;
        }
//        imshow("original", srcImage);

        //五个颜色，聚类之后的颜色随机从这里面选择
        Scalar colorTab[] = {
            Scalar(0,0,255),
            Scalar(0,255,0),
            Scalar(255,0,0),
            Scalar(0,255,255),
            Scalar(255,0,255)
        };

        int width = srcImage.cols;//图像的宽
        int height = srcImage.rows;//图像的高
        int channels = srcImage.channels();//图像的通道数

        //初始化一些定义
        int sampleCount = width*height;//所有的像素
        int clusterCount = k;//分类数
        Mat points(sampleCount, channels, CV_32F, Scalar(10));//points用来保存所有的数据
        Mat labels;//聚类后的标签
        Mat center(clusterCount, 1, points.type());//聚类后的类别的中心

        //将图像的RGB像素转到到样本数据
        int index;
        for (int i = 0; i < srcImage.rows; i++)
        {
            for (int j = 0; j < srcImage.cols; j++)
            {
                index = i*width + j;
                Vec3b bgr = srcImage.at<Vec3b>(i, j);
                //将图像中的每个通道的数据分别赋值给points的值
                points.at<float>(index, 0) = static_cast<int>(bgr[0]);
                points.at<float>(index, 1) = static_cast<int>(bgr[1]);
                points.at<float>(index, 2) = static_cast<int>(bgr[2]);
            }
        }
        //运行K-means算法
        //MAX_ITER也可以称为COUNT最大迭代次数，EPS最高精度,10表示最大的迭代次数，0.1表示结果的精确度
        TermCriteria criteria = TermCriteria(TermCriteria::EPS + TermCriteria::COUNT,10,0.1);
        kmeans(points, clusterCount, labels, criteria, 3, KMEANS_PP_CENTERS, center);

        //显示图像分割结果
        Mat result = Mat::zeros(srcImage.size(), srcImage.type());//创建一张结果图

        QImage imageIn;
        if(!imageIn.load(fileName)) {
            QMessageBox::information(this, "warnning", "can't open file");
            return;
        }
        VVV newrgb = imageToRGBArray(imageIn);
        for (int i = 0; i < srcImage.rows; i++)
        {
            for (int j = 0; j < srcImage.cols; j++)
            {
                index = i*width + j;
                int label = labels.at<int>(index);//每一个像素属于哪个标签
                result.at<Vec3b>(i, j)[0] = colorTab[label][0];//对结果图中的每一个通道进行赋值
                result.at<Vec3b>(i, j)[1] = colorTab[label][1];
                result.at<Vec3b>(i, j)[2] = colorTab[label][2];
                newrgb[1][j][i] = colorTab[label][2];
                newrgb[2][j][i] = colorTab[label][1];
                newrgb[3][j][i] = colorTab[label][0];
            }
        }
//        imshow("Kmeans", result);

//        waitKey(0);

        QString GTfileName = QFileDialog::getOpenFileName(this,
                                                        tr("select image"),
                                                        "",
                                                        tr("Images (*.png *.bmp *.jpg *.tif *.GIF)"));
        if(!GTfileName.isEmpty()) {
            QImage image;
            if(!(image.load(GTfileName))) {
                QMessageBox::information(this, "warning",
                                         "can't open image");
                return;
            }
            QImage imageOut = RGBArrayToimage(newrgb, imageIn);
            double accuracy = getAccuracy(k, getImageDimension(image), getGtLabel(k, imageOut), getGtLabel(k, image));
            cout << accuracy << endl;
            showImage(&imageOut);
            addAccuracyToAna(0, 0, 0, 0, accuracy);
        }
        else {
            QImage imageOut = RGBArrayToimage(newrgb, imageIn);
            showImage(&imageOut);
        }
}

void MainWindow::CV_enhancement(){
    QString fileName = getFileName();
    Mat srcImage = imread(fileName.toStdString());
        if (srcImage.empty())
        {
            std::cout << "failed to open image" << std::endl;
        }
        Mat imageEnhance;
        Mat kernel = (Mat_<float>(3, 3) << 0, -1, 0, 0, 5, 0, 0, -1, 0);
        filter2D(srcImage, imageEnhance, CV_8UC3, kernel);


        //imshow("enhancement", imageEnhance);

        QImage imageIn;
        if(!imageIn.load(fileName)) {
            QMessageBox::information(this, "warnning", "can't open file");
            return;
        }

        VVV newrgb = imageToRGBArray(imageIn);
        for (int i = 0; i < srcImage.rows; i++)
        {
            for (int j = 0; j < srcImage.cols; j++)
            {
                newrgb[1][j][i] = (int)imageEnhance.at<Vec3b>(i, j)[2];
                newrgb[2][j][i] = (int)imageEnhance.at<Vec3b>(i, j)[1];
                newrgb[3][j][i] = (int)imageEnhance.at<Vec3b>(i, j)[0];

            }
        }
//        imshow("Kmeans", result);

//        waitKey(0);

        QImage imageOut = RGBArrayToimage(newrgb, imageIn);
        showImage(&imageOut);
}

void MainWindow::CV_enhance_histo(){
    QString fileName = getFileName();
    Mat srcImage = imread(fileName.toStdString());
        if (srcImage.empty())
        {
            std::cout << "failed to open image" << std::endl;
        }
        Mat imageRGB[3];
        split(srcImage, imageRGB);
        for (int i = 0; i < 3; i++)
        {
            equalizeHist(imageRGB[i], imageRGB[i]);
        }
        merge(imageRGB, 3, srcImage);

        QImage imageIn;
        if(!imageIn.load(fileName)) {
            QMessageBox::information(this, "warnning", "can't open file");
            return;
        }

        VVV newrgb = imageToRGBArray(imageIn);
        for (int i = 0; i < srcImage.rows; i++)
        {
            for (int j = 0; j < srcImage.cols; j++)
            {
                newrgb[1][j][i] = (int)srcImage.at<Vec3b>(i, j)[2];
                newrgb[2][j][i] = (int)srcImage.at<Vec3b>(i, j)[1];
                newrgb[3][j][i] = (int)srcImage.at<Vec3b>(i, j)[0];

            }
        }

        QImage imageOut = RGBArrayToimage(newrgb, imageIn);
        showImage(&imageOut);
}

void MainWindow::CV_enhance_log(){
    QString fileName = getFileName();
    Mat srcImage = imread(fileName.toStdString());
        Mat imageLog(srcImage.size(), CV_32FC3);
        for (int i = 0; i < srcImage.rows; i++)
        {
            for (int j = 0; j < srcImage.cols; j++)
            {
                imageLog.at<Vec3f>(i, j)[0] = log(1 + srcImage.at<Vec3b>(i, j)[0]);
                imageLog.at<Vec3f>(i, j)[1] = log(1 + srcImage.at<Vec3b>(i, j)[1]);
                imageLog.at<Vec3f>(i, j)[2] = log(1 + srcImage.at<Vec3b>(i, j)[2]);
            }
        }
        //归一化到0~255
        normalize(imageLog, imageLog, 0, 255, NORM_MINMAX);
        //转换成8bit图像显示
        convertScaleAbs(imageLog, imageLog);

        QImage imageIn;
        if(!imageIn.load(fileName)) {
            QMessageBox::information(this, "warnning", "can't open file");
            return;
        }

        VVV newrgb = imageToRGBArray(imageIn);
        for (int i = 0; i < srcImage.rows; i++)
        {
            for (int j = 0; j < srcImage.cols; j++)
            {
                newrgb[1][j][i] = (int)imageLog.at<Vec3b>(i, j)[2];
                newrgb[2][j][i] = (int)imageLog.at<Vec3b>(i, j)[1];
                newrgb[3][j][i] = (int)imageLog.at<Vec3b>(i, j)[0];

            }
        }

        QImage imageOut = RGBArrayToimage(newrgb, imageIn);
        showImage(&imageOut);
}

/*********************************************/
/*********************************************/
/*********************************************/


/*******************************************
 *
 *      T-means
 *
 * ****************************************/

void MainWindow::tMeans() {
    QString fileName = getFileName();
    QImage imageIn;
    if(!imageIn.load(fileName)) {
        QMessageBox::information(this, "warnning", "can't open file");
        return;
    }
    VVV rgb = imageToRGBArray(imageIn);
    Point dimension = getImageDimension(imageIn);
    QString str = ui->tableWidget->item(0, 1)->text();
    int k = str.toInt();
    str = ui->tableWidget->item(1, 1)->text();
    double t = str.toDouble();
    str = ui->tableWidget->item(2, 1)->text();
    double ifspatial = str.toDouble();
    str = ui->tableWidget->item(3, 1)->text();
    double w1 = str.toDouble();
    str = ui->tableWidget->item(4, 1)->text();
    double w2 = str.toDouble();
    str = ui->tableWidget->item(5, 1)->text();
    double w3 = str.toDouble();
    tmeans tm(k, t, dimension.x, dimension.y, rgb, ifspatial, w1, w2, w3);
    vector<vector<int>> label = tm.getLabel();
    VVV newrgb = setRGBArrayFromLabel(label);
    QImage imageOut = RGBArrayToimage(newrgb, imageIn);
    showImage(&imageOut);
    double Je = tm.getJe();
    double dmax = tm.getDmax();
    double dmin = tm.getDmin();
    double fittness = tm.getFitness();
    double accuracy = tm.getAccuracy();
    addAccuracyToAna(Je, dmax, dmin, fittness, accuracy);
}

/*********************************************/
/*********************************************/
/*********************************************/


/*******************************************
 *
 *      PBA
 *
 * ****************************************/

void MainWindow::PBA() {
    QString fileName = getFileName();
    QImage imageIn;
    if(!imageIn.load(fileName)) {
        QMessageBox::information(this, "warnning", "can't open file");
        return;
    }
    VVV rgb = imageToRGBArray(imageIn);
    Point dimension = getImageDimension(imageIn);

    QString str = ui->tableWidget->item(0, 1)->text();
    int C = str.toInt();
    str = ui->tableWidget->item(1, 1)->text();
    int D = str.toInt();
    str = ui->tableWidget->item(2, 1)->text();
    double spatial = str.toDouble();
    str = ui->tableWidget->item(3, 1)->text();
    double w1 = str.toDouble();
    str = ui->tableWidget->item(4, 1)->text();
    double w2 = str.toDouble();
    str = ui->tableWidget->item(5, 1)->text();
    double w3 = str.toDouble();
    myPba pba(C, D, rgb, dimension, spatial, w1, w2, w3);
    VVV newrgb = pba.getRGBArray();
    QImage imageOut = RGBArrayToimage(newrgb, imageIn);
    showImage(&imageOut);
    double Je = pba.getJe();
    double dmax = pba.getDmax();
    double dmin = pba.getDmin();
    double fittness = pba.getFitness();
    setAnaTable(Je, dmax, dmin, fittness);
}

void MainWindow::GTPBA(){
    QString GTfileName = QFileDialog::getOpenFileName(this,
                                                    tr("select image"),
                                                    "",
                                                    tr("Images (*.png *.bmp *.jpg *.tif *.GIF)"));
    if(!GTfileName.isEmpty()) {
        QImage image;
        if(!(image.load(GTfileName))) {
            QMessageBox::information(this, "warning",
                                     "can't open image");
            return;
        }
        QString fileName = getFileName();
        QImage imageIn;
        if(!imageIn.load(fileName)) {
            QMessageBox::information(this, "warnning", "can't open file");
            return;
        }
        VVV rgb = imageToRGBArray(imageIn);
        Point dimension = getImageDimension(imageIn);

        QString str = ui->tableWidget->item(0, 1)->text();
        int C = str.toInt();
        str = ui->tableWidget->item(1, 1)->text();
        int D = str.toInt();
        str = ui->tableWidget->item(2, 1)->text();
        double spatial = str.toDouble();
        str = ui->tableWidget->item(3, 1)->text();
        double w1 = str.toDouble();
        str = ui->tableWidget->item(4, 1)->text();
        double w2 = str.toDouble();
        str = ui->tableWidget->item(5, 1)->text();
        double w3 = str.toDouble();
        myPba pba(C, D, rgb, dimension, spatial, w1, w2, w3);
        VVV newrgb = pba.getRGBArray();

        double accuracy = getAccuracy(C, dimension, pba.getLabel(), getGtLabel(C, image));

        QImage imageOut = RGBArrayToimage(newrgb, imageIn);
        showImage(&imageOut);
        double Je = pba.getJe();
        double dmax = pba.getDmax();
        double dmin = pba.getDmin();
        double fittness = pba.getFitness();
        addAccuracyToAna(Je, dmax, dmin, fittness, accuracy);
    }
}

/*********************************************/
/*********************************************/
/*********************************************/


/*******************************************
 *
 *      PBA-K-means
 *
 * ****************************************/

void MainWindow::PBAkMeans() {
    QString fileName = getFileName();
    QImage imageIn;
    if(!imageIn.load(fileName)) {
        QMessageBox::information(this, "warnning", "can't open file");
        return;
    }
    VVV rgb = imageToRGBArray(imageIn);
    Point dimension = getImageDimension(imageIn);

    QString str = ui->tableWidget->item(0, 1)->text();
    int C = str.toInt();
    str = ui->tableWidget->item(1, 1)->text();
    int D = str.toInt();
    str = ui->tableWidget->item(2, 1)->text();
    double spatial = str.toDouble();
    str = ui->tableWidget->item(3, 1)->text();
    double w1 = str.toDouble();
    str = ui->tableWidget->item(4, 1)->text();
    double w2 = str.toDouble();
    str = ui->tableWidget->item(5, 1)->text();
    double w3 = str.toDouble();
    PbaKmeans pk(C, D, rgb, dimension, spatial, w1, w2, w3);
    VVV newrgb = pk.getRGBArray();
    QImage imageOut = RGBArrayToimage(newrgb, imageIn);
    showImage(&imageOut);
    double Je = pk.getJe();
    double dmax = pk.getDmax();
    double dmin = pk.getDmin();
    double fittness = pk.getFitness();
    setAnaTable(Je, dmax, dmin, fittness);
}



void MainWindow::GTPBAkMeans() {
    //name of path
    QString GTfileName = QFileDialog::getOpenFileName(this,
                                                    tr("select image"),
                                                    "",
                                                    tr("Images (*.png *.bmp *.jpg *.tif *.GIF)"));
    if(!GTfileName.isEmpty()) {
        QImage image;
        if(!(image.load(GTfileName))) {
            QMessageBox::information(this, "warning",
                                     "can't open image");
            return;
        }
        QString fileName = getFileName();
        QImage imageIn;
        if(!imageIn.load(fileName)) {
            QMessageBox::information(this, "warnning", "can't open file");
            return;
        }
        VVV rgb = imageToRGBArray(imageIn);
        Point dimension = getImageDimension(imageIn);

        QString str = ui->tableWidget->item(0, 1)->text();
        int C = str.toInt();
        str = ui->tableWidget->item(1, 1)->text();
        int D = str.toInt();
        str = ui->tableWidget->item(2, 1)->text();
        double spatial = str.toDouble();
        str = ui->tableWidget->item(3, 1)->text();
        double w1 = str.toDouble();
        str = ui->tableWidget->item(4, 1)->text();
        double w2 = str.toDouble();
        str = ui->tableWidget->item(5, 1)->text();
        double w3 = str.toDouble();
        PbaKmeans pk(C, D, rgb, dimension, spatial, w1, w2, w3);
        VVV newrgb = pk.getRGBArray();

        double accuracy = getAccuracy(C, dimension, pk.getLabel(), getGtLabel(C, image));

        QImage imageOut = RGBArrayToimage(newrgb, imageIn);
        showImage(&imageOut);
        double Je = pk.getJe();
        double dmax = pk.getDmax();
        double dmin = pk.getDmin();
        double fittness = pk.getFitness();
        addAccuracyToAna(Je, dmax, dmin, fittness, accuracy);
    }
}

void MainWindow::SU_PBAkMeans() {
    //name of path
    QString GTfileName = QFileDialog::getOpenFileName(this,
                                                    tr("select image"),
                                                    "",
                                                    tr("Images (*.png *.bmp *.jpg *.tif *.GIF)"));
    if(!GTfileName.isEmpty()) {
        QImage image;
        if(!(image.load(GTfileName))) {
            QMessageBox::information(this, "warning",
                                     "can't open image");
            return;
        }
        QString fileName = getFileName();
        QImage imageIn;
        if(!imageIn.load(fileName)) {
            QMessageBox::information(this, "warnning", "can't open file");
            return;
        }
        VVV rgb = imageToRGBArray(imageIn);
        Point dimension = getImageDimension(imageIn);

        QString str = ui->tableWidget->item(0, 1)->text();
        int C = str.toInt();
        str = ui->tableWidget->item(1, 1)->text();
        int D = str.toInt();
        str = ui->tableWidget->item(2, 1)->text();
        double spatial = str.toDouble();
        str = ui->tableWidget->item(3, 1)->text();
        double w1 = str.toDouble();
        str = ui->tableWidget->item(4, 1)->text();
        double w2 = str.toDouble();
        str = ui->tableWidget->item(5, 1)->text();
        double w3 = str.toDouble();
        SuedPbaKmeans pk(C, D, rgb, dimension, spatial, w1, w2, w3, getGtLabel(C, image));
        VVV newrgb = pk.getRGBArray();

        QImage imageOut = RGBArrayToimage(newrgb, imageIn);
        showImage(&imageOut);
        double Je = pk.getJe();
        double dmax = pk.getDmax();
        double dmin = pk.getDmin();
        double fittness = pk.getFitness();
        double accuracy = pk.getAccuracy();
        addAccuracyToAna(Je, dmax, dmin, fittness, accuracy);
    }
}



/*********************************************/
/*********************************************/
/*********************************************/




/*******************************************
 *
 *      PSO-K-means
 *
 * ****************************************/

void MainWindow::PSOkMeans() {
    QString fileName = getFileName();
    QImage imageIn;
    if(!imageIn.load(fileName)) {
        QMessageBox::information(this, "warnning", "can't open file");
        return;
    }
    VVV rgb = imageToRGBArray(imageIn);
    Point dimension = getImageDimension(imageIn);

    QString str = ui->tableWidget->item(0, 1)->text();
    int C = str.toInt();
    str = ui->tableWidget->item(1, 1)->text();
    int S = str.toInt();
    str = ui->tableWidget->item(2, 1)->text();
    int tmax = str.toInt();
    str = ui->tableWidget->item(3, 1)->text();
    double spatial = str.toDouble();
    str = ui->tableWidget->item(4, 1)->text();
    double w1 = str.toDouble();
    str = ui->tableWidget->item(5, 1)->text();
    double w2 = str.toDouble();
    str = ui->tableWidget->item(6, 1)->text();
    double w3 = str.toDouble();
    str = ui->tableWidget->item(7, 1)->text();
    double w = str.toDouble();
    str = ui->tableWidget->item(8, 1)->text();
    double c1 = str.toDouble();
    str = ui->tableWidget->item(9, 1)->text();
    double c2 = str.toDouble();
    psokmeans ps(C, S, tmax, rgb, dimension, spatial, w1, w2, w3, w, c1, c2);
    VVV newrgb = ps.getRGBArray();
    QImage imageOut = RGBArrayToimage(newrgb, imageIn);
    showImage(&imageOut);
    double Je = ps.getJe();
    double dmax = ps.getDmax();
    double dmin = ps.getDmin();
    double fittness = ps.getFitness();
    setAnaTable(Je, dmax, dmin, fittness);
}

/*********************************************/
/*********************************************/
/*********************************************/


/*******************************************
 *
 *      ACO-K-means
 *
 * ****************************************/

void MainWindow::ACOkMeans() {

    QString fileName = getFileName();
    QImage imageIn;
    if(!imageIn.load(fileName)) {
        QMessageBox::information(this, "warnning", "can't open file");
        return;
    }
    VVV rgb = imageToRGBArray(imageIn);
    Point dimension = getImageDimension(imageIn);

    QString str = ui->tableWidget->item(0, 1)->text();
    int k = str.toInt();
    str = ui->tableWidget->item(1, 1)->text();
    int m = str.toInt();
    str = ui->tableWidget->item(2, 1)->text();
    double spatial = str.toDouble();
    str = ui->tableWidget->item(3, 1)->text();
    double w1 = str.toDouble();
    str = ui->tableWidget->item(4, 1)->text();
    double w2 = str.toDouble();
    str = ui->tableWidget->item(5, 1)->text();
    double w3 = str.toDouble();
    str = ui->tableWidget->item(6, 1)->text();
    double Q = str.toDouble();
    str = ui->tableWidget->item(7, 1)->text();
    double karma = str.toDouble();
    str = ui->tableWidget->item(8, 1)->text();
    double p = str.toDouble();
    str = ui->tableWidget->item(9, 1)->text();
    double alpha = str.toDouble();
    str = ui->tableWidget->item(10, 1)->text();
    double beta = str.toDouble();
    AcoKmeans ak(k, m, rgb, dimension, spatial, w1, w2, w3, Q, karma, p, alpha, beta);
    VVV newrgb = ak.getRGBArray();
    QImage imageOut = RGBArrayToimage(newrgb, imageIn);
    showImage(&imageOut);
    double Je = ak.getJe();
    double dmax = ak.getDmax();
    double dmin = ak.getDmin();
    double fittness = ak.getFitness();
    setAnaTable(Je, dmax, dmin, fittness);

}


/*********************************************/
/*********************************************/
/*********************************************/


/*******************************************
 *
 *      GA-K-means
 *
 * ****************************************/

void MainWindow::GAkMeans() {

    QString fileName = getFileName();
    QImage imageIn;
    if(!imageIn.load(fileName)) {
        QMessageBox::information(this, "warnning", "can't open file");
        return;
    }
    VVV rgb = imageToRGBArray(imageIn);
    Point dimension = getImageDimension(imageIn);

    QString str = ui->tableWidget->item(0, 1)->text();
    int k = str.toInt();
    str = ui->tableWidget->item(1, 1)->text();
    int population = str.toInt();
    str = ui->tableWidget->item(2, 1)->text();
    double spatial = str.toDouble();
    str = ui->tableWidget->item(3, 1)->text();
    double w1 = str.toDouble();
    str = ui->tableWidget->item(4, 1)->text();
    double w2 = str.toDouble();
    str = ui->tableWidget->item(5, 1)->text();
    double w3 = str.toDouble();
    str = ui->tableWidget->item(6, 1)->text();
    double pc = str.toDouble();
    str = ui->tableWidget->item(7, 1)->text();
    double pm = str.toDouble();
    GaKmeans gk(k, population, rgb, dimension, spatial, w1, w2, w3, pc, pm);
    VVV newrgb = gk.getRGBArray();
    QImage imageOut = RGBArrayToimage(newrgb, imageIn);
    showImage(&imageOut);
    double Je = gk.getJe();
    double dmax = gk.getDmax();
    double dmin = gk.getDmin();
    double fittness = gk.getFitness();
    setAnaTable(Je, dmax, dmin, fittness);

}


/*********************************************/
/*********************************************/
/*********************************************/

/*******************************************
 *
 *      SA-K-means
 *
 * ****************************************/

void MainWindow::SAkMeans() {

    QString fileName = getFileName();
    QImage imageIn;
    if(!imageIn.load(fileName)) {
        QMessageBox::information(this, "warnning", "can't open file");
        return;
    }
    VVV rgb = imageToRGBArray(imageIn);
    Point dimension = getImageDimension(imageIn);

    QString str = ui->tableWidget->item(0, 1)->text();
    int k = str.toInt();
    str = ui->tableWidget->item(1, 1)->text();
    double E = str.toDouble();
    str = ui->tableWidget->item(2, 1)->text();
    double spatial = str.toDouble();
    str = ui->tableWidget->item(3, 1)->text();
    double w1 = str.toDouble();
    str = ui->tableWidget->item(4, 1)->text();
    double w2 = str.toDouble();
    str = ui->tableWidget->item(5, 1)->text();
    double w3 = str.toDouble();
    SaKmeans sk(k, E, rgb, dimension, spatial, w1, w2, w3);
    VVV newrgb = sk.getRGBArray();
    QImage imageOut = RGBArrayToimage(newrgb, imageIn);
    showImage(&imageOut);
    double Je = sk.getJe();
    double dmax = sk.getDmax();
    double dmin = sk.getDmin();
    double fittness = sk.getFitness();
    setAnaTable(Je, dmax, dmin, fittness);

}



/*******************************************
 *
 *      Hyperspectral image
 *
 * ****************************************/


void MainWindow::on_actionOpen_hyperspectral_image_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("select image"),
                                                    "",
                                                    tr("Images (*.png *.bmp *.jpg *.tif *.GIF *.mat)"));
    if(!fileName.isEmpty()) {

        MATFile* inputImage;
        mxArray* pMxArray;
        string stdString = fileName.toStdString();
        char charFileName[100];
        for(int i = 0; i < stdString.size(); i++) {
            charFileName[i] = stdString[i];
        }
        cout << strlen(charFileName) << endl;
        inputImage = matOpen(charFileName, "r");
        if(!inputImage) {
            cout << "unable to open the file" << endl;
            return;
        }
        int nvars;
        int M, N, C;
        const char** var_names = (const char**)matGetDir(inputImage, &nvars);
        pMxArray = matGetVariable(inputImage, var_names[0]);

//        QImage* image = new QImage;
//        if(!(image->load(fileName))) {
//            QMessageBox::information(this, "warning",
//                                     "can't open image");
//            return;
//        }
//        //get name of file
//        /****************************************/
//        QString name; //name of file
//        for(int i = fileName.length(); i >= 0; i--) {
//            if(fileName[i] == '/') break;
//            name.insert(0, fileName[i]);
//        }
//        /****************************************/

//        //the number of images in a tab
//        //i is the row of listwidget
//        tabImageNum.push_back(1);

//        //add an index to the same image of different tab
//        /****************************************/
//        int cnt = 0;
//        for(int i = 0; i < listItems.size(); i++) {
//            if(fileName == listItems[i]) cnt++;
//        }
//        if(cnt) {
//            QString temp = '(' + QString::number(cnt) + ')';
//            int i = 0;
//            for(i = name.length(); i >= 0; i--) {
//                if(name[i] == '.') {
//                    break;
//                }
//            }
//            for(int j = 0; j < temp.length(); j++) {
//                name.insert(i, temp[j]);
//                i++;
//            }
//            for(int e = 0; e < ui->listWidget->count(); e++) {
//                if(name == ui->listWidget->item(e)->text()) {
//                    QString temp = '(' + QString::number(cnt) + ')';
//                    int i = 0;
//                    for(i = name.length(); i >= 0; i--) {
//                        if(name[i] == '.') {
//                            break;
//                        }
//                    }
//                    for(int j = 0; j < temp.length(); j++) {
//                        name.insert(i, temp[j]);
//                        i++;
//                    }
//                    e = 0;
//                }
//            }
//        }
//        /*****************************************/

//        //store the path of an image
//        listItems.push_back(fileName);
//        //add image name to listwidget
//        ui->listWidget->addItem(name);

//        //show image in a tab using scrollarea
//        /****************************************/
//        QScrollArea *scroll = new QScrollArea;
//        scroll->setWidgetResizable(true);
//        QWidget *wid = new QWidget;
//        QGridLayout *layout = new QGridLayout;

//        wid->setLayout(layout);
//        scroll->setWidget(wid);

//        QLabel *label = new QLabel(wid);
//        label->setPixmap(QPixmap::fromImage(*image));
//        label->setAlignment(Qt::AlignLeft);

//        layout->addWidget(label);
//        ui->tabWidget->addTab(scroll, name);
//        ui->tabWidget->setCurrentWidget(scroll);
//        /****************************************/

//        //give each tab a layout
//        la.push_back(layout);
//        //save each label of a tab
//        vector<QLabel*> v;
//        v.push_back(label);
//        tabLabel.push_back(v);
    }
}

/*********************************************/
/*********************************************/
/*********************************************/



