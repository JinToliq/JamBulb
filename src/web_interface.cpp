#include "web_interface.h"
#include <WiFi.h>
#include <AsyncTCP.h>
#include <ESPAsyncWebServer.h>
#include <ArduinoJson.h>

const char index_html[] PROGMEM = R"rawliteral(
<!DOCTYPE HTML><html>
<head>
  <title>JamBulb Control</title>
  <meta name="viewport" content="width=device-width, initial-scale=1">
  <style>
    body { font-family: Arial, sans-serif; margin: 0; padding: 20px; background: #222; color: #eee; }
    .container { max-width: 600px; margin: 0 auto; }
    h1 { text-align: center; }
    .card { background: #333; padding: 20px; border-radius: 8px; margin-bottom: 20px; }
    .form-group { margin-bottom: 15px; }
    label { display: block; margin-bottom: 5px; }
    input[type=range] { width: 100%; }
    input[type=text], input[type=password] { width: 100%; padding: 8px; box-sizing: border-box; }
    button { width: 100%; padding: 10px; background: #007bff; color: white; border: none; border-radius: 4px; cursor: pointer; font-size: 16px; }
    button:hover { background: #0056b3; }
    .btn-secondary { background: #6c757d; }
    .btn-secondary:hover { background: #545b62; }
  </style>
</head>
<body>
  <div class="container">
    <h1>JamBulb</h1>
    
    <div class="card">
      <h2>Controls</h2>
      <div class="form-group">
        <label>Effect</label>
        <button onclick="nextEffect()">Next Effect</button>
      </div>
      <div class="form-group">
        <label>Brightness: <span id="brightnessVal"></span></label>
        <input type="range" min="8" max="210" id="brightness" onchange="setBrightness(this.value)">
      </div>
      <div class="form-group">
        <label>Speed: <span id="speedVal"></span></label>
        <input type="range" min="0.1" max="8.0" step="0.1" id="speed" onchange="setSpeed(this.value)">
      </div>
    </div>

    <div class="card">
      <h2>WiFi Settings</h2>
      <div class="form-group">
        <label>New Password</label>
        <input type="password" id="wifiPass">
      </div>
      <button class="btn-secondary" onclick="updateWifi()">Update Password</button>
    </div>
  </div>

<script>
function updateStatus() {
  fetch('/api/status').then(res => res.json()).then(data => {
    document.getElementById('brightness').value = data.brightness;
    document.getElementById('brightnessVal').innerText = data.brightness;
    document.getElementById('speed').value = data.speed;
    document.getElementById('speedVal').innerText = data.speed.toFixed(1);
  });
}

function nextEffect() {
  fetch('/api/control', {
    method: 'POST',
    headers: {'Content-Type': 'application/json'},
    body: JSON.stringify({action: 'next_effect'})
  });
}

function setBrightness(val) {
  document.getElementById('brightnessVal').innerText = val;
  fetch('/api/control', {
    method: 'POST',
    headers: {'Content-Type': 'application/json'},
    body: JSON.stringify({brightness: parseInt(val)})
  });
}

function setSpeed(val) {
  document.getElementById('speedVal').innerText = parseFloat(val).toFixed(1);
  fetch('/api/control', {
    method: 'POST',
    headers: {'Content-Type': 'application/json'},
    body: JSON.stringify({speed: parseFloat(val)})
  });
}

function updateWifi() {
  const pass = document.getElementById('wifiPass').value;
  if(!pass) return alert('Enter password');
  fetch('/api/wifi', {
    method: 'POST',
    headers: {'Content-Type': 'application/json'},
    body: JSON.stringify({password: pass})
  }).then(res => {
    if(res.ok) alert('Password updated. Device will restart.');
    else alert('Error updating password');
  });
}

// Initial load
updateStatus();
// Poll status every 2 seconds to sync with physical controls
setInterval(updateStatus, 2000);
</script>
</body>
</html>
)rawliteral";

WebInterface::WebInterface(Input& input, State& state) 
    : _input(input), _state(state), _server(80) {}

void WebInterface::begin() {
    WiFi.mode(WIFI_AP);
    WiFi.softAP(WIFI_SSID, "123456789"); // _state.wifiPassword.c_str());
    
    setupRoutes();
    _server.begin();
}

void WebInterface::setupRoutes() {
    _server.on("/", HTTP_GET, [](AsyncWebServerRequest *request){
        request->send_P(200, "text/html", index_html);
    });

    _server.on("/api/control", HTTP_POST, [this](AsyncWebServerRequest *request){
        request->send(200, "application/json", "{\"status\":\"ok\"}");
    }, NULL, [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
        StaticJsonDocument<200> doc;
        DeserializationError error = deserializeJson(doc, data, len);
        if (error) {
          return;
        }

        if (doc.containsKey("action") && doc["action"] == "next_effect") {
            _input.setNextEffectRequest();
        }

        if (doc.containsKey("brightness")) {
            _input.setBrightness(doc["brightness"]);
        }

        if (doc.containsKey("speed")) {
            _input.setSpeed(doc["speed"]);
        }
    });

    _server.on("/api/wifi", HTTP_POST, [this](AsyncWebServerRequest *request){
        request->send(200, "application/json", "{\"status\":\"restarting\"}");
    }, NULL, [this](AsyncWebServerRequest *request, uint8_t *data, size_t len, size_t index, size_t total){
        StaticJsonDocument<200> doc;
        DeserializationError error = deserializeJson(doc, data, len);
        if (error) return;

        if (doc.containsKey("password")) {
            String newPass = doc["password"];
            if (newPass.length() >= 8) {
                _state.wifiPassword = newPass;
                _state.save();
                
                delay(1000);
                ESP.restart();
            }
        }
    });
}
