/* DO NOT EDIT THIS FILE - it is machine generated */
#include <jni.h>
/* Header for class xreal_client_Client */

#ifndef _Included_xreal_client_Client
#define _Included_xreal_client_Client
#ifdef __cplusplus
extern "C" {
#endif
#undef xreal_client_Client_CMD_BACKUP
#define xreal_client_Client_CMD_BACKUP 64L
#undef xreal_client_Client_CMD_MASK
#define xreal_client_Client_CMD_MASK 63L
/*
 * Class:     xreal_client_Client
 * Method:    getConfigString
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_xreal_client_Client_getConfigString
  (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_client_Client
 * Method:    getCurrentSnapshotNumber
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_xreal_client_Client_getCurrentSnapshotNumber
  (JNIEnv *, jclass);

/*
 * Class:     xreal_client_Client
 * Method:    getCurrentSnapshotTime
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_xreal_client_Client_getCurrentSnapshotTime
  (JNIEnv *, jclass);

/*
 * Class:     xreal_client_Client
 * Method:    getSnapshot
 * Signature: (I)Lxreal/client/Snapshot;
 */
JNIEXPORT jobject JNICALL Java_xreal_client_Client_getSnapshot
  (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_client_Client
 * Method:    getServerCommand
 * Signature: (I)[Ljava/lang/String;
 */
JNIEXPORT jobjectArray JNICALL Java_xreal_client_Client_getServerCommand
  (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_client_Client
 * Method:    getKeyCatchers
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_xreal_client_Client_getKeyCatchers
  (JNIEnv *, jclass);

/*
 * Class:     xreal_client_Client
 * Method:    setKeyCatchers
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_xreal_client_Client_setKeyCatchers
  (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_client_Client
 * Method:    getKeyBinding
 * Signature: (I)Ljava/lang/String;
 */
JNIEXPORT jstring JNICALL Java_xreal_client_Client_getKeyBinding
  (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_client_Client
 * Method:    setKeyBinding
 * Signature: (ILjava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_xreal_client_Client_setKeyBinding
  (JNIEnv *, jclass, jint, jstring);

/*
 * Class:     xreal_client_Client
 * Method:    isKeyDown
 * Signature: (I)Z
 */
JNIEXPORT jboolean JNICALL Java_xreal_client_Client_isKeyDown
  (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_client_Client
 * Method:    clearKeyStates
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_xreal_client_Client_clearKeyStates
  (JNIEnv *, jclass);

/*
 * Class:     xreal_client_Client
 * Method:    getCurrentCommandNumber
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_xreal_client_Client_getCurrentCommandNumber
  (JNIEnv *, jclass);

/*
 * Class:     xreal_client_Client
 * Method:    getOldestCommandNumber
 * Signature: ()I
 */
JNIEXPORT jint JNICALL Java_xreal_client_Client_getOldestCommandNumber
  (JNIEnv *, jclass);

/*
 * Class:     xreal_client_Client
 * Method:    getUserCommand
 * Signature: (I)Lxreal/UserCommand;
 */
JNIEXPORT jobject JNICALL Java_xreal_client_Client_getUserCommand
  (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_client_Client
 * Method:    registerSound
 * Signature: (Ljava/lang/String;)I
 */
JNIEXPORT jint JNICALL Java_xreal_client_Client_registerSound
  (JNIEnv *, jclass, jstring);

/*
 * Class:     xreal_client_Client
 * Method:    startSound
 * Signature: (FFFIII)V
 */
JNIEXPORT void JNICALL Java_xreal_client_Client_startSound
  (JNIEnv *, jclass, jfloat, jfloat, jfloat, jint, jint, jint);

/*
 * Class:     xreal_client_Client
 * Method:    startLocalSound
 * Signature: (II)V
 */
JNIEXPORT void JNICALL Java_xreal_client_Client_startLocalSound
  (JNIEnv *, jclass, jint, jint);

/*
 * Class:     xreal_client_Client
 * Method:    addLoopingSound
 * Signature: (IFFFFFFI)V
 */
JNIEXPORT void JNICALL Java_xreal_client_Client_addLoopingSound
  (JNIEnv *, jclass, jint, jfloat, jfloat, jfloat, jfloat, jfloat, jfloat, jint);

/*
 * Class:     xreal_client_Client
 * Method:    addRealLoopingSound
 * Signature: (IFFFFFFI)V
 */
JNIEXPORT void JNICALL Java_xreal_client_Client_addRealLoopingSound
  (JNIEnv *, jclass, jint, jfloat, jfloat, jfloat, jfloat, jfloat, jfloat, jint);

/*
 * Class:     xreal_client_Client
 * Method:    updateEntitySoundPosition
 * Signature: (IFFF)V
 */
JNIEXPORT void JNICALL Java_xreal_client_Client_updateEntitySoundPosition
  (JNIEnv *, jclass, jint, jfloat, jfloat, jfloat);

/*
 * Class:     xreal_client_Client
 * Method:    stopLoopingSound
 * Signature: (I)V
 */
JNIEXPORT void JNICALL Java_xreal_client_Client_stopLoopingSound
  (JNIEnv *, jclass, jint);

/*
 * Class:     xreal_client_Client
 * Method:    clearLoopingSounds
 * Signature: (Z)V
 */
JNIEXPORT void JNICALL Java_xreal_client_Client_clearLoopingSounds
  (JNIEnv *, jclass, jboolean);

/*
 * Class:     xreal_client_Client
 * Method:    respatialize
 * Signature: (IFFFFFFFZ)V
 */
JNIEXPORT void JNICALL Java_xreal_client_Client_respatialize
  (JNIEnv *, jclass, jint, jfloat, jfloat, jfloat, jfloat, jfloat, jfloat, jfloat, jboolean);

/*
 * Class:     xreal_client_Client
 * Method:    startBackgroundTrack
 * Signature: (Ljava/lang/String;Ljava/lang/String;)V
 */
JNIEXPORT void JNICALL Java_xreal_client_Client_startBackgroundTrack
  (JNIEnv *, jclass, jstring, jstring);

/*
 * Class:     xreal_client_Client
 * Method:    stopBackgroundTrack
 * Signature: ()V
 */
JNIEXPORT void JNICALL Java_xreal_client_Client_stopBackgroundTrack
  (JNIEnv *, jclass);

#ifdef __cplusplus
}
#endif
#endif
