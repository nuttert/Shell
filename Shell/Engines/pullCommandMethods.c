//
//  pullCommandMethods.c
//  Мракобес
//
//  Created by vlad on 14.10.2018.
//  Copyright © 2018 vlad. All rights reserved.
//

#include "pullCommandMethods.h"
#include "methodsForAddingNewCharacter.h"
#include <stdlib.h>
/////////////////////////////////////////////////////////////////////////////////////////////////////////
char* getLastCommandName(Text* text){
    return getWordOfCommand(text->conveyorParts->last->commands->last->command.name);
}
char* pullLastCommandName(Text* text){
    return  pullWordOfCommand(&text->conveyorParts->last->commands->last->command.name);
}
char* getLastArgument(Text* text){
    return getWordOfCommand(text->conveyorParts->last->commands->last->command.attributesWithMain->last->arguments);
}
char* pullLastArgument(Text* text){
    ListEntryForAttributesWithMain* last = text->conveyorParts->last->commands->last->command.attributesWithMain->last,
    *first = text->conveyorParts->last->commands->last->command.attributesWithMain->first;
    while(emptyAttributeWithArgumentInLastCommand(text) && last != first){
                ListEntryForAttributesWithMain* previous = text->conveyorParts->last->commands->last->command.attributesWithMain->last->previous;
                free(text->conveyorParts->last->commands->last->command.attributesWithMain->last);
                text->conveyorParts->last->commands->last->command.attributesWithMain->last = previous;
                if(previous == NULL )return NULL;
                previous->next = NULL;
    }
    return  pullWordOfCommand(&text->conveyorParts->last->commands->last->command.attributesWithMain->last->arguments);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////

char* getWordOfCommand(ListOfChars* word){
    if(word && word->lenghtOfWord>0){
        ListEntryForChars* carriage = word->currentPosition,*nextChar;
        char* copyWord = malloc(sizeof(char)*word->lenghtOfWord+1);
        copyWord[word->lenghtOfWord] = '\0';
        for(int i = 0;carriage;i++){
            nextChar = carriage->next;
            copyWord[i] = carriage->symbol;
            carriage = nextChar;
        }
        return copyWord;
    }else
        return NULL;
}
    
char* pullWordOfCommand(ListOfChars** pointerToWord){
    ListOfChars* word = *pointerToWord;
    if(word && word->lenghtOfWord>0){
        ListEntryForChars* carriage = word->currentPosition,*nextChar;
        char* copyWord = malloc(sizeof(char)*word->lenghtOfWord+1);
        copyWord[word->lenghtOfWord] = '\0';
        for(int i = 0;carriage;i++){
            nextChar = carriage->next;
            copyWord[i] = carriage->symbol;
            free(carriage);
            carriage = nextChar;
        }
        free(*pointerToWord);
        *pointerToWord = NULL;
        return copyWord;
    }else
        return NULL;
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////

void getAttributesOfCertainCommand(ListOfAttributesWithArguments* list,char** copyWord){
    ListEntryForAttributesWithMain* carriageList = list->currentPosition,* savedPreviousList = list->currentPosition;
    char** carriageCopyWord = copyWord;
    while(carriageList){
        *carriageCopyWord = getWordOfCommand(carriageList->attributes);
        if(*carriageCopyWord)  carriageCopyWord++;
        *carriageCopyWord = getWordOfCommand(carriageList->arguments);
        if(*carriageCopyWord)  carriageCopyWord++;
        carriageList = carriageList->next;
        savedPreviousList = carriageList;
        list->amountOfAttributesWithMain--;
    }
    *carriageCopyWord = NULL;
}

char** getWholeCommand(Text* text){
    if( text->conveyorParts->currentPosition->commands->amountOfCommands == 0) return NULL;
    ListEntryForCommands* firstCommand = text->conveyorParts->currentPosition->commands->currentPosition;
    ListOfAttributesWithArguments* firstPairOfAttributeWithArgument = firstCommand->command.attributesWithMain;
    int amountOfAttributesWithArgumentsAndName = firstCommand->command.attributesWithMain->amountOfAttributesWithMain+1;
    char* cheker;
    char** carriage;
    char** copyWord = carriage = malloc(sizeof(char*)*amountOfAttributesWithArgumentsAndName+1);
    
    cheker = getWordOfCommand(firstCommand->command.name);
    if (cheker) *carriage++ = cheker;
    getAttributesOfCertainCommand(firstPairOfAttributeWithArgument, carriage);
    
    
    text->conveyorParts->currentPosition->commands->currentPosition = text->conveyorParts->currentPosition->commands->currentPosition->next;
    text->conveyorParts->currentPosition->commands->amountOfCommands--;
    return copyWord;
}

char** getWholeCommandForConveyorPart(Text* text){
    if(text->conveyorParts->amountOfConveyorParts == 0) return NULL;
    char** command = getWholeCommand(text);
    if(command == NULL){
        if(text->conveyorParts->currentPosition->next){
        text->conveyorParts->currentPosition = text->conveyorParts->currentPosition->next;
        text->conveyorParts->currentPosition->amountOfCommands--;
            return getWholeCommand(text);
        }else return NULL;
    }
    return command;
}


