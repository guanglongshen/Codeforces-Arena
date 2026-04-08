#include "cfapiworker.h"
#include <QUrl>
#include <QUrlQuery>
#include <QNetworkRequest>
#include <QNetworkReply>

CFApiWorker::CFApiWorker(QObject *parent)
    : QObject{parent} {}

void CFApiWorker::requestUserInfo(const QString &Handle) {
    // 这个请求将作为子线程去处理，此时 manager 创建，避免在构造函数内绑定到主线程
    if (!manager) {
        manager = new QNetworkAccessManager(this);
    }

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

    // 异步 Get 请求
    QNetworkReply *reply = manager->get(request);

    // 监听请求
    connect(reply, &QNetworkReply::finished, this, [this, reply](){
        // reply 需要销毁
        reply->deleteLater();

        // 网络检测
        if (reply->error() != QNetworkReply::NoError) {
            // TODO 错误情况的提示
            qDebug() << "Network Error: " << reply->errorString();
            return ;
        }

        // 读取获得的数据
        QByteArray responseData = reply->readAll();

        // 发射信号，将得到的数据传回
        emit userInfoData(responseData);
    });
}
