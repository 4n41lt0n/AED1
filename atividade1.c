#include "raylib.h"
#include <stdio.h>
#include <stdlib.h>

#define LARGURA_JANELA 800
#define ALTURA_JANELA 600
#define TAM_CELULA 40
#define QUANTIDADE_INICIAL_BOLAS 12
#define QUANTIDADE_MINIMA_BOLAS 1

typedef struct {
    Vector2 pos;
    Vector2 vel;
    float raio;
    Color cor;
} Bola;

int **criarMatriz(int linhas, int colunas)
{
    int **matriz;

    /* Aloca o vetor de ponteiros para as linhas */
    matriz = (int **)malloc(linhas * sizeof(int *));

    if (matriz == NULL)
    {
        printf("Erro ao alocar a matriz!\n");
        exit(1);
    }

    /* Aloca cada linha da matriz */
    for (int i = 0; i < linhas; i++)
    {
        matriz[i] = (int *)malloc(colunas * sizeof(int));

        if (matriz[i] == NULL)
        {
            printf("Erro ao alocar linha da matriz!\n");
            exit(1);
        }

        /* Inicializa todas as celulas com 0 */
        for (int j = 0; j < colunas; j++)
        {
            matriz[i][j] = 0;
        }
    }

    return matriz;
}


/* função pra liberar a matriz */

void liberarMatriz(int **matriz, int linhas)
{
    if (matriz == NULL)
        return;

    /* Libera cada linha */
    for (int i = 0; i < linhas; i++)
    {
        free(matriz[i]);
    }

    /* Libera o vetor de ponteiros */
    free(matriz);
}


/*iniciando as bolas*/

void inicializarBola(Bola *b)
{
    /* Posicao inicial aleatoria */
    b->pos = (Vector2)
    {
        GetRandomValue(30, LARGURA_JANELA - 30),
        GetRandomValue(30, ALTURA_JANELA - 30)
    };

    /* Velocidade aleatoria */
    b->vel = (Vector2)
    {
        (float)GetRandomValue(-4, 4),
        (float)GetRandomValue(-4, 4)
    };

    /* Evita velocidade X igual a zero */
    if (b->vel.x == 0)
    {
        b->vel.x = 2;
    }

    /* Evita velocidade Y igual a zero */
    if (b->vel.y == 0)
    {
        b->vel.y = 2;
    }

    /* Raio da bola */
    b->raio = (float)GetRandomValue(10, 20);

    /* Cor aleatoria */
    b->cor = (Color)
    {
        GetRandomValue(50, 255),
        GetRandomValue(50, 255),
        GetRandomValue(50, 255),
        255
    };
}


/*Criar VETOR dinâmico  */

Bola *criarBolas(int quantidade)
{
    Bola *bolas;

    bolas = (Bola *)malloc(
        quantidade * sizeof(Bola)
    );

    if (bolas == NULL)
    {
        printf("Erro ao alocar as bolas!\n");
        exit(1);
    }

    for (int i = 0; i < quantidade; i++)
    {
        /*
           bolas + i equivale a &bolas[i]
        */

        Bola *b = bolas + i;

        inicializarBola(b);
    }

    return bolas;
}

void atualizarBola(Bola *b)
{
    /* Atualiza a posicao */

    b->pos.x += b->vel.x;
    b->pos.y += b->vel.y;


    /* Colisao com parede esquerda e direita */

    if (b->pos.x - b->raio < 0)
    {
        b->pos.x = b->raio;
        b->vel.x *= -1;
    }

    if (b->pos.x + b->raio > LARGURA_JANELA)
    {
        b->pos.x = LARGURA_JANELA - b->raio;
        b->vel.x *= -1;
    }


    /* Colisao com parede superior e inferior */

    if (b->pos.y - b->raio < 0)
    {
        b->pos.y = b->raio;
        b->vel.y *= -1;
    }

    if (b->pos.y + b->raio > ALTURA_JANELA)
    {
        b->pos.y = ALTURA_JANELA - b->raio;
        b->vel.y *= -1;
    }
}


/* EXERCICIO 2 */

void marcarCelulaVisitada(
    Bola *b,
    int **grade,
    int linhas,
    int colunas
)
{
    /*
       Converte a posicao da bola em pixels
       para linha e coluna da matriz
    */

    int coluna = (int)(b->pos.x / TAM_CELULA);

    int linha = (int)(b->pos.y / TAM_CELULA);


    /*Verifica se os indices estao dentro dos limites da matriz*/

    if (
        linha >= 0 &&
        linha < linhas &&
        coluna >= 0 &&
        coluna < colunas
    )
    {
        /* Marca a celula como visitada */

        grade[linha][coluna] = 1;
    }
}


int contarCelulasVisitadas(
    int **grade,
    int linhas,
    int colunas
)
{
    int contador = 0;

    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            if (grade[i][j] == 1)
            {
                contador++;
            }
        }
    }

    return contador;
}

void desenharMatriz(
    int **grade,
    int linhas,
    int colunas
)
{
    for (int i = 0; i < linhas; i++)
    {
        for (int j = 0; j < colunas; j++)
        {
            Color corCelula;

            /*
               0 = nao visitada
               1 = visitada
            */

            if (grade[i][j] == 0)
            {
                corCelula = LIGHTGRAY;
            }
            else
            {
                corCelula = YELLOW;
            }


            /* Desenha a celula */

            DrawRectangle(
                j * TAM_CELULA,
                i * TAM_CELULA,
                TAM_CELULA,
                TAM_CELULA,
                corCelula
            );


            /* Desenha a borda da celula */

            DrawRectangleLines(
                j * TAM_CELULA,
                i * TAM_CELULA,
                TAM_CELULA,
                TAM_CELULA,
                GRAY
            );
        }
    }
}


int main(void)
{
    /* Quantidade de linhas e colunas */

    int linhas = ALTURA_JANELA / TAM_CELULA;

    int colunas = LARGURA_JANELA / TAM_CELULA;


    /* Cria a matriz dinamica */

    int **grade = criarMatriz(
        linhas,
        colunas
    );


    /* Quantidade inicial de bolas */

    int quantidadeBolas =
        QUANTIDADE_INICIAL_BOLAS;


    /* Cria o vetor dinamico de bolas */

    Bola *bolas = criarBolas(
        quantidadeBolas
    );


    /* Inicializa a janela */

    InitWindow(
        LARGURA_JANELA,
        ALTURA_JANELA,
        "Alocacao Dinamica - Bolas e Mapa de Calor"
    );


    SetTargetFPS(60);
    
    while (!WindowShouldClose())
    {

        /*
           EXERCICIO 1
           ADICIONAR UMA NOVA BOLA
           TECLA ESPACO */

        if (IsKeyPressed(KEY_SPACE))
        {
            /*
               Primeiro aumenta a quantidade
            */

            int novaQuantidade =
                quantidadeBolas + 1;


            /*
               realloc redimensiona o bloco
               de memoria das bolas
            */

            Bola *temp = (Bola *)realloc(
                bolas,
                novaQuantidade * sizeof(Bola)
            );


            /*
               Verifica se realloc conseguiu
               alocar a memoria
            */

            if (temp != NULL)
            {
                /*Atualiza o ponteiro original */

                bolas = temp;


                /* Atualiza a quantidade */

                quantidadeBolas =
                    novaQuantidade;


                /*Inicializa a nova bola A ultima posicao e: quantidadeBolas - 1 */

                inicializarBola(
                    &bolas[
                        quantidadeBolas - 1
                    ]
                );
            }
            else
            {
                printf(
                    "Erro ao adicionar nova bola!\n"
                );
            }
        }


        /* EXERCICIO 1
           REMOVER A ULTIMA BOLA
           TECLA BACKSPACE */

        if (IsKeyPressed(KEY_BACKSPACE))
        {
            /*
               Verifica se existe mais que
               a quantidade minima
            */

            if (
                quantidadeBolas >
                QUANTIDADE_MINIMA_BOLAS
            )
            {
                int novaQuantidade =
                    quantidadeBolas - 1;


                /*
                   Reduz o tamanho do vetor
                */

                Bola *temp = (Bola *)realloc(
                    bolas,
                    novaQuantidade * sizeof(Bola)
                );


                if (temp != NULL)
                {
                    bolas = temp;

                    quantidadeBolas =
                        novaQuantidade;
                }
                else
                {
                    printf(
                        "Erro ao remover bola!\n"
                    );
                }
            }
        }


        /* =================================================
           ATUALIZAR TODAS AS BOLAS
           ================================================= */

        for (
            int i = 0;
            i < quantidadeBolas;
            i++
        )
        {
            /*
               Atualiza a bola

               bolas + i equivale a
               &bolas[i]
            */

            atualizarBola(
                bolas + i
            );


            /*
               Marca a celula visitada
               pela bola
            */

            marcarCelulaVisitada(
                bolas + i,
                grade,
                linhas,
                colunas
            );
        }

        int celulasVisitadas =
            contarCelulasVisitadas(
                grade,
                linhas,
                colunas
            );



        BeginDrawing();


        ClearBackground(RAYWHITE);


        /* Desenha a matriz */

        desenharMatriz(
            grade,
            linhas,
            colunas
        );


        /* =================================================
           DESENHAR TODAS AS BOLAS
           ================================================= */

        for (
            int i = 0;
            i < quantidadeBolas;
            i++
        )
        {
            DrawCircleV(
                bolas[i].pos,
                bolas[i].raio,
                bolas[i].cor
            );
        }


        /* =================================================
           MOSTRAR INFORMACOES
           ================================================= */

        DrawRectangle(
            10,
            10,
            300,
            100,
            Fade(WHITE, 0.85f)
        );


        DrawText(
            TextFormat(
                "Quantidade de bolas: %d",
                quantidadeBolas
            ),
            20,
            20,
            20,
            BLACK
        );


        DrawText(
            TextFormat(
                "Celulas visitadas: %d",
                celulasVisitadas
            ),
            20,
            50,
            20,
            BLACK
        );


        DrawText(
            "ESPACO: adicionar bola",
            20,
            80,
            15,
            DARKGRAY
        );


        DrawText(
            "BACKSPACE: remover bola",
            20,
            100,
            15,
            DARKGRAY
        );


        EndDrawing();
    }
    /*Libera o vetor dinamico de bola */

    free(bolas);


    /*Libera a matriz dinamico*/

    liberarMatriz(
        grade,
        linhas
    );


    /*
       Fecha a janela
    */

    CloseWindow();


    return 0;
}
