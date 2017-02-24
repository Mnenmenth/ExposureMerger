/**
  * Created by Mnenmenth Alkaborin
  * Please refer to LICENSE file if included
  * for licensing information
  * https://github.com/Mnenmenth
  */

#include <QtWidgets/QApplication>
#include "../ui/ImgDisp.h"

int main(int argc, char **argv) {

    QApplication app(argc, argv);
    QCoreApplication::setOrganizationName("Mnenmenth Software");
    QCoreApplication::setOrganizationDomain("https://mnenmenth.com");
    QCoreApplication::setApplicationVersion("0.1");

    ImgDisp *imgDisp = new ImgDisp;

    app.setActiveWindow(imgDisp);
    imgDisp->show();

    return app.exec();
}