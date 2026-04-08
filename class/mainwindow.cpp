#include "cfapiworker.h"
#include "handledialog.h"
#include "mainwindow.h"
#include "profiles.h"
#include <QFile>
#include <QDialog>
#include <QTimer>
#include <QSettings>
#include <QJsonDocument>
#include <QMenuBar>
#include <QJsonObject>
#include <QThread>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent) {


    QSettings settings;
    bool handleSet = settings.value("firstRun", false).toBool();
    bool alwaysAsk = settings.value("alwaysAskHandle", false).toBool();
    username = settings.value("username").toString();

    if (!handleSet || alwaysAsk) {
        if (!username.isEmpty()) {
            this->setWindowTitle(tr("Codeforces Arena [ %1 ] - logined").arg(username));
        }

        // 为了使主窗口先出现，将这个事件延迟 handle 检测（第一次运行）
        QTimer::singleShot(0, this, &MainWindow::handleCheck);
    } else {
        // 之前已经设置了正确的 Handle，提取即可
        this->setWindowTitle(tr("Codeforces Arena [ %1 ] - Logined" ).arg(username));
    }

    menuInit();

    // 搭建主界面
    mainTags = new QTabWidget;
    mainTags->addTab(new Profiles(), tr("PROFILES"));
    this->setCentralWidget(mainTags);
}

MainWindow::~MainWindow() = default;

void MainWindow::handleCheck() {
    HandleDialog dialog;

    if (dialog.exec() == QDialog::Accepted) {
        QSettings settings;
        settings.setValue("alwaysAskHandle", dialog.getAskHandle());
        settings.setValue("firstRun", true);

        QString Handlename = dialog.getHandle();
        settings.setValue("username", Handlename);

        // 创建线程去获取信息
        QThread *apiThread = new QThread();
        CFApiWorker *worker = new CFApiWorker();

        worker->moveToThread(apiThread);
        connect(apiThread, &QThread::finished, worker, &QObject::deleteLater);
        connect(apiThread, &QThread::finished, apiThread, &QObject::deleteLater);

        // 线程开始 执行的内容
        connect(apiThread, &QThread::started, worker, [worker, Handlename](){
            worker->requestUserInfo(Handlename);
        });

        // 接收用户信息数据信号，把数据得到
        connect(worker, &CFApiWorker::userInfoData, this, [this](const QByteArray &data){
            this->onUserInfoReceived(data);

            // 完成所有内容，退出线程
            sender()->thread()->quit();
        });

        apiThread->start();
        this->setWindowTitle(tr("Codeforces Arena [ %1 ] - Loading...").arg(Handlename));
    } else {
        qDebug() << "Canceled!";
    }
}

void MainWindow::menuInit() {
    mBar = this->menuBar();
    QMenu *platformMenu = mBar->addMenu(tr("Codeforces Arena(&C)"));

    QAction *changeDefaultHandleAct = platformMenu->addAction(tr("Change Default Handle"));
    platformMenu->addSeparator();
    QAction *exitAct = platformMenu->addAction(tr("Exits"));

    connect(exitAct, &QAction::triggered, this, &QMainWindow::close);
    connect(changeDefaultHandleAct, &QAction::triggered, this, &MainWindow::handleCheck);
}

void MainWindow::onUserInfoReceived(const QByteArray &data) {
    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();

    if (obj["status"].toString() == "OK") {
        qDebug() << "Get Data";

        QJsonArray resultArray = obj["result"].toArray();
        if (!resultArray.isEmpty()) {
            QJsonObject userObj = resultArray.first().toObject();

            // 字段
            qDebug() << userObj["rank"] << " " << userObj["rating"];
            qDebug() << userObj["maxRank"] << " " << userObj["maxRating"];
            qDebug() << userObj["avator"];
        }
    } else {
        qDebug() << "API ERROR: " << obj["comment"].toString();
    }
}


