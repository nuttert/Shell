//
//  SymbolHendler.c
//  Мракобес
//
//  Created by vlad on 13.10.2018.
//  Copyright © 2018 vlad. All rights reserved.
//

#include "SymbolHendler.h"
#include "CommandHendler.h"
#include "StructuresForReading.h"
#include "methodsForAddingNewCharacter.h"
#include "pullCommandMethods.h"
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
bool quoteIsOpen(currentConfigurationsForText* configutations);
bool isAttribute(void);
//Symbols
//:::::::::space' ':::::::::quote " :::::::::EndOfCommand \n :::::::::slesh \ :::::::::newAttribute - :::::::::backgroundMode &
//:::::::::space' ':::::::::quote " :::::::::EndOfCommand \n :::::::::slesh \ :::::::::newAttribute - :::::::::backgroundMode &
//:::::::::space' ':::::::::quote " :::::::::EndOfCommand \n :::::::::slesh \ :::::::::newAttribute - :::::::::backgroundMode &
//:::::::::space' ':::::::::quote " :::::::::EndOfCommand \n :::::::::slesh \ :::::::::newAttribute - :::::::::backgroundMode &
//:::::::::space' ':::::::::quote " :::::::::EndOfCommand \n :::::::::slesh \ :::::::::newAttribute - :::::::::backgroundMode &
//:::::::::space' ':::::::::quote " :::::::::EndOfCommand \n :::::::::slesh \ :::::::::newAttribute - :::::::::backgroundMode &
//:::::::::space' ':::::::::quote " :::::::::EndOfCommand \n :::::::::slesh \ :::::::::newAttribute - :::::::::backgroundMode &


ClassifiacationOfSymbols space(currentConfigurationsForText* configurations,Text* text,char* symbol){
    switch (configurations->classification) {
        case name:deleteSpaces();configurations->classification = argument;return nothing;
        case attribute:configurations->classification = argument;break;
        case argument:  if (quoteIsOpen(configurations)) return argument;
                        appendNewAttributeAndMainPair(text); return nothing;
        case redirectionSecondOperandStarted:
            if(configurations->directionOfConveyor == into) redirectionMode(configurations,text,into,symbol);
            else if(configurations->directionOfConveyor == outto) redirectionMode(configurations,text,outto,symbol);
            return conveyorSecondOperandJustEnd;break;
        default:break;
    }
    return nothing;
}
//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ClassifiacationOfSymbols quote(currentConfigurationsForText* configurations){
    switch (configurations->classification) {
        case name:return error;               //error
        case attribute:return error;          //start argument
        case argument:  if (configurations->slashCharacter)
        {configurations->slashCharacter = false;
            return argument;
        }
        else {
            configurations->quotes++;
            return nothing;
        }
        case redirectionSecondOperandStarted:return redirectionSecondOperandStarted;
        default:break;
    }
    return configurations->classification;
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ClassifiacationOfSymbols endOfCommand(currentConfigurationsForText* configurations,Text* text,char* symbol){
    if(configurations->slashCharacter){
        configurations->slashCharacter = false;
        return nothing;
    }
    if(configurations->classification == redirectionSecondOperandStarted){
        if(configurations->directionOfConveyor == into) redirectionMode(configurations,text,into,symbol);
        else if(configurations->directionOfConveyor == outto) redirectionMode(configurations,text,outto,symbol);
        else return error;
    }
    
    if (!quoteIsOpen(configurations)){
        choiceCommandStatement(text,configurations);
        reloadConfigurationsAfterEndString(configurations);
        propertyDeinitializer(text);
        text->conveyorParts = getConveyorPartsList();
        text->statement = classic;
        appendNewConveyorPart(text);
        appendNewCommandInList(text);
        startString();
        printf("\n");
        return nothing;
    }else{
        return error;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ClassifiacationOfSymbols slesh(currentConfigurationsForText* configurations,Text* text,char* symbol){
    switch (configurations->classification) {
        case name:return error;               //error
        case redirectionSecondOperandStarted:return redirectionSecondOperandStarted;
        default:
            configurations->classification = argument;
            if (quoteIsOpen(configurations)){
                if (!(configurations->slashCharacter)){
                    configurations->slashCharacter=true;
                    *symbol = getchar();
                    return checkerSymbol(text,symbol);
                }else{
                    configurations->slashCharacter = false;
                    return argument;
                }
            }else configurations->slashCharacter = true;
            return  nothing;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ClassifiacationOfSymbols newAttribute(currentConfigurationsForText* configurations){
    switch (configurations->classification) {
        case name: return error;               //error
        case redirectionSecondOperandStarted:return redirectionSecondOperandStarted;
        default:configurations->classification = attribute;return attribute;}
}

ClassifiacationOfSymbols backGroundMode(currentConfigurationsForText* configurations,Text* text,char* symbol){
    if (!quoteIsOpen(configurations)){
        text->statement = backgroundMode;
        appendNewConveyorPart(text);
        appendNewCommandInList(text);
        reloadAllConfigurations(configurations);
        deleteSpaces();
        return nothing;
    }else{
        return argument;
    }
}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

ClassifiacationOfSymbols redirectionMode(currentConfigurationsForText* configurations,
                                      Text* text,
                                      DirectionalOfConveyorMode directional,
                                      char* symbol){
    if((configurations->intoRedirectionModeWasClose && directional == into) ||
       (configurations->outRedirectionModeWasClose && directional == outto)||
       configurations->amoutOfOutRedirectionSign > 2 ||
       configurations->amoutOfIntoRedirectionSign > 2) return error;

    if(emptyAttributeWithArgumentInLastCommand(text)&&!configurations->secondArrowWas){
        if(configurations->amoutOfIntoRedirectionSign == 2 &&
           configurations->intoRedirectionModeWasOpen) {configurations->clearFile = false;
            deleteSpaces();configurations->secondArrowWas=true;
            return nothing;}    //////////////////////////////////////////////////////////////////<<
        else
            if(configurations->amoutOfOutRedirectionSign == 2 &&
               configurations->outRedirectionModeWasOpen){configurations->clearFile = false;
                deleteSpaces();configurations->secondArrowWas=true;
                return nothing;}   //////////////////////////////////////////////////////////////////>>
        else
            if(configurations->amoutOfIntoRedirectionSign == 2||configurations->amoutOfOutRedirectionSign == 2)  return error;
    }
    
    
    if(quoteIsOpen(configurations)) return error;
    switch(directional){
        case outto:configurations->directionOfConveyor = outto;
            return treatmentOfOuttoConveyerRedirectional(configurations,
                                                         text,
                                                         symbol,
                                                         &configurations->outRedirectionModeWasClose,
                                                        &configurations->outRedirectionModeWasOpen);break;
        case into:configurations->directionOfConveyor = into;
            return treatmentOfIntoRedirectional(configurations,
                                                        text,
                                                        symbol,
                                                        &configurations->intoRedirectionModeWasClose,
                                                      &configurations->intoRedirectionModeWasOpen);break;
        default:break;
        
    }
    return nothing;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////



ClassifiacationOfSymbols treatmentOfOuttoConveyerRedirectional(currentConfigurationsForText* configurations,
                                                        Text* text,
                                                        char* symbol,
                                                        bool* redirectionModeWasClose,
                                                        bool* redirectionModeWasOpen){
    if(redirectionModeWasClose && *redirectionModeWasClose) return error;
    if(*redirectionModeWasOpen == false) {
        *redirectionModeWasOpen = true;
        configurations->classification = redirectionSecondOperandStarted;
         appendNewAttributeAndMainPair(text);
         deleteSpaces();
        return nothing;
    }
    else{
        *redirectionModeWasClose = true;
        *redirectionModeWasOpen = false;
        configurations->inputFileOperand = pullLastArgument(text);
        if (setDescriptorsForRediractions(text,
                                          configurations->inputFileOperand,
                                          configurations->clearFile,
                                          configurations,
                                          outto) == error) return error;
    }
    return nothing;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

ClassifiacationOfSymbols treatmentOfIntoRedirectional(currentConfigurationsForText* configurations,
                                                        Text* text,
                                                        char* symbol,
                                                        bool* redirectionModeWasClose,
                                                        bool* redirectionModeWasOpen){
    if(redirectionModeWasClose && *redirectionModeWasClose) return error;
    if(*redirectionModeWasOpen == false) {
        char* savePreviousRightOperand = configurations->inputFileOperand;
        configurations->inputFileOperand = pullLastArgument(text);
        if(configurations->inputFileOperand == NULL)configurations->inputFileOperand = savePreviousRightOperand;
        *redirectionModeWasOpen = true;
        configurations->classification = redirectionSecondOperandStarted;
        appendNewAttributeAndMainPair(text);
        deleteSpaces();
        return nothing;
    }
    else{
        *redirectionModeWasClose = true;
        *redirectionModeWasOpen = false;
        if (setDescriptorsForRediractions(text,
                                          configurations->inputFileOperand,
                                          configurations->clearFile,
                                          configurations,
                                          into) == error) return error;
    }
    return nothing;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////

ClassifiacationOfSymbols conveyorMode(Text* text,currentConfigurationsForText* configurations){
    if(quoteIsOpen(configurations) || configurations->slashCharacter)return argument;
    reloadAllConfigurations(configurations);
    text->conveyorParts->last->statement = conveyor;
    appendNewCommandInList(text);
    return nothing;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////





bool quoteIsOpen(currentConfigurationsForText* configutations){
    return (configutations->quotes % 2 == 1);
}




bool isAttribute(){
    char attributeName = getchar();
    char nextSymbolAfterAttribute = getchar();
    ungetc(nextSymbolAfterAttribute, stdin);
    ungetc(attributeName, stdin);
    if(nextSymbolAfterAttribute != ' ') return false;
    return true;
}

void deleteSpaces(){
    char symbol = getchar();
    while(symbol == ' ') {symbol = getchar();}
    ungetc(symbol,stdin);
}

void deleteSpacesInFileName(char* symbol){
    *symbol = getchar();
    while(*symbol == ' ') *symbol = getchar();
}


