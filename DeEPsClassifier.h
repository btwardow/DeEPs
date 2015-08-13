/*	
 * DeEPsClassifier.h
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
 *  Created on: May 29, 2012
 *      Author: Bartlomiej Twardowski (B.Twardowski@ii.pw.edu.pl)
 *
 *  (c) 2012 Politechnika Warszawska
 */

#ifndef DEEPSCLASSIFIER_H_
#define DEEPSCLASSIFIER_H_

#include "LazyClassifier.h"
#include "ReducedDataSet.h"
#include "Pattern.h"
#include <vector>

class DeEPsClassifier: public LazyClassifier {

private:
	double alphaSimilarity;
	double supportEPRatio;

	vector<double> calculateCompactScore( ReducedDataSet& rds,  vector<vector<Pattern> >& emergingPatterns);

public:
	DeEPsClassifier(DataSet& trainSet, double aSimilarity = 0.05, double supportEPRatio = numeric_limits<double>::infinity());
	virtual ~DeEPsClassifier();
	virtual double classify(Instance& i);
};

#endif /* DEEPSCLASSIFIER_H_ */
