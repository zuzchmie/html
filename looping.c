// MONTE CARLO PROJECT

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

int stopien_wielomianu(){
    int st;
    while(1){
    printf("Podaj stopien wielomianu: ");
    if(scanf("%d", &st) == 1){
        break;
    }
        else{
            printf("Nieprawidlowe dane. Podaj poprawny stopien wielomianu (liczba).\n");
             while (getchar() != '\n');
        }
    }
    return st;
}

int *tablica_z_podanymi_wspolczynnikami(int st){
    int* tablica_wspolczynnikow = (int*)malloc(st*sizeof(int));
    while(1){
        for (int i = 0; i < st+1; i++){
            printf("Podaj %d wspolczynnik:", i);
            if(scanf("%d", &tablica_wspolczynnikow[i])==1){
                continue;
        }
            else{
                printf("Nieprawidlowe dane. Podaj poprawny stopien wielomianu (liczba).\n");
                while (getchar() != '\n');
                i--;
            }
        }
        break;
    }
    return tablica_wspolczynnikow;
}

int f(int x, int tab[], int stopien) {
    int wynik = tab[0]; // Początkowa wartość wyniku to współczynnik przy x^3 (najwyższa potęga)

    for (int i = 1; i <= stopien; i++) {
        wynik = wynik * x + tab[i]; // Schemat Hornera
    }

    return wynik;
}

int dolna_gr(){
    int a;
    while(1){
        printf("Podaj dolna granice zasiegu liczenia calki:");
        if(scanf("%d",&a)==1){
            break;
        }
            else{
                printf("Nieprawidlowe dane. Podaj poprawny stopien wielomianu (liczba).\n");
                while (getchar() != '\n');
            }
    }
    return a;
}


int gorna_gr(){
    int b;
    while(1){
        printf("Podaj gorna granice zasiegu liczenia calki:");
        if(scanf("%d",&b)==1){
            break;
        }
            else{
                printf("Nieprawidlowe dane. Podaj poprawny stopien wielomianu (liczba).\n");
                while (getchar() != '\n');
            }
    }
    return b;
}

int il_prob(){
    int n;
    while(1){
        printf("Podaj dokladnosc z jaka chcesz liczyc calke(1-100000):");
        if(scanf("%d", &n)==1){
             break;
        }
            else{
                printf("Nieprawidlowe dane. Podaj poprawny stopien wielomianu (liczba).\n");
                while (getchar() != '\n');
            }
    }
    return n;
}
void createTabXandTabY_monte(int ** xs,int ** ys,int a, int b, int n,int tab[],int stopien){
    srand(time(NULL)); 
    double szerokosc_przedzialu = b - a;
    *xs = (int*)malloc(n*sizeof(int));
    *ys = (int*)malloc(n*sizeof(int));
 
    for (int i = 0; i < n; i++) {
        double xi = a + ((double)rand() / RAND_MAX) * szerokosc_przedzialu; // Losowa wartość z zakresu [a, b]
        (*xs)[i] = xi;
        (*ys)[i] = f(xi, tab, stopien);
    }
}



double monte_carlo(int a,int b, int ys[], int n){
    double suma = 0.0;
    double szerokosc_przedzialu = b - a;
    

    for (int i = 0; i < n; i++) {
       
        suma += ys[i];
    }

    double calka = (suma * szerokosc_przedzialu) / n; // Obliczenie przybliżonej wartości całki
    return calka;
}

void writeDataToFile(int x[],int y[],int n){
    FILE *dataFile = fopen("plot_data.txt", "w");
    if (dataFile != NULL) {
        for (int i = 0; i < n; i++) {
            fprintf(dataFile, "%d %d\n", x[i], y[i]);
        }
        fclose(dataFile);
    } else {
        printf("Error: Unable to open data file.\n");
        exit(EXIT_FAILURE);
    }
}

int main()
{
    printf("Monte Carlo Integration, based on Riemann Sum\n");
    int* TabX;
    int* TabY;
    int st = stopien_wielomianu();
    int *tablica = tablica_z_podanymi_wspolczynnikami(st);
    int a = dolna_gr();
    int b = gorna_gr();
    int n = il_prob();
    createTabXandTabY_monte(&TabX,&TabY,a,b,n,tablica,st);
    printf("Przyblizona wartosc calki numerycznej metoda monte carlo z wielomianu to:%f\n",monte_carlo(a,b, TabY, n));
    writeDataToFile(TabX,TabY,n);
     FILE *gnuplotPipe = popen("gnuplot -persistent", "w");
    if (gnuplotPipe != NULL) {
        fprintf(gnuplotPipe, "set terminal pngcairo enhanced color\n");
        fprintf(gnuplotPipe, "set output 'calka_wykres.png'\n");
        fprintf(gnuplotPipe, "set xlabel 'X'\n");
        fprintf(gnuplotPipe, "set ylabel 'Y'\n");
        fprintf(gnuplotPipe, "set title 'Calka'\n");
        fprintf(gnuplotPipe, "plot 'plot_data.txt' with linespoints title 'Data'\n");

        fclose(gnuplotPipe);
    } else {
        printf("Error: Gnuplot not found or unable to open a pipe.\n");
        exit(EXIT_FAILURE);
    }

    return 0;
}