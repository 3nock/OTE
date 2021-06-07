#include "MainWindow.h"
#include <QApplication>

// perform some checkups when initializing eg. missing files...
void initializationCheckUp();
void initializationCheckUp(){
    return;
}

int main(int argc, char *argv[]){
    // initialization...
    initializationCheckUp();

    QApplication a(argc, argv);
    // removing all the question mark buttons on dialogs...
    QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);

    // starting the application...
    MainWindow w;

    // setting the app to the center of Screen on start...
    QRect screenGeometry = QApplication::desktop()->screenGeometry();
    int x = (screenGeometry.width()-w.width()) / 2;
    int y = (screenGeometry.height()-w.height()) / 2;
    w.move(x, y-30);
    w.show();

    return a.exec();
}
