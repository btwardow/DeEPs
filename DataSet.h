/*	
 * DataSet.h
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

#ifndef DATASET_H_
#define DATASET_H_

#include <iostream>
#include <list>
#include <string>
#include <vector>
#include "Attribute.h"
#include "Instance.h"
#include <algorithm>
#include "Log.h"
#include <ctime>
#include <cstdlib>
using namespace std;



class DataSet {

private:

	string name;

	vector<Attribute> attributes;
	Attribute labelAttribute;

	vector<Instance> instances;
	vector<double> minValues;
	vector<double> maxValues;

	int labelIdx;

	void readDataFromArffFile(string arffFileName, int indexOfLabel);
	Instance createInstanceFromCSVLine(string& csvline);


public:
	DataSet();
	DataSet(string arffFileName);
	DataSet(DataSet& trainDS, string testFileName);
	virtual ~DataSet();

	const vector<Instance>& getInstances() const { return instances; };
	const string& getName() const { return name; };
	const vector<Attribute>& getAttributes() const { return attributes; };
	double getMinValue(int attrIdx) { return minValues[attrIdx]; };
	double getMaxValue(int attrIdx) { return maxValues[attrIdx]; };


	void removeRecordsWithMissingValues();
	const char* toString(bool detail);

	Attribute getLabelAttribute() const {
		return labelAttribute;
	};

	void shuffleData() {   srand ( unsigned ( time (NULL) ) ); LOG_DEBUG("Random shufle data instances..."); random_shuffle ( instances.begin(), instances.end() ); };


	DataSet foldGetTrainDS(const int foldsNumber, const int idx );
	DataSet foldGetTestDS(const int foldsNumber, const int idx );

	void removeFirstElement() { instances.erase(instances.begin()); };
	void addInstance(Instance i) { instances.push_back(i); };



	/** The keyword used to denote the start of an arff attribute declaration */
	static const string ARFF_ATTRIBUTE;
	static const string ARFF_RELATION;
	static const string ARFF_DATA;
};

#endif /* DATASET_H_ */
