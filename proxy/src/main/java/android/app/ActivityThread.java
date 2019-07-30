package android.app;

import android.annotation.TargetApi;
import android.os.Build;
import android.util.ArrayMap;

import java.lang.ref.WeakReference;

/**
 * @author zhukun on 2019-07-28.
 */
@TargetApi(Build.VERSION_CODES.KITKAT)
public class ActivityThread {

    final ArrayMap<String, WeakReference<LoadedApk>> mPackages = new ArrayMap<>();

    public static ActivityThread currentActivityThread() {
        return null;
    }
}
