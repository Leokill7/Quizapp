#ifndef QUIZZUI_H
#define QUIZZUI_H
#include "question.h"
#include "quiz.h"
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <qboxlayout.h>
#include <QPushButton>
#include <qmediaplayer.h>
QT_BEGIN_NAMESPACE
namespace Ui {
class QuizzUI;
}
QT_END_NAMESPACE

class QuizzUI : public QMainWindow
{
    Q_OBJECT

public:
    QuizzUI(QWidget *parent = nullptr);
    ~QuizzUI();
    void generatePlayerSelectorUI(QVBoxLayout *container);
    void showPlayerStats(QHBoxLayout *container);
    void showQuizScreen();
    void startQuiz();
    void stopQuiz();
    void startEditing();
    void questionEditingMenu(Question* questionToEdit, int categoryIndex);
    void clearLayout(QLayout *layout);
    void addCategory();
    bool isImage(QString filetype);
    bool isVideo(QString filetype);
    bool isAudio(QString filetype);
    QPushButton* createMiniButton(QString buttonText);
    QVBoxLayout* createMediaPlayer(QString mediaSource);

private:
    Ui::QuizzUI *ui;
    Quiz* quiz = new Quiz();
    QMediaPlayer *player = nullptr;
    int mediaVolume = 50;
};
#endif // QUIZZUI_H
