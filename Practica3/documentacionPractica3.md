# Inteligencia Artificial #
# Documentación de la práctica 3: DESCONECTA-4 BOOM #

MINIMAX con cota maxima de 4.
PODA ALFABETA con cota maxima de 8.
Cotas dadas como entrada al algoritmo.
Cota = profundidad, cuando actua un nodo min y un nodo max, entonces se suma uno a la profundidad. Por tanto, una profundidad 4 realmente es una profundidad de 8.
Invocacion al metodo del algoritmo dentro del think. En cada think se realiza una accion, por tanto en cada think se explora un nuevo arbol de juego para ver que hacer.

El bot puede ser o jugador 1 o jugador 2. Primera idea: if con una variable de clase que controle si es el jugador 1 o el 2. Si es el 1, empieza desde el principio del arbol, si no, empieza desde el segundo nodo del arbol, siendo este para el el nodo max. Uso JugadorActivo para saber cual.

Los estados del juego estan en la clase Environment, es como el tablero de juego.

Usar primero GenerateAllMoves para obtener todas los posibles sucesores del hijo actual (posibles jugadas, tipo, poner la ficha en el 2 o ponerla en el 4). Y entonces con possible_actions, ver si de todas esas, la que cojas (se cogera la mejor) es posible realizar. Entonces, una vez que se sabe cual es el movimiento que se va a hacer, usar GenerateNextMove (la primera vez que se usa este metodo se le debe pasar -1, el resto de veces se le pasa el movimiento anterior, gracias a la funcion Last_Action pasandole como parametro el numero de jugador 1 o 2).

La funcion evaluacion() es JuegoTerminado(), y se sabe quien ha ganado con RevisarTablero.

Usar el metodo Puntuacion para ver la puntuacion que tengo.
