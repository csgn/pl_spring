/*
Sergen Cepoglu
02190201200
1.Ogretim 3.Sinif
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

#define SIZE 10000

int parseInt(char n) {
	return n - '0';
}

int parseChar(int n) {
	return n + '0';
}

void Reverse(char* str) {
	int len = strlen(str);
	int i = 0;
	while (len > i) {
		char tmp = str[--len];
		str[len] = str[i];
		str[i++] = tmp;
	}
}

void Show(char *fn, char* sn, char* res) {
	int fnl, snl, resl;
	int x, i;
	fnl = strlen(fn);
	snl = strlen(sn);
	resl = strlen(res);

	for (i = 0; i < fnl; i++)
		printf("%c", fn[i]);
	printf("\n");

	x = (fnl - snl)*2;
	while (x-- > 0)
		printf(" ");

	for (i = 0; i < snl; i++)
		printf("%c", sn[i]);

	printf("\n");
	for (i = 0; i < resl; i++)
		printf("%c", res[i]);
}

char* Addition(char* fn, char* sn) {
	int s = 0, c = 0;
	int i = strlen(fn)-1, j = strlen(sn)-1, k = 0;
	int fni, sni;
	char* res = (char*) malloc(sizeof(char)*SIZE);
	
	while (i >= 0 || j >= 0) {
		sni = j < 0 ? 0 : parseInt(sn[j--]);
		fni = i < 0 ? 0 : parseInt(fn[i--]);
		s = fni + sni + c;
		c = s / 10;
		res[k++] = parseChar(s%10);
	}
	
	
	while (c > 0) {
		res[k++] = parseChar(c % 10);
		c /= 10;
	}
	
	
	Reverse(res);
	return res;
}

char* Extraction(char* fn, char* sn) {
	int c = 0, r = 0;
	int i = strlen(fn)-1, j = strlen(sn)-1, k = 0;
	int fni, sni;
	char* res = (char*) malloc(sizeof(char)*SIZE);

	char* cpyfn = (char*) malloc(sizeof(char)*SIZE);
	memcpy(cpyfn, fn, sizeof(char)*SIZE);

	while (i >= 0 || j >= 0 ) {
		sni = j < 0 ? 0 : parseInt(sn[j]);
		fni = i < 0 ? 0 : parseInt(cpyfn[i]);

		if (fni < sni) {
			int tmp = i-1;

			while (parseInt(cpyfn[tmp]) == 0) {
				tmp--;
			}

			cpyfn[tmp] = parseChar(parseInt(cpyfn[tmp]) - 1);

			while (++tmp < i) {
				if (parseInt(cpyfn[tmp]) == 0) {
					cpyfn[tmp] = parseChar(9);
				}
			}

			int t = parseInt(cpyfn[tmp])+10 - sni;
			res[k] = parseChar(t);
		} else {
			res[k] = parseChar(fni-sni);
		}

		i--;
		j--;
		k++;
	}

	Reverse(res);
	return res;
}

char* Multiplication(char* fn, char* sn) {
	int lenFn = strlen(fn);
	int lenSn = strlen(sn);
	
	int fni = lenFn-1;
	int sni = lenSn-1;
	int rsi = 0;
	
	char* result = (char*) malloc(sizeof(char)*SIZE);
	char* tmp = (char*) malloc(sizeof(char)*SIZE);
	
	memset(result, NULL, sizeof(char)*SIZE);
	
	if (lenFn == 0 || lenSn == 0) {
		result[rsi] = '0';
		return result;
	}
	
	while (sni >= 0) {
		rsi = 0;
		memset(tmp, NULL, sizeof(char)*SIZE);
		
		int snel = parseInt(sn[sni]);
		int carry = 0;
		int sum = 0;
		
		fni = lenFn - 1;
		
		while (fni >= 0) {
			int fnel = parseInt(fn[fni--]);
			
			sum = fnel*snel + carry;
			
			if (sum >= 10) {
				carry = sum/10;
			} else {
				carry = 0;
			}
			
			tmp[rsi++] = parseChar(sum % 10);
		}

		while (carry > 0) {
			tmp[rsi++] = parseChar(carry % 10);
			carry /= 10;
		}
		
		Reverse(tmp);
		if (strlen(result) != 0) {
			int i = (lenSn-1)-sni;
			while (i > 0) {
				tmp[rsi++] = parseChar(0);
				i--;
			}
			
			result = Addition(result, tmp);
		} else {
			memcpy(result, tmp, sizeof(char)*SIZE);
		}
		
		sni--;
	}
	
	return result;
}




int main() {
	int fns, sns;
	char* fn = (char*) malloc(sizeof(char)*SIZE);
	char* sn = (char*) malloc(sizeof(char)*SIZE);
	char* res = (char*) malloc(sizeof(char)*SIZE);

	scanf("%s%s", fn, sn);

	res = Multiplication(fn, sn);
	//res = Addition(fn, sn);
	
	printf("res=%s\n", res);
	//Show(fn, sn, res);

	return 0;
}
