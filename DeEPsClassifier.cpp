/*	
 * DeEPsClassifier.cpp
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

#include "DeEPsClassifier.h"
#include "Log.h"
#include "ReducedDataSet.h"
#include "Pattern.h"
#include <vector>
using namespace std;

DeEPsClassifier::DeEPsClassifier(DataSet& trainSet, double aSimilarity, double supportEPRatio) : LazyClassifier(trainSet), alphaSimilarity(aSimilarity), supportEPRatio(supportEPRatio) {
	LOG_DEBUG("DeEPs Classifier created for train data set: %s, size: %d", trainSet.getName().c_str(), (int)trainSet.getInstances().size());
}

DeEPsClassifier::~DeEPsClassifier() {

}

vector<double> DeEPsClassifier::calculateCompactScore( ReducedDataSet& rds,  vector<vector<Pattern> >& emergingPatterns) {

	int numberOfClasses = emergingPatterns.size();
	vector<int> supportOfEPPerClass(numberOfClasses);
	vector<int> rdsInstancesPerClass(numberOfClasses);


	for(unsigned int i = 0; i < rds.getInstances().size(); i++ ) {

		double classValue = rds.getLabels()[i];

		//collect class instance
		rdsInstancesPerClass[classValue]++;

		//collect support for EP for this class
		for(unsigned int j = 0; j < emergingPatterns[classValue].size(); j++) {
			if(emergingPatterns[classValue][j].covers(rds.getInstances()[i])) {
				supportOfEPPerClass[classValue]++;
				break;
			}
		}

	}

	//calculate compact score
	vector<double> cs(numberOfClasses);
	for(unsigned int i = 0; i < numberOfClasses; i++) {

		if(rdsInstancesPerClass[i] == 0) cs[i]= 0; //do not divide by zero!
		else cs[i] = (double)supportOfEPPerClass[i]/(double)rdsInstancesPerClass[i];
	}

	return cs;
}

double DeEPsClassifier::classify(Instance& i) {


	LOG_DEBUG("DeEPs Classification of instance: %s ", i.toString());

	//first - reduce our data set to values that are interested
	//and with concise representation
	ReducedDataSet rds(trainSet, i, alphaSimilarity);

	vector<Pattern> candidates = Pattern::generateLengthOnePatternsFromInstance(rds, i);

	//result of candidate generation
	vector<vector<Pattern> > emergingPatterns;
	int numberOfClasses = trainSet.getLabelAttribute().getNominalValues().size();
	for(unsigned int c = 0; c < numberOfClasses; c++) emergingPatterns.push_back(vector<Pattern>());
	LOG_DEBUG("Number of classes: %d", numberOfClasses);

	vector<Pattern>* candidatesPtr = &candidates;
	vector<Pattern> newCandidates;
	for(unsigned int aprioriPrefix = 0; aprioriPrefix < i.getValues().size() - 2 && candidatesPtr->size() > 0; aprioriPrefix++) {

//		char c; cin >> c;	//debug brak ;)
		LOG_DEBUG("Candidates: %s", Pattern::toString(*candidatesPtr));


		//search candidates for Emerging Patterns with given support ratio
		//(or if lucky - Jumping Pattern :)
		for(vector<Pattern>::iterator it = candidatesPtr->begin(); it != candidatesPtr->end(); ) {

//			LOG_DEBUG("PATTERN: %s", it->toString() );

			if(it->notOccurrAtAll()) {
				//LOG_DEBUG("Not occurred at all!");
				it = candidatesPtr->erase(it);
				continue;
			}

			//check for every class
			bool isAtLeastOneEP = false;
			for(unsigned int c = 0; c < numberOfClasses; c++) {

				if(it->isEmergingPattern(c, this->supportEPRatio)) {
					//emerging pattern found!
					emergingPatterns[c].push_back(*it);
					LOG_DEBUG("Emerging Pattern: %s", it->toString());
					isAtLeastOneEP = true;

				}
			}

			if(isAtLeastOneEP) {
				// this is minimal pattern
				// do not generate from it more
				it = candidatesPtr->erase(it);
			} else {
				//leave it for generating supresets patterns...
				++it;
			}
		}

		//generate superset patterns
		LOG_DEBUG("Creating new candidates");
		newCandidates = Pattern::generateAprioriLikePatterns(*candidatesPtr, aprioriPrefix);
		candidatesPtr = &newCandidates;
	}

	vector<double> compactScore = calculateCompactScore(rds, emergingPatterns);

	LOG_DEBUG("Number of EP per class:");
	LOG_DEBUG("-----------------------");
	for(unsigned int i = 0; i < emergingPatterns.size(); i++) {
		LOG_DEBUG("\t%d %s - %d", i, trainSet.getLabelAttribute().getNominalValues()[i].c_str(), emergingPatterns[i].size());
	}

	LOG_DEBUG("Compact score per class:");
	LOG_DEBUG("----------------------------------------");
	double maxValue = -1;
	int idx = -1;
	for(unsigned int i = 0; i < compactScore.size(); i++ ) {
		LOG_DEBUG("\t%d [%s] - %f", i, trainSet.getLabelAttribute().getNominalValues()[i].c_str(), compactScore[i]);
		if(compactScore[i] > maxValue) {
			idx = i;
			maxValue = compactScore[i];
		}
	}

	LOG_DEBUG("Class with max compact score: %d - %s", idx, trainSet.getLabelAttribute().getNominalValues()[idx].c_str());

	return idx;
}


