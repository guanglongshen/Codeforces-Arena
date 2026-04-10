#ifndef PROFILES_H
#define PROFILES_H

#include <QWidget>
#include <QLabel>

class Profiles : public QWidget {
    Q_OBJECT
public:
    explicit Profiles(QWidget *parent = nullptr);

signals:

private:
    void initUI();

    QFrame *userInfo;
    QLabel *avatar;
    QLabel *name;
    QLabel *rating;
    QLabel *rank, *maxRank;
    QLabel *contribution;

public slots:
    // 接收用户信息
    void onUserInfoReceived(const QByteArray &data);
};

#endif // PROFILES_H
