#include "Calculator.h"
#include <QGridLayout>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QIntValidator>
#include <QMessageBox>

Calculator::Calculator(QWidget *parent) : QWidget(parent) {
    display = new QLineEdit;
    display->setReadOnly(true);
    display->setAlignment(Qt::AlignRight);
    display->setMaxLength(16);
    display->setValidator(new QIntValidator(0, 0xFFFFFFFF, this)); // 限制输入16进制数

    QGridLayout *mainLayout = new QGridLayout;

    QString buttons[16] = {"0", "1", "2", "3", "4", "5", "6", "7",
                           "8", "9", "A", "B", "C", "D", "E", "F"};

    int pos = 0;
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            QPushButton *button = createButton(buttons[pos]);
            mainLayout->addWidget(button, i, j);
            ++pos;
        }
    }

    QPushButton *addButton = createButton("+");
    QPushButton *subButton = createButton("-");
    QPushButton *mulButton = createButton("*");
    QPushButton *divButton = createButton("/");
    QPushButton *eqButton = createButton("=");

    QVBoxLayout *opLayout = new QVBoxLayout;
    opLayout->addWidget(addButton);
    opLayout->addWidget(subButton);
    opLayout->addWidget(mulButton);
    opLayout->addWidget(divButton);
    opLayout->addWidget(eqButton);

    mainLayout->addLayout(opLayout, 0, 4, 4, 1);

    QVBoxLayout *layout = new QVBoxLayout;
    layout->addWidget(display);
    layout->addLayout(mainLayout);

    setLayout(layout);
    setWindowTitle("Hex Calculator");
}

QPushButton* Calculator::createButton(const QString &text) {
    QPushButton *button = new QPushButton(text);
    button->setMinimumSize(40, 40);
    connect(button, &QPushButton::clicked, this, &Calculator::onButtonClicked);
    return button;
}

void Calculator::onButtonClicked() {
    QPushButton *clickedButton = qobject_cast<QPushButton*>(sender());
    QString clickedText = clickedButton->text();

    if (clickedText == "=") {
        // 执行计算操作
    } else {
        display->setText(display->text() + clickedText);
    }
}

void Calculator::calculate(const QString &operation) {
    // 实现计算逻辑
}
