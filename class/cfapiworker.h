#ifndef CFAPIWORKER_H
#define CFAPIWORKER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QQueue>
#include <QTimer>
#include <QThread>

enum class ApiType {
    USERINFO
};

struct ApiTask {
    ApiType type;
    QNetworkRequest request;
};

class CFApiWorker : public QObject {
    Q_OBJECT
public:
    explicit CFApiWorker(QObject *parent = nullptr);
    ~CFApiWorker() override;

public slots:
    void requestUserInfo(const QString &Handle);

private slots:
    // 定时器触发检测队列
    void processQueue();

signals:
    // 查询成功后 分别发送成功信号，将数据传递回 mainWindow 处理
    void sig_GetUserInfo(const QByteArray &data);
    void sig_ErrorOccurred(const ApiType &type, const QByteArray &data, const QString &error);

private:
    QNetworkAccessManager *manager = nullptr;
    QQueue<ApiTask> taskQueue;
    QTimer *timer;

    // 系列 public api 接口
    QString apiUserInfo = "https://codeforces.com/api/user.info?";
};

#endif // CFAPIWORKER_H
