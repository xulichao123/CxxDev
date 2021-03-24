package com.autonavi.cxxdev;

import androidx.appcompat.app.AppCompatActivity;

import android.os.Bundle;
import android.util.Log;
import android.widget.TextView;

import java.util.ArrayList;

public class MainActivity extends AppCompatActivity {
    private final static String TAG = "TestClass-J";

    // Used to load the 'native-lib' library on application startup.
    static {
        System.loadLibrary("native-lib");
    }

    private JniThread mJniThread;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
//
//        // Example of a call to a native method
//        TextView tv = findViewById(R.id.sample_text);
//        tv.setText(stringFromJNI());
//
//        TestClass tc = new TestClass();
//
//        ArrayList<User> userList = new ArrayList<User>();
//        for(int i=0;i<10;i++){
//            User u = new User((long)(1000+i),"xulc"+i,true,21);
//            userList.add(u);
//        }
//        setUserList(userList);
//
//        userList = getUserList();
//        for (int i = 0; i < userList.size(); i++) {
//            Log.d(TAG, "UserItem / id:" + userList.get(i).getId() + " / name:" + userList.get(i).getUserName());
//        }
//
//        copyConstructor();
//
//        Log.d(TAG, "consumerAndProductor-J-start");
//        consumerAndProductor();
//        Log.d(TAG, "consumerAndProductor-J-end");
//
//        templateTest();
//
//        templateParam();
//
//        variadicTemplate();

        //multipleThreadAccess();

        //multipleThreadClassNotFound();


//        startAttachCurrentThread();


//        testObjectGc();


//        callJavaException(new User(1, "HelloJni", true, 2));

//        try {
//            nativeThrowException();
//        } catch (IllegalArgumentException e) {
//            e.printStackTrace();
//        }


        startMonitorThread();
    }

    private class JniThread extends Thread {
        @Override
        public void run() {
            super.run();

//            while (true) {
//                Log.d(TAG, "JniThread / run");
//            }
            startAttachCurrentThread();
        }
    }

    /**
     * JNI层持有对象和Java层生命周期不一致导致崩溃
     */
    private void testObjectGc() {
        User user = new User();
        user.setId(1);
        user.setAge(10);
        user.setMan(true);
        user.setUserName("super");

        setUser(user);
        user = null;
        setUser(user);
    }


    private void startMonitorThread() {
        for (int i = 0; i < 1000; i++) {
            new Thread(new Runnable() {
                @Override
                public void run() {
                    nativeSetMonitorData();
                }
            }).start();
        }
        for (int i = 0; i < 1000; i++) {
            new Thread(new Runnable() {
                @Override
                public void run() {
                    setMonitorData();
                }
            }).start();
        }
    }

    private int mMonitorData = 0;
    private void setMonitorData() {
        synchronized (this) {
            mMonitorData++;
            Log.d(TAG, "setMonitorData / mMonitorData=" + mMonitorData);
        }
    }

    /**
     * A native method that is implemented by the 'native-lib' native library,
     * which is packaged with this application.
     */
    public native String stringFromJNI();

    public native void setUserList(ArrayList<User> list);

    public native ArrayList<User> getUserList();

    public native void copyConstructor();

    public native void consumerAndProductor();

    public native void templateTest();

    public native void templateParam();

    public native void variadicTemplate();

    public native void multipleThreadAccess();

    public native void multipleThreadClassNotFound();

    public native void startAttachCurrentThread();

    public native void setUser(User user);

    public native void callJavaException(User user);

    public native void nativeThrowException();

    public native void nativeSetMonitorData();
}