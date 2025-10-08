#ifndef QUIZ_H
#define QUIZ_H
#include <QList>
#include <qevent.h>
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
    void save(QString savePath);
    void load(QString folderPath);
    QList<Player *> getPlayers() const;
    void nextPlayer();
    int getCurrPlayerIndex() const;
    void randomizePlayers();
    void setSavePath(const QString &newSavePath);
    QString getSavePath() const;
    void deleteFolder(const QString &path);
private:
    QList<Category*> categories = QList<Category*>();
    QList<Player*> players = QList<Player*>();
    int currPlayerIndex = 0;
    QString savePath = "";
};

#endif // QUIZ_H
