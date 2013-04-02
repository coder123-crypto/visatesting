#include "MainWindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    this->createGUI();
    this->loadDataBase();

    connect(this->butAddGroup, SIGNAL(clicked()), this, SLOT(addGroup()));
    connect(this->butRenameGroup, SIGNAL(clicked()), this, SLOT(renameGroup()));
    connect(this->butDeleteGroup, SIGNAL(clicked()), this, SLOT(deleteGroup()));
    connect(this->comboBoxGroups, SIGNAL(currentIndexChanged(int)), this, SLOT(selectGroup(int)));
    connect(this->butRemoveStudent, SIGNAL(clicked()), this, SLOT(removeStudent()));
    this->selectGroup(0);
}

MainWindow::~MainWindow()
{
    db.close();
}

void MainWindow::addGroup()
{
    if (this->db.isOpen()) {
        bool ok;
        QString newGroupName = QInputDialog::getText(this, "", tr("Введите название новой группы"), QLineEdit::Normal, "", &ok);
        newGroupName = newGroupName.trimmed();

        QSqlQuery query(this->db);
        query.exec(QString("SELECT * FROM Groups WHERE Name = %1;").arg(newGroupName));
        if (query.next()) {
            QMessageBox::warning(this, "", tr("Такая группа уже существуетr"));
            return;
        }

        if (ok == true && newGroupName.isEmpty() == false) {
            const int row = this->model->relationModel(1)->rowCount();
            this->model->relationModel(1)->insertRow(row);
            QSqlRecord record = this->model->relationModel(1)->record(row);
            record.setValue("Name", newGroupName);
            this->model->relationModel(1)->setRecord(row, record);
            this->model->relationModel(1)->submitAll();
            this->model->select();
            this->model->relationModel(1)->select();
        }
    }
    else
        QMessageBox::warning(this, "", tr("База данных не открыта"));
}

void MainWindow::renameGroup()
{
    if (this->db.isOpen()) {
        const int row = this->comboBoxGroups->currentIndex();
        if (row > -1) {
            bool ok;
            QString newGroupName = QInputDialog::getText(this, "", tr("Введите название новой имени для группы"), QLineEdit::Normal, this->comboBoxGroups->currentText(), &ok);
            newGroupName = newGroupName.trimmed();
            if (ok == true && newGroupName.isEmpty() == false) {
                QSqlRecord record = this->model->relationModel(1)->record(row);
                record.setValue("Name", newGroupName);
                this->model->relationModel(1)->setRecord(row, record);
                this->model->relationModel(1)->submitAll();
                this->model->select();
            }
        }
        else
            QMessageBox::warning(this, "", tr("Группа не выбрана"));
    }
    else
        QMessageBox::warning(this, "", tr("База данных не открыта"));
}

void MainWindow::deleteGroup()
{
    if (this->db.isOpen()) {
        int r = QMessageBox::question(this, "", tr("Вы действительно хотите удалить группу со всеми студентами?"), QMessageBox::Yes | QMessageBox::No);
        if (r == QMessageBox::Yes) {
            const int index = this->comboBoxGroups->currentIndex();
            const QSqlRecord record = this->model->relationModel(1)->record(index);
            const QString filter = QString("GroupId = %1").arg(record.value(0).toString());
            this->model->setFilter(filter);
            this->model->removeRows(0, this->model->rowCount());
            this->model->relationModel(1)->removeRow(index);
            this->model->relationModel(1)->submitAll();
            this->model->submitAll();
            this->model->select();
        }
    }
    else
        QMessageBox::warning(this, "", tr("База данных не открыта"));
}

void MainWindow::selectGroup(int)
{
    if (this->db.isOpen()) {
        const int index = this->comboBoxGroups->currentIndex();
        const QSqlRecord record = this->model->relationModel(1)->record(index);
        const QString filter = QString("GroupId = %1").arg(record.value(0).toString());
        this->model->setFilter(filter);
    }
}

void MainWindow::removeStudent()
{
    QModelIndex index = this->viewStudents->currentIndex();
    if (index.row() > -1) {
        int r = QMessageBox::question(this, "", tr("Вы действительно хотите удалить эту запись?"), QMessageBox::Yes | QMessageBox::No);
        if (r == QMessageBox::Yes) {
            this->model->removeRow(index.row());
            model->submitAll();
        }
    }
    else
        QMessageBox::warning(this, "", tr("Запись не выбрана"));
}

void MainWindow::showEvent(QShowEvent *e)
{
    this->resize(QApplication::desktop()->size() / 2);
    const int x = (QApplication::desktop()->width() - this->width()) / 2;
    const int y = (QApplication::desktop()->height() - this->height()) / 2;
    this->move(QPoint(x, y));
    QMainWindow::showEvent(e);
}

void MainWindow::showDbParams()
{
    DialogDatabaseParams d(this);
    if (d.exec() == QDialog::Accepted)
        QMessageBox::information(this, "", tr("Для применения настроек - перезапустите приложение"));
}

void MainWindow::createGUI()
{
    this->setWindowTitle(tr("Просмотр результатов тестирования"));
    this->setCentralWidget(new QWidget(this));
    this->layout = new QHBoxLayout(this->centralWidget());

    this->layoutButtons = new QVBoxLayout;
    this->comboBoxGroups = new QComboBox(this->centralWidget());
    this->butAddGroup = new QPushButton(tr("Добавить группу"), this->centralWidget());
    this->butRenameGroup = new QPushButton(tr("Переименовать группу"), this->centralWidget());
    this->butDeleteGroup = new QPushButton(tr("Удалить группу"), this->centralWidget());
    this->butRemoveStudent = new QPushButton(tr("Удалить студента"), this->centralWidget());

    this->layoutButtons->addWidget(this->comboBoxGroups);
    this->layoutButtons->addWidget(this->butAddGroup);
    this->layoutButtons->addWidget(this->butRenameGroup);
    this->layoutButtons->addWidget(this->butDeleteGroup);
    this->layoutButtons->addWidget(this->butRemoveStudent);
    this->layoutButtons->addStretch();

    this->layout->addLayout(this->layoutButtons);

    this->viewStudents = new QTableView(this->centralWidget());
    this->layout->addWidget(this->viewStudents);

    this->menuBar = new QMenuBar(this);
    this->menuDbParams = new QMenu(tr("Настройки"), this);
    this->menuDbParams->addAction(tr("Настройки сервера БД"), this, SLOT(showDbParams()));
    this->menuBar->addMenu(this->menuDbParams);
    this->setMenuBar(this->menuBar);
}

void MainWindow::loadDataBase()
{
    QSqlDatabase::removeDatabase("chupakabra");
    this->db = QSqlDatabase::addDatabase("QMYSQL", "chupakabra");
    if (this->createDataBase() == false) {
        QMessageBox::critical(this, "", tr("Ошибка создания базы данных: \n%1").arg(this->lastError));
        return;
    }

    this->model = new QSqlRelationalTableModel(this, this->db);
    this->model->setTable("Students");

    this->model->setRelation(1, QSqlRelation("Groups", "Id", "Name"));
    this->model->select();
    this->model->setHeaderData(2, Qt::Horizontal, tr("Фамилия\nстудента"));
    this->model->setHeaderData(3, Qt::Horizontal, tr("Имя\nстудента"));
    this->model->setHeaderData(4, Qt::Horizontal, tr("Число\nошибок"));
    this->model->setHeaderData(5, Qt::Horizontal, tr("Время\nтестирования"));

    this->viewStudents->setModel(this->model);
    this->viewStudents->setItemDelegate(new DelegateTestResult(this->viewStudents));
    this->viewStudents->hideColumn(0);
    this->viewStudents->hideColumn(1);
    //    this->viewStudents->resizeColumnsToContents();
    this->viewStudents->setEditTriggers(QAbstractItemView::NoEditTriggers);
    this->viewStudents->setSelectionMode(QAbstractItemView::SingleSelection);
    this->viewStudents->setSelectionBehavior(QAbstractItemView::SelectRows);

    this->comboBoxGroups->setModel(this->model->relationModel(1));
    this->comboBoxGroups->setModelColumn(1);
}

bool MainWindow::createDataBase()
{
    QSettings s("MIREA", "VisaTestViewer");

    this->db.setHostName(s.value("HostName").toString());
    this->db.setUserName(s.value("UserName").toString());
    this->db.setPort(s.value("Port").toInt());
    this->db.setPassword(s.value("Password").toString());

    if (this->db.open() == true) {
        QSqlQuery query(this->db);

        if (query.exec("CREATE DATABASE IF NOT EXISTS VISATESTING;") == false) {
            this->lastError = query.lastError().text();
            return false;
        }

        if (query.exec("USE VISATESTING;") == false) {
            this->lastError = query.lastError().text();
            return false;
        }

        if (query.exec("CREATE TABLE IF NOT EXISTS Groups (Id INT AUTO_INCREMENT PRIMARY KEY, Name VARCHAR(20) NOT NULL UNIQUE);") == false) {
            this->lastError = query.lastError().text();
            return false;
        }

        if (query.exec("CREATE TABLE IF NOT EXISTS Students (Id INT AUTO_INCREMENT PRIMARY KEY, "
                       "GroupId INT, FirstName TEXT NOT NULL, LastName TEXT NOT NULL, "
                       "Results BLOB, TestDate DATETIME, FOREIGN KEY(GroupId) REFERENCES Groups(Id));") == false) {
            this->lastError = query.lastError().text();
            return false;
        }

        return true;
    }
    else {
        this->lastError = this->db.lastError().text();
        return false;
    }
}
