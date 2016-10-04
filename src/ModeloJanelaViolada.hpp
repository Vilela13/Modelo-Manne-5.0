/*
 * ModeloJanelaViolada.hpp
 *
 *  Created on: 30/08/2016
 *      Author: mavus
 */

#ifndef MODELOJANELAVIOLADA_HPP_
#define MODELOJANELAVIOLADA_HPP_

#include "Biblioteca.hpp"

#define TipoAEe IloFloatVarArray
#define TipoPEe IloFloatVarArray

#define TipoAPp IloFloatVarArray
#define TipoPPp IloFloatVarArray

#define TipoRoAEe IloBoolVarArray
#define TipoRoPEe IloBoolVarArray

#define TipoRoAPp IloBoolVarArray
#define TipoRoPPp IloFloatVarArray

class ClasseModeloJanelaViolada : public ClasseModelo{
	public:

	vector< double >	PenalidadeDesrespeitoJanelaDeTempoEmpresa;


// Calcula tempos que podem adiantar ou protergar o funcionamento das construções ou plantas

	void CalculaTempoPodeAdiantarOuPostergarEmpresa(int);

// Calcula Penalidades Desrespeito as Janelas De Tempo das Construções e das Plantas
	void CalculaPenalidadeDesrespeitoJanelaDeTempoEmpresa(int, int);

	void CriaAEe(TipoAEe&, int);
	void CriaPEe(TipoPEe&, int);
	void CriaAPp(TipoAPp&, int);
	void CriaPPp(TipoPPp&, int);

	void CriaRoAEe(TipoRoAEe, int);
	void CriaRoPEe(TipoRoPEe, int);
	void CriaRoAPp(TipoRoAPp, int);
	void CriaRoPPp(TipoRoPPp, int);

	// Funções Objetivo
	void FuncaoObjetivo(TipoZe,  TipoRoAEe, TipoRoPEe,  IloModel&, int);



	void Restricao_LimiteDeTempoNaEntrega( TipoTvei, TipoAEe, TipoPEe, IloModel&, int );
	void Restricao_AtribuiValorRoAEePEe( TipoAEe, TipoRoAEe, TipoPEe, TipoRoPEe, IloModel&, int );
	void DominioAEePEe( TipoAEe, TipoPEe, IloModel&, int);

	void EscreveVariaveisAEeRoAEePEeRoPEeDoModeloAposResolucao(int, int, ofstream&, IloCplex,  TipoAEe, TipoRoAEe, TipoPEe, TipoRoPEe );

	void CalculaEntregasComAtrazo( IloCplex, TipoAEe, TipoRoAEe, TipoPEe, TipoRoPEe, int&, int&, double&);

	// Funções que chama o Cplex

	int Cplex(string, int, int&, double&, double&, double&, int&, int&, double&,  double&, double&, vector < string > , vector < double >);

	~ClasseModeloJanelaViolada();       // Destruidora
};

// Calcula tempos que podem adiantar ou protergar o funcionamento das construções ou plantas
void ClasseModeloJanelaViolada::CalculaTempoPodeAdiantarOuPostergarEmpresa(int Escreve){
	TempoPodeAdiantarEmpresa.resize(NE);
	TempoPodePostergarEmpresa.resize(NE);
	for(int e = 0; e < NE; e++ ){
		if( TminE[e] - HorarioFuncionamentoEmpresaQuePodeAdiantar > 0){
			TempoPodeAdiantarEmpresa[e] = TminE[e] - HorarioFuncionamentoEmpresaQuePodeAdiantar;
		}else{
			TempoPodeAdiantarEmpresa[e] = 0;
		}

		if( HorarioFuncionamentoEmpresaQuePodePostergar - TmaxE[e] > 0){
			TempoPodePostergarEmpresa[e] = HorarioFuncionamentoEmpresaQuePodePostergar - TmaxE[e];
		}else{
			TempoPodePostergarEmpresa[e] = 0;
		}

		if(Escreve == 1){
			cout << " Construcao[" << e << "] tempo que pode adiantar => " << TempoPodeAdiantarEmpresa[e] ;
			cout << " tempo que pode postergar => " << TempoPodePostergarEmpresa[e] << endl;
		}
	}
}


// Calcula Penalidades Desrespeito as Janelas De Tempo das Construções e das Plantas
		// TipoPenalidade => 1 penalidade com o valor maximo da janela de tempo
		// TipoPenalidade => 2 penalidade com o valor 400
void ClasseModeloJanelaViolada::CalculaPenalidadeDesrespeitoJanelaDeTempoEmpresa(int TipoPenalidade, int  Escreve){
	PenalidadeDesrespeitoJanelaDeTempoEmpresa.resize(NE);
	double AuxMaiorValor;

	if( TipoPenalidade == 1){
		AuxMaiorValor = 0;
		for( int e = 0; e < NE; e++){
			if( AuxMaiorValor < TmaxE[e]){
				AuxMaiorValor = TmaxE[e];
			}
		}
		for( int e = 0; e < NE; e++){
			PenalidadeDesrespeitoJanelaDeTempoEmpresa[e] = AuxMaiorValor;
			if( Escreve == 1){
				cout << " Penalidade construcao[" << e << "] = " << PenalidadeDesrespeitoJanelaDeTempoEmpresa[e] << endl;
			}
		}
	}

	if( TipoPenalidade == 2){
		for( int e = 0; e < NE; e++){
			PenalidadeDesrespeitoJanelaDeTempoEmpresa[e] = 100000000;
			if( Escreve == 1){
				cout << " Penalidade construcao[" << e << "] = " << PenalidadeDesrespeitoJanelaDeTempoEmpresa[e] << endl;
			}
		}
	}

	if( TipoPenalidade != 1 && TipoPenalidade != 2 ){
		cout << "   **********((((((((  Problema em definir as penalidades das construcoes   -> ClasseModeloJanelaViolada::CalculaPenalidadeDesrespeitoJanelaDeTempoEmpresa ))))))))))************* " << endl;
	}
}



void ClasseModeloJanelaViolada::CriaAEe(TipoAEe& AEe, int  Escreve){
	char varName[24];
	for (int e = 0; e < NE; e++) {
		sprintf(varName, "AEe_%d", e);
		AEe[e] = IloFloatVar(env,varName);
		if ( Escreve == 1){
			cout << " AEe["<< e << "] "<< endl;
		}
	}
	if ( Escreve == 1){
		cout << endl;
	}

}

void ClasseModeloJanelaViolada::CriaPEe(TipoPEe& PEe, int  Escreve){
	char varName[24];
	for (int e = 0; e < NE; e++) {
		sprintf(varName, "PEe_%d", e);
		PEe[e] = IloFloatVar(env,varName);
		if ( Escreve == 1){
			cout << " PEe["<< e << "] "<< endl;
		}
	}
	if ( Escreve == 1){
		cout << endl;
	}
}


void ClasseModeloJanelaViolada::CriaRoAEe(TipoRoAEe	RoAEe,int Escreve){
	char varName[24];
	for (int e = 0; e < NE; e++) {
		sprintf(varName, "RoAEe_%d", e);
		RoAEe[e] = IloBoolVar(env,varName);
		if ( Escreve == 1){
			cout << " RoAEe["<< e << "] "<< endl;
		}
	}
	if ( Escreve == 1){
		cout << endl;
	}

}

void ClasseModeloJanelaViolada::CriaRoPEe(TipoRoPEe	RoPEe,int Escreve){
	char varName[24];
	for (int e = 0; e < NE; e++) {
		sprintf(varName, "RoPEe_%d", e);
		RoPEe[e] = IloBoolVar(env,varName);
		if ( Escreve == 1){
			cout << " RoPEe["<< e << "] "<< endl;
		}
	}
	if ( Escreve == 1){
		cout << endl;
	}
}

// Função Objetivo
void ClasseModeloJanelaViolada::FuncaoObjetivo(TipoZe Ze,  TipoRoAEe RoAEe, TipoRoPEe RoPEe,  IloModel& model, int Imprime){
	int Ativo;
	Ativo = 0;
	if ( Imprime == 1){
		cout << " MIN  ";
	}
	IloExpr funcao_objetivo(env);
	for (int e = 0; e < NE; e++) {
		funcao_objetivo += Ze[e];
		funcao_objetivo += + PenalidadeDesrespeitoJanelaDeTempoEmpresa[e] * RoAEe[e] + PenalidadeDesrespeitoJanelaDeTempoEmpresa[e] * RoPEe[e];
		if ( Imprime == 1){
			if( Ativo == 1){
				cout << "+";
			}
			cout << " Ze[" << e << "] + PenalidadeDesrespeitoJanelaDeTempoEmpresa[" << e << "] * RoAEe[" << e << "] + PenalidadeDesrespeitoJanelaDeTempoEmpresa[" << e << "] * RoPEe[" << e << "] ";
			Ativo = 1;
		}
	}
	if ( Imprime == 1){
		cout << endl;
	}
	IloObjective obj = IloMinimize(env, funcao_objetivo);
	model.add(obj);
	funcao_objetivo.end();
}


// restrição 14 e 15
void ClasseModeloJanelaViolada::Restricao_LimiteDeTempoNaEntrega( TipoTvei Tvei, TipoAEe AEe,TipoPEe PEe, IloModel& model, int EscreveRestricao){
	IloRangeArray Restricao14;
	IloRangeArray Restricao15;
	char varName[40];

	int NumAux;
	NumAux = 0;
	for (int v = 0; v < NV; v++) {
		for (int e1 = 0; e1 < NE; e1++) {
			for (int i = 0; i < TCDE[e1]; i++) {
				NumAux++;
			}
		}
	}

	Restricao14 = IloRangeArray(env, NumAux);
	Restricao15 = IloRangeArray(env, NumAux);

	NumAux = 0;

	for (int v = 0; v < NV; v++) {
		for (int e1 = 0; e1 < NE; e1++) {
			for (int i = 0; i < TCDE[e1]; i++) {
				// declara expressão
				IloExpr expr1(env);
				IloExpr expr2(env);

				if ( EscreveRestricao == 1){
					cout << "  Tvei[" << v << "][" << e1 << "][" << i << "] >= TminE[" << e1 <<"] - AEe[" << e1 << "]" << endl;
				}
				expr1 += Tvei[v][e1][i] - TminE[e1] + AEe[e1];
				Restricao14[NumAux] = expr1 >= 0;

				sprintf(varName,"Rest14_TviMin_v%de%di%d", v, e1, i);
				Restricao14[NumAux].setName(varName);

				model.add(Restricao14[NumAux]);
				//model.add( Tvei[v][e1][i] >= TminE[e1] - AEe[e1] );

				if ( EscreveRestricao == 1){
					cout << "  Tvei[" << v << "][" << e1 << "][" << i << "] <= TmaxE[" << e1 <<"] + PEe["<< e1 << "]" << endl;
				}
				expr2 += Tvei[v][e1][i] - TmaxE[e1] - PEe[e1];
				Restricao15[NumAux] = expr2 <= 0;

				sprintf(varName,"Rest15_TviMax_v%de%di%d", v, e1, i);
				Restricao15[NumAux].setName(varName);

				model.add(Restricao15[NumAux]);

				//model.add(  Tvei[v][e1][i] <= TmaxE[e1] + PEe[e1] );

				expr1.end();
				expr2.end();
				NumAux++;
			}
		}
	}
}

// restrição 18 e 19
void ClasseModeloJanelaViolada::Restricao_AtribuiValorRoAEePEe( TipoAEe AEe, TipoRoAEe RoAEe, TipoPEe PEe, TipoRoPEe RoPEe, IloModel& model, int EscreveRestricao ){
	IloRangeArray Restricao18;
	IloRangeArray Restricao19;
	char varName[40];

	Restricao18 = IloRangeArray(env, NE);
	Restricao19 = IloRangeArray(env, NE);

	for (int e = 0; e < NE; e++) {
		// declara expressão
		IloExpr expr1(env);
		IloExpr expr2(env);

		if ( EscreveRestricao == 1){
			cout << " AEe[" << e << "]  - TempoPodeAdiantarEmpresa[ " << e << "] * RoAEe[" << e << "] <= 0 " << endl;
		}
		expr1 += AEe[e]  - TempoPodeAdiantarEmpresa[e] * RoAEe[e];
		Restricao18[e] = expr1 <= 0;

		sprintf(varName,"Rest18_AtivaRoAE_e%d",e);
		Restricao18[e].setName(varName);

		model.add(Restricao18[e]);

		//model.add( AEe[e]  - TempoPodeAdiantarEmpresa[e] * RoAEe[e] <= 0 );

		if ( EscreveRestricao == 1){
			cout << " PEe[" << e << "]  - TempoPodePostergarEmpresa[ " << e << "] * RoPEe[" << e << "] <= 0 " << endl;
		}
		expr2 += PEe[e]  - TempoPodePostergarEmpresa[e] * RoPEe[e];
		Restricao19[e] = expr2 <= 0;

		sprintf(varName,"Rest19_AtivaRoPE_e%d",e);
		Restricao19[e].setName(varName);

		model.add(Restricao19[e]);

		//model.add( PEe[e]  - TempoPodePostergarEmpresa[e] * RoPEe[e] <= 0 );
		expr1.end();
		expr2.end();
	}
}


// Dominio Variaveis AEe e PEe ( restrição 22 e 23 )
void ClasseModeloJanelaViolada::DominioAEePEe( TipoAEe AEe, TipoPEe PEe, IloModel& model, int EscreveRestricao ){
	IloRangeArray Restricao22;
	IloRangeArray Restricao23;
	char varName[40];

	Restricao22 = IloRangeArray(env, NE);
	Restricao23 = IloRangeArray(env, NE);
	for (int e = 0; e < NE; e++) {
		// declara expressão
		IloExpr expr1(env);
		IloExpr expr2(env);
		if ( EscreveRestricao == 1){
			cout << " AEe[" << e << "] >= 0 " << endl;
		}

		expr1 += AEe[e];
		Restricao22[e] = expr1 >= 0;

		sprintf(varName,"Rest22_DominioAE_e%d",e);
		Restricao22[e].setName(varName);

		model.add(Restricao22[e]);
		//model.add( AEe[e] >= 0 );

		if ( EscreveRestricao == 1){
			cout << " PEe[" << e << "] >= 0 " << endl;
		}
		expr2 += PEe[e];
		Restricao23[e] = expr2 >= 0;

		sprintf(varName,"Rest23_DominioPE_e%d",e);
		Restricao23[e].setName(varName);

		model.add(Restricao23[e]);
		//model.add( PEe[e] >= 0 );
		expr1.end();
		expr2.end();
	}
}


void ClasseModeloJanelaViolada::EscreveVariaveisAEeRoAEePEeRoPEeDoModeloAposResolucao(int EscreveArquivoComRespostas, int EscreveNaTelaResultados,ofstream& logfile2, IloCplex cplex,  TipoAEe AEe, TipoRoAEe RoAEe, TipoPEe PEe, TipoRoPEe RoPEe ){
	if( EscreveNaTelaResultados == 1){
		cout << " Construcoes " << endl;
	}
	if( EscreveArquivoComRespostas == 1){
		logfile2 << " Construcoes "<< endl;
	}
	for (int e = 0; e< NE; e++) {
		if( EscreveNaTelaResultados == 1){
			cout << AEe[e].getName();
			cout << " [" << cplex.getValue(AEe[e]) << "]  ";
			cout << RoAEe[e].getName();
			cout << " [" << cplex.getValue(RoAEe[e]) << "]  ";
			cout << endl;
		}
		if( EscreveArquivoComRespostas == 1){
			logfile2  << AEe[e].getName();
			logfile2  << " [" << cplex.getValue(AEe[e]) << "]  ";
			logfile2  << RoAEe[e].getName();
			logfile2  << " [" << cplex.getValue(RoAEe[e]) << "]  ";
			logfile2  << endl;
		}

	}
	for (int e = 0; e< NE; e++) {
		if( EscreveNaTelaResultados == 1){
			cout << PEe[e].getName();
			cout << " [" << cplex.getValue(PEe[e]) << "]  ";
			cout << RoPEe[e].getName();
			cout << " [" << cplex.getValue(RoPEe[e]) << "]  ";
			cout << endl;
		}
		if( EscreveArquivoComRespostas == 1){
			logfile2  << PEe[e].getName();
			logfile2  << " [" << cplex.getValue(PEe[e]) << "]  ";
			logfile2  << RoPEe[e].getName();
			logfile2  << " [" << cplex.getValue(RoPEe[e]) << "]  ";
			logfile2  << endl;
		}
	}
	if( EscreveNaTelaResultados == 1 ){
		cout << endl;
	}
	if( EscreveArquivoComRespostas == 1){
		logfile2 << endl;
	}

}

void ClasseModeloJanelaViolada::CalculaEntregasComAtrazo(  IloCplex cplex, TipoAEe AEe, TipoRoAEe RoAEe, TipoPEe PEe, TipoRoPEe RoPEe, int& ConstrucoesComAtrazo, int& DemandasAfetadas, double& ValorAtrazoConstrucoes){
	ValorAtrazoConstrucoes = 0;
	ConstrucoesComAtrazo = 0;
	DemandasAfetadas = 0;
	for( int e = 0; e < NE; e++){
		if( cplex.getValue(RoAEe[e]) == 1){
			ValorAtrazoConstrucoes = ValorAtrazoConstrucoes + cplex.getValue(AEe[e]);
		}
		if( cplex.getValue(RoPEe[e]) == 1){
			ValorAtrazoConstrucoes = ValorAtrazoConstrucoes + cplex.getValue(PEe[e]);
		}
		if( cplex.getValue(RoAEe[e]) == 1 || cplex.getValue(RoPEe[e]) == 1 ){
			ConstrucoesComAtrazo++;
			DemandasAfetadas = DemandasAfetadas + TCDE[e];
		}
	}
}


// Resolve modelo
int ClasseModeloJanelaViolada::Cplex(string Nome, int  TempoExecucao, int &status, double &primal, double &dual, double& SolucaoReal, int& ConstrucoesComAtrazo, int& DemandasAfetadas, double& ValorAtrazoConstrucoes,  double &gap, double &tempo,  vector < string > NomeInstanciaLimiteUpper, vector < double > ValorLimiteUpper){

	int Escreve;				// Escreve variaveis criadas

	int EscreveVariaveis;
	int OutPut1;
	int OutPut2;
	int SaidaPastaSeparada;
	int EscreveArquivoComRespostas;
	int EscreveNaTelaResultados;
	vector < int > EscreveRestricao;
	EscreveRestricao.resize(20+1);
	for( int i = 0; i <= 20; i++){
		EscreveRestricao[i] = 0;
	}

	vector < int > TamanhoPenalidade;
	TamanhoPenalidade.resize(2+1);
	for( int i = 0; i <= 2; i++){
		TamanhoPenalidade[i] = 2;
	}

	Escreve = 0;				// Escreve as variaveis criadas
	EscreveVariaveis = 1;
	OutPut1 = 1;
	OutPut2 = 1;
	SaidaPastaSeparada = 1;
	EscreveArquivoComRespostas = 1;
	EscreveNaTelaResultados = 0;

// Começa a escrever modelo do Cplex
	IloModel 	model(env);

/* Declara variaveis */

// Variavel ALFA
	TipoAlfa 	Alfa(env, NV);
	CriaAlfa(Alfa,Escreve);

// Variavel BETA
	TipoBeta 	Beta(env, NV);
	CriaBeta(Beta,Escreve);

// Variavel BETAProducao
	TipoBeta 	BetaProducao(env, NV);
	CriaBetaProducao(BetaProducao,Escreve);


// Variavel Ze
	TipoZe 		Ze(env,NE);
	CriaZe(Ze ,Escreve);



// Variavel Tvei
	TipoTvei 	Tvei(env,NV);
	CriaTvei( Tvei, Escreve);

// Variavel TPvei
	TipoTPvei 	TPvei(env,NV);
	CriaTPvei( TPvei, Escreve);

// Variaveis de adiantamento e postergamento dos limites de tempo


	TipoAEe		AEe(env,NE);		// Tempo de adiantamento do tempo limite da contrução e
	CriaAEe(AEe, Escreve);

	TipoPEe		PEe(env,NE);		// Tempo de postergamento do tempo limite da contrução e
	CriaPEe(PEe, Escreve);



	TipoRoAEe RoAEe(env,NE);		// Variavel se teve ou não adiantamento do tempo limite da contrução e
	CriaRoAEe(RoAEe,  Escreve);

	TipoRoPEe RoPEe(env,NE);		// Variavel se teve ou não postergamento do tempo limite da contrução e
	CriaRoPEe(RoPEe,  Escreve);



// Calcula variaveis de adiantamento e postergamento dos limites de tempo
	CalculaTempoPodeAdiantarOuPostergarEmpresa(Escreve);

// Calcula Penalidades Desrespeito as Janelas De Tempo das Construções e das Plantas
	CalculaPenalidadeDesrespeitoJanelaDeTempoEmpresa(TamanhoPenalidade[1], Escreve);

// Funcao Objetivo
	FuncaoObjetivo(Ze, RoAEe, RoPEe, model, EscreveRestricao[0]);

// Restrição 1 : Antendimento das Demandas
	Restricao_AtendimentoDasDemandas(Alfa, model, EscreveRestricao[1]);
// Restrição  2 : Lower bound Ze
	Restricao_LowerBoundZe(Ze, Tvei, Alfa, model, EscreveRestricao[2]);
// Restrição  3 : Vinculo Tvei e TPvei
	Restricao_VinculoTveiTPvei( Alfa, TPvei, Tvei,model, EscreveRestricao[3] );
// Restrição  5 e 6 : de precedencia Tvei
	Restricao_PrecedenciaTvei(  Alfa, Beta, Tvei, model,  EscreveRestricao[5], EscreveRestricao[6]);
// Restrição  7:
	Restricao_TempoMaximoEntreDescarregamentosSeguidosNaMesmaEntrega( Alfa, Tvei, model, EscreveRestricao[7] );
// Restrição 8
    Restricao_TempoMinimoEntreDescarregamentosSeguidosNaMesmaEntrega( Alfa, Tvei, model, EscreveRestricao[10] );
// Restrição 9
    Restricao_PrecedenciaTPvei( Alfa, BetaProducao, TPvei, model, EscreveRestricao[11],EscreveRestricao[12]);
// Restrição 10
    Restricao_TempoDeVidaDoConcreto( Alfa,Tvei,TPvei, model,EscreveRestricao[13]);
// Restrição  11
	Restricao_LimiteDeTempoNaEntrega( Tvei, AEe, PEe, model, EscreveRestricao[14] );
// Restrição  12
	Restricao_LimiteDeTempoNaPlanta(  TPvei, model, EscreveRestricao[15] );
// Restrição  13
	Restricao_AtribuiValorRoAEePEe( AEe, RoAEe, PEe, RoPEe, model, EscreveRestricao[16]);

// Dominio AEe e PEe
	DominioAEePEe( AEe, PEe, model, EscreveRestricao[18]);




// Modelo
	IloCplex *cplex;

	cplex = new IloCplex(model);
	//cplex->exportModel("model.lp");

// Cria pasta OUT
	VerificaOuCriaPastaOut(EscreveNaTelaResultados);

	Nome1 = "./Out/";
	Nome1 += Nome;

	//cout << endl << endl << " arquivo a gravar a saida " << c1 << endl << endl;

	ofstream logfile1(Nome1.c_str());

	if(SaidaPastaSeparada == 1){
		cplex->setOut(logfile1);
	}
	cplex->setParam(IloCplex::TiLim,  TempoExecucao);
	cplex->setParam(IloCplex::Threads, 12);

	for( int i = 0; i < (int) NomeInstanciaLimiteUpper.size(); i++){

		if( Nome.compare(NomeInstanciaLimiteUpper[i].c_str()) == 0){
			//cout << endl << endl << "		Usou o Limite superior em " << NomeInstanciaLimiteUpper[i] << " no valor de " << ValorLimiteUpper[i] << endl << endl;
			cplex->setParam(IloCplex::Param::MIP::Tolerances::UpperCutoff, ValorLimiteUpper[i]);
		}
	}

	Tempo1 = cplex->getCplexTime();

	//cout << endl << " setou tempo" << endl << endl;
	primal = -1;
	dual = -1;
	SolucaoReal = -1;
	ConstrucoesComAtrazo = -1;
	DemandasAfetadas = -1;
	ValorAtrazoConstrucoes = -1;
	gap = -1;

// Resolve o modelo.
	if (!cplex->solve()) {
		cerr << "Failed to optimize LP." << endl;
		status = cplex->getStatus();
		cout << " status = (" << status << ")" << endl;
		tempo = cplex->getCplexTime() - Tempo1;
		logfile1.close();
		//throw(-1);                                                   // Olhar!!!!!!!!!!!!!!!!!!

		//model.end();				// problema, trava o programa. olhar! falam que demora muito. que é melhor deletar o objeto IloClpex

		delete(cplex);

		Alfa.clear();
		Beta.clear();
		BetaProducao.clear();
		Ze.clear();
		Tvei.clear();
		TPvei.clear();
		TPvei.clear();
		EscreveRestricao.clear();

		AEe.clear();
		PEe.clear();

		RoAEe.clear();
		RoPEe.clear();


		return (0);
	}else{

		//cout << endl << " rodou " << endl << endl;

		Tempo2 = cplex->getCplexTime();

		VerificaOuCriaPastaSol(EscreveNaTelaResultados);

		Nome2 = "./Sol/";
		Nome2 += Nome;

		//cout << endl << endl << " arquivo a gravar a solucao " << Nome2 << endl << endl;

		ofstream logfile2(Nome2.c_str());

		status = cplex->getStatus();
		primal = cplex->getObjValue();
		dual = cplex->getBestObjValue();
		if( cplex->getStatus() == 1 || cplex->getStatus() == 2 || cplex->getStatus() == 4 ){
			CalculaFuncaoObjetivo(		*cplex, Ze,  SolucaoReal);
			CalculaEntregasComAtrazo(	*cplex, AEe, RoAEe, PEe, RoPEe, ConstrucoesComAtrazo, 	DemandasAfetadas, ValorAtrazoConstrucoes);
		}
		gap =  100 * ( cplex->getObjValue() - cplex->getBestObjValue() ) / cplex->getObjValue();
		tempo = Tempo2 - Tempo1;

		if( EscreveNaTelaResultados == 1){
			cout << "Solution status = " << status << " [" << cplex->getStatus() << "] "<< endl;
			cout << "Solution primal cost = " << primal << endl;
			cout << "Solution dual cost = " << dual << endl ;
			cout << "Soution with delay = " << SolucaoReal << endl;
			cout << "Cosntructions afected by the delay = " << ConstrucoesComAtrazo << endl;
			cout << "Demands afected by the delay = " << DemandasAfetadas << endl;
			cout << "Constructions's Delay = " << ValorAtrazoConstrucoes << endl;
			cout << "Gap = " << gap << "%" << endl ;
			cout << "Tempo = " << tempo << " segundos. " << endl<< endl;
		}

		if( EscreveArquivoComRespostas == 1){
			logfile2 <<  "Solution status = " << " [" << cplex->getStatus() << "] "<< endl;
			logfile2 << "Solution primal cost = " << primal << endl;
			logfile2 << "Solution dual cost = " << dual << endl ;
			logfile2 << "Soution with delay = " << SolucaoReal << endl;
			logfile2 << "Cosntructions afected by the delay = " << ConstrucoesComAtrazo << endl;
			logfile2 << "Demands afected by the delay = " << DemandasAfetadas << endl;
			logfile2 << "Constructions's Delay = " << ValorAtrazoConstrucoes << endl;
			logfile2 << "Gap = " << gap  << "%" << endl ;
			logfile2 << "Tempo = " << tempo << " segundos. " << endl << endl;
		}

		if( EscreveVariaveis == 1){
	// Imprimi Variaveis
			EscreveVariaveisAlfaDoModeloAposResolucao(				EscreveArquivoComRespostas, EscreveNaTelaResultados, logfile2, *cplex, Alfa);
			EscreveVariaveisBetaDoModeloAposResolucao(				EscreveArquivoComRespostas, EscreveNaTelaResultados, logfile2, *cplex, Beta);
			EscreveVariaveisBetaProducaoDoModeloAposResolucao(		EscreveArquivoComRespostas, EscreveNaTelaResultados, logfile2, *cplex, BetaProducao);
			EscreveVariaveisTveiDoModeloAposResolucao(				EscreveArquivoComRespostas, EscreveNaTelaResultados, logfile2, *cplex, Tvei);
			EscreveVariaveisTPveiDoModeloAposResolucao(				EscreveArquivoComRespostas, EscreveNaTelaResultados, logfile2, *cplex, TPvei);
			EscreveVariaveisZeDoModeloAposResolucao(				EscreveArquivoComRespostas, EscreveNaTelaResultados, logfile2, *cplex, Ze);

			EscreveVariaveisAEeRoAEePEeRoPEeDoModeloAposResolucao( 	EscreveArquivoComRespostas, EscreveNaTelaResultados, logfile2, *cplex, AEe, RoAEe, PEe, RoPEe );


		}

		if( OutPut1 == 1){
	// Itinerario dos veiculos
			EscreveItinerarioVeiculos( EscreveNaTelaResultados, EscreveArquivoComRespostas, logfile2,	*cplex, Alfa,  Tvei, TPvei);
		}

		if( OutPut2 == 1){
	// Tempo de cada entrega em cada cliente
			EscreveEntregasNosClientes(EscreveNaTelaResultados,EscreveArquivoComRespostas, logfile2,	*cplex, Alfa,  Tvei);
	// Veiculos usados
			EscreveUtilizacaoVeiculos( EscreveNaTelaResultados,EscreveArquivoComRespostas, logfile2,	*cplex, Alfa,  Tvei);
		}

		logfile1.close();
		logfile2.close();

		//model.end();				// problema, trava o programa. olhar! falam que demora muito. que é melhor deletar o objeto IloClpex

		delete(cplex);

		Alfa.clear();
		Beta.clear();
		BetaProducao.clear();
		Ze.clear();
		Tvei.clear();
		TPvei.clear();
		TPvei.clear();
		EscreveRestricao.clear();

		AEe.clear();
		PEe.clear();


		RoAEe.clear();
		RoPEe.clear();


		return (1);
	}
}

ClasseModeloJanelaViolada::~ClasseModeloJanelaViolada(){












	PenalidadeDesrespeitoJanelaDeTempoEmpresa.clear();



}


#endif /* MODELOJANELAVIOLADA_HPP_ */
