#!/usr/bin/python2

import sys
import os
import re


AdditionalCommentText = \
""" *
 *
 * Bug Fixes and enhancements for Linux Kernels >= 3.2
 * by Benjamin Porter <BenjaminPorter86@gmail.com>
 *
 * Project homepage: https://github.com/FreedomBen/rtl8188ce-linux-driver
 *
"""


def fixFile( fileName ):
    # save to temp file so we don't get jacked up until we're done
    with open( fileName, 'r' ) as inp:
        with open( fileName + '.temp', 'w' ) as out:
            added = False
            for line in inp:
                if re.match('.*by.Benjamin.Porter.*', line) is not None:
                    added = True

            inp.seek(0)
            for line in inp:
                if not added and ("Larry Finger <Larry.Finger@lwfinger.net>" in line or "Hsinchu 300, Taiwan" in line):
                    out.write( line )
                    out.write( AdditionalCommentText )
                    added = True
                else:
                    out.write( line )

    # now copy temp file into other file and delete temp
    with open( fileName + '.temp', 'r' ) as inp:
        with open( fileName, 'w' ) as out:
            for line in inp:
                out.write( line )

    os.remove( fileName + '.temp' )


# removes spaces after ( and before ) instead of adding them
def isInvert():
    return ( '-i' in sys.argv or '--invert' in sys.argv )


def printUsage():
    print 'Usage: ' + sys.argv[0] + '[-i|--invert] file1 [file2] ...'
    sys.exit( 1 )


if __name__ == '__main__':

    files = sys.argv[1:]

    numFiles = 0

    for f in files:
        fixFile( f )
        numFiles += 1

    print 'Done.  Fixed ' + str( numFiles ) + ' files.'  
    sys.exit( 0 )

