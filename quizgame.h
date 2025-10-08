#ifndef QUIZGAME_H
#define QUIZGAME_H
#include "category.h"
#include "participant.h"
using namespace std;
class QuizGame
{
public:
    QuizGame();

private:
    vector<Participant*> participants;
    vector<Category*> categories;
    bool showScoreWhilePlaying = true;
};

#endif // QUIZGAME_H
