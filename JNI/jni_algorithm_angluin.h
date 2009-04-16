/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class de_libalf_jni_AlgorithmAngluin */

#ifndef _Included_de_libalf_jni_AlgorithmAngluin
#define _Included_de_libalf_jni_AlgorithmAngluin
#ifdef __cplusplus
extern "C" {
#endif
/*
 * Class:     de_libalf_jni_AlgorithmAngluin
 * Method:    init
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_de_libalf_jni_AlgorithmAngluin_init
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     de_libalf_jni_AlgorithmAngluin
 * Method:    add_counterexample
 * Signature: ([II)V
 */
JNIEXPORT void JNICALL Java_de_libalf_jni_AlgorithmAngluin_add_1counterexample
  (JNIEnv *, jobject, jintArray, jint);

/*
 * Class:     de_libalf_jni_AlgorithmAngluin
 * Method:    advance
 * Signature: (I)Lde/libalf/jni/BasicAutomaton;
 */
JNIEXPORT jobject JNICALL Java_de_libalf_jni_AlgorithmAngluin_advance
  (JNIEnv *, jobject, jint);

/*
 * Class:     de_libalf_jni_AlgorithmAngluin
 * Method:    conjecture_ready
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_de_libalf_jni_AlgorithmAngluin_conjecture_1ready
  (JNIEnv *, jobject, jint);

/*
 * Class:     de_libalf_jni_AlgorithmAngluin
 * Method:    get_alphabet_size
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_de_libalf_jni_AlgorithmAngluin_get_1alphabet_1size
  (JNIEnv *, jobject, jint);

/*
 * Class:     de_libalf_jni_AlgorithmAngluin
 * Method:    increase_alphabet_size
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_de_libalf_jni_AlgorithmAngluin_increase_1alphabet_1size
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     de_libalf_jni_AlgorithmAngluin
 * Method:    set_alphabet_size
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_de_libalf_jni_AlgorithmAngluin_set_1alphabet_1size
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     de_libalf_jni_AlgorithmAngluin
 * Method:    set_knowledge_source
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_de_libalf_jni_AlgorithmAngluin_set_1knowledge_1source
  (JNIEnv *, jobject, jint, jint);

/*
 * Class:     de_libalf_jni_AlgorithmAngluin
 * Method:    set_knowledge_source_NULL
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_de_libalf_jni_AlgorithmAngluin_set_1knowledge_1source_1NULL
  (JNIEnv *, jobject, jint);

/*
 * Class:     de_libalf_jni_AlgorithmAngluin
 * Method:    sync_to_knowledgebase
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_de_libalf_jni_AlgorithmAngluin_sync_1to_1knowledgebase
  (JNIEnv *, jobject, jint);

/*
 * Class:     de_libalf_jni_AlgorithmAngluin
 * Method:    supports_sync
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_de_libalf_jni_AlgorithmAngluin_supports_1sync
  (JNIEnv *, jobject, jint);

#ifdef __cplusplus
}
#endif
#endif
