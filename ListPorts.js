var latestData = 0;

var serialport = require('serialport');// include the library
var servi = require('servi');

var app = new servi(false); // servi instance
app.port(8080);             // port number to run the server on
 
// configure the server's behavior:
app.serveFiles("public");     // serve static HTML from public folder..
app.route('/data', sendData); // route requests for /data to sendData()
app.route('/input', sendSerialInput); // route requests for /input to sendSerialInput()
// now that everything is configured, start the server:
app.start();

SerialPort = serialport.SerialPort, // make a local instance of it
// get port name from the command line:
portName = process.argv[2];
   
 var myPort = new SerialPort(portName, {
   baudRate: 9600,
   // look for return and newline at the end of each data packet:
   parser: serialport.parsers.readline("\r\n")
 });
 
myPort.on('open', showPortOpen);
myPort.on('data', saveLatestData);
myPort.on('close', showPortClose);
myPort.on('error', showError);
myPort.on('input', sendSerialInput);
 
 function showPortOpen() {			// maybe add a parameter like userInput
   console.log('port open. Data rate: ' + myPort.options.baudRate);
   // myPort.write("5 l30 ");			// Then use userInput down here to write it to the port to send it to Arduino
}
 
function sendSerialInput() {
	myPort.write("1 l30 ");
}

function saveLatestData(data) {
   console.log(data);
   latestData = data;
}
 
function showPortClose() {
   console.log('port closed.');
}
 
function showError(error) {
   console.log('Serial port error: ' + error);
}

function sendData(request) {
  // print out the fact that a client HTTP request came in to the server:
  console.log("Got a client request, sending them the data.");
  // respond to the client request with the latest serial string:
  request.respond(latestData);
  // document.getElementById("btn01").addEventListener("click", function(){ alert("Hello World!"); });
}