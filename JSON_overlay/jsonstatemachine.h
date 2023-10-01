#ifndef JSONSTATEMACHINE_H
#define JSONSTATEMACHINE_H

#include "../Strategy/STM/stm.h"

class JsonStateMachine : public Stm
{
public:
    JsonStateMachine(QString fileName, QString dir);
};

#endif // JSONSTATEMACHINE_H
