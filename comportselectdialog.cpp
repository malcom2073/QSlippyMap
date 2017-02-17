/************************************************************************************
 * QSlippyMap - Tile based slippery map                                             *
 * Copyright (C) 2017  Michael Carpenter (malcom2073@gmail.com)                     *
 *                                                                                  *
 * This file is a part of QSlippyMap                                                *
 *                                                                                  *
 * QSlippyMap is free software; you can redistribute it and/or                      *
 * modify it under the terms of the GNU Lesser General Public                       *
 * License as published by the Free Software Foundation, version                    *
 * 2.1 of the License.                                                              *
 *                                                                                  *
 * QSlippyMap is distributed in the hope that it will be useful,                    *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of                   *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU                *
 * Lesser General Public License for more details.                                  *
 *                                                                                  *
 * You should have received a copy of the GNU Lesser General Public                 *
 * License along with this program; if not, write to the Free Software              *
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301  USA   *
 ************************************************************************************/

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
