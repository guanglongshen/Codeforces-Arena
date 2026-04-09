#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "cfapiworker.h"

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

    void handleCheck();
    void menuInit();

    QThread *apiThread;
    CFApiWorker *worker;
    void apiWorkerThread();

signals:
    // 请求得到 Handle 用户信息信号
    void sig_requestUserInfo(const QString &Handle);
private slots:
    // 接收用户信息
    void onUserInfoReceived(const QByteArray &data);
};
#endif // MAINWINDOW_H
