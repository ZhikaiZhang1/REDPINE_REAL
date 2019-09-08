#!/usr/bin/python

import re, sys

def read_file_contents(filename):

    # Open file for reading
    f     =  open(filename, "r")
    data  =  f.read()
    f.close()

    # Return data
    return data


def main(filename):

    # Read the contents of the file
    html          =  read_file_contents(filename)

    # Minify the html file, each newline & (two or more spaces) are removed
    html          =  re.sub(r"\n|(\s\s+)", "", html)

    # The output file name
    out_filename  =  filename.split('.')[0] + '_c.txt'

    # Open a file for writing
    f             =  open(out_filename, "w")

    # Total Length of the html file
    html_len      =  len(html)

    # dictionary of escape characters
    escape_chars = {'"':True, "\\":True}

    # While we still have some data to write
    f.write('"')
    for c in html:
        if c in escape_chars: f.write('\\')
        f.write(c)
    f.write('"')

    # Close the file when we're done
    f.close()


if __name__ == "__main__":
    if len(sys.argv) == 2:
        main(sys.argv[1])
    else:
        print 'Usage: %s <input-file>' % sys.argv[0]
