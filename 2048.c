#include <ncurses.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <unistd.h>

/* salvam matricea de numere si scorul curent
    in 2 fisiere numite save_score.txt si save_nr.txt */
void save(int nr[4][4],int *score){
    int i,j;
    FILE *pfile_score=fopen("save_score.txt","w");
    FILE *pfile_nr=fopen("save_nr.txt","w");
    fprintf(pfile_score,"%d",*score);
    fclose(pfile_score);
    for(i = 0;i < 4;i++){
     for(j = 0;j < 4;j++){
        fprintf(pfile_nr,"%d ",nr[i][j]);
     }
     fprintf(pfile_nr,"\n");
    }
    fclose(pfile_nr);
}

 /* incarcam valorile numerelor din matricea salvata
    si scorul salvat in fisierele save_score.txt si save_nr.txt */
void load(int nr[4][4],int *score){
    int i,j;
    FILE *pfile_score=fopen("save_score.txt","r");
    FILE *pfile_nr=fopen("save_nr.txt","r");
    fscanf(pfile_score,"%d",score);
    fclose(pfile_score);
    for(i = 0;i < 4;i++){
        for(j = 0;j < 4;j++){
            fscanf(pfile_nr,"%d",&nr[i][j]);
        }
    }
    fclose(pfile_nr);
}

 /* verificam daca o miscare este valida */
int check_valid(int nr[4][4],int *score){
    int i,j,cntr = 0,aux[4][4];
    for(i = 0;i < 4;i++){
        for(j = 0;j < 4;j++){
             /* salveazam valorile numerelor din tabela
             intr-o tabela auxiliara */
            aux[i][j] = nr[i][j];
        }
    }
    /* returnam tabela la valoarea precedenta miscarii */
    load(nr,score);
    for(i = 0;i < 4;i++){
        for(j = 0;j < 4;j++){
            if(nr[i][j] == aux[i][j]){
                /* daca valoare dintr-o pozitie din tabela este 
                neschimbata dupa mutare,incrementam variabila cntr*/
                cntr++; 
            }
        }
    }
    /* daca toate valorile sunt neschimbate, returnam valoarea 1 */
    if(cntr == 16){
        return 1;
    }
     /* daca exista vreo schimbare in tabela, returnam valoarea 0 */
    else {
    return 0;
    }
}

/* verificam daca una dintre conditile pentru terminarea jocului
(nu mai pot fi efectuate miscari sau avem valoarea 2048 pe tabela)
este indeplinita*/
int check(int nr[4][4]){
        int i,j,ok = 1,cntr = 0,check = 0;
        for(i = 0;i < 4;i++){
            for(j = 0;j < 4;j++){
                /* valoarea variabilei ok devine 0
                    daca conditia de castig este indeplinita */
                if (nr[i][j] == 2048){
                    ok = 0;
                }
                /* daca o celula este ocupata,
                    dar nu de numarul 2048,incrementam variabila cntr*/
                 if(nr[i][j] != 2048 && nr[i][j] != 0){
                                cntr++;
                            }
            }
        }
        if(cntr == 16){
            for(i = 0;i < 4;i++){
                for (j = 0; j < 3; j++) {
                /* daca tabela este plina dar se mai pot efectua
                imbinari,variabila check are valoarea 1 */
                if (nr[i][j] == nr[i][j + 1] || nr[j][i] == nr[j + 1][i]) { 
                    check = 1;
                    break;
                }
            }
            }
        }
        /* daca tabela este plina si nu mai se pot efectua
         imbinari,variabila ok are valoarea 2*/
        if(cntr == 16 && check == 0){
            ok = 2;
        }
       return ok;
}

 /* aflam numarul de imbinari posibile pe tabela */
void check_merge(int nr[4][4],int aux[4],int n,int *score,int aux1[4]){
    int i,j;
    for(i = 0;i < 3;i++){
        for(j = 0;j < 3;j++){
            /* daca o imbinare este posibila incrementam valoarea vectorului aux1[n] */
            if (nr[i][j] == nr[i+1][j] || nr[i][j] == nr[i][j+1]){
                aux1[n]++;
            }
            if(nr[i][3] == nr[i+1][3]){
                aux1[n]++;
            }
            if(nr[3][j] == nr[3][j+1]){
                aux1[n]++;
            }
        }
    }
}

/* aflam numarul de spatii libere ramase dupa o mutare */
void check_free(int nr[4][4],int aux[4],int n,int *score,int aux1[4]){
    int i,j;
     /* daca o mutare este valida,numaram spatiile libere*/
    if(check_valid(nr,score) == 0){
    for(i = 0 ;i < 4;i++){
        for(j = 0;j < 4;j++){
             /* daca un spatiu este liber,
            incrementam vectorul aux[n] */
            if(nr[i][j] == 0){
                aux[n]++;
            }
        }
    }
    /* daca toata tabla este ocupata,
     alfam numarul de imbinari posibile*/
    if (aux[n] == 0){
        check_merge(nr,aux,n,score,aux1);
    }
    }
     /* daca mutarea nu este valida,
     atribuim vectorului aux[n] valoarea -1,
    la care nu ar putea ajunge in urma unei miscari valide*/
    else if(check_valid(nr,score) == 1){
        aux[n] = -1;
    }
}

 /* initializam toate valorile din tabela
     cu 0 la inceputul jocului */
void init_nr(int nr[4][4]){
     int i,j;
     for(i = 0;i < 4;i++)
        for(j = 0;j < 4;j++){
            nr[i][j] = 0;
        }
}

/* atribuim unor n celule valori aleatori din intervalul {2,4} */
void RAND(int nr[4][4],int n){
    int pwr[2],i,t,k;
    pwr[0] = 2;
    pwr[1] = 4;
    srand(time(NULL));
    for(i=0;i<n;i++){
        /* alegem aleatoriu celule de pe tabela pana cand gasim una libera*/
    do{
        t = rand()%4;
        k = rand()%4;
        }
        while (nr[k][t] != 0);
        /* cand gasim o celula libera,ii atribuim o valoare aleatorie*/
        nr[k][t] = pwr[rand()%2];
    }    
}

/* mutam numerele din tabela catre stanga */
void MOVE_LEFT(int nr[4][4],int n,int *score){
    int i,aux,j,ok = 1;
    /* mutam numerele in cel mai la stanga spatiu liber*/
     for(j = 0;j < 4;j++){
    for(i = 3;i > 0;i--){
        if(nr[n][i] != 0 && nr[n][i-1] == 0){
            aux = nr[n][i-1];
            nr[n][i-1] = nr[n][i];
            nr[n][i] = aux;
            
        }
    }
     }
     /* imbinam numerele egale din spatii alaturate*/
     if(nr[n][0] == nr[n][1] && nr[n][2] == nr[n][3] && nr[n][0] != nr[n][2]){
            ok = 2;
        }
        for(j = 0;j < ok;j++){
        for(i = 3;i > 0;i--){
        if(nr[n][i] != 0 && nr[n][i-1] == nr[n][i]){
           if(nr[n][i-2] == nr[n][i-1]){
            if(nr[n][i-3] == nr[n][i-2]){
               nr[n][i-3] = 2*nr[n][i-3];
               nr[n][i-2] = 2*nr[n][i-2];
              *score = *score + 2*nr[n][i-2];
               nr[n][i] = 0;
               nr[n][i-1] = 0;
               break;
           }
           else{
                nr[n][i-2] = 2*nr[n][i-2];
                *score = *score + nr[n][i-2];
                nr[n][i] = 0;
                break;
            }
           }
           else {
            nr[n][i-1] = 2*nr[n][i-1];
            *score = *score + nr[n][i-1];
            nr[n][i] = 0;
            break;
           }
        
    
        }
    }
        }
    /* mutam inca o data numerele spre stanga dupa imbinare*/
     for(j = 0;j < 4;j++){
    for(i = 3;i > 0;i--){
        if(nr[n][i] != 0 && nr[n][i-1] == 0){
            aux = nr[n][i-1];
            nr[n][i-1] = nr[n][i];
            nr[n][i] = aux;
            
        }
    }
     }
  } 

/* mutam numerele din tabela catre dreapta*/
void MOVE_RIGHT(int nr[4][4],int n,int *score){
    int i,aux,j,ok = 1;
    /* mutam numerele in cel mai la dreapta spatiu liber*/
    for(j = 0;j < 4;j++){
    for(i = 0;i < 3;i++){
        if(nr[n][i] != 0 && nr[n][i+1] == 0){
            aux = nr[n][i+1];
            nr[n][i+1] = nr[n][i];
            nr[n][i] = aux;
        }  
    }
    }
    /* imbinam numerele egale din spatii alaturate*/
    if(nr[n][0] == nr[n][1] && nr[n][2] == nr[n][3] && nr[n][0] != nr[n][2]){
            ok = 2;
        }
        for(j = 0;j < ok;j++){
        for(i = 0;i < 3;i++){    
        if(nr[n][i] != 0 && nr[n][i+1] == nr[n][i]){
           if(nr[n][i+2] == nr[n][i+1]){
            if(nr[n][i+3] == nr[n][i+2]){
               nr[n][i+3] = 2*nr[n][i+3];
               nr[n][i+2] = 2*nr[n][i+2];
              *score = *score + 2*nr[n][i+2];
               nr[n][i] = 0;
               nr[n][i+1] = 0;
               break;
           }
            else{
                nr[n][i+2] = 2*nr[n][i+2];
                *score = *score + nr[n][i+2];
                nr[n][i] = 0;
                break;
            }
           }
           else {
            nr[n][i+1] = 2*nr[n][i+1];
            *score = *score + nr[n][i+1];
            nr[n][i] = 0;
            break;
           }
        
    
        }
    }
    }
    /* mutam numerele inca o data catre dreapta pentru a lua in
    considerare diferentele rezultate in urma imbinarii*/
    for(j = 0;j < 4;j++){
    for(i = 0;i < 3;i++){
        if(nr[n][i] != 0 && nr[n][i+1] == 0){
            aux = nr[n][i+1];
            nr[n][i+1] = nr[n][i];
            nr[n][i] = aux;
        }  
    }
    }
  } 

/* mutam numerele in jos*/
void MOVE_DOWN(int nr[4][4],int n,int *score){
    int i,aux,j,ok = 1;
    /* mutam numerele in cea mai joasa pozitie posibila */
    for(j = 0;j < 4;j++){
    for(i = 0;i < 3;i++){
       
        if(nr[i][n] != 0 && nr[i+1][n]==0){
            aux = nr[i+1][n];
            nr[i+1][n] = nr[i][n];
            nr[i][n] = aux;
        }
    }
    } 
    /* imbinam numerele egale si alaturate*/  
    if(nr[0][n] == nr[1][n] && nr[2][n] == nr[3][n] && nr[0][n] != nr[2][n]){
            ok = 2;
        }    
        for(j = 0;j < ok;j++){
        for(i = 0;i < 3;i++){
        if(nr[i][n] != 0 && nr[i+1][n] == nr[i][n]){
            if(nr[i+2][n] == nr[i+1][n]){
                if(nr[i+3][n] == nr[i+2][n]){
                    nr[i+3][n] = 2*nr[i+3][n];
                    nr[i+2][n] = nr[i+3][n];
                    *score = *score + 2*nr[i+3][n];
                    nr[i+1][n] = 0;
                    nr[i][n] = 0;
                    break;
                }
                else {
                   nr[i+2][n] = 2*nr[i+2][n];
                  *score = *score + nr[i+2][n];
                  nr[i][n] = 0;
                  break;
               }
            }
            else{
                nr[i+1][n] = 2*nr[i+1][n];
                *score = *score + nr[i+1][n];
                nr[i][n] = 0;
                break;
            }
    
    }
}
        }
        /* mutam numerele in jos inca o data pentru a lua in
        considerare diferentele rezultate in urma imbinarii*/
for(j = 0;j < 4;j++){
    for(i = 0;i < 3;i++){
       
        if(nr[i][n] != 0 && nr[i+1][n] == 0){
            aux = nr[i+1][n];
            nr[i+1][n] = nr[i][n];
            nr[i][n] = aux;
        }
    }
    }
}

/* mutam in sus numerele */
void MOVE_UP(int nr[4][4],int n,int *score){
    int i,aux,j,ok=1;
    /* mutam numerele in cea mai de sus pozitie*/
    for(j = 0;j < 3;j++){
    for(i = 3;i > 0;i--){
        if(nr[i][n] != 0 && nr[i-1][n] == 0){
            aux = nr[i-1][n];
            nr[i-1][n] = nr[i][n];
            nr[i][n] = aux;
        }
    }
    }
    /* imbinam numerele egale si alaturate*/
        if(nr[3][n] == nr[2][n] && nr[1][n] == nr[0][n] && nr[3][n] != nr[1][n]){
            ok = 2;
        }    
        for(j = 0;j < ok;j++){
        for(i = 3;i > 0;i--){
        if(nr[i][n] != 0 && nr[i-1][n] == nr[i][n]){
            if(nr[i-2][n] == nr[i-1][n]){
                if(nr[i-3][n] == nr[i-2][n]){
                    nr[i-3][n] = 2*nr[i-3][n];
                    nr[i-2][n] = nr[i-3][n];
                    *score = *score + 2*nr[i-3][n];
                    nr[i-1][n] = 0;
                    nr[i][n] = 0;
                    break;
                }
                else {
                   nr[i-2][n] = 2*nr[i-2][n];
                  *score = *score + nr[i-2][n];
                  nr[i][n] = 0;
                  break;
        }
            }
            else{
                nr[i-1][n] = 2*nr[i-1][n];
                *score = *score + nr[i-1][n];
                nr[i][n] = 0;
                break;
            }
    
    }
}
        }
    /* mutam inca o data in sus pentru a lua in
    considerare schimbarile rezultate in urma imbinarii*/
 for(j = 0;j < 3;j++){
    for(i = 3;i > 0;i--){
        if(nr[i][n] != 0 && nr[i-1][n] == 0){
            aux = nr[i-1][n];
            nr[i-1][n] = nr[i][n];
            nr[i][n] = aux;
        }
    }
    }
}

void MENIU(int cntr,int nr[4][4]);

/* cream si manipulam tabela de joc*/
void TABLE(int cntr,int nr[4][4],int n,int *score){
    int i,j,ch,aux[4],max_free = -1,aux1[4],max_merge = -1,ch1; 
    /* obtinem data si ora curenta */
    WINDOW *fereastra;
    time_t t;
    struct tm *area;
    t = time (NULL);
    area = localtime(&t);
    initscr();
    /* initializam perechile de culori */
    start_color();
    init_pair(1,COLOR_BLACK,COLOR_WHITE);
    init_pair(2,COLOR_BLACK,COLOR_BLACK);
    init_pair(3,COLOR_BLACK,COLOR_GREEN);
    init_pair(4,COLOR_BLACK,COLOR_BLUE);
    init_pair(5,COLOR_BLACK,COLOR_CYAN);
    init_pair(6,COLOR_BLACK,COLOR_MAGENTA);
    init_pair(7,COLOR_BLACK,COLOR_YELLOW);
    init_pair(8,COLOR_WHITE,COLOR_BLACK);
    init_pair(9,COLOR_BLACK,COLOR_RED);
    init_pair(10,COLOR_WHITE,COLOR_YELLOW);
    init_pair(11,COLOR_WHITE,COLOR_MAGENTA);
    init_pair(12,COLOR_WHITE,COLOR_RED);
    init_pair(13,COLOR_BLACK,COLOR_BLUE);
    /* adaugam un numar n de numere cu
    valoari aleatorii din multimea {2,4} */
    RAND(nr,n);
    /* cream tabela de joc */
    for (i = 0;i < 4;i++){
    for(j = 0;j < 4;j++){
    fereastra = newwin(8,16,(i+1)*7,(j+1)*15);
    box(fereastra,0,0);
    // coloram celule tabelelei corespunzator numerelor din acestea
    if(nr[i][j] == 0){
    wbkgd(fereastra,COLOR_PAIR(1));
    }
    else if(nr[i][j] == 2){
        wbkgd(fereastra,COLOR_PAIR(3));
    }
    else if(nr[i][j] == 4){
        wbkgd(fereastra,COLOR_PAIR(4));
    }
    else if(nr[i][j] == 8){
        wbkgd(fereastra,COLOR_PAIR(5));
    }
    else if(nr[i][j] == 16){
        wbkgd(fereastra,COLOR_PAIR(6));
    }
    else if(nr[i][j] == 32){
        wbkgd(fereastra,COLOR_PAIR(7));
    }
    else if(nr[i][j] == 64){
        wbkgd(fereastra,COLOR_PAIR(8));
    }
    else if(nr[i][j] == 128){
        wbkgd(fereastra,COLOR_PAIR(9));
    }
    else if(nr[i][j] == 256){
        wbkgd(fereastra,COLOR_PAIR(10));
    }
    else if(nr[i][j] == 512){
        wbkgd(fereastra,COLOR_PAIR(11));
    }
    else if(nr[i][j] == 1024){
        wbkgd(fereastra,COLOR_PAIR(12));
    }
    else if(nr[i][j] == 2048){
        wbkgd(fereastra,COLOR_PAIR(13));
    }
    if (nr[i][j] == 0){
    mvwprintw(fereastra,3.5,7.5," ");
    }
    else {
        mvwprintw(fereastra,3.5,7.5,"%d",nr[i][j]);
    }
    wrefresh(fereastra);
    }
    }
    /* cream si afisam tabelul cu data,scorul si legenda */ 
    fereastra = newwin(5.5,61,2.5,15);
    box(fereastra,0,0);
    mvwprintw(fereastra,1.5,5,"Date and hour: %s",asctime(area));
    mvwprintw(fereastra,2.5,5,"Score: %d",*score);
    mvwprintw(fereastra,3.5,5,"Use the arrow keys to move the tiles and Q to exit");
    wbkgd(fereastra,COLOR_PAIR(2));
    wrefresh(fereastra);
    /* verificam daca jocul mai poate fi continuat */
    /* daca jocul este castigat, afisam mesajul corespunzator
    si iesim din program dupa 2 secunde*/
    if(check(nr) == 0){
            save(nr,score);
            clear();
            wbkgd(stdscr,COLOR_PAIR(13));
            mvwprintw(stdscr,20,80,"YOU WON !!! GOOD JOB\n");
            mvwprintw(stdscr,21,80,"Your Score:%d",*score);
             mvwprintw(stdscr,22,80,"Press Q to exit or wait 4 seconds to exit the game");
            refresh();
            halfdelay(40);
            ch1 = getch();
            if (ch1 == 81 || ch1 == 113){
                 wbkgd(stdscr,COLOR_PAIR(0));
                MENIU(10000,nr);
                refresh();
            }
            else if( ch1 == ERR){
            endwin();
            exit(0);
            }
        }
    /* daca jocul este pierdut, afisam mesajul corespunzator
    si iesim din program dupa 2 secunde*/
        else if(check(nr) == 2){
            save(nr,score);
            clear();
            wbkgd(stdscr,COLOR_PAIR(12));
            mvwprintw(stdscr,20,80,"GAME OVER :(\n");
            mvwprintw(stdscr,21,80,"Your Score:%d",*score);
            mvwprintw(stdscr,22,80,"Press Q to exit or wait 4 seconds to exit the game");
            refresh();
            halfdelay(40);
            ch1 = getch();
            if (ch1 == 81 || ch1 == 113){
                 wbkgd(stdscr,COLOR_PAIR(0));
                MENIU(10000,nr);
                refresh();
            }
            else if( ch1 == ERR){
            endwin();
            exit(0);
            }
        }
    /* salvam scorul si matricea inainte de aplicare miscarilor */
    save(nr,score);
    /* setam o limita de 5 secunde in care utilizatorul trebuie sa dea un input,
    in caz contrar sistemul efectuand o miscare automatizata */
    halfdelay(50);
    /* luam input-ul utilizatorului */
    ch = getch(); 
   
    if(ch == KEY_UP){
       /* simulam miscarea */
        for(i = 0;i < 4;i++){
            MOVE_UP(nr,i,score);
       }
       /* daca mutarea este valida,o executam*/
        if(check_valid(nr,score) == 0){
             for(i = 0;i < 4;i++){
            MOVE_UP(nr,i,score);
       }
        refresh();
        TABLE(cntr,nr,1,score);
        refresh();       
        }
        /* daca mutarea nu este valida,
        ne intoarcem la varianta tabelei salvata precedent*/
         else if(check_valid(nr,score) == 1){     
        load(nr,score); 
        refresh();
        TABLE(cntr,nr,0,score);
        refresh(); 
        }
    }
    
    if(ch == KEY_LEFT){
        /* simulam miscarea */
        for(i = 0;i < 4;i++){
            MOVE_LEFT(nr,i,score);
        }
        /* daca mutarea este valida,o executam */
        if(check_valid(nr,score) == 0){
        for(i = 0;i < 4;i++){
            MOVE_LEFT(nr,i,score);
        }
        refresh();
        TABLE(cntr,nr,1,score);
        refresh();       
        }       
        /* daca mutarea nu este valida,
        ne intoarcem la varianta tabelei salvata precedent*/
         else if(check_valid(nr,score) == 1){
        load(nr,score);
        refresh();
        TABLE(cntr,nr,0,score);
        refresh(); 
        }
    }
  
    if(ch == KEY_RIGHT){
        /* simulam miscarea */
        for(i = 0;i < 4;i++){
             MOVE_RIGHT(nr,i,score);
        }
        /* daca mutarea este valida,o executam*/
       if(check_valid(nr,score) == 0){
        for(i = 0;i < 4;i++){
             MOVE_RIGHT(nr,i,score);
        }
        refresh();
        TABLE(cntr,nr,1,score);
        refresh();       
        }
         /* daca mutarea nu este valida,
        ne intoarcem la varianta tabelei salvata precedent*/
        else if(check_valid(nr,score) == 1){
        load(nr,score); 
        refresh();
        TABLE(cntr,nr,0,score);
        refresh(); 
        }
    }
    
    if(ch == KEY_DOWN){
        /* simulam miscarea */
        for(i = 0;i < 4;i++){
             MOVE_DOWN(nr,i,score);
        }
        /* daca mutarea este valida,o executam*/
       if(check_valid(nr,score) == 0){
        for(i = 0;i < 4;i++){
             MOVE_DOWN(nr,i,score);
        }
        refresh();
        TABLE(cntr,nr,1,score);
        refresh();       
        } 
        /* daca mutarea nu este valida,
        ne intoarcem la varianta tabelei salvata precedent*/
         else if(check_valid(nr,score) == 1){
        load(nr,score);    
        refresh();
        TABLE(cntr,nr,0,score);
        refresh(); 
        }     
    }

    /* daca tasta apasata este Q, ne intoarcem la meniu */
     if(ch == 81 || ch == 113){
        clear();
        save(nr,score);
        MENIU(cntr,nr);
        refresh();
    }
    
    /* daca utilizatorul nu da niciun input,
        sistemul alege automat miscarea care elibereaza cele mai multe spatii */
     if(ch == ERR){
        for(i = 0;i < 4;i++){
            aux[i] = 0;
            aux1[i] = 0;
        }
        /* salvam starea curenta a matricei */
       save(nr,score);
       /* executam cele 4 miscari,calculand numarul de celule eliberate */
       for(i = 0;i < 4;i++){
             MOVE_DOWN(nr,i,score);
       } 
        check_free(nr,aux,0,score,aux1);
        load(nr,score);
        for(i = 0;i < 4;i++){
             MOVE_RIGHT(nr,i,score);
        }
        check_free(nr,aux,1,score,aux1);
       load(nr,score);
        for(i = 0;i < 4;i++){
             MOVE_LEFT(nr,i,score);
        }
        check_free(nr,aux,2,score,aux1);
        load(nr,score);
        for(i = 0;i < 4;i++){
             MOVE_UP(nr,i,score);
        }
        check_free(nr,aux,3,score,aux1);
        /* gasim miscarea care elibereaza cele mai multe celule
        si salvam numarul de celule eliberate in variabila max_free */
        for (i = 0;i < 4;i++){
                if(aux[i] > max_free){
                    max_free = aux[i];
            }

            /* daca doua miscari elibereaza acelasi numar de celule,
             vom folosi numarul de imbinari pentru a le departaja */
                else if(aux[i] == max_free){
                    if(aux1[i] > max_merge){
                        max_free = aux[i];
                    }
                }
        }
        /* gasim miscarea care produce cel mai mare numar de imbinari si
         salvam numarul de celule imbinate in variabila max_merge */
        for (i = 0;i < 4;i++){
                if(aux1[i] > max_merge){
                    max_merge = aux1[i];
            }
        }
        /* daca tabela este plina,
        vom decide cea mai buna miscare in functie de imbinarile efectuate */
         if (max_free == 0){
            if (max_merge == aux1[0]){
              load(nr,score);
              for(i = 0;i < 4;i++){
              MOVE_DOWN(nr,i,score);
                } 
            } 
            if (max_merge == aux1[1]){
              load(nr,score);
              for(i = 0;i < 4;i++){
              MOVE_RIGHT(nr,i,score);
                } 
            } 
             if (max_merge == aux1[2]){
              load(nr,score);
              for(i = 0;i < 4;i++){
              MOVE_LEFT(nr,i,score);
                } 
            } 
            if (max_merge == aux1[3]){
              load(nr,score);
              for(i = 0;i < 4;i++){
              MOVE_UP(nr,i,score);
                } 
            }              
        } 
        /* daca tabela nu este plina,miscarea
        care elibereaza cele mai multe celule este aleasa */
        if(max_free == aux[0] && aux[0] > 0){
        load(nr,score);
        for(i = 0;i < 4;i++){
             MOVE_DOWN(nr,i,score);
        } 
        }
        else if(max_free == aux[1] && aux[1] > 0){
        load(nr,score);
        for(i = 0;i < 4;i++){
             MOVE_RIGHT(nr,i,score);
        }
        }
        else if(max_free == aux[2] && aux[2] > 0){
        load(nr,score);
        for(i = 0;i < 4;i++){
             MOVE_LEFT(nr,i,score);
        }
        }
        else if(max_free == aux[3] && aux[3] > 0){
        load(nr,score);
        for(i = 0;i < 4;i++){
             MOVE_UP(nr,i,score);
        }
        }
        refresh();
        TABLE(cntr,nr,1,score);
        refresh(); 
        
    }
   
    endwin();   
}

/* afisam si mutam meniul in functie de valoare variabilelei cntr */
void MENIU(int cntr,int nr[4][4]){
    int ch,score;
    initscr();
    raw();
    keypad(stdscr,TRUE);
    if (cntr%3 == 1){
        refresh();
        mvwprintw(stdscr,20,60,"Use the arrow keys to navigate and the ENTER key to select\n");
        mvwprintw(stdscr,21,60,"                   -> NEW GAME\n");
        mvwprintw(stdscr,22,60,"                      Resume\n");
        mvwprintw(stdscr,23,60,"                      Quit\n");
        ch = getch();
        refresh();
        clear();
    /* daca apasam key_down valoarea variabilei cntr este marita */
    if (ch == KEY_DOWN){
        cntr++;
        MENIU(cntr,nr);
    }
    /* daca apasam key_up valoarea variabilei cntr este micsorata */    
    else if(ch == KEY_UP){
        cntr--;
        MENIU(cntr,nr);
    }
    /* daca apasam ENTER icepem un nou joc*/
    else if(ch == 10){
        refresh();
        init_nr(nr);
        score=0;
        TABLE(cntr,nr,2,&score);
    }
    }

    if (cntr%3 == 2){
        refresh();
        mvwprintw(stdscr,20,60,"Use the arrow keys to navigate and the ENTER key to select\n");
        mvwprintw(stdscr,21,60,"                      New Game\n");
        mvwprintw(stdscr,22,60,"                   -> RESUME\n");
        mvwprintw(stdscr,23,60,"                      Quit\n");
        ch = getch();
        refresh();
        clear();
    /* daca apasam key_down valoarea variabilei cntr este marita */
    if (ch == KEY_DOWN){
        cntr++;
        MENIU(cntr,nr);
    }
    /* daca apasam key_up valoarea variabilei cntr este micsorata */    
    else if(ch == KEY_UP){
        cntr--;
        MENIU(cntr,nr);
    }
    /* daca apasam ENTER continuam jocul salvat */
    else if(ch == 10){
        /* verifica daca exista fisierele necesare
        pentru resume si daca da,le incarca*/
        if (access("save_nr.txt",F_OK) == 0 && access("save_score.txt",F_OK) == 0){
        refresh();
         load(nr,&score);
         TABLE(cntr,nr,0,&score);
        }
        else {
            clear();
            mvwprintw(stdscr,21,80,"You have no saved games");
            refresh();
            sleep(2);
            MENIU(cntr,nr);
            refresh();
        }
    }
    }

    if (cntr%3 == 0){
        refresh();
        mvwprintw(stdscr,20,60,"Use the arrow keys to navigate and the ENTER key to select\n");
        mvwprintw(stdscr,21,60,"                      New Game\n");
        mvwprintw(stdscr,22,60,"                      Resume\n");
        mvwprintw(stdscr,23,60,"                   -> QUIT\n");
        ch = getch();
        refresh();
        clear();
    /* daca apasam key_down valoarea variabilei cntr este marita */    
    if (ch == KEY_DOWN){
        cntr++;
        MENIU(cntr,nr);
    }
     /* daca apasam key_up valoarea variabilei cntr este micsorata */    
    else if(ch == KEY_UP){
        cntr--;
        MENIU(cntr,nr);
    }
    /* daca apasam ENTER iesim din program*/
    else if(ch == 10){
        endwin();
        exit(0);

    }
    }
    endwin();
}

int main(){
    int nr[4][4];
    initscr();
    raw();
    keypad(stdscr,TRUE);
    init_nr(nr);
    MENIU(10000,nr);
    refresh();
    endwin();
    return 0;
}