package wow.arthas.plugin;

import android.content.BroadcastReceiver;
import android.content.Context;
import android.content.Intent;
import android.content.IntentFilter;
import android.os.Handler;
import android.os.Looper;
import android.util.Log;
import freelifer.kotlin.core.B;
import freelifer.kotlin.core.Callback;
import freelifer.kotlin.core.HostProvider;

/**
 * @author zhukun on 2019-07-30.
 */
public class PluginImpl {
    static Handler h = new Handler(Looper.getMainLooper());
    static int number = 0;


    void onCreate(Context context) {
        Log.i("plugin", "C++ call java success,");

        IntentFilter intentFilter = new IntentFilter();
        intentFilter.addAction(Intent.ACTION_SCREEN_ON);
        intentFilter.addAction(Intent.ACTION_SCREEN_OFF);
        intentFilter.addAction(Intent.ACTION_USER_PRESENT);

        context.registerReceiver(new BroadcastReceiver() {
            @Override
            public void onReceive(Context context, Intent intent) {
                Log.i("plugin", "==> " + intent.getAction());
            }
        }, intentFilter);


        Runnable r = new Runnable() {
            @Override
            public void run() {
                Log.i("plugin", "loop " + ++number);
                h.removeCallbacksAndMessages(null);
                h.postDelayed(this, 1000);
            }
        };
        h.postDelayed(r, 1000);

        HostProvider.getInstance().setCallback(new Callback() {
            @Override
            public void handler(String type) {
                Log.i("plugin", "i'm from plugin type " + type + " " + Thread.currentThread().getName());
            }
        });

        Log.i("plugin", "onCreate() end. " + B.get("plugin"));
    }
}
