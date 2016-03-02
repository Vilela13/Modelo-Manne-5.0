#include "Biblioteca.hpp"


int main(int argc, char **argv) {


	if( argc == 2){

		list<string> ListaInstancias;
		string Nome;


		int EscreveDadosLidosNaTela;

		list<string>::iterator it;

		//ofstream ArquivoExcelResposta;
		 FILE *ArquivoExcelResposta;

// Exscrever a dadta
		 time_t timer;
		 char buffer[26];
		 struct tm* tm_info;

// Variavel que indica se resolveu
		int resolveu;

// Status da solução
		int Status;
		double SolucaoPrimal;
		double SolucaoDual;
		double SolucaoReal;
		int ConstrucoesComAtrazo;
		int DemandasAfetadas;
		double ValorAtrazoConstrucoes;
		int PlantasComAtrazo;
		double ValorAtrazoPlantas;
		double Gap;
		double Tempo;
		EscreveDadosLidosNaTela = 0;

	// -------------------------- Le arquivo com as instancias de Solomon e as guarda em uma lista ----------------------- //

		ifstream ArquivoInstanciaSaloman( "ArquivosSaloman.txt" );
		if(!ArquivoInstanciaSaloman){
			cout << " Arquivo nao Existe! \n FUDEU MUITO! \n";
		}else{

			ArquivoInstanciaSaloman >> Nome;
			//cout << " \n   Arqmazena nome de instancias \n " << endl;
			while( Nome != "EOF"){
				cout << "  " << Nome  << endl ;
				ListaInstancias.push_back(Nome);
				ArquivoInstanciaSaloman >> Nome;
			}

			//cout << " Tamanho = " << ListaInstancias.size() << endl << endl;
		 }

		ArquivoInstanciaSaloman.close();

	// -------------------------- Le nome dos arquivos Solomon, carrega os dados, cria arquivo do R, Excel e cria arquivo que pode ser lido pelo Modelo ----------------------- //

		DadosSaloman *InstanciaSaloman;

		while( ListaInstancias.size() > 0){
			InstanciaSaloman = new DadosSaloman;
			it = ListaInstancias.begin();
			Nome = *it;
			ListaInstancias.pop_front();
			//cout <<  " ai 1" << endl;

			cout << Nome << endl;
			//cout <<  " ai 2" << endl;


			cout << " entrei 0 - carrega dados" << endl;
			InstanciaSaloman->CarregarNumeroNosCoordenadas(Nome);
			InstanciaSaloman->EscreverDadosLidosInstanciaSaloman();
			cout << " sai 0 - carrega dados" << endl;

			cout << " entrei 1 - escreve Instancia Salomon" << endl;
			InstanciaSaloman->CriarInstanciaSaloman(Nome);
			cout << " sai 1 - escreve Instancia Salomon" << endl;

			cout << " entrei 2 - escreve comandos R" << endl;
			InstanciaSaloman->EscreverComandosR(Nome,'4');
			// 1 => .ps	  2 =>.png   3 =>.jpeg    4 =>.pdf
			cout << " sai 2 - escreve comandos R" << endl;

			cout << " entrei 3 - escreve comandos excel" << endl;
			InstanciaSaloman->EscreverComandosExcel(Nome);					// Não implementado ainda
			cout << " sai 3 - escreve comandos excel" << endl;
			delete(InstanciaSaloman);




		}


	//Cria instancia manual
	    //No PreInstancia;

//Cria instância que criei na mão
		//PreInstancia.PreencheEstrutura();
		//PreInstancia.CriaTXT();


	// Resolve o problema

		string Instancias;
		Instancias = argv[1];

		ifstream ArquivoInstancia;
		ArquivoInstancia.open(Instancias.c_str());
		if ( ArquivoInstancia.is_open() ){
			ArquivoInstancia >> Nome;
			while( Nome != "EOF"){
				//cout << " coloca lista = " << Nome  << endl ;
				ListaInstancias.push_back(Nome);
				ArquivoInstancia >> Nome;

			}
			ArquivoInstancia.close();

		// ----------- Le um arquivo com as instancias a serem resolvidas pelo modelo, abre o arquivo com a instancia e o resolve -------------------------- //

			//cout << endl << endl << " Lendo arquivo " << endl << endl << endl;

			string Saida;
			Saida = "R-";				// coloca Res- no char*
			Saida += Instancias;
			//Saida += ".txt";

			//cout << " Saida = > "<< Saida << "   Tamanho entrada = " << TamanhoEntrda << endl;

			//ArquivoExcelResposta.open(Saida.c_str());
			ArquivoExcelResposta = fopen(Saida.c_str(), "a");

			//ArquivoExcelResposta << " Instância" << '\t';
			//ArquivoExcelResposta << " Status" << '\t';
			//ArquivoExcelResposta << " Solução Primal" << '\t';
			//ArquivoExcelResposta << " Solução Dual" << '\t';
			//ArquivoExcelResposta << " Gap" << '\t';
			//ArquivoExcelResposta << " Tempo" << '\n';

			time(&timer);
			tm_info = localtime(&timer);
			strftime(buffer, 26, " * %H:%M:%S de %d:%m:%Y", tm_info);


			fprintf(ArquivoExcelResposta,"%s \n", buffer);
			fprintf(ArquivoExcelResposta, "Instância \t Status \t Solução_Primal \t Solução_Dual \t Solução_Com_Atrazo \t Construcoes_Com_Atrazo \t Demandas_Afetadas \t Valor_Atrazo_Construcoes \t Plantas_Com_Atrazo \t Valor_Atrazo_Plantas \t Gap \t Tempo \n");
			fclose(ArquivoExcelResposta);

			No *Instancia;

			while( !ListaInstancias.empty()){
				Instancia = new No;
				it = ListaInstancias.begin();
				Nome = *it;
				ListaInstancias.pop_front();
				cout << " Modelo <= " << Nome << endl ;

				if( Instancia->LeDados(Nome, EscreveDadosLidosNaTela) == 1){

					resolveu = Instancia->Cplex(Nome, Status, SolucaoPrimal, SolucaoDual, SolucaoReal, ConstrucoesComAtrazo, DemandasAfetadas, ValorAtrazoConstrucoes, PlantasComAtrazo, ValorAtrazoPlantas,  Gap, Tempo);
					cout  << " Resolveu = " << resolveu << endl << endl ;

					ArquivoExcelResposta = fopen(Saida.c_str(), "a");
					fprintf(ArquivoExcelResposta, " %s \t", Nome.c_str());
					//ArquivoExcelResposta << Nome  << '\t' ;

					switch (Status){
						//case 0:	ArquivoExcelResposta <<  "Unknow" << '\t';						break;
						case 0:	fprintf(ArquivoExcelResposta, "Unknow \t");						break;
						//case 1:	ArquivoExcelResposta <<  "Feasible" << '\t';					break;
						case 1:	fprintf(ArquivoExcelResposta, "Feasible \t");						break;
						//case 2:	ArquivoExcelResposta <<  "Optimal" << '\t';						break;
						case 2:	fprintf(ArquivoExcelResposta, "Optimal \t");						break;
						//case 3:	ArquivoExcelResposta <<  "Infeasible" << '\t';					break;
						case 3:	fprintf(ArquivoExcelResposta, "Infeasible \t");						break;
						//case 4:	ArquivoExcelResposta <<  "Unbounded" << '\t';					break;
						case 4:	fprintf(ArquivoExcelResposta, "Unbounded \t");						break;
						//case 5: ArquivoExcelResposta <<  "Infeasible Or Unbounded" << '\t';		break;
						case 5:	fprintf(ArquivoExcelResposta, "Infeasible Or Unbounded \t");						break;
						//default: ArquivoExcelResposta <<  "Erro" << '\t';
						default: fprintf(ArquivoExcelResposta, "Erro \t");
					}
					//ArquivoExcelResposta << " " <<   SolucaoPrimal << '\t' <<  " " << SolucaoDual << '\t' << " " <<   Gap << '\t' <<  " " << Tempo << '\n';
					fprintf(ArquivoExcelResposta, "%.3f \t %.3f \t %.3f \t %d \t %d \t %.3f \t %d \t %.3f \t %.3f \t %.3f \t \n", SolucaoPrimal, SolucaoDual, SolucaoReal,ConstrucoesComAtrazo, DemandasAfetadas, ValorAtrazoConstrucoes, PlantasComAtrazo, ValorAtrazoPlantas, Gap, Tempo);
					fclose(ArquivoExcelResposta);

				}
				//cout << endl << " Antes do free " << Nome <<  endl;

				delete(Instancia);
				//cout << endl << " Depois do free "  << Nome << endl;
			}

			//ArquivoExcelResposta.close();

			cout << " \n Acabou!   Galo Doido! \n";



			return 1;
		}else{
			cout << "\n \n Arquivo inexistente! \n \n";
			return 0;
		}


		ListaInstancias.clear();
	}else{
		cout << "\n \n Passagem de parametros errada \n \n";
		return 0;
	}

}
