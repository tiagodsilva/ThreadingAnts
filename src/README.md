# Caracterização das classes 

Neste README.md, descrevemos os aspectos mais programáticos do relatório; leia, por gentileza, o relatório para uma descrição mais legível. 

## Contemplação dos objetos  

Vou, neste excerto, descrever, auspiciosamente sucintamente, os objetos e os métodos disponíveis nas linhas que escrevemos; a sua interface, neste caso, está acessível em [`../include/objects.hpp`](../include/objects.hpp). Especificamente, estamos equipados com os objetos 

+ `Anthill` (formigueiro), 
+ `Food` (comida), 
+ `Ant` (formiga), 
+ `Tile` (quadrado), 
+ `Map` (mapa) e 

Contudo, precisamos *implementar o programa serial* e, em seguida, o *paralelizar*; neste sentido, a identificação de inelutáveis inadequações na paralelização, em oposição à inapropriação do sistema, será plausível.

Nestas condições, o diagrama seguinte almeja sumarizar, em alguma extensão, as classes convenientes. 

```
 	   ++++++++++(está inserido em)+ Map +++++++++(consiste em)++++++ 
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

Verificamos, enfaticamente, que o movimento da formiga é condicional à sua contemplação das proximidades; encapsulamos, na verdade, estas implementações em um método consolidado, `stage`.

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
	std::mutex foodMutex; // Mutex para controlar o acesso ao atributo `foodStorage` 
public: 
	int initialAnts; // a quantidade inicial de formigas 
	int foodStorage; // a quantidade de alimento que as formigas trouxeram 
```

+ Métodos 

```cpp 
Anthill(int x, int y, std::string colonyName); // construtor 
void instantiateAnts(int numAnts); // instancia formigas 
	// no tile com coordenadas `x_pos` e `y_pos` 
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
	int x_pos, y_pos; // as coordenadas da comida 
	int volume; // o volume da alimento disponível 
	
	int initialVolume; // o volume inicial inserido no jogo 

	std::mutex attrMutex; // Acesso exclusivo ao atributo `initialVolume` 
	std::mutex seatMutex; // Controle da quantidade de formigas sentadas 
			// em torno da comida 
	int maxAnts; // A quantidade máxima de formigas que podem acessar esta instância 
	int currAnt // A quantidad de formigas que acessaram o alimento nesta iteração
public: 
	int * seats; // assentos desta instância 
	sem_t * eatSemaphores; // semáforos para controle dos bastões 
``` 

```cpp 
private: 
int LEFT(int i); // captura as condições das formigas em ambos os lados da mesa 
int RIGHT(int i); 
void getFreeSeat() // Verifica se há assentos livres 
public: 
Food(int x, int y, int initVolume); // constructor 
bool consume(); // tenta capturar uma unidade de volume de alimento; 
	// se a operação for executada, o método computa `True` 
	// (se não houver volume de alimento, por exemplo, ele computa `False`) 
void restore(); // modifica o volume atual para o volume inicial 
int getVolume(); // computa o volume atual de comida no tile 
void allowAnts(); // permitir que mais formigas acessem este objeto 

// Implementação de Dijkstra para o problema dos filósofos jantando 
void test(int i); 
void takeRods(int i); 
void eat(int i); 
void putRods(int i); 
``` 

### Ant 

Este é o objeto mais enfático; ele enforma as formigas. 

+ Atributos 

```cpp 
private: 
	int x_pos, y_pos; // as coordenadas da formiga no tabuleiro 
	
	Anthill * antHill; // ponteiro para o formigueiro em que a formiga 
		// foi instanciada 
public: 
	bool hasFood; // identifica se a formiga goza de alimento; 
		// neste caso, seus movimentos serão direcionados ao formigueiro 
	bool isDead; // se a formiga está gravemente ferida e, logo, morrerá 
```

+ Métodos 

```cpp 
Ant(int x, int y, Anthill* colony, int fieldOfView); // constructor 
void move(int x, int y); // direciona à formiga ao tile x_pos + x, y_pos + y 
void eat(Food * food); // tenta capturar uma unidade de volume do alimento `food`; 
		// no cenário positivo, declara `hasFood = true` 
int getX(); // captura as coordenadas horizontais da formiga 
int getY(); // e as verticais 

void releasePheromone(); // incrementa a quantidade de feromônios no tile 
	// em que a formiga está 
void moveToColony(); // direciona a formiga ao tile em que `antHill` está 
void moveInSegment(Vec v, Vec w, std::vector<Tile *> neighbors); // direciona 
	// a formiga em um movimento no segmento subjacente aos vetores `v` e `w` 
TIle * hasFoodNear(); // identifica se existe alimento no campo de visão; 
	// se sim, computa o ponteiro de sua localização 
bool moveToFood(); // se houver alimentos próximos (`hasFoodNear`), executa um movimento 
	// a ele 
Anthill * getAnthill(); // captura o atributo privado `antHill` 
void moveRandomly(); // executa um movimento aleatório no mapa; 
		// a probabilidade de algum tile ser escolhido é proporcional 
		// à quantidade de feromônio inserida nele 
void stage(); // consolida as ações da formiga 
bool fight(); // escolhe, possivelmente jogando uma moeda, se uma guerra deve ser declarada 
``` 

### Tile 

Existe, no tabuleiro, um conjunto de quadrados; esta é a classe que os caracteriza. Eles contemplam, enfaticamente, as características mais essenciais da simulação -- a pilha com as formigas. 

+ Atributos 

```cpp 
private: 
	int x, y; // as coordenadas do tile 
	std::mutex tileMutex; // mutexes para controlar acesso a atributos 
	std::mutex pherMutex; 
	std::mutex deathMutex; 
public: 
	std::map<std::string, std::stack<Ant*> * ants; // mapeia 
		// cada colônia para uma pilha com as formigas 
		// instanciadas por ela 
	std::map<std::string, int> deaths; // Contempla a quantidade de formigas mortas em 
		// em cada colônia 
	bool isAnthill; // se há um formigueiro neste tile; 
		// precisamos controlar os movimentos aleatórios a eles 
	std::string anthillName; // se houver um formigueiro, este é seu nome 
	bool isFood; // se há comida; precisamos, também, controlar 
		// o movimento aleatório de formigas a este tile 
	int pheromone; // a intensidade de feromônio, que são 
		// liberados pelas formigas 
``` 

+ Métodos 

```cpp 
Tile(int x_tile, int y_tile, bool containsFood); 
TIle(int x_tile, int y_tile, std::string anthillName); 
Tile(); // constructores apropriados para os cenários em que há alimento, em que
		// há formigueiros -- ou para tiles canônicos 
void insertAnt(Ant * ant); // insere uma formiga na pilha que aponta para sua 
		// colônia 
Ant * extractAnt(Ant * ant); // quando uma formiga se movimenta, ela precisa 
		// ser extraída da pilha de seu tile atual; este método executa, 
		// importantemente, esta tarefa 
std::map<std::string, int> numAnts(); // computa a quantidade de formigas, 
		// por colônia, neste tile 
void incrementPheromone(); // incrementa a quantidade de feromônio neste tile; 
		// este é o método que a ant->releasePheromone() executa, 
		// convenientemente 
std::string print(); // computa uma string para imprimir as informações do tile 
		// no terminal 
int getX(); // computa as coordenadas horizontais no tile 
int getY(); // e as verticais 
Ant * getAnt(); // captura alguma formiga (de qualquer colônia) 
std::vector<Ant*> getAnts(); // computa as formigas nestas coordenadas 
void checkPheromones(); // verifica os feromônios inativos nestas coordenadas 
void incrementDeaths(std::string anthillName); // incrementa a quantidade de formigas mortas 
void killAnts(); // insere o rótulo da morte nas formigas 
``` 

### Map 

A simulação é empreendida em um tabuleiro; nós o chamamos de `Map`. 

+ Atributos 

```cpp 
private: 
	int width, height; // as dimensões do mapa 
	std::vector<Tile*> tiles; // lista com os tiles subjacentes ao tabuleiro 
	std::map<std::string, Anthill*> anthillMap; // identifica o formigueiro 
		// por seu nome 
	std::map<std::pair<int, int>, Food*> foods; // mapeia uma coordenada 
			// ao ponteiro para o objeto alimento; podemos 
			// inserir uma lista global com as suas coordenadas 
	int fov; // todas as formigas gozam do mesmo campo de visão; a discriminação, 
		// neste universo, é inapropriada 
	int psurvival; // a taxa de sobrevivência dos feromônios 
	std::mutex mapMutex; // mutex para acesso a atributos compartilhados  
	std::mutex ioMutex; 
	bool isInitialized; // se o mapa foi inicializado 
	long unsigned int currAnt; // ponteiro para a próxima formiga a interagir com o mapa 
	std::list<Ant*> * allAnts; // lista com as formigas 
	int ufood; // taxa de atualização das comidas 
public: 
	bool fight; // se deve haver lutas entre formigas 
```

+ Métodos 

```cpp 
Map(int mapWidth, int mapHeight, int fov);  
Map(); // constructores 
Tile * getTile(int x, int y); // computa um tile (da lista `tiles`) com as 
		// suas coordenadas, `x` e `y` 
std::vector<Tile*> neighbors(int x, int y); // computa, 
		// no tabuleiro, os tiles vizinhos ao tile com coordenadas `x` e `y` 
void insertAnthill(int x, int y, std::string anthillName, 
		int nAnts); // insere (e inicializa) um formigueiro nas coordenadas `x` e `y` 
void insertFood(int x, int y, int initialVolume); // insere um volume igual a `initialVolume` 
		// com alimentos nas coordenadas `x` e `y` 
Anthill * getAnthill(std::string anthillName); // captura o formigueiro com nome `anthillName` 
Food * getFood(int x, int y); // identifica o alimento em (x, y); computa o ponteiro 
std::vector<Food*> getFoods(); // identifica as comidas no mapa 
void print(); // imprime o mapa no terminal 
Ant * getAntInTile(int x, int y, std::string colony); // identifica 
		// alguma formiga no tile (x, y); computa, também, um ponteiro 
Ant * getAnyAnt(int x, int y); // captura uma formiga de qualquer colônia 

void updateTiles(long unsigned int lTilesIndex, 
	long unsigned int rTilesIndex); // atualiza as coordenadas em um programa multithread 
void initializeAnts(); // Instancia as formigas nos formigueiros 
Ant * computeNextAnt(); // Computa a próxima formiga a jogar 
bool allAntsPlayed(); // Verifica se todas as formigas interagiram com o mapa nesta iteração 
void prepareNextIter(int nThreads); // Prepara o mapa para a próxima iteração da simulação 

Tile * captureFoodNear(int x, int y); // Captura alimentos próximos às coordenadas (x, y), 
		// utilizando o campo de visão `fov` 
int getPSurvival(); // Computa a taxa de sobrevivência do feromônio 
void reinitializeAnts(); // Atualiza a quantidade de formigas vivas 
void printAnthillsChars(); // Imprime as características de formigueiro no mapa 
```

Em seguida, instanciamos um objeto global `map`, que, no início do programa, será inicializado com os atributos apropriados. 


