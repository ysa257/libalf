/* $Id: jni_algorithm_delete2.cpp 564 2009-04-30 15:11:18Z davidpiegdon $
 * vim: fdm=marker
 *
 * libalf JNI - Java Native Interface for Automata Learning Factory
 *
 * (c) Daniel Neider, i7 Informatik RWTH-Aachen
 *     <neider@automata.rwth-aachen.de>
 *
 * see LICENSE file for licensing information.
 */

#include <iostream>

#include <libalf/knowledgebase.h>
#include <libalf/learning_algorithm.h>
#include <libalf/algorithm_DeLeTe2.h>

#include <jni.h>

#include "jni_algorithm_delete2.h"

using namespace std;
using namespace libalf;

JNIEXPORT jlong JNICALL Java_de_libalf_jni_JNIAlgorithmDeLeTe2_init__JI (JNIEnv *env, jobject obj, jlong knowledgebase_pointer, jint alphabet_size) {
	// Get the knowledgebase object
	knowledgebase<bool> *base = (knowledgebase<bool>*) knowledgebase_pointer;

	/*
	 * Return the new object
	 */
	learning_algorithm<bool>* algorithm = new DeLeTe2<bool>(base, NULL, alphabet_size);
	return ((jlong)algorithm);
}

JNIEXPORT jlong JNICALL Java_de_libalf_jni_JNIAlgorithmDeLeTe2_init__JIJ (JNIEnv *env, jobject obj, jlong knowledgebase_pointer, jint alphabet_size, jlong logger_pointer) {
	// Get the knowledgebase object
	knowledgebase<bool> *base = (knowledgebase<bool>*) knowledgebase_pointer;

	// Get the logger object
	buffered_logger *logger = (buffered_logger*) logger_pointer;

	/*
	 * Return the new object
	 */
	learning_algorithm<bool>* algorithm = new DeLeTe2<bool>(base, logger, alphabet_size);
	return ((jlong)algorithm);
}

