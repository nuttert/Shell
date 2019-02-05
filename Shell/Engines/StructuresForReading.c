//
//  StructuresForReading.c
//  Мракобес
//
//  Created by vlad on 16.10.2018.
//  Copyright © 2018 vlad. All rights reserved.
//

#include "StructuresForReading.h"

#include "SymbolHendler.h"
#include "methodsForAddingNewCharacter.h"
#include "pullCommandMethods.h"
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>



//CLASSIFICATION NEW SYMBOL
ClassifiacationOfSymbols checkerSymbol(Text* text,char* symbol){
    static currentConfigurationsForText configurations;
    static ClassifiacationOfSymbols result = name;
    static bool programHaveStarted = false;
    if (programHaveStarted == false){reloadAllConfigurations(&configurations);programHaveStarted = true;}
    switch (*symbol){
        case '\\':result = slesh(&configurations,text,symbol);break;
        case ' ':result =  space(&configurations,text,symbol);break;
        case '"':result =  quote(&configurations);break;
        case '-':result =  newAttribute(&configurations);break;
        case '&':result = backGroundMode(&configurations,text,symbol);break;
        case '>':configurations.amoutOfOutRedirectionSign++;result =  redirectionMode(&configurations,text,outto,symbol); break;
        case '<':configurations.amoutOfIntoRedirectionSign++;result = redirectionMode(&configurations,text, into,symbol);break;
        case '|':result = conveyorMode(text,&configurations);break;
        case '\n':result = endOfCommand(&configurations,text,symbol);break;
        default:result = configurations.classification; break;
    }
    if(result == error || result == conveyorSecondOperandJustEnd){
        reloadAllConfigurations(&configurations);
        if(result == conveyorSecondOperandJustEnd)
        return conveyorSecondOperandJustEnd;
    }

    
    return result;
}




//Methods of Text;
void append(Text* text,char symbol){
    switch(checkerSymbol(text, &symbol)){
        case name:appendSymbolForName(text,&symbol);break;
        case argument:appendSymbolForArgument(text,&symbol);break;
            
            
        case attribute:appendSymbolForAttribute(text,&symbol);break;
        case spaceInAttribute:appendSymbolForAttributeWithSpace(text,&symbol);break;
        case spaceInArgument:appendSymbolForArgumentWithSpace(text,&symbol);break;
        case error:errorTextAppending(text);break;
        case redirectionSecondOperandStarted:appendSymbolForArgument(text,&symbol);break;
        default:break;
    }
}


//CONFIGURATIONS

void reloadDescriptorsPropertys(currentConfigurationsForText* configurations){
    configurations->descriptors.clearFirstputFile = false;
    configurations->descriptors.clearSecondile = false;
    configurations->intoRedirectionModeWasClose = false;
    configurations->outRedirectionModeWasClose = false;
    configurations->inputFileOperand = NULL;
}
void reloadCommonConfigurations(currentConfigurationsForText* configurations){
    configurations->slashCharacter = false;
    configurations->quotes = 0;
    configurations->outRedirectionModeWasOpen = false;
    configurations->intoRedirectionModeWasOpen = false;
    configurations->secondArrowWas = false;
    configurations->classification = name;
    configurations->directionOfConveyor = haveNoDirection;
    configurations->clearFile = true;
    configurations->amoutOfOutRedirectionSign = 0;
    configurations->amoutOfIntoRedirectionSign = 0;
}

void reloadConfigurationsAfterRedirection(currentConfigurationsForText* configurations){
    deleteSpaces();
    reloadCommonConfigurations(configurations);
}
void reloadConfigurationsAfterEndString(currentConfigurationsForText* configurations){
    configurations->descriptors.firstFile = NULL;
    configurations->descriptors.secondFile = NULL;
    configurations->descriptors.positionOfFirstFile = -1;
    configurations->descriptors.positionOfSecondFile= -1;
    reloadDescriptorsPropertys(configurations);
    reloadCommonConfigurations(configurations);
}
void reloadAllConfigurations(currentConfigurationsForText* configurations){
    reloadDescriptorsPropertys(configurations);
    reloadConfigurationsAfterRedirection(configurations);
}
