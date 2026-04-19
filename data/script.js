var wskt;
var reconnectAttempts = 0;
var maxReconnectAttempts = 5;
var reconnectDelay = 1000; // 1 second

window.onload = function()
{init()};

function init()// open WebSocket
{
  console.log('Trying to open a WebSocket connection...');
  connectWebSocket();
}

function connectWebSocket() {
  wskt = new WebSocket('ws://' + window.location.hostname + ':8081/ws');

  wskt.onopen = function(event) {
    console.log('WebSocket connection opened');
    document.getElementById('errorText').value += "WebSocket connected successfully\n";
    reconnectAttempts = 0; // Reset reconnect counter on successful connection
  };

  wskt.onclose = function(event) {
    console.log('WebSocket connection closed: ' + event.code + ' ' + event.reason);
    document.getElementById('errorText').value += "WebSocket closed. Attempting to reconnect...\n";

    // Attempt to reconnect
    if (reconnectAttempts < maxReconnectAttempts) {
      reconnectAttempts++;
      setTimeout(connectWebSocket, reconnectDelay);
      reconnectDelay *= 2; // Exponential backoff
    } else {
      document.getElementById('errorText').value += "Max reconnect attempts reached. Please refresh the page.\n";
    }
  };

  wskt.onerror = function(event) {
    console.log('WebSocket error: ', event);
    document.getElementById('errorText').value += "WebSocket error occurred\n";
  };

  wskt.onmessage = function(rx)
  {  // client receive message
//      console.log('message received');
//      console.log('rx' + rx);
    var obj = rx.data;
    try {
      var js = JSON.parse(obj);
      const formatted = js.objects.map(element => {
        const unit = element.id.startsWith('I') ? 'A' :
                     element.id.startsWith('U') ? 'V' : '';
        const value = element.id.startsWith('I') ? (element.value/1000).toFixed(1) :
                      element.id.startsWith('U') ? element.value/100 : 0;
        return `${element.id} = ${value}${unit}`;
      }).join(', ');
      document.getElementById('rxText').value += formatted + "\n";

      // Initialize input fields with calibration values
      js.objects.forEach(element => {
        if (element.id === 'Ugain') {
          document.getElementById('Ugain').value = element.value;
        } else if (element.id === 'IgainL') {
          document.getElementById('IgainL').value = element.value;
        } else if (element.id === 'IgainN') {
          document.getElementById('IgainN').value = element.value;
        }
      });
    }
    catch(e)
    {
      document.getElementById('errorText').value += obj + "\n";
    }
  };
}
