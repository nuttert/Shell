//
//  StructureForReading.h
//  Мракобес
//
//  Created by vlad on 11.10.2018.
//  Copyright © 2018 vlad. All rights reserved.
//

#ifndef StructureForReading_h
#define StructureForReading_h
#include <stdio.h>
typedef enum bool{
    false = 0, true = 1
}bool;

typedef struct {
    char* fileName;
    bool clearFile;
    int position;
}FileName;


typedef enum Channels{
    channelIsNotPresent = -1
}Channels;
////////////////////////////////////////////////////////////////////////////////////

typedef struct Descriptors{
    char* firstFile;
    unsigned positionOfFirstFile;
    bool clearFirstputFile;
    char* secondFile;
    unsigned positionOfSecondFile;
    bool clearSecondile;
}Descriptors;
////////////////////////////////////////////////////////////////////////////////////
typedef struct savedStdDescriptors{
    int stdOutDescriptor;
    int stdInDescriptor;
}savedStdDescriptors;
////////////////////////////////////////////////////////////////////////////////////
typedef enum StatementOfOutput{
    classic ,conveyor ,backgroundMode
}StatementOfOutput;
////////////////////////////////////////////////////////////////////////////////////
typedef enum DirectionalOfConveyorMode{
    into = 0,outto = 1,haveNoDirection = 2
}DirectionalOfConveyorMode;
////////////////////////////////////////////////////////////////////////////////////
typedef enum typeOfChannel{
    readingChannel = 0,writingChannel = 1
}typeOfChannel;

////////////////////////////////////////////////////////////////////////////////////
typedef enum ClassifiacationOfSymbols{
    error = 0,
    name = 1,
    attribute = 3,
    argument = 4,
    redirectionSecondOperandStarted= 5,
    conveyorSecondOperandJustEnd = 6 ,
    nothing = 7,
    spaceInAttribute = 8,
    spaceInArgument = 9 ,
    fileDescriptorsHaveChanged = 10,
    recheckSymbol = 11
}ClassifiacationOfSymbols;


////////////////////////////////////////////////////////////////////////////////////
typedef struct currentConfigurationsForText{
    bool slashCharacter;
    int quotes;
    int amoutOfOutRedirectionSign;
    int amoutOfIntoRedirectionSign;
    bool argumentWasnt;
    bool outRedirectionModeWasOpen;
    bool outRedirectionModeWasClose;
    bool intoRedirectionModeWasOpen;
    bool intoRedirectionModeWasClose;
    bool secondArrowWas;
    ClassifiacationOfSymbols classification;
    DirectionalOfConveyorMode directionOfConveyor;
    bool clearFile;
    char* inputFileOperand;
    Descriptors descriptors;
}currentConfigurationsForText;
////////////////////////////////////////////////////////////////////////////////////

void reloadAllConfigurations(currentConfigurationsForText* configurations);
void reloadCommonConfigurations(currentConfigurationsForText* configurations);
void reloadConfigurationsAfterEndString(currentConfigurationsForText* configurations);
//LISTS FOR COMMANDS
typedef struct ListEntryForChars ListEntryForChars;
typedef struct ListOfChars ListOfChars;
typedef struct ListOfConveyorPart ListOfConveyorPart;
typedef struct ListEntryForConveyorPart ListEntryForConveyorPart;
typedef struct ListOfCommands ListOfCommands;
typedef struct ListOfAttributesWithMain ListOfAttributesWithArguments;
typedef struct ListEntryForAttributesWithMain ListEntryForAttributesWithMain;
typedef struct Command  Command;
typedef struct ListEntryForCommands ListEntryForCommands;

//STRUCTURE TEXT/////////////////////////////////////
//STRUCTURES COMMANDS/////////////////////////////////////
typedef struct Text{
    ListOfConveyorPart* conveyorParts;
    StatementOfOutput statement;
}Text;
typedef struct ListOfConveyorPart{
    ListEntryForConveyorPart* first,*last,*currentPosition;
    int amountOfConveyorParts;
} ListOfConveyorPart;

typedef struct Command{
    ListOfChars* name;
    ListOfAttributesWithArguments* attributesWithMain;
}Command;

//LISTS FOR COMMANDS
typedef struct ListEntryForConveyorPart{
    ListOfCommands* commands;
    StatementOfOutput statement;
    FileName* inputFile,*outPutFile;
    ListEntryForConveyorPart* next;
    unsigned amountOfCommands;
} ListEntryForConveyorPart;

typedef struct ListOfCommands{
    ListEntryForCommands* first,*last,*currentPosition;
    unsigned amountOfCommands;
}ListOfCommands;

typedef struct ListOfChars{
    ListEntryForChars* first,*last,*currentPosition;
    unsigned lenghtOfWord;
}ListOfChars;

typedef struct ListOfAttributesWithMain{
    ListEntryForAttributesWithMain* first,*last,*currentPosition;
    unsigned amountOfAttributesWithMain;
}ListOfAttributesWithArguments;

////////////////////////////////////////////////////////////////////////////////////


//LIST'S ElEMENTS
typedef struct ListEntryForCommands{
    Command command;
    struct ListEntryForCommands* next;
}ListEntryForCommands;

typedef struct ListEntryForChars{
    char symbol;
    struct ListEntryForChars* next;
}ListEntryForChars;

typedef struct ListEntryForAttributesWithMain{
    ListOfChars* attributes;
    ListOfChars* arguments;
    ListEntryForAttributesWithMain* next,*previous;
}ListEntryForAttributesWithMain;

////////////////////////////////////////////////////////////////////////////////////


void append(struct Text* text,const char symbol);
ClassifiacationOfSymbols checkerSymbol(Text* text,char* symbol);
#endif /* StructureForReading_h */
