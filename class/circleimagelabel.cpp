#include "circleimagelabel.h"

#include <QPainter>
#include <QPainterPath>


CircleImageLabel::CircleImageLabel(QWidget *parent) : QLabel(parent) {
    // 在 Label 内缩放保持平滑
    setScaledContents(false);
}

void CircleImageLabel::setCirclePixmap(const QPixmap &pixmap) {
    rawPixmap = pixmap;
    update();   // 触发重新绘制
}

void CircleImageLabel::paintEvent(QPaintEvent *event) {
    // 无图片，执行 Label 默认逻辑
    if (rawPixmap.isNull()) {
        QLabel::paintEvent(event);
        return ;
    }

    QPainter painter(this);
    // 抗锯齿
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setRenderHint(QPainter::SmoothPixmapTransform);

    // 圆形半径
    int diameter = qMin(width(), height());
    int x = (width() - diameter) / 2;
    int y = (height() - diameter) / 2;

    painter.save();

    QPainterPath path;
    path.addEllipse(x, y, diameter, diameter);

    // 将绘图区域裁剪
    painter.setClipPath(path);

    QPixmap scaledPixmap = rawPixmap.scaled(size(), Qt::KeepAspectRatioByExpanding, Qt::SmoothTransformation);
    int pixX = (width() - scaledPixmap.width()) / 2;
    int pixY = (height() - scaledPixmap.height()) / 2;
    painter.drawPixmap(pixX, pixY, scaledPixmap);

    painter.restore();
    QPen pen;
    pen.setColor(QColor("gray"));
    painter.setPen(pen);

    painter.drawEllipse(x + 1, y + 1, diameter - 2, diameter - 2);
}


