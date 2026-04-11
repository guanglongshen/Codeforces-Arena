#ifndef CIRCLEIMAGELABEL_H
#define CIRCLEIMAGELABEL_H

#include <QLabel>


class CircleImageLabel : public QLabel {
public:
    explicit CircleImageLabel(QWidget *parent = nullptr);;
    void setCirclePixmap(const QPixmap &pixmap);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QPixmap rawPixmap;
};

#endif // CIRCLEIMAGELABEL_H
