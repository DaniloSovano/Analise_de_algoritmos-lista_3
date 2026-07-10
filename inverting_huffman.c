#include <stdio.h>
#include <stdlib.h>

int compare(const void *a, const void *b) {
    long long va = *(const long long *)a;
    long long vb = *(const long long *)b;
    if (va < vb) return -1;
    if (va > vb) return 1;
    return 0;
}

int main() {
    int N;
    
    while (scanf("%d", &N) != EOF) {
        int L[55];
        int level_counts[55] = {0};
        int max_depth = 0;
        
        for (int i = 0; i < N; i++) {
            scanf("%d", &L[i]);
            level_counts[L[i]]++;
            if (L[i] > max_depth) {
                max_depth = L[i];
            }
        }
        
        long long *current_nodes = (long long *)malloc(100 * sizeof(long long));
        int current_count = 0;
        long long threshold = 1;
        
        for (int d = max_depth; d >= 1; d--) {
            int leaves = level_counts[d];
            
            current_nodes = (long long *)realloc(current_nodes, (current_count + leaves) * sizeof(long long));
            
            for (int i = 0; i < leaves; i++) {
                current_nodes[current_count++] = threshold;
            }
            
            if (current_count == 0) continue;
            
            qsort(current_nodes, current_count, sizeof(long long), compare);
            
            threshold = current_nodes[current_count - 1];
            
            int next_count = current_count / 2;
            long long *next_nodes = (long long *)malloc(next_count * sizeof(long long));
            
            for (int i = 0; i < next_count; i++) {
                next_nodes[i] = current_nodes[2 * i] + current_nodes[2 * i + 1];
            }
            
            free(current_nodes);
            current_nodes = next_nodes;
            current_count = next_count;
        }
        
        printf("%lld\n", current_nodes[0]);
        free(current_nodes);
    }
    
    return 0;
}
