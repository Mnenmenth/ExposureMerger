/**
  * Created by Mnenmenth Alkaborin
  * Please refer to LICENSE file if included
  * for licensing information
  * https://github.com/Mnenmenth
  */

#include "ImgDisp.h"
#include <QtWidgets/QFileDialog>
#include <QtWidgets/QMessageBox>

ImgDisp::ImgDisp() {

    debevecImg = new QLabel();
    debevecImg->setMaximumSize(QSize(400, 400));
    debevecSaveButton = new QPushButton("Save Debevec Merge");
    debevecMat = new cv::Mat();

    tonemapImg = new QLabel();
    tonemapImg->setMaximumSize(QSize(400, 400));
    tonemapSaveButton = new QPushButton("Save Tonemap Merge");
    tonemapMat = new cv::Mat();

    mertensImg = new QLabel();
    mertensImg->setMaximumSize(QSize(400, 400));
    mertensSaveButton = new QPushButton("Save Mertens Merge");
    mertensMat = new cv::Mat();

    imgList = new QTableWidget();
    imgList->setMinimumHeight(200);

    addImgsButton = new QPushButton("Add Images");

    removeRowsButton = new QPushButton("Remove Rows");

    mergeButton = new QPushButton("Merge Images");

    gammaLabel = new QLabel("Gamma Level: ");
    gammaInput = new QLineEdit();
    gammaInput->setPlaceholderText("ex: 1.3");

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
    topLayout->addWidget(removeRowsButton);
    topLayout->addWidget(mergeButton);
    topLayout->addWidget(gammaLabel);
    topLayout->addWidget(gammaInput);

    bottomLayout->addWidget(debevecGroup);
    bottomLayout->addWidget(tonemapGroup);
    bottomLayout->addWidget(mertensGroup);

    debevecLayout->addWidget(debevecSaveButton);
    debevecLayout->addWidget(debevecImg);

    tonemapLayout->addWidget(tonemapSaveButton);
    tonemapLayout->addWidget(tonemapImg);

    mertensLayout->addWidget(mertensSaveButton);
    mertensLayout->addWidget(mertensImg);

    connect(addImgsButton, SIGNAL(clicked()), this, SLOT(addImgs()));
    connect(removeRowsButton, SIGNAL(clicked()), this, SLOT(removeRows()));
    connect(mergeButton, SIGNAL(clicked()), this, SLOT(mergeImgs()));
    connect(debevecSaveButton, SIGNAL(clicked()), this, SLOT(debevecSave()));
    connect(tonemapSaveButton, SIGNAL(clicked()), this, SLOT(tonemapSave()));
    connect(mertensSaveButton, SIGNAL(clicked()), this, SLOT(mertensSave()));

    imgList->setRowCount(0);
    imgList->setColumnCount(2);
    QStringList headers;
    headers.append("Image Paths");
    headers.append("Exposure Times");
    imgList->setHorizontalHeaderLabels(headers);
}

ImgDisp::~ImgDisp() {
    delete debevecImg;
    delete debevecSaveButton;
    delete debevecMat;

    delete tonemapImg;
    delete tonemapSaveButton;
    delete tonemapMat;

    delete mertensImg;
    delete mertensSaveButton;
    delete mertensMat;

    delete imgList;
    delete addImgsButton;
    delete removeRowsButton;
    delete mergeButton;
    delete gammaLabel;
    delete gammaInput;

    delete mertensLayout;
    delete mertensGroup;

    delete tonemapLayout;
    delete tonemapGroup;

    delete debevecLayout;
    delete debevecGroup;

    delete topLayout;
    delete topGroup;

    delete bottomLayout;
    delete bottomGroup;

    delete mainLayout;
}

void ImgDisp::addImgs() {

    QString filter = tr("Image (*.jpg *.jpeg *.png *.tif *.bmp)");
    QStringList filepaths = QFileDialog::getOpenFileNames(this, tr("Images to merge"), QDir::homePath(),
                                                          tr("Image (*.jpg *.jpeg *.png *.tif *.bmp);;All files(*.*)"),
                                                          &filter);
    if (!filepaths.empty()) {
        for (QString file : filepaths) {
            int rowNum = imgList->rowCount();
            imgList->insertRow(rowNum);
            QTableWidgetItem *img = new QTableWidgetItem(file);
            QTableWidgetItem *time = new QTableWidgetItem(tr("0"));
            imgList->setItem(rowNum, 0, img);
            imgList->setItem(rowNum, 1, time);
            imgList->resizeRowToContents(rowNum);
        }
    }

}

void ImgDisp::removeRows() {
    QModelIndexList selection = imgList->selectionModel()->selectedRows();
    std::reverse(selection.begin(), selection.end());
    for (QModelIndex index : selection.toVector()) {
        imgList->removeRow(index.row());
    }

}

void ImgDisp::mergeImgs() {
    std::vector<std::string> files;
    std::vector<float> times;
    float gamma = 0;

    QMessageBox message;
    message.setWindowTitle("Error");
    //message.setMinimumSize(400, 200);
    bool *time_ok = new bool(false);

    bool *gamma_ok = new bool(false);
    float gammaf = gammaInput->text().toFloat(gamma_ok);
    if (*gamma_ok) {
        gamma = gammaf;

        for (int i = 0; i < imgList->rowCount(); i++) {
            std::string img = imgList->item(i, 0)->text().toStdString();
            float time = 0;
            QString text = imgList->item(i, 1)->text();
            if (text.contains('/')) {
                QStringList div = text.split('/');
                float left = div.first().toFloat(time_ok);
                if (*time_ok) {
                    float right = div.last().toFloat(time_ok);
                    time = left / right;
                }
            } else {
                time = imgList->item(i, 1)->text().toFloat(time_ok);
            }

            if (*time_ok) {
                files.push_back(img);
                times.push_back(time);
            } else {
                message.setText("Time conversion error at row: " + QString::number(i));
                message.adjustSize();
                message.exec();
                break;
            }

        }
    } else {
        message.setText("Invalid gamma value");
        message.adjustSize();
        message.exec();
    }
    if (*time_ok && *gamma_ok) {
        MergeExposures::merge(files, times, gamma, debevecMat, tonemapMat, mertensMat);
        Mat2QLabel(debevecMat, debevecImg, 1);
        Mat2QLabel(tonemapMat, tonemapImg, 255);
        Mat2QLabel(mertensMat, mertensImg, 255);
    }
}

void ImgDisp::Mat2QLabel(cv::Mat *src, QLabel *dest, int mult) {
    cv::Mat temp;
    cv::cvtColor(*src * mult, temp, cv::COLOR_BGR2RGB);
    temp.convertTo(temp, CV_8UC3);
    std::cout << temp.type() << std::endl;
    dest->setPixmap(QPixmap::fromImage(QImage(temp.data, temp.cols, temp.rows, temp.step, QImage::Format_RGB888)));
    dest->adjustSize();
}

void ImgDisp::debevecSave() {
    QString filter = tr("Image (*.jpg *.jpeg *.png *.tif *.bmp)");
    std::string path = QFileDialog::getSaveFileName(this, tr("Save File As"), QDir::homePath(),
                                                    tr("Image (*.jpg *.jpeg *.png *.tif *.bmp);;All files(*.*)"),
                                                    &filter).toStdString();
    cv::imwrite(path, *debevecMat);
}

void ImgDisp::tonemapSave() {
    QString filter = tr("Image (*.jpg *.jpeg *.png *.tif *.bmp)");
    std::string path = QFileDialog::getSaveFileName(this, tr("Save File As"), QDir::homePath(),
                                                    tr("Image (*.jpg *.jpeg *.png *.tif *.bmp);;All files(*.*)"),
                                                    &filter).toStdString();
    cv::imwrite(path, *tonemapMat * 255);
}

void ImgDisp::mertensSave() {
    QString filter = tr("Image (*.jpg *.jpeg *.png *.tif *.bmp)");
    std::string path = QFileDialog::getSaveFileName(this, tr("Save File As"), QDir::homePath(),
                                                    tr("Image (*.jpg *.jpeg *.png *.tif *.bmp);;All files(*.*)"),
                                                    &filter).toStdString();
    cv::imwrite(path, *mertensMat * 255);
}