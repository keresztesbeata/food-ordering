//
// Created by q on 11/27/2019.
//

#ifndef _ORDERING_FOOD_LOADING_DATA_H
#define _ORDERING_FOOD_LOADING_DATA_H

#endif //_ORDERING_FOOD_LOADING_DATA_H

#define MAX_FOOD_NAME 30
#define MAX_FOOD_TYPE_NAME 30
#define MAX_DRINK_NAME 20
#define MAX_PRICE 50
#define MAX_NO_ELEMENTS 20
#define MAX_LINE 100
#define LOAD_DATA "Please load the data"
//allocate memory functions
void allocateMemoryFoods(int *noFoods, int **ptrNoTypes, char ***ptrFoods, char ****ptrFoodTypes, double ***ptrFoodPrices);
void allocateMemoryDrinks(int *noDrinks, char ***ptrDrinks, double **ptrDrinkPrices);
//loading data functions
void loadData(int *ptrNoFoods,char ***ptrFoods,int **ptrNoFoodTypes,char ****ptrFoodTypes,double ***ptrFoodPrices,int *ptrNoDrinks,char ***ptrDrinks,double **ptrDrinkPrices,int *state);
void loadFoodsData(char *inputLine, int *noFoods, char ***ptrFoods, int **ptrNoTypes, char ****ptrFoodTypes, double ***ptrFoodPrices);
void loadDrinksData(char *inputLine, int *noDrinks, char ***ptrDrinks, double **ptrDrinkPrices);
void readDataLine(char *inputLine, char *delim, int *noTypes, char **elements, char ***ptrTypes, double **ptrPrices, int MAX_NAME);
void readNoCategory(int *noCategories);
void readFoodType(char * inputLine, char **ptrFoods);
int countNoElements(char *line);
//process strings functions
void separateTwo(char * line, char * type, char *delim);
void splitLineIntoElements(char *line, char **elements, char *delim, int MAX_NAME);
double convertToDouble(char *string);
void deleteUnnecessarySpaces (char * string);
//free memory function
void freeMemory(int noFood, int *noTypes, char **foods, char ***foodTypes, double ** foodPrices, int noDrinks, char **drinks, double *drinkPrices);