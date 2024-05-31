#include "Calculator.h" // 包含 Calculator 类的头文件
#include <QGridLayout> // 包含 QGridLayout 类的头文件，用于网格布局
#include <QLineEdit> // 包含 QLineEdit 类的头文件，用于单行文本框
#include <QPushButton> // 包含 QPushButton 类的头文件，用于按钮
#include <QIntValidator> // 包含 QIntValidator 类的头文件，用于整数验证器
#include <QFont> // 包含 QFont 类的头文件，用于设置字体
#include <QQueue>
#include <QStack>
#include <iostream>

bool flag = false;//false代表不能输入操作符


bool isOperator(QChar c) {// 判断是否为操作符
    return c == '+' || c == '-' || c == '*' || c == '/' || c == '%' || c == '^';
}

// 将十进制整数转换为十六进制字符串
QString TenToSixteen(qint64 res) {
    if (res == 0) return "0";
    bool isNegative = res < 0;// 判断是否为负数
    if (isNegative) res = -res;
    QString s;
    while (res) {
        qint64 k = res % 16;
        if (k < 10) s.prepend(QChar(static_cast<int>('0' + k)));
        else s.prepend(QChar(static_cast<int>('A' + k - 10)));
        res = res / 16;
    }
    if (isNegative) s.prepend('-');
    return s;
}

bool isHex(QChar c) {// 判断是否为十六进制数
    return c.isDigit() || (c >= 'A' && c <= 'F');
}

int precedence(const QString &op) {// 判断操作符的优先级
    if (op == '+' || op == '-') return 1;
    if (op == '*' || op == '/') return 2;
    if (op == '^') return 3;
    return 0;

}

qint64 applyOperator(qint64 left, qint64 right, QChar op, QLineEdit *display) {// 计算每一个独立表达式
    switch (op.toLatin1()) {
        case '+':
            return left + right;
        case '-':
            return left - right;
        case '*':
            return left * right;
        case '/':
            if (right != 0) return left / right;
            display->setText("除数不能为0！");
            return 0;
        case '%':
            if (right != 0) return left % right;
            display->setText("除数不能为0！");
            return 0;
        case '^':
            return pow(left, right);
        default:
            display->setText("无效的操作符！");
            return 0;
    }
}

// 将中缀表达式处理成后缀表达式
QQueue<QString> infixToPostfix(const QString &infix) {
    QStack<QChar> operators;// 保存临时的操作符
    QQueue<QString> output;// 保存后缀表达式
    int i = 0;
    while (i < infix.length()) {
        if (isHex(infix[i])) {// 判断是否为十六进制数的第一位
            QString number;// 保存一个十六进制数
            while (i < infix.length() && isHex(infix[i])) {// 判断是否为十六进制数的后续位
                number.append(infix[i]);// 添加到 number 中
                ++i;
            }
            output.enqueue(number);// 将 number 添加到 output 队列中
        } else if (isOperator(infix[i])) {// 判断是否为操作符
            while (!operators.isEmpty() &&
                   precedence(operators.top()) >= precedence(infix[i])) {//如果操作符栈不为空且栈顶操作符优先级大于等于当前操作符
                output.enqueue(QString(operators.pop()));//将栈顶操作符出栈并添加到 output 队列中
            }
            operators.push(infix[i]);// 将优先级低的当前操作符入栈
            ++i;
        } else {
            ++i; // 跳过空格或者意外的操作符
        }
    }
    while (!operators.isEmpty()) {
        output.enqueue(QString(operators.pop()));// 将剩余的操作符添加到 output 队列中
    }
    return output;
}

// 计算一个十六进制的后缀表达式
qint64 evaluatePostfix(const QQueue<QString> &postfix, QLineEdit *display) {
    QStack<qint64> stack;
    QQueue<QString> tempQueue = postfix;
    while (!tempQueue.isEmpty()) {
        QString token = tempQueue.dequeue();// 从队列中取出一个 token
        if (token.length() > 0 && isHex(token[0])) {//如果是数字，入栈
            bool ok;
            qint64 value = token.toInt(&ok, 16);//将十六进制的token转化为int
            if (ok) {
                stack.push(value);
            }
        } else if (isOperator(token[0])) {//如果是操作符，弹出两个数进行计算
            qint64 right = stack.pop();
            qint64 left = stack.pop();
            qint64 result = applyOperator(left, right, token[0], display);
            stack.push(result);
        }
    }
    return stack.top();
}

// 计算操作的函数
void Calculator::calculate() {
    QString expression = display->text(); // 获取输入的表达式
    QQueue<QString> postfix = infixToPostfix(expression);
    qint64 result = evaluatePostfix(postfix, display);
    // 检查计算结果是否超出预期范围
    if (result > static_cast<qint64>(0xFFFFFFFFFFFFFFF) || result < -static_cast<qint64>(0xFFFFFFFFFFFFFFF)) {
        display->setText("Undefined"); // 设置显示框为 "Undefined"
        return; // 结束函数
    }
    // 将计算结果转换为十六进制字符串并显示在显示框中
    if (display->text() != "除数不能为0！") {
        flag = false;
        display->setText(TenToSixteen(result));
    }
}

// Calculator 类的构造函数，初始化计算器界面
Calculator::Calculator(QWidget *parent) : QWidget(parent) {
    // 创建显示框
    display = new QLineEdit;
    display->setReadOnly(true); // 设置显示框为只读
    display->setAlignment(Qt::AlignRight); // 设置文本右对齐
    display->setMaxLength(16); // 设置最大输入长度为16个字符
    display->setFixedSize(420, 80); // 设置显示框的固定大小

    // 设置显示框的字体
    QFont font;
    font.setPointSize(20); // 设置字体大小为20
    display->setFont(font);
    display->setText("0");

    // 创建网格布局，用于排列按钮
    auto *mainLayout = new QGridLayout;

    // 定义按钮文本数组
    QString buttons[16] = {"F", "7", "8", "9", "E", "4", "5", "6",
                           "D", "1", "2", "3", "C", "B", "A", "0"};

    int pos = 0; // 按钮位置索引
    for (int i = 1; i <= 4; ++i) { // 外层循环，4行
        for (int j = 0; j < 4; ++j) { // 内层循环，4列
            QPushButton * button = createButton(buttons[pos]); // 创建按钮
            mainLayout->addWidget(button, i, j); // 将按钮添加到网格布局中
            ++pos; // 增加按钮位置索引
        }
    }

    // 创建操作符按钮
    QPushButton * addButton = createButton("+");
    QPushButton * subButton = createButton("-");
    QPushButton * mulButton = createButton("*");
    QPushButton * divButton = createButton("/");
    QPushButton * modButton = createButton("%");
    QPushButton * powButton = createButton("^");

    // 创建等号按钮
    QPushButton * eqButton = createButton("=");

    // 创建垂直布局，用于排列操作符按钮
    auto *opLayout = new QVBoxLayout;
    opLayout->addWidget(addButton); // 添加加法按钮到布局
    opLayout->addWidget(subButton); // 添加减法按钮到布局
    opLayout->addWidget(mulButton); // 添加乘法按钮到布局
    opLayout->addWidget(divButton); // 添加除法按钮到布局
    opLayout->addWidget(modButton); // 添加取模按钮到布局
    opLayout->addWidget(powButton); // 添加次方按钮到布局
    opLayout->addWidget(eqButton);  // 添加等号按钮到布局

    // 将操作符按钮布局添加到网格布局的右侧
    mainLayout->addWidget(eqButton, 4, 4);
    mainLayout->addWidget(addButton, 3, 4);
    mainLayout->addWidget(subButton, 2, 4);
    mainLayout->addWidget(mulButton, 1, 4);
    mainLayout->addWidget(divButton, 0, 4);
    mainLayout->addWidget(modButton, 0, 3);
    mainLayout->addWidget(powButton, 0, 2);

    // 创建垂直布局，将显示框和网格布局添加到布局中
    auto *layout = new QVBoxLayout;
    layout->addWidget(display); // 添加显示框到布局
    layout->addLayout(mainLayout); // 添加网格布局到布局

    setLayout(layout); // 设置窗口布局
    setWindowTitle("Hex Calculator"); // 设置窗口标题

    // 创建归零按钮
    QPushButton * zeroButton = createButton("归零");
    mainLayout->addWidget(zeroButton, 0, 0); // 添加归零按钮到网格布局的第五行第一列

    // 连接归零按钮的点击信号到槽函数
    connect(zeroButton, &QPushButton::clicked, this, [this]() {
        flag = false;
        display->setText("0"); // 将显示框的文本设置为空
    });

    QPushButton * backspaceButton = createDesButton("del", SLOT(backspaceClicked()));
    mainLayout->addWidget(backspaceButton, 0, 1); // 添加归零按钮到网格布局的第五行第一列

    //连接删除按钮的点击信号到槽函数
    connect(backspaceButton, &QPushButton::clicked, this, [this]() { backspaceClicked(); });
}

// 创建按钮的辅助函数
QPushButton *Calculator::createButton(const QString &text) {
    auto *button = new QPushButton(text); // 创建按钮
    button->setMinimumSize(40, 40); // 设置按钮的最小大小
    connect(button, &QPushButton::clicked, this, &Calculator::onButtonClicked); // 连接按钮的点击信号到槽函数
    return button; // 返回创建的按钮
}

// 创建删除按钮的辅助函数
QPushButton *Calculator::createDesButton(const QString &text, const char *member) {
    QPushButton * button = new QPushButton(text);
    button->setMinimumSize(40, 40); // 设置按钮的最小大小
    connect(button, &QPushButton::clicked, this, &Calculator::backspaceClicked);
    return button;
}

// 按钮点击事件的槽函数
void Calculator::onButtonClicked() {
    auto *clickedButton = qobject_cast<QPushButton *>(sender()); // 获取被点击的按钮
    QString clickedText = clickedButton->text(); // 获取按钮文本
    std::string Display = display->text().toStdString();

    //防止重复输入操作符
    if (isOperator(clickedText.toStdString()[0])) {
        if (flag) {
            flag = false;
        } else {
            return;
        }
    }

    if (clickedText == "=") { // 如果点击的是等于按钮
        auto temp = display->text().toStdString();
        if (display->text().isEmpty() || isOperator(temp[temp.length() - 1])) {//如果算式的最后一个字符是操作符，返回0
            display->setText("0"); // 如果显示框为空，显示0
        } else {
            flag = true;
            calculate(); // 执行计算操作
        }
    } else {
        if (display->text().isEmpty() && (isOperator(clickedText.toStdString()[0]))) {
            return;
        } else if (Display == "0" ||
                   (Display[Display.length() - 1] == '0' && (isOperator(Display[Display.length() - 2])))) {
            Display.pop_back();
            flag = true;
            display->setText(QString::fromStdString(Display));
        }
        auto result = display->text() + clickedText;
        std::cout << result.toStdString() << std::endl;//输出当前算式，用于调试
        display->setText(result); // 将按钮文本添加到显示框中
    }
}

bool delFlag = true;

void Calculator::backspaceClicked() {
    if (!delFlag) {
        delFlag = true;
        return;
    }
    delFlag = false;
    std::string Display = display->text().toStdString();
    if (Display.length() > 1) {
        Display.pop_back();
        display->setText(QString::fromStdString(Display));
    } else {
        display->setText("0");
    }
}


