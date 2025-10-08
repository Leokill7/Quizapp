#include "category.h"

Category::Category(QString categroyName)
{
    name = categroyName;
}

void Category::addQuestion(QString questionText, int points,QString answer)
{
    questions.push_back(new Question(questionText,points,answer));
}

void Category::removeQuestion(int index)
{
    questions.removeAt(index);
}

QString Category::getName() const
{
    return name;
}

QList<Question *> Category::getQuestions() const
{
    return questions;
}
