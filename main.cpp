#include "quizzui.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setStyleSheet(R"(
QPushButton {
    background-color: #3498db;
    color: white;
    border: none;
    border-radius: 10px;
    font-size: 16px;
    font-weight: 700;
    padding-top: 4px;
    padding-bottom: 4px;
}

QLabel{
    font-size: 16px;
    font-weight: 700;
}
QPushButton:disabled {
    background-color: #103752;
}

QPushButton:hover {
    background-color: #5dade3;
}

QPushButton#mediaUI {
    padding-top: 8px;
    padding-bottom: 8px;
}

QLabel[class="question"]{
    font-size: 40px;
    font-weight: 800;
}




)");
    QuizzUI w;
    w.show();
    return a.exec();
}
