import sys
import time
import subprocess
from subprocess import Popen, PIPE

def testuj( komenda ):
    print 'Testowanie komendy: ', komenda
    proc = subprocess.Popen(['komendy_AT_biblioteka.exe'],
                            shell='True',
                            stdin=subprocess.PIPE,
                            stdout=subprocess.PIPE,
                            )
    stdout_value = proc.communicate(komenda)[0]
    print stdout_value

print "Skrypt testujacy dzialanie programu do komend AT"

testuj("AT+TEST")
