/************************************************************************************
 *  Copyright 2006 Carleton Coffrin
 * 
 *  This file is part of Straight DMX.
 *  
 *  Straight DMX is free software; you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation; either version 2 of the License, or
 *  (at your option) any later version.
 *  
 *  Straight DMX is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with Straight DMX; if not, write to the Free Software
 *  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA
 *  
 *************************************************************************************/
 
#include <jni.h>
#include "dmx_dmxConnect.h"
#include "timedDMX.h"

/*
 * Class:     dmxConnect
 * Method:    open
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_dmx_dmxConnect_open
  (JNIEnv * env, jobject obj, jint i1, jint i2)
{
	return open(i1, i2);
}
/*
 * Class:     dmxConnect
 * Method:    close
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_dmx_dmxConnect_close
  (JNIEnv * env, jobject obj, jint i1)
{
	return close(i1);
}


/*
 * Class:     dmxConnect
 * Method:    isSimulation
 * Signature: (I)I
 */
JNIEXPORT jboolean JNICALL Java_dmx_dmxConnect_isSimulation
(JNIEnv * env, jobject obj, jint i1) {
	return isSimulation(i1);
}

/*
 * Class:     dmx_dmxConnect
 * Method:    setStartCode
 * Signature: (IS)I
 */
JNIEXPORT jint JNICALL Java_dmx_dmxConnect_setStartCode
  (JNIEnv * env, jobject obj, jint i1, jshort c1)
{
	return setStartCode(i1, (unsigned char)c1);
}


/*
 * Class:     dmxConnect
 * Method:    stopFade
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_dmx_dmxConnect_stopFade
(JNIEnv * env, jobject obj, jint i1) {
	return stopFade(i1);
}


/*
 * Class:     dmxConnect
 * Method:    setProfile
 * Signature: (II[S)I
 */
JNIEXPORT jint JNICALL Java_dmx_dmxConnect_setProfile
  (JNIEnv * env, jobject obj, jint i1, jint i2, jshortArray ca1)
{
	//jsize len = (*env)->GetArrayLength(env, arr);		
	DMXDATA * body = (unsigned char *)env->GetShortArrayElements(ca1, 0);
	DMXDATA fixed[256];
	for (int x = 0; x < 256; x++)
	{
		fixed[x] = body[2*x];
	}
	
	dmx_status s = setProfile(i1, i2, fixed);

	env->ReleaseShortArrayElements(ca1, (signed short *)body, 0);

	return s;
}
/*
 * Class:     dmxConnect
 * Method:    setProfiles
 * Signature: (I[[S)I
 */
JNIEXPORT jint JNICALL Java_dmx_dmxConnect_setProfiles
  (JNIEnv * env, jobject obj, jint i1, jobjectArray ca1)
{
	//jsize len = (*env)->GetArrayLength(env, arr);		
	//DMXDATA * body = (unsigned char *)env->GetShortArrayElements(ca1, 0);
	//DMXINT * body2 = (unsigned *)env->GetIntArrayElements(ia2, 0);
	DMXDATA fixed[512][256];
	for (int y = 0; y < 512; y++)
	{
		jshortArray sarr = (jshortArray)env->GetObjectArrayElement(ca1, y);
		DMXDATA * body = (unsigned char *)env->GetShortArrayElements(sarr, 0);
		for (int x = 0; x < 256; x++)
		{
			fixed[y][x] = body[2*x];
		}
		env->ReleaseShortArrayElements(sarr, (signed short *)body, 0);
		//env->ReleaseObjecttArrayElement(ca1, sarr, 0);
	}
	
	dmx_status s = setProfiles(i1, fixed);
	//env->ReleaseShortArrayElements(ca1, (signed short *)body, 0);
	return s;
}

/*
 * Class:     dmxConnect
 * Method:    clearProfile
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_dmx_dmxConnect_clearProfile
  (JNIEnv * env, jobject obj, jint i1, jint i2)
{
	return clearProfile(i1, i2);
}
/*
 * Class:     dmxConnect
 * Method:    clearProfiles
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_dmx_dmxConnect_clearProfiles
  (JNIEnv * env, jobject obj, jint i1)
{
	return clearProfiles(i1);
}



/*
 * Class:     dmx_dmxConnect
 * Method:    setProportionalOutput
 * Signature: (IIS)I
 */
JNIEXPORT jint JNICALL Java_dmx_dmxConnect_setProportionalOutput
  (JNIEnv * env, jobject obj, jint i1, jint i2, jshort c1)
{
	return setProportionalOutput(i1, i2, (unsigned char)c1);
}
/*
 * Class:     dmx_dmxConnect
 * Method:    setProportionalOutputs
 * Signature: (I[S)I
 */
JNIEXPORT jint JNICALL Java_dmx_dmxConnect_setProportionalOutputs
  (JNIEnv * env, jobject obj, jint i1, jshortArray ca1)
{
	//jsize len = (*env)->GetArrayLength(env, arr);		
	DMXDATA * body = (unsigned char *)env->GetShortArrayElements(ca1, 0);
	DMXDATA fixed[256];
	for (int x = 0; x < 256; x++)
	{
		fixed[x] = body[2*x];
	}
	
	dmx_status s = setProportionalOutputs(i1, fixed);

	env->ReleaseShortArrayElements(ca1, (signed short *)body, 0);

	return s;
}
/*
 * Class:     dmx_dmxConnect
 * Method:    clearProportionalOutput
 * Signature: (II)I
 */
JNIEXPORT jint JNICALL Java_dmx_dmxConnect_clearProportionalOutput
  (JNIEnv * env, jobject obj, jint i1, jint i2)
{
	return clearProportionalOutput(i1, i2);
}

/*
 * Class:     dmx_dmxConnect
 * Method:    clearProportionalOutputs
 * Signature: (I)I
 */
JNIEXPORT jint JNICALL Java_dmx_dmxConnect_clearProportionalOutputs
  (JNIEnv * env, jobject obj, jint i1)
{
	return clearProportionalOutputs(i1);
}
/*
 * Class:     dmx_dmxConnect
 * Method:    setGrandMaster
 * Signature: (IS)I
 */
JNIEXPORT jint JNICALL Java_dmx_dmxConnect_setGrandMaster
  (JNIEnv * env, jobject obj, jint i1, jshort c1)
{
	return setGrandMaster(i1, (unsigned char)c1);
}




/*
 * Class:     dmx_dmxConnect
 * Method:    setSingle
 * Signature: (IIS)I
 */
JNIEXPORT jint JNICALL Java_dmx_dmxConnect_setSingle
  (JNIEnv * env, jobject obj, jint i1, jint i2, jshort c2)
{
	//DMXDATA * body = (unsigned char *)env->GetShortArrayElements(ca1, 0);
	//DMXDATA fixed[512];
	//for (int x = 0; x < 512; x++)
	//{
	//	fixed[x] = body[2*x];
	//}
		
	dmx_status s = setSingle(i1, i2, (unsigned char)c2);

	//env->ReleaseShortArrayElements(ca1, (signed short *)body, 0);

	return s;
}
/*
 * Class:     dmx_dmxConnect
 * Method:    setSingleTimed
 * Signature: (IISI)I
 */
JNIEXPORT jint JNICALL Java_dmx_dmxConnect_setSingleTimed
  (JNIEnv * env, jobject obj, jint i1, jint i2, jshort c2, jint i3)
{
	//DMXDATA * body = (unsigned char *)env->GetShortArrayElements(ca1, 0);
	//DMXDATA fixed[512];
	//for (int x = 0; x < 512; x++)
	//{
	//	fixed[x] = body[2*x];
	//}
		
	dmx_status s = setSingleTimed(i1, i2, (unsigned char)c2, i3);

	//env->ReleaseShortArrayElements(ca1, (signed short *)body, 0);

	return s;
}
/*
 * Class:     dmxConnect
 * Method:    quickLoad
 * Signature: (IC[C)I
 */
JNIEXPORT jint JNICALL Java_dmx_dmxConnect_quickLoad
  (JNIEnv * env, jobject obj, jint i1, jshortArray ca1)
{
	//jsize len = (*env)->GetArrayLength(env, arr);		
	DMXDATA * body = (unsigned char *)env->GetShortArrayElements(ca1, 0);
	DMXDATA fixed[512];
	for (int x = 0; x < 512; x++)
	{
		fixed[x] = body[2*x];
	}
	
	dmx_status s = quickLoad(i1, fixed);

	env->ReleaseShortArrayElements(ca1, (signed short *)body, 0);

	return s;
}

/*
 * Class:     dmx_dmxConnect
 * Method:    fullLoadSplit
 * Signature: (I[SII)I
 */
JNIEXPORT jint JNICALL Java_dmx_dmxConnect_fullLoadSplit
  (JNIEnv * env, jobject obj, jint i1, jshortArray ca1, jint i2, jint i3)
{
	//jsize len = (*env)->GetArrayLength(env, arr);		
	DMXDATA * body = (unsigned char *)env->GetShortArrayElements(ca1, 0);
	DMXDATA fixed[512];
	for (int x = 0; x < 512; x++)
	{
		fixed[x] = body[2*x];
	}

	dmx_status s = fullLoadSplit(i1, fixed, i2, i3);

	env->ReleaseShortArrayElements(ca1, (signed short *)body, 0);

	return s;
}

/*
 * Class:     dmx_dmxConnect
 * Method:    fullLoad
 * Signature: (I[SI)I
 */
JNIEXPORT jint JNICALL Java_dmx_dmxConnect_fullLoad
  (JNIEnv * env, jobject obj, jint i1, jshortArray ca1, jint i2)
{
	//jsize len = (*env)->GetArrayLength(env, arr);		
	DMXDATA * body = (unsigned char *)env->GetShortArrayElements(ca1, 0);
	DMXDATA fixed[512];
	for (int x = 0; x < 512; x++)
	{
		fixed[x] = body[2*x];
	}

	dmx_status s = fullLoad(i1, fixed, i2);

	env->ReleaseShortArrayElements(ca1, (signed short *)body, 0);

	return s;
}

/*
 * Class:     dmx_dmxConnect
 * Method:    partialLoadSplit
 * Signature: (I[S[III)I
 */
JNIEXPORT jint JNICALL Java_dmx_dmxConnect_partialLoadSplit
  (JNIEnv * env, jobject obj, jint i1, jshortArray ca1, jintArray ia2, jint i2, jint i3)
{
	//jsize len = (*env)->GetArrayLength(env, arr);		
	DMXDATA * body1 = (unsigned char *)env->GetShortArrayElements(ca1, 0);
	DMXINT * body2 = (unsigned *)env->GetIntArrayElements(ia2, 0);
	DMXDATA fixed1[512];
//	DMXDATA fixed2[512];
	for (int x = 0; x < 512; x++)
	{
		fixed1[x] = body1[2*x];
		//fixed2[x] = body2[2*x];
	}

	dmx_status s = partialLoadSplit(i1, fixed1, body2, i2, i3);

	env->ReleaseShortArrayElements(ca1, (signed short *)body1, 0);
	env->ReleaseIntArrayElements(ia2, (signed long *)body2, 0);

	return s;
}

/*
 * Class:     dmx_dmxConnect
 * Method:    partialLoad
 * Signature: (I[S[II)I
 */
JNIEXPORT jint JNICALL Java_dmx_dmxConnect_partialLoad
  (JNIEnv * env, jobject obj, jint i1, jshortArray ca1, jintArray ia2, jint i2)
{
	//jsize len = (*env)->GetArrayLength(env, arr);		
	DMXDATA * body1 = (unsigned char *)env->GetShortArrayElements(ca1, 0);
	DMXINT * body2 = (unsigned *)env->GetIntArrayElements(ia2, 0);
	DMXDATA fixed1[512];
//	DMXDATA fixed2[512];
	for (int x = 0; x < 512; x++)
	{
		fixed1[x] = body1[2*x];
		//fixed2[x] = body2[2*x];
	}

	dmx_status s = partialLoad(i1, fixed1, body2, i2);

	env->ReleaseShortArrayElements(ca1, (signed short *)body1, 0);
	env->ReleaseIntArrayElements(ia2, (signed long *)body2, 0);

	return s;
}
/*
 * Class:     dmx_dmxConnect
 * Method:    complexLoadSplit
 * Signature: (I[S[I[I[I)I
 */
JNIEXPORT jint JNICALL Java_dmx_dmxConnect_complexLoadSplit
  (JNIEnv * env, jobject obj, jint i1, jshortArray ca1, jintArray ia2, jintArray ia3, jintArray ia4)
{
	//jsize len = (*env)->GetArrayLength(env, arr);		
	DMXDATA * body1 = (unsigned char *)env->GetShortArrayElements(ca1, 0);
	DMXINT * body2 = (unsigned *)env->GetIntArrayElements(ia2, 0);
	DMXINT * body3 = (unsigned *)env->GetIntArrayElements(ia3, 0);
	DMXINT * body4 = (unsigned *)env->GetIntArrayElements(ia4, 0);
	//DMXINT * body5 = (unsigned *)env->GetIntArrayElements(ia5, 0);
	//DMXINT * body6 = (unsigned *)env->GetIntArrayElements(ia6, 0);
	
	DMXDATA fixed1[512];
	for (int x = 0; x < 512; x++)
	{
		fixed1[x] = body1[2*x];
	}
	

	dmx_status s = complexLoadSplit(i1, fixed1, body2, body3, body4);

	env->ReleaseShortArrayElements(ca1, (signed short *)body1, 0);
	env->ReleaseIntArrayElements(ia2, (signed long *)body2, 0);
	env->ReleaseIntArrayElements(ia3, (signed long *)body3, 0);
	env->ReleaseIntArrayElements(ia4, (signed long *)body4, 0);

	return s;
}

/*
 * Class:     dmx_dmxConnect
 * Method:    complexLoad
 * Signature: (I[S[I[I)I
 */
JNIEXPORT jint JNICALL Java_dmx_dmxConnect_complexLoad
  (JNIEnv * env, jobject obj, jint i1, jshortArray ca1, jintArray ia2, jintArray ia3)
{
	//jsize len = (*env)->GetArrayLength(env, arr);		
	DMXDATA * body1 = (unsigned char *)env->GetShortArrayElements(ca1, 0);
	DMXINT * body2 = (unsigned *)env->GetIntArrayElements(ia2, 0);
	DMXINT * body3 = (unsigned *)env->GetIntArrayElements(ia3, 0);
	//DMXINT * body4 = (unsigned *)env->GetIntArrayElements(ia4, 0);
	//DMXINT * body5 = (unsigned *)env->GetIntArrayElements(ia5, 0);
	//DMXINT * body6 = (unsigned *)env->GetIntArrayElements(ia6, 0);
	
	DMXDATA fixed1[512];
	for (int x = 0; x < 512; x++)
	{
		fixed1[x] = body1[2*x];
	}
	

	dmx_status s = complexLoad(i1, fixed1, body2, body3);

	env->ReleaseShortArrayElements(ca1, (signed short *)body1, 0);
	env->ReleaseIntArrayElements(ia2, (signed long *)body2, 0);
	env->ReleaseIntArrayElements(ia3, (signed long *)body3, 0);
//	env->ReleaseIntArrayElements(ia4, (signed long *)body4, 0);

	return s;
}



/*
 * Class:     dmxConnect
 * Method:    getCurrentFade
 * Signature: (I[S)I
 */
JNIEXPORT jint JNICALL Java_dmx_dmxConnect_getCurrentFade
  (JNIEnv * env, jobject obj, jint i1, jshortArray ca1)
{

	DMXDATA d[512];

	dmx_status s = getCurrentFade(i1, d);

	DMXDATA * body1 = (unsigned char *)env->GetShortArrayElements(ca1, 0);

	for (int x = 0; x < 512; x++)
	{
		body1[2*x] = d[x];
	}
	env->ReleaseShortArrayElements(ca1, (signed short *)body1, 0);


	return s;
}

/*
 * Class:     dmxConnect
 * Method:    getCurrentOutput
 * Signature: (I[S)I
 */
JNIEXPORT jint JNICALL Java_dmx_dmxConnect_getCurrentOutput
  (JNIEnv * env, jobject obj, jint i1, jshortArray ca1)
{

	DMXDATA d[512];

	dmx_status s = getCurrentOutput(i1, d);

	DMXDATA * body1 = (unsigned char *)env->GetShortArrayElements(ca1, 0);

	for (int x = 0; x < 512; x++)
	{
		body1[2*x] = d[x];
	}
	env->ReleaseShortArrayElements(ca1, (signed short *)body1, 0);


	return s;
}
/*
 * Class:     dmxConnect
 * Method:    getVersion
 * Signature: ([C)I
 */
JNIEXPORT jint JNICALL Java_dmx_dmxConnect_getVersion
  (JNIEnv * env, jobject obj, jcharArray ca1)
{
	char t[512];
	dmx_status s = getVersion(t);

	DMXDATA * body1 = (unsigned char *)env->GetCharArrayElements(ca1, 0);

	for (int x = 0; x < 512; x++)
	{
		((char *)ca1)[2*x] = t[x];
	}

	env->ReleaseCharArrayElements(ca1, (unsigned short *)body1, 0);

	return s;
}
/*
 * Class:     dmxConnect
 * Method:    lookUpReturn
 * Signature: (I[C)I
 */
JNIEXPORT jint JNICALL Java_dmx_dmxConnect_lookUpReturn
  (JNIEnv * env, jobject obj, jint i1, jcharArray ca1)
{
	char t[512];
	dmx_status s = lookUpReturn(i1, t);

	DMXDATA * body1 = (unsigned char *)env->GetCharArrayElements(ca1, 0);

	for (int x = 0; x < 512; x++)
	{
		((char *)ca1)[2*x] = t[x];
	}
	
	env->ReleaseCharArrayElements(ca1, (unsigned short *)body1, 0);

	return s;
}

