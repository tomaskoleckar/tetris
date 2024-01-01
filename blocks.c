#include "blocks.h"
#include <stdio.h>

SHAPE blocks[7] = {
    {{0,0,255},
        {{0,0,1,0}, 
         {1,1,1,0}, 
         {0,0,0,0}, 
         {0,0,0,0}},
        5, 0, 3, 3
    },
    {{255,0,0},
        {{1,0,0,0}, 
         {1,1,0,0}, 
         {0,1,0,0}, 
         {0,0,0,0}},
        5, 0, 3 , 3
    },
    {{0,255,0},
        {{0,1,0,0}, 
         {1,1,0,0}, 
         {1,0,0,0}, 
         {0,0,0,0}},
        5, 0, 3 , 3
    },
    {{0,255,255},
        {{1,1,1,1}, 
         {0,0,0,0}, 
         {0,0,0,0}, 
         {0,0,0,0}},
        5, 0, 4, 4
    },
    {{255,165,0},
        {{1,1,1,0}, 
         {0,0,1,0}, 
         {0,0,0,0}, 
         {0,0,0,0}},
        5, 0, 3 , 2
    },
    {{255,255,0},
        {{1,1,0,0}, 
         {1,1,0,0}, 
         {0,0,0,0}, 
         {0,0,0,0}},
        5, 0, 2, 2
    },
    {{128,0,128},
        {{1,0,0,0}, 
         {1,1,0,0}, 
         {1,0,0,0}, 
         {0,0,0,0}},
        5, 0, 3, 3
    },
};

int reverseCols(int matrix[4][4], int size) {
    int newSize = size;
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size / 2; j++) {
            int t = matrix[i][j];
            matrix[i][j] = matrix[i][size - j - 1];
            matrix[i][size - j - 1] = t;
        }
    }

    /*for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 4; j++) {
            if (matrix[j][j] == 1) {
                newSize++;
                break;
            }
        }
    }*/

    return newSize;
}

void transpose(int matrix[4][4], int size) {
    int tmp[4][4];
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            tmp[i][j] = matrix[j][i];
        }
    }
    for(int i = 0; i < size; i++) {
        for(int j = 0; j < size; j++) {
            matrix[i][j] = tmp[i][j];
        }
    }
}