# EDAversi

## Integrantes del grupo y contribución al trabajo de cada integrante

* Rita Moschini: construcción de getValidMoves, debuggueo de getBestMove con poda  alfa-beta, 
                 poda por cantidad de donos, README y guía de estilo.

* * Juan I. Caorsi: construcción de getValidMoves, construcción de getBestMove con poda alfa-beta,
                 poda por profundidad, README y guía de estilo.


## Parte 1: Generación de movimientos válidos y algoritmo de jugada

[Enumera aquí las pruebas que hiciste para validar que tu generador de movimientos válidos y algoritmo de jugada funcionan correctamente.]

Para completar el vector con los movimientos válidos, se recorren todos los lugares del tablero. 
Al encontrar un lugar vacío, busca en los 8 lugares que lo rodean una pieza del oponente. Si la
encuentra, "avanza en esa dirección" siempre y cuando continúe encontrando piezas del oponente.
Si cuando deja de avanzar, es decir luego de las piezas del oponente hay una pieza del jugador 
del turno, entonces el lugar vacío del cual se partió es un movimiento válido.

Para probar este algoritmo se probaron muchas rondas de juego, tocando lugares tanto válidos como
inválidos, y prestando atención tanto a las movidas validas del usuario como a las que hacía la 
ia en un principio.

Utilizando la herramienta de debug de visual studio, prestamos atención a que las movidas válidas 
no se repitan en el vector validMoves.

En lo que respecta al algoritmo de jugada, la dificultad de su implementación radicó en voltear 
las piezas del oponente correspondientes una vez colocada la pieza seleccionada por el jugador 
del turno. Para esto, utilizando un algoritmo similar al de la generación de movimientos, se
recorrieron los lugares contiguos al de la pieza colocada; en caso de encontrar una pieza del
oponente, se "avanza en esa dirección" mientras se sigan encontrando piezas del oponente. Si
luego de las piezas del oponente hay una pieza del jugador de turno, eso significa que las piezas
del oponente que acaban de ser recorridas quedaron encerradas entre dos piezas del jugador, por lo
tanto deben ser volteadas. Luego, partiendo del lugar en el que fue colocada la pieza, se voltean
las piezas del oponente en la dirección indicada anteriormente.

Para evaluar el funcionamiento de esta implementación se prestó atención a las fichas volteadas, 
si lo que ocurría tenía sentido o no acorde a las reglas del Reversi.

## Parte 2: Implementación del motor de IA

Elegimos implementar el árbol de juego de la IA de forma iterativa, utilizando un stack de nodos
y aplicando un enfoque DFS. Se creó un constructor y un método que le agrega un nodo hijo al nodo
que se indique con el fin de simplificar el manejo de los mismos y aprovechar las herramientas 
del lenguaje, considerando también posibles casos de error en el manejo de la memoria y 
minimizando el uso de la misma. Aunque inicialmente pensamos utilizar un árbol de tipo "hijo por
izquierda, hermano por derecha" rápidamente nos dimos cuenta de que al utilizar un adaptador de 
tipostack y almacenar las movidas válidas en un vector, no sería necesario guardar la dirección
de memoria del nodo hermano en cada nodo, ya que el mismo está caracterizado por los movimientos
válidos del nodo padre.


En lo que respecta al algoritmo, primero se genera un nodo raíz con la configuración actual del 
tablero, se lo pushea en el stack y se completa su vector de movimientos válidos. Luego, a partir 
del último elemento de este vector se genera un nodo, se quita el último movimiento válido del 
vector, se realiza el movimiento en su tablero, y se completa su vector de movimientos válidos,
para luego generar un nodo con su último elemento, quitarlo y así reiteradamente. Este 
procedimiento se repetie hasta alcanzar un nivel máximo en el árbol, es decir una cantidad máxima 
de jugadas. Una vez alcanzado el nodo con este nivel, ya no se completan los validMoves, sino que 
comienza la implementación del algoritmo miniman y la poda alfa-beta. Consideramos poco pertinente 
y engorroso explicar en detalle el funcionamiento de dichos mecanismos, pero vale la pena realizar 
algunas aclaraciones sobre el modo de implementación de estos algoritmos. En primer lugar, para 
evaluar el "puntaje" de los nodos hoja, comúnmente se emplea algo llamado "función de evaluación"; 
nosotros asignamos el puntaje acorde a la cantidad de piezas volteadas + la pieza agregada por el 
jugador del turno, luego si es el turno de la IA entonces el puntaje del nodo es el descripto, y 
si es el turno del jugador humano el puntaje del nodo es el descripto multiplicado por -1. Dada 
esta asignación, se consideraba a la IA como el maximizador, y al jugador humano como el minimizador,
pues la primera favorecerá puntajes de mayor valor y el humano favorecerá puntajes de menor valor.
Una vez realizadas las comparaciones correspondientes, el nodo hoja visitado se descarta (o se 
"popea" del stack) y se procede a repetir el procedimiento para el siguiente movimiento válido del
vector. Esto sucede hasta que el vector se queda sin movimientos válidos; en ese momento, se 
elimina ese nodo del stack, y se continúa con el siguiente movmimiento válido del nodo padre, 
generándose nodos hasta llegar al nivel máximo, y así reiteradamente.

Tuvimos en consideración de eventos inválidos como pueden ser: el uso de stack.pop() en un stack
vacío, vector.pop_back() en un vector vacío.

## Parte 3: Poda del árbol

No se completa el algoritmo pues el árbol se volvería realmente grande, tanto que es muy difícil 
de abarcar en su totalidad. Consideremos que el juego tiene una cantidad de jugadas posibles del 
orden de 10^58. Porlo tanto, se busca hacer lo siguiente: por un lado una cierta poda de 
posibilidades para filtrar aquellas jugadas que no serán óptimas, quedándonos con las mejores; por 
el otro, se da un límite de nivel o altura que alcanzará el árbol. Todo esto ayuda a que se 
considere una cantidad coherente de nodos o jugadas posibles como también un tope de recorrido de 
las mismas.

En nuestro caso lo que se implementó dessde un principio fue la poda alpha-beta.


## Resumen

Pasos seguidos:
* Implementación de mecanica del juego (armado de getValidMoves() y playMove()): se aplican procedimientos usuales para la determinación de las posiciones válidas,
las cuales varían según el juegador actual y se calculan recorriendo toda la matriz de juego buscando espacios vacíos, válidos, en los que si se coloca
la pieza del jugador, se puede voltear alguna pieza del oponente. En playMove() se aplica el mismo procedimiento solo que en este caso se exploran las posiciones
de la grilla en torno a la jugada realizada, cadenas de fichas del oponente que hayan sido encerradas por la que colocó el jugador en el instante anterior.

* Implementación getBestmove(): se añade la generación de un árbol del tipo nodos con punteros a hijo (pronto explicaré que es suficiente con que cada nodo
cuente con un solo puntero a UN solo hijo). Primero definimos la clase TreeNode, que contendrá la información que consideramos necesaria para la generación de
los nodos hijos como también un método agregarHijo(), dedicado principalmente a evasión de errores en la programación en caso de una próxima implementación.

Una vez hecho esto, pensando en una lógica iterativa nos dedicamos a desarrollar la función que nos devolverá la jugada óptima según el algoritmo minimax.
Se utiliza un stack para el guardado de los punteros a los nodos cuyos hijos se están explorando, uno por uno, jugada válida por jugada válida. 
Se puede pensar que el recorrido/armado de árbol en principio tiene una lógica de DFS, condicionado a la altura máxima que puede alcanzar el árbol.

En un principio se prepara el nodo raiz, y luego de crearlo e inicializarlo como corresponde, el programa entra en un ciclo, con una referencia al nodo raiz en stack.
En cada iteración, se explora el primer hijo del nodo cuya referencia se encuentra sobre la pila (último puntero añadido), formado a partir de la primer 
jugada que se realiza desde la situación actual del juego. De esta forma se van creando uno por uno los hijos de la primer jugada válida del nodo raiz,
de la primera del hijo de la raiz, etcétera. Hasta que se alcanza o un final (no más jugadas válidas para jugar) o un nodo hoja (nivel del nodo máximo).
Una vez alcanzado este nivel, comienza el proceso de minimaz y la poda alfa beta. En ese punto ya no queremos apilar más punteros, así que se visitan los nodos hoja.
Acá se preguntarán qué pasa si los nodos hoja son varios y no uno como el creado. Resulta que una vez visitado y comparado los valores de 
ganancia (aiGain que viajará por minimax) y alfa y beta, del nodo hoja creado con su nodo madre, se elimina el puntero al hoja del stack y se libera su memoria.
Luego se vuelve a crear el próximo hijo del nodo de altura máxima menos uno, que existirá y será visitado y eliminado de la misma forma que su nodo hermano
anterior. 
De esta forma cada vez que tomamos una "foto"" del arbol veremos que tiene forma de lista, pero que irá cambiando de forma a medida que avace la ejecución.

Una vez que se termina de evaluar cada nodo y solo tenemos el nodo raiz en el stack, salimos del ciclo y devolvemos la movida seleccionada por el algoritmo minimax,
que fue guardada en la variable bestMove.


## Comentarios adicionales

Se pasaron valores int de Pieces a char para optimizar memoria, board solo contendrá valores entre 
el 0 y el 2.

Respecto al miembro inProcess en los nodos: un nodo se considera inProcess cuando su vector de 
validMoves está siendo estudiado es decir cuando se están recorriendo sus nodos hijos.

El miembro "level" en los nodos refiere alnivel de ese nodo en el árbol.

El miembro "currentGain" refiere a la cantidad de fichas volteadas en ese turno, independientemente
de su color.

"primordialMove " contiene el movimiento válido de la raíz correspondiente a la rama que está siendo
evaluada actualmente.

La variable "move" en getBestMove contiene el último validMove del último nodo pusheado.

## Bonus points

La explicación de la implementación de los bonus está incluida en secciones anteriores.

## Referencias: 
* Numero de jugadas posibles: https://arxiv.org/html/2310.19387v3
* Videos sobre el algoritmo minimax y la poda alfa beta:
  https://www.youtube.com/watch?v=l-hh51ncgDI&t=1s
  https://www.youtube.com/watch?v=N98F8HYEDCk
