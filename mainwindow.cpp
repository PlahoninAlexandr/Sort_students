#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("./Students.db");
    db.open() ? qDebug("open") : qDebug("no open");

    query = new QSqlQuery(db);
    query->exec("CREATE TABLE students(Firstname TEXT, Secondname TEXT, Faculty TEXT, CompScience INT, Maths INT, English INT, TypeEducation TEXT);");

    model = new QSqlTableModel(this, db);
    model->setTable("students");
    model->select();

    ui->tableView->setModel(model);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_addEntry_clicked()
{
    model->insertRow(model->rowCount());
}


void MainWindow::on_deleteEntry_clicked()
{
    model->removeRow(row);
}


void MainWindow::on_tableView_clicked(const QModelIndex &index)
{
    row = index.row();
}


void MainWindow::on_calculateBudget_clicked()
{
    query->prepare("SELECT * FROM students");
    query->exec();

    QVector<int> marks;

    QString s = "UPDATE students SET TypeEducation = 'budget' WHERE ((CompScience + Maths + English) / 3) >= 80";
    QString ss = "UPDATE students SET TypeEducation = 'contract' WHERE ((CompScience + Maths + English) / 3) < 80";

    while(query->next()){
        marks.push_back((query->value("CompScience").toInt() + query->value("Maths").toInt() + query->value("English").toInt()) / 3);
    }

    for(auto &x : marks){
        if(x >= 80){
            query->prepare(s);
            query->exec();
        }else{
            query->prepare(ss);
            query->exec();
        }
    }
}

