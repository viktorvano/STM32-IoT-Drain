# STM32-IoT-Drain
 IoT Smart Drain that uses STM32, ESP8266 and a servo.  
 [YouTube Video](https://www.youtube.com/watch?v=bjgvxr_YlN0)  
  
  
### Wiring Diagram  
  
![alt text](https://github.com/viktorvano/STM32-IoT-Drain/blob/main/Documents/schematic.png?raw=true)  
  
  
## Code Snippets
  
[STM32 C code] You have to change your WiFi SSID credentials and servo parameters:  
```C
#define WiFi_Credentials	"AT+CWJAP=\"WiFiSSID\",\"WiFiPASSWORD\"\r\n"
#define PWM_OPEN			0x700
#define PWM_CLOSED			0x650
```    
  
[Android Studio - Java]
```Java
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
```  
  
  
### Photos  
  
![alt text](https://github.com/viktorvano/STM32-IoT-Drain/blob/main/Documents/web_open.png?raw=true)  
  
![alt text](https://github.com/viktorvano/STM32-IoT-Drain/blob/main/Documents/web_closed.png?raw=true)  
  
![alt text](https://github.com/viktorvano/STM32-IoT-Drain/blob/main/Documents/setup.jpg?raw=true)  
  
![alt text](https://github.com/viktorvano/STM32-IoT-Drain/blob/main/Documents/detail.jpg?raw=true)  
  
![alt text](https://github.com/viktorvano/STM32-IoT-Drain/blob/main/Documents/App.jpg?raw=true)  
  
