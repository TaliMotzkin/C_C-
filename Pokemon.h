#ifndef Pokemon_h
#define Pokemon_h


typedef struct Type_struct
{
	char *typeName;
	int typeAmount;
	struct Type_struct** effectiveAgainstMe;
	int AgainstMeCount;
	struct Type_struct** effectiveAgainstOthers;
	int AgainstOthersCount;

}PokemonType;


typedef struct Bio_struct
{
	float height;
	float weight;
	int attack;

}Bio;

typedef struct Pokemom_struct
{
	char *poke_name;
	char *species;
	PokemonType *type;
	Bio *bio;

}Pokemon;

//looking for a pokemon in the array types we already created
status findPokemonType(PokemonType **typesArray, const int typeCount, const char *typeName, PokemonType **outType);

//allocate memory for pokemon types array
status make_pokemon_type(PokemonType ***outTypesArray, int index, const int typeCount, const char * typeName);

//destroy all the pokemon types
status destroy_all_pokemon_types(PokemonType **typesArray, const int typeCount);

//add effective against others
status addEffectiveAgainsOthers(PokemonType *myType, PokemonType *otherType);

//add effective against me
status addEffectiveAgainsMe(PokemonType *myType, PokemonType *otherType);


//create abiological data
status createBio (Bio **bio, float height, float weight, int attack);

//create a single pokemon
status createPokemon(Pokemon ***pokeArray,int index ,PokemonType **typesArray ,const int pokeCount, const int typeCount, const char* name, const float height, const float weight, const int attack, const char* type, const char* species);

//destroy biology of pokemon
status destroyBio (Bio* biology);

//destroy single pokemon and the array holding it
status destroyPokemonAndArray(Pokemon** pokemonArray, const int arraySize);

//remove from list others
status removeAgainstOthers( PokemonType *sourceType, PokemonType *otherType);

//remove from list me
status removeAgainstMe(PokemonType *sourceType, PokemonType *otherType);



//pokemon printing
status pokemonPrint(Pokemon* toPrint);

//type printing
status pokemonTypePrint(PokemonType* toPrint);

//status delete_against_others( PokemonType *source_type, PokemonType *add_type);
//
//status print_pokemon(Pokemon *pkmn);
//
//status print_pokemon_type(PokemonType *pkmn_t);
//
//status reading_file(int num_types, int num_pokemon, char *configfile);

//functions that destroy objects??

#endif
