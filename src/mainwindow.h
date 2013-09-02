#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include <QLabel>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private slots:

    void on_actionImport_triggered();

private:
    Ui::MainWindow *ui;
    QSqlTableModel *model;
    QLabel * labelInfo;
    QStringList list;

    void readSQL();
    void updateSQL(QStringList &newWords);
};

#endif // MAINWINDOW_H
