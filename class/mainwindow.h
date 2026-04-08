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
    // Handle 是否需要设置
    bool handleSet;
    QString username;

    QMenuBar *mBar;

    // 主要标签
    QTabWidget *mainTags;

    void handleCheck();
    void menuInit();

private slots:

};
#endif // MAINWINDOW_H
