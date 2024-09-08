#include <Math.h>
#include <ESP8266WiFi.h>
#include <ESP8266WebServer.h>

// Wi-Fi network credentials (replace with your own)
const char* ssid = "MyWiFiNetwork";
const char* password = "MyWiFiPassword";

// Web server on port 80
ESP8266WebServer server(80);

double getTemp() {
  // Inputs ADC Value from Thermistor and outputs Temperature in Celsius
  int RawADC = analogRead(A0);

  long Resistance;
  double Temp;

  // Assuming a 10k Thermistor.  Calculation is actually: Resistance = (1024/ADC)
    Resistance=((10240000/RawADC) - 10000);

  /******************************************************************/
  /* Utilizes the Steinhart-Hart Thermistor Equation:				*/
  /*    Temperature in Kelvin = 1 / {A + B[ln(R)] + C[ln(R)]^3}		*/
  /*    where A = 0.001129148, B = 0.000234125 and C = 8.76741E-08	*/
  /******************************************************************/
  Temp = log(Resistance);
  Temp = 1 / (0.001129148 + (0.000234125 * Temp) + (0.0000000876741 * Temp * Temp * Temp));
  Temp = Temp - 273.15;  // Convert Kelvin to Celsius

  return Temp;  // Return the Temperature
}

void handleRoot() {
  String html = prepareDynamicHTML();
  server.send(200, "text/html", html);
}

void handleTemperature() {
  double temperature = getTemp();
  String json = "{\"temperature\":" + String(temperature) + "}";
  server.send(200, "application/json", json);
}

String prepareDynamicHTML() {
  String html = R"rawliteral(
    <!DOCTYPE html>
<html lang="pt-br">

<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
    <link rel="stylesheet" href="https://cdnjs.cloudflare.com/ajax/libs/font-awesome/6.0.0/css/all.min.css"
        integrity="sha512-..." crossorigin="anonymous">
    <style>
        
        @import url('https://fonts.googleapis.com/css2?family=SUSE:wght@100..800&display=swap');

        /* Global styles */
        * {
            margin: 0;
            padding: 0;
            box-sizing: border-box;
            font-family: "SUSE", sans-serif;
        }

        body {
            width: 100vw;
            min-height: 100vh;
            background-color: #f5f5f5;
            /* Light gray background */
            display: flex;
            flex-direction: column;
            align-items: center;
            font-size: 16px;
            /* Base font size */
        }

        /* Header */
        header {
            background-color: #2196F3;
            /* Blue header for emphasis */
            color: #fff;
            /* White text for readability */
            padding: 48px 0;
            width: 100vw;
            display: flex;
            justify-content: center;
            /* Center title horizontally */
        }

        h1 {
            margin: 0;
            font-size: 1.8rem;
            /* Adjust title size */
        }

        /* Main content */
        main {
            height: calc(100vh - 288px);
            /* Main content takes up remaining space */
            display: flex;
            justify-content: center;
            /* Center container horizontally */
            align-items: center;

        }

        .container {
            border: 1px solid #ddd;
            /* Light gray border */
            border-radius: 8px;
            background-color: #fff;
            /* White background for content */
            padding: 24px;
            /* Add padding for spacing */
            box-shadow: 0 2px 4px rgba(0, 0, 0, 0.1);
            /* Subtle shadow for depth */
            display: flex;
            flex-direction: column;
            align-items: center;
            gap: 16px;
            width: 50vw;
            /* Adjust container width */
            max-width: 400px;
            /* Set a maximum width for responsiveness */
            min-height: 160px;
            max-height: 240px;
        }

        /* Icon and Temperature */
        h2 {
            font-size: 1.5rem;
            margin-top: 0;
            /* Remove top margin */
        }

        .fa-temperature-high {
            color: #f00;
            /* Orange icon for warmth */
            font-size: 3rem;
            /* Increase icon size for prominence */
        }

        #temp {
            font-size: 2rem;
            /* Larger font size for temperature */
            font-weight: bold;
        }

        /* Footer */
        footer {
            background-color: #333;
            /* Dark gray footer */
            color: #ccc;
            /* Light gray text */
            padding: 48px 0;
            width: 100vw;
            display: flex;
            justify-content: center;
            align-items: center;
            position: absolute;
            bottom: 0;
        }

        /* Estilos para telas menores (celular) */
        @media (max-width: 767px) {
            html {
                font-size: 50%;
            }

            .container {
                width: 90vw;
            }

            h2 {
                text-align: center;
            }
        }

        /* Estilos para tablets */
        @media (min-width: 768px) and (max-width: 1023px) {
            html {
                font-size: 55%;
            }

            .container {
                width: 70vw;
                height: 375px;
            }
        }

        /* Estilos para desktops */
        @media (min-width: 1024px) and (max-width: 1923px) {
            html {
                font-size: 62.5%;
            }

            header{
                display: flex;
                justify-content: center;
                align-items: center;
                height: 120px;
            }

            h1{
                font-size: 2rem;
            }

            main{
                height: calc(100vh - 248px);
            }

            .container{
                min-width: 40vw;
                max-width: 630px;
                height: 375px;
            }

            footer{
                height: 120px;
            }
        }

        /* Estilos para telas HD e Full HD */
        @media (min-width: 1924px) {
            html {
                font-size: 75%;
            }

            header, footer{
                display: flex;
                flex-direction: column;
                align-items: center;
                height: 200px;
            }

            footer{
                font-size: 1.7rem;
            }

            h1{
                font-size: 2rem;
            }

            h2{
                font-size: 1.5rem;
            }

            .container{
                min-height: 320px;
                max-height: 620px;
                max-width: 840px;
            }
    </style>
    <title>App de Temperatura - ESP8266</title>
</head>

<body>
    <header>
        <h1>App de Temperatura - ESP8266</h1>
    </header>
    <main>
        <section class="container">
            <h2><i class="fa-solid fa-temperature-high"></i> Temperatura Atual</h2>
            <p id="temp">22ºC</p>
        </section>
    </main>
    <footer>
        <p>Todos os direitos reservados - Copyright 2024</p>
    </footer>
    <script>
        function updateTemperature() {
            fetch('/temperature')
                .then(response => response.json())
                .then(data => {
                    document.getElementById('temp').textContent = data.temperature.toFixed(1) + 'ºC';
                })
                .catch(error => console.error('Error fetching temperature:', error));
        }

        // Update temperature every second
        setInterval(updateTemperature, 1000);

        // Initial update
        updateTemperature();
    </script>
</body>

</html>
  )rawliteral";
  return html;
}

void setup() {
  // Initialize Wi-Fi connection
  WiFi.begin(ssid, password);
  Serial.begin(115200);
  Serial.println();

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  Serial.println("");
  Serial.println("Conectado ao Wi-Fi");
  Serial.print("IP Address: ");
  Serial.println(WiFi.localIP());

  // Define server routes
  server.on("/", HTTP_GET, handleRoot);
  server.on("/temperature", HTTP_GET, handleTemperature);

  // Start the server
  server.begin();
}

void loop() {
  server.handleClient();
  delay(1000);  // Update temperature reading every second
}
