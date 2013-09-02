#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QFileDialog>
#include <QTextStream>
#include <QFile>
#include <QMessageBox>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    labelInfo = new QLabel(this);
    labelInfo->setText("Готов");
    ui->statusBar->addWidget(labelInfo);

    model = new QSqlTableModel;
    model->setTable("translator");
    model->setEditStrategy(QSqlTableModel::OnFieldChange);
    model->select();
    model->setHeaderData(0, Qt::Horizontal, tr("Русский"));
    model->setHeaderData(1, Qt::Horizontal, tr("Арабский"));
    model->setHeaderData(2, Qt::Horizontal, tr("Немецкий"));
    model->setHeaderData(3, Qt::Horizontal, tr("Английский"));
    model->setHeaderData(4, Qt::Horizontal, tr("Испанский"));
    model->setHeaderData(5, Qt::Horizontal, tr("Французский"));
    model->setHeaderData(6, Qt::Horizontal, tr("Итальянский"));
    model->setHeaderData(7, Qt::Horizontal, tr("Норвежский"));
    model->setHeaderData(8, Qt::Horizontal, tr("Китайский"));
    ui->TableTranslate->setModel(model);
    ui->TableTranslate->resizeRowsToContents();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_actionImport_triggered()
{
    QString fileName = QFileDialog::getOpenFileName(this,tr("Открыть файл"),QDir::homePath(),"Languge file (*.jlt)");
    if(fileName.isEmpty())
        return;

    QFile file(fileName);
    QStringList newWords;

    if(file.open(QIODevice::ReadOnly)){
        QTextStream in(&file);
        in.setCodec("UTF-8");

        while(!in.atEnd()){
            QString detectData = in.readLine();
            if(!detectData.isEmpty())
                newWords.append(detectData);
        }

        file.close();
        updateSQL(newWords);
    } else{
        QMessageBox::critical(this,tr("Ошибка чтения"),tr("Не удалось открыть файл"));
    }
}

void MainWindow::readSQL()
{
    list.clear();

    QSqlQuery q("SELECT rus FROM translator");

    if(q.exec() && q.isActive() && q.lastError().type()==QSqlError::NoError){
        while(q.next()){
            list.append(q.record().value(0).toString());
        }
    }
}

void MainWindow::updateSQL(QStringList &newWords)
{
    /*Выбрать уникальные слова*/
    QStringList listUnique;
    for(int i=0, l=newWords.size(); i<l; i++){
        if(!listUnique.contains(newWords.at(i),Qt::CaseInsensitive))
            listUnique.append(newWords.at(i));
    }

    /*Выбор уже существующих слов*/
    readSQL();

    /*Добавление новых слов*/
    QSqlQuery query;
    int count=0;
    query.exec("begin transaction;");
    for(int i=0,l=listUnique.size(); i<l; i++){
        if(!list.contains(listUnique.at(i),Qt::CaseInsensitive)){
            query.clear();
            query.prepare(tr("INSERT INTO translator(rus) VALUES(?)"));
            query.addBindValue(listUnique.at(i));
            query.exec();
            labelInfo->setText(tr("Обработка слова: %1").arg(count++));
            qApp->processEvents();
        }
    }
    if(!query.exec("commit transaction;"))
        query.exec("rollback transaction;");

    labelInfo->setText(tr("Загруженно новых слов: %1").arg(count));
    model->select();
    ui->TableTranslate->resizeRowsToContents();
    QMessageBox::information(this,tr("Загрузка"),tr("Загрузка завершена.\nДобавлено новых слов: %1").arg(count));
}
