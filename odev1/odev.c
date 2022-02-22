/*
Sergen Cepoglu
02190201200
1.Ogretim 3.Sinif
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

	x = (resl - fnl)*2;
	while (x-- > 0)
		printf(" ");

	for (i = 0; i < fnl; i++)
		printf("%c ", fn[i]);
	printf("\n");

	x = (resl - snl)*2;
	while (x-- > 0)
		printf(" ");

	for (i = 0; i < snl; i++)
		printf("%c ", sn[i]);

	printf("\n");
	for (i = 0; i < resl; i++)
		printf("%c ", res[i]);

    printf("\n");
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
    
    free(cpyfn);
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
	
	memset(result, 0, sizeof(char)*SIZE);
	
	if (lenFn == 0 || lenSn == 0) {
		result[rsi] = '0';
		return result;
	}
	
	while (sni >= 0) {
		rsi = 0;
		memset(tmp, 0, sizeof(char)*SIZE);
		
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
	    
    free(tmp);
	return result;
}

int main() {
    int choice;

	char* fn;
	char* sn;
	char* res;

    char *in1 = (char*) malloc(sizeof(char)*SIZE);
    char *in2 = (char*) malloc(sizeof(char)*SIZE);

    while (1) {
        printf("\nSayi 1: ");
	    scanf("%s", in1);

        printf("Sayi 2: ");
	    scanf("%s", in2);

        if (strlen(in1) >= strlen(in2)) {
            fn = in1;
            sn = in2;
        } else {
            fn = in2;
            sn = in1;
        }

        printf("1. Topla\n2. Cikar\n3. Carp\nBir islem seciniz: ");
        scanf("%d", &choice);

        switch (choice) {
        case 1:
            res = Addition(fn, sn);
            break;
        case 2:
            res = Extraction(fn ,sn);
            break;
        case 3:
            res = Multiplication(fn ,sn);
            break;
        default:
            printf("Try again\n");
            continue;
        }

        Show(fn, sn, res);

        memset(in1, 0, sizeof(char)*SIZE);
        memset(in2, 0, sizeof(char)*SIZE);
        memset(res, 0, sizeof(char)*SIZE);
    }


    return 0;
}
