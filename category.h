#ifndef CATEGORY_H
#define CATEGORY_H

#include "question.h"
#include <QList>
class Category
{
public:
    Category(QString categroyName);
    void addQuestion(QString questionText, int points,QString answer);
    void removeQuestion(int index);
    QString getName() const;

    QList<Question *> getQuestions() const;

private:
    QList<Question*> questions = QList<Question*>();
    QString name;
};

#endif // CATEGORY_H
