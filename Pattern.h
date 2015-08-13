/*	
 * Pattern.h
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

#ifndef PATTERN_H_
#define PATTERN_H_

#include <vector>
#include "ReducedDataSet.h"
#include "Instance.h"

using namespace std;


class Pattern {

	vector<int> items;
	ReducedDataSet& ds;
	Instance& instance;
	vector<int> supprtForClasses;

public:
//	Pattern() {};
	Pattern(vector<int> p, ReducedDataSet& d, Instance& i);
	virtual ~Pattern();
	Pattern& operator=(const Pattern& other);

	static vector<Pattern> generateLengthOnePatternsFromInstance(ReducedDataSet& ds, Instance& i);
	static vector<Pattern> generateAprioriLikePatterns(vector<Pattern>& patternSet, int prefixLength);
//	static void generateAprioriLikePatterns(vector<Pattern>& patternSet, int prefixLength, vector<Pattern>& result);


	const char* toString();
	static const char* toString(vector<Pattern> patternSet);
	int getSupportForClass(double classValue);

	bool covers(vector<int> i);

	bool notOccurrAtAll();
	bool isEmergingPattern(int classIndex, double emergingPatternRatio);



};

#endif /* PATTERN_H_ */
