var wskt;

window.onload = function()
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
      const formatted = js.objects.map(element => {
        const unit = element.id.startsWith('I') ? 'A' :
                     element.id.startsWith('U') ? 'V' : '';
        return `${element.id} = ${element.value}${unit}`;
      }).join(', ');
      document.getElementById('rxText').value += formatted + "\n";
    }
    catch(e)
    {
      document.getElementById('errorText').value += obj + "\n";
    }
  };
}
