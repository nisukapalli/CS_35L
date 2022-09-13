#!/usr/local/cs/bin/python3

import os
import sys
import zlib


# strace commentary:
#
# first, I ran 'strace -f php -q topo_order_commits.py > strace.txt'
# then, I opened strace.txt in Emacs and used the C-s I-Search function
#   to look for any git commands that would be invoked
# I didn't find any git commands, and the only matches to my 'git' search
#   was in the name of my function find_git() and in a stderr message


# calls the other functions
def topo_order_commits():
    branches = get_branches()
    roots, graph = build_graph(branches)
    order = topological_sort(roots, graph)
    print_out(graph, order)


# find the .git directory
def find_git():
    cwd = os.getcwd()
    while (cwd != '/'):

        if os.access(cwd, os.R_OK):
            for dir in os.listdir(cwd):
                if os.path.isdir(os.path.join(cwd, dir)) and dir == '.git':
                    os.chdir('.git')
                    cwd = os.getcwd()
                    return cwd

            os.chdir('../')
            cwd = os.getcwd()

    if (cwd == '/'):
        sys.stderr.write('Not inside a Git repository\n')
        sys.exit(1)


# get a list of local branch names
def get_branches():
    branch_names = dict()
    path = os.path.join(find_git(), './refs/heads')
    os.chdir(path)

    for root, dirs, files in os.walk('.'):
        for i in dirs + files:
            temp = os.path.join(root, i)

            if os.path.isfile(temp):
                branch = temp[2:]
                hash = open(branch, 'r').read()[:-1]

                if hash not in branch_names.keys():
                    branch_names[hash] = [branch]
                else:
                    branch_names[hash].append(branch)

    os.chdir(find_git())
    return branch_names


class CommitNode:
    def __init__(self, commit_hash, branches=[]):
        self.commit_hash = commit_hash
        self.parents = set()
        self.children = set()
        self.branches = branches


# build the commit graph
def build_graph(branches):
    os.chdir(os.path.join(find_git(), './objects'))
    graph = {}
    roots = set()

    for hash in branches:
        if hash in graph.keys():
            graph[hash].branches = branches[hash]
            continue

        graph[hash] = CommitNode(hash, branches[hash])
        stack = [graph[hash]]

        while len(stack) > 0:
            top = stack.pop()
            h = top.commit_hash
            parents = get_parents(h)

            if len(parents) == 0:
                roots.add(h)
            for p_hash in parents:
                if p_hash not in graph.keys():
                    graph[p_hash] = CommitNode(p_hash)

                top.parents.add(graph[p_hash])
                graph[p_hash].children.add(top)
                stack.append(graph[p_hash])

    os.chdir(find_git())
    return sorted(roots), graph


# helper function to find parent hashes
def get_parents(hash):
    if hash[:2] not in os.listdir():
        return []

    path = os.path.join('.', hash[:2])
    os.chdir(path)

    if hash[2:] not in os.listdir():
        return []

    dcmp = zlib.decompress(open(hash[2:], 'rb').read())
    dcmp = dcmp.decode().split('\n')
    parents = list()
    os.chdir('../')

    i = 0
    while (i < len(dcmp)):
        if dcmp[i].startswith('parent'):
            words = dcmp[i].split(' ')
            parents.append(words[1])
        i += 1

    return parents


# generate a topological ordering of the commits in the graph
def topological_sort(roots, graph):
    branches = get_branches()
    roots, graph = build_graph(branches)
    os.chdir(find_git())
    ordered_commits = list()
    seen = set()
    stack = roots.copy()

    while len(stack) > 0:
        head = stack[-1]
        seen.add(head)
        children = [c for c in graph[head].children
                    if c.commit_hash not in seen]

        if len(children) == 0:
            ordered_commits.append(stack.pop())
            continue
        stack.append(children[0].commit_hash)

    return ordered_commits


# print the commit hashes in order, from least to greatest
def print_out(graph, order):
    i = 0
    while i < len(order):
        cur = graph[order[i]]

        if len(cur.branches) == 0:
            print(order[i])
        else:
            print(order[i] + " ", end="")
            val = sorted(cur.branches)
            print(*val)

        if i < (len(order) - 1):
            nxt = graph[order[i+1]]
            if order[i+1] not in [par.commit_hash
                                  for par in cur.parents]:
                p = [par.commit_hash for par in cur.parents]
                c = [child.commit_hash for child in nxt.children]
                print(*p, end='=\n')
                print('\n=', end='')
                print(*c)
        i += 1


# main
if __name__ == '__main__':
    topo_order_commits()
