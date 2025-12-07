var wskt;

document.getElementById('initialise').onload = function()
{init()};

function init()// open WebSocket
{
  console.log('Trying to open a WebSocket connection...');
  wskt = new WebSocket('ws://' + window.location.hostname + ':8081/ws');
  wskt.onopen =  function(event){console.log('connection opened');};
  wskt.onclose = function(event){console.log('connection closed');};
  wskt.onmessage = function(rx)
  {  // client receive message
//      console.log('message received');
//      console.log('rx' + rx);
    var obj = rx.data;
    try {
      var js = JSON.parse(obj);
      js.objects.forEach(element => {
        document.getElementById('rxText').value +=
          element.id + " : " + element.value + "\n";
      });
    }
    catch(e)
    {
      document.getElementById('errorText').value += obj + "\n";
    }
  };
}