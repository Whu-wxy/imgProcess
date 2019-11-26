#ifndef RATIONALDLG_COPY_H
#define RATIONALDLG_COPY_H
#include<QDialog>
#include<QComboBox>
#include<QPushButton>
#include<QGridLayout>
#include<QLabel>

class RationalDlg_copy : public QDialog
{
    Q_OBJECT
public:
    RationalDlg_copy(bool grayImage);
    QComboBox *colorCombo;

protected slots:
    void onConfirm();


};



#endif // RATIONALDLG_COPY_H
