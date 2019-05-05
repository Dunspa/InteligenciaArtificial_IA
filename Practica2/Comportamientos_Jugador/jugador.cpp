#include "../Comportamientos_Jugador/jugador.hpp"
#include "motorlib/util.h"

#include <iostream>
#include <cmath>
#include <set>
#include <stack>
#include <queue>
#include <map>

// Este es el método principal que debe contener los 4 Comportamientos_Jugador
// que se piden en la práctica. Tiene como entrada la información de los
// sensores y devuelve la acción a realizar.
Action ComportamientoJugador::think(Sensores sensores){
	Action accion = actIDLE;

	// Estoy en el nivel 1
	if (sensores.nivel != 4){
		// Capturar los valores de filas y columnas
		if (sensores.mensajeF != -1){
			fil = sensores.mensajeF;
			col = sensores.mensajeC;
		}

		// Actualizar el efecto de la última acción
		switch (ultimaAccion){
			case actTURN_R:
				brujula = (brujula + 1) % 4;
				break;
			case actTURN_L:
				brujula = (brujula + 3) % 4;
				break;
			case actFORWARD:
				switch (brujula){
					case 0:
						fil--;
						break;
					case 1:
						col++;
						break;
					case 2:
						fil++;
						break;
					case 3:
						col--;
						break;
				}
				break;
		}

		// Mirar si ha cambiado el destino
		if (sensores.destinoF != destino.fila || sensores.destinoC != destino.columna){
			destino.fila = sensores.destinoF;
			destino.columna = sensores.destinoC;
			hayPlan = false;
		}

		// Calcular un camino hasta el destino
		if (!hayPlan){
			actual.fila = fil;
			actual.columna = col;
			actual.orientacion = brujula;
			hayPlan = pathFinding(sensores.nivel, actual, destino, plan);
		}

		// Siguiente acción a realizar
		// Se sigue el plan ideado
		if (hayPlan && plan.size() > 0){
			accion = plan.front();
			plan.erase(plan.begin());
		}
		// No hay plan y se activa un comportamiento reactivo
		else{
			if (sensores.terreno[2] == 'P' || sensores.terreno[2] == 'M' ||
				 sensores.terreno[2] == 'D' || sensores.superficie[2] == 'a'){
				accion = actTURN_R;
			}
			else{
				accion = actFORWARD;
			}
		}

		// Recordar la última acción
		ultimaAccion = accion;
	}
	// Estoy en el nivel 2
	else{
		// Actualizar el efecto de la última acción
		switch (ultimaAccion){
			case actTURN_R:
				brujula = (brujula + 1) % 4;
				break;
			case actTURN_L:
				brujula = (brujula + 3) % 4;
				break;
			case actFORWARD:
				switch (brujula){
					case 0:
						fil--;
						break;
					case 1:
						col++;
						break;
					case 2:
						fil++;
						break;
					case 3:
						col--;
						break;
				}
				break;
		}

		// Mirar si ha cambiado el destino
		if (sensores.destinoF != destino.fila || sensores.destinoC != destino.columna){
			destino.fila = sensores.destinoF;
			destino.columna = sensores.destinoC;
			hayPlan = false;
		}

		// Si ya he encontrado un PK, se centra en buscar los objetivos
		if (puedoDescubrir){
			// Rellenar mapa con los valores de los sensores
			rellenaMapa(sensores);

			// Se calcula un camino hasta el destino
			if (!hayPlan){
				actual.fila = fil;
				actual.columna = col;
				actual.orientacion = brujula;
				hayPlan = pathFinding(sensores.nivel, actual, destino, plan);
			}

			// Si hay un precipicio, cambia de dirección
			if (sensores.terreno[2] == 'P' && accion == actFORWARD){
				hayPlan = false;
				accion = actTURN_R;
			}

			// Cada cinco pasos recalcula plan
			if (pasos_recalcular >= 3){
				accion = actIDLE;
				pasos_recalcular = 0;
				hayPlan = false;
			}

			// Siguiente acción a realizar
			// Se sigue el plan ideado
			if (hayPlan && plan.size() > 0){
				accion = plan.front();

				// Si hay un aldeano, no hace esa parte del plan y se espera
				if (sensores.superficie[2] == 'a'){
					accion = actIDLE;
				}
				else{
					pasos_recalcular++;
					plan.erase(plan.begin());
				}
			}
		}
		// Si aún no ha encontrado un PK, se centra en buscar ese PK
		else{
			// Se activa un comportamiento reactivo mientras no se haya encontrado un punto de referencia
			if (sensores.terreno[2] == 'P' || sensores.terreno[2] == 'M' ||
				sensores.terreno[2] == 'D' || sensores.superficie[2] == 'a'){
				accion = actTURN_R;
			}
			else{
				accion = actFORWARD;
			}

			// Aleatoriamente gira a la derecha, para evitar que se quede pegado al borde
			// Inicializar semilla aleatoria
		   srand (time(NULL));
			// Generar número entre 1 y 100
	      int n = rand() % 30 + 1;
			if (n == 3){	// Si es el 3, gira a la derecha
				accion = actTURN_R;
			}

			// Si la casilla es un PK, se espera, para empezar a rellenar mapa en la próxima iteración
			if (sensores.terreno[0] == 'K'){
				fil = sensores.mensajeF;
				col = sensores.mensajeC;
				accion = actIDLE;
				puedoDescubrir = true;
			}
		}

		// Recordar la última acción
		ultimaAccion = accion;
	}

  	return accion;
}

// Rellena el mapa con lo que captan los sensores
void ComportamientoJugador::rellenaMapa(Sensores sensores){
	mapaResultado[fil][col] = sensores.terreno[0];

	int i = 1;
	// Los sensores detectan hasta profundidad 3
	int profundo = 1;
	// Amplitud que detectan los sensores a cada profundidad, en profundidad 3 es 7
	int amplitud = 3;

	while (i < 16){
		for (int j = -profundo ; j < amplitud-profundo ; j++){
			switch (brujula){
				case 0:
					mapaResultado[fil-profundo][col+j] = sensores.terreno[i];
					//mapaResultado[fil-1][col] = sensores.terreno[2];
					break;
				case 1:
					mapaResultado[fil+j][col+profundo] = sensores.terreno[i];
					//mapaResultado[fil][col+1] = sensores.terreno[2];
					break;
				case 2:
					mapaResultado[fil+profundo][col-j] = sensores.terreno[i];
					//mapaResultado[fil-1][col] = sensores.terreno[2];
					break;
				case 3:
					mapaResultado[fil-j][col-profundo] = sensores.terreno[i];
					//mapaResultado[fil][col-1] = sensores.terreno[2];
					break;
			}

			i++;
		}

		profundo++;
		amplitud += 2;
	}

	/*mapaResultado[fil-1][col+1] = sensores.terreno[1];
	mapaResultado[fil][col+1] = sensores.terreno[2];
	mapaResultado[fil+1][col+1] = sensores.terreno[3];
	mapaResultado[fil-2][col+2] = sensores.terreno[4];
	mapaResultado[fil-1][col+2] = sensores.terreno[5];
	mapaResultado[fil][col+2] = sensores.terreno[6];
	mapaResultado[fil+1][col+2] = sensores.terreno[7];
	mapaResultado[fil+2][col+2] = sensores.terreno[8];
	mapaResultado[fil-3][col+3] = sensores.terreno[9];
	mapaResultado[fil-2][col+3] = sensores.terreno[10];
	mapaResultado[fil-1][col+3] = sensores.terreno[11];
	mapaResultado[fil][col+3] = sensores.terreno[12];
	mapaResultado[fil+1][col+3] = sensores.terreno[13];
	mapaResultado[fil+2][col+3] = sensores.terreno[14];
	mapaResultado[fil+3][col+3] = sensores.terreno[15];*/
}

// Llama al algoritmo de busqueda que se usará en cada comportamiento del agente
// Level representa el comportamiento en el que fue iniciado el agente.
bool ComportamientoJugador::pathFinding (int level, const estado &origen, const estado &destino, list<Action> &plan){
	switch (level){
		case 1: cout << "Busqueda en profundad\n";
			return pathFinding_Profundidad(origen,destino,plan);
			break;
		case 2: cout << "Busqueda en Anchura\n";
			return pathFinding_Anchura(origen,destino,plan);
			break;
		case 3: cout << "Busqueda Costo Uniforme\n";
			return pathFinding_CostoUniforme(origen,destino,plan);
			break;
		case 4: cout << "Busqueda para el reto\n";
			// Nivel 2 utilizando búsqueda de costo uniforme
			return pathFinding_CostoUniforme(origen,destino,plan);
			break;
	}
	cout << "Comportamiento sin implementar\n";
	return false;
}

//---------------------- Implementación de la busqueda en profundidad ---------------------------

// Dado el código en carácter de una casilla del mapa dice si se puede
// pasar por ella sin riegos de morir o chocar.
bool EsObstaculo(unsigned char casilla){
	if (casilla=='P' or casilla=='M' or casilla =='D')
		return true;
	else
	  return false;
}

// Comprueba si la casilla que hay delante es un obstaculo. Si es un
// obstaculo devuelve true. Si no es un obstaculo, devuelve false y
// modifica st con la posición de la casilla del avance.
bool ComportamientoJugador::HayObstaculoDelante(estado &st){
	int fil=st.fila, col=st.columna;

  // calculo cual es la casilla de delante del agente
	switch (st.orientacion) {
		case 0: fil--; break;
		case 1: col++; break;
		case 2: fil++; break;
		case 3: col--; break;
	}

	// Compruebo que no me salgo fuera del rango del mapa
	if (fil<0 or fil>=mapaResultado.size()) return true;
	if (col<0 or col>=mapaResultado[0].size()) return true;

	// Miro si en esa casilla hay un obstaculo infranqueable
	if (!EsObstaculo(mapaResultado[fil][col])){
		// No hay obstaculo, actualizo el parámetro st poniendo la casilla de delante.
    st.fila = fil;
		st.columna = col;
		return false;
	}
	else{
	  return true;
	}
}

struct nodo{
	estado st;
	list<Action> secuencia;
	int costo;	// Costo hasta la raíz
};

struct ComparaEstados{
	bool operator()(const estado &a, const estado &n) const{
		if ((a.fila > n.fila) or (a.fila == n.fila and a.columna > n.columna) or
	      (a.fila == n.fila and a.columna == n.columna and a.orientacion > n.orientacion))
			return true;
		else
			return false;
	}
};


// Implementación de la búsqueda en profundidad.
// Entran los puntos origen y destino y devuelve la
// secuencia de acciones en plan, una lista de acciones.
bool ComportamientoJugador::pathFinding_Profundidad(const estado &origen, const estado &destino, list<Action> &plan) {
	//Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado,ComparaEstados> generados; // Lista de Cerrados
	stack<nodo> pila;											// Lista de Abiertos

  nodo current;
	current.st = origen;
	current.secuencia.empty();

	pila.push(current);

  while (!pila.empty() and (current.st.fila!=destino.fila or current.st.columna != destino.columna)){

		pila.pop();
		generados.insert(current.st);

		// Generar descendiente de girar a la derecha
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion+1)%4;
		if (generados.find(hijoTurnR.st) == generados.end()){
			hijoTurnR.secuencia.push_back(actTURN_R);
			pila.push(hijoTurnR);

		}

		// Generar descendiente de girar a la izquierda
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion+3)%4;
		if (generados.find(hijoTurnL.st) == generados.end()){
			hijoTurnL.secuencia.push_back(actTURN_L);
			pila.push(hijoTurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st)){
			if (generados.find(hijoForward.st) == generados.end()){
				hijoForward.secuencia.push_back(actFORWARD);
				pila.push(hijoForward);
			}
		}

		// Tomo el siguiente valor de la pila
		if (!pila.empty()){
			current = pila.top();
		}
	}

	cout << "Terminada la busqueda\n";
  	if (current.st.fila == destino.fila and current.st.columna == destino.columna){
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else {
		cout << "No encontrado plan\n";
	}

	return false;
}

//---------------------- Implementación de la busqueda en anchura ---------------------------

// Implementación de la búsqueda en anchura.
// Entran los puntos origen y destino y devuelve la
// secuencia de acciones en plan, una lista de acciones.
bool ComportamientoJugador::pathFinding_Anchura(const estado &origen, const estado &destino, list<Action> &plan){
	//Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado,ComparaEstados> generados;	// Lista de Cerrados
	queue<nodo> cola;								// Lista de Abiertos (Cola FIFO)

  	nodo current;
	current.st = origen;
	current.secuencia.empty();

	cola.push(current);

  	while (!cola.empty() and (current.st.fila!=destino.fila or current.st.columna != destino.columna)){
		cola.pop();
		generados.insert(current.st);

		// Generar descendiente de girar a la derecha
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion+1)%4;
		if (generados.find(hijoTurnR.st) == generados.end()){	// Si no se ha encontrado
			hijoTurnR.secuencia.push_back(actTURN_R);
			cola.push(hijoTurnR);
		}

		// Generar descendiente de girar a la izquierda
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion+3)%4;
		if (generados.find(hijoTurnL.st) == generados.end()){
			hijoTurnL.secuencia.push_back(actTURN_L);
			cola.push(hijoTurnL);
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st)){
			if (generados.find(hijoForward.st) == generados.end()){
				hijoForward.secuencia.push_back(actFORWARD);
				cola.push(hijoForward);
			}
		}

		// Tomo el siguiente valor de la cola
		if (!cola.empty()){
			current = cola.front();
		}
	}

	cout << "Terminada la busqueda\n";
  	if (current.st.fila == destino.fila and current.st.columna == destino.columna){
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else {
		cout << "No encontrado plan\n";
	}

	return false;
}

//---------------------- Implementación de la busqueda con costo uniforme ---------------------------

int ComportamientoJugador::CalculaCosto(const int fila, const int columna, const int orientacion) const{
	int costo = 1;
	unsigned char tipo = mapaResultado[fila][columna];

	switch (tipo){
		case 'B':
			costo = 5;
			break;
		case 'A':
			costo = 10;
			break;
		case 'T':
			costo = 2;
			break;
		case '?':
			costo = 1;
			break;
	}

	return costo;
}

// Implementación de la búsqueda con costo uniforme.
// Entran los puntos origen y destino y devuelve la
// secuencia de acciones en plan, una lista de acciones.
bool ComportamientoJugador::pathFinding_CostoUniforme(const estado & origen, const estado & destino, list<Action> & plan){
	//Borro la lista
	cout << "Calculando plan\n";
	plan.clear();
	set<estado, ComparaEstados> generados;	// Lista de Cerrados (explored)
	multimap<int, nodo> abiertos;				// Lista de Abiertos (Cola con prioridad de nodos ordenados por menor costo) (frontier)

  	nodo current;
	current.st = origen;
	current.secuencia.clear();
	current.costo = 0;
	pair<int, nodo> casilla = {0, current};

	abiertos.insert(casilla);

  	while (!abiertos.empty() and (current.st.fila != destino.fila or current.st.columna != destino.columna)){
		abiertos.erase(abiertos.begin());	// Borrar primero (FIFO)
		generados.insert(current.st);

		// Generar descendiente de girar a la derecha
		nodo hijoTurnR = current;
		hijoTurnR.st.orientacion = (hijoTurnR.st.orientacion+1)%4;
		hijoTurnR.costo += 1;
		if (generados.find(hijoTurnR.st) == generados.end()){	// Si no se ha cerrado
			// Buscar el nodo para ver si ya existe y poder borrarlo
			multimap<int, nodo>::iterator it;
			bool encontrado = false;
			for (it = abiertos.begin() ; it != abiertos.end() && !encontrado ; ++it){
				if (hijoTurnR.st.fila == (*it).second.st.fila && hijoTurnR.st.columna == (*it).second.st.columna && hijoTurnR.st.orientacion == (*it).second.st.orientacion){
					encontrado = true;

					// Si ya se ha abierto (es el mismo nodo) y tiene mayor costo, se sobreescribe
					if (hijoTurnR.costo < (*it).first){
						hijoTurnR.secuencia.push_back(actTURN_R);
						casilla = {hijoTurnR.costo, hijoTurnR};
						abiertos.erase(it);
						abiertos.insert(casilla);
					}
				}
			}



			// Si no se ha abierto, se añade a la frontera
			if (!encontrado){
				hijoTurnR.secuencia.push_back(actTURN_R);
				casilla = {hijoTurnR.costo, hijoTurnR};
				abiertos.insert(casilla);
			}
		}

		// Generar descendiente de girar a la izquierda
		nodo hijoTurnL = current;
		hijoTurnL.st.orientacion = (hijoTurnL.st.orientacion+3)%4;
		hijoTurnL.costo += 1;
		if (generados.find(hijoTurnL.st) == generados.end()){ // Si no se ha cerrado
			// Buscar el nodo para ver si ya existe y poder borrarlo
			multimap<int, nodo>::iterator it;
			bool encontrado = false;
			for (it = abiertos.begin() ; it != abiertos.end() && !encontrado ; ++it){
				if (hijoTurnL.st.fila == (*it).second.st.fila && hijoTurnL.st.columna == (*it).second.st.columna && hijoTurnL.st.orientacion == (*it).second.st.orientacion){
					encontrado = true;

					// Si ya se ha abierto (es el mismo nodo) y tiene mayor costo, se sobreescribe
					if (hijoTurnL.costo < (*it).first){
						hijoTurnL.secuencia.push_back(actTURN_L);
						casilla = {hijoTurnL.costo, hijoTurnL};
						abiertos.erase(it);
						abiertos.insert(casilla);
					}
				}
			}

			// Si no se ha abierto, se añade a la frontera
			if (!encontrado){
				hijoTurnL.secuencia.push_back(actTURN_L);
				casilla = {hijoTurnL.costo, hijoTurnL};
				abiertos.insert(casilla);
			}
		}

		// Generar descendiente de avanzar
		nodo hijoForward = current;
		if (!HayObstaculoDelante(hijoForward.st)){
			// Costo del nodo es el suyo más el de sus ancestros
			hijoForward.costo = current.costo + CalculaCosto(hijoForward.st.fila, hijoForward.st.columna, hijoForward.st.orientacion);
			if (generados.find(hijoForward.st) == generados.end()){	// Si no se ha cerrado
				// Buscar el nodo para ver si ya existe y poder borrarlo
				multimap<int, nodo>::iterator it;
				bool encontrado = false;
				for (it = abiertos.begin() ; it != abiertos.end() && !encontrado ; ++it){
					if (hijoForward.st.fila == (*it).second.st.fila && hijoForward.st.columna == (*it).second.st.columna && hijoForward.st.orientacion == (*it).second.st.orientacion){
						encontrado = true;

						// Si ya se ha abierto (es el mismo nodo) y tiene mayor costo, se sobreescribe
						if (hijoForward.costo < (*it).first){
							hijoForward.secuencia.push_back(actFORWARD);
							casilla = {hijoForward.costo, hijoForward};
							abiertos.erase(it);
							abiertos.insert(casilla);
						}

					}
				}

				if(!encontrado){
					hijoForward.secuencia.push_back(actFORWARD);
					casilla = {hijoForward.costo, hijoForward};
					abiertos.insert(casilla);
				}
			}
		}

		// Tomo el siguiente valor de la cola (el de menor costo)
		if (!abiertos.empty()){
			current = (*abiertos.begin()).second;
		}
	}

	cout << "Terminada la busqueda\n";
  	if (current.st.fila == destino.fila and current.st.columna == destino.columna){
		cout << "Cargando el plan\n";
		plan = current.secuencia;
		cout << "Coste:" << current.costo << endl;
		cout << "Longitud del plan: " << plan.size() << endl;
		PintaPlan(plan);
		// ver el plan en el mapa
		VisualizaPlan(origen, plan);
		return true;
	}
	else{
		cout << "No encontrado plan\n";
	}

	return false;
}

// Sacar por la términal la secuencia del plan obtenido
void ComportamientoJugador::PintaPlan(list<Action> plan) {
	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			cout << "A ";
		}
		else if (*it == actTURN_R){
			cout << "D ";
		}
		else if (*it == actTURN_L){
			cout << "I ";
		}
		else {
			cout << "- ";
		}
		it++;
	}
	cout << endl;
}



void AnularMatriz(vector<vector<unsigned char> > &m){
	for (int i=0; i<m[0].size(); i++){
		for (int j=0; j<m.size(); j++){
			m[i][j]=0;
		}
	}
}


// Pinta sobre el mapa del juego el plan obtenido
void ComportamientoJugador::VisualizaPlan(const estado &st, const list<Action> &plan){
  AnularMatriz(mapaConPlan);
	estado cst = st;

	auto it = plan.begin();
	while (it!=plan.end()){
		if (*it == actFORWARD){
			switch (cst.orientacion) {
				case 0: cst.fila--; break;
				case 1: cst.columna++; break;
				case 2: cst.fila++; break;
				case 3: cst.columna--; break;
			}
			mapaConPlan[cst.fila][cst.columna]=1;
		}
		else if (*it == actTURN_R){
			cst.orientacion = (cst.orientacion+1)%4;
		}
		else {
			cst.orientacion = (cst.orientacion+3)%4;
		}
		it++;
	}
}



int ComportamientoJugador::interact(Action accion, int valor){
  return false;
}
