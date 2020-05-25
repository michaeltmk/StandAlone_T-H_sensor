# StandAlone_T-H_sensor
Monitor the temperature and relative humidity condition in a enclosed area, such as a dry box, with a battery.
Once the reading is over the limit, a message will be sent through a telegram bot.
This version is using a ESP8266 NodeMCU and ESP01 with DHT11 digital humidity temperature sensor, but it can be burn into any ESP8266 board with DHT22/DHT21 sensor.

## Configuration

There are a few parameters that need to be changed:
ssid : SSID of your WIFI connection
password : Password of your WIFI connection
BotKey : Telegram Bot Token
chat_id : chat ID to indentify the chatting client (you)
fingerprint : fingerprint of https://api.telegram.org/, need to update time to time
t_threshold : temperature threshold, will send out a message if over this limit
h_threshold : humidity threshold, will send out a message if over this limit

### Prerequisites

Software: Arduino IDE, CH34x driver (optional)
Hardware: ESP8266 NodeMCU, DHT11, Power Bank with 5V USB output

## License

This project is licensed under the MIT License - see the [LICENSE.md](LICENSE.md) file for details

