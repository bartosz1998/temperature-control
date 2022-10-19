#include <string.h>

char onRegister[] = "<!DOCTYPE html>\
<html>\
<body>\
<style>\
body{\
	background: linear-gradient(120deg,#2980b9, #8e44ad);\
}\
.container{\
  position: absolute;\
  top: 50%;\
  left: 50%;\
  margin-right: -50%;\
  transform: translate(-50%, -50%) ;\
  width: 31%;\
  height: 65%;\
  background: GhostWhite;\
  border-radius:6px;\
}\
input[type=submit]{\
  position: absolute;\
  background-color: DarkSlateBlue;\
  border: none;\
  color: white;\
  padding: 6% 34%;\
  top: 85%;\
  left: 50%;\
  margin-right: -50%;\
  transform: translate(-50%, -50%);\
  border-radius: 10px;\
  font-size:20px;\
 }\
input[type=submit]:hover{\
  background-color: #5a5096;\
}\
h2 {\
  font-family: 'Georgia', serif;\
  font-size: 25px;\
  text-align: center;\
  position: absolute;\
  top: 4%;\
  left: 50%;\
  margin-right: -50%;\
  transform: translate(-50%, -50%);\
}\
input[type=text1]\
{\
  position: absolute;\
  height:20%;\
  width:72%;\
  top: 30%;\
  left: 50%;\
  padding:9px;\
  margin-right: -50%;\
  transform: translate(-50%, -50%);\
  box-sizing: border-box;\
  border: none;\
  border-bottom: 2px solid #555;\
  font-size: 18px;\
}\
input[type=text2]\
{\
  position: absolute;\
  height:20%;\
  width:72%;\
  top: 55%;\
  left: 50%;\
  margin-right: -50%;\
  transform: translate(-50%, -50%);\
  box-sizing: border-box;\
  border: none;\
  border-bottom: 2px solid #555;\
  padding:9px;\
  font-size: 18px;\
}\
</style>\
<div class=\"container\">\
<h2>WELCOME</h2>\
<form method=\"post\" action=\"/postLogin\">\
  <input type=\"text1\" id=\"fname\" name=\"fname\" placeholder=\"Username\"><br>\
  <input type=\"text2\" id=\"lname\" name=\"lname\" placeholder=\"Password\"><br><br>\
  <input type=\"submit\" value=\"Login\" name=\"submit\">\
</form>\
</div>\
</body>\
</html>";

char onRegisterError[] = "<!DOCTYPE html>\
<html>\
<body>\
<style>\
body{\
	background: linear-gradient(120deg,#2980b9, #8e44ad);\
}\
.container{\
  position: absolute;\
  top: 50%;\
  left: 50%;\
  margin-right: -50%;\
  transform: translate(-50%, -50%) ;\
  width: 31%;\
  height: 65%;\
  background: GhostWhite;\
  border-radius:6px;\
}\
input[type=submit]{\
  position: absolute;\
  background-color: DarkSlateBlue;\
  border: none;\
  color: white;\
  padding: 6% 34%;\
  top: 85%;\
  left: 50%;\
  margin-right: -50%;\
  transform: translate(-50%, -50%);\
  border-radius: 10px;\
  font-size:20px;\
 }\
input[type=submit]:hover{\
  background-color: #5a5096;\
}\
h2 {\
  font-family: 'Georgia', serif;\
  font-size: 25px;\
  text-align: center;\
  position: absolute;\
  top: 4%;\
  left: 50%;\
  margin-right: -50%;\
  transform: translate(-50%, -50%);\
}\
h3 {\
  font-family: 'Georgia', serif;\
  font-size: 10px;\
  text-align: center;\
  position: absolute;\
  top: 67%;\
  left: 50%;\
  margin-right: -50%;\
  transform: translate(-50%, -50%);\
  color:red;\
}\
input[type=text1]\
{\
  position: absolute;\
  height:20%;\
  width:72%;\
  top: 30%;\
  left: 50%;\
  padding:9px;\
  margin-right: -50%;\
  transform: translate(-50%, -50%);\
  box-sizing: border-box;\
  border: none;\
  border-bottom: 2px solid #555;\
  font-size: 18px;\
}\
input[type=text2]\
{\
  position: absolute;\
  height:20%;\
  width:72%;\
  top: 55%;\
  left: 50%;\
  margin-right: -50%;\
  transform: translate(-50%, -50%);\
  box-sizing: border-box;\
  border: none;\
  border-bottom: 2px solid #555;\
  padding:9px;\
  font-size: 18px;\
}\
</style>\
<div class=\"container\">\
<h2>WELCOME</h2>\
<form method=\"post\" action=\"/postLogin\">\
  <input type=\"text1\" id=\"fname\" name=\"fname\" placeholder=\"Username\"><br>\
  <input type=\"text2\" id=\"lname\" name=\"lname\" placeholder=\"Password\"><br><br>\
  <h3>Wrong name or password !!!</h3>\
  <input type=\"submit\" value=\"Login\" name=\"submit\">\
</form>\
</div>\
</body>\
</html>";

char onPanel[] = "\
<html>\
<head>\
<style>\
body {\
	background-color: #000d23;\
    margin:0px;\
}\
\
.panel {\
	height:10%;\
    wight:100%;\
    background-color: #1e274e;\
}\
\
.panel h3{\
\
    margin: 0;\
	color:white;\
    position: absolute;\
    top: 4.8%;\
    left: 50%;\
    margin-right: -50%;\
    transform: translate(-50%, -50%)\
}\
\
.flex-container{\
	width: 100%;\
    height:auto;\
    display:flex;\
    flex-direction: row;\
    flex-wrap: wrap;\
    justify-content: space-around;\
    margin-top:5px;\
}\
.flex-box{\
	width: 30%;\
    color: white;\
    text-align: center;\
    margin: 9px;\
    display:block;\
}\
\
.flex1{\
	color:white;\
    background: #1e274e;\
    line-height:40px;\
    font-size:20px;\
    border-radius:15px 15px 0px 0px;\
    font-weight:800;\
    border-bottom: 2.5px solid red;\
}\
\
.flex2{\
	color:white;\
    line-height:150px;\
    background: #1e274e;\
    font-size:45px;\
    border-radius:0px 0px 15px 15px;\
}\
\
.flex2:hover{\
	color:white;\
    line-height:150px;\
    background: #343c5f;\
    font-size:46px;\
    border-radius:0px 0px 15px 15px;\
}\
\
.flex3{\
	color:green;\
    line-height:150px;\
    background: #1e274e;\
    font-size:27px;\
    border-radius:0px 0px 15px 15px;\
}\
\
.flex3:hover{\
	color:green;\
    line-height:150px;\
    background: #343c5f;\
    font-size:28px;\
    border-radius:0px 0px 15px 15px;\
}\
\
\
</style>\
</head>\
<body>\
<div>\
 <div class=\"panel\">\
  	<h3>PANEL ADMIN CONTROL</h3>\
 </div>\
 	<div class=\"flex-container\">\
    	<div class=\"flex-box\">\
        	<div class=\"flex1\">Status controler</div>\
            <div class=\"flex3\"><a id=\"controlerStatus\"></a></div>\
        </div>\
        <div class=\"flex-box\">\
        	<div class=\"flex1\">Temperature</div>\
            <div class=\"flex2\"><a id=\"expectedTemp\"></a></div>\
        </div>\
        <div class=\"flex-box\">\
        	<div class=\"flex1\">Temperature room</div>\
            <div class=\"flex2\"><a id=\"roomTemperature\"></a></div>\
    </div>\
    <div class=\"flex-container\">\
    	<div class=\"flex-box\">\
        	<div class=\"flex1\">Status work heater</div>\
            <div class=\"flex3\"><a id=\"heaterTemperature\">NIE PRACUJE</a></div>\
        </div>\
        <div class=\"flex-box\">\
        	<div class=\"flex1\">Temperature CH</div>\
            <div class=\"flex2\"><a id=\"CHTemperature\"></a></div>\
        </div>\
        <div class=\"flex-box\">\
        	<div class=\"flex1\">Temperature DHW</div>\
            <div class=\"flex2\"><a id=\"DHWTemperature\"></a></div>\
    </div>\
    <div class=\"flex-container\">\
      <div class=\"flex-box\">\
        <div class=\"flex1\">PUMP CH</div>\
          <div class=\"flex3\"><a id=\"pumpChControl\">NIE PRACUJE</a></div>\
      </div>\
    	<div class=\"flex-box\">\
        	<div class=\"flex1\">PUMP DHW</div>\
            <div class=\"flex3\"><a id=\"pumpDHWControl\">NIE PRACUJE</a></div>\
        </div>\
        <div class=\"flex-box\">\
        	<div class=\"flex1\">Status ALARM</div>\
            <div class=\"flex3\"><a id=\"alarm\"></a></div>\
    </div>\
 \
</div>\
<script>\
function loadDoc(variable,variable_click){\
  const xhttp = new XMLHttpRequest();\
  xhttp.open(\"GET\", variable,true);\
  xhttp.send();\
  xhttp.onload = function(){\
    document.getElementById(variable_click).innerHTML = this.responseText;\
    console.log(this.responseText);\
     if(this.responseText==\"WORK\" || this.responseText==\"ON CONTROLER\" || this.responseText==\"NO ALARM\"){\
       document.getElementById(variable_click).style.color = 'green';\
    }else if(this.responseText==\"NOT WORK\" || this.responseText==\"OFF CONTROLER\"){\
      document.getElementById(variable_click).style.color = 'red';\
    }else if(this.responseText==\"OVERHEATING CH\" || this.responseText==\"OVERHEATING DHW\" ||  this.responseText==\"OVERHEATING\"){\
      document.getElementById(variable_click).style.color = 'red';\
    }\
  }}\
setInterval(()=>{\
loadDoc(\"/roomTemperature\",\"roomTemperature\");\
loadDoc(\"/CHTemperature\",\"CHTemperature\");\
loadDoc(\"/DHWTemperature\",\"DHWTemperature\");\
loadDoc(\"/heaterTemperature\",\"heaterTemperature\");\
loadDoc(\"/pumpChControl\",\"pumpChControl\");\
loadDoc(\"/pumpDHWControl\",\"pumpDHWControl\");\
loadDoc(\"/expectedTemp\",\"expectedTemp\");\
loadDoc(\"/alarm\",\"alarm\");\
},1000);\
\
document.getElementById(\"controlerStatus\").addEventListener(\"click\",myFunction);\
function myFunction(){\
  const xhttp2 = new XMLHttpRequest();\
  xhttp2.open(\"GET\", \"/onControler\",true);\
  xhttp2.send();\
  xhttp2.onload = function(){\
    var lon = this.responseText;\
    console.log(lon);\
    document.getElementById(\"controlerStatus\").innerHTML = this.responseText;\
    if(this.responseText==\"OFF CONTROLER\"){\
       document.getElementById('controlerStatus').style.color = 'red';\
    }else{\
      document.getElementById('controlerStatus').style.color = 'green';\
    }\
  }\
\
}\
\
loadDoc(\"/controlerStatus\",\"controlerStatus\");\
document.getElementById(\"expectedTemp\").addEventListener(\"click\",myTemperature);\
function myTemperature(){\
  location.href='/temperature';\
}\
</script>\
</body>\
</html>";

/*°C*/

char onTemperature[] = "<!DOCTYPE html>\
<html>\
<body>\
<style>\
body{\
	background: linear-gradient(120deg,#2980b9, #8e44ad);\
}\
.container{\
  position: absolute;\
  top: 50%;\
  left: 50%;\
  margin-right: -50%;\
  transform: translate(-50%, -50%) ;\
  width: 35%;\
  height: 50%;\
  background: GhostWhite;\
  border-radius:6px;\
}\
input[type=submit]{\
  position: absolute;\
  background-color: DarkSlateBlue;\
  border: none;\
  color: white;\
  padding: 5% 26%;\
  top: 85%;\
  left: 50%;\
  margin-right: -50%;\
  transform: translate(-50%, -50%);\
  border-radius: 10px;\
  font-size:25px;\
 }\
input[type=submit]:hover{\
  background-color: #5a5096;\
}\
h2 {\
  font-family: 'Georgia', serif;\
  font-size: 30px;\
  text-align: center;\
  position: absolute;\
  top: 5%;\
  left: 50%;\
  margin-right: -50%;\
  transform: translate(-50%, -50%);\
  padding:10px 10%;\
}\
\
#demo{\
  font-family: 'Georgia', serif;\
  font-size: 55px;\
  font-weight:540;\
  color:#006400;\
  text-align: center;\
  position: absolute;\
  top: 35%;\
  left: 50%;\
  margin-right: -50%;\
  transform: translate(-50%, -50%);\
  padding:10px 10%;\
}\
\
.slider {\
  -webkit-appearance: none;\
  appearance: none;\
  width: 85%; \
  height: 25px; \
  background: #d3d3d3; \
  outline: none; \
  opacity: 0.7;\
  -webkit-transition: .2s; \
  transition: opacity .2s;\
  position: absolute;\
  top: 58%;\
  left: 50%;\
  margin-right: -50%;\
  transform: translate(-50%, -50%);\
  box-sizing: border-box;\
}\
\
.slider:hover {\
  opacity: 1; \
}\
\
.slider::-webkit-slider-thumb {\
  -webkit-appearance: none;\ 
  appearance: none;\
  width: 25px; \
  height: 35px; \
  background: #04AA6D; \
  cursor: pointer; \
}\
\
</style>\
<div class=\"container\">\
<h2>Temperature</h2>\
<p><span id=\"demo\"></span></p><br>\
<form method=\"post\" action=\"/temperature_action\">\
   <input type=\"range\" min=\"0\" max=\"35\" value=\"20\" class=\"slider\" id=\"myRange\" name=\"myRange\"\" >\
  <br>\
  <input type=\"submit\" value=\"SUBMIT\" name=\"submit\">\
</form>\
</div>\
<script>\ 
var slider = document.getElementById(\"myRange\");\
var output = document.getElementById(\"demo\");\
output.innerHTML = slider.value; \
\
slider.oninput = function() {\
  output.innerHTML = this.value;\
}\
</script>\
</body>\
</html>";