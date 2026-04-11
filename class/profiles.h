#ifndef PROFILES_H
#define PROFILES_H

#include "circleimagelabel.h"
#include <QWidget>
#include <QLabel>
#include <QNetworkAccessManager>

class Profiles : public QWidget {
    Q_OBJECT
public:
    explicit Profiles(QWidget *parent = nullptr);

    void downloadImage(const QString &url);
signals:

private:
    void initUI();

    QFrame *userInfo;
    QLabel *name;
    QLabel *rating;
    QLabel *rank, *maxRank;
    QLabel *contribution;
    CircleImageLabel *avatar;
    QNetworkAccessManager *manager;

public slots:
    // 接收用户信息
    void onUserInfoReceived(const QByteArray &data);
};

#endif // PROFILES_H
