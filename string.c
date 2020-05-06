#include "string.h"

char* strncat(char* dest, const char* src, int n){
   int dest_len = strlen(dest);
   int i;

   for (i = 0 ; i < n && src[i] != '\0' ; i++)
	   dest[dest_len + i] = src[i];
   dest[dest_len + i] = '\0';

   return dest;
}

char* strcat(char* dest, const char* src){
	return strncat(dest, src, strlen(src) + 1);
}

char* strcharcat(char* dest, char src){
   int dest_len = strlen(dest);
   dest[dest_len + 0] = src;
   dest[dest_len + 1] = '\0';

   return dest;
}

char* strncpy(char* dest, const char* src, int n)
{
   int i;

   for (i = 0; i < n && src[i] != '\0'; i++)
	   dest[i] = src[i];
   for ( ; i < n; i++)
	   dest[i] = '\0';

   return dest;
}

char* strcpy(char* dest, const char* src){
	return strncpy(dest, src, strlen(src) + 1);
}

int strlen(const char* str){
	int i = 0;
	while(str[i] != '\0'){
		i++;
	}
	return i;
}


//3 -1 1 4 : 4
//3 
//4 1
double atod(char* src){
	int temp = strlen(src);
	int Digits[temp];
	int result = 0;
	double temp2 = 0;
	int sign;
	unsigned char decFound = 0;
	if(src[0] == '_'){
		sign = -1;
	}
	else{
		sign = 1;
	}
	for(int i = 0; i < temp; i++){
		if(src[i] == '.'){
			Digits[i] = -1;
		}
		else if(src[i]>= 48 || src[i] <= 57){
			Digits[i] = src[i]-48;
		}
	}
	for(int i = 0; i < temp; i++){
		if(Digits[i] != -1 && decFound == 0){
			result *= 10;
			result += Digits[i];
		}
		else if(Digits[i] == -1 && decFound == 0){
			decFound = 1;
		}
		else if(decFound == 1){
			temp2 *= 10;
			temp2 += Digits[i];
		}
	}
	while(temp2 >= 1){
		temp2 /= 10;
	}
	return sign * (temp2 + (double)result);
}

int strcmp(const char *s1, const char *s2){
	return strncmp(s1, s2, strlen(s2));
}

//Returns 1 if the strings are the same, 0 otherwise
int strncmp(const char *s1, const char *s2, int n){
	if(strlen(s1) == strlen(s2)){
		for(int i = 0; i < n; i++){
			if(s1[i] == s2[i]){
				continue;
			}
			else{
				return 0;
			}
		}
		return 1;
	}
	else{
		return 0;
	}
}
