#include "MainWindow.h"
#include "src/utils/Config.h"
#include <QApplication>
#include <QDesktopWidget>
#include <QSplashScreen>
#include <QTimer>

///
/// perform some checkups when initializing eg. missing files...
///
void initializationCheckUp();
void initializationCheckUp(){
    return;
}

int main(int argc, char *argv[]){
    ///
    /// initialization...
    ///
    initializationCheckUp();
    QApplication app(argc, argv);
    ///
    /// splash screen...
    ///
    QPixmap splashImage = QPixmap(":/img/res/icons/splash.png");
    splashImage.scaledToWidth(600);
    QSplashScreen splash(splashImage);
    splash.show();
    ///
    /// removing all the question mark buttons on dialogs...
    ///
    QApplication::setAttribute(Qt::AA_DisableWindowContextHelpButton);
    ///
    /// starting the application...
    ///
    MainWindow w;
    ///
    /// setting the app to the center of Screen on start...
    ///
    int x = (app.desktop()->width()-w.width()) / 2;
    int y = (app.desktop()->height()-w.height()) / 2;
    w.move(x, y-35);
    w.show();
    ///
    /// splashscreen timer...
    ///
    splash.finish(&w);
    ///
    /// set styleSheet...
    ///
    /*
    QFile file(":/themes/res/themes/native.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    app.setStyleSheet(styleSheet);
    */
    ///
    /// setting configurations...
    ///
    Config::generalConfig();
    ///
    /// starting the app...
    ///
    return app.exec();
}
