/* Copyright 2018 Pierre Hauweele

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License. */

#include <nauty/gtools.h>

#define putchar(c) putc((c),stdout)

void printset(setword s, int n) {
    for (int i = 0; i < n; ++i) {
        putchar((s & BITT[i]) == 0 ? '0' : '1');
    }
    putchar('\n');
}

void printgraph(graph *g, int n) {
    printf("[ %i\n", n);
    for (int i = 0; i < n; ++i) {
        printset(*GRAPHROW(g, i, 1), n);
    }
    puts("]");
}

#define NEIGHBORS(g, v) (*GRAPHROW(g, v, 1))
#define NONOPENNEIGHBORS(g, v) (~NEIGHBORS(g, v))
#define NONCLOSEDNEIGHBORS(g, v) (NONOPENNEIGHBORS(g, v) ^ BITT[v])
#define VERTICES(n) (((setword) -1) << (WORDSIZE - n))

/* Claw-free pruner. */
int clawfreeGeneral(graph *g, int n, int maxn) {
    int u, v, w;
    const int m = 1;
    setword sw1, sw2, sw3;
    for (u = 0; u < n; ++u) {
        sw1 = VERTICES(n) & NONOPENNEIGHBORS(g, u);
        for (v = u; (v = nextelement(&sw1,m,v)) >= 0;) {
            sw2 = sw1 & NONOPENNEIGHBORS(g, v);
            for (w = v; (w = nextelement(&sw2,m,w)) >= 0;) {
                if ((NEIGHBORS(g, u) & NEIGHBORS(g, v) &
                     NEIGHBORS(g, w)) != 0) {
                    return 1;
                }
            }
        }
    }
    return 0;
}

/* Claw-free pruner. */
int clawfreeOneShort(graph *g, int n, int maxn) {
    int i, j, k;
    const int m = 1;
    const int v = n - 1;
    set *sv = GRAPHROW(g, v, m);
    setword sw1;
    for (i = -1; (i = nextelement(sv,m,i)) >= 0;) {
        /* Look for a claw with v as the articulation vertex, and i as one of
         * the fingertips. */
        /* neighbors of v but not of i. */
        sw1 = *sv & NONCLOSEDNEIGHBORS(g, i);
        for (j = i; (j = nextelement(&sw1,m,j)) >= 0;) {
            /* neighbors of v but of neither i nor j. */
            if ((sw1 & NONCLOSEDNEIGHBORS(g, j)) != 0) {
                return 1;
            }
        }
        /* Look for a claw with v as one of the fingertips and i as the
         * articulation vertex. */
        /* Neighbors of i but not of v. */
        sw1 = NEIGHBORS(g, i) & NONCLOSEDNEIGHBORS(g, v);
        for (j = -1; (j = nextelement(&sw1,m,j)) >= 0;) {
            /* Neighbors of i but of neither v nor j. */
            if ((sw1 & NONCLOSEDNEIGHBORS(g, j)) != 0) {
                return 1;
            }
        }
    }
    return 0;
}

/* Claw-free pruner. */
int clawfree(graph *g, int n, int maxn) {
    int v, w;
    const int u = n - 1;
    const int m = 1;
    setword sw1, sw2;
    /* Look for a claw with u as the articulation vertex */
    sw1 = NEIGHBORS(g, u);
    for (v = -1; (v = nextelement(&sw1,m,v)) >= 0;) {
        /* Neighbors of u but not of v. */
        sw2 = sw1 & NONCLOSEDNEIGHBORS(g, v);
        for (w = v; (w = nextelement(&sw2,m,w)) >= 0;) {
            /*  There is a claw if there exists a neighbor of u but of neither
             *  v nor w. */
            if ((sw2 & NONCLOSEDNEIGHBORS(g, w)) != 0){
                return 1;
            }
        }
    }
    /* Look for a claw with v as one of the fingertips. We look for a 3 indep
     * set of vertices containing u and check if they have a neighbor in
     * common. */
    /* Nonneighbors of u. */
    sw1 = VERTICES(u) & NONCLOSEDNEIGHBORS(g, u);
    for (v = -1; (v = nextelement(&sw1,m,v)) >= 0;) {
        /* Nonneighbors of neither u nor v (no need to remove v for the later
         * processing). */
        sw2 = sw1 & NONOPENNEIGHBORS(g, v);
        for (w = v; (w = nextelement(&sw2,m,w)) >= 0;) {
            if ((NEIGHBORS(g, u) & NEIGHBORS(g, v) & NEIGHBORS(g, w)) != 0) {
                return 1;
            }
        }
    }
    return 0;
}
