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


#include <stdio.h>
#include <stdlib.h>
#include "DataSet.h"
#include "DeEPsClassifier.h"
#include "ReducedDataSet.h"
#include "Pattern.h"
#include "Log.h"

#include <getopt.h>
#include <string.h>


void print_info() {
	printf("\nEDAMI Project:");
	printf("\nDeEPs Algorithm Implementation");
	printf("\n ");
	printf("\nBased on:");
	printf("\n=========");
	printf("\nInstance-Based Classification by Emerging Patterns (incollection)");
	printf("\nLi1, Jinyan and Dong, Guozhu and Ramamohanarao, Kotagiri");
	printf(
			"\nPrinciples of Data Mining and Knowledge Discovery, 2000, Volume 1910 LNCS, Pages 191-200");
	printf("\n ");
	printf("\n ");
	printf("\nCreated on: May 22, 2012");
	printf("\nAuthor: Bartlomiej Twardowski (B.Twardowski@ii.pw.edu.pl)");
	printf("\n ");
	printf("\n  (c) 2012 Politechnika Warszawska\n\n");

}


void print_usage() {

 printf(" [-h] [-v] [-f FILE] [-n | -t FILE] [-a FLOAT] [-r NUMBER]\n\n");

 printf("  -h              print this help and exit\n");
 printf("  -v              set verbose flag\n");
 printf("  -n NUMBER       set number of instance to perform test on input data set \n");
 printf("  			       (cannot be used with -t). \n");
 printf("  -i FILE         set train data set file name (ARFF format)\n");
 printf("  -t FILE         set test data set file (CSV)\n\n");
 printf("  			       (cannot be used with -n). \n");
 printf("  -t FILE         set test data set file (CSV)\n");
 printf("  -a FLOAT        set alpha similarity for continuous attribute (default: 0.05). \n");
 printf("  -r NUMBER       set support ratio for emerging patterns (default: inf.). \n");

}

extern int LOG_LEVEL;

int main(int argc, char **argv) {

	print_info();

	opterr = 0;
	char *inFileName = NULL;
	char *testFileName = NULL;
	int numberOfInstance = -1 ;
	double alphaSimilarity = 0.05;
	double supportRatio = numeric_limits<double>::infinity();
	int c;

	while ((c = getopt(argc, argv, "vi:t:n:ha:r:")) != -1) {

		switch (c) {
		case 'a':
			alphaSimilarity = atof(optarg);
			if(alphaSimilarity < 0 or alphaSimilarity > 1) {
				printf("Alpha similarity for continuous attribute: %s\n", optarg);
				LOG_ERROR("Alpha similarity for continuous attribute should be value from [0,1]");
			}
			break;
		case 'r':
			supportRatio = atoi(optarg);
			if(supportRatio < 1) {
				printf("Emerging pattern support ratio: %s\n", optarg);
				LOG_ERROR("Emerging pattern support ratio should be > 1!");
			}
			break;
		case 'v':
			LOG_LEVEL = LOG_LEVEL_DEBUG;
			break;
		case 'i':
			inFileName = optarg;
			break;
		case 't':
			testFileName = optarg;
			break;
		case 'n':
			numberOfInstance = atoi(optarg);
			if(numberOfInstance < 1) {
				printf("Number of instance given: %s\n", optarg);
				LOG_ERROR("Number of instances cannot be less than 1!");
			}
			break;
		case 'h':
			print_usage();
			return 1;
			exit(EXIT_SUCCESS);
		case '?':
			if (optopt == 'c' || optopt == 't' || optopt == 'f')
				printf("Option -%c requires an argument.\n", optopt);
			else if (isprint(optopt))
				printf("Unknown option `-%c'.\n", optopt);
			else
				printf("Unknown option character `\\x%x'.\n", optopt);
			return 1;
		default:
			printf("Error!");
		}
	}

	DataSet inputDataSet(inFileName);
	inputDataSet.removeRecordsWithMissingValues();
	inputDataSet.shuffleData();




	if(testFileName != NULL && numberOfInstance < 1) {

		LOG_INFO("Test data file is specified to: %s", testFileName );
		DataSet testDataSet(inputDataSet, testFileName);

		DeEPsClassifier deeps(inputDataSet, alphaSimilarity, supportRatio);


		int total = 0;
		int positive = 0;
		double classifyTimeTaken = 0;



		for(unsigned int i = 0; i < testDataSet.getInstances().size(); i++) {

			//get instance to classify
			Instance t = testDataSet.getInstances()[i];

			//classifiy instance from test data set

			clock_t t_start=clock();
			double result = deeps.classify(t);
			clock_t t_end=clock();
			classifyTimeTaken += ((t_end-t_start)*10)/CLOCKS_PER_SEC;

			LOG_INFO("Test instance %d - %s", i, t.toString() );
			LOG_INFO("Test instance class: %f", t.getLabelValue());
			LOG_INFO("DeEPs class: %f", result);

			if(t.getLabelValue() != Instance::MISSING_VALUE) {
				total++;

				if(result == t.getLabelValue()) {
					LOG_DEBUG("Classified positive");
					positive++;
				} else {
					LOG_DEBUG("Classified negative");
				}
			}

//			char c; cin >> c;
		}


		if(total > 0 ) {

			//summary info
			LOG_INFO("RESULTS:");
			LOG_INFO("--------");
			LOG_INFO("Data Set name: %s", inputDataSet.getName().c_str());
			LOG_INFO("Number of attributes: %d", inputDataSet.getAttributes().size());
			LOG_INFO("Number of train instances: %d", inputDataSet.getInstances().size());
			LOG_INFO("Number of test instances: %d", testDataSet.getInstances().size());
			LOG_INFO("Alpha similarity: %f", alphaSimilarity);
			LOG_INFO("EP support ratio: %f", supportRatio);
			LOG_INFO("Accuracy: %f", ((double)positive/(double)total)*100 );
			LOG_INFO("Time (sec) per instance: %f", classifyTimeTaken/testDataSet.getInstances().size());

		}

	} else if (numberOfInstance > 0 && testFileName == NULL) {

		LOG_INFO("Performing instance based fold test.");
		LOG_INFO("Number of instance to test: %d", numberOfInstance);

		int totalAll = 0;
		int totalPositive = 0;
		double classifyTimeTaken_clocks = 0;
		for(unsigned int i = 0; i < numberOfInstance; i++) {

			//get instance to classify
			Instance t = inputDataSet.getInstances()[0];

			//remove instance from input data sed
			inputDataSet.removeFirstElement();

			//remove instance from input DS
			DeEPsClassifier deeps(inputDataSet, alphaSimilarity, supportRatio);

			//classifiy instance from test data set

			clock_t t_start=clock();
			double result = deeps.classify(t);
			clock_t t_end=clock();
			classifyTimeTaken_clocks +=t_end-t_start;

			inputDataSet.addInstance(t);

			LOG_DEBUG("Test instance class: %f", t.getLabelValue());
			LOG_DEBUG("DeEPs class: %f", result);

			if(t.getLabelValue() != Instance::MISSING_VALUE) {
				totalAll++;

				if(result == t.getLabelValue()) {
					LOG_DEBUG("Classified positive");
					totalPositive++;
				} else {
					LOG_DEBUG("Classified negative");
				}
			}

		}


		if(totalAll > 0 ) {

			//summary info
			LOG_INFO("RESULTS:");
			LOG_INFO("--------");
			LOG_INFO("Data Set name: %s", inputDataSet.getName().c_str());
			LOG_INFO("Number of attributes: %d", inputDataSet.getAttributes().size());
			LOG_INFO("Number of classes: %d", inputDataSet.getLabelAttribute().getNominalValues().size());
			LOG_INFO("Number of all instances: %d", inputDataSet.getInstances().size());
			LOG_INFO("Number of instances in test: %d", numberOfInstance);
			LOG_INFO("Alpha similarity: %f", alphaSimilarity);
			LOG_INFO("EP support ratio: %f", supportRatio);
			LOG_INFO("Number of instance classified possitive: %d", totalPositive);
			LOG_INFO("Accuracy: %f", ((double)totalPositive/(double)totalAll)*100 );
			double timeInSec = ((classifyTimeTaken_clocks)*10)/CLOCKS_PER_SEC;
			LOG_INFO("Time (sec) per instance: %f", (timeInSec/(double)totalAll));

			//latex output
			printf("\n");
			printf("%s & %d & %d & %d & %f & %f & %f & %f", inputDataSet.getName().c_str()
															, inputDataSet.getInstances().size()
															, inputDataSet.getAttributes().size()
															, inputDataSet.getLabelAttribute().getNominalValues().size()
															, alphaSimilarity
															, supportRatio
															, ((double)totalPositive/(double)totalAll)*100
															, (timeInSec/(double)totalAll));
			printf("\n");
		}



	} else {
		LOG_ERROR("To run program test case should be specified! \nAs test input data file with records to classify (-t) or number of instances from source data set (-n).\n");
	}


	return EXIT_SUCCESS;

}






