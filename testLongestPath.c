#include <fcntl.h>
#include <getopt.h>
#include <limits.h>
#include <math.h>
#include <regex.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <time.h>
#include <unistd.h>

#include "panda.h"
int getLongestPath(int g[54][54], int nowNode) {
    int ret = 0;
    for (int i = 0; i < 54; ++i) {
        if (g[nowNode][i]) {
            g[nowNode][i] = 0;
            g[i][nowNode] = 0;
            ret = max(ret, getLongestPath(g, i) + 1);
            g[nowNode][i] = 1;
            g[i][nowNode] = 1;
        }
    }
    return ret;
}
int main(int argc, char **argv) {
    int graph[54][54] = {0};
    int n;
    scanf("%d", &n);
    int on[54] = {0};
    for (int i = 0; i < n; ++i) {
        int a, b;
        scanf("%d%d", &a, &b);
        graph[a][b] = graph[b][a] = 1;
        on[a] = on[b] = 1;
    }
    int longest = -1;
    for (int i = 0; i < 54; ++i) {
        if (on[i]) {
            int nowanswer = getLongestPath(graph, i);
            printf("%d\n", nowanswer);
            if (longest < nowanswer) longest = nowanswer;
        }
    }
    printf("answer:%d\n", longest);
}