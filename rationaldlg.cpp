#include "rationaldlg.h"

RationalDlg::RationalDlg(bool grayImage)
{
    colorCombo = new QComboBox(this);
    if(grayImage)
        colorCombo->addItem("黑色",QVariant("K"));
    else
    {
        colorCombo->addItem("青色",QVariant("C"));
        colorCombo->addItem("品红色",QVariant("M"));
        colorCombo->addItem("黄色",QVariant("Y"));
        colorCombo->addItem("黑色",QVariant("K"));
    }

    angleCombo= new QComboBox(this);
    angleCombo->addItem("0",QVariant(0));
    angleCombo->addItem("15",QVariant(15));
    angleCombo->addItem("45",QVariant(45));
    angleCombo->addItem("75",QVariant(75));

    QPushButton *button =new QPushButton(tr("确定"),this);
    connect(button,SIGNAL(clicked()),this,SLOT(onConfirm()));

    QLabel *colorLabel=new QLabel("颜色:");
    QLabel *angleLabel=new QLabel("角度:");

    QGridLayout *mainlayout= new QGridLayout(this);

    mainlayout->addWidget(colorLabel,0,0);
    mainlayout->addWidget(angleLabel,1,0);
    mainlayout->addWidget(colorCombo,0,1);
    mainlayout->addWidget(angleCombo,1,1);
    mainlayout->addWidget(button,2,1);

    resize(400,200);

}

void RationalDlg::onConfirm()
{
    QDialog::accept();
}
