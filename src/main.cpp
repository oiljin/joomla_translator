#include "mainwindow.h"
#include <QApplication>
#include <QtSql>
#include <QMessageBox>
#include <QTextCodec>
#include <QTranslator>
int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec *cyrillicCodec = QTextCodec::codecForName("UTF-8");
    QTextCodec::setCodecForLocale(cyrillicCodec);

    QString qtTranslatorName = QLatin1String("qt_");
    qtTranslatorName += QLocale::system().name();
    QTranslator *qtTranslator = new QTranslator(&a);

    if (qtTranslator->load(qtTranslatorName, qApp->tr("%1/translations").arg (qApp->applicationDirPath ())))
        a.installTranslator(qtTranslator);


    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("words.db");
    if(!db.open()){
        QMessageBox::critical(0,"",QObject::tr("Не удалось запустить приложение"));
        return 0;
    }

    MainWindow w;
    w.show();
    
    return a.exec();
}
