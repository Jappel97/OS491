#include "types.h"

#define PI 	3.1415926535897
#define E 	2.7182818284590
#define LN2 0.6931471806771875
#define NaN 0x7FF8000000000000

double trigSin(double theta);
double trigCos(double theta);
double trigTan(double theta);
double intPow(double base, int power);
int factorial(unsigned int N);
double pow(double base, double power);
double exp(double x);
double ln(double x);
double log(double base, double x);
char getPrecedence(char op);
double sum(int List);
int countList(int List);
double mean(int List);
double Q1(int List);
double Q3(int List);
double median(int List);
double stdDiv(int List);
void sortList(int List);
UnorderedPair linearRegression(int L1, int L2);
double sqrt(double num);
