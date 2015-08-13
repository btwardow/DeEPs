/*	
 * Pattern.cpp
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

#include "Pattern.h"
#include <iostream>
#include <sstream>
#include "Log.h"
using namespace std;

Pattern::Pattern(vector<int> p, ReducedDataSet& d, Instance& i): items(p), ds(d), instance(i) {

	supprtForClasses = vector<int>(ds.getOrginalDataSet().getLabelAttribute().getNominalValues().size());

	for(unsigned int i = 0; i < ds.getInstances().size(); i++ ) {

		bool cover = true;
		for(unsigned int j = 0; j < items.size(); j++ ) {
			if(items[j] == 1 && ds.getInstances()[i][j] != 1) {
				cover = false;
				break;
			}
		}
		if(cover) {
//			LOG_DEBUG("Instance: %s", ds.getInstances()[j][i] );
//			LOG_DEBUG("Support for class %d", (int)ds.getLabels()[i]);
			supprtForClasses[ds.getLabels()[i]]++;
		}
	}

}

Pattern::~Pattern() {
	// TODO Auto-generated destructor stub
}

 vector<Pattern> Pattern::generateLengthOnePatternsFromInstance(
		ReducedDataSet& ds, Instance& i) {

	vector<Pattern> result;
	for(unsigned int j = 0; j < i.getAttributes().size(); j++) {
		vector<int> p;
		for(unsigned int k = 0; k < i.getAttributes().size(); k++) {
			if(k==j) p.push_back(1);
			else p.push_back(0);
		}
		result.push_back(Pattern(p, ds, i));
	}

	return result;
}

Pattern& Pattern::operator=(const Pattern& other) {
	ds = other.ds;
	instance = other.instance;
	items = other.items;
	supprtForClasses = other.supprtForClasses;
	return *this;
}

const char* Pattern::toString() {
	  ostringstream oss (ostringstream::out);
	oss << " Pattern: [ ";
	for(unsigned int i = 0; i < items.size(); i++) {
		if(i!=0) oss << ", ";
		if(items[i] == 0) oss << " ";
		else oss << instance.getValues()[i];
	}
	oss << " ]  ";
	for(unsigned int i = 0; i < supprtForClasses.size(); i++ ) {
		if(i!=0) oss << " / ";
		oss << supprtForClasses[i];
	}

	return oss.str().c_str();
}

const char* Pattern::toString(vector<Pattern> patternSet) {

	ostringstream oss (ostringstream::out);
	oss << " -- Start Pattern Set -- " << endl;

	for(unsigned int i = 0; i < patternSet.size(); i++) {
		oss << "[" << i << "] " << patternSet[i].toString() << endl;
	}
	oss << " -- End Pattern Set -- " << endl;
	oss << " Number of patterns: " << patternSet.size() << endl;

	return oss.str().c_str();

}

vector<Pattern> Pattern::generateAprioriLikePatterns(vector<Pattern>& patternSet, int prefixLength) {

	clock_t t_start=clock();

	vector<Pattern> result;

	LOG_DEBUG("Generating patterns in Apriori Like way...");
	LOG_DEBUG("Prefix : %d", prefixLength);
	LOG_DEBUG("Base set size: %d", patternSet.size());

	if(patternSet.size() < 2) return result;

	for(unsigned int i = 0; i < patternSet.size() -1 ; i++) {
		for(unsigned int j=i+1; j < patternSet.size(); j++) {

			Pattern& p1 = patternSet[i];
			Pattern& p2 = patternSet[j];

//			LOG_DEBUG("P1: %s", p1.toString());
//			LOG_DEBUG("P2: %s", p2.toString());

			if(p1.items.size() != p2.items.size()) {
				LOG_ERROR("Pattern p1: \n%s\n has different size from pattern p2:\n%s\n Cannot generate new pattern!", p1.toString(), p2.toString());
			}

			//collect common prefix
			int p = 0;
			int r = 0;
			for(unsigned int k=0; k < p1.items.size(); k++) {

				if(p1.items[k] == p2.items[k] && r == 0) {
					if(p1.items[k] == 1) p++;
				}

				if(p1.items[k]!=p2.items[k]) {
					if(p!=prefixLength) break;
					r++;

				}
			}

//			LOG_DEBUG("p: %d", p);
//			LOG_DEBUG("r: %d", r);


			//check if patterns have common prefix
			//records differs only on last position (Apriori like style ;)
			if(p == prefixLength && r == 2) {

				vector<int> newPattern;
				//combine two patterns
				int itemsCount = 0;
				for(unsigned int z = 0; z < p1.items.size(); z++) {
					if(p1.items[z] == 1 || p2.items[z] == 1) {
						newPattern.push_back(1);
						itemsCount++;
					} else {
						newPattern.push_back(0);
					}
				}

				if(itemsCount != prefixLength + 2) {
					LOG_ERROR("Suffix of generated patterns differs on more than 2 positions! \nPattern1: %s\nPattern2: %s", p1.toString(), p2.toString());
				}

				if( p1.items.size() != newPattern.size()) {
					LOG_ERROR("New pattern size different from original ones!");
				}

				Pattern newPat = Pattern(newPattern, p1.ds, p1.instance);
//				LOG_DEBUG("NEW PATTERN: %s", newPat.toString());
				result.push_back(newPat);

			}

		}
	}

	LOG_DEBUG("%d patterns generated for prefix %d", result.size(), prefixLength );
	clock_t t_end=clock();
	double timeTaken =((t_end-t_start)*10)/CLOCKS_PER_SEC;
	LOG_DEBUG("Time taken: %f (s)", timeTaken);

	return result;
}

int Pattern::getSupportForClass(double classValue) {
	return supprtForClasses[classValue];
}

bool Pattern::notOccurrAtAll() {

	for(unsigned int i = 0; i < supprtForClasses.size(); i++) {
		if(supprtForClasses[i] > 0) {
			return false;
		}
	}

	return true;
}

bool Pattern::covers(vector<int> i) {

	for(unsigned int j = 0; j < items.size(); j++ ) {
		if(items[j] == 1 && i[j] != 1) {
			return false;
		}
	}

//	LOG_DEBUG("%s covers item:", toString());
//	for(unsigned int k =0; k < i.size(); k++) {
//		cout << i[k];
//	}
//	cout << endl;

	return true;

}

bool Pattern::isEmergingPattern(int classIndex, double emergingPatternRatio) {

	double positiveSupport = 0;
	double negativeSupport = 0;
	for(unsigned int i = 0; i < supprtForClasses.size(); i++) {
		if(i == classIndex) {
			positiveSupport += supprtForClasses[i];
		} else {
			negativeSupport += supprtForClasses[i];
		}
	}

	if(positiveSupport == 0) return false;

	if(positiveSupport > 0 && negativeSupport == 0) {
		//JEP
		return true;
	}

	double ratio = positiveSupport / negativeSupport;

	if(ratio >= emergingPatternRatio) return true;

	return false;

}







