#include "handledialog.h"
#include <QPixmap>
#include <QSettings>
#include <QVBoxLayout>
#include <QHBoxLayout>

HandleDialog::HandleDialog() {
    this->setWindowTitle(tr("Set Default Handle"));
    this->setWindowIcon(QIcon(":/pictures/assets/handleSetter.png"));
    this->setStyleSheet(
        "HandleDialog {"  // 最好指定类名，防止影响内部控件
        "   background: qradialgradient("
        "       cx:0.5, cy:0.5, radius:0.7,"
        "       fx:0.5, fy:0.5,"
        "       stop:0 rgba(255, 255, 255, 255)," // 中心纯白
        "       stop:1 rgba(173, 216, 230, 255)"  // 边缘浅蓝 (LightBlue)
        "   );"
        "}"
    );

    pic = new QLabel;
    QPixmap pix(":/pictures/assets/cfFullLogo.png");
    pic->setPixmap(pix);
    pic->setAlignment(Qt::AlignCenter);

    handleTxt = new QLineEdit;
    handleTxt->setPlaceholderText(tr("Write your Codeforces handle here..."));
    askCheckbox = new QCheckBox;

    QSettings settings;
    bool ask = settings.value("handle", false).toBool();
    askCheckbox->setChecked(ask);

    askTip = new QLabel(tr("Ask me everytime the application starts"));
    QHBoxLayout *hlayout = new QHBoxLayout;
    hlayout->addWidget(askCheckbox);
    hlayout->addWidget(askTip);
    hlayout->addStretch();

    cancelBtn = new QPushButton(tr("Cancel"));
    setBtn = new QPushButton(tr("Set"));
    QHBoxLayout *btns = new QHBoxLayout;
    btns->addStretch();
    btns->addWidget(cancelBtn);
    btns->addWidget(setBtn);

    QVBoxLayout *vlayout = new QVBoxLayout;
    vlayout->addWidget(pic);        // CF logo
    vlayout->addWidget(handleTxt);  // 输入栏
    vlayout->addLayout(hlayout);    // 每次启动是否弹出设置用户名
    vlayout->addStretch();
    vlayout->addLayout(btns);       // 按钮

    this->setLayout(vlayout);

    connect(cancelBtn, &QPushButton::clicked, this, &QDialog::reject);
    connect(setBtn, &QPushButton::clicked, this, &QDialog::accept);
}

QString HandleDialog::getHandle() {
    return handleTxt->text();
}
