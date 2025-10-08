#ifndef QUIZMANAGER_H
#define QUIZMANAGER_H

#include "quiz.h"
class QuizManager
{
public:
    QuizManager();
    void loadQuizConfig();
    void saveQuizConfig();
    Quiz *getCurrentQuiz() const;

    void setCurrentQuiz(Quiz *newCurrentQuiz);

private:
    Quiz* currentQuiz = nullptr;
};

#endif // QUIZMANAGER_H
