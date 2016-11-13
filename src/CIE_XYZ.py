#!/usr/bin/env python
# -*- coding: utf-8 -*-
import csv

def main():
    with open("CIE_XYZ_CMF.csv", "rb") as f:
        R_X = G_X = B_X = []
        R_Y = []
        G_Y = []
        B_Y = []
        rd = csv.reader(f, delimiter=',', quotechar ='"')
        for row in rd:
            R_X.append(row[0])
            R_Y.append(row[1])
            G_Y.append(row[2])
            B_Y.append(row[3])
        print len(R_X)
        print "{" + ', '.join(R_X) + "}"
        print "-"*32
        print "{" + ', '.join(R_Y) + "}"
        print "-"*32
        print "{" + ', '.join(G_Y) + "}"
        print "-"*32
        print "{" + ', '.join(B_Y) + "}"

main()
