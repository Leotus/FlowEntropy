#ifndef MCUBE_H 
#define MCUBE_H 1 

struct MCubeTable {
  int which_case; 
  int permute[8]; 
  int nbrs; 
};

static MCubeTable mcube_table[256] = {
{0, {1, 2, 3, 4, 5, 6, 7, 8}, 0},
{1, {1, 2, 3, 4, 5, 6, 7, 8}, 25},
{1, {2, 3, 4, 1, 6, 7, 8, 5}, 26},
{2, {1, 2, 3, 4, 5, 6, 7, 8}, 27},
{1, {3, 4, 1, 2, 7, 8, 5, 6}, 22},
{3, {1, 2, 3, 4, 5, 6, 7, 8}, 31},
{2, {2, 3, 4, 1, 6, 7, 8, 5}, 30},
{5, {4, 3, 7, 8, 1, 2, 6, 5}, 31},
{1, {4, 1, 2, 3, 8, 5, 6, 7}, 21},
{2, {4, 1, 2, 3, 8, 5, 6, 7}, 29},
{3, {2, 3, 4, 1, 6, 7, 8, 5}, 31},
{5, {3, 2, 6, 7, 4, 1, 5, 8}, 31},
{2, {3, 4, 1, 2, 7, 8, 5, 6}, 23},
{5, {2, 1, 5, 6, 3, 4, 8, 7}, 31},
{5, {1, 4, 8, 5, 2, 3, 7, 6}, 31},
{8, {3, 2, 6, 7, 4, 1, 5, 8}, 15},
{1, {5, 8, 7, 6, 1, 4, 3, 2}, 41},
{2, {5, 1, 4, 8, 6, 2, 3, 7}, 57},
{3, {5, 6, 2, 1, 8, 7, 3, 4}, 59},
{5, {6, 5, 8, 7, 2, 1, 4, 3}, 59},
{4, {3, 4, 1, 2, 7, 8, 5, 6}, 63},
{6, {5, 1, 4, 8, 6, 2, 3, 7}, 63},
{6, {3, 2, 6, 7, 4, 1, 5, 8}, 63},
{14, {6, 5, 8, 7, 2, 1, 4, 3}, 63},
{3, {5, 1, 4, 8, 6, 2, 3, 7}, 61},
{5, {8, 4, 3, 7, 5, 1, 2, 6}, 61},
{7, {3, 4, 1, 2, 7, 8, 5, 6}, 63},
{9, {2, 6, 7, 3, 1, 5, 8, 4}, 63},
{6, {3, 4, 1, 2, 7, 8, 5, 6}, 63},
{11, {3, 2, 6, 7, 4, 1, 5, 8}, 63},
{12, {1, 4, 8, 5, 2, 3, 7, 6}, 63},
{5, {5, 6, 2, 1, 8, 7, 3, 4}, 47},
{1, {6, 7, 3, 2, 5, 8, 4, 1}, 42},
{3, {6, 2, 1, 5, 7, 3, 4, 8}, 59},
{2, {2, 6, 7, 3, 1, 5, 8, 4}, 58},
{5, {5, 1, 4, 8, 6, 2, 3, 7}, 59},
{3, {6, 7, 3, 2, 5, 8, 4, 1}, 62},
{7, {4, 1, 2, 3, 8, 5, 6, 7}, 63},
{5, {7, 6, 5, 8, 3, 2, 1, 4}, 62},
{9, {6, 5, 8, 7, 2, 1, 4, 3}, 63},
{4, {4, 1, 2, 3, 8, 5, 6, 7}, 63},
{6, {4, 1, 2, 3, 8, 5, 6, 7}, 63},
{6, {6, 2, 1, 5, 7, 3, 4, 8}, 63},
{11, {6, 5, 8, 7, 2, 1, 4, 3}, 63},
{6, {4, 3, 7, 8, 1, 2, 6, 5}, 63},
{12, {2, 1, 5, 6, 3, 4, 8, 7}, 63},
{14, {7, 6, 5, 8, 3, 2, 1, 4}, 63},
{5, {6, 7, 3, 2, 5, 8, 4, 1}, 47},
{2, {6, 5, 8, 7, 2, 1, 4, 3}, 43},
{5, {2, 6, 7, 3, 1, 5, 8, 4}, 59},
{5, {1, 2, 3, 4, 5, 6, 7, 8}, 59},
{8, {1, 2, 3, 4, 5, 6, 7, 8}, 51},
{6, {5, 6, 2, 1, 8, 7, 3, 4}, 63},
{12, {2, 6, 7, 3, 1, 5, 8, 4}, 63},
{11, {5, 1, 4, 8, 6, 2, 3, 7}, 63},
{5, {3, 4, 1, 2, 7, 8, 5, 6}, 55},
{6, {6, 5, 8, 7, 2, 1, 4, 3}, 63},
{14, {2, 6, 7, 3, 1, 5, 8, 4}, 63},
{12, {1, 2, 3, 4, 5, 6, 7, 8}, 63},
{5, {4, 8, 5, 1, 3, 7, 6, 2}, 55},
{10, {4, 1, 2, 3, 8, 5, 6, 7}, 63},
{6, {8, 7, 6, 5, 4, 3, 2, 1}, 63},
{6, {7, 8, 4, 3, 6, 5, 1, 2}, 63},
{2, {8, 7, 6, 5, 4, 3, 2, 1}, 39},
{1, {7, 3, 2, 6, 8, 4, 1, 5}, 38},
{4, {1, 2, 3, 4, 5, 6, 7, 8}, 63},
{3, {2, 6, 7, 3, 1, 5, 8, 4}, 62},
{6, {1, 2, 3, 4, 5, 6, 7, 8}, 63},
{2, {7, 3, 2, 6, 8, 4, 1, 5}, 54},
{6, {7, 3, 2, 6, 8, 4, 1, 5}, 63},
{5, {6, 2, 1, 5, 7, 3, 4, 8}, 62},
{11, {7, 6, 5, 8, 3, 2, 1, 4}, 63},
{3, {7, 8, 4, 3, 6, 5, 1, 2}, 55},
{6, {1, 4, 8, 5, 2, 3, 7, 6}, 63},
{7, {1, 2, 3, 4, 5, 6, 7, 8}, 63},
{12, {3, 2, 6, 7, 4, 1, 5, 8}, 63},
{5, {8, 7, 6, 5, 4, 3, 2, 1}, 55},
{14, {8, 7, 6, 5, 4, 3, 2, 1}, 63},
{9, {7, 6, 5, 8, 3, 2, 1, 4}, 63},
{5, {7, 8, 4, 3, 6, 5, 1, 2}, 47},
{3, {5, 8, 7, 6, 1, 4, 3, 2}, 47},
{6, {1, 5, 6, 2, 4, 8, 7, 3}, 63},
{7, {3, 2, 6, 7, 4, 1, 5, 8}, 63},
{12, {6, 5, 8, 7, 2, 1, 4, 3}, 63},
{6, {3, 7, 8, 4, 2, 6, 5, 1}, 63},
{10, {3, 2, 6, 7, 4, 1, 5, 8}, 63},
{12, {6, 2, 1, 5, 7, 3, 4, 8}, 63},
{6, {4, 8, 5, 1, 3, 7, 6, 2}, 63},
{7, {6, 5, 8, 7, 2, 1, 4, 3}, 63},
{12, {8, 4, 3, 7, 5, 1, 2, 6}, 63},
{13, {2, 3, 4, 1, 6, 7, 8, 5}, 63},
{7, {2, 6, 7, 3, 1, 5, 8, 4}, 63},
{12, {8, 7, 6, 5, 4, 3, 2, 1}, 63},
{6, {2, 6, 7, 3, 1, 5, 8, 4}, 63},
{7, {7, 6, 5, 8, 3, 2, 1, 4}, 63},
{3, {6, 5, 8, 7, 2, 1, 4, 3}, 47},
{2, {6, 7, 3, 2, 5, 8, 4, 1}, 46},
{6, {7, 6, 5, 8, 3, 2, 1, 4}, 63},
{5, {3, 7, 8, 4, 2, 6, 5, 1}, 62},
{14, {5, 1, 4, 8, 6, 2, 3, 7}, 63},
{5, {2, 3, 4, 1, 6, 7, 8, 5}, 62},
{12, {2, 3, 4, 1, 6, 7, 8, 5}, 63},
{8, {2, 3, 4, 1, 6, 7, 8, 5}, 60},
{5, {1, 5, 6, 2, 4, 8, 7, 3}, 61},
{6, {6, 7, 3, 2, 5, 8, 4, 1}, 63},
{10, {1, 2, 3, 4, 5, 6, 7, 8}, 63},
{12, {3, 7, 8, 4, 2, 6, 5, 1}, 63},
{6, {5, 8, 7, 6, 1, 4, 3, 2}, 63},
{11, {4, 8, 5, 1, 3, 7, 6, 2}, 63},
{6, {8, 5, 1, 4, 7, 6, 2, 3}, 63},
{5, {4, 1, 2, 3, 8, 5, 6, 7}, 61},
{2, {5, 8, 7, 6, 1, 4, 3, 2}, 45},
{5, {8, 5, 1, 4, 7, 6, 2, 3}, 47},
{11, {1, 2, 3, 4, 5, 6, 7, 8}, 63},
{9, {2, 3, 4, 1, 6, 7, 8, 5}, 63},
{5, {7, 3, 2, 6, 8, 4, 1, 5}, 55},
{14, {2, 3, 4, 1, 6, 7, 8, 5}, 63},
{6, {8, 4, 3, 7, 5, 1, 2, 6}, 63},
{5, {5, 8, 7, 6, 1, 4, 3, 2}, 61},
{2, {4, 8, 5, 1, 3, 7, 6, 2}, 53},
{12, {8, 5, 1, 4, 7, 6, 2, 3}, 63},
{6, {2, 3, 4, 1, 6, 7, 8, 5}, 63},
{7, {2, 3, 4, 1, 6, 7, 8, 5}, 63},
{3, {8, 4, 3, 7, 5, 1, 2, 6}, 55},
{6, {2, 1, 5, 6, 3, 4, 8, 7}, 63},
{4, {2, 3, 4, 1, 6, 7, 8, 5}, 63},
{3, {1, 4, 8, 5, 2, 3, 7, 6}, 61},
{1, {8, 7, 6, 5, 4, 3, 2, 1}, 37},
{1, {8, 7, 6, 5, 4, 3, 2, 1}, 37},
{3, {1, 4, 8, 5, 2, 3, 7, 6}, 61},
{4, {2, 3, 4, 1, 6, 7, 8, 5}, 63},
{6, {2, 1, 5, 6, 3, 4, 8, 7}, 63},
{3, {8, 4, 3, 7, 5, 1, 2, 6}, 55},
{7, {2, 3, 4, 1, 6, 7, 8, 5}, 63},
{6, {2, 3, 4, 1, 6, 7, 8, 5}, 63},
{12, {4, 3, 7, 8, 1, 2, 6, 5}, 63},
{2, {4, 8, 5, 1, 3, 7, 6, 2}, 53},
{5, {5, 8, 7, 6, 1, 4, 3, 2}, 61},
{6, {8, 4, 3, 7, 5, 1, 2, 6}, 63},
{14, {3, 2, 6, 7, 4, 1, 5, 8}, 63},
{5, {7, 3, 2, 6, 8, 4, 1, 5}, 55},
{9, {3, 2, 6, 7, 4, 1, 5, 8}, 63},
{11, {8, 7, 6, 5, 4, 3, 2, 1}, 63},
{5, {8, 5, 1, 4, 7, 6, 2, 3}, 47},
{2, {5, 8, 7, 6, 1, 4, 3, 2}, 45},
{5, {4, 1, 2, 3, 8, 5, 6, 7}, 61},
{6, {8, 5, 1, 4, 7, 6, 2, 3}, 63},
{11, {2, 6, 7, 3, 1, 5, 8, 4}, 63},
{6, {5, 8, 7, 6, 1, 4, 3, 2}, 63},
{12, {4, 1, 2, 3, 8, 5, 6, 7}, 63},
{10, {3, 4, 1, 2, 7, 8, 5, 6}, 63},
{6, {6, 7, 3, 2, 5, 8, 4, 1}, 63},
{5, {1, 5, 6, 2, 4, 8, 7, 3}, 61},
{8, {4, 1, 2, 3, 8, 5, 6, 7}, 60},
{12, {1, 5, 6, 2, 4, 8, 7, 3}, 63},
{5, {2, 3, 4, 1, 6, 7, 8, 5}, 62},
{14, {7, 3, 2, 6, 8, 4, 1, 5}, 63},
{5, {3, 7, 8, 4, 2, 6, 5, 1}, 62},
{6, {7, 6, 5, 8, 3, 2, 1, 4}, 63},
{2, {6, 7, 3, 2, 5, 8, 4, 1}, 46},
{3, {6, 5, 8, 7, 2, 1, 4, 3}, 47},
{7, {7, 6, 5, 8, 3, 2, 1, 4}, 63},
{6, {2, 6, 7, 3, 1, 5, 8, 4}, 63},
{12, {5, 1, 4, 8, 6, 2, 3, 7}, 63},
{7, {2, 6, 7, 3, 1, 5, 8, 4}, 63},
{13, {1, 2, 3, 4, 5, 6, 7, 8}, 63},
{12, {7, 6, 5, 8, 3, 2, 1, 4}, 63},
{7, {6, 5, 8, 7, 2, 1, 4, 3}, 63},
{6, {4, 8, 5, 1, 3, 7, 6, 2}, 63},
{12, {5, 8, 7, 6, 1, 4, 3, 2}, 63},
{10, {6, 7, 3, 2, 5, 8, 4, 1}, 63},
{6, {3, 7, 8, 4, 2, 6, 5, 1}, 63},
{12, {7, 3, 2, 6, 8, 4, 1, 5}, 63},
{7, {3, 2, 6, 7, 4, 1, 5, 8}, 63},
{6, {1, 5, 6, 2, 4, 8, 7, 3}, 63},
{3, {5, 8, 7, 6, 1, 4, 3, 2}, 47},
{5, {7, 8, 4, 3, 6, 5, 1, 2}, 47},
{9, {1, 2, 3, 4, 5, 6, 7, 8}, 63},
{14, {1, 2, 3, 4, 5, 6, 7, 8}, 63},
{5, {8, 7, 6, 5, 4, 3, 2, 1}, 55},
{12, {7, 8, 4, 3, 6, 5, 1, 2}, 63},
{7, {1, 2, 3, 4, 5, 6, 7, 8}, 63},
{6, {1, 4, 8, 5, 2, 3, 7, 6}, 63},
{3, {7, 8, 4, 3, 6, 5, 1, 2}, 55},
{11, {4, 1, 2, 3, 8, 5, 6, 7}, 63},
{5, {6, 2, 1, 5, 7, 3, 4, 8}, 62},
{6, {7, 3, 2, 6, 8, 4, 1, 5}, 63},
{2, {7, 3, 2, 6, 8, 4, 1, 5}, 54},
{6, {1, 2, 3, 4, 5, 6, 7, 8}, 63},
{3, {2, 6, 7, 3, 1, 5, 8, 4}, 62},
{4, {1, 2, 3, 4, 5, 6, 7, 8}, 63},
{1, {7, 3, 2, 6, 8, 4, 1, 5}, 38},
{2, {8, 7, 6, 5, 4, 3, 2, 1}, 39},
{6, {7, 8, 4, 3, 6, 5, 1, 2}, 63},
{6, {8, 7, 6, 5, 4, 3, 2, 1}, 63},
{10, {2, 3, 4, 1, 6, 7, 8, 5}, 63},
{5, {4, 8, 5, 1, 3, 7, 6, 2}, 55},
{12, {4, 8, 5, 1, 3, 7, 6, 2}, 63},
{14, {4, 8, 5, 1, 3, 7, 6, 2}, 63},
{6, {6, 5, 8, 7, 2, 1, 4, 3}, 63},
{5, {3, 4, 1, 2, 7, 8, 5, 6}, 55},
{11, {7, 3, 2, 6, 8, 4, 1, 5}, 63},
{12, {3, 4, 1, 2, 7, 8, 5, 6}, 63},
{6, {5, 6, 2, 1, 8, 7, 3, 4}, 63},
{8, {3, 4, 1, 2, 7, 8, 5, 6}, 51},
{5, {1, 2, 3, 4, 5, 6, 7, 8}, 59},
{5, {2, 6, 7, 3, 1, 5, 8, 4}, 59},
{2, {6, 5, 8, 7, 2, 1, 4, 3}, 43},
{5, {6, 7, 3, 2, 5, 8, 4, 1}, 47},
{14, {4, 1, 2, 3, 8, 5, 6, 7}, 63},
{12, {6, 7, 3, 2, 5, 8, 4, 1}, 63},
{6, {4, 3, 7, 8, 1, 2, 6, 5}, 63},
{11, {3, 4, 1, 2, 7, 8, 5, 6}, 63},
{6, {6, 2, 1, 5, 7, 3, 4, 8}, 63},
{6, {4, 1, 2, 3, 8, 5, 6, 7}, 63},
{4, {4, 1, 2, 3, 8, 5, 6, 7}, 63},
{9, {4, 1, 2, 3, 8, 5, 6, 7}, 63},
{5, {7, 6, 5, 8, 3, 2, 1, 4}, 62},
{7, {4, 1, 2, 3, 8, 5, 6, 7}, 63},
{3, {6, 7, 3, 2, 5, 8, 4, 1}, 62},
{5, {5, 1, 4, 8, 6, 2, 3, 7}, 59},
{2, {2, 6, 7, 3, 1, 5, 8, 4}, 58},
{3, {6, 2, 1, 5, 7, 3, 4, 8}, 59},
{1, {6, 7, 3, 2, 5, 8, 4, 1}, 42},
{5, {5, 6, 2, 1, 8, 7, 3, 4}, 47},
{12, {5, 6, 2, 1, 8, 7, 3, 4}, 63},
{11, {2, 3, 4, 1, 6, 7, 8, 5}, 63},
{6, {3, 4, 1, 2, 7, 8, 5, 6}, 63},
{9, {3, 4, 1, 2, 7, 8, 5, 6}, 63},
{7, {3, 4, 1, 2, 7, 8, 5, 6}, 63},
{5, {8, 4, 3, 7, 5, 1, 2, 6}, 61},
{3, {5, 1, 4, 8, 6, 2, 3, 7}, 61},
{14, {3, 4, 1, 2, 7, 8, 5, 6}, 63},
{6, {3, 2, 6, 7, 4, 1, 5, 8}, 63},
{6, {5, 1, 4, 8, 6, 2, 3, 7}, 63},
{4, {3, 4, 1, 2, 7, 8, 5, 6}, 63},
{5, {6, 5, 8, 7, 2, 1, 4, 3}, 59},
{3, {5, 6, 2, 1, 8, 7, 3, 4}, 59},
{2, {5, 1, 4, 8, 6, 2, 3, 7}, 57},
{1, {5, 8, 7, 6, 1, 4, 3, 2}, 41},
{8, {6, 7, 3, 2, 5, 8, 4, 1}, 15},
{5, {1, 4, 8, 5, 2, 3, 7, 6}, 31},
{5, {2, 1, 5, 6, 3, 4, 8, 7}, 31},
{2, {3, 4, 1, 2, 7, 8, 5, 6}, 23},
{5, {3, 2, 6, 7, 4, 1, 5, 8}, 31},
{3, {2, 3, 4, 1, 6, 7, 8, 5}, 31},
{2, {4, 1, 2, 3, 8, 5, 6, 7}, 29},
{1, {4, 1, 2, 3, 8, 5, 6, 7}, 21},
{5, {4, 3, 7, 8, 1, 2, 6, 5}, 31},
{2, {2, 3, 4, 1, 6, 7, 8, 5}, 30},
{3, {1, 2, 3, 4, 5, 6, 7, 8}, 31},
{1, {3, 4, 1, 2, 7, 8, 5, 6}, 22},
{2, {1, 2, 3, 4, 5, 6, 7, 8}, 27},
{1, {2, 3, 4, 1, 6, 7, 8, 5}, 26},
{1, {1, 2, 3, 4, 5, 6, 7, 8}, 25},
{0, {1, 2, 3, 4, 5, 6, 7, 8}, 0},
};

#endif 