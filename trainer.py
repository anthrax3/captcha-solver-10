from os import listdir, chdir
from bpnn import NN
from subprocess import Popen, PIPE
from select import select

import simplejson as json

CAPTCHA_DIR = '/home/synack/src/solver/final/captcha.solved/'

def main():
    print 'Building neural network...'
    net = NN(4, 52, 26)
    print 'Training neural network'
    count = 0
    traindata = []
    for filename in listdir(CAPTCHA_DIR):
        if count % 100 == 0: print count
        count += 1
        chdir(CAPTCHA_DIR)
        proc = Popen(['../split', filename, '../out.jpg', '1', '0', '210'], stdout=PIPE)
        vectors = proc.stdout.read()
        proc.wait()
        if proc.returncode != 0:
            continue
        for line in vectors.split('\n'):
            line = [x for x in line.split(' ') if x]
            if not line: continue
            letter = ord(line[0].lower()) - ord('a')
            line = line[1:]
            line = [int(x) for x in line]

            letters = []
            for i in xrange(26):
                if i == letter:
                    letters.append(100)
                else:
                    letters.append(0)
            traindata.append((line, letters))

    net.train(traindata, iterations=10)
    print 'Finished training network... Saving'

    network = {
        'ni': net.ni,
        'nh': net.nh,
        'no': net.no,

        'ai': net.ai,
        'ah': net.ah,
        'ao': net.ao,

        'wi': net.wi,
        'wo': net.wo,

        'ci': net.ci,
        'co': net.co
    }

    json.dump(network, file('../network2.json', 'w'))
    print 'Finished dumping to network2.json'

if __name__ == '__main__':
    main()
