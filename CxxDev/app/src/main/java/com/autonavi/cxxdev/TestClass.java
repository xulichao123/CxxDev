package com.autonavi.cxxdev;

import android.util.Log;

import java.lang.reflect.Array;
import java.util.ArrayList;
import java.util.List;

public class TestClass {
    private final static String TAG = "TestClass-J";

    public TestClass(){
        Log.i(TAG, "TestClass");
    }

    public void test(int index) {
        Log.i(TAG, "test : " + index);
    }

    public int testReturn(int index) {
        Log.i(TAG, "testReturn(index: " + index + ")");
        return index;
    }

    public String testReturn(String s) {
        Log.i(TAG, "testReturn(s: " + s);
        return s;
    }

    public List<Integer> testReturn(int a, String b, List<Integer> c) {
        Log.i(TAG, "testReturn(a: " + a + ", b: " + b);
        List<Integer> list = new ArrayList<>();
        list.add(1);
        list.add(2);
        list.add(3);
        return list;
    }

    public static void testStatic(String str) {
        Log.i(TAG, "testStatic : " + str);
    }

    public static class InnerClass {
        private int num;
        public InnerClass() {
            Log.i(TAG, "InnerClass");
        }

        public void setInt(int n) {
            num = n;
            Log.i(TAG, "setInt: num = " + num);
        }
    }

    public static ClassLoader getClassLoader() {
        return TestClass.class.getClassLoader();
    }
}
