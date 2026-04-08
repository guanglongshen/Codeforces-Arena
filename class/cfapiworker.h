#ifndef CFAPIWORKER_H
#define CFAPIWORKER_H

#include <QObject>
#include <QNetworkAccessManager>

class CFApiWorker : public QObject {
    Q_OBJECT
public:
    explicit CFApiWorker(QObject *parent = nullptr);

public slots:
    void requestUserInfo(const QString &Handle);

signals:
    // 查询成功后 分别发送成功信号，将数据传递回 mainWindow 处理
    void userInfoData(const QByteArray &data);

private:
    QNetworkAccessManager *manager = nullptr;

    // 系列 public api 接口
    QString apiUserInfo = "https://codeforces.com/api/user.info?";
};

#endif // CFAPIWORKER_H
