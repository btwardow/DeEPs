/*	
 * DataSet.cpp
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

#include "DataSet.h"
#include "Log.h"
#include "Attribute.h"
#include <iostream>
#include <fstream>
#include <algorithm>
#include <sstream>
#include <vector>
#include <limits>
#include <ctime>
using namespace std;


const string DataSet::ARFF_ATTRIBUTE = "@ATTRIBUTE";
const string DataSet::ARFF_RELATION = "@RELATION";
const string DataSet::ARFF_DATA = "@DATA";

void trim(string& str);
void tokenize(const string& str, vector<string>& tokens,
		const string& delimiters);

DataSet::DataSet() {
	// TODO Auto-generated constructor stub

}

DataSet::DataSet(string arffFileName) {
	readDataFromArffFile(arffFileName, -1);

}

void DataSet::readDataFromArffFile(string arffFileName, int indexOfLabel = -1) {

	LOG_DEBUG("Reading dataset from ARFF file: %s", arffFileName.c_str());
	clock_t t_start=clock();


	ifstream inputFile (arffFileName.c_str());
	string line;
	labelIdx = indexOfLabel;


	if(inputFile.is_open()) {
		while(inputFile.good()) {
			getline(inputFile, line);
			trim(line);
			if(line.empty()) continue;
			string token;
			stringstream os(line);
			os >> token;
			if(token=="%") continue; //comment in file


			std::transform(token.begin(), token.end(), token.begin(), ::toupper);

			if(token==ARFF_RELATION) {
				string relationName;
				getline(os, relationName);
				this->name = relationName;
				LOG_DEBUG("Relation name: %s", this->name.c_str());
			} else if (token == ARFF_ATTRIBUTE) {
				LOG_DEBUG("Attribute");
				Attribute a = Attribute::createAttributeFromARFFAnnotation(line);
				if(attributes.size() == indexOfLabel ) {
					labelAttribute = a;
				} else if (indexOfLabel == -1) {
					if(labelIdx!= -1) {
						attributes.push_back(labelAttribute);
					}
					labelAttribute = a;
					labelIdx++;
				} else {
					attributes.push_back(a);
				}

			} else if(token == ARFF_DATA) {
				LOG_DEBUG("Start loading data...");

				//initialize min/max values vector
				for(int i = 0; i < attributes.size(); i++ ) {
					minValues.push_back(numeric_limits<double>::max());
					maxValues.push_back(numeric_limits<double>::min());
				}

			} else {

				instances.push_back(createInstanceFromCSVLine(line));
			}

		}
	} else {
		LOG_ERROR("Unable to open a file: %s", arffFileName.c_str());
	}



	inputFile.close();

	LOG_INFO("%d data records was successfully loaded from file: %s", (int)instances.size(), arffFileName.c_str());

	clock_t t_end=clock();

	LOG_DEBUG("Data set created. Number of instances: %d", (int)instances.size() );

	double timeTaken = ((t_end-t_start)*10)/CLOCKS_PER_SEC;
	LOG_DEBUG("Time taken: %f (s)", timeTaken);
}




DataSet::~DataSet() {
	// TODO Auto-generated destructor stub
}

void DataSet::removeRecordsWithMissingValues() {

	LOG_DEBUG("Removing instances with missing values...");

	int count = 0;
	for(std::vector<Instance>::iterator it = instances.begin(); it != instances.end(); ) {

		if(it->hasMissingValues()) {
			//LOG_DEBUG("Removing instance: %s", it->toString());
			it = instances.erase(it);
			count++;
		} else {
			++it;
		}

	}

	LOG_INFO("Data set: %s, %d instances with missing values removed. Size of data set: %d", name.c_str(), count, instances.size());

}

Instance DataSet::createInstanceFromCSVLine(string& csvline) {
	//data record
	vector<string> dataRecordStr;
	//LOG_DEBUG("Data record: %s", line.c_str());
	tokenize(csvline, dataRecordStr, ",");

	if (dataRecordStr.size() != attributes.size() + 1) {
		LOG_ERROR(
				"Data record has %d. Should have values for: %d attributes!", (int)dataRecordStr.size(), (int)attributes.size());
	}

	vector<double> values;
	double label;
	for (unsigned int i = 0; i < dataRecordStr.size(); i++) {
		trim(dataRecordStr[i]);
		double value;
		Attribute attr;

		//					LOG_DEBUG("Choosing attrib.");

		if (i == (unsigned int) labelIdx) {
			attr = labelAttribute;
		} else if (i < (unsigned int) labelIdx) {
			attr = attributes[i];
		} else {
			attr = attributes[i + 1];
		}

		if (dataRecordStr[i] == "?") {
			value = Instance::MISSING_VALUE;
		} else if ((attr).isNumeric()) {
			const char* str = dataRecordStr[i].c_str();
			char* endptr;
			value = strtod(str, &endptr);
			if (*endptr)
				value = 0;

		} else if (attr.isNominal()) {
			value = attr.getValueForNominal(dataRecordStr[i]);
		} else {
			LOG_ERROR("Unsupported Attribute");
		}

		if (i == (unsigned int) labelIdx) {
			label = value;
		} else {
			values.push_back(value);
			if (minValues[i] > value)
				minValues[i] = value;
			if (maxValues[i] < value)
				maxValues[i] = value;
		}

	}

	return Instance(attributes, values, labelAttribute, label);
}

DataSet::DataSet(DataSet& trainDS, string testFileName) {

	labelIdx = trainDS.labelIdx;
	attributes = trainDS.attributes;
	labelAttribute = trainDS.labelAttribute;
	name = trainDS.name;
	maxValues = trainDS.maxValues;
	minValues = trainDS.minValues;


	LOG_DEBUG("Reading test instances from file: %s", testFileName.c_str());
	clock_t t_start=clock();
	ifstream inputFile (testFileName.c_str());
	string line;


		if(inputFile.is_open()) {
			while(inputFile.good()) {
				getline(inputFile, line);
				trim(line);
				if(line.empty()) continue;
				string token;
				stringstream os(line);
				os >> token;
				if(token=="%") continue; //comment in file

				instances.push_back(createInstanceFromCSVLine(line));

			}
		} else {
			LOG_ERROR("Unable to open a file: %s", testFileName.c_str());
		}



		inputFile.close();

		LOG_INFO("%d test data records was successfully loaded from test file: %s", (int)instances.size(), testFileName.c_str());

		clock_t t_end=clock();

		LOG_DEBUG("Test data set created. Number of instances: %d", (int)instances.size() );

		double timeTaken = ((t_end-t_start)*10)/CLOCKS_PER_SEC;
		LOG_DEBUG("Time taken: %f (s)", timeTaken);


}




const char* DataSet::toString(bool details) {

	ostringstream oss (ostringstream::out);
	oss << " DataSet  " << endl;
	oss << "----------" << endl;
	oss << "Name: " << name << endl;
	oss << "Size: " << instances.size() << endl;

	if(details) {

		for (unsigned int i = 0; i < instances.size(); i++) {
			oss << "[" << i << "] - " << instances[i].toString() << endl;
		}
	}
	return oss.str().c_str();
}

DataSet DataSet::foldGetTrainDS(const int foldsNumber, const int idx) {

	DataSet trainDS;
	trainDS.labelIdx = labelIdx;
	trainDS.attributes = attributes;
	trainDS.labelAttribute = labelAttribute;
	trainDS.name = name;
	trainDS.maxValues = maxValues;
	trainDS.minValues = minValues;

	for(unsigned int i = 0; i < instances.size(); i++ ) {
		if( (i % foldsNumber) != idx) {
			trainDS.instances.push_back(instances[i]);
		}
	}

	return trainDS;
}

DataSet DataSet::foldGetTestDS(const int foldsNumber, const int idx) {

	DataSet testDS;
	testDS.labelIdx = labelIdx;
	testDS.attributes = attributes;
	testDS.labelAttribute = labelAttribute;
	testDS.name = name;
	testDS.maxValues = maxValues;
	testDS.minValues = minValues;

	for(unsigned int i = 0; i < instances.size(); i++ ) {
		if( (i % foldsNumber) == idx) {
			testDS.instances.push_back(instances[i]);
		}
	}

	return testDS;
}







