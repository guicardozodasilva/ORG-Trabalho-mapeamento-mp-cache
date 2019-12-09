//-----------------assinaturas das funções-----------------//
int menu();
void limpaTela(void);
void iniciaValoresMemoriaPrincipal();
void iniciaValoresMemoriaCache();
void imprimeConteudoMemoriaPrincipal();
void imprimeConteudoMemoriaCache();
void exportarDadosParaMemoriaPrincipal(int index, char* string);
int lruEmAcao();
int lruSetaIndex(int index);
void verificaMemoriaCache(char* string, char* offset,int* miss, int* hit);
void importaDadosDaMemoriaPrincipal(int celula,int index,char* string);
int converteOffset(char* string);
void escreveDadoNaMemoriaCache(char* string, char* offset, int dado, int* miss, int* hit);
int stringParaInt(char* string);
int verificaSeNumeroEhBinario(char* string);

//-----------------struct's-----------------//
typedef struct {
	int adress;
	int block;
	int data;
}Mem_Principal;
Mem_Principal MP [128];

typedef struct {
	int index;
	int valid;
	char tag[5];
	int data[4];
	int dirty_bit;
	int lru;
}Mem_Cache;
 Mem_Cache cache  [8];


