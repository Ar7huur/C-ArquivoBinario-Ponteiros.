#include <conio.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <windows.h>
#include <time.h>

struct TpData
{
	int dia, ano;   
	char mes[4];
	
};

struct TpPessoa
{
	char Nome[30];  
	int FuncCod, Status; 
	TpData DtAdmissao;
	float Salario;
	
};

struct TpCargo
{
	int CargCod, Status, FuncCod;     
	char Descricao[20];
};

struct TpFilial
{
	int FiCod, dia, ano;   
	char mes[4];
	char Nome[20];
	TpData DtAdmissao;
	int Status, FuncCod;
};

/*struct TpVinc 
{
	int Status, FuncCod, FiCod;
};*/


//=====================J
//================Funcionarios===========================
int BuscSeqFunc(FILE *PtrArqFunc, int cod);//Busca sequencial indexada
int PosMaiorFunc (FILE *PtrArqFunc, int TL);
int OrdenaSelecaoDiFunc(FILE *PtrArqFunc);
void CadastroFunc (char Funcionario[100]);
void AlterarFuncCOD (char Funcionario[100]);
int BuscaNomeFunc(FILE *PtrArqFunc, char NomeFunc[30]);
void Consultar(void);
void MostrarFunc (char Funcionario[100]);
void ExcluFisicaFunc(void);
void Deletar(void); //exclu fisica
void ExclusaoLogica(char NomeArq[100]); //exclu logica
//=========================C
//======================Cargos=============================
int BuscaCargo(FILE *PtrArqCargo, int Cod2);
void CadastrandoCargo(char NomeArquivo[100]);
void AlterarCargo(char NomeArquivo[100]);
void MostrarCargo (char NomeArquivo[100]);
void ConsultarCargo(void);
void DeletarCargo(void);

//=========================D
//======================FILIAL=============================
int BuscaFilial(FILE *PtrArqFilial, int cod2);
void CadFilial(char NomeArquivo[100]);
void MostrarFilial (char NomeArquivo[100]);
void AlterarFilial (char NomeArquivo[100]);

//======================RELATORIO=============================
void Relatorio(void);

//======================MENU=============================
char Menu(void);


char Menu(void)
{
	


	//gotoxy..
	
	system("cls");
	printf("### Menu de Opcoes ###\n");
	printf("[A] Cadastro de Funcionarios\n"); 
	printf("[B] Cadastro de Cargos\n"); 
	printf("[C] Cadastro de Filial\n");
	printf("[D] Alterar Funcionarios pelo CODIGO\n"); 
	printf("[E] Alterar Cargos pelo CODIGO\n"); 
	printf("[F] Alterar Filiais pelo CODIGO\n"); 
	printf("[G] Consultar Funcionario\n");
	printf("[H] Consultar Cargo\n"); 
	printf("[I] Mostrar Funcionarios cadastrados\n"); 
	printf("[J] Mostrar Cargos cadastrados\n");
	printf("[K] Mostrar Filial cadastrada\n"); 
	printf("[L] Exclusao de funcionario\n");
	printf("[M] Exclusao de cargos\n"); 
	printf("[N] Relatorio final \n");    
	printf("[ESC] Sair do Programa\n");
	printf("\nPressione a opcao desejada: ");
	return toupper(getche());
}

//Busca sequencial indexada (para codigo do funcionario)
int BuscSeqFunc(FILE *PtrArqFunc, int cod)
{
	TpPessoa RegPessoa;
	rewind(PtrArqFunc);
	fread(&RegPessoa,sizeof(TpPessoa),1,PtrArqFunc);
	
	while(!feof(PtrArqFunc) && RegPessoa.FuncCod < cod)
		fread(&RegPessoa,sizeof(TpPessoa),1,PtrArqFunc);
	if(!feof(PtrArqFunc) && RegPessoa.FuncCod == cod)
		return ftell (PtrArqFunc)-sizeof(TpPessoa);
	else 
		return -1;	

}


//Posicao maior
int PosMaiorFunc (FILE *PtrArqFunc, int TL)
{
	TpPessoa RegPessoa;
	rewind(PtrArqFunc);
	int maior, posmaior = 0;
	
	fread(&RegPessoa,sizeof(TpPessoa),1,PtrArqFunc);
	maior = RegPessoa.FuncCod;
	
	for (int i=1; i<TL; i++)
	{
		fread(&RegPessoa,sizeof(TpPessoa),1,PtrArqFunc);
		if(RegPessoa.FuncCod>maior)
		{
			maior = RegPessoa.FuncCod;
			posmaior = i;
		}
	}
	return posmaior;	
}


//Ordenação por seleção direta
int OrdenaSelecaoDiFunc(FILE *PtrArqFunc)
{
	TpPessoa RegPessoa;
	TpPessoa RegPessoa2;
	int posmaior, maior, TL;
	
	fseek (PtrArqFunc,0,2);
	TL = ftell(PtrArqFunc)/sizeof(TpPessoa);
	
	while(TL>0)
	{
		posmaior = PosMaiorFunc (PtrArqFunc, TL);
		if (posmaior < TL-1)
		{
			fseek(PtrArqFunc, posmaior * sizeof (TpPessoa),0);
			fread(&RegPessoa2,sizeof(TpPessoa),1,PtrArqFunc);
			fseek(PtrArqFunc, (TL-1)*sizeof(TpPessoa),0);
			fread(&RegPessoa,sizeof(TpPessoa),1,PtrArqFunc);
			fseek(PtrArqFunc, posmaior * sizeof (TpPessoa),0);
			fwrite (&RegPessoa,sizeof(TpPessoa),1,PtrArqFunc);
			fseek(PtrArqFunc, (TL-1)*sizeof(TpPessoa),0);
			fwrite (&RegPessoa2,sizeof (TpPessoa),1,PtrArqFunc);

		}
		TL--;
	}	
}

//Cadastro de funcionario
void CadastroFunc (char Funcionario[100])
{
	int cod, existe;
	FILE *PtrArqFunc = fopen (Funcionario, "ab+");
	
	TpPessoa RegPessoa;
	
	printf("### CADASTRO DE FUNCIONARIO ###\n");
	printf("### PARA CADASTRO DE FUNCIONARIO, FORNECA CODIGOS DIFERENTES! ###\n");
	printf("----------------------------------------------------------------------\n");
	printf("*** DIGITE O CODIGO QUE DESEJA FORNECER PARA O FUNCIONARIO OU DIGITE -1 PARA VOLTAR AO MENU! ***\n");
	scanf("%d", &cod);
	
	while (cod > 0)
	{
		existe = BuscSeqFunc (PtrArqFunc,cod);
		if (existe==-1)
		{
			RegPessoa.FuncCod = cod;
			RegPessoa.Status = 1;
			
			printf("*** DIGITE O NOME DO FUNCIONARIO!! ***\n");
			fflush(stdin);
			gets(RegPessoa.Nome);
			
			printf ("Dia da Admissao: ");
			scanf("%d",&RegPessoa.DtAdmissao.dia);
			
			printf("Mes [as tres primeiras letras]: ");
			fflush(stdin);
			gets(RegPessoa.DtAdmissao.mes);
			strupr(RegPessoa.DtAdmissao.mes);
				
			printf("Ano: ");
			scanf("%d",&RegPessoa.DtAdmissao.ano);
			printf("Salario: R$ ");
			scanf("%f",&RegPessoa.Salario);			
			
			fseek(PtrArqFunc,0,2);
			fwrite(&RegPessoa,sizeof(TpPessoa),1,PtrArqFunc);
			fclose(PtrArqFunc);
			PtrArqFunc = fopen (Funcionario, "rb+");
			OrdenaSelecaoDiFunc(PtrArqFunc);
			
			printf("\n### FUNCIONARIO CADASTRADO!! ###\n");
		}
		else
		{
			printf("\n### ESSE FUNCIONARIO JA EXISTE!! ###\n\n");
			
		}
		PtrArqFunc = fopen (Funcionario, "ab+");
		printf("### CADASTRO DE FUNCIONARIO ###\n");
		printf("### PARA CADASTRO DE FUNCIONARIO, FORNECA CODIGOS DIFERENTES! ###\n");
		printf("----------------------------------------------------------------------\n");
		printf("*** DIGITE O CODIGO QUE DESEJA FORNECER PARA O FUNCIONARIO! ***\n");
		scanf("%d", &cod);
	}
	getch();
	fclose(PtrArqFunc);
}

//Alterar funcionario pelo codigo
void AlterarFuncCOD (char Funcionario[100])
{
	FILE *PtrArqFunc = fopen (Funcionario, "rb+");
	int pos;
	TpPessoa RegPessoa;
	
	printf("### ALTERANDO FUNCIONARIO PELO SEU CODIGO! ###\n");
	printf("----------------------------------------------------------------------\n");
	printf("*** INFORME O CODIGO DO FUNCIONARIO OU DIGITE -1 PARA VOLTAR AO MENU: ***\n");
	scanf("%d",&RegPessoa.FuncCod);
	
	while (RegPessoa.FuncCod>0)
	{
		pos = BuscSeqFunc(PtrArqFunc,RegPessoa.FuncCod);
		if (pos ==-1)
		{
				printf("\n### CODIGO INVALIDO, FUNCIONARIO NAO ENCONTRADO! ###\n");
				getch();
				
		}
		else
		{
			printf("### DETALHES DO FUNCIONARIO: \n");
			fseek(PtrArqFunc,pos,0);
			fread(&RegPessoa,sizeof(TpPessoa),1,PtrArqFunc);
			printf("Nome: %s \t Data de Admissao: %d/%s/%d \t Salario:  R$ %.2f\n",RegPessoa.Nome, RegPessoa.DtAdmissao.dia, RegPessoa.DtAdmissao.mes, RegPessoa.DtAdmissao.ano, RegPessoa.Salario);
			printf("\n*** DESEJA ALTERAR DADOS ? [S/N]: \n");
			if(toupper(getche()) == 'S')
			{
				printf("### DIGITE OS NOVOS DADOS! ###\n");
				
				printf(" Novo nome do funcionario: ");
				fflush(stdin);
				gets(RegPessoa.Nome);
				
				printf ("\nDia da Admissao: ");
				scanf("%d",&RegPessoa.DtAdmissao.dia);
				
				printf("\nMes [as tres primeiras letras]: ");
				fflush(stdin);
				gets(RegPessoa.DtAdmissao.mes);
				strupr(RegPessoa.DtAdmissao.mes);
				
				printf("\nAno: ");
				scanf("%d",&RegPessoa.DtAdmissao.ano);
				
				printf("\nSalario: ");
				scanf("%f",&RegPessoa.Salario);
				
				fseek(PtrArqFunc,pos,0);
				fwrite(&RegPessoa,sizeof(TpPessoa),1,PtrArqFunc);
				
				printf("\n### NOVA INCLUSAO FEITA COM SUCESSO! ###\n");

			}
		}
		printf("### ALTERANDO FUNCIONARIO PELO SEU CODIGO! ###\n");
		printf("----------------------------------------------------------------------\n");
		printf("*** INFORME O CODIGO DO FUNCIONARIO OU DIGITE -1 PARA VOLTAR AO MENU: ***\n");
		scanf("%d",&RegPessoa.FuncCod);	
	}
	fclose(PtrArqFunc);
}

//Buscar funcionario pelo nome
int BuscaNomeFunc(FILE *PtrArqFunc, char NomeFunc[30])
{
	TpPessoa RegPessoa;
	rewind(PtrArqFunc);   
	fread(&RegPessoa,sizeof(TpPessoa),1,PtrArqFunc);
	while (!feof(PtrArqFunc) && stricmp(RegPessoa.Nome,NomeFunc)!=0)
		fread(&RegPessoa,sizeof(TpPessoa),1,PtrArqFunc);

	if (!feof(PtrArqFunc))    	
		return ftell(PtrArqFunc)-sizeof(TpPessoa);
	else
		return -1;	
}

//Consultar funcionario pelo nome
void Consultar(void)
{
	FILE *PtrFunc = fopen("Funcionario.dat","rb");
	TpPessoa RFunc;
	int posicao;
	printf("\n\n ** Consulta de Nomes ou [ESC + ENTER] para voltar ao menu ** \n\n");
	printf("Nome a Consultar: ");  fflush(stdin);
	gets(RFunc.Nome);
	while (strcmp(RFunc.Nome,"\0")!=0)
	{
		posicao = BuscaNomeFunc(PtrFunc,RFunc.Nome);
		if (posicao>=0)
		{
			fseek(PtrFunc,posicao,0);
			fread(&RFunc,sizeof(TpPessoa),1,PtrFunc);
			printf("\n[%s]: Encontrado na posicao [%d]!\n",RFunc.Nome,posicao);
			printf("Data de Admissao: %d/%s/%d \t Salario:  R$ %.2f\n",RFunc.DtAdmissao.dia, RFunc.DtAdmissao.mes, RFunc.DtAdmissao.ano, RFunc.Salario);
			printf("Codigo do funcionario eh: %d",RFunc.FuncCod);			
		}
		else 
			printf("\nNome nao Cadastrado!\n");
		
		getch();
		printf("\n\nNome a Consultar: ");  fflush(stdin);
		gets(RFunc.Nome);
	}
	fclose(PtrFunc);
}

//Mostrar funcionarios cadastrados
void MostrarFunc (char Funcionario[100])
{
	FILE *PtrArqFunc = fopen (Funcionario, "rb");
	TpPessoa Func;
	int y=8;
	
	if (PtrArqFunc!=NULL || feof(PtrArqFunc))
	{
		printf("### LISTANDO FUNCIONARIOS CADASTRADOS, [ESC] PARA VOLTAR AO MENU ###\n");
		fread(&Func,sizeof(TpPessoa),1,PtrArqFunc);
		while(!feof(PtrArqFunc))
		{
			if(Func.Status== 1)
			{
				printf("\nCodigo de funcionario: %d",Func.FuncCod);
				y++;
				
				printf("\nNome do funcionario: %s",Func.Nome);
				y++;
				
				printf("\nData de Admissao: %d/%s/%d\n",Func.DtAdmissao.dia, Func.DtAdmissao.mes, Func.DtAdmissao.ano);
				y++;
				
				printf("Salario: R$ %.2f",Func.Salario);
				y++;
				
				printf("\n-----------------------------------------------------------------------------------------\n");
			}
			else
			{
				printf("### FUNCIONARIOS QUE VOCE INFORMOU NAO FOI CADASTRADO!! ###\n");
			}
			fread(&Func,sizeof(TpPessoa),1,PtrArqFunc);
		}
		getch();
		fclose(PtrArqFunc);
	}	
}

//Exclusao Fisica
void Deletar(void)
{
	FILE *PtrFunc = fopen("Funcionario.dat","rb");
	TpPessoa Reg;
	char AuxNome[30];
	int posicao;
	printf("\n\n ** Exclusao de Registros ou [ESC + ENTER] PARA VOLTAR AO MENU** \n\n");
	printf("Nome a Excluir: ");  fflush(stdin);
	gets(AuxNome);
	if (strcmp(AuxNome,"\0")!=0)
	{
		posicao = BuscaNomeFunc(PtrFunc,AuxNome);
		if (posicao>=0)
		{
			printf("\n*** DETALHES DO FUNCIONARIO QUE SERA EXCLUIDO ***\n");
			fseek(PtrFunc,posicao,0);
			fread(&Reg,sizeof(TpPessoa),1,PtrFunc);
			printf("\n[%s]: Encontrado na posicao [%d]!\n",Reg.Nome,posicao);
			printf("Data de Admissao: %d/%s/%d \t Salario:  R$ %.2f\n",Reg.DtAdmissao.dia, Reg.DtAdmissao.mes, Reg.DtAdmissao.ano, Reg.Salario);			
			printf("\nConfirma a Exclusao do Registro (S/N)? ");
			if (toupper(getche()) == 'S')
			{
				FILE *PtrTemp = fopen("Temp.dat","wb");
				rewind(PtrFunc);
				fread(&Reg,sizeof(TpPessoa),1,PtrFunc);
				while(!feof(PtrFunc))
				{
					if (stricmp(AuxNome,Reg.Nome)!=0)
						fwrite(&Reg,sizeof(TpPessoa),1,PtrTemp);
					
					fread(&Reg,sizeof(TpPessoa),1,PtrFunc);
				}
				fclose(PtrFunc);
				fclose(PtrTemp);
				remove("Funcionario.dat");
				rename("Temp.dat","Funcionario.dat");
				
				printf("\n*** Registro(s) Deletado(s)! ***\n");
				getch();
			}
		}
		else
			{
				printf("\nNome nao Cadastrado!\n");
				fclose(PtrFunc);
				getch();
			}
	}
	else fclose(PtrFunc);
}
//Arrumar...
//Exclusao logica de funcionario
/*void ExclusaoLogica(char NomeArq[100])
{
	FILE *PtrArq = fopen(NomeArq,"rb+");
	TpPessoa RegF;
	char AuxNome[30];
	int pos;
	if (PtrArq == NULL)
		printf("\n### NOME INFORMADO NAO EXISTE, VERIFIQUE O NOME QUE FORNECEU..: %s\n",NomeArq);
	else
		{
			system("cls");
			printf("\n### EXCLUSAO [LOGICA] DE FUNCIONARIOS###\n");
			printf("*** DIGITE O NOME DO FUNCIONARIO A SER EXCLUIDO ***\n");
			fflush(stdin);
			gets(AuxNome);
			while (strcmp(AuxNome,"\0")!=0)
			{
				pos = BuscaNomeFunc(PtrArq,AuxNome);
				if (pos==-1)
					printf("\n### FUNCIONARIO NAO ENCONTRADO, VERIFIQUE O NOME FORNECIDO!! ###\n");
				else{
						fseek(PtrArq,pos,0);
						fread(&RegF,sizeof(TpPessoa),1,PtrArq);
						printf("\n*** DETALHES DO FUNCIONARIO QUE SERA EXCLUIDO ***\n");
						printf("\n[%s]: Encontrado na posicao [%d]!\n",RegF.Nome,pos);
						printf("Seu codigo eh: %d",RegF.FuncCod);
						printf("\nData de Admissao: %d/%s/%d \t Salario:  R$ %.2f\n",RegF.DtAdmissao.dia, RegF.DtAdmissao.mes, RegF.DtAdmissao.ano, RegF.Salario);			
						printf("\n### DESEJA EXCLUIR LOGICAMENTE ESSE FUNCIONARIO ? [S/N]###\n");
						if(toupper(getche())=='S')
						{
							RegF.Status=1;
							fseek(PtrArq,pos,0);
							fwrite(&RegF,sizeof(TpPessoa),1,PtrArq);
							printf("\n### FUNCIONARIO INFORMADO FOI EXCLUIDO :( ###!\n");
							getch();
						}
					}
				printf("\n### EXCLUSAO [LOGICA] DE FUNCIONARIOS###\n");
				printf("*** DIGITE O NOME DO FUNCIONARIO A SER EXCLUIDO ***\n");
				fflush(stdin);
				gets(AuxNome);
			}
			
			fclose(PtrArq);
		}
}*/

//==============================================================================================CARGOS=====================================================================================================================
int BuscaCargo(FILE *PtrArqCargo, int Cod2)
{
	TpCargo RegCargo;
	rewind(PtrArqCargo);
	fread(&RegCargo,sizeof(TpCargo),1,PtrArqCargo);
	while(!feof(PtrArqCargo) && RegCargo.CargCod != Cod2)
		fread(&RegCargo,sizeof(TpCargo),1,PtrArqCargo);
	
	if (!feof(PtrArqCargo))
		return ftell(PtrArqCargo)-sizeof(TpCargo);
	else
		return -1;	
}

void CadastrandoCargo(char NomeArquivo[100])
{
	int cod, existe, cod2;
	
	FILE *PtrArqCargo = fopen (NomeArquivo, "ab+");
	FILE *PtrArqFunc = fopen ("Funcionario.dat", "rb");
	TpCargo RegCargo;
	
	printf("### CADASTRO DE CARGOS PARA FUNCIONARIO ###");
	printf("\n*** INFORME UM CODIGO DE FUNCIONARIO EXISTENTE OU DIGITE [-1 + ENTER] PARA VOLTAR AO MENU!! ***\n");
	scanf("%d", &cod);
	while (cod>0)
	{
		existe =  BuscSeqFunc (PtrArqFunc,cod);
		if(existe!=-1)
		{
			printf("*** DIGITE O CODIGO DO CARGO! ***\n");
			scanf("%d", &cod2);
			existe=BuscaCargo(PtrArqCargo,cod2);
			//existe=-1;
			if(existe==-1)
			{
				RegCargo.FuncCod=cod;
				RegCargo.CargCod=cod2;
				RegCargo.Status=1;
				
				printf("*** DESCRICAO DO CARGO ***\n");
				fflush(stdin);
				gets(RegCargo.Descricao);
				fwrite(&RegCargo,sizeof(TpCargo),1,PtrArqCargo);
				printf("\n### CARGO CADASTRADO!! ###\n");
				
			}
			else
			{
				printf("### ESSE CARGO JA TEVE CADASTRO!! ###\n");
				
			}
		
		}
		else
		{
			printf("### ESSE FUNCIONARIO AINDA NAO TEVE CADASTRO, VERIFIQUE O CODIGO POSTO! ###\n");
			
		}
		printf("### CADASTRO DE CARGOS PARA FUNCIONARIO ###");
		printf("\n*** INFORME UM CODIGO DE FUNCIONARIO EXISTENTE!! ***\n");
		scanf("%d",&cod);
		
			
	}
	getch();
	fclose(PtrArqCargo);
	fclose(PtrArqFunc);
}

//Alterar cargos..
void AlterarCargo(char NomeArquivo[100])
{
	FILE *PtrArqCargo = fopen (NomeArquivo, "rb+");
	int pos;
	int y=8;
	TpCargo RegC;
	
	printf("### ALTERAR CARGO ###\n");	
	printf("\n*** INFORME UM CODIGO DO CARGO QUE DESEJA ALTERAR OU DIGITE [-1 + ENTER] PARA VOLTAR AO MENU!! ***\n");
	scanf("%d",&RegC.CargCod);
	while(RegC.CargCod>0)
	{
		pos = BuscaCargo(PtrArqCargo,RegC.CargCod);
		if(pos==-1)
		{
			printf("### CODIGO DE CARGO INVALIDO, VERIFIQUE-O! ###\n");
		}
		else
		{
			printf("### DETALHES DOS REGISTROS DE CARGOS ###\n");
			y++;
			fseek(PtrArqCargo,pos*sizeof(TpCargo),0);
			fread(&RegC,sizeof(TpCargo),1,PtrArqCargo);
			printf("Codigo do cargo eh: %d",RegC.CargCod);
			y++;
			printf("\nDescricao do cargo eh: %s",RegC.Descricao);
			y++;	
			printf("\n----------------------------------------------\n");
			printf("*** DESEJA FAZER ALGUMA ALTERACAO ? [S/N]");
			y++;
			if(toupper(getche())=='S')
			{
				printf("\nNova descricao:\n");
				fflush(stdin);
				gets(RegC.Descricao);
				y++;
				fseek(PtrArqCargo,pos*sizeof(TpCargo),0);
				fwrite(&RegC,sizeof(TpCargo),1,PtrArqCargo);
				printf("### CADASTRO FEITO!! ###\n");
				
			}
		}
		printf("### ALTERAR CARGO ###\n");	
		printf("\n*** INFORME UM CODIGO DO CARGO QUE DESEJA ALTERAR OU DIGITE [-1 + ENTER] PARA VOLTAR AO MENU!! ***\n");
		scanf("%d",&RegC.CargCod);
	}
	
	fclose(PtrArqCargo);
	
}

void MostrarCargo (char NomeArquivo[100])
{
	FILE *PtrArqCargo = fopen (NomeArquivo, "rb");
	TpCargo RegC;
	int y=8;
	
	if (PtrArqCargo!=NULL || feof(PtrArqCargo))
	{
		printf("### LISTANDO CARGOS CADASTRADOS, [ESC] PARA VOLTAR AO MENU ###\n");
		fread(&RegC,sizeof(TpCargo),1,PtrArqCargo);
		while(!feof(PtrArqCargo))
		{
			if(RegC.Status==1)
			{
				printf("\nCodigo de cargo: %d",RegC.CargCod);
				y++;
				
				printf("\nDescricao do cargo: %s",RegC.Descricao);
				y++;
				
				printf("\nCodigo de funcionario vinculado a ele: %d",RegC.FuncCod);
				y++;
				printf("\n--------------------------------------------\n");
			}
			/*else
			{
				printf("### CARGOS QUE VOCE INFORMOU NAO FOI CADASTRADO!! ###\n");
			}*/
			fread(&RegC,sizeof(TpCargo),1,PtrArqCargo);
		}
		getch();
		fclose(PtrArqCargo);
	}	
}

int BuscaDescricaoCargo(FILE *PtrArqCargo, char CargoDesc[30])
{
	TpCargo RegC;
	rewind(PtrArqCargo);   
	fread(&RegC,sizeof(TpCargo),1,PtrArqCargo);
	while (!feof(PtrArqCargo) && stricmp(RegC.Descricao,CargoDesc)!=0)
		fread(&RegC,sizeof(TpCargo),1,PtrArqCargo);

	if (!feof(PtrArqCargo))    	
		return ftell(PtrArqCargo)-sizeof(TpCargo);
	else
		return -1;	
}

//Consultar Cargo
void ConsultarCargo(void)
{
	FILE *PtrArqCargo = fopen("Cargo.dat","rb");
	TpCargo RegC;
	int posicao;
	printf("\n\n CONSULTA DE CARGO POR DESCRICAO \n\n");
	printf("Descricao de cargo a consultar ou [ESC + ENTER] para retornar ao menu!\n");  
	fflush(stdin);
	gets(RegC.Descricao);
	while (strcmp(RegC.Descricao,"\0")!=0)
	{
		posicao = BuscaDescricaoCargo(PtrArqCargo,RegC.Descricao);
		if (posicao>=0)
		{
			fseek(PtrArqCargo,posicao,0);
			fread(&RegC,sizeof(TpCargo),1,PtrArqCargo);
			printf("\n[%s]: Encontrado na posicao [%d]!",RegC.Descricao,posicao);
			printf("\nDescricao: %s",RegC.Descricao);
			printf("\nCodigo do cargo: %d\n", RegC.CargCod);			
		}
		else 
			printf("\n### CARGO NAO CADASTRADO!! ###\n");
		
		getch();
		printf("\n\n CONSULTA DE CARGO POR DESCRICAO \n\n");
		printf("Descricao de cargo a consultar ou [ESC + ENTER] para retornar ao menu!\n");  
		fflush(stdin);
		gets(RegC.Descricao);
	}
	fclose(PtrArqCargo);
}

//
void DeletarCargo(void)
{
	FILE *PtrCargo = fopen("Cargo.dat","rb");
	TpCargo Reg;
	char AuxNome[30];
	int posicao;
	printf("\n\n ### EXCLUSAO DE CARGOS OU [ESC + ENTER] PARA VOLTAR AO MENU ###\n\n");
	printf("Desc. do cargo a excluir: ");  fflush(stdin);
	gets(AuxNome);
	if (strcmp(AuxNome,"\0")!=0)
	{
		posicao = BuscaDescricaoCargo(PtrCargo,AuxNome);
		if (posicao>=0)
		{
			printf("\n*** DETALHES DO CARGO QUE SERA EXCLUIDO ***\n");
			fseek(PtrCargo,posicao,0);
			fread(&Reg,sizeof(TpCargo),1,PtrCargo);
			printf("\n[%s]: Encontrado na posicao [%d]!\n",Reg.Descricao,posicao);
			printf("Codigo do cargo eh: %d",Reg.CargCod);
			printf("\nCodigo de funcionario vinculado a ele: %d",Reg.FuncCod);		
			printf("\nConfirma a Exclusao do Registro (S/N)? ");
			if (toupper(getche()) == 'S')
			{
				FILE *PtrTempC = fopen("TempC.dat","wb");
				rewind(PtrCargo);
				fread(&Reg,sizeof(TpCargo),1,PtrCargo);
				while(!feof(PtrCargo))
				{
					if (stricmp(AuxNome,Reg.Descricao)!=0)
						fwrite(&Reg,sizeof(TpCargo),1,PtrTempC);
					
					fread(&Reg,sizeof(TpCargo),1,PtrCargo);
				}
				fclose(PtrCargo);
				fclose(PtrTempC);
				remove("Cargo.dat");
				rename("TempC.dat","Cargo.dat");
				
				printf("\n### CARGO EXCLUIDO!! ###\n");
				getch();
			}
		}
		else
			{
				printf("\n### CARGO NAO FOI CADASTRADO AINDA! ###\n");
				fclose(PtrCargo);
				getch();
			}
	}
	else fclose(PtrCargo);
}


//======================================================================================FILIAIS==================================================================
int BuscaFilial(FILE *PtrArqFilial, int cod2)
{
	TpFilial RegFi;
	rewind(PtrArqFilial);
	fread(&RegFi,sizeof(TpFilial),1,PtrArqFilial);
	while(!feof(PtrArqFilial) && RegFi.FiCod != cod2)
		fread(&RegFi,sizeof(TpFilial),1,PtrArqFilial);
	if (!feof(PtrArqFilial))
		return ftell(PtrArqFilial)-sizeof(TpFilial);
	else
		return -1;	
}

void CadFilial(char NomeArquivo[100])
{
	int cod, existe, cod2;
	FILE *PtrArqFilial = fopen (NomeArquivo, "ab+");
	FILE *PtrArqFunc = fopen ("Funcionario.dat","rb");
	TpFilial RegFi;
	
	printf("\n--------------------------------------------------------------------------------------------------------\n");
	printf("### CADASTRO DE FILIAL PARA VINCULO COM FUNCIONARIO! ###\n");
	printf("### FILIAIS SAO IDENTIFICADAS POR CODIGOS QUE NAO SE REPETEM MAS A SUA DESCRICAO, SIM! ###\n");
	printf("\n--------------------------------------------------------------------------------------------------------\n");
	printf("*** DIGITE UM CODIGO DE FUNCIONARIO EXISTENTE OU DIGITE [-1 + ENTER] PARA VOLTAR AO MENU!! ***\n");
	scanf("%d", &cod);
	while (cod > 0)
	{
		existe = BuscSeqFunc(PtrArqFunc,cod);
		if(existe!=-1)
		{
			printf("*** DIGITE O CODIGO DA FILIAL! ***\n");
			scanf("%d", &cod2);
			existe = BuscaFilial(PtrArqFilial,cod2);
			if(existe==-1)
			{
				RegFi.FuncCod=cod;
				RegFi.FiCod=cod2;
				RegFi.Status=1;
				
				printf("*** DESCRICAO DA FILIAL ***\n");
				fflush(stdin);
				gets(RegFi.Nome);
				strupr(RegFi.mes);			
				
				printf ("Dia da Admissao: ");
				scanf("%d",&RegFi.dia);
				
				printf("Mes [03 primeiras letras]: ");
				fflush(stdin);
				gets(RegFi.mes);
				strupr(RegFi.mes);
				
				printf("Ano: ");
				scanf("%d", &RegFi.ano);
				
				fwrite(&RegFi,sizeof(TpFilial),1,PtrArqFilial);
				printf("### FILIAL CADASTRADA E VINCULADA COM SUCESSO! ###\n");
			}
			else
			{
				printf("### ESSA FILIAL JA TEVE CADASTRO!! ###\n");
			}
		}
		else
		{
			printf("### ESSE FUNCIONARIO NAO FOI CADASTRADO!! ###\n");	
		}
		printf("\n--------------------------------------------------------------------------------------------------------\n");
		printf("### CADASTRO DE FILIAL PARA VINCULO COM FUNCIONARIO! ###\n");
		printf("### FILIAIS SAO IDENTIFICADAS POR CODIGOS QUE NAO SE REPETEM MAS A SUA DESCRICAO, SIM! ###\n");
		printf("\n--------------------------------------------------------------------------------------------------------\n");
		printf("*** DIGITE UM CODIGO DE FUNCIONARIO EXISTENTE OU DIGITE [-1 + ENTER] PARA VOLTAR AO MENU!! ***\n");
		scanf("%d", &cod);
	}
	getch();
	fclose(PtrArqFilial);
	fclose(PtrArqFunc);	
}

void AlterarFilial (char NomeArquivo[100])
{
	FILE *PtrArqFilial = fopen(NomeArquivo, "rb+");
	int pos;
	int y=8;
	TpFilial RegFi;
	
	printf("### ALTERAR FILIAL PELO CODIGO ###\n");
	printf("\n*** INFORME UM CODIGO VALIDO DE FILIAL OU DIGITE [-1 + ENTER] PARA VOLTAR AO MENU!! ***\n");
	scanf("%d", &RegFi.FiCod);
	while(RegFi.FiCod>0)
	{
		pos=BuscaFilial(PtrArqFilial,RegFi.FiCod);
		if(pos==-1)
		{
			printf("### CODIGO INVALIDO, VERIFIQUE-O! ###\n");
		}
		else
		{
			printf("### DETALHES DA FILIAL ###\n");
			y++;
			fseek(PtrArqFilial,pos*sizeof(TpFilial),0);
			fread(&RegFi,sizeof(TpFilial),1,PtrArqFilial);
			printf("Codigo da filial eh: %d",RegFi.FiCod);
			y++;
			printf("\nDesc. Da Filial eh:: %s",RegFi.Nome);
			y++;
			printf("\nCodigo de Funcionario vinculado a ela: %d",RegFi.FuncCod);
			y++;
			printf("\nData de Admissao: %d/%s/%d",RegFi.dia, RegFi.mes, RegFi.ano);
			y++;
			printf("\n--------------------------------------------------------------\n");
			printf("### DESEJA FAZER ALGUMA ALTERACAO ? [S/N]");
			y++;
			if(toupper(getche())=='S')
			{
				printf("\nNova descricao:\n");
				fflush(stdin);
				gets(RegFi.Nome);
				y++;
				printf ("\nDia da Admissao: ");
				scanf("%d",&RegFi.dia);
				y++;
				printf("\nMes [as tres primeiras letras]: ");
				fflush(stdin);
				gets(RegFi.mes);
				strupr(RegFi.mes);
				y++;
				printf("\nAno: ");
				scanf("%d",&RegFi.ano);
				y++;
				
				fseek(PtrArqFilial,pos*sizeof(TpFilial),0);
				fwrite(&RegFi,sizeof(TpFilial),1,PtrArqFilial);
				printf("### FILIAL ALTERADA!! ###\n");
			}
		}
		printf("### ALTERAR FILIAL PELO CODIGO ###\n");
		printf("\n*** INFORME UM CODIGO VALIDO DE FILIAL OU DIGITE [-1 + ENTER] PARA VOLTAR AO MENU!! ***\n");
		scanf("%d", &RegFi.FiCod);
	}
	fclose(PtrArqFilial);
}

void MostrarFilial (char NomeArquivo[100])
{
	FILE *PtrArqFilial = fopen (NomeArquivo,"rb");
	TpFilial RegFi;
	int y=8;
	
	if (PtrArqFilial!=NULL || feof(PtrArqFilial))
	{
		printf("### LISTANDO FILIAIS CADASTRADAS OU APERTE [ESC] PARA VOLTAR AO MENU ###\n\n");
		fread(&RegFi,sizeof(TpFilial),1,PtrArqFilial);
		while(!feof(PtrArqFilial))
		{
			if(RegFi.Status==1)
			{
				printf("DESCRICAO DA FILIAL OU IDFILIAL: %s",RegFi.Nome);
				y++;
				
				printf("\nCodigo de Funcionario: %d",RegFi.FuncCod);
				y++;
				
				printf("\nData de vinculo: %d/%s/%d",RegFi.dia, RegFi.mes, RegFi.ano);
				y++;
				printf("\n--------------------------------------------------------------\n");
				
			}
			else
			{
				printf("\n### AINDA NAO HOUVE CADASTRO DE FILIAIS! ###\n");
			}
			fread(&RegFi,sizeof(TpFilial),1,PtrArqFilial);
		}
		getch();
		fclose(PtrArqFilial);
	}
}


//======================================================================================RELATORIO FINAL==================================================================

//Relatorio final
void Relatorio(void)
{
	FILE *Ptr = fopen("Funcionario.dat","rb");
	FILE *Ptr2 = fopen("Cargo.dat","rb");
	TpPessoa RegPessoa;
	TpCargo RegCargo;
	
	printf("\n*** RELATORIO DE FUNCIONARIOS ***\n");
	if (Ptr == NULL)
		printf("\nErro de abertura do Arquivo!\n");
	else
		{
			printf("\n-----------------------------------------------------------------------------------------\n");
			printf("Codigo Cargo \t Desc. Cargo \t Codigo Func \t Nome Func \t DtAdmissao \t Salario R$\n");//\t Nome \t\t Dt de Admissao \t Salario R$ 
			printf("-----------------------------------------------------------------------------------------\n");
			fread(&RegPessoa,sizeof(TpPessoa),1,Ptr);
			fread(&RegCargo,sizeof(TpCargo),1,Ptr2);
			while(!feof(Ptr) || !feof(Ptr2))
			{
				printf("%d \t\t %s \t %d \t\t %s \t\t %d/%s/%d \t %.2f\n",RegCargo.CargCod,RegCargo.Descricao,RegPessoa.FuncCod,RegPessoa.Nome, RegPessoa.DtAdmissao.dia, RegPessoa.DtAdmissao.mes, RegPessoa.DtAdmissao.ano, RegPessoa.Salario);//\t %s \t\t%d/%s/%d 
				fread(&RegPessoa,sizeof(TpPessoa),1,Ptr);
				fread(&RegCargo,sizeof(TpCargo),1,Ptr2);
			}	
			
			fclose(Ptr);
			fclose(Ptr2);
		}
	
	printf("\n\n\n\n\n==================================FIM DO RELATORIO DE FINAL===============================\n");
	getch();
}

void Executa(void)
{
	char opcao;
	do
	{
		opcao = Menu();
		system("cls");
		
		switch(opcao)
		{
			case 'A': CadastroFunc("Funcionario.dat");
					  break;
			
			case 'B': CadastrandoCargo("Cargo.dat");
					  break;
			
			case 'C': CadFilial("Filial.dat");
					  break;
					  
			case 'D': AlterarFuncCOD("Funcionario.dat");
					  break;
			
			case 'E': AlterarCargo("Cargo.dat");
					  break;
					  
			case 'F': AlterarFilial("Filial.dat");
					  break;
			
			case 'G': Consultar();
					  break;
					  
			case 'H': ConsultarCargo();
					  break;
					  
			case 'I': MostrarFunc("Funcionario.dat");
					  break;
					  
			case 'J': MostrarCargo("Cargo.dat");
					  break;
					  
			case 'K': MostrarFilial("Filial.dat");
					  break;					  
			
			case 'L': Deletar();
					  break;
		
			case 'M': DeletarCargo();
					  break;			
			
			case 'N': Relatorio();
					  break;
		  							  
					  
		}
		
	}while(opcao!=27);
}


int main(void)	
{				  
	Executa();
	return 0;
}
