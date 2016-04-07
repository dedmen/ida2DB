#ifndef MAINWIDGET_H
#define MAINWIDGET_H

#include <QWidget>
QT_BEGIN_NAMESPACE
namespace Ui {
class mainWidget;
}

class mainWidget : public QWidget
{
		Q_OBJECT

	public:
		explicit mainWidget(QWidget *parent = 0);
		~mainWidget();

	private slots:
		void on_pushButton_clicked();

	private:
		Ui::mainWidget *ui;
};
QT_END_NAMESPACE
#endif // MAINWIDGET_H
