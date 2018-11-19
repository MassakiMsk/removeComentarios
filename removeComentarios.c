#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef enum boolean {false, true} boolean;

typedef struct marcador {
    char abre[10];
    char fecha[10];
    boolean remover;
} marcador;

void geraMarcadores(marcador m[]) {
    int i = 0;

    strcpy(m[i].abre, "\"");
    strcpy(m[i].fecha, "\"");
    m[i].remover = false;
    i++;

    strcpy(m[i].abre, "\'");
    strcpy(m[i].fecha, "\'");
    m[i].remover = false;
    i++;

    strcpy(m[i].abre, "//");
    strcpy(m[i].fecha, "\n");
    m[i].remover = true;
    i++;

    strcpy(m[i].abre, "/*");
    strcpy(m[i].fecha, "*/");
    m[i].remover = true;
    i++;

    strcpy(m[i].abre, "<!--");
    strcpy(m[i].fecha, "-->");
    m[i].remover = true;
    i++;
}

long buscaFimMarcador(char *strInput, long index, long size, marcador m) {
    int j;

    for(; index < size; index++) {
        if(strncmp((const char *)(strInput + index), m.fecha, strlen(m.fecha)) == 0) {
            return index + strlen(m.fecha);
        }
    }
    return index;
}

void buscaMarcadores(char *strInput, char *strOutput, marcador m[], int qntMarcador) {
    char aux;
    int i, j, k, tamMarcador;
    long index, indexOut;
    long size = strlen(strInput);
    for(indexOut = 0, index = 0; index < size; index++) {
        for(j = 0; j < qntMarcador; j++) {
            if(strncmp((const char *)(strInput + index), m[j].abre, strlen(m[j].abre)) == 0) {
                index = buscaFimMarcador(strInput, index + strlen(m[j].abre), size, m[j]);
                if(strncmp("\n", m[j].fecha, 1) == 0) {
                    *(strOutput + indexOut++) = '\n';
                }
            }
        }
        if(j == qntMarcador) {
            *(strOutput + indexOut++) = strInput[index];
        }
    }
}

void leArquivo(char **strInput, char **strOutput) {
    long size;
    FILE *input = NULL;

    input = fopen("input.txt", "r");

    fseek(input, 0, SEEK_END);
    size = ftell(input);
    fseek(input, 0, SEEK_SET);  

    *strInput = malloc(size);
    *strOutput = malloc(size);

    fread(*strInput, sizeof(char), size, input);
    *(*strInput + size) = '\0';
    
    fclose(input);
}

void escreveArquivo(char *strOutput) {
    FILE *output = NULL;

    output = fopen("output.txt", "w");

    fwrite(strOutput, sizeof(char), strlen(strOutput), output);
    
    fclose(output);
}

int main() {
    char *strInput = NULL;
    char *strOutput = NULL;
    char aux;
    int i;
    int qntMarcador = 5;
    marcador m[qntMarcador];

    geraMarcadores(m);
    leArquivo(&strInput, &strOutput);
    buscaMarcadores(strInput, strOutput, m, qntMarcador);
    escreveArquivo(strOutput);

    return 0;
}
