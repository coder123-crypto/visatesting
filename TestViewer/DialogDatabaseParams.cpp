#include "DialogDatabaseParams.h"

DialogDatabaseParams::DialogDatabaseParams(QWidget *parent) : QDialog(parent)
{
    this->layout = new QVBoxLayout(this);
    this->layout->addWidget((this->groupBoxHost = new QGroupBox(tr("Хост:"), this)));
    this->layout->addWidget((this->groupBoxPort = new QGroupBox(tr("Порт:"), this)));
    this->layout->addWidget((this->groupBoxLogin = new QGroupBox(tr("Логин:"), this)));
    this->layout->addWidget((this->groupBoxPassword = new QGroupBox(tr("Пароль:"), this)));
    this->groupBoxHost->setLayout(new QVBoxLayout(this->groupBoxHost));
    this->groupBoxPort->setLayout(new QVBoxLayout(this->groupBoxPort));
    this->groupBoxLogin->setLayout(new QVBoxLayout(this->groupBoxLogin));
    this->groupBoxPassword->setLayout(new QVBoxLayout(this->groupBoxPassword));
    this->groupBoxHost->layout()->addWidget((this->lineEditHost = new QLineEdit(this->groupBoxHost)));
    this->groupBoxPort->layout()->addWidget((this->lineEditPort = new QLineEdit(this->groupBoxPort)));
    this->groupBoxLogin->layout()->addWidget((this->lineEditLogin = new QLineEdit(this->groupBoxLogin)));
    this->groupBoxPassword->layout()->addWidget((this->lineEditPassword = new QLineEdit(this->groupBoxPassword)));
    this->layout->addWidget((this->buttons = new QDialogButtonBox(this)));
    this->buttons->setStandardButtons(QDialogButtonBox::Ok | QDialogButtonBox::Cancel);

    QSettings s("MIREA", "VisaTestViewer", this);
    this->lineEditHost->setText(s.value("HostName", "127.0.0.1").toString());
    this->lineEditPort->setText(s.value("Port", "3306").toString());
    this->lineEditLogin->setText(s.value("UserName", "root").toString());
    this->lineEditPassword->setText(s.value("Password", "").toString());
    this->lineEditPassword->setEchoMode(QLineEdit::Password);
    this->lineEditPort->setValidator(new QIntValidator(1, 2e15, this->lineEditPort));

    connect(this->buttons, SIGNAL(accepted()), this, SLOT(saveParams()));
    connect(this->buttons, SIGNAL(accepted()), this, SLOT(accept()));
    connect(this->buttons, SIGNAL(rejected()), this, SLOT(reject()));
}

void DialogDatabaseParams::showEvent(QShowEvent *e)
{
    this->setFixedSize(this->size());
    const int x = (QApplication::desktop()->width() - this->width()) / 2;
    const int y = (QApplication::desktop()->height() - this->height()) / 2;
    this->move(QPoint(x, y));
    QDialog::showEvent(e);
}

void DialogDatabaseParams::saveParams()
{
    QSettings s("MIREA", "VisaTestViewer", this);
    s.setValue("HostName", this->lineEditHost->text());
    s.setValue("Port", this->lineEditPort->text().toInt());
    s.setValue("UserName", this->lineEditLogin->text());
    s.setValue("Password", this->lineEditPassword->text());
}
