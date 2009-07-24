from os import listdir
from bpnn import NN
from subprocess import Popen, PIPE
import sys

import simplejson as json

def main():
    net = NN(7, 52, 26)
    sys.stderr.write('Loading neural net\n')
    network = json.load(file('network2.json', 'r'))
    for key in network:
        setattr(net, key, network[key])

    for filename in listdir('test'):
        print filename, ': ',
        proc = Popen(['./split', 'test/' + filename, 'out.jpg', '2', '0', '210'], stdout=PIPE)
        proc.wait()
        for line in proc.stdout.read().split('\n'):
            if not line:
                sys.stdout.write('\n')
                continue

            pattern = [int(x) for x in line.split(' ')]
            outputs = net.update(pattern)
            #break

            high_value = 0
            high_letter = 0
            for i in xrange(len(outputs)):
                if outputs[i] > high_value:
                    high_value = outputs[i]
                    high_letter = i
            sys.stdout.write(chr(ord('a') + high_letter))

if __name__ == '__main__':
    main()
