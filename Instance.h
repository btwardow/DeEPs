/*	
 * Instance.h
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

#ifndef INSTANCE_H_
#define INSTANCE_H_

#include "Attribute.h"
#include <vector>
#include <cmath>
#include <limits>
using namespace std;

class Instance {

private:

	vector<Attribute> attributes;
	vector<double> values;
	Attribute labelAttribute;
	double label;


public:

	static const double MISSING_VALUE;

	Instance(vector<Attribute>& a, vector<double>& v, Attribute& la, double& l): attributes(a), values(v), labelAttribute(la), label(l) {};
	virtual ~Instance();

	const vector<Attribute>& getAttributes() const { return attributes; };
	const vector<double>& getValues() const { return values; };
	const Attribute& getLabelAttribute() const { return labelAttribute; };
	const double& getLabelValue() const { return label; };
	void setLabel(double label) { this->label = label; };
	
	const char* toString() const;

	Instance& operator=(const Instance& from) { this->attributes = from.attributes; this->values = from.values; this-> labelAttribute = from.labelAttribute; this->label = from.label; return *this;};
	Instance(Instance const& other) : attributes(other.attributes), values(other.values), labelAttribute(other.labelAttribute), label(other.label) {};

	static bool isMissingValue(double value) { if(value == Instance::MISSING_VALUE) return true; else return false; };
	bool hasMissingValues() { for(unsigned int i = 0; i < values.size(); i++) { if(isMissingValue(values[i])) return true; } return false; };

};

#endif /* INSTANCE_H_ */
