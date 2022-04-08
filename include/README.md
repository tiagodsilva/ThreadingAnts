# Simulation's objects 

We should implement, still, the class `Food`. 

## Contemplação dos objetos  

Vou, neste excerto, descrever, auspiciosamente sucintamente, os objetos e os métodos disponíveis nas linhas que escrevemos; a sua interface, neste caso, está acessível em [`./headers/objects.hpp`](./headers/objects.hpp). Especificamente, estamos equipados com os objetos 

+ `Anthill` (formigueiro), 
+ `Food` (comida), 
+ `Ant` (formiga), 
+ `Tile` (quadrado), 
+ `Map` (mapa) e 
+ `Region` (região); 

a classe região, enfaticamente, é um artifiício que ensejará, em etapas subsequentes, o controle apropriado do acesso simultâneo a espaços de endereçamento compartilhados. Contudo, precismoas *implementar o programa sequencial* e, em seguida, o *paralelizar*; neste sentido, a identificação de inelutáveis inadequações na paralelização, em oposição à inapropriação do sistema, será plausível. Portanto, esta classe, `Region`, é, inicialmente, uma interface com alguns atributos estáticos; seus métodos para asseverar acessos mutuamente exclusivos ainda serão, esperançosamente, implementados. 

Nestas condições, o diagrama seguinte almeja sumarizar, em alguma extensão, as classes convenientes. 

```
 	   ++++++++++(está inserido em)+ Map +++++++++(consiste em)+++++++++ Region  
Anthill ++++						+ 		+ 
           +(contém)+++   				+		+
	              Ant+++(continência contigente)+++Tile 		+
	  	       +						+
Food ++(interage com)+++++++++++++++++++++++(caracterizada em)+++++++++++ 
``` 

Explicitamente, assim, o formigueiro está inserido em um mapa, e ele contém formigas; e, mais precisamente, ele é um atributo do objeto formiga. Além disso, o mapa é um objeto global; apesar de objetos como formiga e comida estarem contempladas em algum quadrado, *elas não gozam de atributos `Tile`* -- precisamos utilizar suas coordenadas e o mapa global para identificar o `Tile` correspondente ao objeto almejado -- formiga ou comida. Por exemplo, para capturar uma formiga no tile `(1, 3)`, escrevemos 

``` 
Ant * ant = map->getAnt(1, 3); 
``` 

e, se quisermos identificar formigas de alguma colônia `x`, 

``` 
Ant * ant = map->getAntInTile(1, 3, std::string("x")); 
``` 

Correspondentemente, podemos computar as coordenadas de algum tile, utilizando, para isso, os métodos `getX` e `getY`; isto é, 

``` 
int x = tile->getX(); 
int y = tile->getY(); 
``` 

Por outro lado, as instâncias de formigas precisam se movimentar nas dimensões do mapa; existe, com efeito, um par de movimentos. Em um deles, o movimento é *informado*; os insetos, contemplando um objeto `Food` em seu campo de visão, se direcionam ao seu quadrado, enquanto, se estiverem munidos de comida, eles almejarão, informados da localização de seu formigueiro, o quadrado originário. Em outro, o movimento é aleatório; neste caso, as formigas, que não vislumbram um objeto `Food`, executam uma ação ranomizada, em que um quadrado é escolhido randomicamente -- com probabilidade proporcional à quantidade de feromônios. Com esse objetivo, utilizamos 

```
ant->moveToColony(); // Bresenham's algorithm 
bool hasFoodNear = ant->moveToFood(); // whether there is food near; in this case, the insect moves 
if (hasFoodNear) 
	ant->moveRandomly(); 
``` 

Verificamos, enfaticamente, que o movimento da formiga é condicional à sua contemplação das proximidades; podemos, na verdade, encapsular estas implementações em um método consolidado.

Mais tarde, vou continuar a descrição. 

## Classes 

Contemplamos, nesta seção, os atributos e os métodos disponíveis em cada objeto que caracteriza a simulação. 

### Anthill 

Classe equivalente ao formigueiro; almejamos, com ela, iniciar a jornada das formigas. 

+ Atributos  

```cpp 
private: 
	std::string name; // o nome (identificador) do formigueiro 
	int x_pos, y_pos; // as suas coordenadas   

	bool isInitialized; // se o formigueiro foi inicializado; isto é, se 
			// as formigas foram instanciadas 
public: 
	int initialAnts; // a quantidade inicial de formigas 
	int foodStorage; // a quantidade de alimento que as formigas trouxeram 
```

+ Métodos 

```cpp 
Anthill(int x, int y, std::string colonyName); // construtor 
void instantiateAnts(int numAnts, int fov); // instancia formigas 
	// no tile com coordenadas `x_pos` e `y_pos`; elas 
	// gozam de um campo de visão igual a `fov` 
void incrementFood(); // insere comida no formigueiro; incrementa `foodStorage` 

int getX(); // captura a coordenada horizontal do formigueiro 
int getY(); // e a vertical 

std::string getName(); // propicia o acesso ao atributo privado `name`, 
		// com o nome do formigueiro 
```

### Food 

Classe contemplando o alimento, para o qual as formigas se deslocam. 

+ Atributos 

```cpp 
private: 
	int x_pos, int y_pos; // as coordenadas da comida 
	int volume; // o volume da alimento disponível 
	
	int initialVolume; // o volume inicial inserido no jogo 
``` 

```cpp 
Food(int x, int y, int initVolume); // constructor 
bool consume(); // tenta capturar uma unidade de volume de alimento; 
	// se a operação for executada, o método computa `True` 
	// (se não houver volume de alimento, por exemplo, ele computa `False`) 
void restore(); // modifica o volume atual para o volume inicial 
int getVolume(); // computa o volume atual de comida no tile 
``` 


