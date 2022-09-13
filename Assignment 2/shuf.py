#!/bin/env python3

import random, string, sys, argparse


def main():
    parser = argparse.ArgumentParser(description='Shuffles input.')

    parser.add_argument('--echo', '-e', nargs='+', action='store', dest='echo', help="Treat each command-line operand as an input line.")
    parser.add_argument('--input-range', '-i', action='store', dest='input_range', default=None, help="Act as if input came from a file containing the range of unsigned decimal integers lo...hi, one per line.")
    parser.add_argument('--head-count', '-n', nargs=1, action='store', type=int, dest='head_count', help="Output at most count lines. By default, all input lines are output.")
    parser.add_argument('--repeat', '-r', action='store_true', dest='repeat', help="Repeat output values, that is, select with replacement. With this option the output is not a permutation of the input; instead, each output line is randomly chosen from all the inputs. This option is typically combined with --head-count; if --head-count is not given, shuf repeats indefinitely.")
    parser.add_argument('file', nargs='?', default='')

    args = parser.parse_args()
    lines = []

    match parser.parse_known_args():
        case [args, unknown] if unknown:
            print('invalid options')
            sys.exit(1)
        
        case [args, unknown] if args.echo and args.input_range:
            print('cannot combine -e and -i options')
            sys.exit(1)

        case [args, unknown] if args.file != '':
            file = open(args.file)
            for _ in file.readlines():
                lines.append(_.rstrip('\n'))

            
    if args.input_range:
        lo_hi = args.input_range.split('-')
        if len(lo_hi) != 2:
            print('invalid input')
            sys.exit(1)
        lo = int(lo_hi[0])
        hi = int(lo_hi[1])
        if hi < lo:
            print('invalid input')
            sys.exit(1)
        lines = list(range(lo, hi+1))
        

    printLines = []
    if args.echo:
        printLines = args.echo
        lines = random.sample(printLines, len(printLines))
    else:
        lines = random.sample(lines, len(lines))

        
    if args.head_count:
        n = args.head_count
        if args.repeat:
            for x in range(n):
                print(random.choice(lines))
        else:
            for x in range(min(n, len(lines))):
                print(lines[x])
    else:
        if args.repeat:
            while True:
                print(random.choice(lines))
        elif not args.repeat and args.echo:
            for x in random.sample(lines, len(lines)):
                print(x)
        elif not args.repeat and not args.echo:
            for x in range(len(lines)):
                print(lines[x])


if __name__ == '__main__':
    main()
