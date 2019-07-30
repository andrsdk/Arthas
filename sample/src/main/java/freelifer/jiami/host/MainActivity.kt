package freelifer.jiami.host

import android.content.Context
import android.support.v7.app.AppCompatActivity
import android.os.Bundle
import android.util.Log
import android.widget.Button
import android.widget.Toast
import com.yyyy.loader.Loader
import dalvik.system.DexClassLoader
import freelifer.kotlin.core.HostProvider

class MainActivity : AppCompatActivity() {
    var dexPath: String? = null
    var  fileName = "dex-debug.apk"
    var cacheDir: String? = null
    var dexClassLoader: DexClassLoader? = null

    override fun attachBaseContext(newBase: Context) {
        super.attachBaseContext(newBase)
//        dexPath = Utils.copyFiles(newBase, fileName)
//        cacheDir = Utils.getCacheDir(newBase).getAbsolutePath();
//        Log.i("kzhu", "cache $cacheDir")

    }
    override fun onCreate(savedInstanceState: Bundle?) {
        super.onCreate(savedInstanceState)
        setContentView(R.layout.activity_main)

        Log.i("kzhu", "getSystemClassLoader ${ClassLoader.getSystemClassLoader()}")
        Log.i("kzhu", "classLoader ${classLoader}")
        Log.i("kzhu", "Application classLoader ${applicationContext.classLoader}")
//        dexClassLoader = DexClassLoader(dexPath, cacheDir, null, classLoader)
//
        findViewById<Button>(R.id.callDexBtn).setOnClickListener {

            Log.i("kzhu", "I'm from host ${Thread.currentThread().name}");
            HostProvider.getInstance().handle("hahaType")
//            try {
//                val clazz = dexClassLoader!!.loadClass("freelifer.jiami.dexdemo.Core");
//                val method = clazz.getMethod("init", Context::class.java)
//                Toast.makeText(getApplicationContext()," host call dex==>> ${method.invoke(null, applicationContext)}",Toast.LENGTH_LONG).show();
//            } catch (e: Exception ) {
//                e.printStackTrace();
//            }
        }
//
//        Loader.load(dexPath, cacheDir, classLoader)

        val ret = Loader.start(applicationContext)

        Log.i("kzhu", "Current exec result ${ret}");
    }
}
