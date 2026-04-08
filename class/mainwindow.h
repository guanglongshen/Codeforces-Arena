#ifndef MAINWINDOW_H
#define MAINWINDOW_H

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

private slots:
    // 接收用户信息
    void onUserInfoReceived(const QByteArray &data);
};
#endif // MAINWINDOW_H
