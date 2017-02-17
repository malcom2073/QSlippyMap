#ifndef COMPORTSELECTDIALOG_H
#define COMPORTSELECTDIALOG_H

#include <QWidget>

namespace Ui {
class ComPortSelectDialog;
}

class ComPortSelectDialog : public QWidget
{
	Q_OBJECT

public:
	explicit ComPortSelectDialog(QWidget *parent = 0);
	~ComPortSelectDialog();

private:
	Ui::ComPortSelectDialog *ui;
private slots:
	void cancelClicked();
	void saveClicked();
	void refreshClicked();
signals:
	void savePort(QString port);
};

#endif // COMPORTSELECTDIALOG_H
