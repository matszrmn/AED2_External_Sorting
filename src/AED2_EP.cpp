//--------------------------------------------------------------
// NOME1: Matheus Mendes de Sant'Ana
//--------------------------------------------------------------

#include <stdio.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>
#include <ctype.h>


// ######### ESCREVA O NROUSP DO PRIMEIRO INTEGRANTE AQUI
char* nroUSP1() {
    return("8921666");
}



// ######### ESCREVA O NROUSP DO SEGUNDO INTEGRANTE AQUI (OU DEIXE COM ZERO)
char* nroUSP2() {
    return("0000000");
}



void ordenar(char *nomearq);


// esta eh a memoria util para ordenacao
// nao declare nenhuma variavel global alem desta
// e nao use vetores ou listas adicionais
int M[100];

int contarElem(FILE *arq) {
    fseek(arq,0,SEEK_SET);
    int k=0;
    while(!feof(arq)) {
         int i;
         fscanf(arq,"%d ",&i);
         k++;
    }
    return k;
}

int estruturar(FILE *arq,FILE *aux) {
    int k = contarElem(arq);
    fseek(arq,0,SEEK_SET);
    int blocos=0;
    
    int i;
    while(true) { //cria uma estrutura do modo: tamanho /x x x x tamanho /x x x /... no arquivo aux
        int cont = 0;
        while(k > 0 && cont < 100) {
             fscanf(arq,"%d ",&i);
             M[cont]=i;
             cont++;
             k--;
        }
        for(int j=0;j < cont;j++) { //selection sort
             for(int k=j;k < cont;k++) {
                  if(M[k] < M[j]) {
                       int auxx = M[j];
                       M[j]=M[k];
                       M[k]=auxx;
                  }
             }
        }
        fprintf(aux,"%d /",cont);
        for(int j=0;j < cont;j++) fprintf(aux,"%d ",M[j]);
        
        blocos++;
        
        //ou if(k<=0 && cont<100) /<=100
        if(k <= 0) break; //ja terminou o arquivo
    }
    return blocos;
}

void retirarRepetidos(FILE *temp,FILE *saida) {
     fseek(temp,0,SEEK_SET);
     
     int tam; //numero de inteiros
     int primeiro;
     fscanf(temp,"%d /",&tam);
     fscanf(temp,"%d ",&primeiro);
     
     if(tam==1) {
          fprintf(saida,"%d ",primeiro);
          return;           
     }
     int i=primeiro;
     int ultimo;
     fscanf(temp,"%d ",&ultimo);
     
     int cont = 2;
     fprintf(saida,"%d ",primeiro);
     while(cont <= tam) {
          if(ultimo != i) {
               fprintf(saida,"%d ",ultimo);
               i=ultimo;
               fscanf(temp,"%d ",&ultimo);
               cont++;
          }                
          else { //proximas duas posiçoes
               i=ultimo;
               fscanf(temp,"%d ",&ultimo);
               cont++;
          }
     }
}

//------------------------------------------
// O EP consiste em implementar esta funcao
//------------------------------------------
void ordenar(char *nomearq) {
    FILE *arq = fopen(nomearq,"r");
    FILE *saida = fopen("saida.txt","w");
    if(!arq) return; //nao existe
    
    fseek(arq,0,SEEK_SET);
    getc(arq);
    if(feof(arq)) return;
        
    FILE *aux = fopen("auxiliar.tmp","w+"); 
    int blocos = estruturar(arq,aux); //ordena as corridas e define o arquivo do modo X / Y Y Y X / Y Y Y ...
    
    fseek(aux,0,SEEK_SET); 
    
    int tam;
    fscanf(aux,"%d /",&tam); //tamanho do primeiro bloco de inteiros
    int cont = 1;
    int inteiro;
    
    while(cont <= tam) { //primeiro bloco de inteiros
        fscanf(aux,"%d ",&inteiro);
        M[cont-1]=inteiro;
        cont++;
    }
    
    getc(aux);
    if(feof(aux)) { //arquivo tem tamanho < = 100
         FILE *temp = fopen("temporario.tmp","w+");
         fprintf(temp,"%i /",tam);
         int k=0; 
         while(k < tam-1) {
              fprintf(temp,"%i ",M[k]);   
              k++;
         }
         fprintf(temp,"%i ",M[k]);
                                
         retirarRepetidos(temp,saida);       
         fclose(temp);
         return;
    }
    fseek(aux,-1,SEEK_CUR); //volta na posicao anterior
    
    ///////////////////////////////////////////////////////////
    cont = 1;
    fscanf(aux,"%d /",&tam); //tamanho do segundo bloco de inteiros
    while(cont <= tam) {     //segundo bloco de inteiros passados para o vetor
         fscanf(aux,"%d ",&inteiro);
         M[cont-1] = inteiro;
         cont++;
    }
        
    fseek(aux,0,SEEK_SET);
    FILE *sec = fopen("auxiliar2.tmp","w+");
    
    int tam1;
    int tam2 = tam; //tamanho do segundo bloco(vetor)
    fscanf(aux,"%d /",&tam1); //tamanho do primeiro bloco
    
    int cont1=1;
    int cont2=1;
    
    int inteiro1;
    fscanf(aux,"%d ",&inteiro1);
    fprintf(sec,"%d /",tam1+tam2);
    while(cont1 <= tam1 && cont2 <= tam2) { //merge sort entre inteiros do vetor e inteiros do arquivo aux
         if(inteiro1 <= M[cont2-1]) {
              fprintf(sec,"%d ",inteiro1);
              if(cont1+1 <= tam1) fscanf(aux,"%d ",&inteiro1);
              cont1++;
         }
         else {
              fprintf(sec,"%d ",M[cont2-1]);
              cont2++;
         }
    } 
    
    if(cont1 > tam1) { //inteiros restantes do merge sort (vetor)
         while(cont2 <= tam2) {    
              fprintf(sec,"%d ",M[cont2-1]);
              cont2++;
         }
    }
    else if(cont2 > tam2) { //inteiros restantes do merge sort (arquivo)
         while(cont1 <= tam1) {
              fprintf(sec,"%d ",inteiro1);
              if(cont1+1 <= tam1) fscanf(aux,"%d ",&inteiro1);
              cont1++;   
         }
    }
    
    cont1 = 1;
    fscanf(aux,"%d /",&inteiro1);
    while(cont1 <= tam2) { //loop com o simples objetivo de chegar no terceiro bloco
         fscanf(aux,"%d ",&inteiro1);
         cont1++;            
    }
    
    int blocs = 2;
    int op =0;

    FILE *penult = fopen("penult.tmp","w+");
    while(blocs < blocos) {
         op++;
         blocs++;
         fseek(sec,0,SEEK_SET);
         
         FILE* aux3 = fopen("auxiliar3.tmp","w+");
         
         cont1=1;
         fscanf(aux,"%d /",&tam2); 
         while(cont1 <= tam2) {
              fscanf(aux,"%d ",&inteiro1);
              M[cont1-1] = inteiro1;
              cont1++;
         }
                 
         fscanf(sec,"%d /",&tam1);
         fscanf(sec,"%d ",&inteiro1);
         if(blocs == blocos) {
              cont1=1;
              cont2=1;
              fprintf(penult,"%d /",tam1+tam2);
              
              while(cont1 <= tam1 && cont2 <= tam2) { //merge sort
                   if(inteiro1 <= M[cont2-1]) {
                        fprintf(penult,"%d ",inteiro1);
                        if(cont1+1 <= tam1) fscanf(sec,"%d ",&inteiro1);
                        cont1++;
                   }
                   else {
                        fprintf(penult,"%d ",M[cont2-1]);
                        cont2++;
                   }
              } 
              
              if(cont1 > tam1) { //inteiros restantes do merge sort (vetor)
                   while(cont2 <= tam2) {    
                        fprintf(penult,"%d ",M[cont2-1]);
                        cont2++;
                   }
              }
              else if(cont2 > tam2) {
                   while(cont1 <= tam1) { //inteiros restantes do merge sort (arquivo)
                        fprintf(penult,"%d ",inteiro1);
                        if(cont1+1 <= tam1) fscanf(sec,"%d ",&inteiro1);
                        cont1++;   
                   }
              }
              break;
         }   
         cont1=1;
         cont2=1;
         
         fprintf(aux3,"%d /",tam1+tam2);
         while(cont1 <= tam1 && cont2 <= tam2) {
              if(inteiro1 <= M[cont2-1]) {
                   fprintf(aux3,"%d ",inteiro1);
                   if(cont1+1 <= tam1) fscanf(sec,"%d ",&inteiro1);
                   cont1++;
              }
              else {
                   fprintf(aux3,"%d ",M[cont2-1]);
                   cont2++;
              }
         } 
              
         if(cont1 > tam1) { //inteiros restantes do merge sort (vetor)
              while(cont2 <= tam2) {    
                   fprintf(aux3,"%d ",M[cont2-1]);
                   cont2++;
              }
         }
         else if(cont2 > tam2) { //inteiros restantes do merge sort (arquivo)
              while(cont1 <= tam1) {
                   fprintf(aux3,"%d ",inteiro1);
                   if(cont1+1 <= tam1) fscanf(sec,"%d ",&inteiro1);
                   cont1++;   
              }
         }        
         fclose(sec);
         sec = fopen("auxiliar2.tmp","w+"); //sobrescrevendo o arquivo auxiliar
         fseek(aux3,0,SEEK_SET);
         
         fscanf(aux3,"%d /",&tam);
         cont=1;
         fprintf(sec,"%d /",tam);
         while(cont <= tam) { //sobrescrevendo o arquivo auxiliar com dados do arquivo aux3
              fscanf(aux3,"%d ",&inteiro1);      
              fprintf(sec,"%d ",inteiro1);      
              cont++;
         }
         
         fclose(aux3);
    }
    if(op==0) { //dois blocos apenas
         fseek(sec,0,SEEK_SET);          
         fscanf(sec,"%d /",&tam);
         
         fprintf(penult,"%d /",tam);
         cont=1;
         while(cont <= tam) {
              fscanf(sec,"%d ",&inteiro1);
              fprintf(penult,"%d ",inteiro1);
              cont++;     
         }
         retirarRepetidos(penult,saida);
    }
    else { //mais de dois blocos
         retirarRepetidos(penult,saida);
    }
    
    fclose(sec);
    fclose(penult);
    fclose(saida); 
    fclose(arq);
    fclose(aux);
}

//---------------------------------------------------------
// use main() para fazer chamadas de teste ao seu programa
//---------------------------------------------------------
int main() {
}
