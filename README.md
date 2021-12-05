# cg-projeto3 - Special Chess

Atividade desenvolvida para a disciplina de computação Gráfica da UFABC - ministrada pelos docentes Bruno Augusto e Harlen Batagelo.


## O que foi feito

Para esta atividade foi proposto o desenvolvimento de uma aplicação gráfica interativa 3D. Ao invés de dar continuidade

ao segundo projeto (aplicação gráfica 3D), foi decidido começar uma nova aplicação, dessa vez não focada em movimentos

rápidos, ou mudanças de imagens. Foi decidido uma aplicação mais sútil em que pudessem ser visualizadas operações e 

transformações de matrizes, objects e arrays conforme visto em aula em um jogo bem tradicional, xadrez.


## Renderização

Através da biblioteca ABCg utilizada nesse projeto, a renderização dos objetos fica mais simples que chamadas puramente 

escritas utilizando a biblioteca OpenGL. Cada objeto 3D foi encontrado no site Open3DModel (https://open3dmodel.com), 

com exceção do objeto que representa a tela de fundo, que foi encontrado no The Models Resources (https://www.models-resource.com).

Todos objetos utilizados podem ser encontrados na tabela de referências [1] ao final do arquivo, assim como seus 
respectivos links.


### Renderização do fundo e do tabuleiro

Essa parte consiste em renderizações básicas de cada objeto desejado, em que o tabuleiro acaba sendo nosso objeto 

principal (que terá o foco do programa) e o tubo, será um fundo dinâmico, com o aparente movimento de sempre estar 

indo para frente no tubo enquanto joga a partida. (Decidi não utilizar o skybox ou gamecube pois não achei uma boa 

ideia converter uma imagem inteira em 6 partes e depois movimentá-la, invés de apenas limitar a movimentação da câmera).


### Renderização das peças e do seletor de movimentos

Cada peça inicialmente possuiria seu próprio EBO e VAO, mas para poupar recursos foi decidido utilizar uma "espelhagem"

entre as peças. Como torre, cavalo e bispo são exatamente as mesmas peças para cada lado (do rei e da rainha), 

essas peças recebem os mesmos VAO e EBO (tammbém as mesmas matrizes de modelo), assim é possível renderizar as peças 

de maneira mais eficiente.

O mesmo seria feito para os peões, repetindo o mesmo objeto 8 vezes para cada jogador. Porém os peões possuem a 

propriedade de promoção. Uma vez que um peão chega ao fim do tabuleiro, este pode ser promovido para qualquer outra 

peça do tabuleiro (exceto o rei). Então para previnir que ao promover um peão, todos sejam aparentemente promovidos, 

foi melhor atribuir separadamente cada atributo dos peões.

O seletor de movimentos constitui basicamente da mesma estrutura para a renderização das peças, mas chama outro 

objeto renomeado de Old Rug (literalmente, tapete antigo) para Tile (azulejo/ladrilho). Assim que a partida é iniciada 

este objeto é renderizado na Torre esquerda do primeiro jogador (aquele que controla as peças brancas). 

Ao decorrer do jogo é possível notar que tile permanece na mesma posição da última peça selecionada, invés de ter um 

separado para cada jogador. Isso é proposital, para que cada um possa ver onde estava a última peça movimentada 

e onde ela se encontra atualmente (o último movimento realizado).

Outra observação importante é que este seletor é renderizado várias vezes, para mostrar os movimentos possíveis da peça 

selecionada.


## Captura das peças
    
Diferentemente do projeto Asteroids, Flappy Bird e Sonic Test a captura de peças não foi feita através de colisões 

atualizadas a cada frame da aplicação, mas sim uma técnica de sobreposição quando uma peça é movida para uma posição 

atualmente ocupada por outra peça. Essa verificação ocorre apenas quando o movimento é feito, e não a cada frame, 

poupando recursos e auxiliando no processo de renderização rápida do tabuleiro.


## Diferenças do xadrez convencional

O jogo de xadrez tradicional possui vários movimentos especiais e situações que definem o estado do jogo (xeque, 

xequemate e afogamento). Todos os movimentos básicos de cada peça foi implementado com sucesso. Foi possível também 

implementar o movimento Roque (troca entre rei e torre de posição), um dos movimentos especiais do jogo.

O que ficaria faltando em compensação ao xadrez tradicional seria:
1. O movimento En passant (um peão capturar outro que acabou de se movimentar duas casas, esperando fugir da captura);

2. Qualquer forma de validação de xeque, ou xequemate (por extensão, afogamentos também não são validados).

En passant não foi implementado pois algumas mudanças seriam necessárias na classe de movimentos e também alguns 

atributos adicionais necessários para a estrutura de peças para verificar se há a possibilidade de captura de um 

peão que acabou de se movimentar pelo outro.

As validações de xeque são possíveis de serem feitas com a implementação atual, mas do método que estava sendo testado 

acabava deixando qualquer movimento de peça extremamente lento (pois existem formas de xeque comum, xeque duplo ou maior, 

xeque descoberto, entre outros), então para a apresentação inicial do projeto foi pensado que o melhor a fazer é seguir 

as regras de alguns ensinamentos para principiantes (principalmente usado em partidas bullet - partidas de 1 minuto para 

cada jogador), que não existe xeque. O jogador que captura o rei primeiro vence, ou aquele que possui mais pontos em seu 

jogo (no caso do projeto, pontuação de peças não existe, então vence apenas quem capturar o rei primeiro).


## Trechos de código

Alguns trechos importantes para o funcionamento do programa são as funções de movimento e a struct que define as peças.

Apenas para facilitar o desenvolvimento, todo objeto renderizado acabou sendo criado dentro da struct Piece, pois 

facilitava muito o desenvolvimento continuo do programa, com as funções e métodos criados.

// Struct Pieces

    struct Piece {
        char type{};
        char color{};

        bool first_move{true};
        bool is_positioned{false};
        bool blocked{false};
        bool promoted{false};
        
        int castling_index{-1};
        int trianglesDraw{};
        int index{};

        float castling_x{};
        float xPos{0.58f};
        float yPos{0.58f};
        float zPos{0.18f};
        float shininess{};
        
        Model model{};
        
        glm::mat4 modelMatrix{1.0f};
        
        glm::vec3 position{xPos, yPos, zPos};

        glm::vec4 ka{0.0f};
        glm::vec4 kd{0.0f};
        glm::vec4 ks{0.0f};
    };

    inline bool compareFloat(float x, float y, float epsilon = 0.006f) {
        return (fabs(x - y) < epsilon) ? true : false;
    }

Importante: a struct é definida em um arquivo header, dessa forma é acessível à diversas classes do programa que 

fazem uso dessa struct sem precisar repetir vários trechos de código. A função compareFloat definida aqui também é 

muito importante para definir a precisão que queremos comparar os valores ponto flutuante de precisão única em C/C++.

Como tentativa do projeto anterior de testar as colisões e primeiros testes nesse de substituir as peças inimigas em uma 

posição não estavam dando certo, foi explorada a possibilidade de definir um limite para essa "igualdade" 

(ou seja, arredondar/aproximar os valores), invés de verificar se os valores eram totalmente iguais. Essa solução 

funcionou bem para esse projeto.


## Opções de renderização

As opções habilitadas de renderização, do mesmo modo como vistas em aula, também foram uma opção proposital, para que 

quem esteja utilizando a aplicação possa ver a importância da texturização correta e como o projeto ficaria com outros 

parâmetros (shaders, tipos de mapeamento, cores diferentes, entre outros).
 

## Teclas e movimentos possíveis

[1] Tabela de mapeamento de teclas e movimentos com o mouse e funções na aplicação

| Tecla/Movimento | Função  |
| :-------------: | :--------------------------------------------------------------------------------------------  |
| Espaço          | Seleciona peça ou movimento válido, se pressionado em uma casa inválida, desmarca a peça atual |
| Seta Esquerda   | Move o seletor para a esquerda                                                                 |
| Seta Direita    | Move o seletor para a direita                                                                  |
| Seta para Cima  | Move o seletor para cima                                                                       |
| Seta para Baixo | Move o seletor para baixo                                                                      |
| A               | Olhar para a esquerda                                                                          |
| D               | Olhar para a direita                                                                           |
| Clique Esquerdo | Habilitar o tabuleiro para a rotação. Rotação feita movendo o mouse quando clique ativo        |
| Scroll          | Amplia/diminui zoom                                                                            |


## Resultado Final

O resultado final foi uma aplicação que simula o jogo clássico (com as restrições descritas anteriormente), num visual 

dinâmico para criar a ilusão de que o jogo acontece em um ambiente que os jogadores estão sempre correndo (o fundo é de 

um ambiente do jogo Sonic Rush, chamado Special Stage).

<!-- ![sample4](images/zoom.png) -->


### Desenvolvido por

Thiago Silva das Mercês RA - 1120181014

### Referências

[2] Tabela de referências para os objetos utilizados

| Objeto          | Link de onde foi retirado  |
| :-------------: | :----------------------------------------------------------------------------- |
| Tubo (fundo)    | https://www.models-resource.com/ds_dsi/sonicrush/model/11674/                  |
| Tabuleiro       | https://open3dmodel.com/3d-models/chess-board-wooden-black-color_492541.html   |
| Seletor (tile)  | https://open3dmodel.com/3d-models/rectangular-old-rug_478728.html              |
| Torre           | https://www.models-resource.com/ds_dsi/sonicrush/model/11674/                  |
| Cavalo          | https://www.models-resource.com/ds_dsi/sonicrush/model/11674/                  |
| Bispo           | https://www.models-resource.com/ds_dsi/sonicrush/model/11674/                  |
| Dama (Rainha)   | https://www.models-resource.com/ds_dsi/sonicrush/model/11674/                  |
| Rei             | https://www.models-resource.com/ds_dsi/sonicrush/model/11674/                  |
| Peão            | https://www.models-resource.com/ds_dsi/sonicrush/model/11674/                  |