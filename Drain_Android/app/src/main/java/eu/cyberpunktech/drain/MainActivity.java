package eu.cyberpunktech.drain;

import androidx.appcompat.app.AppCompatActivity;

import android.content.pm.ActivityInfo;
import android.os.Bundle;
import android.view.View;
import android.widget.Button;

public class MainActivity extends AppCompatActivity {

    private static String IP = "192.168.1.42";
    private static int port = 80;

    private Button buttonOPEN, buttonCLOSE;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_PORTRAIT);

        buttonOPEN = findViewById(R.id.buttonOPEN);
        buttonOPEN.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Client client = new Client(IP, port, "OPEN DRAIN");
                client.start();
            }
        });

        buttonCLOSE = findViewById(R.id.buttonCLOSE);
        buttonCLOSE.setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                Client client = new Client(IP, port, "CLOSE DRAIN");
                client.start();
            }
        });
    }
}