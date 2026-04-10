#include "profiles.h"

#include <QJsonArray>
#include <QJsonDocument>
#include <QJsonObject>
#include <QLabel>
#include <QSettings>
#include <QVBoxLayout>

Profiles::Profiles(QWidget *parent)
    : QWidget{parent} {
    initUI();
}

void Profiles::initUI() {
    QVBoxLayout *mainLayout = new QVBoxLayout;
    this->setLayout(mainLayout);

    userInfo = new QFrame;
    userInfo->setFrameShape(QFrame::StyledPanel);
    userInfo->setFrameShadow(QFrame::Raised);
    userInfo->setLineWidth(2);
    mainLayout->addWidget(userInfo);
    mainLayout->addStretch(8);

    QHBoxLayout *hUserInfo = new QHBoxLayout;
    QVBoxLayout *v1Text = new QVBoxLayout;
    QVBoxLayout *v2Avatar = new QVBoxLayout;
    userInfo->setLayout(hUserInfo);
    hUserInfo->addLayout(v1Text, 6);
    hUserInfo->addLayout(v2Avatar, 4);

    QHBoxLayout *hrank = new QHBoxLayout;
    hrank->setContentsMargins(0, 0, 0, 0);
    QLabel *rankPic = new QLabel;
    rankPic->setPixmap(QPixmap(":/pictures/assets/rank.png").scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    rank = new QLabel(tr("Rank loading..."));
    hrank->addWidget(rankPic);
    hrank->addWidget(rank);
    hrank->addStretch();

    QHBoxLayout *hname = new QHBoxLayout;
    hname->setContentsMargins(0, 0, 0, 0);
    QLabel *namePic = new QLabel;
    namePic->setPixmap(QPixmap(":/pictures/assets/name.png").scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    name = new QLabel(tr("Handle loading..."));
    hname->addWidget(namePic);
    hname->addWidget(name);
    hname->addStretch();

    QHBoxLayout *hrating = new QHBoxLayout;
    hrating->setContentsMargins(0, 0, 0, 0);
    QLabel *ratingPic = new QLabel;
    ratingPic->setPixmap(QPixmap(":/pictures/assets/rating.png").scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    rating = new QLabel(tr("Contest rating: ? "));
    maxRank = new QLabel("(max. ?)");
    hrating->addWidget(ratingPic);
    hrating->addWidget(rating);
    hrating->addWidget(maxRank);
    hrating->addStretch();

    QHBoxLayout *hcontribution = new QHBoxLayout;
    hcontribution->setContentsMargins(0, 0, 0, 0);
    QLabel *contributionPic = new QLabel;
    contributionPic->setPixmap(QPixmap(":/pictures/assets/contribution.png").scaled(16, 16, Qt::KeepAspectRatio, Qt::SmoothTransformation));
    contribution = new QLabel(tr("Contribution: ?"));
    hcontribution->addWidget(contributionPic);
    hcontribution->addWidget(contribution);
    hcontribution->addStretch();

    avatar = new QLabel(tr("pic"));
    avatar->setFixedSize(64, 64);

    v1Text->addLayout(hrank);
    v1Text->addLayout(hname);
    v1Text->addLayout(hrating);
    v1Text->addLayout(hcontribution);
    v1Text->addStretch();
    v2Avatar->addWidget(avatar);
    v2Avatar->addStretch();
}

void Profiles::onUserInfoReceived(const QByteArray &data) {

    QJsonDocument doc = QJsonDocument::fromJson(data);
    QJsonObject obj = doc.object();

    QString handle, rank, mxRank, avatar;
    int rating, mxRating, contribution;

    if (obj["status"].toString() == "OK") {
        qDebug() << "Get Data";

        QJsonArray resultArray = obj["result"].toArray();
        if (!resultArray.isEmpty()) {
            QJsonObject userObj = resultArray.first().toObject();

            handle = userObj["handle"].toString();
            rank = userObj["rank"].toString();
            rating = userObj["rating"].toInt();
            mxRank = userObj["maxRank"].toString();
            mxRating = userObj["maxRating"].toInt();
            avatar = userObj["avatar"].toString();
            contribution = userObj["contribution"].toInt();
        }
    } else {
        qDebug() << "API ERROR: " << obj["comment"].toString();
        return ;
    }

    if (!rank.isEmpty()) {
        rank[0] = rank[0].toUpper();
    }

    QString color, mxColor;
    if (rating < 1200) color = "#808080";
    else if (rating < 1400) color = "#008000";
    else if (rating < 1600) color = "#03a89e";
    else if (rating < 1900) color = "#0000ff";
    else if (rating < 2100) color = "#aa00aa";
    else if (rating < 2400) color = "#ff8c00";
    else if (rating < 4000) color = "#ff0000";
    else color = "#000000";

    if (mxRating < 1200) mxColor = "#808080";
    else if (mxRating < 1400) mxColor = "#008000";
    else if (mxRating < 1600) mxColor = "#03a89e";
    else if (mxRating < 1900) mxColor = "#0000ff";
    else if (mxRating < 2100) mxColor = "#aa00aa";
    else if (mxRating < 2400) mxColor = "#ff8c00";
    else if (mxRating < 4000) mxColor = "#ff0000";
    else mxColor = "#000000";

    if (rating == 0) {
        this->rank->setText(QString("%1").arg(rank));
        this->name->setText(QString("%1").arg(handle));
        this->rating->setText(QString("Contest rating: %1").arg(rating));

        this->maxRank->clear();
    } else {
        this->rank->setText(QString("<span style='color:%1'><b>%2</b></span>").arg(color, rank));

        if (rating >= 3000 && rating < 4000) {  // 黑红名
            this->name->setText(QString("<span style='color:#000000;'><b>%1</b></span><span style='color:#ff0000;'><b>%2</b></span>").arg(handle.left(1), handle.mid(1)));
        } else {                                // 正常
            this->name->setText(QString("<span style='color:%1'><b>%2</b></span>").arg(color, handle));
        }

        this->rating->setText(QString("Contest rating: <span style='color:%1'><b>%2</b></span>").arg(color, QString::number(rating)));

        this->maxRank->setText(QString("(max. <span style='color:%1;'><b>%2, %3</b></span>)").arg(mxColor, mxRank, QString::number(mxRating)));
    }

    if (contribution == 0) {
        this->contribution->setText(QString("Contribution: <span style='color:#808080;'><b>%1</b></span>").arg(contribution));
    } else if (contribution > 0) {
        this->contribution->setText(QString("Contribution: <span style='color:green;'><b>+%1</b></span>").arg(contribution));
    } else {
        this->contribution->setText(QString("Contribution: <span style='color:red;'><b>-%1</b></span>").arg(contribution));
    }
}
