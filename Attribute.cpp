/*	
 * Attribute.cpp
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
 *  Created on: May 22, 2012
 *      Author: Bartlomiej Twardowski (B.Twardowski@ii.pw.edu.pl)
 *
 *  (c) 2012 Politechnika Warszawska
 */

#include "Attribute.h"
#include "Log.h"
#include "DataSet.h"
#include <string>
#include <algorithm>
#include <vector>
#include "StringUtils.h"
using namespace std;


const string Attribute::ARFF_ATTRIBUTE_INTEGER = "INTEGER";
const string Attribute::ARFF_ATTRIBUTE_REAL = "REAL";
const string Attribute::ARFF_ATTRIBUTE_NUMERIC = "NUMERIC";
const string Attribute::ARFF_ATTRIBUTE_STRING = "STRING";
const string Attribute::ARFF_ATTRIBUTE_DATE = "DATE";

Attribute::Attribute() {
	// TODO Auto-generated constructor stub

}

Attribute::~Attribute() {
	// TODO Auto-generated destructor stub
}

Attribute Attribute::createAttributeFromARFFAnnotation(string annotation)
{
	Attribute newAttr = Attribute();
	LOG_DEBUG("Creating attribute from line: %s", annotation.c_str());

	vector<string> tokens;
	tokenize(annotation, tokens, " \t,");
	trim(tokens[1]);
	newAttr.setName(tokens[1]);
	LOG_DEBUG("Attribute name: %s", tokens[1].c_str());
	trim(tokens[2]);

	if(annotation.find("{") != string::npos) {
		LOG_DEBUG("Nominal attribute.");
		newAttr.setType(Attribute::TYPE_NOMINAL);

		int idxL = annotation.find("{");
		int idxR = annotation.find("}");
		string valuesStr = annotation.substr(idxL +1 , idxR - idxL -1 );
		LOG_DEBUG("Nominal values: %s", valuesStr.c_str());
		tokenize(valuesStr, newAttr.nominalValues, ",");
		for(unsigned int i = 0; i < newAttr.nominalValues.size(); i++ ) trim(newAttr.nominalValues[i]);

	} else {

		std::transform(tokens[2].begin(), tokens[2].end(), tokens[2].begin(), ::toupper);

		if(tokens[2] == ARFF_ATTRIBUTE_INTEGER || tokens[2] == ARFF_ATTRIBUTE_INTEGER || tokens[2] == ARFF_ATTRIBUTE_REAL) {
			LOG_DEBUG("Numeric attribute");
			newAttr.setType(TYPE_NUMERIC);
		} else if (tokens[2] == ARFF_ATTRIBUTE_DATE) {
			LOG_DEBUG("Date attribute");
			newAttr.setType(TYPE_DATE);
			//now not supported!
			LOG_ERROR("Attribute type: %s not supported!", tokens[2].c_str());
		} else if (tokens[2] == ARFF_ATTRIBUTE_STRING) {
			LOG_DEBUG("String attribute");
			newAttr.setType(TYPE_STRING);
			//now not supported!
			LOG_ERROR("Attribute type: %s not supported!", tokens[2].c_str());
		} else {
			LOG_ERROR("Unknown attribute type: %s", tokens[2].c_str());
		}


	}


	return newAttr;
}

double Attribute::getValueForNominal(string strValue) {

	trim(strValue);

	if(!isNominal()) {
		LOG_ERROR("Attribute is not nominal!");
	}

	for(unsigned int i = 0; i < nominalValues.size(); i++) {
		if(nominalValues[i] == strValue) {
			return i;
		}
	}

	LOG_ERROR("Cannot find nominal value: %s for attribute: %s", strValue.c_str(), name.c_str());
}







