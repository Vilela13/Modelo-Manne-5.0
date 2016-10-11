/*
 * Biblioteca.hpp
 *
 *  Created on: 16/04/2015
 *      Author: mateus
 */

#ifndef BIBLIOTECA_HPP_
#define BIBLIOTECA_HPP_

#include <ilcplex/ilocplex.h>



#include <sstream>

#include <iostream>     // Biblioteca padrão de c++
#include <istream>      // Biblioteca padrão de c++
#include <list>         // Biblioteca que contem a estrutura listas
#include <string>       // Biblioteca que contem a estrutura string ( vetor de caracteres)
#include <fstream>      // Biblioteca que contem a estrutura para acessar arquivos txt
#include <vector>       // Biblioteca que contem a estrutura Vector

#include <stdio.h>      // Biblioteca padão de C, principalmente a operações de entrada e saída
#include <stdlib.h>     // Biblioteca padão de C, possui funções envolvendo alocação de memória, controle de processos, conversões e outras.
#include <string>
#include <string.h>

#include <time.h>

#include <math.h>

#include <climits>


#include <dirent.h> 	// Manipulação de arquivos em C (meu caso, ver se arquivo existe)

//#include <random>		// Biblioteca para gerar numeros segundo probabilidades

/* Outra maneira de criar arquivos
#include <sys/types.h>
#include <sys/stat.h>		// Manipulação de arquivos em C (meu caso, criar arquivo )
*/



#define BigM 2147483647    // maior Int

ILOSTLBEGIN

#include "VariaveisFixas.hpp"

using namespace std;


#include "Modelo.hpp"
#include "ModeloJanelaViolada.hpp"

#include "FuncoesDoCPP.hpp"

#endif /* BIBLIOTECA_HPP_ */
