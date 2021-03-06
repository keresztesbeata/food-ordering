//
// Created by q on 11/6/2019.
//

#include "input_from_customer.h"
#include "display_order.h"
#include <stdlib.h>
#include <stdio.h>
#include <stdbool.h>
#include<string.h>

#define FILE_PATH "../accounts.txt"

#define MAX_USERNAME 30
#define MAX_PASSWORD 30

void displaySignOptions () {
    printf("Welcome to Food Thingies!\n");
    printf("%s\n",SIGN_IN_UP);
    printf("a) %s\n",SIGN_IN);
    printf("b) %s\n",SIGN_UP);
}

void signUp(FILE *loginFile, char username[], char password[], int *state, int *goOn ) {
    //open file for reading+modifying
    loginFile=fopen(FILE_PATH,"r+");
    if(loginFile==NULL) {
        perror("File could not be open");
        exit(-1);
    }
    int key;
    //read key
    fscanf(loginFile,"%d",&key);
    //save position of the file_pointer, to know where to return later
    int cur_position=ftell(loginFile)+1;
    //read no of users
    int noOfUsers;
    fscanf(loginFile,"%d",&noOfUsers);
    fgetc(loginFile);   //read new line
    printf("%s\n",SIGNING_UP);
    printf("---Username:\n>");
    gets(username);
    char matchingPassword[MAX_PASSWORD];
    int conditionUsername=validateUsername(loginFile, noOfUsers, username, matchingPassword);
    //decrypt password
    decrypt_pass(key,matchingPassword);
    if(!conditionUsername) {
        printf("---Password:\n>");
        gets(password);
        int conditionPassword = validatePassword(password, matchingPassword, username);
        if(conditionPassword==2) {
            //encrypt password
            encrypt_pass(key,password);
            noOfUsers++;
            fseek(loginFile,cur_position,SEEK_SET);
            fprintf(loginFile,"%d",noOfUsers);
            fseek(loginFile,0,SEEK_END);
            fprintf(loginFile,"\r%s %s",username,password);
            (*state)++;//increments the state
            *goOn=0;//comes out of the while loop
        }
        else
            incorrectNewPassword(password,username);
    }
    else
        printf("%s\n",DUPLICATE_USER);
    //close file
    fclose(loginFile);
}
void signIn(FILE *loginFile, char username[], char password[], int *state, int *goOn ) {
    //open file for reading
    loginFile=fopen(FILE_PATH,"r");
    if(loginFile==NULL) {
        perror("File could not be open");
        exit(-1);
    }
    int key;
    fscanf(loginFile,"%d",&key);//read key
    int noOfUsers;
    fscanf(loginFile,"%d",&noOfUsers);//read no of users
    fgetc(loginFile);//read new line
    printf("%s\n", SIGNING_IN);
    printf("---Username:\n>");
    gets(username);
    char matchingPassword[MAX_PASSWORD];
    int conditionUsername = validateUsername(loginFile, noOfUsers, username,matchingPassword);
    //decrypt password
    decrypt_pass(key,matchingPassword);
    if (conditionUsername) {
        printf("---Password:\n>");
        gets(password);
        int conditionPassword = validatePassword(password, matchingPassword, username);
        if (conditionPassword==1) {
            (*state)++;//increment the state
            *goOn = 0;//comes out of the while loop
        } else
            printf("%s\n", INCORRECT_PASSWORD);
    }
    else {
            printf("%s\n", USER_NOT_FOUND);
            *goOn = 0;//comes out of the while loop
        }
    //close file
    fclose(loginFile);
}

void login(char username[], char password[], int *state) {
    FILE *loginFile;
    displaySignOptions();
    char signChoice=getchar();
    getchar();//new line
    switch (signChoice) {
        case 'a': {
            int goOn = 1;
            while (goOn)
                signIn(loginFile,username, password, state, &goOn);
            break;
        }
        case 'b':{
            int goOn = 1;
            while (goOn)
                signUp(loginFile,username, password, state, &goOn);
            break;
        }
        default: break;
    }
}
void decrypt_pass(int key, char password[]) {
    //decrypt by substracting the 'key' integer from each character
    int j=0;
    while(password[j]!='\0') {
        password[j]=password[j]-key;
        j++;
    }
}
void encrypt_pass(int key, char password[]) {
    //encrypt by adding the 'key' integer to each character
    int j=0;
    while(password[j]!='\0') {
        password[j]=password[j]+key;
        j++;
    }
}
int validateUsername(FILE *loginFile, int noOfUsers, char username[], char matchingPassword[]) {
    char oldUsername[MAX_USERNAME],oldPassword[MAX_PASSWORD];
    for(int i=0;i<noOfUsers;i++) {
        fscanf(loginFile,"%s %s",oldUsername,oldPassword);//read username from file
        //identify username
        if (strcmp(oldUsername,username)==0) {
            strcpy(matchingPassword,oldPassword);//save the corresponding password in case username coincides
           return 1;
        }
    }
    return 0;
}
int validatePassword(char password[], char matchingPassword[], char username[]) {
   if(strcmp(matchingPassword,password)==0)
       return 1;
    else if(isLongEnough(password) && notContainUsername(password,username) && containSpecialCharacters(password) && containDigits(password))
        return 2; // valid password
    return 0;
}
int isLongEnough(char password[]) {
    if(strlen(password) >= 7)
        return 1;
    return 0;
}
int notContainUsername(char password[],char username[]) {
    if(!strstr(password,username))
        return 1;
    return 0;
}
int containSpecialCharacters(char password[]) {
    if(strpbrk(password,".,_!"))
        return 1;
    return 0;
}
int containDigits(char password[]) {
    if(strpbrk(password,"01234556789"))
        return 1;
    return 0;
}

void incorrectNewPassword(char password[], char username[]) {
    if(!isLongEnough(password)) printf("%s\n",ERROR_PASSWORD_LONG);
    if(!notContainUsername(password, username)) printf("%s\n",ERROR_PASSWORD_NOT_USERNAME);
    if(!containSpecialCharacters(password)) printf("%s\n",ERROR_PASSWORD_SPECIAL_CHAR);
    if(!containDigits(password)) printf("%s\n",ERROR_PASSWORD_DIGITS);
}

int getChoiceIndex(int nrChoices, int *state) {
    int customerChoice;
    char choice=getchar();
    getchar();
    if(choice=='a'+nrChoices)
        (*state)--;
    else{
        customerChoice=choice-'a';
        (*state)++;
    }
    return customerChoice;
}

int getOptionalChoiceIndex(int nrChoices, int *state) {
    int chosenElement;
    int choice=getchar();
    getchar();
    if(choice=='a'+nrChoices+1)
        (*state)--;
    else {
        if (choice =='a'+nrChoices)
            chosenElement=-1;  //if the customer chose nothing the function returns negative value
        else
            chosenElement=choice - 'a';
        (*state)++;
    }
    return chosenElement;
}

int chooseCutlery (int *state) {
    int wantCutlery;
    int choice=getchar();
    getchar();
    if(choice=='c') {
        (*state)--;
        wantCutlery = -1;
    }
    else {
        if(choice=='b') wantCutlery=0;
        if(choice=='a') wantCutlery=1;
        (*state)++;
    }
    return wantCutlery;
}

void getAdditionalInfo(char addInfo[], int *state) {
    printf("Any additional info?\n>");
    gets(addInfo);
    (*state)++;
}

void placeOrder(bool *confirm, char username[], int *state) {
    displayOrderConfirm();
    int choice=getchar();
    getchar();
    if(choice=='a') {
        printf("You order has been registered. Thank you for buying from us, %s!\n", username);
        *confirm=true;
    }
    else {
        if (choice=='b')
            (*state)-=2;     //it brings us back to the cutlery option
        else
            getchar();
    }
}
