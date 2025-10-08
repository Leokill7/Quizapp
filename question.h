#ifndef QUESTION_H
#define QUESTION_H
#include <QString>

class Question
{
public:
    Question(QString question, int points,QString answer);
    int getPoints() const;
    QString getQuestionText() const;

    void setPoints(int newPoints);

    void setQuestionText(const QString &newQuestionText);

    void setAnswered(bool newAnswered);

    bool getAnswered() const;

    void setAnswer(const QString &newAnswer);

    QString getAnswer() const;

    void setContentSource(const QString &newContentSource);

    QString getContentSource() const;

private:
    int points;
    QString questionText;
    QString answer;
    bool answered = false;
    QString contentSource = "";
};

#endif // QUESTION_H
