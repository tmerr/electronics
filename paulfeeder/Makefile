PORT = /dev/ttyUSB0
upload:
	python3 buildhtml.py
	arduino --upload paulfeeder.ino --port $(PORT)
	stty -F $(PORT) cs8 115200 ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts
	tail -f $(PORT)
