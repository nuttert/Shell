//
//  methodsForAddingNewCharacter.h
//  Мракобес
//
//  Created by vlad on 13.10.2018.
//  Copyright © 2018 vlad. All rights reserved.
//

#ifndef methodsForAddingNewCharacter_h
#define methodsForAddingNewCharacter_h

#include <stdio.h>
#include "StructuresForReading.h"
#include "SymbolHendler.h"
#include "CommandHendler.h"
#include "initializerOfStructures.h"
bool emptyAttributeWithArgument(ListOfAttributesWithArguments* list);
bool emptyAttributeWithArgumentInLastCommand(Text* text);
void appendNewAttributeAndMainPair(Text* text);
ListEntryForChars* newListEntry(char symbol);
ListOfChars* getSymbolsList(void);
void appendNewCommandInList(Text* text);
void appendNewSymbolInList(ListOfChars* list, char symbol);
ListEntryForCommands* newWordsListEntry(void);
ListOfCommands* getCommandsList(void);
void appendNewAttributeAndMainPairForElement(ListOfAttributesWithArguments* list);
ListEntryForConveyorPart* newConveyorPartListEntry(void);
ListOfConveyorPart* getConveyorPartsList(void);
void appendNewConveyorPart(Text* text);

void appendSymbolForName(Text* text,char* symbol);
void appendSymbolForAttribute(Text* text,char* symbol);
void appendSymbolForArgument(Text* text,char* symbol);
void appendSymbolForArgumentWithSpace(Text* text,char* symbol);
void appendSymbolForAttributeWithSpace(Text* text,char* symbol);
void errorTextAppending(Text* text);
void createFile(FileName** file);
#endif /* methodsForAddingNewCharacter_h */
