
#include "Defs.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "Pokemon.h"




//round to decimel
float roundToDecimal(const float value);


//round
float roundToDecimal(float value){
	return roundf(value *100)/100;
}



//make pokemon type function
status make_pokemon_type(PokemonType ***outTypesArray, int index, const int typeCount, const char *typeName){
	//check if pointer valid!
	if (outTypesArray == NULL){
		return pointerNotValid;
	}

	//check if pointer valid!
	if (typeName == NULL){
		return pointerNotValid;
	}



	// see if it is the first pokemon - if it is allocate space in an array of pokemons.
	if (index == 0){
	//Allocate memory for all the array of pointers
	*outTypesArray = (PokemonType **) malloc(typeCount * sizeof(PokemonType *));
		if (*outTypesArray == NULL){
			return allocationFailure;
		}
	}

	///create a memory allocation for pokemontype in the array
	(*outTypesArray)[index]  = (PokemonType*) malloc(sizeof(PokemonType));
	if ((*outTypesArray)[index] == NULL){
		//free all related pokemonstypes
		status destroy = destroy_all_pokemon_types(*outTypesArray, typeCount);
		if (destroy!=success){
			return destroy;
		}
		return allocationFailure;
	}

	//first initiate the name allocation (remember to free!!)
	(*outTypesArray)[index]->typeName = (char *)malloc(strlen(typeName)+1);
	if ((*outTypesArray)[index]->typeName == NULL){
		//free the previous type pointer
		status destroy = destroy_all_pokemon_types(*outTypesArray, typeCount);
		if (destroy!=success){
			return destroy;
		}
	return allocationFailure;
	}

	//set the name
	strcpy((*outTypesArray)[index]->typeName, typeName);


	//initiate all features of pokemon type
	(*outTypesArray)[index]->typeAmount=0;
	(*outTypesArray)[index]->effectiveAgainstMe = NULL;
	(*outTypesArray)[index]->effectiveAgainstOthers = NULL;
	(*outTypesArray)[index]->AgainstMeCount = 0;
	(*outTypesArray)[index]->AgainstOthersCount = 0;

	return success;


}

//destroy all the pokemon types
status destroy_all_pokemon_types(PokemonType **typesArray, const int typeCount){
	//check if pointer valid!
	if (typesArray == NULL){
		return distractionError;
	}


	for (int i = 0; i < typeCount; i++){
		if(typesArray[i] == NULL){
			continue;
		}

		//free name
		if (typesArray[i]->typeName){
			free(typesArray[i]->typeName);
		}


		//free arrays of pointers to effective against me (each type will be freed in its own stracture!
		if (typesArray[i]->effectiveAgainstMe){
			free(typesArray[i]->effectiveAgainstMe);

		}

		if (typesArray[i]->effectiveAgainstOthers){
				free(typesArray[i]->effectiveAgainstOthers);
		}

		free(typesArray[i]);

	}

	free(typesArray);
	return success;
}


//looking for a pokemon in the array types we already created
status findPokemonType(PokemonType **typesArray, const int typeCount, const char *typeName, PokemonType **outType){
	//check if pointer valid!
	if (typesArray == NULL){
		return pointerNotValid;
	}

	if (outType == NULL){
		return pointerNotValid;
	}

	for (int i=0; i < typeCount; i++){
		if (strcmp(typesArray[i]->typeName, typeName) == 0){

			//the outtype will catch the value of the needed pokemon type by its name
			*outType = typesArray[i];

			return success;
		}
	}
	//type not found will return failure
	return failure;
}


//add effective against others
status addEffectiveAgainsOthers(PokemonType *myType, PokemonType *otherType){
	//check if pointer valid!
	if (myType == NULL){
		return pointerNotValid;
	}

	if (otherType == NULL){
		return pointerNotValid;
	}

	int newSize = myType->AgainstOthersCount + 1;
	//before the array was null/num of pokemon types so I need to adjust the memory allocation to num+1
	PokemonType **tempArray = (PokemonType **)realloc(myType->effectiveAgainstOthers, newSize*sizeof(PokemonType *));
	if (tempArray == NULL){
		return allocationFailure;
	}
	//put the newtype in the list
	tempArray[myType->AgainstOthersCount]=otherType;
	//set the list to be the new array (he will be disructed at the end of the function scope!
	//and we saved it adress
	myType->effectiveAgainstOthers = tempArray;
	myType->AgainstOthersCount = newSize;
	return success;

}

//add effective against me
status addEffectiveAgainsMe(PokemonType *myType, PokemonType *otherType){
	//check if pointer valid!
	if (myType == NULL){
		return pointerNotValid;
	}

	if (otherType == NULL){
		return pointerNotValid;
	}


	int newSize = myType->AgainstMeCount + 1;
	//before the array was null/add num of pokemon types so I need to adjust the memory allocation to unm+1
	PokemonType **tempArray = (PokemonType **)realloc(myType->effectiveAgainstMe, newSize*sizeof(PokemonType *));
	if (tempArray == NULL){
		return allocationFailure;
	}
	//put the newtype in the list
	tempArray[myType->AgainstMeCount]=otherType;
	//set the list to be the new array (he will be disructed at the end of the function scope!
	//and we saved it adress
	myType->effectiveAgainstMe = tempArray;
	myType->AgainstMeCount = newSize;
	return success;

}



//create abiological data REMEMNER FREE
status createBio (Bio **bio, float height, float weight, int attack){
	//check if pointer valid!
	if (bio == NULL){
		return pointerNotValid;
	}

	//allocate memory for biodata
	*bio = (Bio*)malloc(sizeof(Bio));
	if (*bio == NULL){
		return allocationFailure;
	}


	(*bio)->height = height;
	(*bio)->weight = weight;
	(*bio)->attack = attack;
	return success;
}


//creates a pokemonand its array .
status createPokemon(Pokemon ***pokeArray,int index ,PokemonType **typesArray ,const int pokeCount, const int typeCount, const char* name, const float height, const float weight, const int attack, const char* type, const char* species){
	//check if pointer valid!
	if (typesArray == NULL){
		return pointerNotValid;
	}

	//check if pointer valid!
	if (pokeArray == NULL){
		return pointerNotValid;
	}
	//check if pointer valid!
	if (name == NULL || type ==NULL || species ==NULL){
		return pointerNotValid;
	}

	// see if it is the first pokemon - if it is allocate space in an array of pokemons.
	if (index == 0){
		//Allocate memory for all the array of pointers
		*pokeArray = (Pokemon **) malloc(pokeCount * sizeof(Pokemon *));
		if (*pokeArray == NULL){
			return allocationFailure;
		}
	}

	///create a memory allocation for pokemon in the array
	(*pokeArray)[index]  = (Pokemon*) malloc(sizeof(Pokemon));
	if ((*pokeArray)[index] == NULL){
		//free all related pokemons
		status destroy = destroyPokemonAndArray(*pokeArray, pokeCount);
		if (destroy!=success){
			return destroy;
		}
		return allocationFailure;
	}


	//first initiate the name allocation (remember to free!!)
	(*pokeArray)[index]->poke_name = (char *)malloc(strlen(name)+1);
	if ((*pokeArray)[index]->poke_name == NULL){
			//free the previous type pointer
			status destroy = destroyPokemonAndArray(*pokeArray, pokeCount);
				if (destroy!=success){
					return destroy;
				}
			return allocationFailure;
	}

	//set the name
	strcpy((*pokeArray)[index]->poke_name, name);

	//then the species aloc
	(*pokeArray)[index]->species = (char *)malloc(strlen(species)+1);
	if ((*pokeArray)[index]->species == NULL){
			//free the previous type pointer
			status destroy = destroyPokemonAndArray(*pokeArray, pokeCount);
			if (destroy!=success){
				return destroy;
			}
			return allocationFailure;
	}
	//set the species
	strcpy((*pokeArray)[index]->species, species);

	//then add the type pointer
	//search for the right type
	PokemonType *pointertype = NULL;
	status typestat = findPokemonType(typesArray, typeCount, type, &pointertype);
	if (typestat != success){
		status destroy = destroyPokemonAndArray(*pokeArray, pokeCount);
		if (destroy!=success){
			return destroy;
		}
		return typestat;
	}

	//set the type
	(*pokeArray)[index]->type = pointertype;

	//we want to update this typenumber of pokemons!
	pointertype->typeAmount += 1;

	//initialize the bio data already in the pointer defined for it in the stracture
	status bioStat = createBio(&((*pokeArray)[index]->bio), height,  weight,  attack);
	if (bioStat != success){
		status destroy = destroyPokemonAndArray(*pokeArray, pokeCount);
		if (destroy!=success){
			return destroy;
		}
		//no allcation for type just ointer so we don't nees to set it NULL or free
		return bioStat;
	}


	return success;
}

//destroyPokemon
status destroyPokemonAndArray(Pokemon** pokemonArray, const int arraySize){
//check if pointer valid!
	if (pokemonArray == NULL){
		return distractionError;
	}

	//for each pokemon in the array
	for (int i = 0; i< arraySize; i++){
		if(pokemonArray[i] == NULL){
			continue;
		}
		//free name
		if (pokemonArray[i]->poke_name){
			free(pokemonArray[i]->poke_name);
		}


		//free species
		if (pokemonArray[i]->species){
			free(pokemonArray[i]->species);

		}

		//free biology
		if (pokemonArray[i]->bio){
			status distroy = destroyBio(pokemonArray[i]->bio);
			if (distroy != success){
				return distroy;
			}
		free(pokemonArray[i]);
		}
	}
	free(pokemonArray);
	return success;
}


//destroy biology of pokemon
status destroyBio (Bio* biology){
	//check pointer!
	if (biology ==NULL){
		return distractionError;
	}
	free(biology);
	return success;

}
//remove specific type from lists


//remove from list others
status removeAgainstOthers(PokemonType *sourceType, PokemonType *otherType){
	//check if pointer valid!
		if (sourceType == NULL){
			return pointerNotValid;
		}

		if (otherType == NULL){
			return pointerNotValid;
		}

		//check if the other pokemon on its others list
		int indexRemove = -1;
		for (int i = 0 ; i<sourceType->AgainstOthersCount; i++){
			if (otherType == sourceType->effectiveAgainstOthers[i]){
				indexRemove = i;
			}
		}
		//other is not in the list
		if (indexRemove == -1){
			return failure;
		}

		//single vlaue handaling!:
		if (sourceType->AgainstOthersCount ==1){
			free(sourceType->effectiveAgainstOthers);
			sourceType->effectiveAgainstOthers = NULL;
			sourceType->AgainstOthersCount = 0;
			return success;
		}

		//shift the types in the list to the "left"
		for (int j = indexRemove; j <sourceType->AgainstOthersCount -1; j++ ){
			sourceType->effectiveAgainstOthers[j] = sourceType->effectiveAgainstOthers[j+1];
		}



		//set pointer to null so it won't be seen at use
		sourceType->effectiveAgainstOthers[sourceType->AgainstOthersCount -1] = NULL;

		//resize the amount
		int newSize = sourceType->AgainstOthersCount -1;

		//reallocate the array
		PokemonType **newArray = (PokemonType **)realloc(sourceType->effectiveAgainstOthers, newSize*sizeof(PokemonType *));
		if (newArray == NULL && newSize >0){
			return allocationFailure;
		}



		//set the list to be the new array (he will be disructed at the end of the function scope!
		//and we saved it adress
		if (newArray != NULL){
			sourceType->effectiveAgainstOthers = newArray;
			sourceType->AgainstOthersCount =newSize;
		}

		return success;
}


//remove from list others
status removeAgainstMe(PokemonType *sourceType, PokemonType *otherType){
	//check if pointer valid!
		if (sourceType == NULL){
			return pointerNotValid;
		}

		if (otherType == NULL){
			return pointerNotValid;
		}

		//check if the other pokemon on its others list
		int indexRemove = -1;
		for (int i = 0 ; i<sourceType->AgainstMeCount; i++){
			if (otherType == sourceType->effectiveAgainstMe[i]){
				indexRemove = i;
			}
		}
		//other is not in the list
		if (indexRemove == -1){
			return failure;
		}

		//single vlaue handaling!:
		if (sourceType->AgainstMeCount ==1){
			free(sourceType->effectiveAgainstMe);
			sourceType->effectiveAgainstMe = NULL;
			sourceType->AgainstMeCount = 0;
			return success;
		}
		//shift the types in the list to the "left"
		for (int j = indexRemove; j <sourceType->AgainstMeCount -1; j++ ){
			sourceType->effectiveAgainstMe[j] = sourceType->effectiveAgainstMe[j+1];
		}


		//set pointer to null so it won't be seen at use
		sourceType->effectiveAgainstMe[sourceType->AgainstMeCount -1] = NULL;

		//resize the amount
		int newSize = sourceType->AgainstMeCount -1;

		//reallocate the array
		PokemonType **newArray = (PokemonType **)realloc(sourceType->effectiveAgainstMe, newSize*sizeof(PokemonType *));
		if (newArray == NULL && newSize >0){
			return allocationFailure;
		}



		//set the list to be the new array (he will be disructed at the end of the function scope!
		//and we saved it adress
		if (newArray != NULL){
			sourceType->effectiveAgainstMe = newArray;
			sourceType->AgainstMeCount =newSize;
		}

		return success;
}


status pokemonPrint(Pokemon* toPrint){
	//check pointer
	if (toPrint == NULL){
		return pointerNotValid;
	}
	const float rHeight = roundToDecimal(toPrint->bio->height);
	const float rWeight = roundToDecimal(toPrint->bio->weight);

	printf("%s :\n", toPrint->poke_name);
	printf("%s, %s Type.\n", toPrint->species, toPrint->type->typeName);
	printf("Height: %.2f m    Weight: %.2f kg    Attack: %d\n\n", rHeight,rWeight , toPrint->bio->attack);
	return success;
}



status pokemonTypePrint(PokemonType* toPrint){
	//check pointer
	if (toPrint == NULL){
		return pointerNotValid;
	}

	printf("Type %s -- %d pokemons", toPrint->typeName, toPrint->typeAmount);
	if (toPrint->AgainstOthersCount !=0){
		printf("\n");
		printf("\t");
		printf("These types are super-effective against %s:",toPrint->typeName);
		for (int i =0; i<toPrint->AgainstOthersCount; i++){
			printf("%s",toPrint->effectiveAgainstOthers[i]->typeName);
			if (i != toPrint->AgainstOthersCount - 1){
				printf(" ,");
			}
		}
	}
	if (toPrint->AgainstMeCount !=0){
		printf("\n");
		printf("\t");
		printf("%s moves are super-effective against:",toPrint->typeName);
		for (int i =0; i<toPrint->AgainstMeCount; i++){
			printf("%s",toPrint->effectiveAgainstMe[i]->typeName);
			if (i != toPrint->AgainstMeCount - 1){
				printf(" ,");
			}
		}
	}

	printf("\n\n");
	return success;

}
