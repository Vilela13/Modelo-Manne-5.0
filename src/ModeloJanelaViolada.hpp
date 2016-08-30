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
	vector< double >	PenalidadeDesrespeitoJanelaDeTempoPlanta;


// Calcula tempos que podem adiantar ou protergar o funcionamento das construções ou plantas

	void CalculaTempoPodeAdiantarOuPostergarEmpresa(int);
	void CalculaTempoPodeAdiantarOuPostergarPlantas(int);

// Calcula Penalidades Desrespeito as Janelas De Tempo das Construções e das Plantas
	void CalculaPenalidadeDesrespeitoJanelaDeTempoEmpresa(int, int);
	void CalculaPenalidadeDesrespeitoJanelaDeTempoPlanta(int, int);

	void CriaAEe(TipoAEe&, int);
	void CriaPEe(TipoPEe&, int);
	void CriaAPp(TipoAPp&, int);
	void CriaPPp(TipoPPp&, int);

	void CriaRoAEe(TipoRoAEe, int);
	void CriaRoPEe(TipoRoPEe, int);
	void CriaRoAPp(TipoRoAPp, int);
	void CriaRoPPp(TipoRoPPp, int);

	// Funções Objetivo
	void FuncaoObjetivo(TipoZe, TipoZr, TipoRoAEe, TipoRoPEe, TipoRoAPp, TipoRoPPp, IloModel&, int);

	void Restricao_LowerBoundZr( TipoZr, TipoTvei, TipoAlfa, TipoRoAPp, IloModel&, int);

	void Restricao_LimiteDeTempoNaEntrega( TipoTvei, TipoAEe, TipoPEe, IloModel&, int );
	void Restricao_LimiteDeTempoNaPlanta( TipoTvei, TipoAPp, TipoPPp, IloModel&, int);
	void Restricao_AtribuiValorRoAEePEe( TipoAEe, TipoRoAEe, TipoPEe, TipoRoPEe, IloModel&, int );
	void Restricao_AtribuiValorRoAPpPPp( TipoAPp, TipoRoAPp, TipoPPp, TipoRoPPp, IloModel&, int );
	void DominioAEePEe( TipoAEe, TipoPEe, IloModel&, int);
	void DominioAPpPPp( TipoAPp, TipoPPp, IloModel&, int);

	void EscreveVariaveisAEeRoAEePEeRoPEeDoModeloAposResolucao(int, int, ofstream&, IloCplex,  TipoAEe, TipoRoAEe, TipoPEe, TipoRoPEe );
	void EscreveVariaveisAPpRoAPpPPpRoPPpDoModeloAposResolucao(int, int, ofstream&, IloCplex,  TipoAPp, TipoRoAPp, TipoPPp, TipoRoPPp );

	void CalculaEntregasComAtrazo( IloCplex, TipoAEe, TipoRoAEe, TipoPEe, TipoRoPEe, int&, int&, double&);
	void CalculaPlantasComAtrazo( IloCplex, TipoAPp, TipoRoAPp, TipoPPp, TipoRoPPp, int&, double&);

	// Funções que chama o Cplex

	int Cplex(string, int, int&, double&, double&, double&, int&, int&, double&, int&, double&s, double&, double&, vector < string > , vector < double >);

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
void ClasseModeloJanelaViolada::CalculaTempoPodeAdiantarOuPostergarPlantas(int Escreve){
	TempoPodeAdiantarPlanta.resize(NP);
	TempoPodePostergarPlanta.resize(NP);
	for(int p = 0; p < NP; p++ ){
		if( TminP[p] - HorarioFuncionamentoPlantaQuePodeAdiantar > 0){
			TempoPodeAdiantarPlanta[p] = TminP[p] - HorarioFuncionamentoPlantaQuePodeAdiantar;
		}else{
			TempoPodeAdiantarPlanta[p] = 0;
		}

		if( HorarioFuncionamentoPlantaQuePodePostergar - TmaxP[p] > 0){
			TempoPodePostergarPlanta[p] = HorarioFuncionamentoPlantaQuePodePostergar - TmaxP[p];
		}else{
			TempoPodePostergarPlanta[p] = 0;
		}
		if(Escreve == 1){
			cout << " Planta[" << p << "] tempo que pode adiantar => " << TempoPodeAdiantarPlanta[p] ;
			cout << " tempo que pode postergar => " << TempoPodePostergarPlanta[p] << endl;
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

void ClasseModeloJanelaViolada::CalculaPenalidadeDesrespeitoJanelaDeTempoPlanta(int TipoPenalidade, int  Escreve){
	PenalidadeDesrespeitoJanelaDeTempoPlanta.resize(NP);
	double AuxMaiorValor;

	if( TipoPenalidade == 1){
		AuxMaiorValor = 0;
		for( int p = 0; p < NP; p++){
			if( AuxMaiorValor < TmaxP[p]){
				AuxMaiorValor = TmaxP[p];
			}
		}
		for( int p = 0; p < NP; p++){
			PenalidadeDesrespeitoJanelaDeTempoPlanta[p] = AuxMaiorValor;
			if( Escreve == 1){
				cout << " Penalidade planta[" << p << "] = " << PenalidadeDesrespeitoJanelaDeTempoPlanta[p] << endl;
			}
		}
	}

	if( TipoPenalidade == 2){
		for( int p = 0; p < NP; p++){
			PenalidadeDesrespeitoJanelaDeTempoPlanta[p] = 100000000;
			if( Escreve == 1){
				cout << " Penalidade planta[" << p << "] = " << PenalidadeDesrespeitoJanelaDeTempoPlanta[p] << endl;
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

void ClasseModeloJanelaViolada::CriaAPp(TipoAPp& APp, int Escreve){
	char varName[24];
	for (int p = 0; p < NP; p++) {
		sprintf(varName, "APp_%d", p);
		APp[p] = IloFloatVar(env,varName);
		if ( Escreve == 1){
			cout << " APp["<< p << "] "<< endl;
		}
	}
	if ( Escreve == 1){
		cout << endl;
	}
}

void ClasseModeloJanelaViolada::CriaPPp(TipoPPp& PPp, int Escreve){
	char varName[24];
	for (int p = 0; p < NP; p++) {
		sprintf(varName, "PPp_%d", p);
		PPp[p] = IloFloatVar(env,varName);
		if ( Escreve == 1){
			cout << " PPp["<< p << "] "<< endl;
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

void ClasseModeloJanelaViolada::CriaRoAPp(TipoRoAPp	RoAPp,int Escreve){
	char varName[24];
	for (int p = 0; p < NP; p++) {
		sprintf(varName, "RoAPp_%d", p);
		RoAPp[p] = IloBoolVar(env,varName);
		if ( Escreve == 1){
			cout << " RoAPp["<< p << "] "<< endl;
		}
	}
	if ( Escreve == 1){
		cout << endl;
	}
}

void ClasseModeloJanelaViolada::CriaRoPPp(TipoRoPPp	RoPPp,int Escreve){
	char varName[24];
	for (int p = 0; p < NP; p++) {
		sprintf(varName, "RoPPp_%d", p);
		RoPPp[p] = IloBoolVar(env,varName);
		if ( Escreve == 1){
			cout << " RoPPp["<< p << "] "<< endl;
		}
	}
	if ( Escreve == 1){
		cout << endl;
	}
}

// Função Objetivo
void ClasseModeloJanelaViolada::FuncaoObjetivo(TipoZe Ze, TipoZr Zr, TipoRoAEe RoAEe, TipoRoPEe RoPEe, TipoRoAPp RoAPp, TipoRoPPp RoPPp, IloModel& model, int Imprime){
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
	for (int p = 0; p < NP; p++) {
		funcao_objetivo += Zr[p];
		funcao_objetivo +=  + PenalidadeDesrespeitoJanelaDeTempoPlanta[p] * RoAPp[p] + PenalidadeDesrespeitoJanelaDeTempoPlanta[p] * RoPPp[p];
		if ( Imprime == 1){
			if( Ativo == 1){
				cout << "+";
			}
			cout << " Zr[" << p << "] + PenalidadeDesrespeitoJanelaDeTempoPlanta[" << p << "] * RoAPp[" << p << "] + PenalidadeDesrespeitoJanelaDeTempoPlanta[" << p << "] * RoPPp[" << p << "]";
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

void ClasseModeloJanelaViolada::Restricao_LowerBoundZr( TipoZr Zr,TipoTvei Tvei, TipoAlfa Alfa, TipoRoAPp RoAPp, IloModel& model, int EscreveRestricao){
	int vAux;
	float BigMauternativo;
	IloRangeArray Restricao5;
	char varName[40];

	int NumAux;
	NumAux = 0;

	for (int e = 0; e < NE; e++) {
		for (int i = 0; i < TCDE[e]; i++) {
			vAux = 0;
			for (int p = 0; p < NP; p++) {
				for (int v = 0; v < TCVP[p]; v++) {
					NumAux++;
				}
			}
		}
	}

	Restricao5 = IloRangeArray(env, NumAux);
	NumAux = 0;


	for (int e = 0; e < NE; e++) {
		for (int i = 0; i < TCDE[e]; i++) {
			vAux = 0;
			for (int p = 0; p < NP; p++) {
				for (int v = 0; v < TCVP[p]; v++) {
					if ( EscreveRestricao == 1){
						cout << " Zr[ " << p << "] >=  Tvei[" << vAux << "][" << e << "][" << i << "] +";
						cout << " TEMep[" << vAux << "][" << e << "][" << i << "] + TEMep[" << e << "][" << p << "]";
						cout << " - BigM * ( 1 - Alfa[" << vAux << "][" << e << "][" << i << "])" << endl;
					}
					BigMauternativo = TmaxE[e] + TempoPodePostergarEmpresa[e] + DESCvi[vAux][e][i] + TEMep[e][p];	// M4

					// declara expressão
					IloExpr expr1(env);

					expr1 += Tvei[vAux][e][i] + DESCvi[vAux][e][i] + TEMep[e][p] - BigMauternativo * ( 1 - Alfa[vAux][e][i]) - Zr[p];
					Restricao5[NumAux] = expr1 <= 0;

					sprintf(varName,"Rest5_LimInfZr_p%dv%de%di%d",p,vAux, e, i);
					Restricao5[NumAux].setName(varName);

					model.add(Restricao5[NumAux]);

					expr1.end();

					//model.add( Zr[p] >=  Tvei[vAux][e][i] + DESCvi[vAux][e][i] + TEMep[e][p] - BigMauternativo * ( 1 - Alfa[vAux][e][i]) );
					vAux = vAux + 1;
					NumAux++;
				}
			}
		}
	}

	IloRangeArray Restricao6;

	Restricao6 = IloRangeArray(env, NP);

	for (int p = 0; p < NP; p++) {
		if ( EscreveRestricao == 1){
			cout << " Zr[ " << p << "] >=  TminP[" << p <<"] - TempoPodeAdiantarPlanta[ " << p << "] * RoAPp[" << p << "]  " << endl;
		}
		// declara expressão
		IloExpr expr2(env);
		expr2 +=   TminP[p] - ( TempoPodeAdiantarPlanta[p] * RoAPp[p] ) - Zr[p];
		Restricao6[p] = expr2 <= 0;

		sprintf(varName,"Rest6_LimInfZr_p%d",p);
		Restricao6[p].setName(varName);

		model.add(Restricao6[p]);

		expr2.end();

		//model.add( Zr[p] >=  TminP[p] - TempoPodeAdiantarPlanta[p] * RoAPp[p]);
	}

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
// restrição 16 e 17
void ClasseModeloJanelaViolada::Restricao_LimiteDeTempoNaPlanta( TipoTvei TPvei, TipoAPp APp, TipoPPp PPp, IloModel& model, int EscreveRestricao ){
	int vAux;
	IloRangeArray Restricao16;
	IloRangeArray Restricao17;
	char varName[40];

	int NumAux;
	NumAux = 0;
	for (int p = 0; p < NP; p++) {
		for (int v = 0; v < TCVP[p]; v++) {
			for (int e1 = 0; e1 < NE; e1++) {
				for (int i = 0; i < TCDE[e1]; i++) {
					NumAux++;
				}
			}
		}
	}

	Restricao16 = IloRangeArray(env, NumAux);
	Restricao17 = IloRangeArray(env, NumAux);

	NumAux = 0;

	vAux = 0;
	for (int p = 0; p < NP; p++) {
		for (int v = 0; v < TCVP[p]; v++) {
			for (int e1 = 0; e1 < NE; e1++) {
				for (int i = 0; i < TCDE[e1]; i++) {
					// declara expressão
					IloExpr expr1(env);
					IloExpr expr2(env);

					if ( EscreveRestricao == 1){
						cout << "  TPvei[" << vAux << "][" << e1 << "][" << i << "] >= TminP[" << p <<"] - APp[" << p << "]" << endl;
					}
					expr1 += TPvei[vAux][e1][i] - TminP[p] + APp[p];
					Restricao16[NumAux] = expr1 >= 0;

					sprintf(varName,"Rest16_TPviMin_p%dv%de%di%d",p, vAux, e1, i);
					Restricao16[NumAux].setName(varName);

					model.add(Restricao16[NumAux]);

					//model.add(  TPvei[vAux][e1][i] >= TminP[p] - APp[p]);
					if ( EscreveRestricao == 1){
						cout << "  TPvei[" << vAux << "][" << e1 << "][" << i << "] <= TmaxP[" << p <<"] + PPp[" << p << "]" << endl;
					}
					expr2 += TPvei[vAux][e1][i] - TmaxP[p] - PPp[p];
					Restricao17[NumAux] = expr2 <= 0;

					sprintf(varName,"Rest17_TPviMax_p%dv%de%di%d",p, vAux, e1, i);
					Restricao17[NumAux].setName(varName);

					model.add(Restricao17[NumAux]);
					//model.add(  TPvei[vAux][e1][i] <= TmaxP[p] + PPp[p]);

					expr1.end();
					expr2.end();
					NumAux++;
				}
			}
			vAux = vAux + 1;
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
// Restrição  20 e 21
void ClasseModeloJanelaViolada::Restricao_AtribuiValorRoAPpPPp( TipoAPp APp, TipoRoAPp RoAPp, TipoPPp PPp, TipoRoPPp RoPPp, IloModel& model, int EscreveRestricao ){
	IloRangeArray Restricao20;
	IloRangeArray Restricao21;
	char varName[40];

	Restricao20 = IloRangeArray(env, NP);
	Restricao21 = IloRangeArray(env, NP);

	for (int p = 0; p < NP; p++) {
		// declara expressão
		IloExpr expr1(env);
		IloExpr expr2(env);

		if ( EscreveRestricao == 1){
			cout << " APp[" << p << "]  - TempoPodeAdiantarPlanta[ " << p << "] * RoAPp[" << p << "] <= 0 " << endl;
		}
		expr1 += APp[p]  - TempoPodeAdiantarPlanta[p] * RoAPp[p];
		Restricao20[p] = expr1 <= 0;

		sprintf(varName,"Rest20_AtivaRoAP_p%d",p);
		Restricao20[p].setName(varName);

		model.add(Restricao20[p]);

		//model.add( APp[p]  - TempoPodeAdiantarPlanta[p] * RoAPp[p] <= 0 );
		if ( EscreveRestricao == 1){
			cout << " PPp[" << p << "]  - TempoPodePostergarPlanta[ " << p << "] * RoPPp[" << p << "] <= 0 " << endl;
		}

		expr2 += PPp[p]  - TempoPodePostergarPlanta[p] * RoPPp[p];
		Restricao21[p] = expr2 <= 0;

		sprintf(varName,"Rest21_AtivaRoPP_p%d",p);
		Restricao21[p].setName(varName);

		model.add(Restricao21[p]);

		//model.add( PPp[p]  - TempoPodePostergarPlanta[p] * RoPPp[p] <= 0 );
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
// Dominio Variaveis APp e PPp  ( restrição 24 e 25 )
void ClasseModeloJanelaViolada::DominioAPpPPp( TipoAPp APp, TipoPPp PPp, IloModel& model, int EscreveRestricao ){
	IloRangeArray Restricao24;
	IloRangeArray Restricao25;
	char varName[40];

	Restricao24 = IloRangeArray(env, NP);
	Restricao25 = IloRangeArray(env, NP);
	for (int p = 0; p < NP; p++) {
		// declara expressão
		IloExpr expr1(env);
		IloExpr expr2(env);
		if ( EscreveRestricao == 1){
			cout << " APp[" << p << "] >= 0 " << endl;
		}
		expr1 += APp[p];
		Restricao24[p] = expr1 >= 0;

		sprintf(varName,"Rest24_DominioAP_p%d",p);
		Restricao24[p].setName(varName);

		model.add(Restricao24[p]);
		//model.add( APp[p] >= 0 );
		if ( EscreveRestricao == 1){
			cout << " PPp[" << p << "] >= 0 " << endl;
		}
		expr2 += PPp[p];
		Restricao25[p] = expr2 >= 0;

		sprintf(varName,"Rest25_DominioPP_p%d",p);
		Restricao25[p].setName(varName);

		model.add(Restricao25[p]);
		//model.add( PPp[p] >= 0 );
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
void ClasseModeloJanelaViolada::EscreveVariaveisAPpRoAPpPPpRoPPpDoModeloAposResolucao(int EscreveArquivoComRespostas, int EscreveNaTelaResultados,ofstream& logfile2, IloCplex cplex,  TipoAPp APp, TipoRoAPp RoAPp, TipoPPp PPp, TipoRoPPp RoPPp ){
	if( EscreveNaTelaResultados == 1){
		cout << " Plantas " << endl;
	}
	if( EscreveArquivoComRespostas == 1){
		logfile2 << " Plantas "<< endl;
	}
	for (int p = 0; p< NP; p++) {
		if( EscreveNaTelaResultados == 1){
			cout << APp[p].getName();
			cout << " [" << cplex.getValue(APp[p]) << "]  ";
			cout << RoAPp[p].getName();
			cout << " [" << cplex.getValue(RoAPp[p]) << "]  ";
			cout << endl;
		}
		if( EscreveArquivoComRespostas == 1){
			logfile2  << APp[p].getName();
			logfile2  << " [" << cplex.getValue(APp[p]) << "]  ";
			logfile2  << RoAPp[p].getName();
			logfile2  << " [" << cplex.getValue(RoAPp[p]) << "]  ";
			logfile2  << endl;
		}

	}
	for (int p = 0; p< NP; p++) {
		if( EscreveNaTelaResultados == 1){
			cout << PPp[p].getName();
			cout << " [" << cplex.getValue(PPp[p]) << "]  ";
			cout << RoPPp[p].getName();
			cout << " [" << cplex.getValue(RoPPp[p]) << "]  ";
			cout << endl;
		}
		if( EscreveArquivoComRespostas == 1){
			logfile2  << PPp[p].getName();
			logfile2  << " [" << cplex.getValue(PPp[p]) << "]  ";
			logfile2  << RoPPp[p].getName();
			logfile2  << " [" << cplex.getValue(RoPPp[p]) << "]  ";
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

void ClasseModeloJanelaViolada::CalculaPlantasComAtrazo(  IloCplex cplex, TipoAPp APp, TipoRoAPp RoAPp, TipoPPp PPp, TipoRoPPp RoPPp, int& PlantasComAtrazo, double& ValorAtrazoPlantas){
	ValorAtrazoPlantas = 0;
	PlantasComAtrazo = 0;
	for( int p = 0; p < NP; p++){
		if( cplex.getValue(RoAPp[p]) == 1){
			ValorAtrazoPlantas = ValorAtrazoPlantas + cplex.getValue(APp[p]);
		}
		if( cplex.getValue(RoPPp[p]) == 1){
			ValorAtrazoPlantas = ValorAtrazoPlantas + cplex.getValue(PPp[p]);
		}
		if( cplex.getValue(RoAPp[p]) == 1 || cplex.getValue(RoPPp[p]) == 1 ){
			PlantasComAtrazo++;
		}
	}
}

// Resolve modelo
int ClasseModeloJanelaViolada::Cplex(string Nome, int  TempoExecucao, int &status, double &primal, double &dual, double& SolucaoReal, int& ConstrucoesComAtrazo, int& DemandasAfetadas, double& ValorAtrazoConstrucoes, int& PlantasComAtrazo, double& ValorAtrazoPlantas , double &gap, double &tempo,  vector < string > NomeInstanciaLimiteUpper, vector < double > ValorLimiteUpper){

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

// Variavel Zr
	TipoZr 		Zr(env,NP);
	CriaZr(Zr ,Escreve);

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

	TipoAPp		APp(env,NP);		// Tempo de adiantamento do tempo limite da planta p
	CriaAPp(APp, Escreve);

	TipoPPp		PPp(env,NP);		// Tempo de adiantamento do tempo limite da planta p
	CriaPPp(PPp, Escreve);

	TipoRoAEe RoAEe(env,NE);		// Variavel se teve ou não adiantamento do tempo limite da contrução e
	CriaRoAEe(RoAEe,  Escreve);

	TipoRoPEe RoPEe(env,NE);		// Variavel se teve ou não postergamento do tempo limite da contrução e
	CriaRoPEe(RoPEe,  Escreve);

	TipoRoAPp	RoAPp(env,NP);		// Variavel se teve ou não adiantamento do tempo limite da planta p
	CriaRoAPp(RoAPp,  Escreve);

	TipoRoPPp	RoPPp(env,NP);		// Variavel se teve ou não adiantamento do tempo limite da planta p
	CriaRoPPp(RoPPp,  Escreve);



// Calcula variaveis de adiantamento e postergamento dos limites de tempo
	CalculaTempoPodeAdiantarOuPostergarEmpresa(Escreve);
	CalculaTempoPodeAdiantarOuPostergarPlantas(Escreve);

// Calcula Penalidades Desrespeito as Janelas De Tempo das Construções e das Plantas
	CalculaPenalidadeDesrespeitoJanelaDeTempoEmpresa(TamanhoPenalidade[1], Escreve);
	CalculaPenalidadeDesrespeitoJanelaDeTempoPlanta(TamanhoPenalidade[2],Escreve);

// Funcao Objetivo
	FuncaoObjetivo(Ze, Zr, RoAEe, RoPEe, RoAPp, RoPPp,model, EscreveRestricao[0]);

// Restrição 1 : Antendimento das Demandas
	Restricao_AtendimentoDasDemandas(Alfa, model, EscreveRestricao[1]);
// Restrição  2 : Lower bound Ze
	Restricao_LowerBoundZe(Ze, Tvei, Alfa, model, EscreveRestricao[2]);
// Restrição  3 : Vinculo Tvei e TPvei
	Restricao_VinculoTveiTPvei( Alfa, TPvei, Tvei,model, EscreveRestricao[3] );
// Restrição  4 : de lower bound Zr
	Restricao_LowerBoundZr( Zr, Tvei, Alfa, RoAPp, model, EscreveRestricao[4]);
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
	Restricao_LimiteDeTempoNaPlanta(  TPvei, APp, PPp, model, EscreveRestricao[15] );
// Restrição  13
	Restricao_AtribuiValorRoAEePEe( AEe, RoAEe, PEe, RoPEe, model, EscreveRestricao[16]);
// Restrição  14
	Restricao_AtribuiValorRoAPpPPp( APp, RoAPp, PPp, RoPPp, model, EscreveRestricao[17]);
// Dominio AEe e PEe
	DominioAEePEe( AEe, PEe, model, EscreveRestricao[18]);
// Dominio APp e PPp
	DominioAPpPPp( APp, PPp, model, EscreveRestricao[19]);



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
	PlantasComAtrazo = -1;
	ValorAtrazoPlantas = -1;
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
		Zr.clear();
		Tvei.clear();
		TPvei.clear();
		TPvei.clear();
		EscreveRestricao.clear();

		AEe.clear();
		PEe.clear();
		APp.clear();
		PPp.clear();

		RoAEe.clear();
		RoPEe.clear();
		RoAPp.clear();
		RoPPp.clear();

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
			CalculaFuncaoObjetivo(		*cplex, Ze,  Zr, 	SolucaoReal);
			CalculaEntregasComAtrazo(	*cplex, AEe, RoAEe, PEe, RoPEe, ConstrucoesComAtrazo, 	DemandasAfetadas, ValorAtrazoConstrucoes);
			CalculaPlantasComAtrazo( 	*cplex, APp, RoAPp, PPp, RoPPp, PlantasComAtrazo, 		ValorAtrazoPlantas);
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
			EscreveVariaveisZrDoModeloAposResolucao(				EscreveArquivoComRespostas, EscreveNaTelaResultados, logfile2, *cplex, Zr);
			EscreveVariaveisAEeRoAEePEeRoPEeDoModeloAposResolucao( 	EscreveArquivoComRespostas, EscreveNaTelaResultados, logfile2, *cplex, AEe, RoAEe, PEe, RoPEe );
			EscreveVariaveisAPpRoAPpPPpRoPPpDoModeloAposResolucao( 	EscreveArquivoComRespostas, EscreveNaTelaResultados, logfile2, *cplex, APp, RoAPp, PPp, RoPPp );

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
		Zr.clear();
		Tvei.clear();
		TPvei.clear();
		TPvei.clear();
		EscreveRestricao.clear();

		AEe.clear();
		PEe.clear();
		APp.clear();
		PPp.clear();

		RoAEe.clear();
		RoPEe.clear();
		RoAPp.clear();
		RoPPp.clear();

		return (1);
	}
}

ClasseModeloJanelaViolada::~ClasseModeloJanelaViolada(){



	TCVP.clear();
	CVP.clear();
	TCDE.clear();
	CDE.clear();
	TEMpe.clear();
	TEMep.clear();
	DESCvi.clear();
	CARRp.clear();
	Teta.clear();
	Svii.clear();
	TminE.clear();
	TmaxE.clear();
	TminP.clear();
	TmaxP.clear();
	Nome1.clear();
	Nome2.clear();
	TempoPodeAdiantarEmpresa.clear();
	TempoPodePostergarEmpresa.clear();
	TempoPodeAdiantarPlanta.clear();
	TempoPodePostergarPlanta.clear();
	PenalidadeDesrespeitoJanelaDeTempoEmpresa.clear();
	PenalidadeDesrespeitoJanelaDeTempoPlanta.clear();

	env.end();


}


#endif /* MODELOJANELAVIOLADA_HPP_ */
