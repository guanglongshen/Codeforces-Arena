#include "handledialog.h"
#include <QPixmap>
#include <QSettings>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QToolTip>
#include <QTimer>

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
    connect(setBtn, &QPushButton::clicked, this, [this](){
        // 先检查用户名是否为空
        if (handleTxt->text().trimmed().isEmpty()) {
            handleTxt->clear();

            // 只有当 StyleSheet 不包含样式，才会设置红色醒目，避免多次点击 Set 而触发多个 QTimer
            if (!handleTxt->styleSheet().contains("red")) {
                handleTxt->setStyleSheet("border: 1px solid red; background-color: #FFF0F0;");

                // 2s 后恢复默认
                QTimer::singleShot(2000, this, [this](){
                    handleTxt->setStyleSheet("");
                });
            }

            handleTxt->setFocus();
            QToolTip::showText(handleTxt->mapToGlobal(QPoint(handleTxt->width(), -15)), tr("Handle cannot be null or spaces!"), handleTxt);
            return ;
        }

        this->accept();
    });

    // 2s 内可能用户修改了 handleTxt
    connect(handleTxt, &QLineEdit::textChanged, this, [this](const QString &txt){
        if (!txt.trimmed().isEmpty()) {
            handleTxt->setStyleSheet("");
        }
    });
}

QString HandleDialog::getHandle() {
    return handleTxt->text().trimmed();
}

bool HandleDialog::getAskHandle() {
    return askCheckbox->checkState();
}
