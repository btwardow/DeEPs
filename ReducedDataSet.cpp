/*	
 * ReducedDataSet.cpp
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

#include "ReducedDataSet.h"
#include "Log.h"
#include <time.h>


ReducedDataSet::ReducedDataSet(DataSet& ds, Instance& instance, double alphaSimilarityPercent = 0.05) : orginalDataSet(ds), reducingInstance(instance), alphaSimilarityPercent(alphaSimilarityPercent) {

	LOG_DEBUG("Creating Reduced Data set.");
	LOG_DEBUG("Alpha similarity for continuous attributes set to: %f", alphaSimilarityPercent);
	LOG_DEBUG("Input data set: %s, number of records: %d", ds.getName().c_str(), (int) ds.getInstances().size());
	LOG_DEBUG("Instance:  %s", instance.toString());

	clock_t t_begin=clock();

	if(reducingInstance.getAttributes().size() != ds.getAttributes().size()) {
		LOG_ERROR("Number of attributes from reducing instance is different than data set!");
	}
	int numOfAttribs = reducingInstance.getAttributes().size();

	for(unsigned int i = 0; i < ds.getInstances().size(); i++ ) {

		vector<int> row;
		bool add = false;
//		LOG_DEBUG("ROW: %s", ds.getInstances()[i].toString() );

		for( int j = 0; j < numOfAttribs; j++) {

			double rV = ds.getInstances()[i].getValues()[j];
			double iV = instance.getValues()[j];
			double dAlpha = alphaSimilarityPercent * (ds.getMaxValue(j) - ds.getMinValue(j));
			//LOG_DEBUG("instance value: %f ", iV);
			//LOG_DEBUG("ds.instance value : %f ", rV);
			//LOG_DEBUG("Alpha similarity diff: %f", dAlpha);
			//if(ds.getAttributes()[i].isNominal()) LOG_DEBUG("Attribute is nominal");
			//if(ds.getAttributes()[i].isNumeric()) LOG_DEBUG("Attribute is numeric");



			if(ds.getAttributes()[i].isNominal() && iV == rV) {
//				LOG_DEBUG("Equal values of nominal attribute");
				row.push_back(1);
				add = true;
			} else if(ds.getAttributes()[i].isNumeric() && rV >= ( iV - dAlpha) && rV <= (iV + dAlpha)) {
//				LOG_DEBUG("Value is similar.");
				row.push_back(1);
				add = true;

			} else {
//				LOG_DEBUG("Not equal.");
				row.push_back(0);
			}
		}

		if(add) {

			instances.push_back(row);
			labels.push_back(ds.getInstances()[i].getLabelValue());
		}

	}


	clock_t t_end=clock();

	LOG_DEBUG("Reduced data set created. Number of instances: %d", (int)instances.size() );

	double timeTaken = ((t_end-t_begin)*10)/CLOCKS_PER_SEC;
	LOG_DEBUG("Time taken: %f (s)", timeTaken);

}

ReducedDataSet::~ReducedDataSet() {
}





