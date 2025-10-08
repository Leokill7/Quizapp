#include "quizmanager.h"

QuizManager::QuizManager() {}

Quiz *QuizManager::getCurrentQuiz() const
{
    return currentQuiz;
}

void QuizManager::setCurrentQuiz(Quiz *newCurrentQuiz)
{
    currentQuiz = newCurrentQuiz;
}
