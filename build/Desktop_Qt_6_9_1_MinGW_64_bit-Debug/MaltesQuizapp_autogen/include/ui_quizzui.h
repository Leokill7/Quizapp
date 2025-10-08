/********************************************************************************
** Form generated from reading UI file 'quizzui.ui'
**
** Created by: Qt User Interface Compiler version 6.9.1
**
** WARNING! All changes made in this file will be lost when recompiling UI file!
********************************************************************************/

#ifndef UI_QUIZZUI_H
#define UI_QUIZZUI_H

#include <QtCore/QVariant>
#include <QtGui/QAction>
#include <QtWidgets/QApplication>
#include <QtWidgets/QMainWindow>
#include <QtWidgets/QMenu>
#include <QtWidgets/QMenuBar>
#include <QtWidgets/QPushButton>
#include <QtWidgets/QStatusBar>
#include <QtWidgets/QVBoxLayout>
#include <QtWidgets/QWidget>

QT_BEGIN_NAMESPACE

class Ui_QuizzUI
{
public:
    QAction *actionLaden;
    QAction *actionSpeichern;
    QAction *actionStarten;
    QAction *actionStopeen;
    QWidget *centralwidget;
    QVBoxLayout *verticalLayout;
    QVBoxLayout *categoryContainer;
    QPushButton *pushButton;
    QMenuBar *menubar;
    QMenu *menuQuiz;
    QStatusBar *statusbar;

    void setupUi(QMainWindow *QuizzUI)
    {
        if (QuizzUI->objectName().isEmpty())
            QuizzUI->setObjectName("QuizzUI");
        QuizzUI->resize(800, 600);
        actionLaden = new QAction(QuizzUI);
        actionLaden->setObjectName("actionLaden");
        actionSpeichern = new QAction(QuizzUI);
        actionSpeichern->setObjectName("actionSpeichern");
        actionStarten = new QAction(QuizzUI);
        actionStarten->setObjectName("actionStarten");
        actionStopeen = new QAction(QuizzUI);
        actionStopeen->setObjectName("actionStopeen");
        centralwidget = new QWidget(QuizzUI);
        centralwidget->setObjectName("centralwidget");
        verticalLayout = new QVBoxLayout(centralwidget);
        verticalLayout->setObjectName("verticalLayout");
        categoryContainer = new QVBoxLayout();
        categoryContainer->setObjectName("categoryContainer");
        pushButton = new QPushButton(centralwidget);
        pushButton->setObjectName("pushButton");

        categoryContainer->addWidget(pushButton);


        verticalLayout->addLayout(categoryContainer);

        QuizzUI->setCentralWidget(centralwidget);
        menubar = new QMenuBar(QuizzUI);
        menubar->setObjectName("menubar");
        menubar->setGeometry(QRect(0, 0, 800, 21));
        menuQuiz = new QMenu(menubar);
        menuQuiz->setObjectName("menuQuiz");
        QuizzUI->setMenuBar(menubar);
        statusbar = new QStatusBar(QuizzUI);
        statusbar->setObjectName("statusbar");
        QuizzUI->setStatusBar(statusbar);

        menubar->addAction(menuQuiz->menuAction());
        menuQuiz->addAction(actionLaden);
        menuQuiz->addAction(actionSpeichern);
        menuQuiz->addAction(actionStarten);
        menuQuiz->addAction(actionStopeen);

        retranslateUi(QuizzUI);

        QMetaObject::connectSlotsByName(QuizzUI);
    } // setupUi

    void retranslateUi(QMainWindow *QuizzUI)
    {
        QuizzUI->setWindowTitle(QCoreApplication::translate("QuizzUI", "QuizzUI", nullptr));
        actionLaden->setText(QCoreApplication::translate("QuizzUI", "Laden", nullptr));
        actionSpeichern->setText(QCoreApplication::translate("QuizzUI", "Speichern", nullptr));
        actionStarten->setText(QCoreApplication::translate("QuizzUI", "Starten", nullptr));
        actionStopeen->setText(QCoreApplication::translate("QuizzUI", "Stoppen", nullptr));
        pushButton->setText(QCoreApplication::translate("QuizzUI", "PushButton", nullptr));
        menuQuiz->setTitle(QCoreApplication::translate("QuizzUI", "Quiz", nullptr));
    } // retranslateUi

};

namespace Ui {
    class QuizzUI: public Ui_QuizzUI {};
} // namespace Ui

QT_END_NAMESPACE

#endif // UI_QUIZZUI_H
