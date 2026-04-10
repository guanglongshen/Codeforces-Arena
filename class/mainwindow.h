#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "cfapiworker.h"
#include "profiles.h"

#include <QMainWindow>
#include <QTabWidget>

class MainWindow : public QMainWindow {
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow() override;

protected:

private:
    QString username;

    QMenuBar *mBar;

    // 主要标签
    QTabWidget *mainTags;
    Profiles *profiles;

    void handleCheck();
    void menuInit();

    QThread *apiThread;
    CFApiWorker *worker;
    void apiWorkerThread();

signals:
    // 请求得到 Handle 用户信息信号
    void sig_requestUserInfo(const QString &Handle);
private slots:
    void onUserInfoReceived();
};
#endif // MAINWINDOW_H
