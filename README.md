# Prácticas de la asignatura Inteligencia Artificial #
Prácticas de la asignatura Inteligencia Artificial del grado en Ingeniería Informática (UGR). Consisten en 3 prácticas diferentes que están divididas por niveles, cada uno realizando una función específica dentro de cada práctica.

## Práctica 1: Agente Conversacional ##

Bot conversacional usando el lenguaje AIML 2.0. He desarrollado 3 niveles de los 4 propuestos.

- **Nivel 0:** Contesta un total de 18 preguntas frecuentes que puedan aparecer en una entrevista de trabajo.
- **Nivel 1:** Introducción de contenidos en una plataforma de vídeo. Permite:
   + Incluir un nuevo capítulo en una temporada de una serie.
   + Incluir el primer capítulo de una nueva temporada en una serie.
   + Incluir una nueva serie en una de las plataformas.
- **Nivel 2:** Consultas sobre el contenido en una plataforma de vídeo. Permite:
   + Indicar los idiomas y subtítulos en los que se puede visionar una serie. Esto incluye preguntar si la serie está en un idioma concreto.
   + Responder al número de idiomas, subtítulos o capítulos totales o de una temporada que tiene una serie en concreto.
   + Indicar el número de temporadas que tiene la serie.
   + Decir el nombre de un capítulo concreto de una temporada de una serie.
   + Indicar la duración en minutos de un capítulo concreto de una serie.

Para iniciar el bot hay que ejecutar el archivo *run.sh*.

## Práctica 2: Agentes Reactivos/Deliberativos (Los extraños mundos de Belkan)

Los extraños mundos de Belkan es un simulador desarrollado por los profesores de la asignatura, a partir de la versión inicial del profesor Tsung-Che Chiang de la NTNU (Norwegian University of Science and Technology, Trondheim).

Se desarrolla sobre un un mapa cuadrado bidimensional discreto que contiene como máximo 100 filas y 100 columnas. El mapa representa los accidentes geográficos de la superficie de parte de un planeta semejante a la Tierra. Estos accidentes geográficos tendrán un coste mayor o menor de instantes de tiempo, lo que hará que el personaje tarde más tiempo en pasar por uno de estos lugares. Sus elementos son considerados inmutables, es decir, el mapa no cambia durante el desarrollo del juego.

El objetivo de esta práctica es dotar de un comportamiento inteligente al personaje usando un agente reactivo/deliberativo para definir las habilidades que le permitan alcanzar una meta concreta dentro del juego según el nivel seleccionado:

- **Nivel 1:** Agente deliberativo básico que conoce por completo el mapa y no hay aldeanos en este (los aldeanos son el único elemento móvil del juego). El agente creará y llevará a cabo un plan que esquivará los muros y evitará caerse por los precipicios (límites del mapa) para así llegar al destino usando tres tipos de algoritmos de búsqueda:
   + Búsqueda en profundidad.
   + Búsqueda en anchura.
   + Búsqueda de costo uniforme.
- **Nivel 2:** Agente reactivo/deliberativo complejo que no conoce nada del mapa ni sabe donde se encuentra. Además hay aldeanos, que se mueven por el mapa de forma aleatoria.  
El agente creará un plan para llegar al objetivo, sin embargo hasta que no llegue a un punto de referencia PK no sabe donde está, por lo que en un principio realizará un comportamiento reactivo hasta llegar a un punto de referencia, a partir del cual seguirá el plan generado y rellenando el mapa, recalculando este plan en caso de encontrar obstáculos. Para crear el plan usa el algoritmo de búsqueda de costo uniforme.

El programa se inicia mediante el ejecutable *Belkan* (siendo *BelkanSG* una versión que funciona solo por terminal sin interfaz gráfica). Entonces se debe elegir uno de los niveles descritos y el mapa en el que se resolverá el problema. Para elegir una meta dentro del mapa sólo hace falta hacer click sobre el lugar en el mapa, y presionar el botón de "ejecución" para que el personaje vaya hacia ese lugar.

## Práctica 3: Métodos de Búsqueda con Adversario (Juegos [DesConecta-4 BOOM])

DesConecta-4 BOOM es un juego derivado del clásico juego Conecta-4. Usaremos un entorno simulado, basado en el ejemplo de la aspiradora inteligente anteriormente visto en clase (del libro "Inteligencia Artificial: Un enfoque Moderno", Stuart Russell, Peter Norvig, y adaptado de la versión usada en la práctica anterior del profesor Tsung-Che Chiang), para representar el tablero del juego y los agentes que competirán con el objetivo de ganar.

Es un juego bipersonal con información completa, jugado en un tablero de 7xx7 casillas, en el que los jugadores colocan fichas de forma alternativa. Las fichas se pueden colocar en una de las 7 columnas del tablero, colocándose por arriba y cayendo por gravedad hasta que toquen la primera fila del tablero o una ficha previamente colocada.

La meta del juego es hacer que el adversario alinee 4 fichas en el tablero, ya sea de manera horizontal, vertical o diagonal. Para ello, cada jugador dispone de una ficha bomba, que sustituye a una ficha normal cada 5 movimientos. Si un jugador tiene una ficha bomba en el tablero, posee una acción más, BOOM, que consume el turno del jugador. Hacer BOOM elimina la ficha bomba y todas las fichas del mismo color en la misma fila. Los huecos generados por las fichas eliminadas son rellenados por las fichas que estaban en filas superiores, simulando una caída por gravedad.

El objetivo de la práctica será implementar un MINIMAX (con profundidad máxima de 4) o una Poda ALFA-BETA (profundidad máxima de 8) para que, dado un estado de una partida de DESCONECTA-4 BOOM, se pueda determinar la acción a realizar que permite conseguir la victoria en el juego. También se deberá crear una heurística apropiada en conjunto con el algoritmo implementado.

La nota de la práctica vendrá dada por la correcta implementación del algoritmo (que será enfrentado a tres jugadores "ninja" a los que tendrá que ganar) y la memoria de prácticas, que se deberá entregar junto al código.
