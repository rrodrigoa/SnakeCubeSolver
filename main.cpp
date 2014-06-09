#include <cstdlib>
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <algorithm>
#include <sstream>
#include <iostream>
#include <vector>
#include <queue>
#include <stack>
#include <set>
#include <tuple>
#include <bitset>

#define XDIR 1
#define YDIR 2
#define ZDIR 3
#define MXDIR 4
#define MYDIR 5
#define MZDIR 6

using namespace std;

bool calcHamiltonian(int xx, int yy, int zz, int dir, vector<vector<vector<bool>>> data, int xsize, int ysize, int zsize, string snake, int index, vector<tuple<int,int,int>> *stackTemp);
void printStack(vector<tuple<int,int,int>> stackTemp);

int snake_cube_solution(int x, int y, int z, string snake)
{
bool anyZeroCord = (x == 0 || y == 0 || z == 0);
bool hasOneOneCord = (x == 1 || y == 1 || z == 1);
bool hasTwoOneCord = (x == 1 && y == 1) || (x == 1 && z == 1) || (y == 1 && z == 1);

bool stringHasRightSize = (snake.size() == x*y*z);
bool stringStartsAndEndsRight = (snake[0] == '0' && snake[snake.size()-1] == '0');
bool stringHasRightFormat = true;
for(int index = 0; index < snake.size(); index++){
if(snake[index] != '0' && snake[index] != '1'){
stringHasRightFormat = false;
}
}

bool twoSequencesOfKMinusOneSize = false;
bool sequenceKMinusOneSizeWrongPos = false;
bool threeSequencesOfKMinusOneSize = false;
int k = max(x, max(y, z));
string zeroSec;
for(int index = 0; index < k-1; index++){
zeroSec.append("0");
}
int kMinusOneSeqPos = snake.find(zeroSec, 0);
int secMinusOneSeqPos = snake.find(zeroSec, kMinusOneSeqPos+1);
if(kMinusOneSeqPos != -1 && secMinusOneSeqPos != -1){
twoSequencesOfKMinusOneSize = true;
}
if(twoSequencesOfKMinusOneSize && secMinusOneSeqPos != -1 && snake.find(zeroSec, secMinusOneSeqPos+1) != -1){
threeSequencesOfKMinusOneSize = true;
}
if(twoSequencesOfKMinusOneSize && (kMinusOneSeqPos != 0 || secMinusOneSeqPos != snake.size()-(k-1)) ){
sequenceKMinusOneSizeWrongPos = true;
}

bool isZigZag = true;
for(int index = 0; index < snake.size(); index++){
if(index == 0 || index == snake.size()-1){
if(snake[index] != '0'){
isZigZag = false;
break;
}
}else if(snake[index] != '1'){
isZigZag = false;
break;
}
}

// No zero dimentions
if(anyZeroCord || // no cordinates are zero
stringHasRightSize == false || // needs to have right number of integers on string
stringStartsAndEndsRight == false || // starts and ends with 0
stringHasRightFormat == false || // only has 0 and 1
threeSequencesOfKMinusOneSize == true || // no more than 2 sequences of k-1 size of zeros, where k is max(x, y, z)
(twoSequencesOfKMinusOneSize && sequenceKMinusOneSizeWrongPos) // has only 2 k-1 size of zeros and needs to be at begining and end only
){	
return 0;
}else if(isZigZag){ // if it is a zigzag snake
bool allAreOdd = (x % 2 != 0) && (y % 2 != 0) && (z % 2 != 0);
bool allAreGEThan3 = (x >= 3) && (y >= 3) && (z >= 3);
if(allAreOdd && allAreGEThan3){ // if all are odds and >= 3 there is no solution
return 0;
}
}

// Compute hamiltoniam path	
vector<bool> walkedZ(z, false);
vector<vector<bool>> walkedY(y, walkedZ);
vector<vector<vector<bool>>> walked(x, walkedY);

int index = 0;

// initialize available pos and available direction to pos 0	
int maxX = (x%2 != 0) ? x/2 + 1 : x/2;
int maxY = (y%2 != 0) ? y/2 + 1 : y/2;
int maxZ = (z%2 != 0) ? z/2 + 1 : z/2;

vector<tuple<int,int,int>> possibleStarts;
for(int indexX = 0; indexX < maxX; indexX++){
for(int indexY = 0; indexY < maxY; indexY++){
for(int indexZ = 0; indexZ < maxZ; indexZ++){
possibleStarts.push_back(tuple<int,int,int>(indexX, indexY, indexZ));
}
}
}

while(possibleStarts.size() != 0){
vector<tuple<int, int, int>> stackResult;
tuple<int,int,int> start = possibleStarts[0]; possibleStarts.erase(possibleStarts.begin());
int xx = get<0>(start);
int yy = get<1>(start);
int zz = get<2>(start);
bool value = calcHamiltonian(xx, yy, zz, XDIR, walked, x, y, z, snake, 0, &stackResult);
if(value == false){
value = calcHamiltonian(xx, yy, zz, MXDIR, walked, x, y, z, snake, 0, &stackResult);
}else if(value == false){
value = calcHamiltonian(xx, yy, zz, YDIR, walked, x, y, z, snake, 0, &stackResult);
}else if(value == false){
value = calcHamiltonian(xx, yy, zz, MYDIR, walked, x, y, z, snake, 0, &stackResult);
}else if(value == false){
value = calcHamiltonian(xx, yy, zz, ZDIR, walked, x, y, z, snake, 0, &stackResult);
}else if(value == false){
value = calcHamiltonian(xx, yy, zz, MZDIR, walked, x, y, z, snake, 0, &stackResult);
}else{
printStack(stackResult);
return 1;
}
}

// no more available pos return 0
return 0;
}

bool calcHamiltonian(int xx, int yy, int zz, int dir, vector<vector<vector<bool>>> data, int xsize, int ysize, int zsize, string snake, int index, vector<tuple<int, int, int>> *stackTemp){
if(index == snake.size()){
for(int xpos = 0; xpos < xsize; xpos++){
for(int ypos = 0; ypos < ysize; ypos++){
for(int zpos = 0; zpos < zsize; zpos++){
if(data[xpos][ypos][zpos] == false){
return false;
}
}
}
}
return true;
}else if(xx < 0 || xx >= xsize || yy < 0 || yy >= ysize || zz < 0 || zz >= zsize){
return false;
}else{
if(data[xx][yy][zz] == true){
return false;
}
data[xx][yy][zz] = true;
(*stackTemp).push_back(tuple<int,int,int>(xx, yy, zz));
if(snake[index] == '0'){
if(dir == XDIR && calcHamiltonian(xx+1, yy, zz, dir, data, xsize, ysize, zsize, snake, index+1, stackTemp) == 1){
return true;
}else if(dir == YDIR && calcHamiltonian(xx, yy+1, zz, dir, data, xsize, ysize, zsize, snake, index+1, stackTemp) == 1){
return true;
}else if(dir == ZDIR && calcHamiltonian(xx, yy, zz+1, dir, data, xsize, ysize, zsize, snake, index+1, stackTemp) == 1){
return true;
}else if(dir == MXDIR && calcHamiltonian(xx-1, yy, zz, dir, data, xsize, ysize, zsize, snake, index+1, stackTemp) == 1){
return true;
}else if(dir == MYDIR && calcHamiltonian(xx, yy-1, zz, dir, data, xsize, ysize, zsize, snake, index+1, stackTemp) == 1){
return true;
}else if(dir == MZDIR && calcHamiltonian(xx, yy, zz-1, dir, data, xsize, ysize, zsize, snake, index+1, stackTemp) == 1){
return true;	
}else{
data[xx][yy][zz] = false;
(*stackTemp).erase((*stackTemp).end()-1);
return false;
}
}else{
bool canContinue = false;
if(dir == XDIR || dir == MXDIR){
canContinue = calcHamiltonian(xx, yy-1, zz, MYDIR, data, xsize, ysize, zsize, snake, index+1, stackTemp);
if(canContinue == false){
canContinue = calcHamiltonian(xx, yy+1, zz, YDIR, data, xsize, ysize, zsize, snake, index+1, stackTemp);
}
if(canContinue == false){
canContinue = calcHamiltonian(xx, yy, zz-1, MZDIR, data, xsize, ysize, zsize, snake, index+1, stackTemp);
}
if(canContinue == false){
canContinue = calcHamiltonian(xx, yy, zz+1, ZDIR, data, xsize, ysize, zsize, snake, index+1, stackTemp);
}
if(canContinue == false){
data[xx][yy][zz] = false;
(*stackTemp).erase((*stackTemp).end()-1);
}
return canContinue;
}else if(dir == YDIR || dir == MYDIR){
canContinue = calcHamiltonian(xx-1, yy, zz, MXDIR, data, xsize, ysize, zsize, snake, index+1, stackTemp);
if(canContinue == false){
canContinue = calcHamiltonian(xx+1, yy, zz, XDIR, data, xsize, ysize, zsize, snake, index+1, stackTemp);
}
if(canContinue == false){
canContinue = calcHamiltonian(xx, yy, zz-1, MZDIR, data, xsize, ysize, zsize, snake, index+1, stackTemp);
}
if(canContinue == false){
canContinue = calcHamiltonian(xx, yy, zz+1, ZDIR, data, xsize, ysize, zsize, snake, index+1, stackTemp);
}
if(canContinue == false){
data[xx][yy][zz] = false;
(*stackTemp).erase((*stackTemp).end()-1);
}
return canContinue;
}else{
canContinue = calcHamiltonian(xx-1, yy, zz, MXDIR, data, xsize, ysize, zsize, snake, index+1, stackTemp);
if(canContinue == false){
canContinue = calcHamiltonian(xx+1, yy, zz, XDIR, data, xsize, ysize, zsize, snake, index+1, stackTemp);
}
if(canContinue == false){
canContinue = calcHamiltonian(xx, yy-1, zz, MYDIR, data, xsize, ysize, zsize, snake, index+1, stackTemp);
}
if(canContinue == false){
canContinue = calcHamiltonian(xx, yy+1, zz, YDIR, data, xsize, ysize, zsize, snake, index+1, stackTemp);
}
if(canContinue == false){
data[xx][yy][zz] = false;
(*stackTemp).erase((*stackTemp).end()-1);
}
return canContinue;
}
}
}
}

void printStack(vector<tuple<int,int,int>> stackTemp)
{
for(int index = 0; index < stackTemp.size(); index++){
printf("(%d,%d,%d)\n", get<0>(stackTemp[index]), get<1>(stackTemp[index]), get<2>(stackTemp[index]));
}
}

int main(){
if(snake_cube_solution(3, 3, 3, "001110110111010111101010100") == 1){
printf("passed\n");
}else{
printf("failed\n");
}

if(snake_cube_solution(3, 3, 3, "011111101111101011111010110") == 1){
printf("passed\n");
}else{
printf("failed\n");
}

if(snake_cube_solution(3, 3, 3, "011111101011011101110111110") == 1){
printf("passed\n");
}else{
printf("failed\n");
}

if(snake_cube_solution(3, 3, 3, "011011111110101011111101010") == 1){
printf("passed\n");
}else{
printf("failed\n");
}

if(snake_cube_solution(3, 3, 3, "011111111111111111111111110") == 0){
printf("passed\n");
}else{
printf("failed\n");
}

if(snake_cube_solution(3, 3, 3, "011111101110110111111110100") == 0){
printf("passed\n");
}else{
printf("failed\n");
}
}
