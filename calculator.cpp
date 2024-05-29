#include "Calculator.h" // 包含 Calculator 类的头文件
#include <QGridLayout> // 包含 QGridLayout 类的头文件，用于网格布局
#include <QLineEdit> // 包含 QLineEdit 类的头文件，用于单行文本框
#include <QPushButton> // 包含 QPushButton 类的头文件，用于按钮
#include <QVBoxLayout> // 包含 QVBoxLayout 类的头文件，用于垂直布局
#include <QHBoxLayout> // 包含 QHBoxLayout 类的头文件，用于水平布局
#include <QIntValidator> // 包含 QIntValidator 类的头文件，用于整数验证器
#include <QMessageBox> // 包含 QMessageBox 类的头文件，用于消息框
#include <QFont> // 包含 QFont 类的头文件，用于设置字体

// Calculator 类的构造函数，初始化计算器界面
Calculator::Calculator(QWidget *parent) : QWidget(parent) {
    // 创建显示框
    display = new QLineEdit;
    display->setReadOnly(true); // 设置显示框为只读
    display->setAlignment(Qt::AlignRight); // 设置文本右对齐
    display->setMaxLength(16); // 设置最大输入长度为16个字符
    display->setValidator(new QIntValidator(0, 0xFFFFFF, this)); // 限制输入16进制数
    display->setFixedSize(500, 80); // 设置显示框的固定大小

    // 设置显示框的字体
    QFont font;
    font.setPointSize(20); // 设置字体大小为20
    display->setFont(font);

    // 创建网格布局，用于排列按钮
    QGridLayout *mainLayout = new QGridLayout;

    // 定义按钮文本数组
    QString buttons[16] = {"0", "1", "2", "3", "4", "5", "6", "7",
                           "8", "9", "A", "B", "C", "D", "E", "F"};

    int pos = 0; // 按钮位置索引
    for (int i = 0; i < 4; ++i) { // 外层循环，4行
        for (int j = 0; j < 4; ++j) { // 内层循环，4列
            QPushButton *button = createButton(buttons[pos]); // 创建按钮
            mainLayout->addWidget(button, i, j); // 将按钮添加到网格布局中
            ++pos; // 增加按钮位置索引
        }
    }

    // 创建操作符按钮
    QPushButton *addButton = createButton("+");
    QPushButton *subButton = createButton("-");
    QPushButton *mulButton = createButton("*");
    QPushButton *divButton = createButton("/");

    // 创建等号按钮
    QPushButton *eqButton = createButton("=");

    // 创建垂直布局，用于排列操作符按钮
    QVBoxLayout *opLayout = new QVBoxLayout;
    opLayout->addWidget(addButton); // 添加加法按钮到布局
    opLayout->addWidget(subButton); // 添加减法按钮到布局
    opLayout->addWidget(mulButton); // 添加乘法按钮到布局
    opLayout->addWidget(divButton); // 添加除法按钮到布局
    opLayout->addWidget(eqButton); // 添加等号按钮到布局

    // 将操作符按钮布局添加到网格布局的右侧
    mainLayout->addLayout(opLayout, 0, 4, 4, 1);

    // 创建垂直布局，将显示框和网格布局添加到布局中
    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(display); // 添加显示框到布局
    layout->addLayout(mainLayout); // 添加网格布局到布局

    setLayout(layout); // 设置窗口布局
    setWindowTitle("Hex Calculator"); // 设置窗口标题

    // 创建归零按钮
    QPushButton *zeroButton = createButton("归零");
    mainLayout->addWidget(zeroButton, 4, 0); // 添加归零按钮到网格布局的第五行第一列

    // 连接归零按钮的点击信号到槽函数
    connect(zeroButton, &QPushButton::clicked, this, [this]() {
        display->setText(" "); // 将显示框的文本设置为空格
    });
}

// 创建按钮的辅助函数
QPushButton* Calculator::createButton(const QString &text) {
    QPushButton *button = new QPushButton(text); // 创建按钮
    button->setMinimumSize(40, 40); // 设置按钮的最小大小
    connect(button, &QPushButton::clicked, this, &Calculator::onButtonClicked); // 连接按钮的点击信号到槽函数
    return button; // 返回创建的按钮
}

// 按钮点击事件的槽函数
void Calculator::onButtonClicked() {
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender()); // 获取被点击的按钮
    QString clickedText = clickedButton->text(); // 获取按钮文本

    if (clickedText == "=") { // 如果点击的是等于按钮
        calculate(currentOperation); // 执行计算操作
    } else if (clickedText == "+" || clickedText == "-" || clickedText == "*" || clickedText == "/") {
        // 如果点击的是加减乘除按钮，则更新当前操作符
        currentOperation = clickedText;
        firstOperand = display->text(); // 保存当前显示的值作为第一个操作数
        display->clear(); // 清空显示框，等待输入第二个操作数
    } else {
        display->setText(display->text() + clickedText); // 将按钮文本添加到显示框中
    }
}

// 计算操作的函数
void Calculator::calculate(const QString &operation) {
    QString secondOperand = display->text(); // 获取第二个操作数

    // 将十六进制字符串转换为整数
    bool ok;
    qint64 first = firstOperand.toInt(&ok, 16);
    qint64 second = secondOperand.toInt(&ok, 16);

    // 检查操作数的范围是否超出预期范围
    if (first > static_cast<qint64>(0xFFFFFFFF) || first < static_cast<qint64>(0xFFFFFFFF) * -1 ||
        second > static_cast<qint64>(0xFFFFFFFF) || second < static_cast<qint64>(0xFFFFFFFF) * -1) {
        display->setText("Undefined"); // 设置显示框为 "Undefined"
        return; // 结束函数
    }

    // 执行相应的计算操作
    qint64 result;
    if (operation == "+") {
        result = first + second;
    } else if (operation == "-") {
        result = first - second;
    } else if (operation == "*") {
        result = first * second;
    } else if (operation == "/") {
        if (second != 0) {
            result = first / second;
        } else {
            display->setText("Undefined"); // 设置显示框为 "Undefined"
            return; // 结束函数
        }
    }

    // 检查计算结果是否超出预期范围
    if (result > static_cast<qint64>(0xFFFFFFFF) || result < static_cast<qint64>(0xFFFFFFFF) * -1) {
        display->setText("Undefined"); // 设置显示框为 "Undefined"
        return; // 结束函数
    }

    // 将计算结果转换为十六进制字符串并显示在显示框中
    display->setText(QString::number(result, 16).toUpper());
}
