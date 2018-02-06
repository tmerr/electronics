#!/usr/bin/env python3
"""Compile html into a c++ string constant because no filesystem :("""

import os


def escapeline(line):
    return '"{}"'.format(line.replace('"', r'\"'))


def generate_html():
    """index.template, style.css -> index.html"""
    with open('index.template') as f1:
        template = f1.read()
    with open('style.css') as f2:
        style = f2.read()

    html = template.format(style=style)
    with open('build/index.html', 'w') as f3:
        f3.write(html)


def generate_header():
    """index.html -> index.hpp"""
    with open('build/index.html') as f:
        text = f.read()
    newlines = '\n'.join(map(escapeline, text.splitlines()))
    output = (
        '#ifndef _INDEX_HPP\n'
        '#define _INDEX_HPP\n'
        '\n'
        'const String INDEX_HTML = {};\n'
        '\n'
        '#endif'
    ).format(newlines)
    with open('build/index.hpp', 'w') as f:
        f.write(output)


def run():
    try:
        os.mkdir('build')
    except FileExistsError:
        pass
    generate_html()
    generate_header()


if __name__ == '__main__':
    run()
