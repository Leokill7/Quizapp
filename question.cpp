#include "question.h"

Question::Question(QString question, int points, QString answer) {
    this->questionText = question;
    this->points = points;
    this->answer = answer;
}

int Question::getPoints() const
{
    return points;
}

QString Question::getQuestionText() const
{
    return questionText;
}

void Question::setPoints(int newPoints)
{
    points = newPoints;
}

void Question::setQuestionText(const QString &newQuestionText)
{
    questionText = newQuestionText;
}

void Question::setAnswered(bool newAnswered)
{
    answered = newAnswered;
}

bool Question::getAnswered() const
{
    return answered;
}

void Question::setAnswer(const QString &newAnswer)
{
    answer = newAnswer;
}

QString Question::getAnswer() const
{
    return answer;
}

void Question::setContentSource(const QString &newContentSource)
{
    contentSource = newContentSource;
}

QString Question::getContentSource() const
{
    return contentSource;
}
