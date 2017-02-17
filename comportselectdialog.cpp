#include "comportselectdialog.h"
#include "ui_comportselectdialog.h"
#include <QSerialPortInfo>
ComPortSelectDialog::ComPortSelectDialog(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::ComPortSelectDialog)
{
	ui->setupUi(this);
	connect(ui->savePushButton,SIGNAL(clicked()),this,SLOT(saveClicked()));
	connect(ui->cancelPushButton,SIGNAL(clicked()),this,SLOT(cancelClicked()));
	connect(ui->refreshPushButton,SIGNAL(clicked()),this,SLOT(refreshClicked()));
	refreshClicked();
}

ComPortSelectDialog::~ComPortSelectDialog()
{
	delete ui;
}
void ComPortSelectDialog::cancelClicked()
{
	this->close();
}

void ComPortSelectDialog::saveClicked()
{
	emit savePort(ui->portListComboBox->currentText());
	this->close();
}
void ComPortSelectDialog::refreshClicked()
{
	ui->portListComboBox->clear();
	QList<QSerialPortInfo> portlist = QSerialPortInfo::availablePorts();
	foreach (QSerialPortInfo info,portlist)
	{
		ui->portListComboBox->addItem(info.portName());
	}
}
