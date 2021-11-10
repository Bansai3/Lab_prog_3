#define _CRT_SECURE_NO_WARNINGS
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#define LENGTH 1024

typedef struct tm Time;

int getStatusCode(char* str)
{
	char* ptr = strrchr(str, '\"');
	ptr++;
	char num[4];
	return atoi(strncpy(num, ptr, 4));
}

Time getTime(char* str)
{
	char* monthName[12] = { "Jan", "Feb", "Mar", "Apr",
		"May", "Jun", "Jul", "Aug", "Sen", "Okt", "Nov", "Dec" };

	Time time;
	char* ptr = strrchr(str, '[');
	ptr++;
	char mon[4] = { 0 };
	sscanf(ptr, "%d/%3c/%d:%d:%d:%d",
		&time.tm_mday, &mon, &time.tm_year,
		&time.tm_hour, &time.tm_min, &time.tm_sec);
	for (size_t i = 0; i < 12; i++)
		if (strcmp(mon, monthName[i]) == 0)
		{
			time.tm_mon = i;
			break;
		}
	time.tm_isdst = 0;
	time.tm_year -= 1900;
	return time;
}

void printMaxRequests(Time* timeStamps, int size, int duration)
{
	int max = 0;
	Time start;
	Time finish;
	int i = 0;
	int iBegin = 0, iEnd = 0;
	while (iEnd < size)
	{
		if (difftime(mktime(&timeStamps[iEnd]), mktime(&timeStamps[iBegin])) <= duration)
		{
			iEnd++;
		}
		else
		{
			if (iEnd - iBegin > max)
			{
				max = iEnd - iBegin;
				start = timeStamps[iBegin];
				finish = timeStamps[iEnd];
			}
			iBegin++;
		}
	}

	char strTimeStart[80], strTimeEnd[80];
	strftime(strTimeStart, 80, "%d/%b/%G:%H:%M:%S", &start);
	strftime(strTimeEnd, 80, "%d/%b/%G:%H:%M:%S", &finish);
	printf("Max requests was from %s to %s : %d requests", strTimeStart, strTimeEnd, max);
}


int main()
{
	FILE* file = fopen("access_log_Jul95", "r");
	char str[LENGTH] = { 0 };
	int errorCount = 0;
	int duration;
	Time* timeStamps = (Time*)malloc(sizeof(Time) * 1);
	int size = 0;
	while (fgets(str, LENGTH, file))
	{
		if (getStatusCode(str) / 100 == 5)
			errorCount++;
		timeStamps = (Time*)realloc(timeStamps, (sizeof(Time) * (size + 1)));
		timeStamps[size] = getTime(str);
		size++;
	}


	printf("Enter duration: ");
	scanf("%d", &duration);
	printMaxRequests(timeStamps, size, duration);


	printf("\n5xx error count: %d", errorCount);
	fclose(file);
	free(timeStamps);
	return 0;
}