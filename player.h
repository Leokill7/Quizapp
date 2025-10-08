#ifndef PLAYER_H
#define PLAYER_H
#include <QString>
class Player
{
public:
    Player(QString name);
    void setPoints(int newPoints);

    int getPoints() const;
    void addPoints(int amount);
    void setName(const QString &newName);

    QString getName() const;
    void addCorrectAnswer();
    int getCorrectAnswers() const;

    void setCorrectAnswers(int newCorrectAnswers);

private:
    int points = 0;
    QString name;
    int correctAnswers = 0;
};

#endif // PLAYER_H
