//
//  methodsForAddingNewCharacter.c
//  Мракобес
//
//  Created by vlad on 13.10.2018.
//  Copyright © 2018 vlad. All rights reserved.
//

#include "methodsForAddingNewCharacter.h"
#include <stdlib.h>

////////////////////////////////////////////////////////////////////////////////////////
//methods ForA dding New Character
ListEntryForChars* newListEntry(char symbol){
    ListEntryForChars* element = malloc(sizeof(ListEntryForChars));
    element->next = NULL;
    element->symbol = symbol;
    return element;
}
ListOfChars* getSymbolsList(){
    ListOfChars* list = malloc(sizeof(ListOfChars));
    list->first = list->last = list->currentPosition = NULL;
    
    list->lenghtOfWord = 0;
    return list;
}

void appendNewSymbolInList(ListOfChars* list, char symbol){
    if (list->first) { list->last = list->last->next  =  newListEntry(symbol);}
    else{ list->last = list->first  = list->currentPosition = newListEntry(symbol);list->lenghtOfWord = 0;}
    list->lenghtOfWord++;
}





////////////////////////////////////////////////////////////////////////////////////////
ListEntryForConveyorPart* newConveyorPartListEntry(){
    ListEntryForConveyorPart* element = malloc(sizeof(ListEntryForConveyorPart));
    element->commands = getCommandsList();
    element->next = NULL;
    element->amountOfCommands = 0;
    element->inputFile = element->outPutFile = NULL;
    return element;
}

ListOfConveyorPart* getConveyorPartsList(){
    ListOfConveyorPart* list = malloc(sizeof(ListOfConveyorPart));
    list->first = list->last = list->currentPosition = NULL;
    list->amountOfConveyorParts = 0;
    return list;
}
//append pair of Attribute with argument
void appendNewConveyorPart(Text* text){
    ListOfConveyorPart * list = text->conveyorParts;
    if (list->first) list->last = list->last->next  = newConveyorPartListEntry();
    else list->last = list->first = list->currentPosition = newConveyorPartListEntry();
    list->amountOfConveyorParts++;
}







////////////////////////////////////////////////////////////////////////////////////////
ListEntryForAttributesWithMain* newAttributeAndMainListEntry(){
    ListEntryForAttributesWithMain* element = malloc(sizeof(ListEntryForAttributesWithMain));
    element->arguments = NULL;
    element->attributes = NULL;
    element->next = NULL;
    element->previous = NULL;
    return element;
}

ListOfAttributesWithArguments* getAttributeAndMainList(){
    ListOfAttributesWithArguments* list = malloc(sizeof(ListOfAttributesWithArguments));
    list->first = list->last = list->currentPosition = NULL;
    list->amountOfAttributesWithMain = 0;
    return list;
}
//append pair of Attribute with argument
void appendNewAttributeAndMainPair(Text* text){
    ListOfAttributesWithArguments* list = text->conveyorParts->last->commands->last->command.attributesWithMain;
    appendNewAttributeAndMainPairForElement(list);
}
void appendNewAttributeAndMainPairForElement(ListOfAttributesWithArguments* list){
    ListEntryForAttributesWithMain* previous;
    if (list->first) {if(!emptyAttributeWithArgument(list)){
        previous = list->last;
        list->last = list->last->next  = newAttributeAndMainListEntry();
        list->last->previous = previous;
         list->amountOfAttributesWithMain++;}
    }
    else{list->last = list->first = list->currentPosition = newAttributeAndMainListEntry();
         list->amountOfAttributesWithMain++;
    }
   
}



////////////////////////////////////////////////////////////////////////////////////////
ListEntryForCommands* newWordsListEntry(){
    ListEntryForCommands* element = malloc(sizeof(ListEntryForCommands));
    element->command.name = NULL;
    element->command.attributesWithMain = getAttributeAndMainList();
    appendNewAttributeAndMainPairForElement(element->command.attributesWithMain);
    element->next = NULL;
    return element;
}

ListOfCommands* getCommandsList(){
    ListOfCommands* list = malloc(sizeof(ListOfCommands));
    list->amountOfCommands = 0;
    list->first = list->last = list->currentPosition = NULL;
    return list;
}
//append command
void appendNewCommandInList(Text* text){
    ListOfCommands* list = text->conveyorParts->last->commands;
    if (list->first) { list->last = list->last->next  =  newWordsListEntry();}
    else{ list->last = list->first  = list->currentPosition = newWordsListEntry();list->amountOfCommands = 0;}
    list->amountOfCommands++;
}




/////////////////////////////////////////////////////////////////////////////////////////////////////////

void appendSymbolForName(Text* text,char* symbol){
    if((*symbol < 'A' || *symbol > 'Z') &&
       (*symbol < 'a' || *symbol > 'z' )) return;
    ListEntryForCommands* lastWord = text->conveyorParts->last->commands->last;
    if (!lastWord->command.name) lastWord->command.name = getSymbolsList();
    appendNewSymbolInList(lastWord->command.name,*symbol);
}
void appendSymbolForAttribute(Text* text,char* symbol){
    ListEntryForCommands* lastWord = text->conveyorParts->last->commands->last;
    ListOfAttributesWithArguments* listPairOfAttributeWithArgument = lastWord->command.attributesWithMain;
    if (listPairOfAttributeWithArgument->last->arguments) appendNewAttributeAndMainPair(text);
    if (!(listPairOfAttributeWithArgument->last->attributes)){
        lastWord->command.attributesWithMain->amountOfAttributesWithMain++;
        listPairOfAttributeWithArgument->last->attributes = getSymbolsList();
    }
    appendNewSymbolInList(listPairOfAttributeWithArgument->last->attributes,*symbol);
}
void appendSymbolForArgument(Text* text,char* symbol){
    ListEntryForCommands* lastWord = text->conveyorParts->last->commands->last;
    ListEntryForAttributesWithMain* lastPairOfAttributeWithArgument = lastWord->command.attributesWithMain->last;
    if (!(lastPairOfAttributeWithArgument->arguments)){
         lastWord->command.attributesWithMain->amountOfAttributesWithMain++;
         lastPairOfAttributeWithArgument->arguments = getSymbolsList();
    }
    appendNewSymbolInList(lastPairOfAttributeWithArgument->arguments,*symbol);
}
void appendSymbolForArgumentWithSpace(Text* text,char* symbol){
    appendNewAttributeAndMainPair(text);
    ListEntryForCommands* lastWord = text->conveyorParts->last->commands->last;
    lastWord->command.attributesWithMain->last->arguments = getSymbolsList();
    ListEntryForAttributesWithMain* lastPairOfAttributeWithArgument = lastWord->command.attributesWithMain->last;
    if (!(lastPairOfAttributeWithArgument->arguments)) lastWord->command.attributesWithMain->amountOfAttributesWithMain++;
    appendNewSymbolInList(lastPairOfAttributeWithArgument->arguments,*symbol);
}
void appendSymbolForAttributeWithSpace(Text* text,char* symbol){
    ListEntryForCommands* lastWord = text->conveyorParts->last->commands->last;
    ListEntryForAttributesWithMain* lastPairOfAttributeWithArgument = lastWord->command.attributesWithMain->last;
    if (!(lastPairOfAttributeWithArgument->arguments)) lastWord->command.attributesWithMain->amountOfAttributesWithMain++;
    appendNewSymbolInList(lastPairOfAttributeWithArgument->attributes,*symbol);
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

void errorTextAppending(Text* text){
    text->conveyorParts = getConveyorPartsList();
    appendNewConveyorPart(text);
    appendNewCommandInList(text);
    printf("Error!\n");
    startString();
}





void createFile(FileName** file){
    *file = malloc(sizeof(FileName));
}
bool emptyAttributeWithArgument(ListOfAttributesWithArguments* list){
    if(list == NULL || list->last == NULL) return true;
    return (list->last->arguments == NULL && list->last->attributes == NULL);
}
bool emptyAttributeWithArgumentInLastCommand(Text* text){
    return emptyAttributeWithArgument(text->conveyorParts->last->commands->last->command.attributesWithMain);
}

