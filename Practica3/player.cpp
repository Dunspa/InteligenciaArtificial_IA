#include <iostream>
#include <cstdlib>
#include <vector>
#include <queue>
#include "player.h"
#include "environment.h"
using namespace std;

const double masinf=9999999999.0, menosinf=-9999999999.0;

// Constructor
Player::Player(int jug){
    jugador_=jug;
}

// Actualiza el estado del juego para el jugador
void Player::Perceive(const Environment & env){
    actual_=env;
}

double Puntuacion(int jugador, const Environment &estado){
    double suma=0;

    for (int i=0; i<7; i++)
      for (int j=0; j<7; j++){
         if (estado.See_Casilla(i,j)==jugador){
            if (j<3)
               suma += j;
            else
               suma += (6-j);
         }
      }

    return suma;
}


// Funcion de valoracion para testear Poda Alfabeta
double ValoracionTest(const Environment &estado, int jugador){
    int ganador = estado.RevisarTablero();

    if (ganador==jugador)
      return 99999999.0; // Gana el jugador que pide la valoracion
    else if (ganador!=0)
      return -99999999.0; // Pierde el jugador que pide la valoracion
    else if (estado.Get_Casillas_Libres()==0)
      return 0;  // Hay un empate global y se ha rellenado completamente el tablero
    else
      return Puntuacion(jugador,estado);
}

// ------------------- Los tres metodos anteriores no se pueden modificar

// Funciones auxiliares
double minimo(const double a, const double b){
   if (a < b)
      return a;
   else
      return b;
}

double maximo(const double a, const double b){
   if (a > b)
      return a;
   else
      return b;
}

// Funcion heuristica
double HeuristicaDesconecta4Boom(const Environment & estado, int jugador){
   const int FILAS = 7;
   const int COLUMNAS = 7;
   double valoracion = 0;
   int fichasJugador = 0, fichasOponente = 0;

   if (estado.Have_BOOM(jugador)){
      valoracion += 100;
   }

   // Lo cerca que estoy de ganar - lo cerca que está el otro de ganar
   // Cuantas mas fichas mias juntas = menos puntuacion

   // Primero comprobar que no es desfavorable, si lo es = valor muy negativo (-9999)
   // Comprobar horizontal
   for (int i = 0 ; i < FILAS ; i++){
      fichasJugador = 0, fichasOponente = 0;

      for (int j = 0 ; j < COLUMNAS ; j++){
         int propietario = estado.See_Casilla(i, j);

         // Si es mi casilla
         if (propietario == jugador){
            fichasJugador++;

            if (fichasJugador >= 4){
               valoracion += -9999;
            }

            // Cuantas más fichas juntas tenga el oponente, mejor valoración
            switch (fichasOponente){
               case 1:
                  valoracion += 10;
                  break;

               case 2:
                  valoracion += 20;
                  break;

               case 3:
                  valoracion += 30;
                  break;
            }
            fichasOponente = 0;
         }
         // Si está vacía
         else if (propietario == 0){
            // Cuantas más fichas mias juntas, peor valoración
            switch (fichasJugador){
               case 1:
                  valoracion += -10;
                  break;

               case 2:
                  valoracion += -20;
                  break;

               case 3:
                  valoracion += -30;
                  break;
            }

            // Cuantas más fichas juntas tenga el oponente, mejor valoración
            switch (fichasOponente){
               case 1:
                  valoracion += 10;
                  break;

               case 2:
                  valoracion += 20;
                  break;

               case 3:
                  valoracion += 30;
                  break;
            }

            // Se reinician las fichas consecutivas
            fichasOponente = 0;
            fichasJugador = 0;
         }
         // Si es del otro jugador
         else{
            fichasOponente++;

            if (fichasOponente >= 4){
               valoracion += 9999;
            }

            // Cuantas más fichas mias juntas, peor valoración
            switch (fichasJugador){
               case 1:
                  valoracion += -10;
                  break;

               case 2:
                  valoracion += -20;
                  break;

               case 3:
                  valoracion += -30;
                  break;
            }
            fichasJugador = 0;
         }
      }
   }

   // Comprobar vertical
   for (int i = 0 ; i < FILAS ; i++){
      fichasJugador = 0, fichasOponente = 0;

      for (int j = 0 ; j < COLUMNAS ; j++){
         int propietario = estado.See_Casilla(j, i);

         // Si es mi casilla
         if (propietario == jugador){
            fichasJugador++;

            if (fichasJugador >= 4){
               valoracion += -9999;
            }

            // Cuantas más fichas juntas tenga el oponente, mejor valoración
            switch (fichasOponente){
               case 1:
                  valoracion += 10;
                  break;

               case 2:
                  valoracion += 20;
                  break;

               case 3:
                  valoracion += 30;
                  break;
            }
            fichasOponente = 0;
         }
         // Si está vacía
         else if (propietario == 0){
            // Cuantas más fichas mias juntas, peor valoración
            switch (fichasJugador){
               case 1:
                  valoracion += -10;
                  break;

               case 2:
                  valoracion += -20;
                  break;

               case 3:
                  valoracion += -30;
                  break;
            }

            // Cuantas más fichas juntas tenga el oponente, mejor valoración
            switch (fichasOponente){
               case 1:
                  valoracion += 10;
                  break;

               case 2:
                  valoracion += 20;
                  break;

               case 3:
                  valoracion += 30;
                  break;
            }

            // Se reinician las fichas consecutivas
            fichasOponente = 0;
            fichasJugador = 0;
         }
         // Si es del otro jugador
         else{
            fichasOponente++;

            if (fichasOponente >= 4){
               valoracion += 9999;
            }

            // Cuantas más fichas mias juntas, peor valoración
            switch (fichasJugador){
               case 1:
                  valoracion += -10;
                  break;

               case 2:
                  valoracion += -20;
                  break;

               case 3:
                  valoracion += -30;
                  break;
            }
            fichasJugador = 0;
         }
      }
   }

   // Comprobar diagonal creciente
   for (int i = 1 ; i <= (FILAS + COLUMNAS - 1) ; i++){
      fichasJugador = 0, fichasOponente = 0;

      // Índice de la primera columna de la diagonal
      // 0 para la diagonal superior, i - FILAS para la diagonal inferior
      int primeraColumna = maximo(0, i - FILAS);

      // Número de elementos que hay en esa diagonal
      int elementosDiagonal = minimo(minimo(i, COLUMNAS - primeraColumna), FILAS);

      for (int j = 0 ; j < elementosDiagonal ; j++){
         int propietario = estado.See_Casilla(minimo(COLUMNAS, i) - j - 1, primeraColumna + j);

         // Si es mi casilla
         if (propietario == jugador){
            fichasJugador++;

            if (fichasJugador >= 4){
               valoracion += -9999;
            }

            // Cuantas más fichas juntas tenga el oponente, mejor valoración
            switch (fichasOponente){
               case 1:
                  valoracion += 10;
                  break;

               case 2:
                  valoracion += 20;
                  break;

               case 3:
                  valoracion += 30;
                  break;
            }
            fichasOponente = 0;
         }
         // Si está vacía
         else if (propietario == 0){
            // Cuantas más fichas mias juntas, peor valoración
            switch (fichasJugador){
               case 1:
                  valoracion += -10;
                  break;

               case 2:
                  valoracion += -20;
                  break;

               case 3:
                  valoracion += -30;
                  break;
            }

            // Cuantas más fichas juntas tenga el oponente, mejor valoración
            switch (fichasOponente){
               case 1:
                  valoracion += 10;
                  break;

               case 2:
                  valoracion += 20;
                  break;

               case 3:
                  valoracion += 30;
                  break;
            }

            // Se reinician las fichas consecutivas
            fichasOponente = 0;
            fichasJugador = 0;
         }
         // Si es del otro jugador
         else{
            fichasOponente++;

            if (fichasOponente >= 4){
               valoracion += 9999;
            }

            // Cuantas más fichas mias juntas, peor valoración
            switch (fichasJugador){
               case 1:
                  valoracion += -10;
                  break;

               case 2:
                  valoracion += -20;
                  break;

               case 3:
                  valoracion += -30;
                  break;
            }
            fichasJugador = 0;
         }
      }
   }

   // Comprobar diagonal decreciente
   for (int i = 1 ; i <= (FILAS + COLUMNAS - 1) ; i++){
      fichasJugador = 0, fichasOponente = 0;

      // Índice de la primera columna de la diagonal
      // 0 para la diagonal superior, i - FILAS para la diagonal inferior
      int primeraColumna = maximo(0, i - FILAS);

      // Número de elementos que hay en esa diagonal
      int elementosDiagonal = minimo(minimo(i, COLUMNAS - primeraColumna), FILAS);

      for (int j = 0 ; j < elementosDiagonal ; j++){
         int propietario = estado.See_Casilla(maximo(COLUMNAS, i) + j - i, primeraColumna + j);

         // Si es mi casilla
         if (propietario == jugador){
            fichasJugador++;

            if (fichasJugador >= 4){
               valoracion += -9999;
            }

            // Cuantas más fichas juntas tenga el oponente, mejor valoración
            switch (fichasOponente){
               case 1:
                  valoracion += 10;
                  break;

               case 2:
                  valoracion += 20;
                  break;

               case 3:
                  valoracion += 30;
                  break;
            }
            fichasOponente = 0;
         }
         // Si está vacía
         else if (propietario == 0){
            // Cuantas más fichas mias juntas, peor valoración
            switch (fichasJugador){
               case 1:
                  valoracion += -10;
                  break;

               case 2:
                  valoracion += -20;
                  break;

               case 3:
                  valoracion += -30;
                  break;
            }

            // Cuantas más fichas juntas tenga el oponente, mejor valoración
            switch (fichasOponente){
               case 1:
                  valoracion += 10;
                  break;

               case 2:
                  valoracion += 20;
                  break;

               case 3:
                  valoracion += 30;
                  break;
            }

            // Se reinician las fichas consecutivas
            fichasOponente = 0;
            fichasJugador = 0;
         }
         // Si es del otro jugador
         else{
            fichasOponente++;

            if (fichasOponente >= 4){
               valoracion += 9999;
            }

            // Cuantas más fichas mias juntas, peor valoración
            switch (fichasJugador){
               case 1:
                  valoracion += -10;
                  break;

               case 2:
                  valoracion += -20;
                  break;

               case 3:
                  valoracion += -30;
                  break;
            }
            fichasJugador = 0;
         }
      }
   }

   // Comprobar caso de bomba con last action

   return valoracion;
}

// Versión de ValoracionTest que añade mi heurística
// Comprueba ganar, perder, empate, o cualquier otra situación distinta
double Valoracion(const Environment & estado, int jugador){
   int ganador = estado.RevisarTablero();

   if (ganador == jugador)
      return 99999999.0;   // Gana el jugador que pide la valoracion
   else if (ganador != 0)
      return -99999999.0;  // Pierde el jugador que pide la valoracion
   else if (estado.Get_Casillas_Libres() == 0)
      return 0;   // Hay un empate global y se ha rellenado completamente el tablero
   else
      // Cualquier otra situación: mi heuristica
      return HeuristicaDesconecta4Boom(estado, jugador);
}

// Función que implementa el algoritmo de Poda Alfa-Beta
// Devuelve la valoración del mejor estado encontrado
double Player::Poda_AlfaBeta(Environment & estado, int profundidad, double alfa, double beta, int jugador, Environment::ActionType & accion){
   // Generar todos los posibles hijos del estado actual
   Environment hijo[8];
   int n_act = estado.GenerateAllMoves(hijo);

   const int PROFUNDIDAD = 8;

   // Caso base
   if (estado.JuegoTerminado() || profundidad == 0){
      return Valoracion(estado, jugador);
   }

   // Distinguir entre nodos MIN y nodos MAX con profundidad%2. Si es par es MAX, si no es MIN.
   if (profundidad % 2 == 0){
      // Para cada hijo de nodo
      for (int i = n_act - 1 ; i >= 0 ; i--){
         double valor_alfabeta = Poda_AlfaBeta(hijo[i], profundidad - 1, alfa, beta, jugador, accion);

         // Si el valor de poda alfabeta es mayor que alfa
         if (valor_alfabeta > alfa){
            alfa = valor_alfabeta;

            // Compruebo si estoy en el nodo raíz, y entonces actualizo la acción
            // ya que la acción a realizar es mejor que la que tenía
            if (profundidad == PROFUNDIDAD){ // if estado == actual_
               accion = static_cast<Environment::ActionType>(hijo[i].Last_Action(jugador));
            }
         }

         // Criterio de poda beta
         if (alfa >= beta)
            return beta;
      }

      return alfa;
   }
   else{
      // Para cada hijo de nodo
      for (int i = 0 ; i < n_act ; i++){
         double valor_alfabeta = Poda_AlfaBeta(hijo[i], profundidad - 1, alfa, beta, jugador, accion);

         // Si el valor de poda alfabeta es mayor que beta
         if (valor_alfabeta < beta)
            beta = valor_alfabeta;

         // Criterio de poda alfa
         if (alfa >= beta)
            return alfa;
      }

      return beta;
   }
}

// Invoca el siguiente movimiento del jugador
Environment::ActionType Player::Think(){
    const int PROFUNDIDAD_MINIMAX = 6;  // Umbral maximo de profundidad para el metodo MiniMax
    const int PROFUNDIDAD_ALFABETA = 8; // Umbral maximo de profundidad para la poda Alfa_Beta

    Environment::ActionType accion; // Acción que se va a devolver
    bool aplicables[8]; // Vector bool usado para obtener las acciones que son aplicables en el estado actual. La interpretacion es
                        // aplicables[0]==true si PUT1 es aplicable
                        // aplicables[1]==true si PUT2 es aplicable
                        // aplicables[2]==true si PUT3 es aplicable
                        // aplicables[3]==true si PUT4 es aplicable
                        // aplicables[4]==true si PUT5 es aplicable
                        // aplicables[5]==true si PUT6 es aplicable
                        // aplicables[6]==true si PUT7 es aplicable
                        // aplicables[7]==true si BOOM es aplicable

    double valor;       // Almacena el valor con el que se etiqueta el estado tras el proceso de busqueda.
    double alfa = menosinf, beta = masinf;  // Cotas de la poda AlfaBeta
    int n_act;          // Acciones posibles en el estado actual

    // Obtengo las acciones aplicables al estado actual en "aplicables"
    n_act = actual_.possible_actions(aplicables);
    int opciones[10];

    // Muestra por la consola las acciones aplicable para el jugador activo
    //actual_.PintaTablero();
    cout << " Acciones aplicables ";
    (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
    for (int t = 0 ; t < 8 ; t++)
      if (aplicables[t])
         cout << " " << actual_.ActionStr( static_cast< Environment::ActionType > (t)  );
    cout << endl;

    /*--------------------- COMENTAR Desde aqui
    cout << "\n\t";
    int n_opciones=0;
    JuegoAleatorio(aplicables, opciones, n_opciones);

    if (n_act==0){
      (jugador_==1) ? cout << "Verde: " : cout << "Azul: ";
      cout << " No puede realizar ninguna accion!!!\n";
      //accion = Environment::actIDLE;
    }
    else if (n_act==1){
           (jugador_==1) ? cout << "Vhttps://www.google.com/search?client=firefox-b-d&biw=1920&bih=917&tbm=isch&sa=1&ei=ewz0XPzuA--HjLsPj_CI0AU&q=lsi+ugr&oq=lsi+ugr&gs_l=img.3..0i24l2.122271.123108..123300...0.0..0.134.759.1j6......0....1..gws-wiz-img.......35i39j0i67j0j0i30j0i10i24.iOxen1GkSjU#imgrc=GzMts7DGDLSL8M:erde: " : cout << "Azul: ";
            cout << " Solo se puede realizar la accion "
                 << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[0])  ) << endl;
            accion = static_cast< Environment::ActionType > (opciones[0]);

         }
         else { // Hay que elegir entre varias posibles acciones
            int aleatorio = rand()%n_opciones;
            cout << " -> " << actual_.ActionStr( static_cast< Environment::ActionType > (opciones[aleatorio])  ) << endl;
            accion = static_cast< Environment::ActionType > (opciones[aleatorio]);
         }

    //--------------------- COMENTAR Hasta aqui*/


    //--------------------- AQUI EMPIEZA LA PARTE A REALIZAR POR EL ALUMNO ------------------------------------------------
    // Opcion: Poda AlfaBeta

    //valor = Poda_AlfaBeta(actual_, jugador_, 0, PROFUNDIDAD_ALFABETA, accion, alpha, beta);
    valor = Poda_AlfaBeta(actual_, PROFUNDIDAD_ALFABETA, alfa, beta, actual_.JugadorActivo(), accion);
    cout << "Valor MiniMax: " << valor << "  Accion: " << actual_.ActionStr(accion) << endl;

    return accion;
}
