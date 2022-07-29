# levitador-ultrasonico

El presente proyecto expone el diseño y la construcción un levitador capaz de hacer 
suspender partículas en el aire a través del principio de las ondas 
electromagnéticas, en este caso, las ondas acústicas mediante transductores 
ultrasónicos; con este principio, una partícula se levitará en contra de la fuerza de 
gravedad por medio de la presión sónica generada y alcanzará el equilibrio al 
contrarrestasen las ondas estacionarias. Además de esto, mediante un sistema de 
control clásico discreto, se realizará control de posición de las partículas en dos 
dimensiones (X, Z) al posicionar dos levitadores perpendicularmente haciendo 
necesario que el centroide de ambos sea común y se hará monitorización a través 
de un sistema HMI usando una cámara y visión artificial, quien detectara la posición 
de la partícula en las dos dimensiones. 

El proyecto inicia con el estudio del fenómeno detrás que la levitación acústica, 
dando a entender el por qué y las condiciones para hacer levitar objetos de mayor 
tamaño, seguidamente de esto, se diseña el modelado en donde se observa el 
posicionamiento de los transductores y las cotas del prototipo, a continuación, se 
realizan las pruebas de levitación de partículas en el prototipo. Una de las secciones 
fundamentales para el control de posición de partículas, es la implementación de la 
visión artificial, responsable de localizar y calcular la posición de la partícula los ejes 
X y Z; por último, se implementa un control clásico PID para controlar la posición de 
las partículas en el levitador que será supervisado por una interfaz humano maquina 
HMI.

