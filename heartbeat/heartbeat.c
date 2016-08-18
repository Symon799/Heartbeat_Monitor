#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>
#include <time.h>
#include <conio.h>

FILE* file = NULL;
FILE* pass = NULL;
char user[10];

/*Simon Rigard*/

void log(int state, int heart) //function to print in the log
{
	file = fopen("log.txt", "a");
	if (state == 5)	//fprintf bpm
		fprintf(file, "%s bpm : %d \n", user, heart);
	if (state == 6)
		fprintf(file, "%s bpm : %d  ALERT !!!\n", user, heart);
	else
	{
		time_t rawtime;			//to print the time in the logfile
		time(&rawtime);
		struct tm *info = localtime(&rawtime);

			if (state == 1)		//user logged
				fprintf(file, "%s%s logged\n", asctime(info), user);
			if (state == 2)		//paswword change
				fprintf(file, "%s%s changed is password\n", asctime(info), user);
			if (state == 3)		//simulation of state
				fprintf(file, "%s%s simulated the state %d\n", asctime(info), user, heart);
			if (state == 4)		//user logged out
				fprintf(file, "%s%s logged out\n", asctime(info), user);
	}
	fclose(file);
}

void bpm(int j)		//calculate the bpm
{
	system("cls");

	printf("\n  Simulating state %d\n\n", j);
	int peakNb = 0;		//nb of peaks
	int decrease = 0;	//number of consecutuve decrease
	double prev = 0;	//previous value
	double tab[13];		//tab to get the data line by line

	int i = 0;
	file = fopen("data.txt", "r");

	while (!_kbhit())
	{
		if (i > 7661)		//restart when the file is finished
		{
			i = 0;
			file = fopen("data.txt", "r");
		}

		fscanf(file, "%lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf %lf", &tab[0], &tab[1],
			&tab[2], &tab[3], &tab[4], &tab[5], &tab[6], &tab[7],
			&tab[8], &tab[9], &tab[10], &tab[11], &tab[12]); //we put the line in a tab and take only the one we want corresponding to the state
		i++;

		double current = tab[j];

		if (prev > current)		//if the values are decreasing
			decrease++;
		else
			decrease = 0;

		if (current > 2 && decrease > 1)	//if the value is greater than 2 and the values are decreasing
		{
			peakNb++;
		}

		Sleep(10);

		if (i % 500 == 0 && i != 0)		//we do bpm : peak * 12 each 5 secs
		{
			int bpmin = peakNb * 12;
			if (bpmin > 100 || bpmin < 40)	//if bpm is in a critical state
			{
				FILE *file2 = fopen("log.txt", "a");
				fprintf(file2, "%s bpm : %d ALERT !!!\n", user, bpmin);
				printf("bpm : %d ALERT !!!\n", bpmin);
				fclose(file2);
			}
			else
			{
				FILE *file2 = fopen("log.txt", "a");
				fprintf(file2, "%s bpm : %d \n", user, bpmin);
				printf("bpm : %d\n", bpmin);
				fclose(file2);
			}
			peakNb = 0;
		}
		prev = tab[j];	//previous value
	}
	printf("\n");
	fclose(file);
}

void login()	//to authenticate the user with the pass.txt file
{
	system("cls");
	char password[10];
	char Ruser[10], Rpassword[10];

	printf("        Welcome to Symon's heartbeat program\n\n");
	file = fopen("pass.txt", "r");

	do
	{
		printf("Please enter your username : ");
		scanf("%s", &user);
		FILE *file2 = NULL;
		file2 = fopen("pass.txt", "r");
		while(strcmp(user, Ruser) != 0 && fscanf(file2, "%s %s", &Ruser, &Rpassword) != EOF);	//put the first word in Ruser and the second one in Rpassword
		fclose(file2);
	} while (strcmp(user, Ruser) != 0); //continue until the user is found on the pass.txt
	
	printf("Password : ");
	scanf("%s", password);

	fclose(file);

	if (strcmp(user, Ruser) == 0 && strcmp(password, Rpassword) == 0)	//good password
	{
		system("cls");
	}
	else
		login();		//wrong password

	log(1, 0);		//print in logfile
}

void changePass() //call this fonction tho change the current user password
{
	system("cls");
	FILE* file3 = NULL;		//To delete one line in c we must copy the whole txt in another txt and then rename it and delete the first one
	FILE* file2 = NULL; 
	char Ruser[10];			//password of the pass file (database)
	char Rpassword[10];		//user of the pass file (database)
	
	file = fopen("pass.txt", "r+");
	file2 = fopen("pass2.txt", "w");

	while (fscanf(file, "%s %s", &Ruser, &Rpassword) != EOF)
	{
		if(strcmp(user, Ruser) != 0)					//if this is the line we want to delete, we skip it
			fprintf(file2, "%s %s\n", Ruser, Rpassword);
	}

	printf("\nEnter new Password :");
	scanf("%s", &Rpassword);
	fprintf(file2, "%s %s", user, Rpassword);	// print the new line with the user and new password
	fclose(file2);
	fclose(file);

	char oldname[] = "pass2.txt";
	char newname[] = "pass.txt";
	remove(newname);	// remove the old pass.txt
	rename(oldname, newname);	//rename the new one to pass.txt
	log(2, 0);	//print password change in logfile
}

void seelog()		//Call this function to print the logfile, 
{
	system("cls");
	FILE *file;
	char filename[100], c;
	printf("    -----STARTLOG----");
	file = fopen("log.txt", "a+");

	c = fgetc(file);
	printf("\n");
	while (c != EOF)	//print until end of file
	{
		printf("%c", c);
		c = fgetc(file);
	}
	printf("    -----ENDLOG------\n");

	fclose(file);
	system("pause");
}

int main(int argc, char *argv[]) //main function, main menu
{	
	int loop = -1;
	login();
	while (loop != 0)	//while user has not entered 0
	{
		if (loop == 14)
		{
			log(4, 0);	//print in logfile
			login();
		}
		system("cls");
		printf("\nWelcome %s!", user);
		if (strcmp(user, "admin") != 0)
			printf("\nWhat state would you like to simulate ?\n\n  (0) To quit\n  (1-12) Select state\n  (13) To change Password\n  (14) To logout\n");
		else
			printf("\nWhat state would you like to simulate ?\n\n  (0) To quit\n  (1-12) Select state\n  (13) To change Password\n  (14) To logout\n  (15) To see log\n  (16) Reset log\n");
		int state;
		
		while (scanf("%d", &state) != 1)	//fix errors when user uses letters instead of numbers
		{
			system("cls");
			printf("\nWelcome %s!", user);
			if (strcmp(user, "admin") != 0)
				printf("\nWhat state would you like to simulate ?\n\n  (0) To quit\n  (1-12) Select state\n  (13) To change Password\n  (14) To logout\n");
			else
				printf("\nWhat state would you like to simulate ?\n\n  (0) To quit\n  (1-12) Select state\n  (13) To change Password\n  (14) To logout\n  (15) To see log\n  (16) Reset log\n");
			int c;
			while ((c = getchar()) != '\n' && c != EOF);
		}

		loop = state;
		if (loop == 13)
			changePass();
		if (loop == 15 && strcmp(user, "admin") == 0)	//only available for admin
			seelog();
		if (loop == 16 && strcmp(user, "admin") == 0)	//only available for admin
			remove("log.txt");
		if (loop != 0 && loop < 13)
		{
			log(3, state);	//print in logfile
			bpm(state);
		}

	}
	log(4, 0);	//print in logfile
	return 0;
}

