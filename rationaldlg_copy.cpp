#include "RationalDlg_copy.h"

RationalDlg_copy::RationalDlg_copy(bool grayImage)
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


    QPushButton *button =new QPushButton(tr("确定"),this);
    connect(button,SIGNAL(clicked()),this,SLOT(onConfirm()));

    QLabel *colorLabel=new QLabel("颜色:");

    QGridLayout *mainlayout= new QGridLayout(this);

    mainlayout->addWidget(colorLabel,0,0);
    mainlayout->addWidget(colorCombo,0,1);
    mainlayout->addWidget(button,1,1);

    resize(400,200);

}

void RationalDlg_copy::onConfirm()
{
    QDialog::accept();
}
