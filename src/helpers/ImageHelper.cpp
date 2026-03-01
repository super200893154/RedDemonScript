#include "ImageHelper.h"
#include <QScreen>
#include <QApplication>
#include <QPixmap>
#include <QRect>
#include <QColor>
#include <QImage>

ImageHelper::ImageHelper()
{
}

ImageHelper::~ImageHelper()
{
}

QRect ImageHelper::findImage(const QString& templatePath, const QRect& searchRegion, double threshold)
{
    // TODO: 实现图像匹配算法
    // 这里应该使用模板匹配算法来查找图像
    Q_UNUSED(templatePath);
    Q_UNUSED(searchRegion);
    Q_UNUSED(threshold);
    
    // 临时返回无效矩形
    return QRect();
}

bool ImageHelper::waitImage(const QString& templatePath, int timeout)
{
    // TODO: 实现等待图像出现的逻辑
    Q_UNUSED(templatePath);
    Q_UNUSED(timeout);
    
    // 临时返回 true
    return true;
}

QColor ImageHelper::getColor(int x, int y)
{
    QScreen *screen = QApplication::primaryScreen();
    if (screen) {
        QPixmap pixmap = screen->grabWindow(0, x, y, 1, 1);
        if (!pixmap.isNull()) {
            QImage image = pixmap.toImage();
            if (!image.isNull()) {
                return image.pixelColor(0, 0);
            }
        }
    }
    return QColor();
}

QImage ImageHelper::captureScreen(const QRect& region)
{
    QScreen *screen = QApplication::primaryScreen();
    if (screen) {
        QRect captureRect = region;
        if (captureRect.isNull()) {
            captureRect = screen->geometry();
        }
        QPixmap pixmap = screen->grabWindow(0, captureRect.x(), captureRect.y(), 
                                          captureRect.width(), captureRect.height());
        return pixmap.toImage();
    }
    return QImage();
}