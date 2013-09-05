/*
 * Log.h
 *
 *  Created on: Oct 24, 2011
 *      Author: prophesci
 */

#ifndef LOG_H_
#define LOG_H_

#include <stdio.h>
#include <iostream>

#ifdef SLH_ANDROID
	#include <android/log.h>

	#define  LOG_TAG    "libslhjni"
	#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
	#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)
	#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG, LOG_TAG, __VA_ARGS__)
#else
	#define  LOGI(...)  fprintf(stdout, __VA_ARGS__); fprintf(stdout, "\n"); fflush(stdout);
	#define  LOGE(...)  fprintf(stdout, "!!!ERROR!!! "); fprintf(stdout, __VA_ARGS__); fprintf(stdout, "\n"); fflush(stdout);
	#define  LOGD(...)  fprintf(stdout, "<<<DEBUG>>> "); fprintf(stdout, __VA_ARGS__); fprintf(stdout, "\n"); fflush(stdout);
#endif


#endif /* LOG_H_ */
