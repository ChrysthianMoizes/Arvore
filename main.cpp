#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iostream>
#include <ctype.h>
#include <windows.h>

using namespace std;

typedef struct tipoArvore {
	char nome[50];
	int pagina;
	tipoArvore *esquerda, *direita;
}TArvore;

typedef struct tipoElemento {
	char nome[50];
	int matricula;
	float salario;
	char genero[1];
	tipoElemento *prox;
}TElemento;

typedef struct tipoPagina {
	TElemento *inicio, *fim;
	int pagina;
	tipoPagina *prox;
}TPagina;

TArvore *raiz;

int menu();
void inicializaRaiz(TArvore **raiz);
void inicializaPagina(TPagina *Pag, TPagina *Pag1, TPagina *Pag2, TPagina *Pag3, TPagina *Pag4);
void verificaArquivo(FILE *arquivo, TPagina *Pag);
void lerArquivo(FILE *arquivo, TPagina *Pag);
void insereFuncionario(TPagina *Pag, TElemento *novo, int pagina);
void identificaRaiz(TPagina *pagina, TArvore **raiz);
void insereRaiz(TArvore **raiz, TElemento *atual, TPagina *pag);
void insereRecursivo(TArvore *atual, TArvore *novo);
void consultarPagina(TArvore *raiz);
void consultarFuncionario(TArvore *raiz, TPagina *pag);
void buscarFuncionario(TArvore *raiz, char *nome, TPagina *pag);
void printarFuncionario(int np, char *nome, TPagina *pag);
void printarPagina(TArvore *raiz, int pag);
void excluirFuncionario(TArvore *raiz);
TArvore *exclui(TArvore *raiz, char *nome, TArvore *pai);
void preOrdem(TArvore *raiz);
void emOrdem(TArvore *raiz);
void posOrdem(TArvore *raiz);

int main(int argc, char** argv) {
	
	FILE *arquivo = fopen("arquivo_arvore.txt", "r");
	int opcao;
	
	TPagina *Pag = (TPagina *)malloc(sizeof(TPagina));
	TPagina *Pag1 = (TPagina *)malloc(sizeof(TPagina));
	TPagina *Pag2 = (TPagina *)malloc(sizeof(TPagina));
	TPagina *Pag3 = (TPagina *)malloc(sizeof(TPagina));
	TPagina *Pag4 = (TPagina *)malloc(sizeof(TPagina));
	
	inicializaRaiz(&raiz);
	inicializaPagina(Pag, Pag1, Pag2, Pag3, Pag4);
	verificaArquivo(arquivo, Pag);
	identificaRaiz(Pag, &raiz);

	do{
		
		opcao = menu();
		
		switch(opcao){

			case 0: system("PAUSE"); exit(1);
			case 1: consultarPagina(raiz); break;
			case 2: consultarFuncionario(raiz, Pag); break;
			case 3: excluirFuncionario(raiz); break;
			case 4: printf("\n\n\t\t CAMINHAMENTO: Pre-Ordem\n\n"); preOrdem(raiz); printf("\n\n"); system("PAUSE"); system("CLS"); break;
			case 5: printf("\n\n\t\t CAMINHAMENTO: Em-Ordem\n\n"); emOrdem(raiz); printf("\n\n"); system("PAUSE"); system("CLS"); break;
			case 6: printf("\n\n\t\t CAMINHAMENTO: Pos-Ordem\n\n"); posOrdem(raiz); printf("\n\n"); system("PAUSE"); system("CLS"); break;
		}
	}while(opcao != 0);		
	
	return 0;
}

void inicializaRaiz(TArvore **raiz){
	*raiz = NULL;	
}

void inicializaPagina(TPagina *Pag, TPagina *Pag1, TPagina *Pag2, TPagina *Pag3, TPagina *Pag4){
	
	Pag->prox = Pag1;
	Pag1->prox = Pag2;
	Pag2->prox = Pag3;
	Pag3->prox = Pag4;
	Pag4->prox = NULL;
	
	TPagina *aux = Pag;
	
	for(int i = 0; i < 5; i++){
		aux->inicio = NULL;
		aux->fim = NULL;
		aux->pagina = i;
		aux= aux->prox;		
	}
}

int menu(){
	
	int opcao;
	
	printf("\n\n\t===========================| MENU |==============================\n\n\n");
	printf("\t 0 - SAIR \n\n");
	printf("\t 1 - CONSULTAR PAGINA \n\n");
	printf("\t 2 - CONSULTAR FUNCIONARIO \n\n");
	printf("\t 3 - EXCLUIR FUNCIONARIO \n\n");
	printf("\t 4 - PRE-ORDEM \n\n");
	printf("\t 5 - EM-ORDEM \n\n");
	printf("\t 6 - POS-ORDEM \n\n");

	do{
		printf("\n\n\t\t Informe a OPCAO desejada: ");
		scanf("%d", &opcao);
		
		if((opcao > 6) || (opcao < 0)){
			
			printf("\n\n\t Opcao Invalida! \n\n");
		}
		
	}while((opcao > 6) || (opcao < 0));	
	
	return opcao;
}

void verificaArquivo(FILE *arquivo, TPagina *Pag){
	
	printf("\n\n\t=================================================================\n\n\n");
	if(arquivo == NULL){
		
		printf("\n\n\t\t\t Arquivo Corrompido!\n\n");
		
	}else{
		printf("\n\n\t\t\t Arquivo Aberto com Sucesso!\n\n");
		lerArquivo(arquivo, Pag);	
	}
}

void lerArquivo(FILE *arquivo, TPagina *Pag){
	
	int i = 0;
	
	char registro[200];
	char *tok;
	
	while(fgets(registro, sizeof(registro), arquivo) != NULL){
		
		TElemento *novo = (TElemento *)malloc(sizeof(TElemento));
		novo->prox = NULL;
		
		tok = strtok(registro, ";");
		
		while(tok != NULL){
			
			if(i == 0){
				novo->matricula = atoi(tok);
			}else if(i == 1){
				strcpy(novo->nome, tok);
			}else if(i == 2){
				novo->salario = atof(tok);
			}else if(i == 3){ 
				strcpy(novo->genero, tok);
			}
			
			i++;
			tok = strtok(NULL, ";");
		}
		
		if((novo->nome[0] >= 'A')&&(novo->nome[0] <= 'E')){
			insereFuncionario(Pag, novo, 0);
			
		}else if((novo->nome[0] >= 'F')&&(novo->nome[0] <= 'J')){
			insereFuncionario(Pag, novo, 1);
		
		}else if((novo->nome[0] >= 'K')&&(novo->nome[0] <= 'O')){
			insereFuncionario(Pag, novo, 2);
		
		}else if((novo->nome[0] >= 'P')&&(novo->nome[0] <= 'T')){
			insereFuncionario(Pag, novo, 3);
	
		}else if((novo->nome[0] >= 'U')&&(novo->nome[0] <= 'Z')){
			insereFuncionario(Pag, novo, 4);
		}	
		i = 0;
	}
}

void insereFuncionario(TPagina *Pag, TElemento *novo, int p){
	
	TPagina *pagina = Pag;
	TElemento *anterior, *atual;	
	int inserido = 0;
	
	novo->prox == NULL;
	
	for(int i = 0; i < p; i++){
		pagina = pagina->prox;
	}
	
	if (pagina->inicio == NULL){
		
		pagina->inicio = novo;
		pagina->fim = novo;
		
	} else {
	    atual = pagina->inicio;
	    anterior = NULL;
	    while(atual != NULL){
	    	if(strcmp(atual->nome,novo->nome) == 1){
	    		inserido = 1;
				novo->prox = atual;
	    		
				if(atual == pagina->inicio){
	    		   pagina->inicio = novo; 	
	    		   
	    		}else{
				   anterior->prox = novo;
				}
				break;
	    	}
			anterior = atual;
			atual = atual->prox;	    	
		}
		if (inserido == 0){
			anterior->prox = novo;
			pagina->fim = novo;
		}	
	}
}

void identificaRaiz(TPagina *pagina, TArvore **raiz){
	
	TPagina *pag = pagina;
	TElemento *atual = pag->inicio;
	
	while(atual != NULL){
		insereRaiz(raiz, atual, pag);
		atual = atual->prox;
	}
	
	if(pag->prox == NULL){
		printf("\n\n\t=================================================================\n\n\n");
		printf("\n\t\t\t\t Arvore Montada!\n\n");
		
	}else{
		
		if(atual == NULL){
			pag = pag->prox;
			atual = pag->inicio;
			identificaRaiz(pag, raiz);	
		}	
	}	
}

void insereRaiz(TArvore **raiz, TElemento *atual, TPagina *pag){
	
	TArvore *novo = (TArvore *)malloc(sizeof(TArvore));
	
	strcpy(novo->nome, atual->nome);
	novo->pagina = pag->pagina;
	novo->direita = NULL;
	novo->esquerda = NULL;
	
	if (*raiz == NULL){
		*raiz = novo;
	}else{
		 insereRecursivo(*raiz, novo);
	}
}

void insereRecursivo(TArvore *atual, TArvore *novo){
	
	if(strcmp(novo->nome, atual->nome)== 1){
		
		if(atual->esquerda == NULL){
			
			atual->esquerda = novo;
			
		}else{
			insereRecursivo(atual->esquerda, novo);
		}
		
	}else{
		
		if(atual->direita == NULL){
			atual->direita = novo;
			
		}else{
			insereRecursivo(atual->direita, novo);
		}
	}
}

void consultarPagina(TArvore *raiz){

	int p;
	
	printf("\n\n\t Insira o numero da Pagina: ");
	scanf("%d", &p);
	printf("\n\n\t\t Pagina %d:\n", p);
	printarPagina(raiz, p);
	
	printf("\n\n");
	system("PAUSE");
	system("CLS");	
}

void printarPagina(TArvore *raiz, int pag){
	
	if(raiz != NULL){
		if(raiz->pagina == pag){
			printf("\n\t%s", raiz->nome);
		}
		printarPagina(raiz->esquerda, pag);
		printarPagina(raiz->direita, pag);	
	}
}

void consultarFuncionario(TArvore *raiz, TPagina *pag){
	
	char nome[50];
	
	printf("\n\n\t Insira o nome do Funcionario: ");
	fflush(stdin);
	gets(nome);
	
	buscarFuncionario(raiz, nome, pag);
	system("PAUSE");
	system("CLS");
}

void buscarFuncionario(TArvore *raiz, char *nome, TPagina *pag){
	
	if (raiz != NULL){
		
		if (strcmp(raiz->nome, nome)== 0){
			printf("\n\t\t\t<<<ENCONTRADO>>>\n");
			printarFuncionario(raiz->pagina, raiz->nome, pag);
			
		}else{
			
			if(strcmp(raiz->nome, nome)== 1){
				buscarFuncionario(raiz->direita, nome, pag);
				
			}else{
				buscarFuncionario(raiz->esquerda, nome, pag);				
			}
		}
	}
}

void printarFuncionario(int np, char *nome, TPagina *pag){
	
	TPagina *pagina = pag;
	int cont = 0;
	
	for(int i = 0; i < np; i++){
		pagina = pagina->prox;
	}
	
	TElemento *atual = pagina->inicio;
	
	while((cont != 1)||(atual != NULL)){
		
		if(strcmp(atual->nome, nome)== 0){
			
			printf("\n\n\t\t Matricula: %d\n\t\t Nome: %s\n\t\t Salario: %.2f\n\t\t Genero: %s \n\n", atual->matricula, atual->nome, atual->salario, atual->genero);
			cont = 1;
		}
		atual = atual->prox;
	}	
	
	if(cont == 0){
		printf("\n\n\t\t Funcionario nao encontrado!\n\n");
	}
}

void excluirFuncionario(TArvore *raiz){
	
	char nome[50];
	
	printf("\n\n\t Digite o nome do Funcionario: ");
	fflush(stdin);
	gets(nome);
	
	TArvore *aux = exclui(raiz, nome, NULL);
	
	if(aux == NULL){
		printf("\n\n\t Funcionario nao encontrado!\n\n");
	}else{
		printf("\n\n\t Funcionario Excluido Com Sucesso!\n\n");
	}
	
	system("PAUSE");
	system("CLS");
}

TArvore *exclui(TArvore *raiz, char *nome, TArvore *pai){
	
    if (raiz == NULL){
    	return NULL;
    	
    } else if (strcmp(raiz->nome, nome) == 1){
		exclui(raiz->direita, nome, raiz);
    	
	} else if (strcmp(raiz->nome, nome) == -1){
		exclui(raiz->esquerda, nome, raiz);
    } else {
    	
    	//Encontrado Nodo a ser eliminado
    	printf("\n\t\(%s)RAIZ ESQUERDA / (%s)RAIZ DIREITA ...",raiz->esquerda->nome, raiz->direita->nome);
    	
    	if((raiz->esquerda == NULL) && (raiz->direita == NULL)){
    		//Nó (a ser excluído) SEM FILHOS...
			free(raiz);
    		raiz = NULL;
    	} else if (raiz->esquerda == NULL){
    		//Nó (a ser excluído) tem apena UM FILHO à direita...
    		TArvore *temp = raiz;
    		raiz = raiz->direita;
    		pai->direita = raiz;
    		free(temp);
    	} else if (raiz->direita == NULL){
    		//Nó (a ser excluído) tem apena UM FILHO à esquerda ...
    		TArvore *temp = raiz;
    		raiz = raiz->esquerda;
    		pai->esquerda = raiz;
    		free(temp);
    	} else {
    		//Nó (a ser excluído) tem DOIS FILHOS ...
    		TArvore *filho = raiz->esquerda;
    		while (filho->direita != NULL){
    			filho = filho->direita;
    		}//while
    		strcpy(raiz->nome, filho->nome);
    		raiz->pagina = filho->pagina;
    		strcpy(filho->nome, nome);
    		exclui(raiz->esquerda, nome, raiz);
    	}
    }
    return raiz;
}

void preOrdem(TArvore *raiz){
	
	if(raiz != NULL){
		
		printf("\n\t%s", raiz->nome);
		preOrdem(raiz->esquerda);
		preOrdem(raiz->direita);
	}
}

void emOrdem(TArvore *raiz){
	
	if(raiz != NULL){
	
		emOrdem(raiz->esquerda);
		printf("\n\t%s", raiz->nome);
		emOrdem(raiz->direita);
	}
}

void posOrdem(TArvore *raiz){
	
	if(raiz != NULL){
	
		posOrdem(raiz->esquerda);
		posOrdem(raiz->direita);
		printf("\n\t%s", raiz->nome);
	}	
}


