#include "roidlg.h"

ROIDlg::ROIDlg()
{
    x_label = new QLabel(tr("x:"));
    label1 = new QLabel(tr("-"));
    y_label = new QLabel(tr("y:"));
    label2 = new QLabel(tr("-"));
    x1_text = new QLineEdit;
    x2_text = new QLineEdit;
    y1_text = new QLineEdit;
    y2_text = new QLineEdit;
    button = new QPushButton(tr("确定"),this);
    connect(button,SIGNAL(clicked()),this,SLOT(onConfirm()));
    mainLayout = new QGridLayout(this);
    mainLayout->addWidget(x_label,0,0);
    mainLayout->addWidget(y_label,1,0);
    mainLayout->addWidget(x1_text,0,1);
    mainLayout->addWidget(y1_text,1,1);
    mainLayout->addWidget(label1,0,2);
    mainLayout->addWidget(label2,1,2);
    mainLayout->addWidget(x2_text,0,3);
    mainLayout->addWidget(y2_text,1,3);
    mainLayout->addWidget(button,2,3);
    resize(400,200);
}

void ROIDlg::onConfirm()
{
    QDialog::accept();
}
