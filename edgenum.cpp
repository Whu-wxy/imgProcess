#include "edgenum.h"
#include <QIntValidator>
EdgeNum::EdgeNum()
{
     lab=new QLabel("检测图像边缘使用的阈值为：");
     num=new QLineEdit("5",this);
     const QIntValidator *v=new QIntValidator(0,255);
     num->setValidator(v);
     /////虽然设置只能输入数字，但是输入后的类型还是QString

     Btn=new QDialogButtonBox(QDialogButtonBox::Ok|QDialogButtonBox::Cancel,Qt::Horizontal,this);

     layout=new QVBoxLayout(this);
     layout->addWidget(lab);
     layout->addWidget(num);
     layout->addWidget(Btn);

     connect(Btn,SIGNAL(accepted()),this,SLOT(accept()));
     connect(Btn,SIGNAL(rejected()),this,SLOT(reject()));
/////提供的标准按钮有一个自己的信号

}

int EdgeNum::getNum()
{
    int number=num->text().toInt();
    return number;
}


