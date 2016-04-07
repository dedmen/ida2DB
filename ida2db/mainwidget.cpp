#include "mainwidget.h"
#include "ui_mainwidget.h"
#include "ida2db.h"
extern Ida2db* pIda2db = nullptr;
mainWidget::mainWidget(QWidget *parent) :
	QWidget(parent),
	ui(new Ui::mainWidget)
{
	ui->setupUi(this);
}

mainWidget::~mainWidget()
{
	delete ui;
}

void mainWidget::on_pushButton_clicked()
{
		ui->list_namedFunctions->addItems(pIda2db->getAllNamedFunctions());

}
