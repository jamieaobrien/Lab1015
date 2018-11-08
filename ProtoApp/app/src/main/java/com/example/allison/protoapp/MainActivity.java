package com.example.allison.protoapp;

import android.bluetooth.BluetoothGattService;
import android.support.v7.app.AppCompatActivity;
import android.os.Bundle;
import java.util.ArrayList;
import java.util.Locale;

import android.content.ActivityNotFoundException;
import android.bluetooth.BluetoothAdapter;
import android.bluetooth.BluetoothDevice;
import android.content.Intent;
import android.speech.RecognizerIntent;
import android.view.View;
import android.widget.TextView;
import android.widget.Button;
import android.widget.Toast;

//import static com.example.allison.protoapp.R.id.txtSpeechInput;

public class MainActivity extends AppCompatActivity {
    private TextView txtSpeechInput;
    private Button btnSpeak;
    private final int REQ_CODE_SPEECH_INPUT = 100;

    public static final int MESSAGE_DEVICE_NAME = 4;
    public static final int MESSAGE_TOAST = 5;
    public static final int MESSAGE_STATE_CHANGE = 1;
    public static final String DEVICE_NAME = "device_name";
    public static final String TOAST = "toast";

    // Intent request codes
    private static final int REQUEST_CONNECT_DEVICE = 1;
    private static final int REQUEST_ENABLE_BT = 2;

    // Toolbar
    private Button mToolbarConnectButton;
    private Button mToolbarDisconnectButton;
    private Button mToolbarPauseButton = null, mToolbarPlayButton;

    // Name of the connected device
    private String mConnectedDeviceName = null;
    private BluetoothAdapter mBluetoothAdapter = null;
    // Member object for the Bluetooth services
    private BluetoothChatService mBluetoothService = null;

    // State variables
    private boolean paused = false;
    private boolean connected = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Bluetooth stuff
        mToolbarConnectButton =  findViewById(R.id.toolbar_btn_connect);
        mToolbarConnectButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                startDeviceListActivity();
            }
        });

        mToolbarDisconnectButton =  findViewById(R.id.toolbar_btn_disconnect);
        mToolbarDisconnectButton.setOnClickListener(new View.OnClickListener() {
            public void onClick(View v) {
                disconnectDevices();
            }
        });

        // Not sure if we need this
        // Get local Bluetooth adapter

        mBluetoothAdapter = BluetoothAdapter.getDefaultAdapter();
        // Android Speech

        txtSpeechInput = findViewById(R.id.txtSpeechInput);
        btnSpeak = findViewById(R.id.btnSpeak);

        btnSpeak.setOnClickListener(new View.OnClickListener() {

            @Override
            public void onClick(View view) {
                promptSpeechInput();
            }
        });
    }

    @Override
    public void onStart() {
        super.onStart();
        // If BT is not on, request that it be enabled.
        // setupUserInterface() will then be called during onActivityResult
        if (!mBluetoothAdapter.isEnabled()) {
            Intent enableIntent = new Intent(BluetoothAdapter.ACTION_REQUEST_ENABLE);
            startActivityForResult(enableIntent, REQUEST_ENABLE_BT);
            // Otherwise, setup the Bluetooth session
        } else {
            if (mBluetoothService == null) setupUserInterface();
        }
    }

    private void startDeviceListActivity() {
        Intent serverIntent = new Intent(this, DeviceListActivity.class);
        startActivityForResult(serverIntent, REQUEST_CONNECT_DEVICE);
    }

    /**
     * Showing google speech input dialog
     * */
    private void promptSpeechInput() {
        /* Takes user's speech input and returns it to same activity*/
        Intent intent = new Intent(RecognizerIntent.ACTION_RECOGNIZE_SPEECH);
        /* Considers input in free form English*/
        intent.putExtra(RecognizerIntent.EXTRA_LANGUAGE_MODEL,
                RecognizerIntent.LANGUAGE_MODEL_FREE_FORM);
        /* Extra prompt is a text prompt to show user when asking them to speak*/
        intent.putExtra(RecognizerIntent.EXTRA_LANGUAGE, Locale.getDefault());
        intent.putExtra(RecognizerIntent.EXTRA_PROMPT,
                getString(R.string.speech_prompt));
        try {
            startActivityForResult(intent, REQ_CODE_SPEECH_INPUT);
        } catch (ActivityNotFoundException a) {
            Toast.makeText(getApplicationContext(),
                    getString(R.string.speech_not_supported),
                    Toast.LENGTH_SHORT).show();
        }
    }

    /**
     * Receiving speech input
     * */
    @Override
    protected void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);

        switch (requestCode) {
            case REQ_CODE_SPEECH_INPUT: {
                if (resultCode == RESULT_OK && null != data) {

                    ArrayList<String> result = data
                            .getStringArrayListExtra(RecognizerIntent.EXTRA_RESULTS);
                    txtSpeechInput.setText(result.get(0));
                }
                break;
            }
            case REQUEST_CONNECT_DEVICE:
                // When DeviceListActivity returns with a device to connect
                if (resultCode == android.app.Activity.RESULT_OK) {
                    // Get the device MAC address
                    String address = data.getExtras().getString(DeviceListActivity.EXTRA_DEVICE_ADDRESS);
                    // Get the BluetoothDevice object
                    BluetoothDevice device = mBluetoothAdapter.getRemoteDevice(address);
                    // Attempt to connect to the device
                    mBluetoothService.connect(device);
                }
                break;

            case REQUEST_ENABLE_BT:
                // When the request to enable Bluetooth returns
                if (resultCode != android.app.Activity.RESULT_OK) {
                    // User did not enable Bluetooth or an error occurred
                    Toast.makeText(this, "BT NOT ON", Toast.LENGTH_SHORT).show();
                }
                setupUserInterface();
        }
    }

    private void setupUserInterface()
    {
        // Initialize the BluetoothService to perform Bluetooth connections
        mBluetoothService = new BluetoothChatService();

    }

    @Override
    public void onDestroy()
    {
        super.onDestroy();

        if (mBluetoothService != null)
        {
            mBluetoothService.stop();
        }
    }

    private void disconnectDevices()
    {
        if (mBluetoothService != null)
        {
            mBluetoothService.stop();
        }
    }
//    @Override
//    public boolean onCreateOptionsMenu(Menu menu) {
//        // Inflate the menu; this adds items to the action bar if it is present.
//        getMenuInflater().inflate(R.menu.main, menu);
//        return true;
//    }
}
