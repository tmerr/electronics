PORT = /dev/ttyUSB0
default:
	arduino --upload esptests.ino --port $(PORT)
	stty -F $(PORT) cs8 115200 ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts
	tail -f $(PORT)
