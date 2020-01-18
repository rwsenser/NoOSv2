//
// atoi: loosely based on url http://geeksforgeeks.org/write-your-own_atoi
//
int atoi(char* str) {
	int res = 0;
	int sign = 1;
	int i=0;
	for(;str[i] != '\0';i++) {
		if (str[i] != ' ') break;
	}
	if (str[i] == '-') {
		sign = -1;
		i++;
	}
	for (;str[i] != '\0';i++){
		if (str[i] < '0' || str[i] > '9') break;
		res = res * 10;
		res = res + ((int) str[i]) - ((int) '0');
	}
	if (sign < 0) res *= -1;
	return res;
}
