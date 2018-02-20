PORT = /dev/ttyUSB0

all: build/sampletable.c build/sintable.c build/knobtable.c
	arduino --upload main.ino --port $(PORT)
	stty -F $(PORT) min 0 time 0 cs8 115200 ignbrk -brkint -icrnl -imaxbel -opost -onlcr -isig -icanon -iexten -echo -echoe -echok -echoctl -echoke noflsh -ixon -crtscts
	tail -f $(PORT)

build/sampletable.c: gen_sample_table.py settings.py
	python3 gen_sample_table.py

build/sintable.c: gen_sin_table.py settings.py
	python3 gen_sin_table.py

build/knobtable.c: gen_knob_table.py settings.py
	python3 gen_knob_table.py
