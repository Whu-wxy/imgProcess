#ifndef ROIDLG_H
#define ROIDLG_H
#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QGridLayout>

class ROIDlg : public QDialog
{
    Q_OBJECT
public:
    ROIDlg();
    QLabel *x_label;
    QLabel *y_label;
    QLabel *label1;
    QLabel *label2;
    QLineEdit *x1_text;
    QLineEdit *x2_text;
    QLineEdit *y1_text;
    QLineEdit *y2_text;
    QPushButton *button;
    QGridLayout *mainLayout;
protected slots:
    void onConfirm();
};

#endif // ROIDLG_H
