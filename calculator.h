#ifndef CALCULATOR_H
#define CALCULATOR_H

#include <QWidget> // 包含 QWidget 类的头文件，用于创建应用程序的窗口

class QLineEdit; // 前向声明 QLineEdit 类，减少头文件依赖
class QPushButton; // 前向声明 QPushButton 类，减少头文件依赖

class Calculator : public QWidget {
    Q_OBJECT // Qt 宏，启用信号和槽机制

private:
    QString currentOperation; // 当前的操作符
    QString firstOperand; // 第一个操作数
    QLineEdit *display; // 显示输入和结果的文本框
    QPushButton *createButton(const QString &text); // 创建按钮的辅助函数
//    QPushButton *createDesButton(const QString &text, const char *member);
    void calculate(); // 执行计算操作的辅助函数

public:
    explicit Calculator(QWidget *parent = nullptr); // 公有构造函数，带一个可选的父指针参数

private slots: // 私有槽函数，用于响应信号
    void onButtonClicked(); // 槽函数，处理按钮点击事件
};

#endif // CALCULATOR_H
