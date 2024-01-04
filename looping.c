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
void createTabXandTabY_monte(double ** xs,double ** ys,int a, int b, int n,int tab[],int stopien){
    srand(time(NULL)); 
    double szerokosc_przedzialu =(double) b - a;
    *xs = (double*)malloc(n*sizeof(double));
    *ys = (double*)malloc(n*sizeof(double));
 
    for (int i = 0; i < n; i++) {
        double xi = a + ((double)rand() / RAND_MAX) * szerokosc_przedzialu; // Losowa wartość z zakresu [a, b]
        (*xs)[i] = xi;
        (*ys)[i] = f(xi, tab, stopien);
    }
    /*for(int i = 0; i < n; i++){
        printf("x:%f\n",(*xs)[i]);
        }

    for(int i = 0; i < n; i++){
        printf("y:%f\n",(*ys)[i]);
        }*/ //drukowanie tablic z wart x i y
}

void createTabXandTabY_simpson(double ** xs,double ** ys,int a, int b, int n,int tab[],int stopien){
     double przedzial = (double)(b - a)/n;
    *xs = (double*)malloc(n*sizeof(double));
    *ys = (double*)malloc(n*sizeof(double));

    for(int i=0; i<=n; i++){
        double k = a + przedzial * i;
        (*xs)[i] = k;
        (*ys)[i] = f(k, tab, stopien);
    }
    
   /* for(int i = 0; i < n; i++){
        printf("x:%f\n",(*xs)[i]);
        }

    for(int i = 0; i < n; i++){
        printf("y:%f\n",(*ys)[i]);
        }*/ //drukowanie tablic z wartosciami x i y
}

double simpsons(int a,int b,double ys[], int n,int tab[],int stopien){
    double przedzial = (double)(b - a) / n;
    double suma = 0.0;
    
  
   for (int i = 0; i <= n; i++) {
     double k = a + przedzial * i;
        if (i == 0 || i == n) {
            suma += f(k, tab, stopien);
        } else if (i % 2 == 0) {
            suma += 2 * f(k, tab, stopien);
        } else {
            suma += 4 * f(k, tab, stopien);
        }
    }

    double calka = (przedzial / 3) * suma;
    return calka;
}

double monte_carlo(int a,int b, double ys[], int n){
    double suma = 0.0;
    double szerokosc_przedzialu =(double) b - a;
    

    for (int i = 0; i < n; i++) {
       
        suma += ys[i];
    
    }

    double calka = (suma * szerokosc_przedzialu) / n; // Obliczenie przybliżonej wartości całki
    return calka;
}

void writeDataToFile_monte(double x[],double y[],int n){
    FILE *dataFile = fopen("plot_data_monte.txt", "w");
    if (dataFile != NULL) {
        for (int i = 0; i < n; i++) {
            fprintf(dataFile, "%f %f\n", x[i], y[i]);
        }
        fclose(dataFile);
    } else {
        printf("Error: Unable to open data file.\n");
        exit(EXIT_FAILURE);
    }
}

void writeDataToFile_simpson(double x[],double y[],int n){
    FILE *dataFile = fopen("plot_data_simpson.txt", "w");
    if (dataFile != NULL) {
        for (int i = 0; i < n; i++) {
            fprintf(dataFile, "%f %f\n", x[i], y[i]);
        }
        fclose(dataFile);
    } else {
        printf("Error: Unable to open data file.\n");
        exit(EXIT_FAILURE);
    }
}

void drawPlot_monte(){ 
    FILE *gnuplotPipeMonte = popen("gnuplot -persistent", "w");
    if (gnuplotPipeMonte != NULL) {
        fprintf(gnuplotPipeMonte, "set terminal pngcairo enhanced color\n");
        fprintf(gnuplotPipeMonte, "set output 'calka_wykres_monte.png'\n");
        fprintf(gnuplotPipeMonte, "set xlabel 'X'\n");
        fprintf(gnuplotPipeMonte, "set ylabel 'Y'\n");
        fprintf(gnuplotPipeMonte, "set title 'Calka Monte Carlo'\n");
        fprintf(gnuplotPipeMonte, "plot 'plot_data_monte.txt' smooth unique title 'Data'\n");

    } else {
        printf("Error: Gnuplot not found or unable to open a pipe.\n");
        exit(EXIT_FAILURE);
    }
    }

void drawPlot_simpson(){ 
    FILE *gnuplotPipeSimpson = popen("gnuplot -persistent", "w");
    if (gnuplotPipeSimpson != NULL) {
        fprintf(gnuplotPipeSimpson, "set terminal pngcairo enhanced color\n");
        fprintf(gnuplotPipeSimpson, "set output 'calka_wykres_simpson.png'\n");
        fprintf(gnuplotPipeSimpson, "set xlabel 'X'\n");
        fprintf(gnuplotPipeSimpson, "set ylabel 'Y'\n");
        fprintf(gnuplotPipeSimpson, "set title 'Calka Simpson'\n");
        fprintf(gnuplotPipeSimpson, "plot 'plot_data_simpson.txt' smooth unique title 'Data'\n");

    } else {
        printf("Error: Gnuplot not found or unable to open a pipe.\n");
        exit(EXIT_FAILURE);
    }
    }

int main()
{
    printf("Monte Carlo Integration, based on Riemann Sum\n");
    double* TabX;
    double* TabY;
    double* tabX;
    double* tabY;
    int st = stopien_wielomianu();
    int *tablica = tablica_z_podanymi_wspolczynnikami(st);
    int a = dolna_gr();
    int b = gorna_gr();
    int n = il_prob();
    createTabXandTabY_monte(&TabX,&TabY,a,b,n,tablica,st);
    createTabXandTabY_simpson(&tabX,&tabY,a,b,n,tablica,st);
    printf("Przyblizona wartosc calki numerycznej metoda monte carlo z wielomianu to:%f\n",monte_carlo(a,b, TabY, n));
    printf("Przyblizona wartosc calki numerycznej metoda Simpsona z wielomianu to:%f\n",simpsons(a,b, tabY, n,tablica,st));
    writeDataToFile_monte(TabX,TabY,n);
    drawPlot_monte();
    writeDataToFile_simpson(tabX,tabY,n);
    drawPlot_simpson();
    free(TabX);
    free(TabY); 
    free(tabX);
    free(tabY);

    return 0;
}