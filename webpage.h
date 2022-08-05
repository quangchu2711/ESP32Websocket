//=====================
//HTML code for webpage
//=====================
const char web[] PROGMEM = 
R"=====(
<!DOCTYPE html>
<html>
<head>
  <title>Arduino and ESP32 Websocket</title>
  <meta name='viewport' content='width=device-width, initial-scale=1.0' />
  <meta charset='UTF-8'>
  <style>
    body {
      background-image: url('https://img4.thuthuatphanmem.vn/uploads/2019/12/17/hinh-nen-powerpoint-chu-de-dien-tu_103812785.jpg');
      background-repeat: no-repeat;
      background-attachment: fixed;
      background-size: 100% 100%;      
      text-align: center;
    }
    div{
        margin-left: auto;
        margin-right: auto;
    }
    h1 {
      color: black;
    }
    .myDiv {
      border: 5px outset black;
      background-color: #FFFBD1;
      text-align: center;
    }
  </style>
</head>
<!--------------------------HTML-------------------------->
<body>
  <div class="myDiv">
    <h1>Phạm Thành Đạt và Đoàn Việt Dũng</h1>  
  </div>
  <div class="myDiv">
    <h1>TEMPERATURE: <span id='temp'>-</span> &#x2103</h1>
    <h1>HUMIDITY: <span id='hum'>-</span> %</h1>
    <h1>LIGHT: <span id='light'>-</span> </h1>
    <h1>POT-HG: <span id='pot'>-</span> </h1>    
  </div>
  <div class="myDiv">
    <h1>RECEIVED MESSAGE: <span id='message'>-</span></h1><button type='button' id='BTN_1'>
      <h1>ON</h1>
    </button><button type='button' id='BTN_2'>
      <h1>OFF</h1>
    </button>
  </div>
</body>
<!----------------------JavaScript------------------------>
<script>
  var Socket;
  document.getElementById('BTN_1').addEventListener('click', button_1_pressed);
  document.getElementById('BTN_2').addEventListener('click', button_2_pressed);
  function init() {
    Socket = new WebSocket('ws://' + window.location.hostname + ':81/');
    Socket.onmessage = function(event) {
      processCommand(event);
    };
  }
  function processCommand(event) {
    var obj = JSON.parse(event.data);
    document.getElementById('message').innerHTML = obj.PIN_Status;
    document.getElementById('temp').innerHTML = obj.Temp;
    document.getElementById('hum').innerHTML = obj.Hum;
    document.getElementById('light').innerHTML = obj.Light;
    document.getElementById('pot').innerHTML = obj.PotHG;    
    console.log(obj.PIN_Status);
    console.log(obj.Temp);
    console.log(obj.Hum);
  }
  function button_1_pressed() {
    Socket.send('1');
  }
  function button_2_pressed() {
    Socket.send('0');
  }
  window.onload = function(event) {
    init();
  }
</script>
</html>
)=====";
