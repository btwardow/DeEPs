/*	
 * Log.h
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

#ifndef LOG_H_
#define LOG_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>

#define LOG_LEVEL_INFO  0
#define LOG_LEVEL_DEBUG   1

extern int LOG_LEVEL;


//#define LOG_INFO(...)  { printf("[INFO]  ");printf(__VA_ARGS__); printf("\n"); }
//#define LOG_DEBUG(...) { printf("[DEBUG] ");printf(__VA_ARGS__); printf("\n"); }

void LOG_INFO(const char * fmt, ...);
void LOG_DEBUG(const char * fmt, ...);


#define LOG_ERROR(...) { printf("[ERROR] "); printf(__VA_ARGS__); printf("\n"); exit(EXIT_FAILURE); }





#endif /* LOG_H_ */
