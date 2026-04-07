#include "profiles.h"

#include <QLabel>
#include <QVBoxLayout>

Profiles::Profiles(QWidget *parent)
    : QWidget{parent} {
    QLabel *justTest = new QLabel("Hello, Codeforces Arena!");
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(justTest);

    this->setLayout(layout);
}
