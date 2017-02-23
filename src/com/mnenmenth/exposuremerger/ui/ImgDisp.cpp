/**
  * Created by Mnenmenth Alkaborin
  * Please refer to LICENSE file if included
  * for licensing information
  * https://github.com/Mnenmenth
  */

#include "ImgDisp.h"
#include <QtWidgets/QFileDialog>

ImgDisp::ImgDisp() {

    debevecImg = new QLabel();
    debevecSaveButton = new QPushButton("Save Debevec Merge");

    tonemapImg = new QLabel();
    tonemapSaveButton = new QPushButton("Save Tonemap Merge");

    mertensImg = new QLabel();
    mertensSaveButton = new QPushButton("Save Mertens Merge");

    imgList = new QTableWidget();

    addImgsButton = new QPushButton("Add Images");

    mergeButton = new QPushButton("Merge Merged Images");

    mainLayout = new QVBoxLayout();
    mainLayout->setAlignment(Qt::AlignCenter);
    this->setLayout(mainLayout);

    topGroup = new QGroupBox();
    topLayout = new QHBoxLayout();
    topLayout->setAlignment(Qt::AlignCenter);
    topGroup->setLayout(topLayout);

    bottomGroup = new QGroupBox();
    bottomLayout = new QHBoxLayout();
    bottomLayout->setAlignment(Qt::AlignCenter);
    bottomGroup->setLayout(bottomLayout);

    debevecGroup = new QGroupBox();
    debevecLayout = new QVBoxLayout();
    debevecLayout->setAlignment(Qt::AlignCenter);
    debevecGroup->setLayout(debevecLayout);

    tonemapGroup = new QGroupBox();
    tonemapLayout = new QVBoxLayout();
    tonemapLayout->setAlignment(Qt::AlignCenter);
    tonemapGroup->setLayout(tonemapLayout);

    mertensGroup = new QGroupBox();
    mertensLayout = new QVBoxLayout();
    mertensLayout->setAlignment(Qt::AlignCenter);
    mertensGroup->setLayout(mertensLayout);

    mainLayout->addWidget(topGroup);
    mainLayout->addWidget(imgList);
    mainLayout->addWidget(bottomGroup);

    topLayout->addWidget(addImgsButton);
    topLayout->addWidget(mergeButton);

    bottomLayout->addWidget(debevecGroup);
    bottomLayout->addWidget(tonemapGroup);
    bottomLayout->addWidget(mertensGroup);

    debevecLayout->addWidget(debevecSaveButton);
    debevecLayout->addWidget(debevecImg);

    tonemapLayout->addWidget(tonemapSaveButton);
    tonemapLayout->addWidget(tonemapImg);

    mertensLayout->addWidget(mertensSaveButton);
    mertensLayout->addWidget(mertensImg);

    connect(addImgsButton, SIGNAL(released()), this, SLOT(addImgs()));
    connect(mergeButton, SIGNAL(released()), this, SLOT(mergeImgs()));
    connect(debevecSaveButton, SIGNAL(released()), this, SLOT(debevecSave()));
    connect(tonemapSaveButton, SIGNAL(released()), this, SLOT(tonemapSave()));
    connect(mertensSaveButton, SIGNAL(released()), this, SLOT(mertensSave()));

    imgList->setRowCount(0);
    imgList->setColumnCount(3);
}

ImgDisp::~ImgDisp() {
    delete debevecImg;
    delete debevecSaveButton;
    //delete debevecMat;
    delete tonemapImg;
    delete tonemapSaveButton;
    //delete tonemapMat;
    delete mertensImg;
    delete mertensSaveButton;
    //delete mertensMat;
    delete imgList;
    delete addImgsButton;
    delete mergeButton;
}

void ImgDisp::addImgs() {

    QString filter = tr("Image (*.jpg *.jpeg *.png *.tif *.bmp)");
    QStringList filepaths = QFileDialog::getOpenFileNames(this, tr("Images to merge"), QDir::homePath(), tr("Image (*.jpg *.jpeg *.png *.tif *.bmp);;All files(*.*)"), &filter);
    if(!filepaths.empty()) {
        for(QString file : filepaths) {
            int rowNum = imgList->rowCount();
            imgList->insertRow(rowNum);
            QTableWidgetItem* img = new QTableWidgetItem(file);
            QTableWidgetItem* time = new QTableWidgetItem(tr("0"));
            QPushButton* remove = new QPushButton("Remove Img");
            connect(remove, SIGNAL(released()), this, SLOT(removeRow(rowNum, img, time, remove)));
            imgList->setItem(rowNum, 0, img);
            imgList->setItem(rowNum, 1, time);
            imgList->setCellWidget(rowNum, 2, remove);
            imgList->resizeRowToContents(rowNum);
        }
    }

}

void ImgDisp::removeRow(int rowNum, QTableWidgetItem* img, QTableWidgetItem* time, QPushButton* remove) {
    imgList->removeRow(rowNum);
    delete img;
    delete time;
    delete remove;
}

void ImgDisp::mergeImgs() {
    std::vector<std::string> files;
    std::vector<float> times;
    for(int i = 0; i <= imgList->rowCount()-1; i++) {
        std::string img = imgList->itemAt(i, 0)->text().toStdString();
        bool* ok = new bool(true);
        float time  = imgList->itemAt(i, 1)->text().toFloat();
        if(*ok) {
            files.push_back(img);
            times.push_back(time);
        } else {
            std::cout << "Time conversion error at row: " << i << std::endl;
        }
    }
    MergeExposures::merge(files, times, debevecMat, tonemapMat, mertensMat);
    Mat2QLabel(debevecMat, debevecImg);
    Mat2QLabel(tonemapMat, tonemapImg);
    Mat2QLabel(mertensMat, mertensImg);
}

void ImgDisp::Mat2QLabel(cv::Mat *src, QLabel* dest) {
    cv::Mat temp;
    src->copyTo(temp);
    dest->setPixmap(QPixmap::fromImage(QImage(temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB32)));
    dest->adjustSize();
}

void ImgDisp::debevecSave() {
    QString filter = tr("Image (*.jpg *.jpeg *.png *.tif *.bmp)");
    std::string path = QFileDialog::getSaveFileName(this, tr("Save File As"), QDir::homePath(), tr("Image (*.jpg *.jpeg *.png *.tif *.bmp);;All files(*.*)"), &filter).toStdString();
    cv::imwrite(path, *debevecMat * 255);
}

void ImgDisp::tonemapSave() {
    QString filter = tr("Image (*.jpg *.jpeg *.png *.tif *.bmp)");
    std::string path = QFileDialog::getSaveFileName(this, tr("Save File As"), QDir::homePath(), tr("Image (*.jpg *.jpeg *.png *.tif *.bmp);;All files(*.*)"), &filter).toStdString();
    cv::imwrite(path, *tonemapMat * 255);
}

void ImgDisp::mertensSave() {
    QString filter = tr("Image (*.jpg *.jpeg *.png *.tif *.bmp)");
    std::string path = QFileDialog::getSaveFileName(this, tr("Save File As"), QDir::homePath(), tr("Image (*.jpg *.jpeg *.png *.tif *.bmp);;All files(*.*)"), &filter).toStdString();
    cv::imwrite(path, *mertensMat);
}