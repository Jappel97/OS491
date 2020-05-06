#include "math.h"
#include "univals.h"
#include "frame.h"
#include "types.h"

double trigCos(double theta){
	double result = 0;
	int sign = -1;
	double denom, temp;
	for(int i = 0; i < 10; i++){
		sign *= -1;
		denom = factorial(2 * i);
		temp = intPow(theta, 2*i);
		result += ((sign/denom) * temp);
	}
	return result;
}

double trigSin(double theta){
	double result = 0;
	int sign = -1;
	double denom, temp;
	for(int i = 0; i < 10; i++){
		sign *= -1;
		denom = factorial((2 * i) + 1);
		temp = intPow(theta, (2*i) + 1);
		result += ((sign/denom) * temp);
	}
	return result;
}

double trigTan(double theta){
	return trigSin(theta)/trigCos(theta);
}

double intPow(double base, int power){
	double total = 1;
	for(int i = 0; i < power; i++){
		total *= base;
	}
	return total;
}


//EXAMPLE: 2^3.1
//First, convert to e^2*log(3.1)
//Second, calculate log(3.1)
//Third, multiply that by two
//Fourth, raise that on e.
//exp(x)=limn→∞(1+x+x^2/2!+x^3/3!+...+xnn!)
double pow(double base, double power){
	double temp1 = ln(power);
	temp1 *= base;
	return exp(temp1);
}

//exp(x)=limn→∞(1+x+x^2/2!+x^3/3!+...+xnn!) x ^ 0 / 0!, x ^ 1/ 1!
double exp(double x){
	double result = 1 + x;
	int itterations = 10;
	double currentPow = x;
	double currentFac = 1;
	double temp;
	for(int i = 2; i < itterations; i++){
		currentPow *= x;
		currentFac *= i;
		temp = currentPow/currentFac;
		result += temp;
	}
	return result;
}

// = 2 [ (x-1)/(x+1)  + (1/3)( (x-1)/(x+1) )3 + (1/5) ( (x-1)/(x+1) )5 + (1/7) ( (x-1)/(x+1) )7 + ... ] 
double ln(double x){
	if(x < 0){
		return NaN;
	}
	else{
		double core = (x-1)/(x+1);
		double result = core;
		int temp1;
		double temp2, temp3;
		//Change the number of itterations to change the accuracy.
		int itterations =  10;
		//We avoid deviding by zero. Because we aren't monsters.
		for(int i = 1; i < itterations; i++){
			temp1 = (2 * i) + 1;
			temp2 = intPow(core, temp1);
			temp3 = temp2 / temp1;
			result += temp3;
		}
		return 2 * result;
	}
}


int factorial(unsigned int N){
	if(N > 0){
		return N * factorial(N - 1);
	}
	else{
		return 1;
	}
}

char getPrecedence(char op){
	if(op == '-' || op == '+'){
		return 0;
	}
	else if(op == '*' || op == '/'){
		return 1;
	}
	else if(op == '^'){
		return 2;
	}
	else{
		return 10;
	}
}

//Logb x = Loga x/Loga b
double log(double base, double x){
	return ln(x)/ln(base);
}


double sum(int List){
	thing temp = lists[List][0];
	double sum = 0;
	int i = 1;
	while(temp.type == 1 && i < 256){
		sum += temp.value;
		temp = lists[List][i];
		i++;
	}
	return sum;
}

int countList(int List){
	int count = 0;
	thing temp = lists[List][0];
	int i = 1;
	while(temp.type == 1 && i < 256){
		count++;
		temp = lists[List][i];
		i++;
	}
	return count;
}

double mean(int List){
	double total = sum(List);
	int count = countList(List);
	return total/count;
}

double Q1(int List){
	sortList(List);
	int count = countList(List)/2;
	double temp1, temp2;
	if(count%2 == 0){
		temp1 = lists[List][count/2].value;
		temp2 = lists[List][count/2].value;
		temp1 = (temp1 + temp2)/2;
	}
	else{
		temp1 = lists[List][count/2+1].value;
	}
	return temp1;
}

double Q3(int List){
	sortList(List);
	int count = countList(List)/2;
	double temp1, temp2;
	if(count%2 == 0){
		temp1 = lists[List][(count/2) + count].value;
		temp2 = lists[List][(count/2) + count + 1].value;
		temp1 = (temp1 + temp2)/2;
	}
	else{
		temp1 = lists[List][(count/2) + count + 1].value;
	}
	return temp1;
}

double median(int List){
	sortList(List);
	int count = countList(List);
	double temp1, temp2;
	if(count%2 == 0){
		temp1 = lists[List][count/2].value;
		temp2 = lists[List][count/2+1].value;
		temp1 = (temp1 + temp2)/2;
	}
	else{
		temp1 = lists[List][count/2+1].value;
	}
	return temp1;
}

double stdDiv(int List){
	double average = mean(List);
	int count = countList(List);
	double temp1, temp2, temp3 = 0;
	for(int i = 0; i < count; i++){
		temp1 = lists[List][i].value;
		temp2 = temp1 - average;
		temp1 = temp2 * temp2;
		temp3 += temp1;
	}
	temp3/=count;
	return sqrt(temp3);
}

void sortList(int List){
	int count = countList(List);
	int max = count - 1;
	char sorted = 0, swaps = 0;
	thing temp;
	while(!sorted){
		for(int i = 0; i < max; i++){
			if(lists[List][i].value > lists[List][i + 1].value){
				temp = lists[List][i];
				lists[List][i] = lists[List][i + 1];
				lists[List][i + 1] = temp;
			}
		}
		max--;
		if(!swaps){
			sorted = 1;
		}
	}
}

//point (x¯,y¯) and has gradient ∑ni=1(xi−x¯)(yi−y¯)∑ni=1(xi−x¯)2.
UnorderedPair linearRegression(int L1, int L2){
	double xBar = mean(L1);
	double yBar = mean(L2);
	int count = countList(L1);
	thing temp1, temp2;
	double temp3, temp4;
	double topSum = 0, bottomSum = 0;
	for(int i = 0; i < count; i++){
		temp1 = lists[L1][i];
		temp2 = lists[L1][i];
		temp3 = temp1.value - xBar;
		temp4 = temp2.value - yBar;
		topSum += (temp3) * (temp4);
		bottomSum += temp3 * temp3;
	}
	double slope = topSum/bottomSum;
	double intercept = yBar - (slope * xBar);
	//Figure out how to return these
	return (UnorderedPair){slope, intercept};
}

double sqrt(double num){
	double x0 = num/2;
	double temp1, temp2;
	for(int i = 0; i < 10; i++){
		temp1 = num/x0;
		temp2 = temp1 + x0;
		temp2 /= 2;
		x0 = temp2;
	}
	return x0;
}
