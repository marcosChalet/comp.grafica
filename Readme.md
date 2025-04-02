# Paint

Projeto 1 da cadeira de Computação Gráfica - Universidade Federal do Cariri

## Atalhos do Teclado

| Tecla        | Ação                     |
| ------------ | ------------------------ |
| `p`          | Criar Ponto              |
| `l`          | Criar Linha              |
| `g`          | Criar Polígono           |
| `d`          | Deletar Objeto           |
| `r`          | Rotacionar               |
| `s`          | Selecionar               |
| `t`          | Transladar               |
| `1`          | Aumentar Escala em X e Y |
| `2`          | Aumentar Escala em X     |
| `3`          | Aumentar Escala em Y     |
| `4`          | Diminuir Escala em X e Y |
| `5`          | Diminuir Escala em X     |
| `6`          | Diminuir Escala em Y     |
| `up/down`    | Refletir em X            |
| `right/left` | Refletir em Y            |
| `c`          | Cisalhar (Shear)         |
| `F8`         | Salvar Dados             |
| `F9`         | Carregar Dados           |
| `Esc`        | Modo de Visualização     |

## Informações Úteis

1. Ao iniciar, o programa estará no modo de visualização, onde nenhuma alteração pode ser feita na tela. Para começar a criar objetos, pressione p, l ou g para entrar, respectivamente, nos modos de criação de ponto, linha ou polígono.

2. Quando estiver em um modo de criação, você pode alternar entre ele e o modo de visualização pressionando a tecla correspondente novamente. Por exemplo, se estiver no modo de criação de linhas (l) e pressionar l novamente, retornará ao modo de visualização. Você também pode sair para o modo de visualização pressionando Esc.

3. Caso alterne para outro modo antes de concluir um objeto, o progresso será descartado se os requisitos mínimos não forem atendidos. Por exemplo, uma linha só é criada se possuir dois pontos, e um polígono precisa de pelo menos três pontos.

## Como Rodar o Projeto

Abra um terminal na pasta do projeto. Com o **make** e as dependências do **OpenGL** instaladas rode os seguintes passos:

1. compile o projeto

   ```c
   $ make
   ```

2. Rode o executável gerado

   ```c
   $ make run
   ```

\* Caso Queira limpar os arquivos binários utilize o seguinte comando:

```c
    $ make clear
```
