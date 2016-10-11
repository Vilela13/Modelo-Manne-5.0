#include "Biblioteca.hpp"


int main(int argc, char **argv) {


	if( argc == 4){

#include "DeclaracaoVariaveisMain.hpp"




		EscreveDadosLidosNaTela = 1;
		ComViolacao = 1;

	//Cria instancia manual
	    //No PreInstancia;

//Cria instância que criei na mão
		//PreInstancia.PreencheEstrutura();
		//PreInstancia.CriaTXT();


// *************** Pega valores de um arquivo dos Upperbound das intâncias ***************
		TemLimites = 0;
		UpperBounds.open("ValoresLimitesUpper.txt");
		if ( UpperBounds.is_open() ){
			TemLimites = 1;

			UpperBounds >> NomeInstanciaUpper;
			while( NomeInstanciaUpper != "EOF"){
				//cout << " coloca lista = " << Nome  << endl ;
				UpperBounds >> ValorUpper;
				//cout << "   " << NomeInstanciaUpper << " : " << ValorUpper << endl;

				if(ValorUpper.compare("------") == 0 ){
					//cout << "  Sem resposta" << endl;
				}else{
					NomeInstanciaLimiteUpper.push_back(NomeInstanciaUpper);
					ValorDoubleUpper = atof(ValorUpper.c_str());
					//cout << "            " << NomeInstanciaUpper << " valor double : " << ValorDoubleUpper << endl;
					ValorLimiteUpper.push_back(ValorDoubleUpper);
				}
				UpperBounds >> NomeInstanciaUpper;
			}
		}

		//cout << endl<< endl<< " Passou limites :" << TemLimites << endl<< endl;
		if( TemLimites == 1){
			for( int i = 0; i < (int) NomeInstanciaLimiteUpper.size(); i++){
				//cout << NomeInstanciaLimiteUpper[i] << " => " << ValorLimiteUpper[i] << endl;
			}
		}

// *****************************************************************************************
	// Resolve o problema


		TipoDeEntrada = argv[1];
		Instancias = argv[2];
		if( strcmp(argv[3],"-") == 0 ){
			TempoExecucao = INT_MAX;
		}else{
			TempoExecucao = atoi( argv[3] ) ;
		}

		if( TipoDeEntrada.compare(0,3,"arq") == 0 ){
			ArquivoInstancia.open(Instancias.c_str());
			if ( ArquivoInstancia.is_open() ){
				ArquivoInstancia >> Nome;
				while( Nome != "EOF"){
					//cout << " coloca lista = " << Nome  << endl ;
					ListaInstancias.push_back(Nome);
					ArquivoInstancia >> Nome;

				}
				ArquivoInstancia.close();
			}
		}else{
			if( TipoDeEntrada.compare(0,4,"inst") == 0 ){
				ListaInstancias.push_back(Instancias);
			}else{
				cout << "(" << TipoDeEntrada << ")";
				printf( " TipoDeEntrada  -> Problema na definição da entrada das instancias. \n\n\n");
				ListaInstancias.clear();
				Nome.clear();
				Instancias.clear();
				Saida.clear();
				return 0;
			}
		}



		// ----------- Le um arquivo com as instancias a serem resolvidas pelo modelo, abre o arquivo com a instancia e o resolve -------------------------- //

			//cout << endl << endl << " Lendo arquivo " << endl << endl << endl;


		Saida = "R-";				// coloca Res- no char*
		if( TipoDeEntrada.compare(0,3,"arq") == 0 ){
			Saida += Instancias;
		}else{
			Saida += "Instancias.txt";
		}

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


		if( TipoDeEntrada.compare("arq") == 0 ){
			fprintf(ArquivoExcelResposta,"%s \n", buffer);
			fprintf(ArquivoExcelResposta, "Instância \t Status \t Solução_Primal \t Solução_Dual \t Solução_Com_Atrazo \t Construcoes_Com_Atrazo \t Demandas_Afetadas  \t Plantas_Com_Atrazo \t Valor_Atrazo_Plantas \t Gap \t Tempo \n");
		}
		fclose(ArquivoExcelResposta);



		while( !ListaInstancias.empty()){

			if( ComViolacao == 0){
				InstanciaSemViolacao = new ClasseModelo;
				it = ListaInstancias.begin();
				Nome = *it;
				ListaInstancias.pop_front();
				cout << " Modelo <= " << Nome << endl ;

				if( InstanciaSemViolacao->LeDados(Nome, EscreveDadosLidosNaTela) == 1){

					resolveu = InstanciaSemViolacao->Cplex(Nome,  TempoExecucao, Status, SolucaoPrimal, SolucaoDual, SolucaoReal,   Gap, Tempo, NomeInstanciaLimiteUpper, ValorLimiteUpper);
					cout  << " Resolveu = " << resolveu << endl << endl ;

					ArquivoExcelResposta = fopen(Saida.c_str(), "a");
					fprintf(ArquivoExcelResposta, " %s \t", Nome.c_str());
					//ArquivoExcelResposta << Nome  << '\t' ;

					DescobreStatus( Status, ArquivoExcelResposta);

					//ArquivoExcelResposta << " " <<   SolucaoPrimal << '\t' <<  " " << SolucaoDual << '\t' << " " <<   Gap << '\t' <<  " " << Tempo << '\n';
					fprintf(ArquivoExcelResposta, "%.3f \t %.3f \t %.3f \t %.3f \t %.3f \t \n", SolucaoPrimal, SolucaoDual, SolucaoReal, Gap, Tempo);
					fclose(ArquivoExcelResposta);

				}
				//cout << endl << " Antes do free " << Nome <<  endl;

				delete(InstanciaSemViolacao);
				//cout << endl << " Depois do free "  << Nome << endl;

			}else{

				InstanciaComViolacao = new ClasseModeloJanelaViolada ;
				it = ListaInstancias.begin();
				Nome = *it;
				ListaInstancias.pop_front();
				cout << " Modelo <= " << Nome << endl ;

				if( InstanciaComViolacao->LeDados(Nome, EscreveDadosLidosNaTela) == 1){

					resolveu = InstanciaComViolacao->Cplex(Nome,  TempoExecucao, Status, SolucaoPrimal, SolucaoDual, SolucaoReal, ConstrucoesComAtrazo, DemandasAfetadas,   Gap, Tempo, NomeInstanciaLimiteUpper, ValorLimiteUpper);
					cout  << " Resolveu = " << resolveu << endl << endl ;

					ArquivoExcelResposta = fopen(Saida.c_str(), "a");
					fprintf(ArquivoExcelResposta, " %s \t", Nome.c_str());
					//ArquivoExcelResposta << Nome  << '\t' ;

					DescobreStatus( Status, ArquivoExcelResposta);

					//ArquivoExcelResposta << " " <<   SolucaoPrimal << '\t' <<  " " << SolucaoDual << '\t' << " " <<   Gap << '\t' <<  " " << Tempo << '\n';
					fprintf(ArquivoExcelResposta, "%.3f \t %.3f \t %.3f \t %d \t %d \t %.3f \t %d \t %.3f \t %.3f \t %.3f \t \n", SolucaoPrimal, SolucaoDual, SolucaoReal,ConstrucoesComAtrazo, DemandasAfetadas, ValorAtrazoConstrucoes, PlantasComAtrazo, ValorAtrazoPlantas, Gap, Tempo);
					fclose(ArquivoExcelResposta);

				}
				//cout << endl << " Antes do free " << Nome <<  endl;

				delete(InstanciaComViolacao);
				//cout << endl << " Depois do free "  << Nome << endl;
			}

		}

		//ArquivoExcelResposta.close();

		if( TipoDeEntrada.compare("arq") == 0 ){
			cout << " \n Acabou!   Galo Doido! \n";
		}



		ListaInstancias.clear();
		Nome.clear();
		Instancias.clear();
		Saida.clear();
		return 1;

	}else{
		cout << "\n \n Passagem de parametros errada \n \n";
		return 0;
	}

}
