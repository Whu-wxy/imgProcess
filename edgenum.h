#ifndef EDGENUM_H
#define EDGENUM_H

#include <QDialog>
#include <QLineEdit>
#include <QDialogButtonBox>
#include <QVBoxLayout>
#include <QLabel>
class EdgeNum : public QDialog
{
        Q_OBJECT
public:
    EdgeNum();
    int getNum();

private:
    QLineEdit *num;
    QLabel *lab;
    QVBoxLayout *layout;
    QDialogButtonBox *Btn;

};

#endif // EDGENUM_H
