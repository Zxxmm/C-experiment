#include <QApplication> // 包含 QApplication 类的头文件，用于创建 Qt 应用程序
#include "Calculator.h" // 包含自定义的 Calculator 类的头文件

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);  // 创建 QApplication 对象，管理应用程序的控制流和主要设置
    Calculator calculator;  // 创建 Calculator 对象
    calculator.show();  // 显示 Calculator 窗口
    return app.exec();  // 进入 Qt 的事件循环，等待和处理用户输入
}
