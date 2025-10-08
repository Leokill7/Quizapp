#ifndef PARTICIPANT_H
#define PARTICIPANT_H
#include <qobject.h>
class Participant
{
public:
    Participant();
private:
    QString name;
    int points = 0;
};

#endif // PARTICIPANT_H
