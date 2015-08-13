/*	
 * Log.cpp
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

#include "Log.h"

//default log level
int LOG_LEVEL = LOG_LEVEL_INFO;


void LOG_INFO(const char * fmt, ...) {
	if(LOG_LEVEL >= LOG_LEVEL_INFO) {
		printf("[INFO]    ");
		va_list ap;
		va_start(ap, fmt); /* Initialize the va_list */
		vprintf(fmt, ap); /* Call vprintf */
		va_end(ap); /* Cleanup the va_list */
		printf("\n");
	}
}


void LOG_DEBUG(const char * fmt, ...) {
	if(LOG_LEVEL >= LOG_LEVEL_DEBUG) {
		printf("[VERBOSE] ");
		va_list ap;
		va_start(ap, fmt); /* Initialize the va_list */
		vprintf(fmt, ap); /* Call vprintf */
		va_end(ap); /* Cleanup the va_list */
		printf("\n");
	}
}

