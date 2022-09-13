# Git Repository Organization

A Python shell script that topologically sorts Git commits and prints the commit hashes in order. It builds a directed acyclic graph, with the vertices being the commits. There is a directed edge from each child commit to each of its parent commits, and a directed edge from each parent to each of its children.
