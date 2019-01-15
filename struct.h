#include<stdlib.h>
#include<stdio.h>
#include<string.h>

typedef struct _game{
	int game_state; // de 0 a 15 (15 c'est le million!)
	int le_50;
	int le_25; // les 3 jokers
	int swap; 
	int score;
	//todo	
}game;

typedef struct _question{
	int id;
	int diff;
	int answer;
	//todo
}question;	

typedef struct _user{
	char username[30];
	char passwd[30];
	int admin; //  previleges
}user;