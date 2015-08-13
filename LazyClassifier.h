/*	
 * LazyClassifier.h
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

#ifndef LAZYCLASSIFIER_H_
#define LAZYCLASSIFIER_H_

#include "DataSet.h"

class LazyClassifier {

protected:
	DataSet& trainSet;
public:
	LazyClassifier(DataSet& trainSet) : trainSet(trainSet) {};
	virtual ~LazyClassifier() {};
	virtual double classify(Instance& i) = 0;

};

#endif /* LAZYCLASSIFIER_H_ */
