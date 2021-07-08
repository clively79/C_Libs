/***************************************************************
	Given pointers to two strings, both strings are combined 
	and a pointer to the new string is returned.  the new string
	is NULL terminated */
char* strcat(char* str1, char* str2)
{
	int a=strlen(str1);
	int b=strlen(str2);
	int sum = a+b;
	int i=0;
	char* temp = (char*) malloc( (sum+1)*sizeof(char));
	strcpy(temp, str1);
	while(a<sum)
		temp[a++] = str2[i++];
	
	temp[strlen(temp)] = NULL;
	return temp;
}