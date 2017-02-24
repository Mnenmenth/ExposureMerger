/**
  * Created by Mnenmenth Alkaborin
  * Please refer to LICENSE file if included
  * for licensing information
  * https://github.com/Mnenmenth
  */

#ifndef EXPOSUREMERGER_IMGDISP_H
#define EXPOSUREMERGER_IMGDISP_H

#include <memory>
#include <QtWidgets/QWidget>
#include <QtWidgets/QLabel>
#include <QtWidgets/QTableWidget>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QGroupBox>
#include <QtWidgets/QHBoxLayout>
#include <QtWidgets/QVBoxLayout>
#include <QtCore/QSignalMapper>
#include <QtWidgets/QLineEdit>
#include "../imgprocessing/MergeExposures.h"

class ImgDisp : public QGroupBox {
Q_OBJECT
public:
    ImgDisp();

    ~ImgDisp();

private:
    QLabel *debevecImg;
    QPushButton *debevecSaveButton;
    cv::Mat *debevecMat;

    QLabel *tonemapImg;
    QPushButton *tonemapSaveButton;
    cv::Mat *tonemapMat;

    QLabel *mertensImg;
    QPushButton *mertensSaveButton;
    cv::Mat *mertensMat;

    QTableWidget *imgList;

    QPushButton *addImgsButton;
    QPushButton *removeRowsButton;
    QLabel *gammaLabel;
    QLineEdit *gammaInput;

    QPushButton *mergeButton;

    QVBoxLayout *mainLayout;

    QGroupBox *topGroup;
    QHBoxLayout *topLayout;

    QGroupBox *bottomGroup;
    QHBoxLayout *bottomLayout;

    QGroupBox *debevecGroup;
    QVBoxLayout *debevecLayout;

    QGroupBox *tonemapGroup;
    QVBoxLayout *tonemapLayout;

    QGroupBox *mertensGroup;
    QVBoxLayout *mertensLayout;

    void Mat2QLabel(cv::Mat *, QLabel *, int);

private slots:

    void addImgs();

    void removeRows();

    void mergeImgs();

    void debevecSave();

    void tonemapSave();

    void mertensSave();
};


#endif //EXPOSUREMERGER_IMGDISP_H
