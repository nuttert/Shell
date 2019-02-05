//
//  initializerOfStructures.c
//  Мракобес
//
//  Created by vlad on 13.10.2018.
//  Copyright © 2018 vlad. All rights reserved.
//

#include "initializerOfStructures.h"
#include <stdlib.h>
#include "SymbolHendler.h"
#include "CommandHendler.h"
#include "methodsForAddingNewCharacter.h"
//Initializer
Text* getTextObject(){
    Text* text = malloc(sizeof(Text));
    text->statement = classic;
    text->conveyorParts = getConveyorPartsList();
    appendNewConveyorPart(text);
    appendNewCommandInList(text);
    return text;
}

//Deinitializers
void listOfCharsDeinitializer(ListOfChars** listOfChars){
    ListEntryForChars* saveNext,*carriage;
    ListOfChars* list = *listOfChars;
    if (list){
        carriage = list->first;
        while(carriage){
            saveNext = carriage->next;
            free(carriage);
            carriage = saveNext;
        }
    }
    free(*listOfChars);
}

void attributeDeinitializer(ListOfAttributesWithArguments** listOfAttribute){
    ListEntryForAttributesWithMain* saveNext,*carriage;
    ListOfAttributesWithArguments* list = *listOfAttribute;
    if (list){
        carriage = list->first;
        while(carriage){
            saveNext = carriage->next;
            listOfCharsDeinitializer(&carriage->arguments);
            listOfCharsDeinitializer(&carriage->attributes);
            free(carriage);
            carriage = saveNext;
        }
    }
    free(*listOfAttribute);
}
void commandDeinitializer(ListOfCommands** listOfCommands){
    ListEntryForCommands* saveNext,*carriage;
    ListOfCommands* list = *listOfCommands;
    if (list){
        carriage = list->first;
        while(carriage){
            saveNext = carriage->next;
            attributeDeinitializer(&carriage->command.attributesWithMain);
            listOfCharsDeinitializer(&carriage->command.name);
            free(carriage);
            carriage = saveNext;
            }
    }
    free(*listOfCommands);
}

void conveyorPartDeinitializer(ListOfConveyorPart** listOfConveyorPart){
    ListEntryForConveyorPart* saveNext,*carriage;
    ListOfConveyorPart* list = *listOfConveyorPart;
    if (list){
        carriage = list->first;
        while(carriage){
            saveNext = carriage->next;
            commandDeinitializer(&carriage->commands);
            free(carriage);
            carriage = saveNext;
        }
    }
    free(*listOfConveyorPart);
}

void propertyDeinitializer(Text* text){
    conveyorPartDeinitializer(&text->conveyorParts);
}









/*
//OUT COMMANDS
void outWord(ListOfChars* list){
    if(list){
    if (list->first != NULL){
        ListEntryForChars* temp = list->first;
        while(temp){
            printf("%c",temp->symbol);
            temp = temp->next;
        }
      }
    }
}
void outText(ListOfCommands* list){
    if (list->first != NULL){
        ListEntryForCommands* tempCommand = list->first;
       
        while(tempCommand){
            ListEntryForAttributesWithMain* tempAttribute = tempCommand->command.attributesWithMain->first;
            printf("\nCommand name: ");
            outWord(tempCommand->command.name);
            while (tempAttribute) {
                if (tempAttribute->attributes) {
                    printf(" Attribute: ");
                    outWord(tempAttribute->attributes);
                }
                if (tempAttribute->arguments) {
                     printf(" Argument: ");
                     outWord(tempAttribute->arguments);
                }
                tempAttribute = tempAttribute->next;
            }
            tempCommand = tempCommand->next;
        }
        printf("\n");
    }
}
*/
