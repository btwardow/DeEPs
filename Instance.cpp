/*	
 * Instance.cpp
 *
 *  EDAMI Project:
 *	DeEPs Algorithm Implementation
 *  
 * Based on:
 * =========
 * Instance-Based Classification by Emerging Patterns (incollection)
 * Li1, Jinyan and Dong, Guozhu and Ramamohanarao, Kotagiri
 * Principles of Data Mining and Knowledge Discovery, 2000, Volume 1910 LNCS, Pages 191-200
 *
 *
 *  Created on: May 26, 2012
 *      Author: Bartlomiej Twardowski (B.Twardowski@ii.pw.edu.pl)
 *
 *  (c) 2012 Politechnika Warszawska
 */

#include "Instance.h"
#include <iostream>
#include <sstream>
using namespace std;

const double Instance::MISSING_VALUE = numeric_limits<double>::infinity();


Instance::~Instance() {
	// TODO Auto-generated destructor stub
}

const char* Instance::toString() const {
	  ostringstream oss (ostringstream::out);
	oss << "[ ";
	for(unsigned int i = 0; i < getValues().size(); i++) {
		if(i!=0) oss << ", ";
		oss << getValues()[i];
	}
	oss << " ] : ";
	oss << getLabelValue();
	return oss.str().c_str();
}

