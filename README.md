# Simulação de um sistema emergente 

*Remark.* Todas as implementações foram testadas no sistema operacional Ubuntu 20.04; utilizamos, também, `clang++-10` como compilador. Neste sentido, é plausível que haja falhas em outras circunstâncias. 
  
Implementamos um sistema que consolida o comportamento de uma colônia de formigas; seu objetivo é capturar comidas e as levar aos formigueiros. Com o objetivo de a executar, digite 

```
make 
``` 

no terminal para compilar os arquivos. Em seguida, execute 

``` 
./main 
```

para que o programa seja iniciado com os parâmetros padrões. Contudo, existem 11 atributos modificáveis; eles estão descritos na tabela seguinte. 

| Comando | Descrição | Exemplo | 
| ------- | --------- | ------- | 
| `--nthreads` | A quantidade de threads no programa. | `--nthreads 3` | 
| `--width` | A largura do mapa. | `--width 5` | 
| `--height` | A altura do mapa. | `--height 4` | 
| `--iters` | A quantidade de iterações na simulação (isto é, de estágios em que as formigas executam algum movimento). | `--iters 99` |  
| `--psurvival` | A taxa de sobrevivência dos feromônios, medida em quantidade de iterações. | `--psurvival 9` | 
| `--fov` | A medida do campo de visão da formiga. | `--fov 2` | 
| `--colonies` | As coordenadas e as quantidades iniciais de formigas no mapa. | `--colonies 1,3,32` (coordenadas (1, 3) com 32 formigas) | 
| `--foods` | As coordenadas e os volumes iniciais de alimentos no mapa | `--fodos 2,4,45` (coordenadas (2, 4) com 45 unidades de volume) | 
| `--fight` | Se as formigas devem, ou não, lutar. | `--fight 1` (elas devem) | 
| ` --ufood` | A taxa de reposição (*update*) de alimentos no mapa. | `--ufood 3` | 
| `--cfood` | A quantidade de formigas que podem consumir, simultaneamente, as comidas. | `--cfood 2` | 
