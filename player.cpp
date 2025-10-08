#include "player.h"


Player::Player(QString name)
{
    this->name = name;
}

void Player::setPoints(int newPoints)
{
    points = newPoints;
}

int Player::getPoints() const
{
    return points;
}

void Player::addPoints(int amount)
{
    points += amount;
}

void Player::setName(const QString &newName)
{
    name = newName;
}

QString Player::getName() const
{
    return name;
}

void Player::addCorrectAnswer()
{
    correctAnswers +=1;
}

int Player::getCorrectAnswers() const
{
    return correctAnswers;
}

void Player::setCorrectAnswers(int newCorrectAnswers)
{
    correctAnswers = newCorrectAnswers;
}
