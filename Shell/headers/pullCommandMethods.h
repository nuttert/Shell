//
//  pullCommandMethods.h
//  Мракобес
//
//  Created by vlad on 14.10.2018.
//  Copyright © 2018 vlad. All rights reserved.
//

#ifndef pullCommandMethods_h
#define pullCommandMethods_h


#include "StructuresForReading.h"
char* getLastCommandName(Text* text);
char* pullLastCommandName(Text* text);
char* pullWordOfCommand(ListOfChars** pointerToWord);
char* getWordOfCommand(ListOfChars* word);
void getAttributesOfCertainCommand(ListOfAttributesWithArguments* list,char** copyWord);
char** getWholeCommand(Text* text);
char* getLastArgument(Text* text);
char* pullLastArgument(Text* text);
char** getWholeCommandForConveyorPart(Text* text);
#endif /* pullCommandMethods_h */
