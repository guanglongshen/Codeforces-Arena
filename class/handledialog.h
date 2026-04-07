#ifndef HANDLEDIALOG_H
#define HANDLEDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QLineEdit>
#include <QCheckBox>
#include <QPushButton>

class HandleDialog : public QDialog {
    Q_OBJECT
public:
    HandleDialog();
    QString getHandle();

private:
    QLabel *askTip;
    QLabel *pic;
    QLineEdit *handleTxt;
    QCheckBox *askCheckbox;
    QPushButton *cancelBtn;
    QPushButton *setBtn;
};

#endif // HANDLEDIALOG_H
