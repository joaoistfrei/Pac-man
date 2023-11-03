#include <stdio.h>
#include <math.h>
#include <conio.h>
#include <time.h>
#include <unistd.h>
#include <windows.h>
#define TAMY 31 //numero de linhas do mapa
#define TAMX 28 //numero de colunas do mapa
#define RANDMAX 100 //definir um maximo pra funcao rand funcionar
#define SLEEPTIME 120

int y, x, pontos = 0;
int movinky, movpinky, movclyde, movblinky;
char movp;
int passos = 0, tempoder = 0;
int estado = 0; // 1 = PACMAN com powerup;
int contaballs = 0;
int ganhou = 0; // 1 - ganhou!
int wave = 0; //numero de waves
int vidas = 3;
char mapa[28][32];
char opc, omenu = 'm';
char again = 's';


void gotoxy(int a, int b){
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE),(COORD){a, b});
}

//exibe as regras caso o jogador aperte o botao
void mais_regras() {
    printf("\033[1;31mREGRAS:\n\n");
    printf("\033[34m- \033[1;33mO Pacman e controlado por voce! Pressione w, a, s ou d para mover o personagem;\n");
    printf("\033[34m- \033[1;33mVoce comeca o jogo no nivel 0 e a cada vez que todos os doces sao comidos, voce avanca na sua jornada;\n");
    printf("\033[34m- \033[1;33mO jogo acaba quando voce alcanca o nivel 10. Ate la, tente juntar o maximo de pontos que conseguir!\n");
    printf("\033[34m- \033[1;33mCada doce pequeno vale 10 pontos;\n");
    printf("\033[34m- \033[1;33mCuidado com os fantasmas! Eles rodam o mapa te procurando e, caso te alcancem, voce perde uma de suas 3 vidas;\n");
    printf("\033[34m- \033[1;33mQuando suas 3 vidas se esgotam, o jogo acaba...\n");
    printf("\033[34m- \033[1;33mOs fantasmas comecam o jogo na toca deles e sao liberados aos poucos;\n");
    printf("\033[34m- \033[1;33mOs fantasmas se movem de forma imprevisivel! Mantenha distancia dos 4 para se manter vivo!\n");
    printf("\033[34m- \033[1;33mEspalhados pelo mapa, existem 6 SUPER-DOCES, que voce pode comer para ganhar 100 pontos;\n");
    printf("\033[34m- \033[1;33mAlem de te dar pontos, os SUPER-DOCES te tornam invencivel por 15 movimentos!\n");
    printf("\033[34m- \033[1;33mCaso encontre algum fantasma enquanto invencivel, voce os devora e manda de volta para a toca deles! HUAHUAHUAHUA\n");
    printf("\033[34m- \033[1;33mCada fantasma devorado lhe garante 50 pontos;\n");
    printf("\033[34m- \033[1;33mExistem aberturas no mapa e, caso passe por elas, sera teleportado para o lado oposto;\n");
    printf("\033[34m- \033[1;33mO Pacman tem muita fome, entao, coma muitos doces! Se nao ele pode acabar morrendo...\n\n");
    printf("\033[1;33mAgora, se entendeu tudo, pressione 'm' para voltar ao menu ou qualquer outro botao para voltar comecar a jogar! \nE boa sorte!\033[0m");
    omenu = getch();
    system("cls");

}

//exibe o podio caso o jogador aperte o botao
void mais_podio() {
    printf("\033[1;33mEsse jogo possui um podio! E, por isso, lembramos pra sempre dos 5 melhores desempenhos!\n");
    printf("Essa e a tabela de maiores pontuacoes:\n");
    char nomes[5][4];
    int scores[5];

    FILE* ppt;
    ppt = fopen("HIGHSCORES.txt", "r+");
    if (ppt == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        printf("Saindo...");
        return;
    }

    for (int i = 0; i < 5; i++) {
        fscanf(ppt, "%s", nomes[i]);
        fscanf(ppt, " ");
        fscanf(ppt, "%d", &scores[i]);
        fscanf(ppt, "\n");
    }

        for (int i = 0; i < 5; i++) {
            fprintf(ppt, "%s %d\n", nomes[i], scores[i]);
        }
            printf("\n\033[1;31mHIGHSCORES: \033[0m\n");
        for(int i = 0; i < 5; i++) {
            printf("\033[1;34m%d. \033[1;33m%s - \033[1;37m%d\033[0m\n", i+1, nomes[i], scores[i]);
        }

    fclose(ppt);

    printf("\n\033[1;33mPressione qualquer botao para continuar...");
    opc = getch();
    system("cls");
    printf("Agora, se quiser marcar seu nome nessa tabela, precisara superar esses jogadores que voce viu!\n");
    opc = getch();
    system("cls");
    printf("No entanto, sera uma tarefa bem dificil...\n");
    opc = getch();
    system("cls");
    printf("Mas... Claro que nao estou duvidando de voce!");
    opc = getch();
    system("cls");
    printf("Tenho certeza de que voce e capaz de marcar muitos pontos!\n");
    printf("Mas sera que consegue destronar algum de nossos campeoes?");
    opc = getch();
    system("cls");
    printf("Bom, ja estou falando demais!\n");
    opc = getch();
    system("cls");
    printf("Entao chega de papo...");
    printf("Pressione 'm' para voltar ao menu, ou qualquer outra tecla para comecar a jogar!");
    omenu = getch();
    system("cls");
    if(omenu != 'm') {
        printf("E lembre-se, os fantasmas sempre vao achar um jeito de chegar ate voce!");
        opc = getch();
        system("cls");
    }

    system("cls");
}

//salva o mapa atual em um arquivo para ficar guardado
void salvarmapa() {

    FILE *sm;
    sm = fopen("LAST MAP.txt", "w");
    for(int i = 0; i < 31; i++) {
        for(int j = 0; j < 28; j++) {
            fprintf(sm, "%c ", mapa[j][i]);
        }
        fprintf(sm, "\n");
    }
    fclose(sm);
}

//encerra o jogo, analisando se houve substituicao no podio ou nao
void termino(int pts) {
    char nomes[5][4];
    int scores[5];
    char autor[4];

    FILE* ppt;
    ppt = fopen("HIGHSCORES.txt", "r+");


    if (ppt == NULL) {
        printf("Erro ao abrir o arquivo!\n");
        printf("Saindo...");
        return;
    }

    //dps disso
    
    for (int i = 0; i < 5; i++) {
        fscanf(ppt, "%s", nomes[i]);
        fscanf(ppt, " ");
        fscanf(ppt, "%d", &scores[i]);
        fscanf(ppt, "\n");
    }
    //antes disso
    int posicaoInserir = -1;
    for (int i = 0; i < 5; i++) {
        if (pts > scores[i]) {
            posicaoInserir = i;
            break;
        }
    }
    if (posicaoInserir != -1) {
        
        printf("\033[1;33mParabens, voce entrou no podio!\n");
        printf("Digite 3 letras de seu nome: ");
        fflush(stdin);
        fgets(autor, 4, stdin);
        autor[strcspn(autor, "\n")] = '\0';

        // Movendo as pontuações abaixo
        for (int i = 4; i > posicaoInserir; i--) {
            strcpy(nomes[i], nomes[i - 1]);
            scores[i] = scores[i - 1];
        }

        strcpy(nomes[posicaoInserir], autor);
        scores[posicaoInserir] = pts;

        fseek(ppt, 0, SEEK_SET); // Volta ao início do arquivo

        for (int i = 0; i < 5; i++) {
            fprintf(ppt, "%s %d\n", nomes[i], scores[i]);
        }
            printf("\n\033[1;31mHIGHSCORES: \033[0m\n");
        for(int i = 0; i < 5; i++) {
            printf("\033[1;34m%d. \033[1;37m%s - %d\033[0m\n", i+1, nomes[i], scores[i]);
        }
    } else {
        printf("\033[1;33mPontuacao insuficiente para entrar no podio.\033[0m\n");
    }

    fclose(ppt);
}

//entra no arquivo original de mapa e deposita na matriz mapa[j][i]
void iniciamapa() {
    FILE *fp;

    fp = fopen("MAPA OFICIAL PACMAN TARBALHO.txt", "r");

    if(fp == NULL) 
    {
        printf("Erro ao abrir o arquivo...");
    }

    for(int l = 0; l < 31; l++) 
    {
        for(int c = 0; c < 28; c++) 
        {
            fscanf(fp, "%c", &mapa[c][l]);
            fgetc(fp);
        }
    
    }

    fclose(fp);

}

//struct p fantasma
struct fantasma {
    int y;
    int yi;
    int x; 
    int xi;
    int solto;
    int volta;
}inky, pinky, clyde, blinky; //criar uma variavel pra unir a posicao y, x dos bonecos, guardando a posicao inicial caso morram 

//struct p pacman (principau escrito errado de proposito rs)
struct principau {
    int y;
    int yi;
    int x; 
    int xi;
} pacman;

//funcao pra ver se os comandos de movimentacao p pacman sao validos
void movimentaele(char comando) {
    switch(comando)  {
                case 'w':
                    if(mapa[pacman.x][pacman.y-1] != '#') {
                    pacman.y--;
                    passos++;
                    }
                    break;
                case 'a':
                    if(mapa[pacman.x-1][pacman.y] != '#') {
                    pacman.x--;
                    passos++;
                    }
                    break;
                case 's':
                    if(mapa[pacman.x][pacman.y+1] != '#') {
                    pacman.y++;
                    passos++;
                    }
                    break;
                case 'd':
                    if(mapa[pacman.x+1][pacman.y] != '#') {
                    pacman.x++;
                    passos++;
                    }
                    break;
                default:
                    break;
                }
}



int main() {

    //printf("\033[?251");

    iniciamapa();

    srand(time(NULL));


    while(omenu == 'm') {

        system("cls");
        printf("\033[1;31mBem vindo ao PACMAN!\n");
        printf("\033[1;33mPressione:\n");
        printf("\033[34m- \033[1;33m'r' para ler as regras;\n");
        printf("\033[34m- \033[1;33m't' para ver a tabela de maiores pontuacoes;\n");
        printf("ou qualquer outra tecla para ir direto ao jogo!\033[0m\n");
        opc = getch();
        printf("\033[?25l");
        system("cls");
        switch(opc) { 

            case 'r':
                mais_regras();
                break;
            case 't':
                mais_podio();
                break;
            default:
                omenu = opc;
                break;

        }   
    }

{ //declarando posicao inicial do pacman e dos fantasmazzzz
    pacman.yi = pacman.y = 22; 
    pacman.xi = pacman.x = 13;

    inky.yi = inky.y = 14;
    inky.xi = inky.x = 12;
    inky.solto = 0;
    
    pinky.yi = pinky.y = 14;
    pinky.xi = pinky.x = 13;
    pinky.solto = 0;

    blinky.yi = blinky.y = 14;
    blinky.xi = blinky.x = 14;
    blinky.solto = 0;

    clyde.yi = clyde.y = 14;
    clyde.xi = clyde.x = 15;
    clyde.solto = 0;
}
        int caso;
        int quantos, quantosinky, quantospinky, quantosblinky, quantosclyde;
        while(again == 's') {
            
            while(vidas > 0)  { 
                
                //mini ia
                {
                
                //inky
                {
                        quantosinky = 0;
                        caso = 5;
                        if(mapa[inky.x+1][inky.y] != '#') {
                            quantosinky++;
                        }
                        if(mapa[inky.x-1][inky.y] != '#') {
                            quantosinky++;
                        }
                        if(mapa[inky.x][inky.y+1] != '#') {
                            quantosinky++;
                        }
                        if(mapa[inky.x][inky.y-1] != '#') {
                            quantosinky++;
                        }

                        //se tem 3 espacos livres, decidir entre um dos 3
                        if(quantosinky == 3 && mapa[inky.x][inky.y-1] == '#') {
                            movinky = (rand() % 3) + 1;
                        } else if(quantosinky == 3 && mapa[inky.x][inky.y+1] == '#') {
                                movinky = rand() % 3;
                                if(movinky == 2) {
                                    movinky = 3;
                                }
                            } else if(quantosinky == 3 && mapa[inky.x-1][inky.y] == '#') {
                                    movinky = rand() % 3;
                                    if(movinky == 1) {
                                        movinky = 3;
                                    }
                                } else if(quantosinky == 3 && mapa[inky.x+1][inky.y] == '#') {
                                        movinky = rand() % 3;
                                }
                                    
                        //se tiver em alguma quina, escolher uma das duas saidas
                        if(mapa[inky.x+1][inky.y] == '#' && mapa[inky.x][inky.y-1] == '#' && mapa[inky.x-1][inky.y] != '#' && mapa[inky.x][inky.y+1] != '#') {
                            caso = 0;
                            movinky = rand() % 2;
                            if(movinky == 0) {
                                movinky = 2;
                            }
                        } 
                        if(mapa[inky.x-1][inky.y] == '#' && mapa[inky.x][inky.y-1] == '#' && mapa[inky.x+1][inky.y] != '#' && mapa[inky.x][inky.y+1] != '#') {
                            caso = 1;
                            movinky = rand() % 2;
                            if(movinky == 0) {
                                movinky = 2;
                            }
                            if(movinky == 1) {
                                movinky == 3;
                            }
                        }
                        if(mapa[inky.x-1][inky.y] == '#' && mapa[inky.x][inky.y+1] == '#' && mapa[inky.x+1][inky.y] != '#' && mapa[inky.x][inky.y-1] != '#') {
                            caso = 2;
                            movinky = rand() % 2;
                            if(movinky == 1) {
                                movinky = 3;
                            }
                        }
                        if(mapa[inky.x+1][inky.y] == '#' && mapa[inky.x][inky.y+1] == '#' && mapa[inky.x-1][inky.y] != '#' && mapa[inky.x][inky.y-1] != '#') {
                            caso = 3;
                            movinky = rand() % 2;
                        }

                        //se tiver em uma encrusilhada de 4
                        if(quantosinky == 4) {
                            movinky = rand() % 4;
                        }


                    }

                //pinky
                {
                        quantospinky = 0;
                        caso = 5;
                        if(mapa[pinky.x+1][pinky.y] != '#') {
                            quantospinky++;
                        }
                        if(mapa[pinky.x-1][pinky.y] != '#') {
                            quantospinky++;
                        }
                        if(mapa[pinky.x][pinky.y+1] != '#') {
                            quantospinky++;
                        }
                        if(mapa[pinky.x][pinky.y-1] != '#') {
                            quantospinky++;
                        }

                        //se tem 3 espacos livres, decidir entre um dos 3
                        if(quantospinky == 3 && mapa[pinky.x][pinky.y-1] == '#') {
                            movpinky = (rand() % 3) + 1;
                        } else if(quantospinky == 3 && mapa[pinky.x][pinky.y+1] == '#') {
                                movpinky = rand() % 3;
                                if(movpinky == 2) {
                                    movpinky = 3;
                                }
                            } else if(quantospinky == 3 && mapa[pinky.x-1][pinky.y] == '#') {
                                    movpinky = rand() % 3;
                                    if(movpinky == 1) {
                                        movpinky = 3;
                                    }
                                } else if(quantospinky == 3 && mapa[pinky.x+1][pinky.y] == '#') {
                                        movpinky = rand() % 3;
                                }
                                    
                        //se tiver em alguma quina, escolher uma das duas saidas
                        if(mapa[pinky.x+1][pinky.y] == '#' && mapa[pinky.x][pinky.y-1] == '#' && mapa[pinky.x-1][pinky.y] != '#' && mapa[pinky.x][pinky.y+1] != '#') {
                            caso = 0;
                            movpinky = rand() % 2;
                            if(movpinky == 0) {
                                movpinky = 2;
                            }
                        } 
                        if(mapa[pinky.x-1][pinky.y] == '#' && mapa[pinky.x][pinky.y-1] == '#' && mapa[pinky.x+1][pinky.y] != '#' && mapa[pinky.x][pinky.y+1] != '#') {
                            caso = 1;
                            movpinky = rand() % 2;
                            if(movpinky == 0) {
                                movpinky = 2;
                            }
                            if(movpinky == 1) {
                                movpinky == 3;
                            }
                        }
                        if(mapa[pinky.x-1][pinky.y] == '#' && mapa[pinky.x][pinky.y+1] == '#' && mapa[pinky.x+1][pinky.y] != '#' && mapa[pinky.x][pinky.y-1] != '#') {
                            caso = 2;
                            movpinky = rand() % 2;
                            if(movpinky == 1) {
                                movpinky = 3;
                            }
                        }
                        if(mapa[pinky.x+1][pinky.y] == '#' && mapa[pinky.x][pinky.y+1] == '#' && mapa[pinky.x-1][pinky.y] != '#' && mapa[pinky.x][pinky.y-1] != '#') {
                            caso = 3;
                            movpinky = rand() % 2;
                        }

                        //se tiver em uma encrusilhada de 4
                        if(quantospinky == 4) {
                            movpinky = rand() % 4;
                        }


                    }


                //blinky
                {
                        quantosblinky = 0;
                        caso = 5;
                        if(mapa[blinky.x+1][blinky.y] != '#') {
                            quantosblinky++;
                        }
                        if(mapa[blinky.x-1][blinky.y] != '#') {
                            quantosblinky++;
                        }
                        if(mapa[blinky.x][blinky.y+1] != '#') {
                            quantosblinky++;
                        }
                        if(mapa[blinky.x][blinky.y-1] != '#') {
                            quantosblinky++;
                        }

                        //se tem 3 espacos livres, decidir entre um dos 3
                        if(quantosblinky == 3 && mapa[blinky.x][blinky.y-1] == '#') {
                            movblinky = (rand() % 3) + 1;
                        } else if(quantosblinky == 3 && mapa[blinky.x][blinky.y+1] == '#') {
                                movblinky = rand() % 3;
                                if(movblinky == 2) {
                                    movblinky = 3;
                                }
                            } else if(quantosblinky == 3 && mapa[blinky.x-1][blinky.y] == '#') {
                                    movblinky = rand() % 3;
                                    if(movblinky == 1) {
                                        movblinky = 3;
                                    }
                                } else if(quantosblinky == 3 && mapa[blinky.x+1][blinky.y] == '#') {
                                        movblinky = rand() % 3;
                                }
                                    
                        //se tiver em alguma quina, escolher uma das duas saidas
                        if(mapa[blinky.x+1][blinky.y] == '#' && mapa[blinky.x][blinky.y-1] == '#' && mapa[blinky.x-1][blinky.y] != '#' && mapa[blinky.x][blinky.y+1] != '#') {
                            caso = 0;
                            movblinky = rand() % 2;
                            if(movblinky == 0) {
                                movblinky = 2;
                            }
                        } 
                        if(mapa[blinky.x-1][blinky.y] == '#' && mapa[blinky.x][blinky.y-1] == '#' && mapa[blinky.x+1][blinky.y] != '#' && mapa[blinky.x][blinky.y+1] != '#') {
                            caso = 1;
                            movblinky = rand() % 2;
                            if(movblinky == 0) {
                                movblinky = 2;
                            }
                            if(movblinky == 1) {
                                movblinky == 3;
                            }
                        }
                        if(mapa[blinky.x-1][blinky.y] == '#' && mapa[blinky.x][blinky.y+1] == '#' && mapa[blinky.x+1][blinky.y] != '#' && mapa[blinky.x][blinky.y-1] != '#') {
                            caso = 2;
                            movblinky = rand() % 2;
                            if(movblinky == 1) {
                                movblinky = 3;
                            }
                        }
                        if(mapa[blinky.x+1][blinky.y] == '#' && mapa[blinky.x][blinky.y+1] == '#' && mapa[blinky.x-1][blinky.y] != '#' && mapa[blinky.x][blinky.y-1] != '#') {
                            caso = 3;
                            movblinky = rand() % 2;
                        }

                        //se tiver em uma encrusilhada de 4
                        if(quantosblinky == 4) {
                            movblinky = rand() % 4;
                        }


                    }


                //clyde
                {
                        quantosclyde = 0;
                        caso = 5;
                        if(mapa[clyde.x+1][clyde.y] != '#') {
                            quantosclyde++;
                        }
                        if(mapa[clyde.x-1][clyde.y] != '#') {
                            quantosclyde++;
                        }
                        if(mapa[clyde.x][clyde.y+1] != '#') {
                            quantosclyde++;
                        }
                        if(mapa[clyde.x][clyde.y-1] != '#') {
                            quantosclyde++;
                        }

                        //se tem 3 espacos livres, decidir entre um dos 3
                        if(quantosclyde == 3 && mapa[clyde.x][clyde.y-1] == '#') {
                            movclyde = (rand() % 3) + 1;
                        } else if(quantosclyde == 3 && mapa[clyde.x][clyde.y+1] == '#') {
                                movclyde = rand() % 3;
                                if(movclyde == 2) {
                                    movclyde = 3;
                                }
                            } else if(quantosclyde == 3 && mapa[clyde.x-1][clyde.y] == '#') {
                                    movclyde = rand() % 3;
                                    if(movclyde == 1) {
                                        movclyde = 3;
                                    }
                                } else if(quantosclyde == 3 && mapa[clyde.x+1][clyde.y] == '#') {
                                        movclyde = rand() % 3;
                                }
                                    
                        //se tiver em alguma quina, escolher uma das duas saidas
                        if(mapa[clyde.x+1][clyde.y] == '#' && mapa[clyde.x][clyde.y-1] == '#' && mapa[clyde.x-1][clyde.y] != '#' && mapa[clyde.x][clyde.y+1] != '#') {
                            caso = 0;
                            movclyde = rand() % 2;
                            if(movclyde == 0) {
                                movclyde = 2;
                            }
                        } 
                        if(mapa[clyde.x-1][clyde.y] == '#' && mapa[clyde.x][clyde.y-1] == '#' && mapa[clyde.x+1][clyde.y] != '#' && mapa[clyde.x][clyde.y+1] != '#') {
                            caso = 1;
                            movclyde = rand() % 2;
                            if(movclyde == 0) {
                                movclyde = 2;
                            }
                            if(movclyde == 1) {
                                movclyde == 3;
                            }
                        }
                        if(mapa[clyde.x-1][clyde.y] == '#' && mapa[clyde.x][clyde.y+1] == '#' && mapa[clyde.x+1][clyde.y] != '#' && mapa[clyde.x][clyde.y-1] != '#') {
                            caso = 2;
                            movclyde = rand() % 2;
                            if(movclyde == 1) {
                                movclyde = 3;
                            }
                        }
                        if(mapa[clyde.x+1][clyde.y] == '#' && mapa[clyde.x][clyde.y+1] == '#' && mapa[clyde.x-1][clyde.y] != '#' && mapa[clyde.x][clyde.y-1] != '#') {
                            caso = 3;
                            movclyde = rand() % 2;
                        }

                        //se tiver em uma encrusilhada de 4
                        if(quantosclyde == 4) {
                            movclyde = rand() % 4;
                        }


                    }

                }

                movimentaele(movp);


                //switches pra movimentar os fantasmas
                {
                switch(movinky)  {
                    case 0:
                        if(mapa[inky.x][inky.y-1] != '#') {
                        inky.y--;
                        }
                        break;
                    case 1:
                        if(mapa[inky.x-1][inky.y] != '#') {
                        inky.x--;
                        }
                        break;
                    case 2:
                        if(mapa[inky.x][inky.y+1] != '#') {
                        inky.y++;
                        }
                        break;
                    case 3:
                        if(mapa[inky.x+1][inky.y] != '#') {
                        inky.x++;
                        }
                        break;
                    default:
                        break;
                }

                switch(movpinky)  {
                    case 0:
                        if(mapa[pinky.x][pinky.y-1] != '#') {
                        pinky.y--;
                        }
                        break;
                    case 1:
                        if(mapa[pinky.x-1][pinky.y] != '#') {
                        pinky.x--;
                        }
                        break;
                    case 2:
                        if(mapa[pinky.x][pinky.y+1] != '#') {
                        pinky.y++;
                        }
                        break;
                    case 3:
                        if(mapa[pinky.x+1][pinky.y] != '#') {
                        pinky.x++;
                        }
                        break;
                    default:
                        break;
                    }    
                    
                switch(movclyde)  {
                    case 0:
                        if(mapa[clyde.x][clyde.y-1] != '#') {
                        clyde.y--;
                        }
                        break;
                    case 1:
                        if(mapa[clyde.x-1][clyde.y] != '#') {
                        clyde.x--;
                        }
                        break;
                    case 2:
                        if(mapa[clyde.x][clyde.y+1] != '#') {
                        clyde.y++;
                        }
                        break;
                    case 3:
                        if(mapa[clyde.x+1][clyde.y] != '#') {
                        clyde.x++;
                        }
                        break;
                    default:
                        break;
                    }
                    
                switch(movblinky)  {
                    case 0:
                        if(mapa[blinky.x][blinky.y-1] != '#') {
                        blinky.y--;
                        }
                        break;
                    case 1:
                        if(mapa[blinky.x-1][blinky.y] != '#') {
                        blinky.x--;
                        }
                        break;
                    case 2:
                        if(mapa[blinky.x][blinky.y+1] != '#') {
                        blinky.y++;
                        }
                        break;
                    case 3:
                        if(mapa[blinky.x+1][blinky.y] != '#') {
                        blinky.x++;
                        }
                        break;
                    default:
                        break;
                    }
                }
                    
                    //colisoes normais
                {
                    if(pacman.y == inky.y && pacman.x == inky.x && estado == 0) {
                        vidas--;
                        passos = 0;
                        inky.solto = 0;
                        pinky.solto = 0;
                        blinky.solto = 0;
                        clyde.solto = 0;
                        inky.y = inky.yi;
                        inky.x = inky.xi;
                        pinky.y = pinky.yi;
                        pinky.x = pinky.xi;
                        blinky.y = blinky.yi;
                        blinky.x = blinky.xi;
                        clyde.y = clyde.yi;
                        clyde.x = clyde.xi;
                        pacman.x = pacman.xi;
                        pacman.y = pacman.yi;

                    }
                    if(pacman.y == pinky.y && pacman.x == pinky.x && estado == 0) {
                        vidas--;
                        passos = 0;
                        inky.solto = 0;
                        pinky.solto = 0;
                        blinky.solto = 0;
                        clyde.solto = 0;
                        inky.y = inky.yi;
                        inky.x = inky.xi;
                        pinky.y = pinky.yi;
                        pinky.x = pinky.xi;
                        blinky.y = blinky.yi;
                        blinky.x = blinky.xi;
                        clyde.y = clyde.yi;
                        clyde.x = clyde.xi;
                        pacman.x = pacman.xi;
                        pacman.y = pacman.yi;

                    }
                    if(pacman.y == blinky.y && pacman.x == blinky.x && estado == 0) {
                        vidas--;
                        passos = 0;
                        inky.solto = 0;
                        pinky.solto = 0;
                        blinky.solto = 0;
                        clyde.solto = 0;
                        inky.y = inky.yi;
                        inky.x = inky.xi;
                        pinky.y = pinky.yi;
                        pinky.x = pinky.xi;
                        blinky.y = blinky.yi;
                        blinky.x = blinky.xi;
                        clyde.y = clyde.yi;
                        clyde.x = clyde.xi;
                        pacman.x = pacman.xi;
                        pacman.y = pacman.yi;

                    }
                    if(pacman.y == clyde.y && pacman.x == clyde.x && estado == 0) {
                        vidas--;
                        passos = 0;
                        inky.solto = 0;
                        pinky.solto = 0;
                        blinky.solto = 0;
                        clyde.solto = 0;
                        inky.y = inky.yi;
                        inky.x = inky.xi;
                        pinky.y = pinky.yi;
                        pinky.x = pinky.xi;
                        blinky.y = blinky.yi;
                        blinky.x = blinky.xi;
                        clyde.y = clyde.yi;
                        clyde.x = clyde.xi;
                        pacman.x = pacman.xi;
                        pacman.y = pacman.yi;
                    }
                }
                    //colisoes com poder
                    {
                    if(pacman.y == inky.y && pacman.x == inky.x && estado == 1) {
                        inky.solto = 0;
                        pontos += 50;
                        passos = 0;
                        inky.y = inky.yi;
                        inky.x = inky.xi;
                    }
                    if(pacman.y == pinky.y && pacman.x == pinky.x && estado == 1) {
                        pinky.solto = 0;
                        pontos += 50;
                        passos = 0;
                        pinky.y = pinky.yi;
                        pinky.x = pinky.xi;
                    }
                    if(pacman.y == blinky.y && pacman.x == blinky.x && estado == 1) {
                        blinky.solto = 0;
                        passos = 0;
                        pontos += 50;
                        blinky.y = blinky.yi;
                        blinky.x = blinky.xi;
                    }
                    if(pacman.y == clyde.y && pacman.x == clyde.x && estado == 1) {
                        clyde.solto = 0;
                        passos = 0;
                        pontos += 50;
                        clyde.y = clyde.yi;
                        clyde.x = clyde.xi;
                    }
                    }
                //teleporte
                {

                    if(pacman.y == 10 && pacman.x == -1) {
                        pacman.y = 10;
                        pacman.x = 27;
                    }
                    if(pacman.y == 10 && pacman.x == 28) {
                        pacman.y = 10;
                        pacman.x = 0;
                    }
                    if(pacman.y == 20 && pacman.x == -1) {
                        pacman.y = 20;
                        pacman.x = 27;
                    }
                    if(pacman.y == 20 && pacman.x == 28) {
                        pacman.y = 20;
                        pacman.x = 0;
                    }

                    if(inky.y == 10 && inky.x == -1) {
                        inky.y = 10;
                        inky.x = 27;
                    }
                    if(inky.y == 10 && inky.x == 28) {
                        inky.y = 10;
                        inky.x = 0;
                    }
                    if(inky.y == 20 && inky.x == -1) {
                        inky.y = 20;
                        inky.x = 27;
                    }
                    if(inky.y == 20 && inky.x == 28) {
                        inky.y = 20;
                        inky.x = 0;
                    }

                    if(pinky.y == 10 && pinky.x == -1) {
                        pinky.y = 10;
                        pinky.x = 27;
                    }
                    if(pinky.y == 10 && pinky.x == 28) {
                        pinky.y = 10;
                        pinky.x = 0;
                    }
                    if(pinky.y == 20 && pinky.x == -1) {
                        pinky.y = 20;
                        pinky.x = 27;
                    }
                    if(pinky.y == 20 && pinky.x == 28) {
                        pinky.y = 20;
                        pinky.x = 0;
                    }
                    
                    if(clyde.y == 10 && clyde.x == -1) {
                        clyde.y = 10;
                        clyde.x = 27;
                    }
                    if(clyde.y == 10 && clyde.x == 28) {
                        clyde.y = 10;
                        clyde.x = 0;
                    }
                    if(clyde.y == 20 && clyde.x == -1) {
                        clyde.y = 20;
                        clyde.x = 27;
                    }
                    if(clyde.y == 20 && clyde.x == 28) {
                        clyde.y = 20;
                        clyde.x = 0;
                    }
                    if(blinky.y == 10 && blinky.x == -1) {
                        blinky.y = 10;
                        blinky.x = 27;
                    }
                    if(blinky.y == 10 && blinky.x == 28) {
                        blinky.y = 10;
                        blinky.x = 0;
                    }
                    if(blinky.y == 20 && blinky.x == -1) {
                        blinky.y = 20;
                        blinky.x = 27;
                    }
                    if(blinky.y == 20 && blinky.x == 28) {
                        blinky.y = 20;
                        blinky.x = 0;
                    }

                    //pontuacao e power up
                    if(mapa[pacman.x][pacman.y] == '.') {
                        pontos += 10;
                        mapa[pacman.x][pacman.y] = ' ';
                        contaballs++;
                    } 
                    else if(mapa[pacman.x][pacman.y] == 'o') {
                        pontos += 100;
                        estado = 1;
                        contaballs++;
                        tempoder = 30;
                        mapa[pacman.x][pacman.y] = ' ';
                    }
                    
                }
                {
                    if(contaballs == 325) {
                        iniciamapa();
                        movp = 'o';
                        contaballs = 0;
                        passos = 0;
                        inky.solto = 0;
                        pinky.solto = 0;
                        blinky.solto = 0;
                        clyde.solto = 0;
                        inky.y = inky.yi;
                        inky.x = inky.xi;
                        pinky.y = pinky.yi;
                        pinky.x = pinky.xi;
                        blinky.y = blinky.yi;
                        blinky.x = blinky.xi;
                        clyde.y = clyde.yi;
                        clyde.x = clyde.xi;
                        pacman.x = pacman.xi;
                        pacman.y = pacman.yi;
                        wave++;
                    }
                } //se comer tudo reinicia mapa!!!
                
                //imprimir mapa no modo 0 ou 1
                    {
                    if(estado == 0) {
                        gotoxy(0, 0);
                        for(int i = 0; i < TAMY; i++) {
                            for(int j = 0; j < TAMX; j++) {
                                if(j == pacman.x && i == pacman.y)  {
                                    printf("\x1b[38;2;255;255;0mC \x1b[0m");
                                } 
                                else if(j == inky.x && i == inky.y) {
                                    printf("\x1b[38;5;117m& \x1b[0m");
                                }
                                else if(j == pinky.x && i == pinky.y) {
                                    printf("\x1b[38;5;207m& \x1b[0m");
                                }
                                else if(j == clyde.x && i == clyde.y) {
                                    printf("\x1b[38;5;202m& \x1b[0m");
                                }
                                else if(j == blinky.x && i == blinky.y) {
                                    printf("\x1b[31m& \x1b[0m");
                                }
                                else if((i == 11 && j == 13) || (i == 11 && j == 14) ) {
                                    printf("\033[1;33m= \x1b[0m");
                                }
                                else if(mapa[j][i] == '#') {
                                    printf("\x1b[34m# \x1b[0m");
                                }
                                else if(mapa[j][i] == '.') {
                                    printf("\x1b[38;2;245;245;220m. \x1b[0m");
                                }
                                else if(mapa[j][i] == 'o') {
                                    printf("\x1b[38;2;245;245;220mo \x1b[0m");
                                }
                                else {
                                    printf("  ");
                                }
                            }
                            printf("\n");
                        }
                       
                    }
                    else if(estado) {
                        gotoxy(0, 0);
                        for(int i = 0; i < TAMY; i++) {
                            for(int j = 0; j < TAMX; j++) {
                                if(j == pacman.x && i == pacman.y)  {
                                    printf("\x1b[38;2;255;255;0mC \x1b[0m");
                                } 
                                else if(j == inky.x && i == inky.y) {
                                    printf("\x1b[37m& \x1b[0m");
                                }
                                else if(j == pinky.x && i == pinky.y) {
                                    printf("\x1b[37m& \x1b[0m");
                                }
                                else if(j == clyde.x && i == clyde.y) {
                                    printf("\x1b[37m& \x1b[0m");
                                }
                                else if(j == blinky.x && i == blinky.y) {
                                    printf("\x1b[37m& \x1b[0m");
                                }
                                else if((i == 11 && j == 13) || (i == 11 && j == 14) ) {
                                    printf("\033[1;33m= \x1b[0m");
                                }
                                else if(mapa[j][i] == '#') {
                                    printf("\x1b[34m# \x1b[0m");
                                }

                                else if(mapa[j][i] == '.') {
                                    printf("\x1b[38;2;245;245;220m. \x1b[0m");
                                }
                                else if(mapa[j][i] == 'o') {
                                    printf("\x1b[38;2;245;245;220mo \x1b[0m");
                                }
                                else {
                                    printf("  ");
                                }
                            }
                            printf("\n");
                }
                
            }
                    
                    }
                    
                    //imprimir vidas pontos e wave
                    {
                    printf("\033[1;33mSCORE: %d\033[0m", pontos);

                    printf("\033[1;31m               VIDAS: %d\033[0m", vidas);

                    printf("\033[1;33m               NIVEL: %d\033[0m", wave);
                    }
                
                //soltar fantasmas com os passos
                {
                    if(passos ==  5 && inky.solto == 0) {
                        inky.solto = 1;
                        inky.y = 10;
                        inky.x = 13;
                        movinky = rand() % 2;
                        if(movinky == 0) {
                            movinky = 3;
                        }
                        
                    }
                    if(passos == 10 && pinky.solto == 0) {
                        pinky.solto = 1;
                        pinky.y = 10;
                        pinky.x = 13;
                        movpinky = rand() % 2;
                        if(movpinky == 0) {
                            movpinky = 3;
                        }

                    }
                    if(passos == 15 && blinky.solto == 0) {
                        blinky.solto = 1;
                        blinky.y = 10;
                        blinky.x = 14;
                        movblinky = rand() % 2;
                        if(movblinky == 0) {
                            movblinky = 3;
                        }

                    }
                    if(passos == 20 && clyde.solto == 0) {
                        clyde.solto = 1;
                        clyde.y = 10;
                        clyde.x = 14;
                        movclyde = rand() % 2;
                        if(movclyde == 0) {
                            movclyde = 3;
                        }

                    }

                }

                        //timer para o poder acabar
                        {
                        if(estado) {
                            tempoder--;
                        }
                        if(tempoder == 0) {
                            estado = 0;
                        }
                        }

                    //busca a tecla apertada, ou deixa o loop rodar a cada SLEEPTIME
                    if(kbhit() && movp != 'g') {
                        movp = getch();
                    }

                    Sleep(SLEEPTIME);

                    //atalho para acabar o jogo (g = ganha // t = sobe um nivel)
                    {
                    if(movp == 'g') {
                        ganhou = 1;
                        movp = 'o';
                        break;
                    }
                    
                    //atalho para subir um nivel
                    if(movp == 't') {
                        wave++;
                        movp = 'o';
                    }

                    if(wave == 10) {
                        ganhou = 1;
                        break;
                    }
                    }

                    //pause do jogo (p)
                    if(movp == 'p') {
                        system("cls");
                        printf("\033[1;33mJogo pausado!\033[0m\n");
                        printf("\033[1;33mPressione qualquer tecla para continuar...\033[0m\n");
                        int qqr;
                        qqr = getch();
                        movp = 'o';
                    }
                
                printf("\033[?25h");
            }
                
                //encerra o jogo e pergunta se quer jogar
                system("cls");
                printf("\033[1;33mGAME OVER!\033[0m\n");
                termino(pontos);
                printf("\nDeseja jogar novamente?\n");
                printf("'s' ou 'n'\n");
                again = getch();
                if(again == 's') {

                    contaballs = 0;
                    ganhou = 0;
                    wave = 0;
                    passos = 0;
                    pontos = 0;
                    vidas = 3;
                    inky.solto = 0;
                    pinky.solto = 0;
                    blinky.solto = 0;
                    clyde.solto = 0;
                    inky.y = inky.yi;
                    inky.x = inky.xi;
                    pinky.y = pinky.yi;
                    pinky.x = pinky.xi;
                    blinky.y = blinky.yi;
                    blinky.x = blinky.xi;
                    clyde.y = clyde.yi;
                    clyde.x = clyde.xi;
                    pacman.x = pacman.xi;
                    pacman.y = pacman.yi;
                    iniciamapa();
                }
        }

        salvarmapa();

    return 0;
}