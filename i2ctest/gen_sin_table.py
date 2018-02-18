import os
import math


def build_data(bitdepth, sinresolution):
    result = []
    for x in range(sinresolution):
        floating = math.sin(2 * math.pi * (x / (sinresolution - 1)))
        norm = 0.5 * (1 + floating)
        sample = round(norm * (2**bitdepth - 1))
        clamped = max(0, min(2**bitdepth - 1, sample))
        result.append(clamped)
    return result


def run(bitdepth, sinresolution):
    try:
        os.mkdir('build')
    except FileExistsError:
        pass

    data = build_data(bitdepth, sinresolution);
    template = 'const uint16_t sinetable[{sinresolution}] = {datastr};\n'
    with open('build/sintable.c', 'w') as f:
        f.write("#define SINRESOLUTION {}\n".format(sinresolution))
        f.write(template.format(
            sinresolution=len(data),
            datastr='{' + ','.join(map(hex, data)) + '}'
        ))

if __name__ == '__main__':
    import settings
    run(settings.BITDEPTH, settings.SINRESOLUTION)
