#include <stdio.h>

char *ft_strcat(char *dest, const char *src)
{
	char *temp;

	temp = dest;
	while (*temp)
		temp++;
	while (*src)
		*temp++ = *src++;
	*temp = '\0';

	return dest;
}
// int main()
// {
// 	char res[20] = "hello\n";

// 	strcat(res, "world");
// 	printf("%s", res);
// 	return 0;
// }
