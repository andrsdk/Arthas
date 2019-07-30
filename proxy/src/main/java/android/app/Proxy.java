package android.app;

import dalvik.system.DexClassLoader;

import java.lang.ref.WeakReference;

/**
 * @author zhukun on 2019-07-28.
 */
public class Proxy {

    public static void invoke() {
        WeakReference<LoadedApk> wr = ActivityThread.currentActivityThread().mPackages.get("packagename");
        DexClassLoader dLoader = new DexClassLoader("", "", "", wr.get().getClassLoader());
//        wr.get().setClass(dLoader);
    }
}
