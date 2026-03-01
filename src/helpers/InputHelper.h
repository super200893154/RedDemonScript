#ifndef INPUTHELPER_H
#define INPUTHELPER_H

#include <QString>
#include <QThread>

/**
 * @brief 输入辅助类，封装鼠标键盘模拟功能
 */
class InputHelper
{
public:
    InputHelper();
    ~InputHelper();

    /**
     * @brief 点击指定坐标
     * @param x X坐标
     * @param y Y坐标
     */
    static void click(int x, int y);

    /**
     * @brief 移动鼠标到指定坐标
     * @param x X坐标
     * @param y Y坐标
     */
    static void move(int x, int y);

    /**
     * @brief 按下键盘按键
     * @param key 键值（如 "A", "Enter", "Space" 等）
     */
    static void pressKey(const QString& key);

    /**
     * @brief 输入文本
     * @param text 要输入的文本
     */
    static void typeText(const QString& text);

    /**
     * @brief 等待一段时间
     * @param milliseconds 等待毫秒数
     */
    static void wait(int milliseconds);

private:
    // 私有成员和辅助方法
};

#endif // INPUTHELPER_H