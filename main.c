#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "funcoes.h"

int main(){
 char entrada[7], index[6], offset[3];
 int opcao = 0, ender_mp[7], i, dado, miss = 0, hit = 0, geral = 0;

 iniciaValoresMemoriaCache();
 iniciaValoresMemoriaPrincipal();

//--------------------------------Menu--------------------------------//
do{
    limpaTela();

    if ((opcao < 0)||(opcao > 3))
        printf("\n\tOpcao invalida\n");

    opcao = menu(); //exibe menu
    getchar(); //le caracter do teclado

    if (opcao == 1){ //ler endereco da memoria
        limpaTela();

        while(1){
            printf("Informe um numero binario de 7 (sete) bits:\n");
            fgets(entrada, 8, stdin);
            if(strlen(entrada) < 7 || strlen(entrada) > 7){
                printf("Tamanho incorreto!\n");
            }else if(verificaSeNumeroEhBinario(entrada)){
                printf("Isso nao eh um numero binario, verifique! Dica: digite apenas 1 e 0.\n");
                getchar();
            }else{
                printf("Numero informado para leitura:   %s\n", entrada);
                break;
            }
            getchar();
            limpaTela();
        }

        offset[0] = entrada[5];
        offset[1] = entrada[6];
        offset[2] = '\0';
        index[0] = entrada[0];
        index[1] = entrada[1];
        index[2] = entrada[2];
        index[3] = entrada[3];
        index[4] = entrada[4];
        index[5] = '\0';

        verificaMemoriaCache(index,offset,&miss,&hit);		//ler da memoria
        }

	if (opcao == 2){      					//ESCREVER NA MEMÓRIA
		limpaTela();
		while(1){
            printf("informe um numero binario de 7 (sete) bits:\n");
			fgets(entrada, 8, stdin);

			if(strlen(entrada) < 7 || strlen(entrada) > 7){
				printf("Tamanho incorreto!\n");
			}else if(verificaSeNumeroEhBinario(entrada)){
				printf("Isso nao eh um numero binario, verifique! Dica: digite apenas 1 e 0.\n");
				getchar();
			}else{
				printf("Numero informado para leitura:   %s\n", entrada);
				break;
			}
			getchar();
			limpaTela();
		}

		printf("Digite um dado para escrever na memória:\n");
		scanf("%d",&dado);				//le conteudo para escrever na memoria
		getchar();

		offset[0] = entrada[5];
		offset[1] = entrada[6];
		offset[2] = '\0';
		index[0] = entrada[0];
		index[1] = entrada[1];
		index[2] = entrada[2];
		index[3] = entrada[3];
		index[4] = entrada[4];
		index[5] = '\0';

		escreveDadoNaMemoriaCache(index,offset,dado,&miss,&hit);
	}

	if (opcao == 3){
		limpaTela();
		printf("ESTATISTICAS:\n\n");
		printf("Total de acesso: %d\n",miss+hit);
		printf("Hits: %.2f%%\n", (float)hit/(hit+miss)*100);
		printf("Misses: %.2f%%\n",(float)miss/(hit+miss)*100);
		getchar();
	}

	if (opcao == 0){
		printf("Tchau!\n");
		exit(0);
	}

	}while((opcao>0)||(opcao<3));

	return 0;
}
