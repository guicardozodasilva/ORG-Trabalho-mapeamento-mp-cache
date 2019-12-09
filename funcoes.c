#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "funcoes.h"

int menu(){
	int opcao;
	imprimeConteudoMemoriaPrincipal();
	imprimeConteudoMemoriaCache();
	printf("PROXIMA POSICAO A SER SUBSTITUIDA: %d", lruEmAcao());
	printf("\n");
	printf("\t1 - Ler um endereco de memoria\n");
	printf("\t2 - Escrever na memoria\n" );
	printf("\t3 - Apresentar estatisticas\n");
	printf("\t0 - Sair\n");
	printf("\t    Selecione sua opcao:");
	scanf("%d", &opcao);

	return opcao;
}

void limpaTela(void)
{
    #if defined(linux)  || defined(LINUX) || defined(Linux) || defined(UNIX)
    printf("\e[H\e[2J");
    #elif defined WIN32
    system ("cls");
    #else
    printf("\e[H\e[2J");
    #endif
}

void iniciaValoresMemoriaPrincipal(){
	int i, j=0, dado = 1 ;
	for ( i = 0; i < 128; i++){
		MP[i].adress = i;
		MP[i].data = dado;
		if(((i%4) == 0)&&(i>3)){
			j++;
		}
		MP[i].block = j;
		dado++;
	}
}

void iniciaValoresMemoriaCache(){
	int i, j;
	for( i = 0; i <= 7; i++){
		cache[i].index = i;
		cache[i].valid = 0;
		for (j = 0; j <= 4; j++){
			cache[i].tag[j] = '0';
		}
		for (j = 0; j <= 4; j++){
			cache[i].data[j] = 0;
		}
		cache[i].dirty_bit = 0;
		cache[i].lru = 0;
	}
}

void imprimeConteudoMemoriaPrincipal(){
	int i, j;
	printf("\n");
	printf("Conteudo da Memoria Principal:\n");
	printf("__________________________________________________________________________________________________________\n");
	for(i=0;i<128;i++){
		if(i%4 == 0)
			printf("\nBLOCK => %d\t", MP[i].block);
		printf("\t%d\t", MP[i].data);
	}
	printf("\n");
}

void imprimeConteudoMemoriaCache(){
	int i, j, x;
	printf("\n\n");
	printf("Conteudo da Memoria Cache:\n");
	printf("_________________________________________________________________________________________________________________________________________________\n");
	printf("|\tindex\t|\tValid\t|\tTag\t|\tD0\t|\tD1\t|\tD2\t|\tD3\t|\tD Bit\t|\tLRU\t|\n");
	for (i = 0; i < 8; i++)	{
		printf("|\t%d\t|\t%d\t|", cache[i].index, cache[i].valid);//imprime index e valid
		printf("\t");
		for (j = 0; j < 5; j++){
			printf("%c", cache[i].tag[j] );
		}
		printf("\t|");
		for (j = 0; j < 4; j++){
			printf("\t");
			printf("%d", cache[i].data[j]);
			printf("\t|");
		}
		printf("\t%d\t|\t%d\t|",cache[i].dirty_bit, cache[i].lru);
		printf("\n");
	}
	printf("-------------------------------------------------------------------------------------------------------------------------------------------------\n");
}

void exportarDadosParaMemoriaPrincipal(int index, char* string){
	int celula;
	celula = 4 * stringParaInt(cache[index].tag);
	for(int i=0;i<4;i++){
		MP[celula+i].data = cache[index].data[i];
	}
 }

//--------------------retorna o index da cache mais inutil--------------------//
int lruEmAcao(){
	int maior = 0, index, i;
	for (i = 0; i < 8; i++){
		if(cache[i].valid == 0){
			index = i;
			return index;
		}else if(cache[i].lru > maior){
			maior = cache[i].lru;
			index = i;
		}
	}
	return index;
}

//--------------recebe o index acessado da cache que sera setado para 0. Os demais serão setados em 1--------------//
int lruSetaIndex(int index){
	int i;
	for (i = 0; i < 8; i++){
		if(index != i){
			if(cache[i].lru < 7)
				cache[i].lru++;
		}else{
			cache[i].lru = 0;
		}
	}
}

void verificaMemoriaCache(char* string, char* offset, int* miss, int* hit){      //faz leitura da memoria cache
	int index, bloco, celula, deslocamento;
	for(int i=0;i<8;i++){
		if(strcmp(string, cache[i].tag)==0 && cache[i].valid != 0 ){	//compara entrada do usuario com as tags da cache
			printf("\nHIT");
			getchar();
			getchar();
			*hit = *hit + 1;
			printf("Bloco: %d\n", stringParaInt(string));
			getchar();
			getchar();
			deslocamento = converteOffset(offset);						//transforma o OFFSET em um inteiro
			printf("\nDADO LIDO: %d", cache[i].data[deslocamento]);
			lruSetaIndex(i);							//politica LRU
			getchar();
			return;
		}
	}
	printf("\nMISS");
	getchar();
	getchar();
	*miss = *miss + 1;
	index = lruEmAcao();  		//retorna index da cache mais antigo de acordo com LRU
	bloco = stringParaInt(string);	//transforma entrada usuario em inteiro
	celula = bloco * 4;					//multiplica por 4 para achar a celula
	if(cache[index].dirty_bit == 1){	//se o bloco a ser substituido estiver sujo
		exportarDadosParaMemoriaPrincipal(index, string);		//grava dados da cache na Memoria Principal
	}
	importaDadosDaMemoriaPrincipal(celula, index, string);		//importa dados da MP para cache
	printf("Bloco: %d\n", stringParaInt(string));
	getchar();
	getchar();
	printf("ELEMENTO IMPORTADO DA MP PARA A CACHE!");
	getchar();
	getchar();
}

void importaDadosDaMemoriaPrincipal(int celula, int index, char* string){ //importa dados da MP
	cache[index].valid = 1;
	strcpy(cache[index].tag, string);
	for(int i = 0; i < 4; i++){
		cache[index].data[i] = MP[celula+i].data;
	}
	cache[index].dirty_bit = 0;
	lruSetaIndex(index);				//politica LRU
}

//----------recebe uma string binaria de char's (offset) e retorna um inteiro----------//
int converteOffset(char* string){
	int soma = 0;
	int cont = 0;
	for(int i = 1; i >= 0; i--){
		if(string[i] ==  '1'){
			soma+= pow(2,cont);
		}
		cont++;
	}
	return soma;
}

void escreveDadoNaMemoriaCache(char* string, char* offset, int dado, int* miss, int* hit){    //escreve dado na cache
	int index, bloco, celula, deslocamento;
	for(int i=0;i<8;i++){
		if(strcmp(string, cache[i].tag)==0 && cache[i].valid != 0 ){
			printf("\nHIT");
			getchar();
			getchar();
			*hit = *hit + 1;
			printf("Bloco: %d\n", stringParaInt(string));
			getchar();
			getchar();
			deslocamento = converteOffset(offset);					//transforma OFFSET em inteiro
			printf("\nDADO LIDO: %d", cache[i].data[deslocamento]);
			cache[i].data[deslocamento] = dado;						//escreve na cache o dado
			cache[i].dirty_bit = 1;									//seta dirty bit para 1
			lruSetaIndex(i);										//politica LRU
			getchar();
			return;
		}
	}
	printf("\nMISS");
	getchar();
	getchar();
	*miss = *miss + 1;
	index = lruEmAcao();  		//retorna index da cache mais antigo de acordo com LRU
	bloco = stringParaInt(string);	//coverte entrada do usuario em inteiro
	celula = bloco * 4;					//multiplica por 4
	if(cache[index].dirty_bit == 1){	// se dirty bit estir ligado, grava linha da cache na MP
		exportarDadosParaMemoriaPrincipal(index,string);		//grava na MP
	}
	importaDadosDaMemoriaPrincipal(celula,index,string);		//importa bloco da MP
	printf("Bloco: %d\n", stringParaInt(string));
	getchar();
	getchar();
	deslocamento = converteOffset(offset);		//converte entrada do usuario em int
	cache[index].data[deslocamento] = dado;		//grava na cache o dado
	cache[index].dirty_bit = 1;
	printf("ELEMENTO IMPORTADO DA MP PARA A CACHE!");
	getchar();
	getchar();
}

 //----------recebe uma string binaria de char's e retorna um inteiro----------//
int stringParaInt(char* string){
	int soma = 0;
	int cont = 0;
	for(int i = 4; i >= 0; i--){
		if(string[i] ==  '1'){
			soma+= pow(2,cont);
		}
		cont++;
	}
	return soma;
}

int verificaSeNumeroEhBinario(char* string){
	for(int i = 0; i < 7; i++){
		if(string[i] != '0' && string[i] != '1'){
			return 1;
		}
	}
	return 0;
}
