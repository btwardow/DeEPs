/*	
 * ReducedDataSet.h
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
 *  Created on: May 27, 2012
 *      Author: Bartlomiej Twardowski (B.Twardowski@ii.pw.edu.pl)
 *
 *  (c) 2012 Politechnika Warszawska
 */

#ifndef REDUCEDDATASET_H_
#define REDUCEDDATASET_H_

#include "DataSet.h"
#include <vector>
using namespace std;


class ReducedDataSet {


private:
	DataSet& orginalDataSet;

	vector<vector<int> > instances;
	vector<double> labels;

	Instance& reducingInstance;
	double alphaSimilarityPercent;


public:
	ReducedDataSet(DataSet& ds, Instance& reducingInstance, double alphaSimilarityPercent);
	virtual ~ReducedDataSet();

	const vector<vector<int> >& getInstances() const {
		return instances;
	};

	const vector<double>& getLabels() const {
		return labels;
	};

	const DataSet& getOrginalDataSet() const {
		return orginalDataSet;
	};

	ReducedDataSet& operator =(const ReducedDataSet& other) {
		orginalDataSet = other.orginalDataSet;
		instances = other.instances;
		labels = other.labels;
		reducingInstance = other.reducingInstance;
		alphaSimilarityPercent = other.alphaSimilarityPercent;
		return *this;
	};
};

#endif /* REDUCEDDATASET_H_ */
