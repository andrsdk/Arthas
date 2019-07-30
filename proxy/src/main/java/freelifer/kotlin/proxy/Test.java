package freelifer.kotlin.proxy;

import android.app.ActivityThread;
import android.app.Proxy;

/**
 * @author zhukun on 2019-07-28.
 */
public class Test {

    public static void test() {
//        ActivityThread.currentActivityThread()
        Proxy.invoke();
    }
}
