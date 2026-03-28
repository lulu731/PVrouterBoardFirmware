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
