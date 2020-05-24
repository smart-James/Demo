#include "helpframe.h"
#include "ui_helpframe.h"

HelpFrame::HelpFrame(QWidget *parent) :
    QFrame(parent),
    ui(new Ui::HelpFrame)
{
    ui->setupUi(this);
}

HelpFrame::~HelpFrame()
{
    delete ui;
}
