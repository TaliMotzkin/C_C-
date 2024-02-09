/*
 ============================================================================
 Name        : Pokedex.c
 Author      :
 Version     :
 Copyright   : Your copyright notice
 Description : Hello World in C, Ansi-style
 ============================================================================
 */
#include "Defs.h"
#include "Pokemon.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

//Error handaling
void hadleError(status err);

//translate the line of types
status typesEffectiveListLine(PokemonType **typesArray, const int typeCount,  char *line);

//reading file logic and putting data in the system
status reading_file(PokemonType*** arrayTypes, Pokemon*** arrayPoke,int pokeCount, int numberTypes, int numberPokemons, char * configfile);

//searching types in a list
status findingTypeinList(PokemonType **typeList, int typeListCount,PokemonType *othertype );

//handaling getting data on pokemon adding to me list
status userAddList(PokemonType **arrayTypes, int numberTypes, int choise2);


int main(int argc, char *argv[]){

	//convert the number of types and pokemon to int
	int numberTypes = atoi(argv[1]);
	int numberPokemons = atoi(argv[2]);

	//convert the file path to pointer char
	char *configfile = argv[3];

	//make Null pointers to arrays of types
	PokemonType** arrayType =NULL;

	//make a null pointer to array of pokemons
	Pokemon** arrayPoke = NULL;

	//read the file and set all the pokemons etc in the system
	status read = reading_file(&arrayType, &arrayPoke, numberPokemons,numberTypes, numberPokemons, configfile);
	if (read !=success){
		hadleError(read);
		return 1;
	}


	int choise;
	//infinate loop
	do {
		printf("Please choose one of the following numbers:\n");
		printf("1 : Print all Pokemons\n");
		printf("2 : Print all Pokemons types\n");
		printf("3 : Add type to effective against me list\n");
		printf("4 : Add type to effective against others list\n");
		printf("5 : Remove type from effective against me list\n");
		printf("6 : Remove type from effective against others list\n");
		printf("7 : Print Pokemon by name\n");
		printf("8 : Print Pokemons by type\n");
		printf("9 : Exit\n");

		//read choise
		if (scanf("%d", &choise) != 1){
			//if the chise is invalid not number
			printf("Please choose a valid number.\n");
			while (getchar() != '\n');
			continue; //skip the rest of the cases
		}

		//proccess
		switch (choise){
			case 1:
				for (int i =0 ;i<numberPokemons;i++){
					status pprint = pokemonPrint(arrayPoke[i]);
					if (pprint != success){
						hadleError(pprint);
						return 1;
					}
				}
//				choise = 0;
				break;
			case 2:
				for (int i =0 ;i<numberTypes;i++){
					status tprint = pokemonTypePrint(arrayType[i]);
					if (tprint != success){
						hadleError(tprint);
						return 1;
					}
				}
//				choise = 0;
				break;
			case 3:
				status addingme = userAddList(arrayType, numberTypes, choise);
				if (addingme != success){
					hadleError(addingme);
					return 1;
				}
//				choise = 0;
				break;
			case 4:
				status addingother = userAddList(arrayType, numberTypes, choise);
				if (addingother != success){
					hadleError(addingother);
					return 1;
				}
//				choise = 0;
				break;
			case 5:

				break;
			case 6:

				break;
			case 7:

				break;
			case 8:

				break;
			case 9:
				printf("All the memory cleaned and the program is safely closed\n");
				break;
			default:
				//if the number is not between 1-9
				printf("Please choose a valid number.\n");
				//clear inccorext input
				while (getchar() != '\n');
				break;


		}


	}while (choise !=9);



	//destroy all before living
	destroy_all_pokemon_types(arrayType, numberTypes);
	destroyPokemonAndArray(arrayPoke, numberPokemons);

	return 0;
}


void hadleError(status err){
	switch(err){
		case success:
			break;
		case failure:
			fprintf(stderr, "Invalid input or something not found");
			break;
		case allocationFailure:
			fprintf(stderr, "Memory Problem");
			break;
		case pointerNotValid:
			fprintf(stderr, "NULL pointer");
			break;
		case distractionError:
			fprintf(stderr,"NOT FREED MEMORY");
			break;
	}
}

status reading_file(PokemonType*** arrayTypes, Pokemon*** arrayPoke,int pokeCount,  int numberTypes,int numberPokemons,char* configfile){

	//check if pointer valid!
	if (arrayTypes == NULL){
		return pointerNotValid;
	}

		//check if pointer valid!
	if (configfile == NULL){
		return pointerNotValid;
	}

		//open file
	FILE *file = fopen(configfile, "r");
	if (file == NULL){

		return failure;
	}

	//make buffer for dinamically allocating memory
	char line[301];
	//set flag if we are on types line.

	bool readPokemons = false;
	bool readTypes = false;
	bool readList = false;
	int counter = 0;

	while (fgets(line, sizeof(line), file)){

		//deal with the hidden character r and n
		char* crPosition = strchr(line, '\r');
		if (crPosition !=NULL && crPosition[1] =='\n'){
			* crPosition = '\0';
		} else{
			//remove "/n"
			line[strcspn(line, "\n")] = 0;
		}


		//If we are in the right line of Types
		if ((readTypes==false) && (strstr(line, "Types") != NULL)) {
			readTypes = true;
			continue;
		}
		//If we are in the right line of Pokemons
		if ((readTypes==false) && (readList) && !readPokemons && (strstr(line, "Pokemons") != NULL)) {
			readPokemons = true;
			readList = false;
			continue;
		}

		//start reading the types
		if (readTypes == true && !readPokemons && !readList) {
			//seperate each type by ","
			char *token = strtok(line, ",");
			for (int i =0 ; i <numberTypes;i++) {
				make_pokemon_type(arrayTypes, i, numberTypes, token);
				//get next token
				token =strtok(NULL, ",");

			}
			readTypes = false;
			readList = true;
			continue;
		}
		if (!readTypes && !readPokemons && readList) {
			status typeslines = typesEffectiveListLine(*arrayTypes, numberTypes, line);
			if (typeslines != success){
				return typeslines;
			continue;
			}
		}

		//If we are in the right line of Pokemons
		if (!readTypes && readPokemons && !readList) {
			char * name = strtok(line, ",");
			char * species = strtok(NULL, ",");
			float height = atof(strtok(NULL, ","));
			float weight = atof(strtok(NULL, ","));
			int attack = atoi(strtok(NULL, ","));
			char * type = strtok(NULL, ",");
			createPokemon(arrayPoke,counter,*arrayTypes ,pokeCount, numberTypes, name, height, weight, attack, type, species);

			counter += 1;
			continue;
		}


	}
	fclose(file);
	return success;
}


//translate the line of types to the lists of effectivness
status typesEffectiveListLine(PokemonType **typesArray, const int typeCount, char *line){
	//check if pointer valid!
	if (typesArray == NULL){
		return pointerNotValid;
	}

	//check if pointer valid!
	if (line == NULL){
		return pointerNotValid;
	}

	//skip tabs - CHECK if needed
	while (*line == ' ' || *line == '\t'){
		++line;

	}

	//finds the position of first space
	char *spacePos = strchr(line, ' ');
	if (spacePos == NULL){
		return failure;
	}

	//set the end of the name
	*spacePos ='\0';


//create a pointer that will hold the found pokemontype from the search in the types array
	PokemonType *meType = NULL;

	//find the object of self/me by the function I build before
	status findPokemonStat = findPokemonType(typesArray, typeCount,line, &meType);
	//Handle different status in creation types
	if (findPokemonStat != success){
		return findPokemonStat;
	}

	//restore the space
	*spacePos = ' ';

	//see if I have effective against me or others

	bool isOthers = false;
	if (strstr(spacePos, "effective-against-other") != NULL){
		isOthers = true;

	}


	//set pinter to the starting types names! and tokenize them
	char *typesStart = strchr(spacePos, ':');
	if (typesStart == NULL){
		return failure;
	}
	typesStart++;

	char *typeToken = strtok(typesStart, ",");

	//loop on the types I need to add to the against me\others (by the bool val from above)
	while (typeToken != NULL){

		//cut space of value (*) and adds to the address ++
		while (*typeToken == ' '){
			++typeToken;
		}


		//create a pointer that will hold the found pokemontype from the search in the types array
		PokemonType *otherType = NULL;


		//find the object of other by the function I build before

		status findOtherStat = findPokemonType(typesArray, typeCount,typeToken, &otherType);
		//Handle different status in creation types

		if (findOtherStat != success){
			return findPokemonStat;
		}


		//see where to add the type
		status addingToLists;
		if (isOthers){
			addingToLists = addEffectiveAgainsOthers(meType, otherType);

		}
		else {
			addingToLists = addEffectiveAgainsMe(meType,otherType);
		}

		if (addingToLists != success){
			return addingToLists;
		}


		//the next name until NULL (in while loop)
		typeToken = strtok(NULL, ",");
	}

	return success;
}


status findingTypeinList(PokemonType **typeList, int typeListCount,PokemonType *othertype ){
	//check if pointer valid!
	if (typeList == NULL){
		return pointerNotValid;
	}

	//check if pointer valid!
	if (othertype == NULL){
		return pointerNotValid;
	}

	for (int i = 0; i< typeListCount; i++){
		if(typeList[i]==othertype){
			return failure;
		}
	}
	return success;
}

//handaling getting data on pokemon adding to me list
status userAddList(PokemonType **arrayTypes, int numberTypes, int choise){
	//check if pointer valid!
	if (arrayTypes == NULL){
		return pointerNotValid;
	}

	char typeName[301];
	PokemonType *mytype = NULL;
	PokemonType *othertype = NULL;

	//ask the user to the type we are adding to
	printf("Please enter type name:\n");
	scanf("%300s", typeName);

	//search type in the system:
	status found = findPokemonType(arrayTypes, numberTypes, typeName, &mytype);
	if (found == failure){
		printf("Type name doesn't exist.\n");
		//I dont want it to exit entirely from the program but just print that it is not found~
		return success;
	}
	if (found == pointerNotValid){
		return found;
	}

	if (choise == 3){
		//ask the user to add other type
		printf("Please enter type name to add to %s effective against me list:\n", mytype->typeName);
		scanf("%300s", typeName);
	}

	if (choise == 4){
		//ask the user to add other type
		printf("Please enter type name to add to %s effective against others list:\n", mytype->typeName);
		scanf("%300s", typeName);
	}


	//search type in the system:
	status found2 = findPokemonType(arrayTypes, numberTypes, typeName, &othertype);
	if (found2 == failure){
		printf("Type name doesn't exist.\n");
		return success;
	}
	if (found2 == pointerNotValid){
		return found2;
	}
	//now dealing with the situation if the type already in the effective me list
	if (choise == 3){
		if (mytype->effectiveAgainstMe != NULL){
			status findinList = findingTypeinList(mytype->effectiveAgainstMe, mytype->AgainstMeCount,othertype );
			if (findinList == failure){
				printf("This type already exist in the list.\n");
				return success;
			}
			if (findinList == pointerNotValid){
				return findinList;
			}
		}
		//everyting good so add to list
		status adding = addEffectiveAgainsMe(mytype, othertype);
		if (adding != success){
			return adding;
		}
	}

	if (choise == 4){
		if (mytype->effectiveAgainstOthers != NULL){
			status findinList = findingTypeinList(mytype->effectiveAgainstOthers, mytype->AgainstOthersCount,othertype );
			if (findinList == failure){
				printf("This type already exist in the list.\n");
				return success;
			}
			if (findinList == pointerNotValid){
				return findinList;
			}
		}
		//everyting good so add to list
		status adding = addEffectiveAgainsOthers(mytype, othertype);
		if (adding != success){
			return adding;
		}
	}


	//if the adding went good - print the type.
	status printing = pokemonTypePrint(mytype);
	if (printing != success){
		return printing;
	}
	return success;
}


////translate the line of types to the lists of effectivness
//status typesEffectiveListLine(PokemonType **typesArray, const int typeCount, char *line){
//	//check if pointer valid!
//	if (typesArray == NULL){
//		return pointerNotValid;
//	}
//
//	//check if pointer valid!
//	if (line == NULL){
//		return pointerNotValid;
//	}
//
//
//	char *lineCopy = strdup(line);
//	if (lineCopy == NULL){
//		return allocationFailure;
//	}
//
//	//skip tabs - CHECK if needed
//	while (*lineCopy == ' ' || *lineCopy == '\t'){
//		++lineCopy;
//
//	}
//
//	//finds the position of first space
//	char *spacePos = strchr(lineCopy, ' ');
//	if (spacePos == NULL){
//		free(lineCopy);
//		return failure;
//	}
//
//	//calculate the typename length and copy it into new buffer
//	size_t typeNameLength = spacePos -lineCopy;
//	char meTypeName[typeNameLength+1];
//	strncpy(meTypeName, lineCopy, typeNameLength);
//	meTypeName[typeNameLength] = '\0';
//
//
////create a pointer that will hold the found pokemontype from the search in the types array
//	PokemonType *meType = NULL;
//
//	//find the object of self/me by the function I build before
//	status findPokemonStat = findPokemonType(typesArray, typeCount,meTypeName, &meType);
//	//Handle different status in creation types
//	if (findPokemonStat != success){
//		free(lineCopy);
//		return findPokemonStat;
//	}
//
//	//see if I have effective against me or others
//	//finds the position of first space
//	char *colPos = strchr(spacePos + 1, ':');
//	if (colPos == NULL){
//		free(lineCopy);
//		return failure;
//	}
//
//
//
//	bool isOthers = false;
//	if (strstr(spacePos, "effective-against-other") != NULL){
//		isOthers = true;
//	}
//
//
//
//	//set pinter to the starting types names! and tokenize them
//	char *typesStart = colPos+1;
//	char *typeToken = strtok(typesStart, ", ");
//
//	//loop on the types I need to add to the against me\others (by the bool val from above)
//	while (typeToken != NULL){
//
//		//cut space of value (*) and adds to the address ++
//		while (*typeToken == ' '){
//			++typeToken;
//		}
//
//
//		//create a pointer that will hold the found pokemontype from the search in the types array
//		PokemonType *otherType = NULL;
//
//
//		//find the object of other by the function I build before
//		status findOtherStat = findPokemonType(typesArray, typeCount,typeToken, &otherType);
//		//Handle different status in creation types
//		if (findOtherStat != success){
//			free(lineCopy);
//			return findPokemonStat;
//		}
//
//
//		//see where to add the type
//		status addingToLists;
//		if (isOthers){
//			addingToLists = addEffectiveAgainsOthers(meType, otherType);
//
//		}
//		else {
//			addingToLists = addEffectiveAgainsMe(meType,otherType);
//		}
//
//		if (addingToLists == allocationFailure || addingToLists == pointerNotValid ){
//			free(lineCopy);
//			return addingToLists;
//		}
//
//
//		//the next name until NULL (in while loop)
//		typeToken = strtok(NULL, ", ");
//	}
//	free(lineCopy);
//	return success;
//}

//	const int typesize = 3; //example size
//
//	//string to coppy to allocated memory
//	const char *names[] = {"Fire","Grass", "Water"};
//
//	//a pointer to the array of pokemon types
//	PokemonType **pokemonTypes = NULL;
//
//	//create all types given in the configuration file
//	status status_types_create = create_all_types(&pokemonTypes, typesize, names);
//
//	//Handle different status in creation types
//	switch (status_types_create){
//	case allocationFailure:
//		printf("Memory Problem");
//		return 1;
//	case pointerNotValid:
//		printf("NULL pointer");
//		return 1;
//	case distractionError:
//		printf("FREE MEMORY NOO");
//		return 1;
//	case failure:
//		printf("failure");
//		return 1;
//	case success:
//		//print FOR ME!!
//		for (int i = 0; i < typesize; i++){
//			printf("Type created: %s\n", pokemonTypes[i]->typeName);
//			printf("At: %p\n", pokemonTypes[i]);
//		}
//		break;
//	}
//
//	//check if line exists..
//	//create line of effective agains types given in the configuration file
//	char *lines[] = {"Fire effective-against-others: Water", "Fire effective-against-me:Grass, Water"};
////	int length = sizeof(lines)/sizeof(lines[0]);
//
//	status stat1 =  typesEffectiveListLine(pokemonTypes,typesize, lines[0]);
//	//Handle different status in creation types
//	switch (stat1){
//		case allocationFailure:
//			printf("Memory Problem");
//			return 1;
//		case pointerNotValid:
//			printf("NULL pointer");
//			return 1;
//		case distractionError:
//			printf("FREE MEMORY NOO");
//			return 1;
//		case failure:
//			printf("token or type not found");
//			return 1;
//		case success:
//			break;
//	}
//	typesEffectiveListLine(pokemonTypes,typesize, lines[1]);
//	const float height = 33;
//	const float weight =4.67678;
//	const int attack = 6;
//	const char* species = "Metal";
//	const char* name = "picatchu";
//	const char* type_p = "Fire";
//	Pokemon **poke = NULL;
//	int pokeCount = 2;
//	for (int i=0; i<pokeCount; i++){
//		status statpoke =  createPokemon(&poke,i ,pokemonTypes ,pokeCount, typesize, name,  height, weight, attack, type_p,  species);
//		switch (statpoke){
//				case allocationFailure:
//					printf("Memory Problem");
//					return 1;
//				case pointerNotValid:
//					printf("NULL pointer");
//					return 1;
//				case failure:
//					printf("token or type not found");
//					return 1;
//				case distractionError:
//					printf("FREE MEMORY NOO");
//					return 1;
//				case success:
//					break;
//			}
//	}
//	printf("count before %d\n", pokemonTypes[0]->AgainstMeCount);
//	printf("name of first '%s' is\n", pokemonTypes[0]->effectiveAgainstMe[0]->typeName);
////	printf("name of second '%s' is\n", pokemonTypes[0]->effectiveAgainstMe[1]->typeName);
//
////	status remove = removeAgainstMe(pokemonTypes[0],pokemonTypes[2]);
////	switch (remove){
////		case allocationFailure:
////			printf("Memory Problem");
////			return 1;
////		case pointerNotValid:
////			printf("NULL pointer");
////			return 1;
////		case failure:
////			printf("token or type not found");
////			return 1;
////		case distractionError:
////			printf("FREE MEMORY NOO");
////			return 1;
////		case success:
////			printf("count now %d\n", pokemonTypes[0]->AgainstMeCount);
//////			printf("name of first '%s' is\n", pokemonTypes[0]->effectiveAgainstMe[0]->typeName);
////			break;
////
////	}
//
//	pokemonTypePrint(pokemonTypes[0]);
//
/////home/motzkina/eclipse-workspace/Pokedex/poke2
////gcc -g -O1 Pokedex.c Pokemon.c
////valgrind --leak-check=yes --log-file=valgrind.rpt ./a.out
//
//	//	status status_types_added = typesEffectiveListLine(pokemonTypes, typesize, lines[0]);
//
//
////	for (int i = 0; i < length; ++i){
////		status status_types_added = typesEffectiveListLine(pokemonTypes, typesize, lines[i]);
////
////		//Handle different status in creation types
////		switch (status_types_added){
////		case allocationFailure:
////			printf("Memory Problem");
////			return 1;
////		case pointerNotValid:
////			printf("NULL pointer");
////			return 1;
////		case failure:
////			printf("token or type not found");
////			return 1;
////		case success:
////			//print FOR ME!!
////			printf("line added successfuly");
////			break;
////
////		}
////	}
////
//
//	//destroy all allocated memory
//	destroy_all_pokemon_types(pokemonTypes, typesize);
//	destroyPokemonAndArray(poke, pokeCount);
//	return 0;
//
//	}
//	//add to each of adding to list etc destruction!!
//



