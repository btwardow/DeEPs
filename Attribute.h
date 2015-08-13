/*	
 * Attribute.h
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

#ifndef ATTRIBUTE_H_
#define ATTRIBUTE_H_

#include <string>
#include <vector>
using namespace std;




class Attribute {

private:
	string name;
	int type;
	std::vector<std::string> nominalValues;

public:

	Attribute();
	virtual ~Attribute();

	string toString() const {
		return "Attribute [ " + this->name + "] ";
	}

	string getName() const
    {
        return name;
    }

    int getType() const
    {
        return type;
    }

    void setName(string name)
    {
        this->name = name;
    }

    void setType(int type)
    {
        this->type = type;
    }



    const bool isNumeric() const { return type == TYPE_NUMERIC; };
    const bool isNominal() const { return type == TYPE_NOMINAL; };
    const bool isString() const { return type == TYPE_STRING; };
    const bool isData() const { return type == TYPE_DATE; };

    static Attribute createAttributeFromARFFAnnotation(string annotation);

    double getValueForNominal(string strValue);

	std::vector<std::string> getNominalValues() const {
		return nominalValues;
	};

	/** Constant set for numeric attributes. */
	static const int TYPE_NUMERIC = 0;
	/** Constant set for nominal attributes. */
	static const int TYPE_NOMINAL = 1;
	/** Constant set for attributes with string values. */
	static const int TYPE_STRING = 2;
	/** Constant set for attributes with date values. */
	static const int TYPE_DATE = 3;
	/** A keyword used to denote a numeric attribute */
	static const string ARFF_ATTRIBUTE_INTEGER;
	/** A keyword used to denote a numeric attribute */
	static const string ARFF_ATTRIBUTE_REAL;
	/** A keyword used to denote a numeric attribute */
	static const string ARFF_ATTRIBUTE_NUMERIC;
	/** The keyword used to denote a string attribute */
	static const string ARFF_ATTRIBUTE_STRING;
	/** The keyword used to denote a date attribute */
	static const string ARFF_ATTRIBUTE_DATE;
};

#endif /* ATTRIBUTE_H_ */
