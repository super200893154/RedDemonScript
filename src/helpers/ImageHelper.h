#ifndef IMAGEHELPER_H
#define IMAGEHELPER_H

#include <QString>
#include <QRect>
#include <QColor>
#include <QImage>

/**
 * @brief 图色识别辅助类，封装图像识别功能
 */
class ImageHelper
{
public:
    ImageHelper();
    ~ImageHelper();

    /**
     * @brief 查找图像中的特定区域
     * @param templatePath 模板图像路径
     * @param searchRegion 搜索区域
     * @param threshold 匹配阈值
     * @return 匹配结果区域，如果未找到则返回无效 QRect
     */
    static QRect findImage(const QString& templatePath, const QRect& searchRegion = QRect(), double threshold = 0.8);

    /**
     * @brief 等待图像出现
     * @param templatePath 模板图像路径
     * @param timeout 超时时间（毫秒）
     * @return 是否在超时前找到图像
     */
    static bool waitImage(const QString& templatePath, int timeout = 5000);

    /**
     * @brief 获取指定位置的颜色
     * @param x X坐标
     * @param y Y坐标
     * @return 颜色值
     */
    static QColor getColor(int x, int y);

    /**
     * @brief 截取屏幕指定区域
     * @param region 截取区域
     * @return 截图图像
     */
    static QImage captureScreen(const QRect& region = QRect());

private:
    // 私有成员和辅助方法
};

#endif // IMAGEHELPER_H