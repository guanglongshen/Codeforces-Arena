#include "cfapiworker.h"
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkReply>

CFApiWorker::CFApiWorker(QObject *parent)
    : QObject{parent} {
    timer = new QTimer(this);
    // 定时器间隔 3s，CF 官方推荐至少 2s，保险起见设置为 3s
    timer->setInterval(3000);

    // 定时器一旦时间到，去处理队列内的 API 请求
    connect(timer, &QTimer::timeout, this, &CFApiWorker::processQueue);
}

CFApiWorker::~CFApiWorker() {
    if (timer) {
        timer->stop();
    }
}

// 只做入队列操作
void CFApiWorker::requestUserInfo(const QString &Handle) {
    // 请求查询用户信息
    // handles (Required)	Semicolon-separated list of handles. No more than 10000 handles is accepted.
    // checkHistoricHandles	Boolean, the default value is true. If this flag is enabled, then use the history of handle changes when searching for a user.
    //
    // Return value: Returns a list of User objects for requested handles.
    // Example [https://codeforces.com/api/user.info?handles=DmitriyH;Fefer_Ivan&checkHistoricHandles=false]
    QUrl url(apiUserInfo);
    QUrlQuery query;
    query.addQueryItem("handles", Handle);
    query.addQueryItem("checkHistoricHandles", "false");
    url.setQuery(query);

    QNetworkRequest request(url);
    // 服务器可能会拦截没有 Agent 的请求
    request.setHeader(QNetworkRequest::UserAgentHeader, "Qt6 CF Arena Client");

    ApiTask task = { ApiType::USERINFO, request };
    taskQueue.enqueue(task);

    // 定时器未启动，则说明这个请求是第一个
    if (!timer->isActive()) {
        QTimer::singleShot(0, this, &CFApiWorker::processQueue);
        timer->start();
    }
}

void CFApiWorker::processQueue() {
    // 队列空 则返回，不需要处理
    if (taskQueue.isEmpty()) {
        return ;
    }

    if (!manager) {
        manager = new QNetworkAccessManager(this);
    }

    // 执行网络请求
    ApiTask task = taskQueue.dequeue();
    QNetworkReply *reply = manager->get(task.request);

    // 将得到的数据通过信号转发回 MainWindow
    connect(reply, &QNetworkReply::finished, this, [this, reply, task]() {
        if (reply->error() == QNetworkReply::NoError) {
            switch (task.type) {
            case ApiType::USERINFO:
                emit sig_GetUserInfo(reply->readAll());
                break;
            }
        } else {
            emit sig_ErrorOccurred(task.type, reply->readAll(), reply->errorString());
        }
        reply->deleteLater();
    });
}
