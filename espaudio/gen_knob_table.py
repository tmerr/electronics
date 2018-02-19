"""
Map the 0-1023 analog input values to frequency on an exponential scale.
"""


import os


ADCBITS = 10


def buildfunc(freqrange, clockhz):
    def closure(reading):
        # off by ones, whatever
        assert reading >= 0
        assert reading <= 1023
        normalized = reading/(2**ADCBITS - 1)
        freq = (freqrange ** normalized) * (freqrange - 1)/freqrange
        result = round(clockhz / freq)
        assert result > 0
        assert result >= clockhz * 1/freqrange
        return result
    return closure


def run(freqrange, clockhz):
    try:
        os.mkdir('build')
    except FileExistsError:
        pass

    compute = buildfunc(freqrange, clockhz)
    data = list(map(compute, range(2**ADCBITS)))
    template = 'const uint32_t knobtable[{freqrange}] = {datastr};\n'
    with open('build/knobtable.c', 'w') as f:
        f.write(template.format(
            freqrange=len(data),
            datastr='{' + ','.join(map(hex, data)) + '}'
        ))

if __name__ == '__main__':
    import settings
    run(settings.FREQRANGE, settings.CLOCKHZ)
