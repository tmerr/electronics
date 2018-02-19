import os


template = 'static uint32_t sampletable[{rows}][{columns}] = \n{datastr};\n'


def build_bitstring(intval, bitdepth):
    """
    Adapted from psuedo code here
    https://en.wikipedia.org/wiki/Pulse-density_modulation
    """
    normalized_sample = intval / 2**bitdepth

    result = []
    error = 0
    ones = 0
    for i in range(2**bitdepth):
        if normalized_sample > error:
            bit = 1
            ones += 1
        else:
            bit = 0
        error += bit - normalized_sample

        result.append(str(bit))

    assert intval == ones

    return ''.join(result)


def split_every_nth(iterable, n):
    return [iterable[i:i+n] for i in range(0, len(iterable), n)]


def bitstring_to_hex_array(bitstring):
    return [hex(int(bits32, 2)) for bits32 in split_every_nth(bitstring, 32)]


def generate_data(bitdepth):
    result = []
    for value in range(2**bitdepth):
        result.append(
            bitstring_to_hex_array(build_bitstring(value, bitdepth))
        )
    return result


def stringify_data(data):
    lines = []
    lines.append('{')

    innerlines = []
    for hex_array in data:
        innerlines.append('{' + ', '.join(hex_array) + '}')
    lines.append(',\n'.join(innerlines))

    lines.append('}')

    return '\n'.join(lines)


def run(bitdepth):
    data = generate_data(bitdepth)
    rows = len(data)
    columns = len(data[0])
    table = template.format(
        rows=rows,
        columns=columns,
        datastr=stringify_data(data)
    )

    try:
        os.mkdir('build')
    except FileExistsError:
        pass

    with open('build/sampletable.c', 'w') as f:
        f.write('#define BITDEPTH {}\n'.format(bitdepth))
        f.write(table)


if __name__ == '__main__':
    import settings
    run(settings.BITDEPTH)
