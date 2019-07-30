package freelifer.kotlin.core;

/**
 * @author zhukun on 2019-07-30.
 */
public class HostProvider {

    private static class Holder {
        private static final HostProvider INSTANCE = new HostProvider();
    }

    public static HostProvider getInstance() {
        return Holder.INSTANCE;
    }


    private Callback callback;

    public void setCallback(Callback cb) {
        this.callback = cb;
    }

    public void handle(String type) {
        if (callback != null) {
            callback.handler(type);
        }
    }
}
