/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class xreal_CVar */

#ifndef _Included_xreal_CVar
#define _Included_xreal_CVar
#ifdef __cplusplus
extern "C" {
#endif
#undef xreal_CVar_ARCHIVE
#define xreal_CVar_ARCHIVE 1L
#undef xreal_CVar_USERINFO
#define xreal_CVar_USERINFO 2L
#undef xreal_CVar_SERVERINFO
#define xreal_CVar_SERVERINFO 4L
#undef xreal_CVar_SYSTEMINFO
#define xreal_CVar_SYSTEMINFO 8L
#undef xreal_CVar_INIT
#define xreal_CVar_INIT 16L
#undef xreal_CVar_LATCH
#define xreal_CVar_LATCH 32L
#undef xreal_CVar_ROM
#define xreal_CVar_ROM 64L
#undef xreal_CVar_TEMP
#define xreal_CVar_TEMP 256L
#undef xreal_CVar_CHEAT
#define xreal_CVar_CHEAT 512L
#undef xreal_CVar_NORESTART
#define xreal_CVar_NORESTART 1024L
/*
 * Class:     xreal_CVar
 * Method:    register0
 * Signature: (Ljava/lang/String;Ljava/lang/String;I)I
 */
JNIEXPORT jint JNICALL Java_xreal_CVar_register0
  (JNIEnv *, jclass, jstring, jstring, jint);

/*
 * Class:     xreal_CVar
 * Method:    set0
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_xreal_CVar_set0
  (JNIEnv *, jclass, jstring, jstring);

/*
 * Class:     xreal_CVar
 * Method:    reset0
 * Signature: (Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_xreal_CVar_reset0
  (JNIEnv *, jclass, jstring);

/*
 * Class:     xreal_CVar
 * Method:    getString0
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_xreal_CVar_getString0
  (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_CVar
 * Method:    getValue0
 * Signature: (I)F
 */
JNIEXPORT jfloat JNICALL Java_xreal_CVar_getValue0
  (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_CVar
 * Method:    getInteger0
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_xreal_CVar_getInteger0
  (JNIEnv *, jclass, jint);

#ifdef __cplusplus
}
#endif
#endif
