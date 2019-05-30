#ifndef PLAYER_H
#define PLAYER_H

#include "environment.h"

class Player{
    public:
      Player(int jug);
      Environment::ActionType Think();
      void Perceive(const Environment &env);
      double Poda_AlfaBeta(Environment & estado, int profundidad, double alfa, double beta, int jugador, Environment::ActionType & accion);
    private:
      int jugador_;
      Environment actual_;
};
#endif
