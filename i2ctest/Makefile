PORT = /dev/ttyUSB0

all: build/sampletable.c
	arduino --upload main.ino --port $(PORT)
	stty -F $(PORT) cs8 115200 ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts
	tail -f $(PORT)

build/sampletable.c: gen_sample_table.py
	python3 gen_sample_table.py
