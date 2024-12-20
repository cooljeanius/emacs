Tutorial de Emacs. Vea al final las condiciones de copiado.

Generalmente los comandos de Emacs involucran la tecla CONTROL
(algunas veces llamada CTRL O CTL) o la tecla meta (algunas veces
llamada EDIT o ALT).  En lugar de escribir completamente esto en cada
ocasi�n, usaremos las siguientes abreviaturas.

 C-<car> significa mantener presionada la tecla CONTROL mientras
         teclea el car�cter <car>.  Por lo tanto C-f ser�: Mantenga
         presionada la tecla CONTROL y teclee f.
 M-<car> significa mantener presionada la tecla META o EDIT o ALT
 	 mientras teclea <car>.  Si no hay teclas META, EDIT o ALT, en
 	 su lugar presione y libere la tecla ESC y luego teclee
 	 <car>.  Escribimos <ESC> para referirnos a la tecla ESC.

Nota importante: para terminar la sesi�n de Emacs teclee C-x C-c (dos
caracteres).  Los caracteres ">>" en el margen izquierdo indican
instrucciones para que usted trate de usar un comando.  Por ejemplo:
<<Blank lines inserted around following line by help-with-tutorial>>
[Mitad de p�gina en blanco para prop�sitos did�cticos. El texto contin�a abajo]
>> Ahora teclee C-v (ver la pr�xima pantalla) para desplazarse a la
	siguiente pantalla (h�galo manteniendo la tecla control
	oprimida mientras teclea v).  Desde ahora deber�a hacer esto
	cada vez que termine de leer la pantalla.

Note que se superponen dos l�neas cuando se mueve de pantalla en
pantalla; esto provee una continuidad para que pueda seguir leyendo el
texto.

Lo primero que necesita saber es como moverse de un lugar a otro en el
texto.  Ya sabe como avanzar una pantalla, con C-v.  Para retroceder
una pantalla teclee M-v (mantenga oprimida la tecla META y teclee v, o
teclee <ESC>v si no tiene las teclas META, EDIT o ALT).

>> Intente teclear M-v y luego C-v, varias veces.


* RESUMEN
---------

Los siguientes comandos son �tiles para ver pantallas completas:

	C-v 	Avanzar una pantalla completa
	M-v 	Retroceder una pantalla completa
	C-l 	Limpiar la pantalla y mostrar todo el texto de nuevo,
 		 moviendo el texto alrededor del cursor al centro de la
		 pantalla (Esto es CONTROL-L, no CONTROL-1.)

>> Encuentre el cursor, y f�jese qu� texto hay cerca de �ste.
   Luego teclee C-l.
   Encuentre el cursor otra vez y note que el mismo texto est� cerca
   del cursor ahora.

Si su terminal las soporta, tambi�n puede usar las teclas AvP�g o
RegP�g para moverse por pantallas completas, pero puede editar m�s
eficientemente si usa C-v y M-v.

* MOVIMIENTO B�SICO DE CURSOR
-----------------------------

Es �til moverse de una pantalla completa a otra pero, �c�mo moverse a
un lugar espec�fico dentro del texto en la pantalla?

Puede hacerlo de diversas formas.  Puede usar las teclas de flechas,
pero es mas eficiente mantener las manos en la posici�n est�ndar y
usar los comandos C-p, C-b, C-f, y C-n.  Estos caracteres son
equivalentes a las cuatro teclas de flechas, de esta manera:

		        L�nea anterior, C-P
				:
				:
   Atr�s, C-b.... Posici�n actual del cursor .... Adelante, C-f
				:
				:
			L�nea siguiente, C-n

>> Mueva el cursor a la l�nea en la mitad del diagrama
   usando C-n o C-p. Luego teclee C-l para ver el
   diagrama completo centrado en la pantalla.

Le resultar� f�cil recordar estas letras por las palabras que
representan: P de Previous (anterior), N de Next (siguiente), B de
Backward (atr�s) y F de Forward (adelante).  Estar� usando estos
comandos de posicionamiento b�sico del cursor todo el tiempo.

>> Teclee algunos C-n para traer el cursor a esta l�nea.

>> Mu�vase a la l�nea con C-f y hacia arriba con algunos C-p.  Observe
   lo que hace C-p cuando el cursor est� en medio de la l�nea.

Cada l�nea de texto termina con un car�cter de nueva l�nea (Newline),
que sirve para separarla de la l�nea siguiente.  La �ltima l�nea de su
archivo debe de tener un car�cter de nueva l�nea al final (pero Emacs
no requiere que �sta lo tenga).

>> Intente usar C-b al comienzo de una l�nea.  Deber�a moverse al
   final de la l�nea previa.  Esto sucede porque retrocede a trav�s
   del car�cter de nueva l�nea.

C-f puede moverse a trav�s de una nueva l�nea igual que C-b.

>> Teclee algunos C-b m�s para que sienta por donde se encuentra el
   cursor.
   Luego teclee C-f para regresar al final de la l�nea.
   Luego teclee otro C-f m�s para moverse a la l�nea siguiente.

Cuando pase el tope o el final de la pantalla, se mostrar� el texto
m�s all� del borde de la pantalla.  Esto recibe el nombre de
"desplazamiento".  Esto le permite a Emacs mover el cursor al lugar
especificado en el texto sin moverlo fuera de la pantalla.

>> Intente mover el cursor fuera del borde de la pantalla con C-n, y
   observe lo que sucede.

Si moverse por caracteres es muy lento, puede moverse por palabras.
M-f (META-f) mueve adelante una palabra y M-b mueva atr�s una palabra.

>> Teclee algunos M-f y M-b

Cuando est� en la mitad de una palabra, M-f mueve al final de la
palabra.  Cuando est� en un espacio entre dos palabras, M-f mueve al
final de la siguiente palabra.  M-b trabaja de la misma forma en la
direcci�n opuesta.

>> Teclee algunos M-f y M-b, intercaladas con C-f y C-b de tal forma
   que observe la acci�n de M-f y M-b desde varios sitios dentro y
   entre palabras.

Note el paralelo entre C-f y C-b de un lado y M-f y M-b del otro.  Muy
frecuentemente los caracteres Meta se usan para operaciones
relacionadas, con las unidades definidas por el lenguaje (palabras,
oraciones y p�rrafos), mientras los caracteres Control operan sobre
unidades b�sicas que son independientes de lo que est� editando
(caracteres, l�neas, etc).

Este paralelo se aplica entre l�neas y oraciones: C-a y C-e para
moverse al comienzo o al final de la l�nea; y M-a y M-e para mover al
comienzo o al final de una oraci�n.

>> Intente algunos C-a, y despu�s unos cuantos C-e.
>> Intente algunos M-a, y despu�s unos cuantos M-e.

Vea c�mo la repetici�n de C-a no hace nada, pero la repetici�n de M-a
sigue moviendo una oraci�n m�s.  Aunque no son muy an�logas, cada una
parece natural.

La ubicaci�n del cursor en el texto se llama tambi�n "punto".  En
otras palabras, el cursor muestra sobre la pantalla donde est�
situado el punto dentro del texto.

Aqu� hay un resumen de operaciones simples de movimiento del cursor,
incluyendo los comandos de movimiento por palabra y oraci�n:

	C-f 	Avanzar un car�cter
	C-d 	Retroceder un car�cter

	M-f 	Avanzar una palabra
	M-b 	Retroceder una palabra

	C-n 	Avanzar a la l�nea siguiente
	C-p 	Retroceder a la l�nea anterior

	C-a 	Retroceder al comienzo de la l�nea
	C-e 	Avanzar al final de la l�nea

	M-a 	Retroceder al comienzo de la oraci�n
	M-e 	Avanzar al final de la oraci�n

>> Ahora pruebe todos estos comandos algunas veces para practicar.
   Estos comandos son los m�s frecuentemente usados.

Otros dos comandos importantes de movimiento del cursor son M-< (META
Menor que), el cual se mueve al comienzo del texto entero, y M-> (META
Mayor que), el cual se mueve al final del texto entero.

En la mayor�a de las terminales, el "<" est� sobre la coma, por lo
tanto tiene que usar la tecla shift para teclearlo.  En estas
terminales tendr� que usar la tecla shift tambi�n al teclear M-<; sin
la tecla shift, usted estar�a escribiendo M-coma.

>> Ahora pruebe M-<, para moverse al comienzo del tutorial.
   Despu�s use C-v repetidamente para regresar aqu�.

>> Ahora pruebe M->, para moverse al final del tutorial.
   Despu�s use M-v repetidamente para regresar aqu�.

Tambi�n puede mover el cursor con las teclas de flecha si su terminal
dispone de ellas.  Recomendamos aprender C-b, C-f, C-n y C-p por tres
razones.  Primero, funcionan en todo tipo de terminales.  Segundo, una
vez que gane pr�ctica usando Emacs, encontrar� que teclear estos
caracteres Control es m�s r�pido que usar teclas de flecha (porque no
tendr� que mover las manos de la posici�n para mecanografiar).
Tercero, una vez tenga el h�bito de usar estos comandos Control,
tambi�n puede aprender a usar otros comandos avanzados de movimiento
del cursor f�cilmente.

La mayor�a de comandos de Emacs aceptan un argumento num�rico; para la
mayor�a de comandos esto sirve como un factor de repetici�n.  La
manera de pasarle un factor de repetici�n a un comando es tecleando
C-u y luego los d�gitos antes de introducir los comandos.  Si tiene
una tecla META (o EDIT o ALT), hay una manera alternativa para
ingresar un argumento num�rico: teclear los d�gitos mientras presiona
la tecla META.  Recomendamos aprender el m�todo C-u porque este
funciona en cualquier terminal.  El argumento num�rico es tambi�n
llamado un "argumento prefijo", porque usted teclea el argumento antes
del comando al que se aplica.

Por ejemplo, C-u 8 C-f mueve hacia adelante ocho caracteres.

>> Pruebe usar C-n o C-p con un argumento num�rico, para mover el
   cursor a una l�nea cercana a �sta con un solo comando.

La mayor�a de comandos usan el argumento num�rico como un factor de
repetici�n, pero algunos comandos le dan otros usos.  Varios comandos
(pero ninguno de los que ha aprendido hasta ahora) lo usan como una
bandera: la presencia de un argumento prefijo, sin tener en cuenta su
valor, hace que el comando act�e de forma diferente.

C-v y M-v son otro tipo de excepci�n.  Cuando se les da un argumento,
desplazan la pantalla arriba o abajo esa cantidad de l�neas, en vez de
una pantalla completa.  Por ejemplo, C-u 8 C-v desplaza la pantalla 8
l�neas.

>> Pruebe tecleando C-u 8 C-v ahora.

Esto debi� haber desplazado la pantalla hacia arriba 8 l�neas.  Si
quisiera desplazarla hacia abajo de nuevo, puede dar un argumento a
M-v.

Si est� usando un sistema de ventanas, como X11 o MS-Windows, debe
haber una larga �rea rectangular llamada una barra de desplazamiento
en el lado izquierdo de la ventana de Emacs.  Puede desplazar el texto
al oprimir el bot�n del rat�n en la barra de desplazamiento.

>> Pruebe presionando el bot�n del medio en la parte superior del �rea
   resaltada en la barra de desplazamiento.  �ste debe desplazar el
   texto a una posici�n determinada seg�n cuan alto o bajo oprima el
   bot�n.

>> Intente mover el rat�n arriba y abajo, mientras mantiene el bot�n
   del medio presionado.  Ver� que el texto se desplaza arriba y abajo
   a medida que mueve el rat�n.



* CUANDO EMACS EST� BLOQUEADO
-----------------------------

Si Emacs dejara de responder a sus comandos, puede detenerlo con
seguridad al teclear C-g.  Puede usar C-g para detener un comando que
est� tomando mucho tiempo para ejecutarse.

Tambi�n puede usar C-g para descartar un argumento num�rico o el
comienzo de un comando que no quiere finalizar.

>> Escriba C-u 100 para hacer un argumento num�rico de 100, entonces
   pruebe C-g.
   Ahora pruebe C-f.  Esto deber� mover s�lo un car�cter, ya que
   cancel� el argumento con C-g.

Si ha tecleado <ESC> por error, puede desecharlo con un C-g.


* COMANDOS DESACTIVADOS
-----------------------

Algunos comandos de Emacs est�n "desactivados" de manera que los
usuarios principiantes no puedan usarlos accidentalmente.

Si teclea uno de los comandos desactivados, Emacs muestra un mensaje
informando acerca de qu� comando era, y pregunt�ndole si quiere
continuar y ejecutar el comando.

Si realmente quiere intentar el comando, teclee Espacio como repuesta
a la pregunta.  Normalmente, si no quiere ejecutar el comando
desactivado, conteste la pregunta con "n".

>> Escriba C-x C-l (que es un comando desactivado), a continuaci�n
   escriba n para responder la pregunta.


* VENTANAS
----------

Emacs puede tener varias ventanas, cada una mostrando su propio texto.
Explicaremos despu�s como usar m�ltiples ventanas.  Ahora mismo
queremos explicar c�mo deshacerse de ventanas adicionales y volver a
la edici�n b�sica en una ventana.  Es sencillo:

	C-x 1 	Una ventana (p.ej.,  elimina todas las otras ventanas).

Esto es CONTROL-x seguido por el d�gito 1.  C-x 1 expande la ventana que
contiene el cursor, para ocupar toda la pantalla.  Esto borra todas las
dem�s ventanas.

>> Mueva el cursor a esta l�nea y escriba C-u 0 C-l.
>> Escriba Control-h k Control-f.
   Vea como esta ventana se encoge, mientras una nueva aparece y
   muestra documentaci�n sobre el comando Control-f.

>> Escriba C-x 1 y vea que la ventana de listado de documentaci�n
   desaparece.

Este comando es diferente a los otros que ha aprendido en que �ste
consiste de dos caracteres.  Comienza con el car�cter CONTROL-x.  Hay
toda una serie de comandos que comienzan con CONTROL-x; muchos de
ellos tienen que ver con ventanas, archivos, buffers y cosas
relacionadas.  Estos comandos son de una longitud de dos, tres o
cuatro caracteres.


* INSERTAR Y BORRAR
-------------------

Si quiere insertar un texto, basta con que lo teclee.  Emacs
interpreta los caracteres que usted puede ver, tales como A, 7, *,
etc. como texto y los inserta inmediatamente.  Teclee <Return> (la
tecla Enter) para insertar un car�cter de nueva l�nea.

Puede borrar el �ltimo car�cter que escribi� oprimiendo <Delback>.
<Delback> es una tecla en el teclado--la misma que normalmente usa
fuera de emacs para borrar el �ltimo car�cter que escribi�.
Normalmente es una tecla una o dos filas arriba de la tecla <Return>,
y que esta usualmente rotulada como "Backspace", "Del" o simplemente
con una flecha en direcci�n izquierda que no es parte de las teclas de
flecha.

Si la tecla larga esta rotulada "Backspace", entonces esa es la que
debe de usar para <Delback>. Puede haber otra tecla llamada "Del" en
otra parte, pero esa no es <Delback>.

Generalmente, <Delback> borra el car�cter inmediatamente anterior a la
posici�n actual del cursor.

>> Haga esto ahora: teclee unos pocos caracteres, despu�s b�rrelos
   tecleando <Delback> varias veces.  No se preocupe si este archivo
   cambia, no alterar� el tutorial principal.  �sta es su copia
   personal de �l.

Cuando una l�nea de texto se hace muy grande para una sola l�nea en la
pantalla, la l�nea de texto "contin�a" en una segunda l�nea en la
pantalla.  Un backslash ("\") (o, si est� usando un sistema de
ventanas, una peque�a flecha curva) en el margen derecho indica que la
l�nea "contin�a".

>> Inserte texto hasta que llegue al margen derecho, y siga
   insertando.  Ver� aparecer una l�nea de continuaci�n.

>> Use <Delback> para borrar el texto hasta que la l�nea de nuevo
   quepa en la pantalla.  La l�nea de continuaci�n se pierde.

Puede borrar un car�cter de nueva l�nea como cualquier otro car�cter.
Al borrar el car�cter de nueva l�nea entre dos l�neas las junta en una
sola l�nea.  Si el resultado de la combinaci�n de l�neas es demasiado
largo para caber en el ancho de la pantalla, se mostrar� con una l�nea
de continuaci�n.

>> Mueva el cursor al comienzo de una l�nea y teclee <Delback>.  Esto
   juntar� esa l�nea con la l�nea anterior.

>> Teclee <Return> para reinsertar la nueva l�nea que borr�.

Recuerde que a la mayor�a de los comandos de Emacs se les puede dar un
factor de repetici�n; esto incluye los caracteres de texto.  Repetir
un car�cter de texto lo inserta varias veces.

>> Int�ntelo ahora: teclee C-u 8 * para insertar ********.

Ya ha aprendido la manera m�s b�sica de teclear algo en Emacs y
corregir errores.  Puede borrar por palabras o por l�neas.  He aqu� un
resumen de las operaciones de borrado:

	<Delback> 	borra el car�cter justo antes que el cursor
	C-d		borra el siguiente car�cter despu�s del cursor

	M-<Delback> 	Elimina la palabra inmediatamente antes del
                         cursor
	M-d        	Elimina la siguiente palabra despu�s del cursor

	C-k  		Elimina desde el cursor hasta el fin de la l�nea
	M-k  		Elimina hasta el final de la oraci�n actual

Note que <Delback> y C-d, comparados con M-<Delback> y M-d, extienden
el paralelismo iniciado por C-f y M-f (bien, <Delback> no es realmente
una tecla de control, pero no nos preocuparemos de eso ahora).  C-k y
M-k, en ciertas forma, son como C-e y M-e, en que las l�neas son
oraciones opuestas.

Tambi�n puede eliminar cualquier parte del buffer con un m�todo
uniforme.  Mu�vase a un extremo de esa parte, y teclee C-@ o C-SPC
(cualquiera de los dos).  (SPC es la barra espaciadora.)  Mu�vase al
otro extremo de esa parte, y teclee C-w.  Eso elimina todo el texto
entre las dos posiciones.

>> Mueva el cursor a la letra T del inicio del p�rrafo anterior.
>> Teclee C-SPC. Emacs debe mostrar el mensaje "Mark set" en la parte
   de abajo de la pantalla.
>> Mueva el cursor a la x en "extremo", en la segunda l�nea del
   p�rrafo.
>> Teclee C-w.  Esto eliminar� el texto que comienza desde la T, y
   termina justo antes de la x.

La diferencia entre "eliminar" y "borrar" es que el texto "eliminado"
puede ser reinsertado, mientras que las cosas "borradas" no pueden ser
reinsertadas.  La reinserci�n de texto eliminado se llama "yanking" o
"pegar".  Generalmente, los comandos que pueden quitar mucho texto lo
eliminan, mientras que los comandos que quitan solo un car�cter, o
solo lineas en blanco y espacios, borran (para que no pueda pegar ese
texto).

>> Mueva el cursor al comienzo de una l�nea que no est� vac�a.
   Luego teclee C-k para eliminar el texto de esa l�nea.
>> Teclee C-k por segunda vez.  Ver� que elimina la nueva l�nea que
   sigue a esa l�nea.

Note que al teclear C-k una sola vez elimina el contenido de la l�nea,
y un segundo C-k elimina la l�nea misma, y hace que todas las otras
l�neas se muevan hacia arriba.  C-k trata un argumento num�rico
especialmente: Elimina ese n�mero de l�neas y TAMBI�N sus
contenidos. Esto no es una simple repetici�n.  C-u 2 C-k elimina dos
l�neas y sus nuevas l�neas, tecleando C-k dos veces no hace esto.

Traer texto eliminado de regreso es llamado "yanking" o "pegar".
(Piense en ello como pegar de nuevo, o traer de vuelta, alg�n texto
que le fue quitado.)  Puede pegar el texto eliminado en, ya sea el
lugar en que fue eliminado, o en otra parte del buffer, o hasta en un
archivo diferente. Puede pegar el texto varias veces, lo que hace
varias copias de �l.

El comando para pegar es C-y.  Reinserta el �ltimo texto eliminado, en
la posici�n actual del cursor.

>> Int�ntelo; teclee C-y para pegar de nuevo el texto.

Si hace varios C-k seguidos, todo el texto eliminado se guarda junto,
de manera que un C-y pegar� todas las l�neas al mismo tiempo.

>> Haga esto ahora, teclee C-k varias veces.

Ahora para recuperar ese texto eliminado:

>> Teclee C-y.  Luego baje el cursor unas pocas l�neas y teclee C-y de
   nuevo.  De esta forma puede copiar un texto.

�Qu� hacer si tiene alg�n texto que quiere pegar, y entonces elimina
otra cosa?  C-y pegar�a la eliminaci�n m�s reciente.  Pero el texto
previo no est� perdido.  Puede regresar a �ste usando el comando M-y.
Despu�s de haber tecleado C-y para conseguir la eliminaci�n m�s
reciente, tecleando M-y reemplaza el texto pegado con la eliminaci�n
previa.  Tecleando M-y una y otra vez traer� las eliminaciones
anteriores.  Cuando haya encontrado el texto que buscaba, no tiene que
hacer nada para conservarlo.  S�lo siga con su edici�n, dejando el
texto pegado en donde est�.

Si teclea M-y suficientes veces, regresa al punto inicial (la
eliminaci�n m�s reciente).

>> Elimine una l�nea, mu�vase un poco, elimine otra l�nea.
   Luego teclee C-y para recuperar la segunda l�nea eliminada.
   Luego teclee M-y y ser� reemplazado por la primera l�nea eliminada.
   Teclee m�s veces M-y y vea lo que obtiene.  Siga haci�ndolo hasta
   que la segunda l�nea eliminada regrese, y entonces unas pocas
   m�s. Si quiere, puede tratar de darle a M-y argumentos positivos y
   negativos.


* DESHACER
----------

Si hace un cambio al texto, y luego decide que fue un error,
puede deshacer el cambio con el comando deshacer, C-x u.

Normalmente, C-x u deshace los cambios hechos por un comando; si repite
varias veces seguidas C-x u, cada repetici�n deshar� un comando
adicional.

Pero hay dos excepciones: los comandos que no cambian el texto no
cuentan (esto incluye los comandos de movimiento del cursor y el
comando de desplazamiento), y los caracteres de autoinserci�n se
manejan usualmente en grupos de hasta 20. (Esto es para reducir el
numero de C-x u que tenga que teclear para deshacer una inserci�n en
el texto.)

>> Elimine esta l�nea con C-k, despu�s teclee C-x u y deber�a
   reaparecer.

C-_ es un comando alternativo para deshacer; funciona igual que C-x u,
pero es m�s f�cil de teclear varias veces seguidas.  La desventaja de
C-_ es que en algunos teclados no es obvio c�mo se teclea.  Por esto
existe tambi�n C-x u.  En algunas terminales, puede teclear C-_ al
teclear / mientras oprime CONTROL.

Un argumento num�rico para C-_ o C-x u act�a como un factor de
repetici�n.

Uuede deshacer un texto borrado justo como puede deshacer el texto
eliminado.  La distinci�n entre eliminar algo y borrar algo afecta en
si puede pegarlo con C-y; no hay diferencia alguna para deshacer.


* ARCHIVOS
----------

Para que pueda hacer permanente el texto que edite, lo debe colocar en
un archivo.  De otra manera, �ste se perder� cuando cierre Emacs.
Para poder poner su texto en un archivo, debe "encontrar" el archivo
antes de ingresar el texto. (Esto se llama tambi�n "visitar" el
archivo.)

Encontrar un archivo significa que puede ver su contenido dentro de
Emacs.  En cierta forma, es como si estuviera editando el archivo
mismo.  Sin embargo los cambios que haga mediante Emacs no ser�n
permanentes hasta que "guarde" el archivo.  Esto es para evitar dejar
un archivo a medio cambiar en el sistema cuando no quiera.  Incluso
cuando guarde, Emacs dejar� el archivo original bajo un nombre
cambiado en caso de que luego decida que sus cambios fueron un error.

Si mira cerca del final de la pantalla podr� ver una l�nea que
comienza y termina con guiones, y comienza con "--:-- TUTORIAL.es" o
algo as�.  Esta parte de la pantalla normalmente muestra el nombre del
archivo que est� visitando.  En este momento est� visitando un archivo
llamado "TUTORIAL.es" que es su borrador personal del tutorial de
Emacs.  Cuando encuentre un archivo con Emacs, el nombre de ese
archivo aparecer� en ese mismo punto.

Una cosa especial acerca del comando para encontrar un archivo, es que
tendr� que decir que nombre de archivo desea.  Decimos que el comando
"lee un argumento desde la terminal" (en este caso, el argumento es el
nombre del archivo).  Despu�s de teclear el comando:

	C-x C-f   Encontrar un archivo

Emacs le pide que teclee el nombre del archivo.  El nombre de archivo
que teclee aparece en la l�nea final de la pantalla.  A la l�nea final
de la pantalla se la denomina minibuffer cuando se utiliza para este
tipo de entradas.  Puede usar comandos de edici�n ordinarios de Emacs
para editar el nombre del archivo.

Mientras est� ingresando el nombre del archivo (o cualquier otra
entrada al minibuffer) puede cancelar el comando con C-g.

>> Teclee C-x C-f, luego teclee C-g.  Esto cancela el minibuffer, y
   tambi�n cancela el comando C-x C-f que estaba usando el minibuffer.
   As� que no encontrar� archivo alguno.

Cuando haya finalizado de ingresar el nombre del archivo, teclee
<Return> para terminarlo.  Entonces el comando C-x C-f trabaja, y
encuentra el archivo que escogi�.  El minibuffer desaparece cuando el
comando C-x C-f termina.

Poco tiempo despu�s aparecer� el contenido del archivo en la pantalla,
y puede editarlo.  Cuando quiera que sus cambios sean permanentes,
teclee el comando

	C-x C-s   Guardar el archivo

Esto copia el texto dentro de Emacs al archivo.  La primera vez que
haga esto, Emacs renombrar� el archivo original con un nuevo nombre
para que este no se pierda. El nuevo nombre se hace agregando "~" al
final del nombre del archivo original.

Cuando guardar haya terminado, Emacs mostrar� el nombre del archivo
escrito.  Deber� guardar frecuentemente, para que no pierda mucho
trabajo si el sistema falla.

>> Teclee C-x C-s, guardando la copia del tutorial.
   Esto deber�a mostrar "Wrote ...TUTORIAL.es" al final de la
   pantalla.

NOTA: En algunos sistemas, teclear C-x C-s dejar� inm�vil la pantalla
y no podr� ver m�s respuesta de Emacs.  Esto indica que una
"caracter�stica" del sistema operativo llamada "control de flujo" est�
interceptando el C-s y no permiti�ndole llegar hasta Emacs.  Para
descongelar la pantalla, teclee C-q.  Luego consulte la secci�n
"Entrada Espont�nea para B�squeda Incremental" en el manual de Emacs
para consejos de c�mo tratar con esta "caracter�stica".

Puede encontrar un archivo existente, para verlo o editarlo.  Tambi�n
puede hacerlo con un archivo que no exista.  �sta es la forma de crear
un archivo en Emacs: encuentre el archivo, que comenzar� vac�o, luego
comience a insertar el texto para ese archivo.  Cuando invoque
"guardar" el archivo, Emacs crear� realmente el archivo con el texto
que ha insertado.  De ah� en adelante, puede considerarse estar
editando un archivo existente.


* BUFFERS
---------

Si visita un segundo archivo con C-x C-f, el primer archivo permanece
dentro de Emacs.  Puede volver a el encontr�ndolo de nuevo con C-x
C-f.  De esta forma puede mantener un gran n�mero de archivos dentro
de Emacs.

>> Cree un archivo llamado "foo" tecleando C-x C-f foo <Return>.
   Luego inserte alg�n texto, ed�telo, y guarde "foo" tecleando C-x
   C-s.
   Finalmente teclee C-x C-f TUTORIAL.es <Return>
   para regresar al tutorial.

Emacs almacena cada texto del archivo dentro de un objeto llamado
"buffer".  Al encontrar un archivo se crea un nuevo buffer dentro de
Emacs.  Para mirar la lista de los buffers que existen actualmente en
su sesi�n de Emacs, teclee:

	C-x C-b   Lista de Buffers

>> Pruebe C-x C-b ahora.

Vea como cada buffer tiene un nombre, y adem�s puede tener un nombre
de archivo para el archivo que contiene. CUALQUIER texto que vea en
una ventana de Emacs es siempre parte de alg�n Buffer.

>> Teclee C-x 1 para deshacerse de la lista de buffers.

Cuando tenga varios buffers, solo uno de ellos es "actual" en alg�n
momento.  Ese buffer es el que actualmente edita.  Si quiere editar
otro buffer, necesita "cambiar" a �l.  Si quiere cambiar a un buffer
que corresponde a un archivo, puede hacerlo visitando el archivo de
nuevo con C-x C-f.  Pero existe una manera m�s r�pida: use el comando
C-x b. En ese comando, necesita teclear el nombre de buffer.

>> Teclee C-x b foo <Return> para volver al buffer "foo" que contiene
   el texto del archivo "foo".  Despu�s teclee C-x b TUTORIAL.es
   <Return> para regresar a este tutorial.

La mayor�a del tiempo el nombre del buffer es el mismo que el nombre
del archivo (sin la parte del directorio del archivo). Sin embargo,
esto no es as� siempre.  La lista de buffers que hace con C-x C-b
siempre muestra el nombre de todos los buffers.

CUALQUIER texto que vea en una ventana de Emacs siempre es parte de un
buffer.  Algunos buffers no corresponden a un archivo.  Por ejemplo,
el buffer llamado "*Buffer List*" no tiene ning�n archivo.  Es el
buffer que contiene la lista de buffers que ha creado con C-x C-b.  El
buffer llamado "*Messages*" tampoco tiene un archivo correspondiente;
contiene los mensajes que han aparecido en la l�nea de abajo durante
su sesi�n de Emacs.

>> Teclee C-x b *Messages* <Return> para ver el buffer de mensajes.
   Luego teclee C-b TUTORIAL <Return> para regresar a este tutorial.

Si hace cambios al texto de un archivo, y luego encuentra otro
archivo, esto no guarda el primer archivo.  Sus cambios permanecer�n
dentro de Emacs en ese buffer del archivo.  La creaci�n o edici�n del
segundo buffer de archivo no afecta al primero.  Esto es muy �til,
pero tambi�n significa que necesita una forma conveniente para guardar
el archivo del primer buffer.  Ser�a una molestia tener que volver a
este con C-x C-f para guardarlo con C-x C-s. As� tenemos

	C-x s 	Guardar algunos buffers

C-x s le pregunta sobre cada buffer que contenga cambios que no haya
guardada.  Le pregunta, por cada buffer, si quiere guardarlo o no.

>> Inserte una l�nea de texto, luego teclee C-x s.
   Deber�a preguntarle si desea guardar el buffer llamado TUTORIAL.es.
   Conteste si a la pregunta tecleando "y".


* EXTENDER EL CONJUNTO DE COMANDOS
----------------------------------

Hay much�simos m�s comandos de Emacs que los que podr�an asignarse a
todos los caracteres control y meta.  Emacs puede darle la vuelta a
esto usando el comando X (eXtendido).  Este viene de dos formas:

	C-x 	Car�cter eXtendido. Seguido por un car�cter.
        M-x 	Comando eXtendido por nombre. Seguido por un nombre
                largo.

Estos comandos son generalmente �tiles pero menos usados que los
comandos que ha aprendido hasta ahora.  Ya ha visto dos: los comandos
de archivo C-x C-f para Encontrar y C-x C-s para Guardar.  Otro
ejemplo es el comando para terminar la sesi�n de Emacs: se trata del
comando C-x C-c.  (No se preocupe por perder los cambios que haya
hecho; C-x C-c ofrece guardar cada archivo alterado antes de finalizar
Emacs.)

C-z es el comando para salir de Emacs *temporalmente*: para que pueda
regresar a la misma sesi�n de Emacs despu�s.

En sistemas que lo permiten C-z "suspende" Emacs; esto es, se regresa
al int�rprete de comandos pero no se destruye Emacs.  En los
int�rpretes de comandos m�s comunes, puede reanudar Emacs con el
comando `fg' o con `%emacs'.

En sistemas que no implementen el suspendido, C-z crea un
subint�rprete que corre bajo Emacs para darle la opci�n de correr
otros programas y regresar a Emacs despu�s; esto en realidad no "sale"
de Emacs.  En este caso, el comando `exit' del int�rprete es la v�a
usual para regresar a Emacs desde �ste.

El momento para usar C-x C-c es cuando est� listo para salir del
sistema.  Es adem�s el paso correcto para salir de un Emacs llamado
bajo programas de manejo de correo y diversas otras utilidades, puesto
que ellos no saben c�mo lidiar con la suspensi�n de Emacs.  En
circunstancias normales, si no va a salir, es mejor suspender
Emacs con C-z en lugar de salir de �l.

Existen varios comandos C-x.  Aqu� hay una lista de los que ha
aprendido:

	C-x C-f 	Encontrar archivo.
        C-x C-s 	Guardar archivo.
        C-x C-b 	Lista de buffers.
        C-x C-c 	Salir de Emacs.
        C-x 1 		Borrar todo menos una ventana.
        C-x u 		Deshacer.

Los comandos eXtendidos por nombre son comandos que se utilizan a�n
con menos frecuencia, o �nicamente en ciertos modos.  Un ejemplo es el
comando replace-string, el cual globalmente substituye una cadena de
caracteres por otra.  Cuando teclea M-x, Emacs le pregunta al
final de la pantalla con M-x y debe escribir el nombre del
comando; en este caso "replace-string".  Solo teclee "repl s<TAB>" y
Emacs completar� el nombre.  Finalice el nombre del comando con
<Return>.

El comando replace-string requiere dos argumentos: la cadena de
caracteres a reemplazar, y la cadena de caracteres para reemplazarla.
Debe terminar cada argumento con <Return>.

>> Mueva el cursor hacia la l�nea en blanco dos l�neas abajo de esta.
   A continuaci�n escriba
   M-x repl s<Return>cambiado<Return>alterado<Return>.

   Note c�mo esta l�nea ha cambiado: ha substituido la palabra
   c-a-m-b-i-a-d-o por "alterado" en cada ocurrencia, despu�s de la
   posici�n inicial del cursor.


* AUTO GUARDADO
---------------

Si ha hecho cambios en un archivo, pero no los ha guardado, �stos
podr�an perderse si su computadora falla.  Para protegerlo de esto,
Emacs peri�dicamente escribe un archivo "auto guardado" para cada
archivo que est� editando.  El nombre del archivo auto guardado tiene
un # al principio y al final; por ejemplo, si su archivo se llama
"hola.c", su archivo auto guardado es "#hola.c#".  Cuando guarda por
la v�a normal, Emacs borra su archivo de auto guardado.

Si la computadora falla, puede recuperar su edici�n de auto
guardado encontrando el archivo normal (el archivo que estuvo
editando, no el archivo de auto guardar) y entonces tecleando M-x
recover file<Return>.  Cuando le pregunte por la confirmaci�n, teclee
yes<Return> para ir y recuperar la informaci�n del auto guardado.


* �REA DE ECO
-------------

Si Emacs ve que usted est� tecleando comandos de multicaracteres
lentamente, se los muestra al final de la pantalla en un �rea llamada
"�rea de eco".  El �rea de eco contiene la l�nea final de la pantalla.


* L�NEA DE MODO
---------------

La l�nea inmediatamente encima del �rea de eco recibe el nombre de
"l�nea de modo" o "mode line".  La l�nea de modo dice algo as�:

--:** TUTORIAL.es         (Fundamental)--l765--65%---------

Esta l�nea da informaci�n �til acerca del estado de Emacs y del texto
que est� editando.

Ya sabe qu� significa el nombre del archivo: es el archivo que usted
ha encontrado.  -NN%-- indica su posici�n actual en el texto; esto
significa que NN por ciento del texto est� encima de la parte superior
de la pantalla.  Si el principio del archivo est� en la pantalla, este
dir� --Top-- en vez de --00%--.  Si el final del texto est� en la
pantalla, dir� --Bot--.  Si est� mirando un texto tan peque�o que cabe
en la pantalla, el modo de l�nea dir� --All--.

La L y los d�gitos indican la posici�n de otra forma: ellos dan el
n�mero de l�nea actual del punto.

Los asteriscos cerca del frente significan que usted ha hecho cambios
al texto.  Inmediatamente despu�s que visite o guarde un archivo, esa
parte de la l�nea de modo no muestra asteriscos, solo guiones.

La parte de la l�nea de modo dentro de los par�ntesis es para
indicarle en qu� modo de edici�n est�.  El modo por omisi�n es
Fundamental, el cual est� usando ahora.  Este es un ejemplo de un
"modo mayor".

Emacs tiene diferentes modos mayores.  Algunos est�n hechos para
editar diferentes lenguajes y/o clases de texto, tales como modo de
Lisp, modo de Texto, etc.  En cualquier momento uno y solo un modo
mayor est� activo, y su nombre siempre se puede encontrar en la l�nea
de modo, justo en donde "Fundamental" est� ahora.

Cada modo mayor hace que algunos comandos act�en diferente.  Por
ejemplo, hay comandos para crear comentarios en un programa, y como
cada lenguaje de programaci�n tiene una idea diferente de c�mo debe
verse un comentario, cada modo mayor tiene que insertar comentarios de
forma distinta.  Cada modo mayor es el nombre de un comando extendido,
que es como puede cambiar a ese modo.  Por ejemplo, M-x
fundamental-mode es un comando para cambiar al modo fundamental.

Si va a editar un texto de alg�n lenguaje humano, como este archivo,
deber�a usar el modo de texto.
>> Teclee M-x text mode<Return>.

No se preocupe, ninguno de los comandos de Emacs que ha aprendido
cambia de manera significativa.  Pero puede observar que M-f y M-b
tratan los ap�strofes como parte de las palabras.  Previamente, en
modo Fundamental, M-f y M-b trataban los ap�strofes como separadores
de palabras.

Los modos mayores normalmente hacen cambios sutiles como el anterior:
la mayor�a de comandos hacen "el mismo trabajo" en cada modo mayor,
pero funcionan un poco diferente.

Para ver documentaci�n en el modo mayor actual, teclee C-h m.

>> Use C-u C-v una o m�s veces para traer esta l�nea cerca de la
   parte superior de la pantalla.

>> Teclee C-h m, para ver como el modo de Texto difiere del modo
   Fundamental.

>> Teclee C-x 1 para eliminar la documentaci�n de la pantalla.

Los modos mayores son llamados as� porque tambi�n hay modos menores.
Los modos menores no son alternativas para los modos mayores, solo
modificaciones menores de �stos.  Cada modo menor puede ser activado o
desactivado por s� mismo, independiente de todos los otros modos
menores, e independiente de su modo mayor.  Por tanto, puede no usar
modos menores, o solamente uno, o cualquier combinaci�n de varios
modos menores.

Un modo menor que es muy �til, especialmente para editar textos en
espa�ol, es el modo Auto Fill.  Cuando este modo est� activado, Emacs
rompe la l�nea entre palabras autom�ticamente siempre que inserte
texto y la l�nea sea demasiado ancha.

Puede activar el modo Auto Fill al hacer M-x auto fill mode<Return>.
Cuando el modo est� activado, puede desactivarlo nuevamente usando M-x
auto fill mode<Return>.  Si el modo est� desactivado, este comando lo
activa, y si el modo est� activado, este comando lo desactiva.
Decimos que el comando "cambia el modo".

>> teclee M-x auto fill mode<Return> ahora.  Luego inserte una l�nea
   de "asdf " repetidas veces hasta que la vea dividida en dos l�neas.
   Debe intercalar espacios porque Auto Fill s�lo rompe l�neas en los
   espacios.

El margen esta normalmente puesto en 70 caracteres, pero puede
cambiarlo con el comando C-x f.  Debe indicar el margen deseado como
un argumento num�rico.

>> Teclee C-x f con un argumento de 20.  (C-u 2 0 C-x f).  Luego
   teclee alg�n texto y vea como Emacs lo parte en l�neas de 20
   caracteres.  A continuaci�n ponga de nuevo el margen a 70 usando
   otra vez C-x f.

Si hace cambios en el medio de un p�rrafo, el modo Auto Fill no lo
rellenar� por usted.
Para rellenar el p�rrafo, teclee M-q (META-q) con el cursor dentro de
ese p�rrafo.

>> Mueva el cursor al p�rrafo anterior y teclee M-q.


* BUSCAR
--------

Emacs puede hacer b�squedas de cadenas (grupos de caracteres o
palabras contiguos) hacia adelante a trav�s del texto o hacia atr�s en
el mismo.  La b�squeda de una cadena es un comando de movimiento de
cursor; mueve el cursor al pr�ximo lugar donde esa cadena aparece.

El comando de b�squeda de Emacs es diferente a los comandos de
b�squeda de los dem�s editores, en que es "incremental".  Esto
significa que la b�squeda ocurre mientras teclea la cadena para
buscarla.

El comando para iniciar una b�squeda es C-s para b�squeda hacia
adelante, y C-r para la b�squeda hacia atr�s.  �PERO ESPERE!  No los
intente a�n.

Cuando teclee C-s ver� que la cadena "I-search" aparece como una
petici�n en el �rea de eco.  Esto le indica que Emacs est� en lo que
se conoce como b�squeda incremental, esperando que teclee lo que
quiere buscar.  <Return> termina una b�squeda.

>> Ahora teclee C-s para comenzar la b�squeda.  LENTAMENTE, una letra
   a la vez, teclee la palabra 'cursor', haciendo pausa despu�s de
   cada car�cter para notar lo que pasa con el cursor.
   Ahora ha buscado "cursor", una vez.
>> Teclee C-s de nuevo, para buscar la siguiente ocurrencia de
   "cursor".
>> Ahora teclee <Delback> cuatro veces y vea como se mueve el cursor.
>> Teclee <Return> para terminar la b�squeda.

�Vi� lo que ocurri�?  Emacs, en una b�squeda incremental, trata de ir
a la ocurrencia de la cadena que ha tecleado hasta el momento.  Para
ir a la pr�xima ocurrencia de 'cursor' solo teclee C-s de nuevo.  Si
tal ocurrencia no existe, Emacs pita y le dice que la b�squeda actual
est� fallando ("failing").  C-g tambi�n termina la b�squeda.

NOTA: En algunos sistemas, teclear C-s dejar� inm�vil la pantalla y no
podr� ver m�s respuesta de Emacs.  Esto indica que una
"caracter�stica" del sistema operativo llamada "control de flujo" est�
interceptando el C-s y no permiti�ndole llegar hasta Emacs.  Para
descongelar la pantalla, teclee C-q.  Luego consulte la secci�n
"Entrada Espont�nea para B�squeda Incremental" en el manual de Emacs
para consejos de c�mo tratar con esta "caracter�stica".

Si se encuentra en medio de una b�squeda incremental y teclea
<Delback>, notar� que el �ltimo car�cter de la cadena buscada se borra
y la b�squeda vuelve al sitio anterior de la b�squeda.  Por ejemplo,
suponga que ha tecleado "c", para buscar la primera ocurrencia de "c".
Ahora, si teclea "u", el cursor se mover� a la primera ocurrencia de
"cu".  Ahora teclee <Delback>.  Esto borra la "u" de la cadena
buscada, y el cursor vuelve a la primera ocurrencia de "c".

Si est� en medio de una b�squeda y teclea un car�cter control o meta
(con algunas pocas excepciones: los caracteres que son especiales en
una b�squeda, tales como C-s y C-r), la b�squeda termina.

El C-s inicia una exploraci�n que busca alguna ocurrencia de la cadena
buscada DESPU�S de la posici�n actual del cursor.  Si quiere buscar
algo anterior en el texto, teclee en cambio C-r.  Todo lo que hemos
dicho sobre C-s tambi�n se aplica a C-r, excepto que la direcci�n de
la b�squeda se invierte.


* M�LTIPLES VENTANAS
--------------------

Una de las caracter�sticas agradables de Emacs es que se puede mostrar
m�s de una ventana en la pantalla al mismo tiempo.

>> Mueva el cursor a esta l�nea y teclee C-u 0 C-l (eso es CONTROL-L,
   no CONTROL-1).

>> Ahora teclee C-x 2 que divide la pantalla en dos ventanas.  Ambas
   ventanas muestran este tutorial.  El cursor permanece en la ventana
   superior.

>> Teclee C-M-v para desplazar la ventana inferior.
   (Si no tiene una tecla META real, teclee ESC C-v.)

>> Teclee C-x o ("o" para "otro") para mover el cursor a la ventana
   inferior.
>> Use C-v y M-v en la ventana inferior para desplazarla.
   Siga leyendo estas direcciones en la ventana superior.

>> Teclee C-x o de nuevo para mover el cursor de vuelta a la ventana
   superior.
   El cursor en la ventana superior est� justo donde estaba antes.

Puede continuar usando C-x o para cambiar entre las ventanas.  Cada
ventana tiene su propia posici�n del cursor, pero �nicamente una
ventana actual muestra el cursor.  Todos los comandos de edici�n
comunes se aplican a la ventana en que est� el cursor.  Llamaremos
esto la "ventana seleccionada".

El comando C-M-v es muy �til cuando est� editando un texto en una
ventana y usando la otra ventana como referencia.  Puede mantener el
cursor siempre en la ventana donde est� editando, y avanzar a la otra
ventana secuencialmente con C-M-v.

C-M-v es un ejemplo de un car�cter CONTROL-META.  Si tiene una tecla
META real, puede teclear C-M-v pulsando a la vez CONTROL y META
mientras teclea v.  No importa qu� tecla "vaya primero", CONTROL o
META, porque las dos teclas act�an modificando los caracteres que
teclea.

Si no tiene una tecla META real, y en vez de eso usa ESC, el orden s�
importa: debe teclear ESC seguido de Control-v, porque Control-ESC v
no funcionar�.  Esto es porque ESC es un car�cter que tiene valor por
s� mismo, no es una tecla modificadora.

>> Teclee C-x 1 (en la ventana de arriba) para deshacerse de la
   ventana de abajo.

(Si hubiera tecleado C-x 1 en la ventana inferior, esto eliminar�a la
superior.  Piense en este comando como "mantener s�lo una
ventana--aquella en la cual estoy.")

No tiene por qu� mostrarse el mismo buffer en ambas ventanas.  Si usa
C-x C-f para encontrar un archivo en una ventana, la otra ventana no
cambia.  Puede encontrar un archivo en cada ventana
independientemente.

Aqu� hay otra forma para usar dos ventanas para mostrar dos cosas
diferentes:

>> Teclee C-x 4 C-f seguido del nombre de uno de sus archivos.
   Finalice con <Return>.  Vea que el archivo especificado aparece en
   la ventana inferior.  El cursor v� all� tambi�n.

>> Teclee C-x o para regresar a la ventana superior, y C-x 1 para
   borrar la ventana inferior.


* NIVELES RECURSIVOS DE EDICI�N
--------------------------------

Algunas veces entrar� a lo que es llamado un "nivel recursivo de
edici�n".  Esto se indica en la l�nea de modo mediante corchetes en la
l�nea de modo, rodeando los par�ntesis del nombre del modo mayor.  Por
ejemplo, probablemente vea [(Fundamental)] en vez de (Fundamental).

Para salir de los niveles recursivos de edici�n, teclee ESC ESC ESC.
�ste es un comando de "salida" para todo prop�sito.  Tambi�n lo puede
usar para eliminar ventanas extras, y salir del minibuffer.

>> Teclee M-x para entrar a un minibuffer; luego teclee ESC ESC ESC
   para salir.

No se puede usar C-g para salir de los "niveles recursivos de
edici�n".  Esto es porque C-g es usado para cancelar comandos y
argumentos DENTRO del nivel recursivo de edici�n.


* CONSEGUIR MAS AYUDA
---------------------

En este tutorial hemos tratado de ofrecer suficiente informaci�n para
que empiece a usar Emacs.  Hay tanto disponible en Emacs que ser�a
imposible explicar todo aqu�.  Sin embargo, quiz� desee aprender m�s
sobre Emacs, ya que tiene muchas otras caracter�sticas �tiles.  Emacs
provee comandos para leer documentaci�n acerca de los comandos de
Emacs.  Todos estos comandos de "ayuda" comienzan con el car�cter
Control-h, que es llamado "el car�cter de Ayuda (Help)".

Para usar las funciones de ayuda, teclee el car�cter C-h, y luego un
car�cter decidiendo qu� tipo de ayuda quiere.  Si est� REALMENTE
perdido teclee C-h ? y Emacs le dir� qu� tipo de ayuda puede
ofrecerle.  Si ha tecleado C-h y decide que no quiere ninguna ayuda,
teclee C-g para cancelarlo.

(En algunas instalaciones cambian el significado del car�cter C-h.
Realmente no deber�an hacer esto como una pol�tica para todos los
usuarios, as� que tiene argumentos para quejarse al administrador del
sistema.  Mientras tanto, si C-h no muestra un mensaje de ayuda en el
final de la pantalla, intente teclear la tecla F1 o, en su lugar, M-x
help <Return>).

La funci�n de AYUDA m�s b�sica es C-h c.  Teclee C-h, el car�cter c y
un car�cter de comando o secuencia de comando; Emacs le mostrar�
una descripci�n muy breve del comando.

>> Teclee C-h c C-p.
  El mensaje debe ser algo como

	  C-p runs the command previous-line

Esto le dice el "nombre de la funci�n".  Los nombres de funci�n se
usan principalmente para adecuar y extender Emacs.  Pero ya que los
nombres de las funciones se eligen para indicar lo que el comando
hace, tambi�n pueden servir como una breve documentaci�n: suficiente
para recordarle los comandos que ha aprendido.

Los comandos de m�ltiples caracteres tales como C-x C-s y (s� no tiene
las teclas META o EDIT o ALT) <ESC>v tambi�n est�n permitidos despu�s
de C-h c.

Para conseguir m�s informaci�n sobre un comando use C-h k en vez de
C-h c.

>> Teclee C-h k C-p.

Esto muestra la documentaci�n de la funci�n, al igual que el nombre,
en una ventana de Emacs.  Cuando haya terminado de leer el resultado,
teclee C-x 1 para deshacerse del texto de ayuda.  No tiene que hacer
esto ahora.  Puede hacer algunas ediciones mientras se refiere
al texto de ayuda, y entonces teclear C-x 1.

Aqu� hay algunas otras opciones �tiles de C-h:

   C-h f	Describe una funci�n.  Usted teclea el nombre de la
		funci�n.

>> Intente teclear C-h f previous-line<Return>.
   Esto muestra toda la informaci�n que Emacs tiene sobre la funci�n
   que implementa el comando C-p

Un comando similar, C-h v, muestra la documentaci�n de variables cuyos
valores pueda poner para adecuar el comportamiento de Emacs.  Necesita
teclear el nombre de la variable cuando Emacs pregunte por ella.

   C-h a 	Comando Apropos. Teclee una palabra y Emacs har� una
		lista de todos los comandos que contengan esa palabra.
		Todos estos comandos pueden ser invocados con META-x.
		Para algunos comandos, el Comando Apropos tambi�n
		listar� una secuencia de uno o dos caracteres la cual
		ejecutar� el mismo comando.

>> Teclee C-h a file<Return>.

Esto muestra en otra ventana una lista de todos los comandos M-x con
la palabra "file" en sus nombres.  Ver� comandos de caracteres como
C-x C-f listados adem�s de los nombres de los comandos
correspondientes tales como find-file.

>> Teclee C-M-v para desplazar la ventana de ayuda.  Haga esto unas
   cuantas veces.

>> Teclee C-x 1 para borrar la ventana de ayuda.

   C-h i        Leer los Manuales En-L�nea (alias Info).  Este comando
                lo pone en un buffer especial llamado `*info*' donde
                puede leer manuales en l�nea de los paquetes
                instalados en su sistema.  Teclee m Emacs <Return>
                para leer el manual de Emacs.  S� nunca ha usado Info
                antes, teclee ? y Emacs lo llevar� en una visita
                guiada de los servicios del modo de Info.  Una vez que
                haya terminado este tutorial, deber�a considerar el
                manual Info de Emacs como su documentaci�n primaria.


* M�S CARACTER�STICAS
---------------------

Puede aprender m�s de Emacs leyendo su manual, ya sea como libro o en
l�nea en el Info (use el men� Ayuda--"Help"--o teclee F10 h r). Dos
caracter�sticas que pueden gustarle son la completaci�n, que ahorra
teclear, y dired, que simplifica el manejo de archivos.

La completaci�n es una manera de ahorrar teclear innecesariamente.
Por ejemplo, si quiere cambiarse al buffer "*Messages*", puede teclear
C-x b *M<Tab> y emacs encontrar� el resto del nombre del buffer tan
lejos como pueda determinar de lo que ya haya tecleado.  La
completaci�n es descrita en el Info del manual de Emacs en el nodo
llamado "Completation".

Dired le permite listar los archivos en un directorio (y opcionalmente
sus subdirectorios), moverse alrededor de esa lista, visitar,
renombrar, borrar y aparte de eso operar en los archivos.  Dired esta
descrito en el Info en el manual de Emacs en el nodo llamado "Dired".

El manual tambi�n describe otras caracter�sticas de Emacs.


* CONCLUSI�N
------------

Recuerde, para salir permanentemente de Emacs use C-x C-c.  Para salir
temporalmente a un int�rprete de comandos, de forma que puede volver a
Emacs despu�s, use C-z.

Este tutorial intenta ser comprensible para todos los usuarios nuevos,
as� que si encuentra algo que no est� claro, no se siente y se culpe a
s� mismo: �Qu�jese!


* COPIA
-------

Este tutorial desciende de una larga l�nea de tutoriales de Emacs
comenzando con el escrito por Stuart Cracraft para el Emacs original.

La versi�n en espa�ol fue originalmente traducida por estudiantes del
Gimnasio Fidel Cano (un colegio en Santaf� de Bogot�, Colombia):

	Carlos Alberto L�pez Troncoso
	Andr�s Felipe Mancipe Galvis
	Lina Fernanda Pinto Garc�a
	Liliana Carolina Quiti�n Cede�o
	Leonardo Ram�rez Vargas <leonardoramirez@latinmail.com>
	Juan David Vargas Botero <cyberbob1164@hotmail.com>
	Juan Pablo Yela Gall�n
	Jorge Enrique C�rdenas Carrillo <platypus_life@hotmail.com>

La versi�n en espa�ol ha sido revisada y corregida por:

	Pablo Reyes <reyes_pablo@hotmail.com>
	Igor T�mara <ikks@bigfoot.com>
	Melissa Giraldo de T�mara <melagira@yahoo.com>
	Vladimir T�mara <vtamara@gnu.org>
        Rafael Sep�lveda <drs@gnulinux.org.mx>
        Juanma Barranquero <lektu@terra.es>

La versi�n en espa�ol ha sido actualizada por:

	Rafael Sep�lveda <drs@gnulinux.org.mx>

Por favor, en caso de duda, s�lo es v�lido el original en ingl�s de la
siguiente nota de derechos de reproducci�n (que puede encontrar en el
archivo TUTORIAL).

Copyright (C) 1985, 1996, 1998, 2001, 2002, 2003, 2004,
   2005, 2006, 2007  Free Software Foundation, Inc.

   Se permite a cualquiera hacer o distribuir copias literales de este
   documento como se recibe, en cualquier medio, siempre que la nota
   de derechos de reproducci�n y la nota de permiso se preserven, y
   que el distribuidor permita que el que la recibe hacer distribuci�n
   posterior como lo permite esta nota.

   Se permite distribuir versiones modificadas de este documento, o
   porciones de este, bajo las condiciones anteriores, siempre que
   ellas tengan nota visible especificando qui�n fue el �ltimo en
   alterarlas.

Las condiciones para copiar Emacs mismo son m�s complejas, pero con el
mismo esp�ritu.  Por favor lea el archivo COPYING y luego distribuya
copias de GNU Emacs a sus amigos.  �Ayude a erradicar el
obstruccionismo del software ("propietariedad") usando, escribiendo, y
compartiendo software libre!

--- end of TUTORIAL.es ---

;;; Local Variables:
;;;   mode: text;
;;;   coding: latin-1
;;; End:

;;; arch-tag: 66aae86e-6f86-4a3e-b82a-44a783f774fd
