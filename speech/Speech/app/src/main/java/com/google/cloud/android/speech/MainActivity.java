/*
 * Copyright 2016 Google Inc. All Rights Reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.google.cloud.android.speech;

import android.Manifest;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.bluetooth.BluetoothSocket;
import android.content.ComponentName;
import android.content.Intent;
import android.content.ServiceConnection;
import android.content.pm.PackageManager;
import android.content.res.Resources;
import android.os.AsyncTask;
import android.os.Bundle;
import android.os.IBinder;
import android.support.annotation.NonNull;
import android.support.v4.app.ActivityCompat;
import android.support.v4.content.res.ResourcesCompat;
import android.support.v7.app.AppCompatActivity;
import android.support.v7.widget.LinearLayoutManager;
import android.support.v7.widget.RecyclerView;
import android.support.v7.widget.Toolbar;
import android.text.TextUtils;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.Menu;
import android.view.MenuItem;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import java.io.IOException;
import java.util.ArrayList;
import java.util.UUID;
// Regular Expressions
import java.util.regex.Matcher;
import java.util.regex.Pattern;


public class MainActivity extends AppCompatActivity implements MessageDialogFragment.Listener {
    //My code:
    private final int REQ_CODE_SPEECH_INPUT = 100;
    public static String EXTRA_DEVICE_ADDRESS = "device_address";

    // Member fields
    private BluetoothDevice mBluetooth = null;
    private BluetoothAdapter mBtAdapter = null;

    BluetoothSocket btSocket = null;
    private boolean isBtConnected = false;
    static final UUID myUUID = UUID.fromString("00001101-0000-1000-8000-00805F9B34FB");
    String drago_address = "98:D3:32:31:18:B8";
    String bee_address = "98:D3:34:91:0A:BF";
    String arma_address = "98:D3:32:10:F7:87";
    String heg_address = "98:D3:32:30:F0:B5";

    private boolean ConnectSuccess = true;
    private Toolbar mtoolbar;


    private static final String FRAGMENT_MESSAGE_DIALOG = "message_dialog";

    private static final String STATE_RESULTS = "results";

    private static final int REQUEST_RECORD_AUDIO_PERMISSION = 1;

    // These two are executed in SpeechService
    private boolean interim_results = false;
    private boolean single_utterance = true;

    private SpeechService mSpeechService;

    private BTService mBTService;

    private VoiceRecorder mVoiceRecorder;

    private final VoiceRecorder.Callback mVoiceCallback = new VoiceRecorder.Callback() {

        @Override
        public void onVoiceStart() {
            showStatus(true);
            if (mSpeechService != null) {
                mSpeechService.startRecognizing(mVoiceRecorder.getSampleRate());
            }
        }

        @Override
        public void onVoice(byte[] data, int size) {
            if (mSpeechService != null) {
                mSpeechService.recognize(data, size);
            }
        }

        @Override
        public void onVoiceEnd() {
            showStatus(false);
            if (mSpeechService != null) {
                mSpeechService.finishRecognizing();
            }
        }

    };

    // Resource caches
    private int mColorHearing;
    private int mColorNotHearing;

    // View references
    private TextView mStatus;
    private TextView mText;
    private ResultAdapter mAdapter;
    private RecyclerView mRecyclerView;

    private final ServiceConnection mServiceConnection = new ServiceConnection() {

        @Override
        public void onServiceConnected(ComponentName componentName, IBinder binder) {
            mSpeechService = SpeechService.from(binder);
            mSpeechService.addListener(mSpeechServiceListener);
            mStatus.setVisibility(View.VISIBLE);
        }

        @Override
        public void onServiceDisconnected(ComponentName componentName) {
            mSpeechService = null;
        }

    };

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

//        BtConnect(); // This is a hack. Ideally, open to an option menu and select the BT you'd like to connect to.

        final Resources resources = getResources();
        final Resources.Theme theme = getTheme();
        mColorHearing = ResourcesCompat.getColor(resources, R.color.status_hearing, theme);
        mColorNotHearing = ResourcesCompat.getColor(resources, R.color.status_not_hearing, theme);

        setSupportActionBar((Toolbar) findViewById(R.id.toolbar));
        mStatus = findViewById(R.id.status);
        mText = findViewById(R.id.text);

        mRecyclerView = findViewById(R.id.recycler_view);
        mRecyclerView.setLayoutManager(new LinearLayoutManager(this));
        final ArrayList<String> results = savedInstanceState == null ? null :
                savedInstanceState.getStringArrayList(STATE_RESULTS);


        mAdapter = new ResultAdapter(results);
        mRecyclerView.setAdapter(mAdapter);


        // My Code
        mtoolbar = findViewById(R.id.toolbar);
        setSupportActionBar(mtoolbar);
        // Remove default title text
//        getSupportActionBar().setDisplayShowTitleEnabled(false);
    }


    @Override
    protected void onStart() {
        super.onStart();

        // Prepare Cloud Speech API
        bindService(new Intent(this, SpeechService.class), mServiceConnection, BIND_AUTO_CREATE);

        // Start listening to voices
        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.RECORD_AUDIO)
                == PackageManager.PERMISSION_GRANTED) {
            startVoiceRecorder();
        } else if (ActivityCompat.shouldShowRequestPermissionRationale(this,
                Manifest.permission.RECORD_AUDIO)) {
            showPermissionMessageDialog();
        } else {
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.RECORD_AUDIO},
                    REQUEST_RECORD_AUDIO_PERMISSION);
        }
    }

    @Override
    protected void onStop() {
        // Stop listening to voice
        stopVoiceRecorder();

        // Stop Cloud Speech API
        mSpeechService.removeListener(mSpeechServiceListener);
        unbindService(mServiceConnection);
        mSpeechService = null;

        super.onStop();
    }

    @Override
    protected void onSaveInstanceState(Bundle outState) {
        super.onSaveInstanceState(outState);
        if (mAdapter != null) {
            outState.putStringArrayList(STATE_RESULTS, mAdapter.getResults());
        }
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions,
                                           @NonNull int[] grantResults) {
        if (requestCode == REQUEST_RECORD_AUDIO_PERMISSION) {
            if (permissions.length == 1 && grantResults.length == 1
                    && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
                startVoiceRecorder();
            } else {
                showPermissionMessageDialog();
            }
        } else {
            super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        }
    }

    @Override
    public boolean onCreateOptionsMenu(Menu menu) {
        getMenuInflater().inflate(R.menu.main, menu);
        return true;
    }

    @Override
    public boolean onOptionsItemSelected(MenuItem item) {
        int id = item.getItemId();
        switch (id) {
//            case R.id.armadillo:
//                if (btSocket == null) {
//                    String[] s = {"armadillo"};
//                    new BTService().execute(s);
//
//                } else {
//                    // could put a disconnect function here
//                    Toast myToast = Toast.makeText(this, "Already connected", Toast.LENGTH_SHORT);
//                    myToast.show();
//                }
//                return true;
            case R.id.dragon:
                if (btSocket == null) {
                    String[] s = {"dragon"};
                    new BTService().execute(s);
                } else {
                    Toast myToast = Toast.makeText(this, "Already connected", Toast.LENGTH_SHORT);
                    myToast.show();
                }
                return true;
            case R.id.bee:
                if (btSocket == null) {
                    String[] s = {"bee"};
                    new BTService().execute(s);
                } else {
                    Toast myToast = Toast.makeText(this, "Already connected", Toast.LENGTH_SHORT);
                    myToast.show();
                }
                return true;
            case R.id.warning:
                btSocket = null;
                isBtConnected = false;
                Toast myToast = Toast.makeText(this, "BT Disconnected", Toast.LENGTH_SHORT);
                myToast.show();
                return true;
            default:
                return super.onOptionsItemSelected(item);

        }
    }

    private void startVoiceRecorder() {
        if (mVoiceRecorder != null) {
            mVoiceRecorder.stop();
        }
        mVoiceRecorder = new VoiceRecorder(mVoiceCallback);
        mVoiceRecorder.start();
    }

    private void stopVoiceRecorder() {
        if (mVoiceRecorder != null) {
            mVoiceRecorder.stop();
            mVoiceRecorder = null;
        }
    }

    private void showPermissionMessageDialog() {
        MessageDialogFragment
                .newInstance(getString(R.string.permission_message))
                .show(getSupportFragmentManager(), FRAGMENT_MESSAGE_DIALOG);
    }

    private void showStatus(final boolean hearingVoice) {
        runOnUiThread(new Runnable() {
            @Override
            public void run() {
                mStatus.setTextColor(hearingVoice ? mColorHearing : mColorNotHearing);
            }
        });
    }

    @Override
    public void onMessageDialogDismissed() {
        ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.RECORD_AUDIO},
                REQUEST_RECORD_AUDIO_PERMISSION);
    }

    private final SpeechService.Listener mSpeechServiceListener =
            new SpeechService.Listener() {
                @Override
                public void onSpeechRecognized(final String text, final boolean isFinal) {
                    if (isFinal) {
                        mVoiceRecorder.dismiss();
                    }
                    if (mText != null && !TextUtils.isEmpty(text)) {
                        runOnUiThread(new Runnable() {
                            @Override
                            public void run() {
                                if (isFinal) {
                                    mText.setText(null);
                                    mAdapter.addResult(text);
                                    mRecyclerView.smoothScrollToPosition(0);
                                    Log.d("speech:", text);

                                    String[] s = {text};
                                    new CheckWords().execute(s);
//
//                                    // my code
//                                    if (btSocket != null & checkWords(text) != null) { //& checkWords(text)!=""
//                                        try {
//                                            btSocket.getOutputStream().write(checkWords(text).getBytes()); //checWords(text)
//                                            Toast mToast = Toast.makeText(getApplicationContext(), "Sending to BT", Toast.LENGTH_SHORT);
//                                            mToast.show();
//                                        } catch (IOException | NullPointerException e) {
//                                            e.printStackTrace();
//                                        }
//                                    }
                                } else {
                                    mText.setText(text);
                                }
                            }
                        });
                    }
                }
            };

    private static class ViewHolder extends RecyclerView.ViewHolder {

        TextView text;

        ViewHolder(LayoutInflater inflater, ViewGroup parent) {
            super(inflater.inflate(R.layout.item_result, parent, false));
            text = itemView.findViewById(R.id.text);
        }

    }

    private static class ResultAdapter extends RecyclerView.Adapter<ViewHolder> {

        private final ArrayList<String> mResults = new ArrayList<>();

        ResultAdapter(ArrayList<String> results) {
            if (results != null) {
                mResults.addAll(results);
            }
        }

        @Override
        public ViewHolder onCreateViewHolder(ViewGroup parent, int viewType) {
            return new ViewHolder(LayoutInflater.from(parent.getContext()), parent);
        }

        @Override
        public void onBindViewHolder(ViewHolder holder, int position) {
            holder.text.setText(mResults.get(position));
        }

        @Override
        public int getItemCount() {
            return mResults.size();
        }

        void addResult(String result) {
            mResults.add(0, result);
            notifyItemInserted(0);
        }

        public ArrayList<String> getResults() {
            return mResults;
        }

    }

    private class CheckWords extends AsyncTask<String, Void, String> {
        @Override
        protected String doInBackground(String... s) {
            String text = s[0].toLowerCase();
            if (text.contains("stop") | text.contains("halt") | text.contains("off") | text.contains("terminate")) {
                SendtoBT("stop");
            }
            if (text.contains("forward") | text.contains(" go")) {
                SendtoBT("forward");
            }
            if (text.contains("shoot") | text.contains("pew") | text.contains("fire")) {
                SendtoBT("shoot");
            }
            if (text.contains("back") | text.contains("retreat") | text.contains("beep")) {
                SendtoBT("back");
            }
            if (text.contains("spin")) {
                SendtoBT("spin");
            }
            if (text.contains("dance") | text.contains("macarena")) {
                SendtoBT("dance");
            }
            if (text.contains("run") | text.contains("boo") | text.contains("surprise") | text.contains("fuck you")) {
                SendtoBT("run");
            }
            if (text.contains("ram") | text.contains("fast")) {
                SendtoBT("ram");
            }
            if (text.contains("left") | (text.contains("lef"))) {
                if (text.contains("45") | text.contains("30") | text.contains("forty")) {
                    SendtoBT("left 45");
                }
                if (text.contains("90") | text.contains("ninety")) {
                    SendtoBT("left 90");
                }
                if (text.contains("180")) {
                    SendtoBT("left 180");
                }
                SendtoBT("left");
            }
            if (text.contains("right") | text.contains("write") | text.contains("rite")) {
                if (text.contains("45") | text.contains("30") | text.contains("forty")) {
                    SendtoBT("right 45");
                }
                if (text.contains("90") | text.contains("ninety")) {
                    SendtoBT("right 90");
                }
                if (text.contains("180")) {
                    SendtoBT("right 180");
                }
                SendtoBT("right");
            }
            return null;
        }

        private void SendtoBT(String text){
            if (btSocket != null) {
                try {
                    btSocket.getOutputStream().write(text.getBytes());
                    // Tell user it is sending
                    runOnUiThread(new Runnable() {
                        public void run() {
                            Toast mToast = Toast.makeText(getApplicationContext(), "Sent to BT!", Toast.LENGTH_SHORT);
                            mToast.show();
                        }
                    });
                    // Tell debugger it's working
                    Log.d("BT", "BT message is sending!");
                } catch (IOException | NullPointerException e) {
                    e.printStackTrace();
                }
            }
        }
    }



    private class BTService extends AsyncTask<String, Integer, Void> {

        @Override
        protected Void doInBackground(String... s) {
            if (s[0].equals("bee")) {
                BeeConnect();
            }
            if (s[0].equals("dragon")) {
                DragonConnect();
            }
//            if (s[0].equals("armadillo")) {
//                ArmaConnect();
//            }
            if (s[0].equals("hedgehog")) {
                HedgehogConnect();
            }
            return null;
        }


//      Tried using RegEx
//      if (text.toLowerCase().matches("\\d+")) {
//            Log.d("Regex", "Identified");
//            if (text.toLowerCase().contains("left")) {
//                Log.d("Left", "lefting");
//                Pattern p = Pattern.compile("\\d+");   // the pattern to search for \d+or\d+
//                Matcher m = p.matcher(text);
//                String num = m.group(1);
//                Log.d("Regex", num + "hi");
//
//                // Convert m to angle time number
//                return "left" + m;
//            }
//        }


        // Connect to the bee robot
        public void BeeConnect() //while the progress dialog is shown, the connection is done in background
        {
            try {
                if (btSocket == null || !isBtConnected) {
                    // Display that it's starting to connect to user
                    runOnUiThread(new Runnable() {
                        public void run() {
                            Toast mToast = Toast.makeText(getApplicationContext(), "Connecting...", Toast.LENGTH_SHORT);
                            mToast.show();
                        }
                    });

                    // Get the mobile bluetooth device
                    mBtAdapter = BluetoothAdapter.getDefaultAdapter();
                    // Connects to the device's address and checks if it's available
                    BluetoothDevice dispositivo = mBtAdapter.getRemoteDevice(bee_address);
                    // Create a RFCOMM (SPP) connection
                    btSocket = dispositivo.createRfcommSocketToServiceRecord(myUUID);
                    BluetoothAdapter.getDefaultAdapter().cancelDiscovery();
                    // Start connection
                    btSocket.connect();

                    // Tell user it connected
                    runOnUiThread(new Runnable() {
                        public void run() {
                            Toast mToast = Toast.makeText(getApplicationContext(), "Connected to Bee!", Toast.LENGTH_SHORT);
                            mToast.show();
                        }
                    });

                    // Tell debugger it's working
                    Log.d("BT", "BT is connected!");
                }
            } catch (IOException e) {
                ConnectSuccess = false;//if the try failed, you can check the exception here
            }
        }

        public void DragonConnect() //while the progress dialog is shown, the connection is done in background
        {
            try {
                if (btSocket == null || !isBtConnected) {
                    runOnUiThread(new Runnable() {
                        public void run() {
                            Toast mToast = Toast.makeText(getApplicationContext(), "Connecting...", Toast.LENGTH_SHORT);
                            mToast.show();
                        }
                    });

                    mBtAdapter = BluetoothAdapter.getDefaultAdapter();
                    BluetoothDevice dispositivo = mBtAdapter.getRemoteDevice(drago_address);
                    btSocket = dispositivo.createRfcommSocketToServiceRecord(myUUID);
                    BluetoothAdapter.getDefaultAdapter().cancelDiscovery();
                    btSocket.connect();

                    runOnUiThread(new Runnable() {
                        public void run() {
                            Toast mToast = Toast.makeText(getApplicationContext(), "Connected to Dragon!", Toast.LENGTH_SHORT);
                            mToast.show();
                        }
                    });
                    Log.d("BT", "BT is connected!");
                }
            } catch (IOException e) {
                ConnectSuccess = false;//if the try failed, you can check the exception here
            }
        }


        public void HedgehogConnect() //while the progress dialog is shown, the connection is done in background
        {
            try {
                if (btSocket == null || !isBtConnected) {
                    runOnUiThread(new Runnable() {
                        public void run() {
                            Toast mToast = Toast.makeText(getApplicationContext(), "Connecting...", Toast.LENGTH_SHORT);
                            mToast.show();
                        }
                    });

                    mBtAdapter = BluetoothAdapter.getDefaultAdapter();
                    BluetoothDevice dispositivo = mBtAdapter.getRemoteDevice(heg_address);
                    btSocket = dispositivo.createRfcommSocketToServiceRecord(myUUID);
                    BluetoothAdapter.getDefaultAdapter().cancelDiscovery();
                    btSocket.connect();

                    runOnUiThread(new Runnable() {
                        public void run() {
                            Toast mToast = Toast.makeText(getApplicationContext(), "Connected to Hedgehog!", Toast.LENGTH_SHORT);
                            mToast.show();
                        }
                    });
                    Log.d("BT", "BT is connected!");
                }
            } catch (IOException e) {
                ConnectSuccess = false;//if the try failed, you can check the exception here
            }
        }


//        public void ArmaConnect() //while the progress dialog is shown, the connection is done in background
//        {
//            try {
//                if (btSocket == null || !isBtConnected) {
//                    runOnUiThread(new Runnable() {
//                        public void run() {
//                            Toast mToast = Toast.makeText(getApplicationContext(), "Connecting...", Toast.LENGTH_SHORT);
//                            mToast.show();
//                        }
//                    });
//
//                    mBtAdapter = BluetoothAdapter.getDefaultAdapter();
//                    BluetoothDevice dispositivo = mBtAdapter.getRemoteDevice(arma_address);
//                    btSocket = dispositivo.createRfcommSocketToServiceRecord(myUUID);
//                    BluetoothAdapter.getDefaultAdapter().cancelDiscovery();
//                    btSocket.connect();
//
//                    runOnUiThread(new Runnable() {
//                        public void run() {
//                            Toast mToast = Toast.makeText(getApplicationContext(), "Connected to Bee!", Toast.LENGTH_SHORT);
//                            mToast.show();
//                        }
//                    });
//                    Log.d("BT", "BT is connected!");
//                }
//            } catch (IOException e) {
//                ConnectSuccess = false;//if the try failed, you can check the exception here
//            }
//        }

    }
}