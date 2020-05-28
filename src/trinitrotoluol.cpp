
#include <QMainWindow>
#include <QApplication>
#include <QDebug>
#include <QIcon>

#include <src/main_window.h>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    
    QApplication::setApplicationName("trinitrotoluol");
    
    QTNTMainWindow mainWin;
    
#ifdef __linux__
#else
        QIcon::setThemeSearchPaths(QStringList(QList<QString>() << "./Adwaita"));
        QIcon::setThemeName("Adwaita");
#endif
    
    mainWin.setWindowIcon(QIcon(":logo"));
    mainWin.show();

    //mainWin.resize(1080, 1080);

    return app.exec();
}
