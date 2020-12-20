# LED_Clock
This code is not perfect, but works and is a good starting point to learn from and improve.<br>
<br>
(No-Wifi Folder)<br>
This code doesn't keep perfect time. In order to do that you will need an Arduino with either a Real-Time-Clock or a Wifi connection and also the necessary code to work with those. But this code is functional and a good starting point.<br>
<br>
(Wifi Folder)<br>
This code requires a Wifi connection. The .INO file will need your wifi name and password entered in to connect to a time server. You will also need to adjust the utcOffset value to display the correct hour for your time zone (+/- up to 12 hours.) Also, it tries to reconnect to the time server at 12:00 (every 12 hours.) If it cannot connect, it will not display the time until it does.
