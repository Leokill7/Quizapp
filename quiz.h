#ifndef QUIZ_H
#define QUIZ_H
#include <QList>
#include "category.h"
#include "player.h"
class Quiz
{
public:
    Quiz();
    void addCategory(QString categoryName);
    void removeCategory(int index);
    QList<Category *> getCategories() const;
    void addPlayer(QString playerName);
    void removePlayer(int index);
    void saveQuiz(QString quizname);
    void loadQuiz(QString filepath);
    QList<Player *> getPlayers() const;

private:
    QList<Category*> categories = QList<Category*>();
    QList<Player*> players = QList<Player*>();
};

#endif // QUIZ_H
