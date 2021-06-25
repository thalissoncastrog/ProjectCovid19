#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>
#include <ctype.h>
#define max 32 //COLUNAS DE OBITOS


typedef struct ne
{
    char date[11];       /* DATE = 00-00-0000\0 */
    char state[3];      /* STATE = XX\0 */
    int dead[max];      /* dead[0] = epidemiological_week_2019 , dead[1] = epidemiological_week_2020 */
    int altura;
    struct ne *esq;
    struct ne *dir;
}NE;


//STRUCT PARA FILA E TAMBÉM UTILIZADO EM PILHA
typedef struct node
{
    char date[11];       /* DATE = 00-00-0000\0 */
    char state[3];      /* STATE = XX\0 */
    int dead[max];      /* dead[0] = epidemiological_week_2019 , dead[1] = epidemiological_week_2020 */
	struct node * next;
}NODE;

typedef struct
{
	int size;            //NO CABEÇALHO
	NODE *INICIO;
	NODE *FIM;
}DESCRITOR;
////////////////////////////

// STRUCTS PARA LISTA
typedef struct nodo
{
    int size;
	char date[11];  /* DATE = 00-00-0000\0 */
	int dead[max];   /* dead[0] = epidemiological_week_2019 , dead[1] = epidemiological_week_2020 */
	struct nodo *ant;         // Anterior
	struct nodo *prox;	      // Proximo
}NODO;

typedef struct no
{
    int size;
	char state[3]; /* STATE = XX\0 */
	struct nodo *BDdatas;
	struct no *ant;         // Anterior
	struct no *prox;	    // Proximo
}NO;
////////////////////////////

void maiusculo(char s1[], char s2[]);
void limparvetor(char*,int);
void lerArquivo ();
void menu();

typedef NO *LISTA_CIR_DUP_ENC_NC; // LISTA DOS ESTADOS
typedef NODO *LISTA_CIR_DUP_ENC_NC2; // LISTA DAS DATAS

typedef DESCRITOR *FILA_ENC; //FILA

typedef NODE *PILHA_ENC; //PILHA

typedef NE* ArvAVL; //ÁRVORE

// FUNÇÕES BÁSICAS DA LISTA DE ESTADOS
void cria_listaState(LISTA_CIR_DUP_ENC_NC *);
int eh_vaziaState(LISTA_CIR_DUP_ENC_NC);
int tamState(LISTA_CIR_DUP_ENC_NC);
void insState(LISTA_CIR_DUP_ENC_NC l, char v[], int k);
char* recupState(LISTA_CIR_DUP_ENC_NC, int);
void inserirAUX(LISTA_CIR_DUP_ENC_NC l, char e[], char d[], int o[]);
void retState(LISTA_CIR_DUP_ENC_NC, int);
void destruirState(LISTA_CIR_DUP_ENC_NC);
void inverter_listaState(LISTA_CIR_DUP_ENC_NC);
LISTA_CIR_DUP_ENC_NC2 recupBDdata(LISTA_CIR_DUP_ENC_NC, int );
////////////////////////////////////

//FUNÇÕES BÁSICAS DA LISTA DE DATAS
void cria_listaDates(LISTA_CIR_DUP_ENC_NC2 *);
int eh_vaziaDates(LISTA_CIR_DUP_ENC_NC2);
int tamDates(LISTA_CIR_DUP_ENC_NC2);
void insDates(LISTA_CIR_DUP_ENC_NC2 l, char d[], int m[], int k);
char* recupDates(LISTA_CIR_DUP_ENC_NC2, int);
int recupDates2(LISTA_CIR_DUP_ENC_NC2, int, int);
void retDates(LISTA_CIR_DUP_ENC_NC2, int);
void destruirDates(LISTA_CIR_DUP_ENC_NC2);
void inverter_listaDates(LISTA_CIR_DUP_ENC_NC2);
///////////////////////////////////

//FUNÇÕES BÁSICAS DA FILA
void cria_fila(FILA_ENC *);
int eh_vazia(FILA_ENC);
void ins(FILA_ENC f, char d[], char e[], int o[]);
char* consData(FILA_ENC f);
char* consEstado(FILA_ENC f);
int consObitos(FILA_ENC f, int indice);
int tam(FILA_ENC);
void ret(FILA_ENC );
int cons_retObitos(FILA_ENC, int);
char* cons_retData(FILA_ENC);
char* cons_retEstado(FILA_ENC);
void destruir(FILA_ENC);
///////////////////////////////////

//FUNÇÕES BÁSICAS DA PILHA
void create(PILHA_ENC *);
int is_empty(PILHA_ENC);        //EH_VAZIA.
void push(PILHA_ENC *pp, char d[], char e[], int o[]);   // Faz o empilhamento.
char* topData(PILHA_ENC );           // Verifica valor da data no topo da pilha.
char* topEstado(PILHA_ENC );         // Verifica valor do estado no topo da pilha.
int topObitos(PILHA_ENC, int);           // Verifica valor de um obito no topo da pilha.
void pop(PILHA_ENC *);         // Faz o desenpilhamento.
int top_popObitos(PILHA_ENC *, int);
char* top_popEstado(PILHA_ENC *);
char* top_popData(PILHA_ENC *);
void destroy(PILHA_ENC l);
///////////////////////////////////

//FUNÇÕES BÁSICAS DA ÁRVORE
ArvAVL* cria_ArvAVL();
int altura_NO(struct ne* );
void RotacaoLL(ArvAVL *);
int maior(int, int );
int fatorBalanceamento_NO(struct ne* );
void RotacaoRR(ArvAVL *);
void RotacaoLR(ArvAVL *);
void RotacaoRL(ArvAVL *);
int insere_ArvAVL(ArvAVL *raiz, char state[], char date[], int dead[]);
void inserir_elemento(ArvAVL *, FILA_ENC);
void precursorPreOrdem(ArvAVL arvore, int *pico, char *estado, char *data);
int infoObitos(ArvAVL, int);
char* infoData(ArvAVL);
char* infoState(ArvAVL);
ArvAVL right(ArvAVL);
ArvAVL left(ArvAVL);
void picoArvore(ArvAVL);
void DataEstadoArvore(ArvAVL arvore, char estado[], char data[]);
void precursorPreOrdemEstadoData(ArvAVL arvore, char estado[], char data []);
///////////////////////////////////

//FUNÇÕES DE BUSCA DA LISTA UTILIZADAS NO MENU DE OPÇÕES//
void BuscarLista(LISTA_CIR_DUP_ENC_NC l, char data[], char estado[]);
void BuscarLista2(LISTA_CIR_DUP_ENC_NC l);
///////////////////////////////////

//FUNÇÕES DE BUSCA DA FILA UTILIZADAS NO MENU DE OPÇÕES//
void DataEstado (FILA_ENC f, char estado[], char data[]);
void pico(FILA_ENC f);
///////////////////////////////////

void DataEstadoPilha(PILHA_ENC f, char estado[], char data[]);
void picoPilha(PILHA_ENC);

LISTA_CIR_DUP_ENC_NC LISTA;

FILA_ENC FILA;

PILHA_ENC PILHA;

ArvAVL *ARVORE;

int main()
{
    setlocale(LC_ALL, "Portuguese");
    lerArquivo ();
    menu();
    return 0;
}


// FUNÇÕES DA LISTA DE ESTADOS
void cria_listaState(LISTA_CIR_DUP_ENC_NC *pl)
{
	*pl = (LISTA_CIR_DUP_ENC_NC) malloc (sizeof(NO));

	if(!(*pl))
	{
		printf("\nERRO! Memória insuficiente!\n");
		exit(1);
	}
    (*pl)->BDdatas = NULL;
    (*pl)->size = 0;
	(*pl)->ant=(*pl)->prox=*pl;
}

int eh_vaziaState(LISTA_CIR_DUP_ENC_NC l)
{
	return(l->size == 0);
}

int tamState(LISTA_CIR_DUP_ENC_NC l)
{
	return(l->size);
}

void insState(LISTA_CIR_DUP_ENC_NC l, char v[], int k)
{
	LISTA_CIR_DUP_ENC_NC aux;
	NO *novo;
    LISTA_CIR_DUP_ENC_NC2 *ldates;

	if(k < 1 || k > l->size+1)
	{
		printf("\nERRO! PosiÃ§Ã£o inválida para inserção.\n");
		exit(2);
	}

	novo = (NO*)malloc (sizeof(NO));
	if(!novo)
	{
		printf("\nERRO! MemÃ³ria insuficiente!\n");
		exit(3);
	}

	ldates = (LISTA_CIR_DUP_ENC_NC2*) malloc (sizeof (LISTA_CIR_DUP_ENC_NC2));
	if(!ldates)
	{
		printf("\nERRO! MemÃ³ria insuficiente!\n");
		exit(4);
	}

    strcpy(novo->state, v);         // strcpy(string_destino, string_origem);
    cria_listaDates(ldates);
    novo->BDdatas = *ldates;

    if(k<=(l->size)/2)
    {
    	for(aux=l; k>1; aux=aux->prox, k--);
    	novo->prox = aux->prox;
    	novo->ant = aux;
    	aux->prox = novo;
    	novo->prox->ant = novo;
    }else{
    	for(aux=l; k<=l->size; aux=aux->ant, k++);
    	novo->ant = aux->ant;
    	novo->prox = aux;
    	aux->ant = novo;
    	novo->ant->prox = novo;
	}
	l->size++;
}


char* recupState(LISTA_CIR_DUP_ENC_NC l, int k)
{
	int tamanho = l->size;
	if(k < 1 || k > tamanho)
	{
		printf("\nERRO! Consulta invalida.");
		exit(5);
	}
	if(k <= tamanho/2)
		for(; k>0; k--)
			l=l->prox;
	else
		for(; k<=tamanho; k++)
			l = l->ant;
	return (l->state);
}

void inserirAUX(LISTA_CIR_DUP_ENC_NC l, char e[], char d[], int o[])
{
    int i;

    if(eh_vaziaState(l))
    {
        insState(l, e, 1);
        insDates(recupBDdata(l, 1), d, o, 1);
        return;
    }else
    {
        for (i=1; i<=l->size; i++)
        {
            if(!strcmp(e, recupState(l,i)))
            {
                insDates(recupBDdata(l,i), d, o, (tamDates(recupBDdata(l,i)))+1);
                return;
            }
        }

        insState(l, e, (l->size)+1);
        insDates(recupBDdata(l, l->size), d, o, ((recupBDdata(l, l->size))->size)+1);
    }
}

LISTA_CIR_DUP_ENC_NC2 recupBDdata(LISTA_CIR_DUP_ENC_NC l, int k)
{
	int tamanho = l->size;
	if(k < 1 || k > tamanho)
	{
		printf("\nERRO! Consulta invalida.");
		exit(6);
	}
	if(k <= tamanho/2)
		for(; k>0; k--)
			l=l->prox;
	else
		for(; k<=tamanho; k++)
			l = l->ant;
	return (l->BDdatas);
}

void retState(LISTA_CIR_DUP_ENC_NC l, int k)
{
	int tamanho = l->size;

	if(k < 1 || k > tamanho){
	printf("\nERRO! Posição inválida para retirada.\n");
	exit(5);
	}

	l->size--;
	if(k<=(l->size)/2)
		for(; k>0; k--, l=l->prox);
	else
		for(; k<=tamanho; k++, l=l->ant);
	l->ant->prox = l->prox;
	l->prox->ant = l->ant;
	free(l);
}

void destruirState(LISTA_CIR_DUP_ENC_NC l)
{
	LISTA_CIR_DUP_ENC_NC aux;
	int tam = l->size;

	do{
		aux = l;
		l = l->prox;
		free(aux);
	}while (tam--);
}

void inverter_listaState(LISTA_CIR_DUP_ENC_NC l)
{
	int i=tamState(l);

	if(i>1)
	{
		NO *aux;
		for(i++; i; l=l->ant, i--){
			aux = l->ant;
			l->ant = l->prox;
			l->prox = aux;
		}
	}
}

///////////////////////////////////////////////


//FUNÇÕES DA LISTA DE DATAS
void cria_listaDates(LISTA_CIR_DUP_ENC_NC2 *pl)
{
	*pl = (LISTA_CIR_DUP_ENC_NC2) malloc (sizeof(NODO));

	if(!(*pl))
	{
		printf("\nERRO! Memória insuficiente!\n");
		exit(7);
	}
    (*pl)->size = 0 ;
	(*pl)->ant=(*pl)->prox=*pl;
}

int eh_vaziaDates(LISTA_CIR_DUP_ENC_NC2 l)
{
	return(l->size == 0);
}

int tamDates(LISTA_CIR_DUP_ENC_NC2 l)
{
	return(l->size);
}

void insDates(LISTA_CIR_DUP_ENC_NC2 l, char d[], int m[], int k)
{

	LISTA_CIR_DUP_ENC_NC2 aux;
	NODO *novo;

	if(k < 1 || k > l->size+1)
	{
		printf("\nERRO! Posição inválida para inserção.\n");
		exit(8);
	}

	novo = (NODO*)malloc (sizeof(NODO));
	if(!novo)
	{
		printf("\nERRO! Memória insuficiente!\n");
		exit(9);
	}

    strcpy(novo->date, d);         // strcpy(string_destino, string_origem);;
    int i;

    for(i=0; i<32; i++)
        novo->dead[i] = m[i];

    if(k<=(l->size)/2)
    {
    	for(aux=l; k>1; aux=aux->prox, k--);
    	novo->prox = aux->prox;
    	novo->ant = aux;
    	aux->prox = novo;
    	novo->prox->ant = novo;
    }else{
    	for(aux=l; k<=l->size; aux=aux->ant, k++);
    	novo->ant = aux->ant;
    	novo->prox = aux;
    	aux->ant = novo;
    	novo->ant->prox = novo;
	}
	l->size++;
}

char* recupDates(LISTA_CIR_DUP_ENC_NC2 l, int k)
{
	int tamanho = l->size;
	if(k < 1 || k > tamanho)
	{
		printf("\nERRO! Consulta invalida.");
		exit(10);
	}
	if(k <= tamanho/2)
		for(; k>0; k--)
			l=l->prox;
	else
		for(; k<=tamanho; k++)
			l = l->ant;
	return (l->date);
}

int recupDates2(LISTA_CIR_DUP_ENC_NC2 l, int k, int indice)
{
	int tamanho = l->size;
	if(k < 1 || k > tamanho)
	{
		printf("\nERRO! Consulta invalida.");
		exit(11);
	}
	if(k <= tamanho/2)
		for(; k>0; k--)
			l=l->prox;
    else
		for(; k<=tamanho; k++)
			l = l->ant;
	return (l->dead [indice]);
}

void retDates(LISTA_CIR_DUP_ENC_NC2 l, int k)
{
	int tamanho = l->size;

	if(k < 1 || k > tamanho){
	printf("\nERRO! Posição inválida para retirada.\n");
	exit(5);
	}

	l->size--;
	if(k<=(l->size)/2)
		for(; k>0; k--, l=l->prox);
	else
		for(; k<=tamanho; k++, l=l->ant);
	l->ant->prox = l->prox;
	l->prox->ant = l->ant;
	free(l);
}

void destruirDates(LISTA_CIR_DUP_ENC_NC2 l)
{
	LISTA_CIR_DUP_ENC_NC2 aux;
	int tam = l->size;

	do{
		aux = l;
		l = l->prox;
		free(aux);
	}while (tam--);
}

void inverter_listaDates(LISTA_CIR_DUP_ENC_NC2 l)
{
	int i=tamDates(l);

	if(i>1)
	{
		NODO *aux;
		for(i++; i; l=l->ant, i--){
			aux = l->ant;
			l->ant = l->prox;
			l->prox = aux;
		}
	}
}
/////////////////////////////////////////////////


//FUNÇÕES DE BUSCA DA LISTA
void BuscarLista(LISTA_CIR_DUP_ENC_NC l, char data[], char estado[])
{
    int i,j;
    for (i=1; i<=l->size; i++)
    {
        if(!strcmp(estado, recupState(l,i)))
        {
            for (j=1; j<= tamDates(recupBDdata(l,i)); j++)
            {
                if(!strcmp(data, recupDates(recupBDdata(l,i),j)))
                {
                        printf("Data: %s\n", data);
                        printf("Estado: %s\n", estado);
                        printf("semana epidemiológica 2019: %i\n", recupDates2(recupBDdata(l,i),j, 0));
                        printf("semana epidemiológica 2020: %i\n", recupDates2(recupBDdata(l,i),j, 1));
                        printf("óbitos indeterminados 2019: %i\n", recupDates2(recupBDdata(l,i),j, 2));
                        printf("óbitos por insuficiência respiratória 2019: %i\n", recupDates2(recupBDdata(l,i),j, 3));
                        printf("outros óbitos 2019: %i\n", recupDates2(recupBDdata(l,i),j, 4));
                        printf("óbitos pneumonia 2019: %i\n", recupDates2(recupBDdata(l,i),j, 5));
                        printf("óbitos septicemia 2019: %i\n", recupDates2(recupBDdata(l,i),j, 6));
                        printf("óbitos sars 2019: %i\n", recupDates2(recupBDdata(l,i),j, 7));
                        printf("óbitos covid19: %i\n", recupDates2(recupBDdata(l,i),j, 8));
                        printf("óbitos indeterminados 2020: %i\n", recupDates2(recupBDdata(l,i),j, 9));
                        printf("óbitos insuficiência respiratória 2020: %i\n", recupDates2(recupBDdata(l,i),j, 10));
                        printf("outros óbitos 2020: %i\n", recupDates2(recupBDdata(l,i),j, 11));
                        printf("mortes por pneumonia 2020: %i\n", recupDates2(recupBDdata(l,i),j, 12));
                        printf("mortes septicemia 2020: %i\n", recupDates2(recupBDdata(l,i),j, 13));
                        printf("mortes sars 2020: %i\n", recupDates2(recupBDdata(l,i),j, 14));
                        printf("mortes totais 2019: %i\n", recupDates2(recupBDdata(l,i),j, 15));
                        printf("mortes totais 2020: %i\n", recupDates2(recupBDdata(l,i),j, 16));
                        printf("novas mortes indeterminadas 2019: %i\n", recupDates2(recupBDdata(l,i),j, 17));
                        printf("novas mortes por insuficiência respiratória 2019: %i\n", recupDates2(recupBDdata(l,i),j, 18));
                        printf("novas outras mortes 2019: %i\n", recupDates2(recupBDdata(l,i),j, 19));
                        printf("novas mortes por pneumonia 2019: %i\n", recupDates2(recupBDdata(l,i),j, 20));
                        printf("novas mortes septicemia 2019: %i\n", recupDates2(recupBDdata(l,i),j, 21));
                        printf("novas mortes sars 2019: %i\n", recupDates2(recupBDdata(l,i),j, 22));
                        printf("novas mortes covid19: %i\n", recupDates2(recupBDdata(l,i),j, 23));
                        printf("novas mortes indeterminadas em 2020: %i\n", recupDates2(recupBDdata(l,i),j, 24));
                        printf("novas mortes por insuficiência respiratória 2020: %i\n", recupDates2(recupBDdata(l,i),j, 25));
                        printf("novas outras mortes 2020: %i\n", recupDates2(recupBDdata(l,i),j, 26));
                        printf("novas mortes pneumonia 2020: %i\n", recupDates2(recupBDdata(l,i),j, 27));
                        printf("novas mortes septicemia 2020: %i\n", recupDates2(recupBDdata(l,i),j, 28));
                        printf("novas mortes sars 2020: %i\n", recupDates2(recupBDdata(l,i),j, 29));
                        printf("novas mortes totais 2019: %i\n", recupDates2(recupBDdata(l,i),j, 30));
                        printf("novas mortes totais 2020: %i\n", recupDates2(recupBDdata(l,i),j, 31));
                }
            }
        }
    }

}

void BuscarLista2(LISTA_CIR_DUP_ENC_NC l)
{
    int pico = 0, i, j;
    char data[11], estado [3];

    for (i=1; i<=l->size; i++)
    {
        for(j=1; j<= tamDates(recupBDdata(l,i)); j++)
        {
            if(recupDates2(recupBDdata(l,i),j , 23) > pico)
            {
                pico = recupDates2(recupBDdata(l,i),j , 23);
                strcpy(data, recupDates(recupBDdata(l,i), j));
                strcpy(estado, recupState(l, i));
            }
        }
    }

    printf("######PICO DE MORTES EM 2020######\n");
    printf("ESTADO: %s\n", estado);
    printf("DATA: %s\n", data);
    printf("QUANTIDADE DE OBITOS = %i\n", pico);

}
/////////////////////////////////////////////////

//FUNÇÕES BÁSICAS DA FILA
void cria_fila(FILA_ENC *pf)
{
	*pf=(DESCRITOR *)malloc(sizeof(DESCRITOR));
	if(!*pf){
		printf("\nERRO! Memoria insuficiente!\n");
		exit(12);
	}
	(*pf)->INICIO=(*pf)->FIM = NULL;
	(*pf)->size = 0;
}

int eh_vazia(FILA_ENC f)
{
	return (f->INICIO==NULL);
}

void ins(FILA_ENC f, char d[], char e[], int o[])
{

	NODE *novo;
	novo = (NODE *) malloc (sizeof(NODE));
	if(!novo)
	{
		printf("\nERRO! Memoria insuficiente!\n");
		exit(13);
	}
	strcpy(novo->date, d);
	strcpy(novo->state, e);

	int i;

	for(i=0; i < max; i++)
        novo->dead[i] = o[i];

	novo->next = NULL;
	if(eh_vazia(f))
		f->INICIO = novo;
	else
		f->FIM->next = novo;
	f->FIM = novo;
	f->size++;
}

char* consData(FILA_ENC f)
{
	if(!f->INICIO)
	{
		printf("\nERRO! Consultar  em fila vazia!\n");
		exit(14);
	}else
		return(f->INICIO->date);
}

char* consEstado(FILA_ENC f)
{
	if(!f->INICIO)
	{
		printf("\nERRO! Consultar  em fila vazia!\n");
		exit(15);
	}else
		return(f->INICIO->state);
}

int consObitos(FILA_ENC f, int indice)
{
	if(!f->INICIO)
	{
		printf("\nERRO! Consultar em fila vazia!\n");
		exit(16);
	}else
		return(f->INICIO->dead[indice]);
}

int tam(FILA_ENC f)
{
	return(f->size);
}

void ret(FILA_ENC f)
{
	if(!f->INICIO)
	{
		printf("\nERRO! Retirada em fila vazia!\n");
		exit(17);
	}else{
		NODE *aux=f->INICIO;
		f->INICIO = f->INICIO->next;
		if(!f->INICIO)
			f->FIM=NULL;
		free(aux);
		f->size--;       // Acrescentado
	}
}

int cons_retObitos(FILA_ENC f, int i)
{
		if(!f->INICIO){
		printf("\nERRO! Consultar e Retirada em fila vacia!\n");
		exit(4);
	}else{
		int v=f->INICIO->dead[i];
		NODE *aux=f->INICIO;
		f->INICIO = f->INICIO->next;
		if(!f->INICIO)
			f->FIM = NULL;
		free(aux);
		f->size--;         // Acrescentado
		return(v);
	}
}

char* cons_retData(FILA_ENC f)
{
		if(!f->INICIO){
		printf("\nERRO! Consultar e Retirada em fila vacia!\n");
		exit(4);
	}else{
		NODE *v; strcpy(v->date, f->INICIO->date);
		NODE *aux=f->INICIO;
		f->INICIO = f->INICIO->next;
		if(!f->INICIO)
			f->FIM = NULL;
		free(aux);
		f->size--;         // Acrescentado
		return(v->date);
	}
}

char* cons_retEstado(FILA_ENC f)
{
		if(!f->INICIO){
		printf("\nERRO! Consultar e Retirada em fila vacia!\n");
		exit(4);
	}else{
		NODE *v; strcpy(v->state, f->INICIO->state);
		NODE *aux=f->INICIO;
		f->INICIO = f->INICIO->next;
		if(!f->INICIO)
			f->FIM = NULL;
		free(aux);
		f->size--;         // Acrescentado
		return(v->state);
	}
}

void destruir(FILA_ENC f)
{
	NODE *aux;
	while(f->INICIO){
		aux=f->INICIO;
		f->INICIO=f->INICIO->next;
		free(aux);
	}
	free(f);
}

/////////////////////////////////////////////////

//FUNÇÕES DE BUSCA DA FILA
/* 1 - Efetuar uma busca pelos dados referentes a uma determinada data e um estado, apresentando os mesmos na saída padrão. */
void DataEstado(FILA_ENC f, char estado[], char data[])
{

 	while(f != NULL){
 		if(!strcmp(consEstado(f), estado) && !strcmp(consData(f), data))
 		{
                        printf("Data: %s\n", data);
                        printf("Estado: %s\n", estado);
                        printf("semana epidemiológica 2019: %i\n", consObitos(f, 0));
                        printf("semana epidemiológica 2020: %i\n", consObitos(f, 1));
                        printf("óbitos indeterminados 2019: %i\n", consObitos(f, 2));
                        printf("óbitos por insuficiência respiratória 2019: %i\n", consObitos(f, 3));
                        printf("outros óbitos 2019: %i\n", consObitos(f, 4));
                        printf("óbitos pneumonia 2019: %i\n", consObitos(f, 5));
                        printf("óbitos septicemia 2019: %i\n", consObitos(f, 6));
                        printf("óbitos sars 2019: %i\n", consObitos(f, 7));
                        printf("óbitos covid19: %i\n", consObitos(f, 8));
                        printf("óbitos indeterminados 2020: %i\n", consObitos(f, 9));
                        printf("óbitos insuficiência respiratória 2020: %i\n", consObitos(f, 10));
                        printf("outros óbitos 2020: %i\n", consObitos(f, 11));
                        printf("mortes por pneumonia 2020: %i\n", consObitos(f, 12));
                        printf("mortes septicemia 2020: %i\n", consObitos(f, 13));
                        printf("mortes sars 2020: %i\n", consObitos(f, 14));
                        printf("mortes totais 2019: %i\n", consObitos(f, 15));
                        printf("mortes totais 2020: %i\n", consObitos(f, 16));
                        printf("novas mortes indeterminadas 2019: %i\n", consObitos(f, 17));
                        printf("novas mortes por insuficiência respiratória 2019: %i\n", consObitos(f, 18));
                        printf("novas outras mortes 2019: %i\n", consObitos(f, 19));
                        printf("novas mortes por pneumonia 2019: %i\n", consObitos(f, 20));
                        printf("novas mortes septicemia 2019: %i\n", consObitos(f, 21));
                        printf("novas mortes sars 2019: %i\n", consObitos(f, 22));
                        printf("novas mortes covid19: %i\n", consObitos(f, 23));
                        printf("novas mortes indeterminadas em 2020: %i\n", consObitos(f, 24));
                        printf("novas mortes por insuficiência respiratória 2020: %i\n", consObitos(f, 25));
                        printf("novas outras mortes 2020: %i\n", consObitos(f, 26));
                        printf("novas mortes pneumonia 2020: %i\n", consObitos(f, 27));
                        printf("novas mortes septicemia 2020: %i\n", consObitos(f, 28));
                        printf("novas mortes sars 2020: %i\n", consObitos(f, 29));
                        printf("novas mortes totais 2019: %i\n", consObitos(f, 30));
                        printf("novas mortes totais 2020: %i\n", consObitos(f, 31));
 			return;
 		}else
		 	ret(f);
	}
}

 /* Efetuar a busca pela data e local de ocorrência do pico de óbitos por COVID19 no ano
de 2020, apresentando o mesmo na saída padrão.*/
void pico(FILA_ENC f)
{
	int pico = 0;
	char dia[11], lugar[3];

	while(!eh_vazia(f)){
		if(pico < consObitos(f, 23)){
			pico = consObitos(f, 23);
			strcpy(dia, consData(f));
			strcpy(lugar, consEstado(f));
		}else{
			ret(f);
		}
	}
	printf("######PICO DE MORTES EM 2020######\n");
    printf("ESTADO: %s\n", lugar);
    printf("DATA: %s\n", dia);
    printf("QUANTIDADE DE OBITOS = %i\n", pico);
}
/////////////////////////////////////////////////

//FUNÇÕES BÁSICAS DA PILHA
void create(PILHA_ENC *pp)
{
	*pp==NULL;
}

int is_empty(PILHA_ENC p)
{
	return(!p);
}

void push(PILHA_ENC *pp, char d[], char e[], int o[])
{
	NODE *new;
	new = (NODE*) malloc(sizeof(NODE));

	if(!new)
	{
		printf("\nERRO! Memoria insuficiente!\n");
		exit(18);
	}

	strcpy(new->date, d);
	strcpy(new->state, e);

	int i;

	for(i=0; i < max; i++)
        new->dead[i] = o[i];

	new->next = *pp;
	*pp = new;
}

char* topData(PILHA_ENC p)
{
	if(!p)
	{
		printf("\nERRO! Consulta em pilha vazia!\n");
		exit(19);
	}
	return(p->date);
}

char* topEstado(PILHA_ENC p)
{
	if(!p)
	{
		printf("\nERRO! Consulta em pilha vazia!\n");
		exit(20);
	}
	return(p->state);
}

int topObitos(PILHA_ENC p, int indice)
{
	if(!p)
	{
		printf("\nERRO! Consulta em pilha vazia!\n");
		exit(21);
	}
	return(p->dead[indice]);
}

void pop(PILHA_ENC *pp)
{
	if(!(*pp))
	{
		printf("\nERRO! Retirada em pilha vazia!\n");
		exit(22);
	}else{
		NODE *aux=*pp;
		*pp = (*pp)->next;
		free(aux);
	}
}

int top_popObitos(PILHA_ENC *pp, int i)
{
		if(!(*pp)){
		printf("\nERRO! Consulta e retirada em pilha vazia!\n");
		exit(23);
	}else{
		int v = (*pp)->dead[i];
		NODE *aux = *pp;
		*pp = (*pp)->next;
		free(aux);
		return(v);
	}
}

char* top_popEstado(PILHA_ENC *pp)
{

    if(!(*pp))
    {
		printf("\nERRO! Consulta e retirada em pilha vazia!\n");
		exit(24);
	}else{
		NODE *v; strcpy(v->state, (*pp)->state);
		NODE *aux = *pp;
		*pp = (*pp)->next;
		free(aux);
		return(v->state);
	}
}

char* top_popData(PILHA_ENC *pp)
{
    if(!(*pp))
    {
		printf("\nERRO! Consulta e retirada em pilha vazia!\n");
		exit(25);
	}else{
		NODE *v; strcpy(v->date, (*pp)->date);
		NODE *aux = *pp;
		*pp = (*pp)->next;
		free(aux);
		return(v->date);
	}
}

void destroy(PILHA_ENC l)
{
	PILHA_ENC aux;
	while(l){
		aux = l;
		l = l->next;
		free(aux);
	}
}

/////////////////////////////////////////////////

//FUNÇÕES DE BUSCA DA PILHA
/* 1 - Efetuar uma busca pelos dados referentes a uma determinada data e um estado, apresentando os mesmos na saída padrão. */
void DataEstadoPilha(PILHA_ENC f, char estado[], char data[])
{
 	while(f != NULL){
 		if(!strcmp(topEstado(f), estado) && !strcmp(topData(f), data))
 		{
                        printf("Data: %s\n", data);
                        printf("Estado: %s\n", estado);
                        printf("semana epidemiológica 2019: %i\n", topObitos(f, 0));
                        printf("semana epidemiológica 2020: %i\n", topObitos(f, 1));
                        printf("óbitos indeterminados 2019: %i\n", topObitos(f, 2));
                        printf("óbitos por insuficiência respiratória 2019: %i\n", topObitos(f, 3));
                        printf("outros óbitos 2019: %i\n", topObitos(f, 4));
                        printf("óbitos pneumonia 2019: %i\n", topObitos(f, 5));
                        printf("óbitos septicemia 2019: %i\n", topObitos(f, 6));
                        printf("óbitos sars 2019: %i\n", topObitos(f, 7));
                        printf("óbitos covid19: %i\n", topObitos(f, 8));
                        printf("óbitos indeterminados 2020: %i\n", topObitos(f, 9));
                        printf("óbitos insuficiência respiratória 2020: %i\n", topObitos(f, 10));
                        printf("outros óbitos 2020: %i\n", topObitos(f, 11));
                        printf("mortes por pneumonia 2020: %i\n", topObitos(f, 12));
                        printf("mortes septicemia 2020: %i\n", topObitos(f, 13));
                        printf("mortes sars 2020: %i\n", topObitos(f, 14));
                        printf("mortes totais 2019: %i\n", topObitos(f, 15));
                        printf("mortes totais 2020: %i\n", topObitos(f, 16));
                        printf("novas mortes indeterminadas 2019: %i\n", topObitos(f, 17));
                        printf("novas mortes por insuficiência respiratória 2019: %i\n", topObitos(f, 18));
                        printf("novas outras mortes 2019: %i\n", topObitos(f, 19));
                        printf("novas mortes por pneumonia 2019: %i\n", topObitos(f, 20));
                        printf("novas mortes septicemia 2019: %i\n", topObitos(f, 21));
                        printf("novas mortes sars 2019: %i\n", topObitos(f, 22));
                        printf("novas mortes covid19: %i\n", topObitos(f, 23));
                        printf("novas mortes indeterminadas em 2020: %i\n", topObitos(f, 24));
                        printf("novas mortes por insuficiência respiratória 2020: %i\n", topObitos(f, 25));
                        printf("novas outras mortes 2020: %i\n", topObitos(f, 26));
                        printf("novas mortes pneumonia 2020: %i\n", topObitos(f, 27));
                        printf("novas mortes septicemia 2020: %i\n", topObitos(f, 28));
                        printf("novas mortes sars 2020: %i\n", topObitos(f, 29));
                        printf("novas mortes totais 2019: %i\n", topObitos(f, 30));
                        printf("novas mortes totais 2020: %i\n", topObitos(f, 31));
 			return;
 		}else
		 	pop(&f);
	}
}

 /* Efetuar a busca pela data e local de ocorrência do pico de óbitos por COVID19 no ano
de 2020, apresentando o mesmo na saída padrão.*/
void picoPilha(PILHA_ENC f)
{
	int pico = 0;
	char dia[11], lugar[3];

	while(!is_empty(f))
	{
		if(pico < topObitos(f, 23))
		{
			pico = topObitos(f, 23);
			strcpy(dia, topData(f));
			strcpy(lugar, topEstado(f));
			pop(&f);
		}else
		{
			pop(&f);
		}
	}
	printf("######PICO DE MORTES EM 2020######\n");
    printf("ESTADO: %s\n", lugar);
    printf("DATA: %s\n", dia);
    printf("QUANTIDADE DE OBITOS = %i\n", pico);
}
/////////////////////////////////////////////////

//FUNÇÕES BÁSICAS DA ÁRVORE
ArvAVL* cria_ArvAVL()
{
    ArvAVL* raiz = (ArvAVL*) malloc(sizeof(ArvAVL));
    if(raiz != NULL)
        *raiz = NULL;
    return raiz;
}

int altura_NO(struct ne* no)
{
    if(no == NULL)
        return -1;
    else
    return no->altura;
}

int fatorBalanceamento_NO(struct ne* no)
{
    return labs(altura_NO(no->esq) - altura_NO(no->dir));
}

int maior(int x, int y)
{
    if(x > y)
        return x;
    else
        return y;
}

void RotacaoLL(ArvAVL *A)//LL
{
    NE *B;
    B = (*A)->esq;
    (*A)->esq = B->dir;
    B->dir = *A;
    (*A)->altura = maior(altura_NO((*A)->esq),altura_NO((*A)->dir)) + 1;
    B->altura = maior(altura_NO(B->esq),(*A)->altura) + 1;
    *A = B;
}

void RotacaoRR(ArvAVL *A)//RR
{
    NE *B;
    B = (*A)->dir;
    (*A)->dir = B->esq;
    B->esq = (*A);
    (*A)->altura = maior(altura_NO((*A)->esq),altura_NO((*A)->dir)) + 1;
    B->altura = maior(altura_NO(B->dir),(*A)->altura) + 1;
    (*A) = B;
}

void RotacaoLR(ArvAVL *A)//LR
{
    RotacaoRR(&(*A)->esq);
    RotacaoLL(A);
}

void RotacaoRL(ArvAVL *A)//RL
{
    RotacaoLL(&(*A)->dir);
    RotacaoRR(A);
}

int insere_ArvAVL(ArvAVL *raiz, char estado[], char data[], int obitos[])
{
    int res, i;
    if(*raiz == NULL)
	{
        NE *novo;
        novo = (NE*)malloc(sizeof(NE));
        if(novo == NULL)
        {
            printf("Não foi possivel reservar espaço na memória!\n");
            return 0;
        }
        strcpy(novo->state, estado);         // strcpy(string_destino, string_origem);
        strcpy(novo->date, data);         // strcpy(string_destino, string_origem);

        for(i=0; i<max; i++)
            novo->dead[i] = obitos[i];

        novo->altura = 0;
        novo->esq = NULL;
        novo->dir = NULL;
        *raiz = novo;
        return 1;
    }
    NE *atual = *raiz;
    if(estado < atual->state)
    {
        if((res = insere_ArvAVL(&(atual->esq), estado, data, obitos)) == 1)
        {
            if(fatorBalanceamento_NO(atual) >= 2)
            {
                if(estado < (*raiz)->esq->state)
                {
                    RotacaoLL(raiz);
                }else{
                    RotacaoLR(raiz);
                }
            }
        }
    }else{

            if((res = insere_ArvAVL(&(atual->dir), estado, data, obitos)) == 1)
            {
                if(fatorBalanceamento_NO(atual) >= 2)
                {
                    if((*raiz)->dir->state < estado)
                    {
                        RotacaoRR(raiz);
                    }else{
                        RotacaoRL(raiz);
                    }
                }
            }
        }

    atual->altura = maior(altura_NO(atual->esq),altura_NO(atual->dir)) + 1;

    return (res);
}

void inserir_elemento(ArvAVL *t, FILA_ENC f)
{
	int res;
	NODE *aux;
	for(aux = f->INICIO; aux != NULL; aux = aux->next)
		res = insere_ArvAVL(t, aux->state, aux->date, aux->dead);
}

ArvAVL left(ArvAVL t)
{
	return t->esq;
}

ArvAVL right(ArvAVL t)
{
	return t->dir;
}

char* infoState(ArvAVL t)
{
	return t->state;
}

char* infoData(ArvAVL t)
{
	return t->date;
}

int infoObitos(ArvAVL t, int indice)
{
	return(t->dead[indice]);
}
/////////////////////////////////////////////////

//FUNÇÕES DE BUSCA DA ARVORE
void precursorPreOrdem(ArvAVL arvore, int *pico, char *estado, char *data)
{
 	if(arvore)
 	{

        if(*pico < infoObitos(arvore, 23))
        {
            strcpy(data, infoData(arvore));
            strcpy(estado, infoState(arvore));
            *pico = infoObitos(arvore, 23);
        }
 		precursorPreOrdem(left(arvore),pico, estado, data);    // L (percorre a sub-árvore esquerda)
 		precursorPreOrdem(right(arvore), pico, estado, data);   // R (percorre a sub-árvore direita)
	 }
}


void picoArvore(ArvAVL arvore)
{
    int *p, pico = -1;
	char data[11], estado[3];

    p = &pico;


    precursorPreOrdem(arvore, p, estado, data);

	printf("######PICO DE MORTES EM 2020######\n");
 	printf("ESTADO: %s\n", estado);
    printf("DATA: %s\n", data);
    printf("QUANTIDADE DE OBITOS = %i\n", pico);
}

void DataEstadoArvore(ArvAVL arvore, char estado[], char data[])
{
    precursorPreOrdemEstadoData(arvore, estado, data);
}

void precursorPreOrdemEstadoData(ArvAVL arvore, char estado[], char data [])
{
 	if(arvore)
 	{
       int aux = strcmp(estado, infoState(arvore));
       int aux1 = strcmp(data, infoData(arvore));

        if((aux == 0) && (aux1 == 0))
        {
                        printf("Data: %s\n", infoData(arvore));
                        printf("Estado: %s\n", infoState(arvore));
                        printf("semana epidemiológica 2019: %i\n", infoObitos(arvore, 0));
                        printf("semana epidemiológica 2020: %i\n", infoObitos(arvore, 1));
                        printf("óbitos indeterminados 2019: %i\n", infoObitos(arvore, 2));
                        printf("óbitos por insuficiência respiratória 2019: %i\n", infoObitos(arvore, 3));
                        printf("outros óbitos 2019: %i\n", infoObitos(arvore, 4));
                        printf("óbitos pneumonia 2019: %i\n", infoObitos(arvore, 5));
                        printf("óbitos septicemia 2019: %i\n", infoObitos(arvore, 6));
                        printf("óbitos sars 2019: %i\n", infoObitos(arvore, 7));
                        printf("óbitos covid19: %i\n", infoObitos(arvore, 8));
                        printf("óbitos indeterminados 2020: %i\n", infoObitos(arvore, 9));
                        printf("óbitos insuficiência respiratória 2020: %i\n", infoObitos(arvore, 10));
                        printf("outros óbitos 2020: %i\n", infoObitos(arvore, 11));
                        printf("mortes por pneumonia 2020: %i\n", infoObitos(arvore, 12));
                        printf("mortes septicemia 2020: %i\n", infoObitos(arvore, 13));
                        printf("mortes sars 2020: %i\n", infoObitos(arvore, 14));
                        printf("mortes totais 2019: %i\n", infoObitos(arvore, 15));
                        printf("mortes totais 2020: %i\n", infoObitos(arvore, 16));
                        printf("novas mortes indeterminadas 2019: %i\n", infoObitos(arvore, 17));
                        printf("novas mortes por insuficiência respiratória 2019: %i\n", infoObitos(arvore, 18));
                        printf("novas outras mortes 2019: %i\n", infoObitos(arvore, 19));
                        printf("novas mortes por pneumonia 2019: %i\n", infoObitos(arvore, 20));
                        printf("novas mortes septicemia 2019: %i\n", infoObitos(arvore, 21));
                        printf("novas mortes sars 2019: %i\n", infoObitos(arvore, 22));
                        printf("novas mortes covid19: %i\n", infoObitos(arvore, 23));
                        printf("novas mortes indeterminadas em 2020: %i\n", infoObitos(arvore, 24));
                        printf("novas mortes por insuficiência respiratória 2020: %i\n", infoObitos(arvore, 25));
                        printf("novas outras mortes 2020: %i\n", infoObitos(arvore, 26));
                        printf("novas mortes pneumonia 2020: %i\n", infoObitos(arvore, 27));
                        printf("novas mortes septicemia 2020: %i\n", infoObitos(arvore, 28));
                        printf("novas mortes sars 2020: %i\n", infoObitos(arvore, 29));
                        printf("novas mortes totais 2019: %i\n", infoObitos(arvore, 30));
                        printf("novas mortes totais 2020: %i\n", infoObitos(arvore, 31));

            return;
        }
 		precursorPreOrdemEstadoData(left(arvore), estado, data);    // L (percorre a sub-árvore esquerda)
 		precursorPreOrdemEstadoData(right(arvore), estado, data);   // R (percorre a sub-árvore direita)
	 }
}

/////////////////////////////////////////////////


void limparvetor(char *vet,int tam)
{
	int i=0;
	for (i=0; i<tam; i++)
	{
		vet[i]='\0';
	}
}

void lerArquivo ()
{
 char data[11];
	int contvirgulas=0;
	char estado[3];
	int i,j,k, posicao,qtdlinhas=0;
	char aux[8];
	FILE *arq;
	char str[500];
	int valorint;
	int obitos[32];
	int Qlinhas =0;

	cria_listaState(&LISTA);
	cria_fila(&FILA);
	create(&PILHA);
    ARVORE = cria_ArvAVL();

	//////////////////////////////////////////////////////////////////////////////////////////////
	if (!(arq = fopen("obito_cartorio.txt","r")))
	{
		printf ("Nao foi possivel abrir o arquivo obito_cartorio! \n");
		exit(23);
	}

	while(!feof(arq) && qtdlinhas < 9882)
	{
		limparvetor(str,500);
        fgets(str, 499, arq);
//		printf("%s",str);
		///////////////////////////////////

        limparvetor(estado, 3);
		// INSERINDO O ESTADO
		estado[0]= str[11];
		estado[1]= str[12];
		///////////////////////////////////

		// INSERINDO A DATA
		limparvetor(data,11);
		i = -1;
		while(i<10)
		{
			data[i]=str[i]; //variavel data vai receber a data da linha, pegando caractere por caractere
			i++;
		}

        //INSERINDO OS OBITOS
        limparvetor(aux,6);
		posicao=13; 			//vai começar da posição 13, pra começar a pegar os valores dos obitos, se for uma virgula, avança, senão armazena o valor
		i=0;
		int foivirgula=0;

		while(contvirgulas < 32)
		{

			if(str[posicao+1]!=',' && str[posicao+1]!='\n')
			{
				aux[i]=str[posicao+1];
				i++;
				posicao++;
				foivirgula=0;
			}else{
				if (!foivirgula)
				{
					valorint = atoi(aux);
					obitos[contvirgulas]=valorint;
					limparvetor(aux,8);
				}else{
					obitos[contvirgulas]=0;
				}
				contvirgulas++;
				posicao++;
				i=0;
				foivirgula=1;
			}
		}

		contvirgulas = 0;

        inserirAUX(LISTA, estado, data, obitos); //INSERINDO DADOS NA LISTA
        ins(FILA, data, estado, obitos); //INSERINDO DADOS NA FILA
        push(&PILHA, data, estado, obitos); //INSERINDO DADOS NA PILHA

        i=0;
        qtdlinhas++;
    }
    inserir_elemento(ARVORE, FILA);
    fclose(arq);

}


// procedimento que converte uma string para maiúsculo
void maiusculo(char s1[], char s2[])
{
    int i = 0;
    while(s1[i] != '\0')
    {
        s2[i] = toupper(s1[i]);
        i++;
    }
    s2[i] = '\0'; // caracteer que indica o fim da string
}

void menu()
{
    int op;
    char d[11];
    char e[3];
    clock_t t; //TEMPO DE EXECUÇÃO DO PICO DE OBITOS DA LISTA




        printf("================================================================================\n");
        printf("================================COVID19=========================================\n");
        printf("================================================================================\n");
        printf("DIGITE '1' PARA SABER AS INFORMAÇÕES DE COVID19 DE UMA DETERMINADA DATA E ESTADO\n");
        printf("DIGITE '2' PARA SABER O PICO DE MORTES DE 2019 E 2020\n");
    do
    {
        scanf("%i", &op);

        if((op != 1) && (op != 2))
            printf("Opção inválida\nPor favor digite uma opção válida!\n");

    }while((op != 1) && (op != 2));
        switch(op)
        {
            case 1:
                printf("INFORME UMA DATA (ano-mes-dia): *Utilizando os tracos\n");
                setbuf(stdin,NULL);
                gets(d);

                printf("INFORME O ESTADO POR SUA SIGLA:\n");
                setbuf(stdin,NULL);
                gets(e);
                maiusculo(e, e);

                printf("\n====RESULTADO NA ESTRUTURA LISTA====\n");
                //BUSCANDO NA LISTA
                t = clock(); //variável para armazenar tempo
                BuscarLista(LISTA, d, e);
                t = clock() - t; //tempo final - tempo inicial

                //imprime o tempo na tela
                printf("Tempo de execucao da lista: %lfms\n\n", ((double)t)/((CLOCKS_PER_SEC/1000)));
                printf("--------------------------------------------------------------------------------\n");
                printf("--------------------------------------------------------------------------------\n");

                printf("\n====RESULTADO NA ESTRUTURA FILA====\n");
                //BUSCANDO NA FILA
                t = clock(); //variável para armazenar tempo
                DataEstado(FILA, e, d);
                t = clock() - t; //tempo final - tempo inicial

                //imprime o tempo na tela
                printf("Tempo de execucao da fila: %lfms\n\n", ((double)t)/((CLOCKS_PER_SEC/1000)));
                printf("--------------------------------------------------------------------------------\n");
                printf("--------------------------------------------------------------------------------\n");

                printf("\n====RESULTADO NA ESTRUTURA PILHA====\n");
                //BUSCANDO NA PILHA
                t = clock(); //variável para armazenar tempo
                DataEstadoPilha(PILHA, e, d);
                t = clock() - t; //tempo final - tempo inicial

                //imprime o tempo na tela
                printf("Tempo de execucao da pilha: %lfms\n\n", ((double)t)/((CLOCKS_PER_SEC/1000)));
                printf("--------------------------------------------------------------------------------\n");
                printf("--------------------------------------------------------------------------------\n");

                printf("\n====RESULTADO NA ESTRUTURA ÁRVORE====\n");
                //BUSCANDO NA ÁRVORE
                t = clock(); //variável para armazenar tempo
                DataEstadoArvore(*ARVORE, e, d);
                t = clock() - t; //tempo final - tempo inicial

                //imprime o tempo na tela
                printf("Tempo de execucao da árvore: %lfms\n\n", ((double)t)/((CLOCKS_PER_SEC/1000)));
                printf("--------------------------------------------------------------------------------\n");
                printf("--------------------------------------------------------------------------------\n");

                break;

            case 2:
                setbuf(stdin,NULL);

                printf("\n====RESULTADO NA ESTRUTURA LISTA====\n");
                //BUSCANDO NA LISTA
                t = clock(); //variável para armazenar tempo
                BuscarLista2(LISTA);
                t = clock() - t; //tempo final - tempo inicial

                //imprime o tempo na tela
                printf("Tempo de execucao da lista: %lfms\n\n", ((double)t)/((CLOCKS_PER_SEC/1000)));
                printf("--------------------------------------------------------------------------------\n");
                printf("--------------------------------------------------------------------------------\n");

                printf("\n====RESULTADO NA ESTRUTURA FILA====\n");
                //BUSCANDO NA FILA
                t = clock(); //variável para armazenar tempo
                pico(FILA);
                t = clock() - t; //tempo final - tempo inicial

                //imprime o tempo na tela
                printf("Tempo de execucao da fila: %lfms\n\n", ((double)t)/((CLOCKS_PER_SEC/1000)));
                printf("--------------------------------------------------------------------------------\n");
                printf("--------------------------------------------------------------------------------\n");

                printf("\n====RESULTADO NA ESTRUTURA PILHA====\n");
                //BUSCANDO NA PILHA
                t = clock(); //variável para armazenar tempo
                picoPilha(PILHA);
                t = clock() - t; //tempo final - tempo inicial

                //imprime o tempo na tela
                printf("Tempo de execucao da Pilha: %lfms\n\n", ((double)t)/((CLOCKS_PER_SEC/1000)));
                printf("--------------------------------------------------------------------------------\n");
                printf("--------------------------------------------------------------------------------\n");

                printf("\n====RESULTADO NA ESTRUTURA ÁRVORE====\n");
                //BUSCANDO NA ÁRVORE
                t = clock(); //variável para armazenar tempo
                picoArvore(*ARVORE);
                t = clock() - t; //tempo final - tempo inicial

                //imprime o tempo na tela
                printf("Tempo de execucao da árvore: %lfms\n\n", ((double)t)/((CLOCKS_PER_SEC/1000)));
                printf("--------------------------------------------------------------------------------\n");
                printf("--------------------------------------------------------------------------------\n");


                break;

        }

        printf("\n\n##PRESSIONE ENTER PARA CONTINUAR##\n");
        getchar();
        system(" clear || cls");
}
//////////////////////////////////////////////////////////////////////////////////////////////////////
