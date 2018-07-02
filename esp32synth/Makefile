PORT = /dev/ttyUSB*

all:
	arduino --upload main.ino --port $(PORT) --board "espressif:esp32:esp32"
	stty -F $(PORT) min 0 time 0 cs8 921600 ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts
	tail -f $(PORT)
