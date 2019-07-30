package freelifer.jiami.dexdemo;

import android.os.Handler;
import android.os.Looper;
import android.util.Log;

/**
 * @author zhukun on 2019-07-27.
 */
public class A {

    static Handler h = new Handler(Looper.getMainLooper());
    static int number = 0;

    public static String get() {
        Log.i("kzhu", "start .");
        Runnable r = new Runnable() {
            @Override
            public void run() {
                Log.i("kzhu", "loop " + ++number);
                h.removeCallbacksAndMessages(null);
                h.postDelayed(this, 1000);
            }
        };
        h.postDelayed(r, 1000);
        return "hello from dex!!!";
    }
}
