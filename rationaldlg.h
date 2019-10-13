#ifndef RATIONALDLG_H
#define RATIONALDLG_H
#include<QDialog>
#include<QComboBox>
#include<QPushButton>
#include<QGridLayout>
#include<QLabel>

class RationalDlg : public QDialog
{
    Q_OBJECT
public:
    RationalDlg();
    QComboBox *colorCombo;
    QComboBox *angleCombo;

protected slots:
    void onConfirm();


};



#endif // RATIONALDLG_H
