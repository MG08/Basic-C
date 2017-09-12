//
//  main.c
//  ReadData
//
//  Created by Miguel Garcia on 8/30/17.
//  Copyright © 2017 Miguel Garcia. All rights reserved.
//

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <sys/time.h>

int nData,nFeatures, nCentroides;
double **datos, **centroides,**centroidesNuevos;
static FILE *fp_data, *fp_centroides,*fp_resultados;

//clock_t start;
//clock_t end;

struct timeval t0;
struct timeval t1;
double elapsed;

int change;
int iteracion;

void readData(FILE *data, int Ndata,int NFeatures,double **datos);
void inicializacion(int argc, char *argv[]);
void asignacion(double **datos,double **centroides,int nData, int nFeatures, int nCentroides);
void recalculo(double **datos, double **centroides,int nData, int nFeatures, int nCentroides);
double timedifference_msec(struct timeval t0, struct timeval t1);
double sumatoria_error(double **datos,int nData,double **centroides,int nCentroides, int nFeatures);
//double calculoErrorCuadrado(int Nobjetos, int Ncentros, int Ncaracteristicas, double **centroides, double **datos);


int main(argc, argv)
    int argc;
    char * argv[];
{
    
    if ((fp_data=fopen(argv[1], "r"))==NULL) {
        fprintf(stderr, "No file datos %s\n",argv[1]);
        return 0;
    }
    if ((fp_data=fopen(argv[4], "r"))==NULL) {
        fprintf(stderr, "No file centriodes %s\n",argv[4]);
        return 0;
    }
    if ((fp_data=fopen(argv[6], "a"))==NULL) {
        fprintf(stderr, "No file resultados %s\n",argv[6]);
        return 0;
    }
    
    //Comienza el tiempo
//    start = clock();
    iteracion=1;
    gettimeofday(&t0, 0);
    
    inicializacion(argc,argv);

    fp_data=fopen(argv[1],"r");
    readData(fp_data, nData,nFeatures, datos);
    fclose(fp_data);

    fp_centroides=fopen(argv[4],"r");
    readData(fp_centroides, nCentroides, nFeatures, centroides);

    centroidesNuevos=centroides;

    fclose(fp_centroides);
    

    
//    for (int i=0; i<nCentroides; i++) {
//        
//        
//        for (int j=0; j<nFeatures; j++) {
//            printf("%lf ",centroides[i][j]);
//        }
//        printf("\n");
//    }
//    
//    for (int i=0; i<nCentroides; i++) {
//        
//        printf("Vacios");
//        for (int j=0; j<nFeatures; j++) {
//            printf("%lf ",centroidesVacio[i][j]);
//        }
//        printf("\n");
//    }


    
    asignacion(datos,centroides,nData,nFeatures,nCentroides);
//    for (int i=0; i<nCentroides; i++) {
//        
//        printf("Vacios :");
//        for (int j=0; j<nFeatures; j++) {
//            printf("%lf ",centroidesNuevos[i][j]);
//        }
//        printf("\n");
//    }
    recalculo(datos, centroides, nData, nFeatures, nCentroides);
//    for (int i=0; i<nCentroides; i++) {
//        
//        printf("Vacios :");
//        for (int j=0; j<nFeatures; j++) {
//            printf("%lf ",centroidesNuevos[i][j]);
//        }
//        printf("\n");
//    }

    
    
//    gettimeofday(&t1, 0);
//    elapsed = timedifference_msec(t0, t1);
//    printf("Iteracion %d, %f milliseconds.\n",iteracion, elapsed);
    
//    end = clock();
//    printf("Tiempo: %f\n", (double)(end - start));

    do {
//        gettimeofday(&t1, 0);
//        elapsed = timedifference_msec(t0, t1);
        
//        printf("Iteracion %d, %f milliseconds.\n",iteracion, elapsed);
//        end = clock();
//        printf("Tiempo: %f\n", (double)(end - start));
        asignacion(datos,centroidesNuevos,nData,nFeatures,nCentroides);
        recalculo(datos, centroidesNuevos, nData, nFeatures, nCentroides);
        iteracion++;
    }
    while(change==1);
//    end = clock();
//    printf("Tiempo final: %f\n", (double)((end - start)));
    
    gettimeofday(&t1, 0);
    
    //Error de agrupamiento
//    double error=calculoErrorCuadrado(nData, nCentroides, nFeatures, centroidesNuevos, datos);
//    printf("Error: %lf\n",error);
    double error=sumatoria_error(datos,nData,centroidesNuevos,nCentroides,nFeatures);
    
    //Numero de iteraciones
//    printf("Iteraciones: %d\n",iteracion);
//    
//    //Tiempo en milisegundos
    elapsed = timedifference_msec(t0, t1);
//    printf("%lf milliseconds.\n", elapsed);
    
//    for (int i=0; i<nCentroides; i++) {
//        
//        printf("Vacios :");
//        for (int j=0; j<nFeatures; j++) {
//            printf("%lf ",centroidesNuevos[i][j]);
//        }
//        printf("\n");
//    }


    fp_resultados = fopen(argv[6],"a");
    fprintf(fp_resultados,"%.16g\t %lf\t %d\n", elapsed, error, iteracion); //Imprime en el archivo [Tiempo Error Iteracion]
    fclose(fp_resultados);
   

    return 0;
}

void readData(FILE *data, int Ndatos,int Nfeatures,double **datos){
    int i;
    int j;
    
    char palabra[30];
    
    
    for (i=0;i<Ndatos; i++) {
        for (j=0; j<Nfeatures; j++) {
            fscanf(data, "%s",palabra);
            datos[i][j]=atof(palabra);
        }
    }
}

void inicializacion(int argc, char *argv[]){
    

    nData=atoi(argv[2]);
    nFeatures=atoi(argv[3]);
    
    nCentroides=atoi(argv[5]);
    
    datos=(double **) malloc(nData*sizeof(double *));
    
    int i;
    for(i=0;i<nData;i++){
        datos[i]=(double *)malloc((nFeatures+1)*sizeof(double));
    }
    
    
    centroides=(double **) malloc(nCentroides*sizeof(double *));
    for(i=0;i<nCentroides;i++){
        centroides[i]=(double *)malloc(nFeatures*sizeof(double));
    }
    
    centroidesNuevos=(double **) malloc(nCentroides*sizeof(double *));
    for(i=0;i<nCentroides;i++){
        centroidesNuevos[i]=(double *)malloc(nFeatures*sizeof(double));
    }
    
}

void asignacion(double **datos,double **centroides,int nData, int nFeatures, int nCentroides){
    
    int i,j,k,group;
    double distancia;

    
        for (i=0; i<nData; i++) {
            

            datos[i][nFeatures]=pow(10, 8);
            group=0;

            for (k=0; k<nCentroides; k++) {
                
                distancia=0;
                
                for (j=0; j<nFeatures; j++) {

                    distancia+=pow(datos[i][j]-centroides[k][j], 2);
                }
                distancia=sqrt(distancia);
                if (distancia<datos[i][nFeatures]){
                    
                    datos[i][nFeatures]=distancia;
                    group=(k+1);
                }
                
//                printf("Distancia [%d][%d] =%lf\n",i,j,distancia);
//                printf("Group: %d\n",group);
                if (k==nCentroides-1) {
             //       printf("Asigno: %d\n",group);
                    datos[i][nFeatures]=group;
//                    printf("datosTres [%d][%d]: %lf\n",i,j,datos[i][nFeatures]);
//                    printf("Termino una fila\n");
                }
            }
    }
    
//    for (int i=0; i<nData; i++) {
//        
//        
//        for (int j=0; j<nFeatures+1; j++) {
//            printf("%lf ",datos[i][j]);
//        }
//        printf("\n");
//    }
}

void recalculo(double **datos, double **centroides,int nData, int nFeatures, int nCentroides){
    
    int sum;
    int size;
    change=0;
    int i,j,k;
    for (k=1; k<=nCentroides; k++) {
        for (j=0; j<nFeatures; j++) {
            sum=0;
            size=0;
 
            for (i=0; i<nData; i++) {
                if ((int)datos[i][nFeatures]==k) {
                    sum+=datos[i][j];
                    size++;
                }
            }
            double centroide=((double)sum/(double)size);
//            printf("Sum: %d\n",sum);
//            printf("Size: %d\n",size);

            if ((double)centroidesNuevos[k-1][j]!=centroide&&isnan(centroide)==0) {
//                printf("Hubo cambio: %lf",centroidesNuevos[k-1][j]);
//                printf("Hubo cambio: %lf\n ",((double)sum/(double)size));
                change=1;
                
                centroidesNuevos[k-1][j]=((double)sum/(double)size);
            }
            //Colocar centroides default cuando no cambia
        
        }
    }
}

double sumatoria_error(double **datos,int nData,double **centroides,int nCentroides, int nFeatures ){
    int i,j,k;
    double error=0;
    
    for (k=1; k<=nCentroides; k++) {
        for (i=0; i<nData; i++) {
            double distancia=0;
            for (j=0; j<nFeatures; j++) {
                if (datos[i][nFeatures]==k) {
                    distancia+=pow(datos[i][j]-centroides[k-1][j], 2);
                }
            }
            error+=sqrt(distancia);
        }
    }
    return error;
}

//double calculoErrorCuadrado(int Nobjetos, int Ncentros, int Ncaracteristicas, double **centroides, double **datos){
//    
//    int i,j,k;
//    double sum, error = 0;
//    
//    for (i = 0; i < Ncentros; i++){ //Numero de objetos
//        
//        for (j = 0; j < Nobjetos; j++){ //Numero de centroides
//            sum = 0;
//            
//            if(datos[j][Ncaracteristicas] == (i+1)){
//                
//                for (k = 0; k < Ncaracteristicas; k++){ //Numero de caracteristicas
//                    sum+= pow( centroides[i][k] - datos[j][k] , 2);
//                }
//                error += sqrt(sum);
//                
//            }
//        }
//        
//    }
//    return error;
//}


double timedifference_msec(struct timeval t0, struct timeval t1)
{
    return (t1.tv_sec - t0.tv_sec) * 1000.0f + (t1.tv_usec - t0.tv_usec) / 1000.0f;
}

