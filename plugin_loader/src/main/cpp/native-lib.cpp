#include <jni.h>
#include <android/log.h>
#include <stdio.h>
#include <string>
#include <android/asset_manager.h>
#include <android/asset_manager_jni.h>

#ifndef LOG_TAG
#define LOG_TAG "JNI_LOG" //Log 的 tag 名字
//定义各种类型 Log 的函数别名
#define LOGD(...) __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG ,__VA_ARGS__)
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO,LOG_TAG ,__VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN,LOG_TAG ,__VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR,LOG_TAG ,__VA_ARGS__)
#define LOGF(...) __android_log_print(ANDROID_LOG_FATAL,LOG_TAG ,__VA_ARGS__)
#endif

#ifdef __cplusplus
extern "C" {
#endif
static const char JSTRING[] = "Ljava/lang/String;";
static const char JCLASS_LOADER[] = "Ljava/lang/ClassLoader;";
static const char JCLASS[] = "Ljava/lang/Class;";
//定义类名
static const char *className = "wow/arthas/loader/Loader";
static const char *dexFileName = "plugin.apk";
static const char *pluginClassName = "wow.arthas.plugin.PluginImpl";


// 获取缓存apk目录
static void a(JNIEnv *env, jobject context, char *out);

// copy文件，从assets到目标文件路径
static bool b(AAssetManager *mgr, const char *dexFileName, char *dstPath);

// 获取DexClassLoader对象
static void c(JNIEnv *env, jstring dexPath, jstring optimizedDirectory, jobject classLoader);
// 确定优化dex目录
static void d();

static jint start(JNIEnv *env, jobject, jobject context);

static int registerNativeMethods(JNIEnv *env, const char *className, const JNINativeMethod *gMethods, int numMethods);

static char sig_buffer[512];
static char buffer[512];
static jobject dexClassLoader;

//定义对应Java native方法的 C++ 函数，函数名可以随意命名
static jstring stringFromJNI(JNIEnv *env, jobject) {
    LOGI("hello, this is native log.");
    const char *hello = "Hello from C++.";
    return env->NewStringUTF(hello);
}

long getCurrentTime() {
    struct timeval tv{};
    gettimeofday(&tv, nullptr);
    return tv.tv_sec * 1000 + tv.tv_usec / 1000;
}

static void a(JNIEnv *env, jobject context, char *out) {
    jclass contextCls = env->GetObjectClass(context);
    jmethodID getCacheId = env->GetMethodID(contextCls, "getCacheDir", "()Ljava/io/File;");
    jobject fileObject = env->CallObjectMethod(context, getCacheId);

    jclass fileCls = env->GetObjectClass(fileObject);
    jmethodID getAbsolutePathId = env->GetMethodID(fileCls, "getAbsolutePath", "()Ljava/lang/String;");
    auto absolutePath = (jstring) env->CallObjectMethod(fileObject, getAbsolutePathId);
    const char *absolutePathStr = env->GetStringUTFChars(absolutePath, nullptr);
    env->ReleaseStringUTFChars(absolutePath, absolutePathStr);
    strcpy(out, absolutePathStr);
}

/**
 * copy文件 从assets到指定文件
 * @param mgr asset管理工具
 * @param dexFileName assets下文件名称
 * @param dexFilePath copy到目标文件
 */
static bool b(AAssetManager *mgr, const char *dexFileName, char *dstPath) {
    // 获取 asset 对象，对应的文件名    UNKNOWN 0, RANDOM 1, STREAMING 2, BUFFER 3
    AAsset *asset = AAssetManager_open(mgr, dexFileName, AASSET_MODE_STREAMING);
    if (nullptr == asset) {
        LOGE("[plugin_loader] AAssetManager_open failed");
        return false;
    }

    off_t bufferSize = AAsset_getLength(asset);
    LOGD("[plugin_loader] Asset FileName:%s,extract path:%s,size:%ld\n", dexFileName, dstPath, bufferSize);

    FILE *file = fopen(dstPath, "wb");
    if (nullptr == file) {
        LOGE("[plugin_loader] fopen path failed");
        return false;
    }
    void *buffer = malloc(4096);
    while (true) {
        int numBytesRead = AAsset_read(asset, buffer, 4096);
        if (numBytesRead <= 0)
            break;
        fwrite(buffer, numBytesRead, 1, file);      // 写入到 file 内
    }
    free(buffer);
    fclose(file);
    AAsset_close(asset);
//    chmod(sig_buffer, 493);
    return true;
}

static void c(JNIEnv *env, jstring dexPath, jstring optimizedDirectory, jobject classLoader) {
    LOGI("loadDex start");

    jclass cls = env->FindClass("dalvik/system/DexClassLoader");
    LOGI("loadDex cls %p", cls);

    snprintf(sig_buffer, 512, "(%s%s%s%s)V", JSTRING, JSTRING, JSTRING, JCLASS_LOADER);
    jmethodID jmID = env->GetMethodID(cls, "<init>", sig_buffer);
    LOGI("loadDex jmID %p", jmID);

    jobject dex_loader_obj = env->NewObject(cls, jmID, dexPath, optimizedDirectory, NULL, classLoader);
    LOGI("loadDex dex_loader_obj %p", dex_loader_obj);
    dexClassLoader = dex_loader_obj;
}

static void loadDex(JNIEnv *env, jobject, jstring dexPath, jstring optimizedDirectory, jobject classload) {
    LOGI("loadDex start");

    jclass cls = env->FindClass("dalvik/system/DexClassLoader");

    LOGI("loadDex cls %p", cls);

    snprintf(sig_buffer, 512, "(%s%s%s%s)V", JSTRING, JSTRING, JSTRING, JCLASS_LOADER);
    jmethodID jmID = env->GetMethodID(cls, "<init>", sig_buffer);

    LOGI("loadDex jmID %p", jmID);
    jobject dex_loader_obj = env->NewObject(cls, jmID, dexPath, optimizedDirectory, NULL, classload);
    LOGI("loadDex dex_loader_obj %p", dex_loader_obj);


    snprintf(sig_buffer, 512, "(%s)%s", JSTRING, JCLASS);
    jmethodID loadClass_method = env->GetMethodID(cls, "loadClass", sig_buffer);
    LOGI("loadDex loadClass_method %p", loadClass_method);
    jstring class_name = env->NewStringUTF("freelifer.jiami.dexdemo.A");
//    jclass entry_class = static_cast<jclass>(env->CallObjectMethod(dex_loader_obj, loadClass_method, class_name));
    auto entry_class = (jclass) env->CallObjectMethod(dex_loader_obj, loadClass_method, class_name);

    LOGI("loadDex entry_class %p", entry_class);
    jmethodID invoke_method = env->GetStaticMethodID(entry_class, "get", "()Ljava/lang/String;");

    LOGI("loadDex invoke_method %p", invoke_method);
    auto pring = (jstring) env->CallStaticObjectMethod(entry_class, invoke_method);


//    jclass clazz = env->FindClass("com/yyyy/loader/A");
//    jmethodID jmId = env->GetStaticMethodID(clazz, "get", "()Ljava/lang/String;");
//    jstring pring =(jstring)env->CallStaticObjectMethod(clazz, jmId);
    const char *printStr = env->GetStringUTFChars(pring, nullptr);

//    LOGI("loadDex end %p", obj);
    LOGI("loadDex end %s", printStr);
    env->ReleaseStringUTFChars(pring, printStr);
}


static jint start(JNIEnv *env, jobject, jobject context) {
    long startTime = getCurrentTime();
    LOGI("[plugin_loader] start %ld", startTime);

    jclass contextCls = env->GetObjectClass(context);

    // 获取Application下的PathClassLoader
    jmethodID getClassLoaderMethodId = env->GetMethodID(contextCls, "getClassLoader", "()Ljava/lang/ClassLoader;");
    jobject classLoader = env->CallObjectMethod(context, getClassLoaderMethodId);

    // 获取assets
    jmethodID getAssets = env->GetMethodID(contextCls, "getAssets", "()Landroid/content/res/AssetManager;");
    jobject Assets_obj = env->CallObjectMethod(context, getAssets);

    // 获取asset
    AAssetManager *mgr = AAssetManager_fromJava(env, Assets_obj);      // 获取 AssetManager 对象
    LOGI("[plugin_loader] AAssetManager %p", mgr);
    if (mgr == nullptr) {
        LOGE("[plugin_loader] getAAssetManager failed");
        return 1;
    }

    char cacheDir[256];
    char dstPath[256];
    a(env, context, cacheDir);
    snprintf(dstPath, 256, "%s/%s", cacheDir, dexFileName);

    if (!b(mgr, dexFileName, dstPath)) {
        LOGE("[plugin_loader] copy from asset to cacheDir failed");
        return 2;
    }

    c(env, env->NewStringUTF(dstPath), env->NewStringUTF(cacheDir), classLoader);
    if (nullptr == dexClassLoader) {
        LOGE("[plugin_loader] create dexClassLoader failed");
        return 3;
    }

    // 调用插件中的启动类初始化
    jclass dexClassLoaderCls = env->GetObjectClass(dexClassLoader);
    snprintf(sig_buffer, 512, "(%s)%s", JSTRING, JCLASS);
    jmethodID loadClassMethodID = env->GetMethodID(dexClassLoaderCls, "loadClass", sig_buffer);
    LOGI("loadDex loadClass_method %p", loadClassMethodID);

    jstring pluginClassNameStr = env->NewStringUTF(pluginClassName);
    auto pluginClass = (jclass) env->CallObjectMethod(dexClassLoader, loadClassMethodID, pluginClassNameStr);
    if (nullptr == pluginClass) {
        LOGE("[plugin_loader] DexClassLoader loadClass failed");
        return 100;
    }

    jmethodID initMethodId = env->GetMethodID(pluginClass, "<init>", "()V");
    if (nullptr == initMethodId) {
        LOGE("[plugin_loader] %s not found ()V", pluginClassName);
        return 101;
    }

    jobject pluginObject = env->NewObject(pluginClass, initMethodId);
    if (nullptr == pluginObject) {
        LOGE("[plugin_loader] %s NewObject failed", pluginClassName);
        return 102;
    }

    jmethodID onCreateMethodId = env->GetMethodID(pluginClass, "onCreate", "(Landroid/content/Context;)V");
    if (nullptr == onCreateMethodId) {
        LOGE("[plugin_loader] %s onCreate not found", pluginClassName);
        return 103;
    }

    env->CallVoidMethod(pluginObject, onCreateMethodId, context);

    long endTime = getCurrentTime();
    LOGI("[plugin_loader] end %ld %ld", endTime, (endTime - startTime));
    return 0;
}

/*
 * 定义函数映射表（是一个数组，可以同时定义多个函数的映射）
 * 参数1：Java 方法名
 * 参数2：方法描述符，也就是签名
 * 参数3：C++定义对应 Java native方法的函数名
 */
static JNINativeMethod jni_Methods_table[] = {
//        {"load",  "(Ljava/lang/String;Ljava/lang/String;Ljava/lang/ClassLoader;)V", (void *) loadDex},
        {"start", "(Landroid/content/Context;)I",                                   (void *) start},
};

//根据函数映射表注册函数
static int registerNativeMethods(JNIEnv *env, const char *className,
                                 const JNINativeMethod *gMethods, int numMethods) {
    jclass clazz;
    LOGI("Registering %s natives\n", className);
    clazz = (env)->FindClass(className);
    if (nullptr == clazz) {
        LOGE("Native registration unable to find class '%s'\n", className);
        return JNI_ERR;
    }

    if ((env)->RegisterNatives(clazz, gMethods, numMethods) < 0) {
        LOGE("Register natives failed for '%s'\n", className);
        return JNI_ERR;
    }
    //删除本地引用
    (env)->DeleteLocalRef(clazz);
    return JNI_OK;
}

jint JNI_OnLoad(JavaVM *vm, void *reserved) {
    LOGI("call JNI_OnLoad");

    JNIEnv *env = nullptr;

    if (vm->GetEnv((void **) &env, JNI_VERSION_1_4) != JNI_OK) {  //判断 JNI 版本是否为JNI_VERSION_1_4
        return JNI_EVERSION;
    }

    registerNativeMethods(env, className, jni_Methods_table, sizeof(jni_Methods_table) / sizeof(JNINativeMethod));

    return JNI_VERSION_1_4;
}
#ifdef __cplusplus
}
#endif