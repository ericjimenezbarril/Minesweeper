// CREAMOS UN BUSCAMINAS
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<string.h>
#include<ctype.h>
#include <time.h>


/**
 * @brief Prints the matrix with visual representation for special cells.
 *
 * This function traverses and prints an integer matrix given its dimensions.
 * If an element is equal to 7, it prints '*', if it is equal to 99 it prints '||>',
 * otherwise, it prints the value of the element. Headers with indices for rows and
 * columns are included to facilitate visualization.
 *
 * @param m Pointer to the pointer of the integer matrix.
 * @param filas Number of rows in the matrix.
 * @param cols Number of columns in the matrix.
 */
void imprime(int **m, int filas, int cols){
    //position: row up or row down
    for (int i=0; i<filas; i++){
            if(i==0){
                printf("    ");
                 for(int j=0; j<cols; j++){
                    printf("| %d |", j);
                 }
                 printf("\n");
            }
            printf("%d ||", i);
            for(int j=0; j<cols; j++){
                if (m[i][j]==7){
                    printf("| * |");
                }
                else if(m[i][j]==99){
                    printf("||> |");
                }
                else{
                    printf("| %d |", m[i][j]);
                }
            }
            printf("\n");
    }
}


/**
 * @brief Generates random bombs in the matrix.
 *
 * This function places a specified number of bombs (represented by the number 9)
 * at random positions within the matrix, ensuring that it does not place a bomb
 * at the given initial position (a, b) or on top of an existing bomb.
 *
 * @param m Pointer to the pointer of the integer matrix.
 * @param filas Number of rows in the matrix.
 * @param cols Number of columns in the matrix.
 * @param bombs Number of bombs to place.
 * @param a Row index of the initial position where no bomb should be placed.
 * @param b Column index of the initial position where no bomb should be placed.
 */
void bombs_generator(int **m, int filas, int cols,int bombs ,int a, int b){
    int k=0;
    while(k<bombs){
        srand(time(NULL));
        int fAL= rand() % (filas);
        int cAL=rand() % (cols);
        if (m[fAL][cAL]!=9 && (fAL!=a || cAL!=b)){
            m[fAL][cAL] =9;
            k=k+1;
        }
    }
}


/**
 * @brief Generates numbers around bombs in the matrix.
 *
 * For each cell in the matrix, this function calculates the number of adjacent cells
 * containing bombs (represented by the number 9) and increments the cell's value by
 * the number of adjacent bombs. This function skips cells that contain bombs.
 *
 * @param m Pointer to the pointer of the integer matrix.
 * @param filas Number of rows in the matrix.
 * @param cols Number of columns in the matrix.
 */
void num_generator (int **m, int filas, int cols){
    for (int i=0; i<filas; i++){
        for (int j=0; j<cols; j++){
            //para each row y column
            if(m[i][j]!=9){
                // if is not a bomb
                for(int p=-1; p<=1; p++){
                    for(int q=-1; q<=1; q++){
                        if ((i+p)>=0 && (i+p)<filas && (j+q)>=0 && (q+j)<cols){
                            if(m[i+p][j+q]==9){
                                m[i][j]++;
                            }
                        }
                    }
                }
            }

        }
    }
}


/**
 * @brief Initializes a game matrix with specified dimensions and places bombs.
 *
 * This function creates a matrix of specified rows and columns, initializes all elements to zero,
 * and then calls the bombs_generator to randomly place a specified number of bombs in the matrix.
 * It also calls num_generator to populate the matrix with numbers indicating the count of adjacent bombs.
 *
 * @param filas Number of rows in the matrix.
 * @param cols Number of columns in the matrix.
 * @param bombs Number of bombs to place in the matrix.
 * @param a Row index to avoid placing a bomb in the initial setup.
 * @param b Column index to avoid placing a bomb in the initial setup.
 * @return Returns a pointer to the initialized matrix.
 */
int **inicialization_matrix(int filas, int cols,int bombs ,int a, int b) {
    ///WE CREATE A COLUMN FULL OF ZEROS
    //int *v = malloc(n * sizeof(int));  // array 1dim of ints
    int* *m = malloc( filas * sizeof(int *)); // Each case holds a memory card.
    for(int i =0;i<filas;i++) {
        m[i] = malloc(cols * sizeof(int));
        /* if we want to fill the matrix with zeros: */
        for(int j=0;j<cols;j++){
            m[i][j]=0;
        }
    }

    /// WE ADDED THE NUMBER OF BOMBS (currently 2)
    bombs_generator(m, filas, cols,bombs ,a, b);


     /// WE ADD THE BOARD NUMBERS
    num_generator(m, filas, cols);


    return m;
}


/**
 * @brief Executes a move in the game based on user input.
 *
 * This function updates the game board based on the player's move. It places or removes a bomb marker,
 * reveals the number of adjacent bombs, or shows an explosion if a bomb is hit. It recursively reveals
 * adjacent cells if an empty cell (zero bombs adjacent) is revealed.
 *
 * @param tablero The player's view of the game board.
 * @param m The actual game matrix with bombs and numbers.
 * @param i Row index of the move.
 * @param j Column index of the move.
 * @param k Move type (0 for placing/removing a bomb marker, any other for revealing the cell).
 * @param casillas Pointer to the count of non-bomb cells left to reveal to win the game.
 * @param filas Number of rows in the matrix.
 * @param cols Number of columns in the matrix.
 */
void jugada(int **tablero, int **m, int i, int j,int k,int *casillas,int filas, int cols ){
    if (tablero[i][j]==7 || tablero[i][j]==99){
        if (k==0){
            tablero[i][j]=99;
        }
        else{
            (*casillas)--;

            if (m[i][j]==9){
                printf("BOOOOM\n");
                imprime(tablero, filas, cols);
                printf("INSERT COIN\n");
                exit(-1);
            }

            if (m[i][j]==0){
                tablero[i][j]=0;
                for (int p=-1; p<=1; p++){
                    for (int q=-1; q<=1; q++){
                        if (p!=0 || q!=0){
                            if ((p+i)>=0 && (p+i)<filas && (j+q)>=0 && (j+q)<cols){
                                jugada(tablero, m, i+p, j+q,k, casillas,filas, cols);
                            }
                        }
                    }
                }
            }

            if (m[i][j]!=0 && m[i][j]!=9){
                tablero[i][j]=m[i][j];
            }
        }
    }
}


/**
 * @brief Manages the game rounds, processes user input, and updates the game state.
 *
 * The function handles the main game loop, processing user inputs for each round and updating
 * the game state accordingly. It checks for win conditions and terminates the game when all
 * non-bomb cells are revealed or a bomb is triggered.
 *
 * @param tablero The player's view of the game board.
 * @param m The actual game matrix with bombs and numbers.
 * @param filas Number of rows in the matrix.
 * @param cols Number of columns in the matrix.
 * @param bombs Number of bombs in the game.
 * @param a Initial safe row index for the first move.
 * @param b Initial safe column index for the first move.
 */
void joc(int **tablero, int **m, int filas, int cols,int bombs ,int a, int b){

   //LET'S CREATE A VARIABLE TO KNOW IF WE ARE STILL ALIVE (simplified way to make a loop)
    int bombas=0; //recuento de bombas seleccionadas
    int bombas_acertadas=0;
    int rondas=1;
    int casillas= filas*cols - bombs;   //boxes that are not bombs that we have to fill in

    /// RONDA INICIAL
    printf("TU TABLERO INICIAL\n");
    jugada(tablero, m, a, b, 1, &casillas,filas, cols);
    imprime(tablero, filas, cols);

    while(casillas>0){
        rondas=rondas+1;
        printf("Por favor, indica la coordenada que quieres seleccionar separada un espacio:\n");
        printf("Ronda %d, %d/%d bombas seleccionadas\n",rondas, bombas, bombs);
        int i,j,k;
        scanf("%d %d %d", &i, &j, &k);
        printf("Coordenada (%d, %d)\n", i,j);
        if (i>=0 && i<filas && j>=0 && j<cols){
            if (tablero[i][j]==7 || tablero[i][j]==99){
            ///Bombs count
                if (k==0){  ///If we select a bomb
                    if (tablero[i][j]!=99){ ///si no estaba seleccionada sumamos
                        bombas=bombas+1;
                        if(m[i][j]==9){
                            bombas_acertadas=bombas_acertadas+1;
                        }
                    }
                    else{
                        printf("Esta casilla ya ha sido seleccionada\n");
                    }
                }

                else{
                    if(tablero[i][j]==99){
                        bombas=bombas-1;
                        if (m[i][j]==9){
                            bombas_acertadas=bombas_acertadas-1;
                        }
                    }
                }

                /// jugada
                jugada(tablero, m, i, j,k,&casillas,filas, cols);
            }
            else{
                printf("Esta casilla ya ha sido seleccionada\n");
            }

            ///IMPRIMIMOS EL TABLERO ACTUALIZADO
            imprime(tablero, filas, cols);
        }
        else{
            printf("La casilla está fuera del tablero\n");
        }

        if (bombas_acertadas==bombs && bombas==bombs){
            printf("HAS GANADO EN LA RONDA %d!!!!\n", rondas);
            imprime(tablero, filas, cols);
            exit(-1);
        }
    }

    printf("HAS GANADO EN LA RONDA %d!!!\n", rondas);
    imprime(tablero, filas, cols);
}

/**
 * @brief The main entry point for the game.
 *
 * Sets up the game based on user-selected difficulty, initializes the game matrices,
 * and starts the main game loop. It handles the setup, including difficulty selection,
 * initial instructions, and first move input.
 */
int main(){
    printf("INICIO\n");
    printf("Indique la dificultad a la que quiere enfrentarse\n");
    printf("FACIL: 6x6 con 5 bombas (PULSA 1)\n");
    printf("MEDIO: 8x8 con 10 bombas (PULSA 2)\n");
    printf("DIFICIL: 10x10 con 20 bombas (PULSA 3)\n");

    int FILAS, COLS, BOMBAS, dificultad;
    scanf("%d", &dificultad);

    if (dificultad==1 || dificultad==2 || dificultad==3){
        FILAS=4+2*dificultad;
        COLS=FILAS;
        BOMBAS=5;
        for (int i=1; i<dificultad; i++){
                BOMBAS=BOMBAS*2;
        }
    }
    else{
        printf("MODO DE JUEGO NO DISPONIBLE\n");
        printf("INSERTE UNA MONEDA :)\n");
    }

    printf("Si quiere leer las instrucciones, PULSE 0, si no pulse cualquier otro:\n");
    int ins;
    scanf("%d", &ins);
    if (ins==0){
        printf("INTRUCCIONES\n");
        printf("1. AL INICIAR EL JUEGO, INDIQUE CON COORDENADAS ('x y') SEPARADAS POR UN ESPACIO LA PRIMERA CASILLA QUE QUIERE DESTAPAR.\n");
        printf("2. LAS SIGUIENTES RONDAS, INDIQUE 'x y a' SEPARADOS POR UN ESPACIO\n");
        printf("3. a=0 si quiere marcar bomba, a != 0 si no\n");
        printf("4. EL JUEGO ACABA CUANDO ENCUENTRE TODAS LAS BOMBAS.\n");
        printf("QUE EMPIECE EL JUEGO\n");


    }

    //CREAMOS UNA MATRIZ QUE ES LA QUE VERÁ EL JUGADOR POR PANTALLA
    int* *tablero = malloc( FILAS * sizeof(int *));
    for(int i =0;i<FILAS;i++) {
        tablero[i] = malloc(COLS * sizeof(int));
        for(int j=0;j<COLS;j++){
            tablero[i][j]=7;
        }
    }

    imprime(tablero, FILAS, COLS);
    /// WE ASK FOR THE FIRST COORDINATE SO IT DOESN'T DIE IN THE FIRST ONE
    printf("Por favor, indica la coordenada que quieres seleccionar separada un espacio 'x y' \n");
    int a,b;
    scanf("%d %d", &a, &b);
    printf("Coordenada (%d, %d)\n", a,b);

    int **m = inicialization_matrix(FILAS, COLS,BOMBAS ,a, b);

    // FOR RANDOM: You create a matrix of 0, you randomly generate the position and put a 1 in it

    /// The Game starts
    joc(tablero, m, FILAS, COLS,BOMBAS ,a, b);

    return 0;
}


