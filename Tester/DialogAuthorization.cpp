#include "DialogAuthorization.h"

DialogAuthorization::DialogAuthorization(QWidget *parent) : QDialog(parent)
{
    this->db = QSqlDatabase::addDatabase("QODBC", "qwerty");

    this->db.setDatabaseName("VISATESTING");
    this->db.open();

    QSqlQuery query(this->db);
    query.exec("USE VISATESTING;");
    this->model = new QSqlTableModel(this, this->db);
    this->model->setTable("Groups");
    this->model->select();

    this->layout = new QVBoxLayout(this);
    this->groupBoxGroups = new QGroupBox(tr("Группа:"), this);
    this->groupBoxGroups->setLayout(new QVBoxLayout(this->groupBoxGroups));
    this->comboGroup = new QComboBox(this);
    this->comboGroup->setModel(this->model);
    this->comboGroup->setModelColumn(1);
    this->groupBoxGroups->layout()->addWidget(this->comboGroup);

    this->groupBoxName = new QGroupBox(tr("Имя:"), this);
    this->groupBoxName->setLayout(new QVBoxLayout(this->groupBoxName));
    this->lineEditName = new QLineEdit(this);
    this->groupBoxName->layout()->addWidget(this->lineEditName);

    this->groupBoxSurname = new QGroupBox(tr("Фамилия:"), this);
    this->groupBoxSurname->setLayout(new QVBoxLayout(this->groupBoxSurname));
    this->lineEditSurname = new QLineEdit(this);
    this->groupBoxSurname->layout()->addWidget(this->lineEditSurname);

    this->buttons = new QDialogButtonBox(this);
    this->buttons->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    this->layout->addWidget(this->groupBoxGroups);
    this->layout->addWidget(this->groupBoxName);
    this->layout->addWidget(this->groupBoxSurname);
    this->layout->addWidget(this->buttons);

    connect(this->buttons, SIGNAL(accepted()), this, SLOT(ok()));
    connect(this->buttons, SIGNAL(rejected()), this, SLOT(cancel()));
}

DialogAuthorization::~DialogAuthorization()
{
    delete this->model;
    this->db.close();
}

void DialogAuthorization::showEvent(QShowEvent *e)
{
    this->setFixedSize(this->size());
    const int x = (QApplication::desktop()->width() - this->width()) / 2;
    const int y = (QApplication::desktop()->height() - this->height()) / 2;
    this->move(QPoint(x, y));
    QDialog::showEvent(e);
}

void DialogAuthorization::ok()
{
    const int id = this->model->record(this->comboGroup->currentIndex()).value(0).toInt();
    QSqlTableModel m(this, this->model->database());
    m.setTable("Students");
    m.select();

    QSqlQuery query(m.database());
    query.exec(QString("SELECT * FROM Students WHERE FirstName = '%1' AND LastName = '%2' AND GroupId = %3;").arg(this->lineEditName->text()).arg(this->lineEditSurname->text()).arg(id));
    if (query.next()) {
        QMessageBox::warning(this, "", tr("Вы уже прошли тестирование"));
        return;
    }

//    const int row = m.rowCount();
//    m.insertRow(row);
//    QSqlRecord record = m.record(row);
//    record.setValue("GroupId", id);
//    record.setValue("FirstName", this->lineEditName->text());
//    record.setValue("LastName", this->lineEditSurname->text());
//    m.setRecord(row, record);
//    m.submitAll();

//    query.exec(QString("SELECT * FROM Students WHERE FirstName = '%1' AND LastName = '%2' AND GroupId = %3;").arg(this->lineEditName->text()).arg(this->lineEditSurname->text()).arg(id));
//    query.next();
//    this->studentId = query.value(0).toInt();
    this->name = this->lineEditName->text();
    this->surname = this->lineEditSurname->text();
    this->groupId = id;
    this->db.close();
    this->done(12);
}

void DialogAuthorization::cancel()
{
    this->db.close();
    this->done(0);
}
