package wow.arthas.loader;

import android.content.Context;

/**
 * @author zhukun on 2019-07-28.
 */
public class Loader {

    static {
        System.loadLibrary("native-lib");
    }

    public static native int start(Context context);

    public static native void load(String dexPath, String optimizedDirectory, ClassLoader cl);
}
