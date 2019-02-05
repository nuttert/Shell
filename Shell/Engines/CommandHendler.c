//
//  CommandHendler.c
//  Мракобес
//
//  Created by vlad on 13.10.2018.
//  Copyright © 2018 vlad. All rights reserved.
//

#include "CommandHendler.h"
#include "pullCommandMethods.h"
#include "initializerOfStructures.h"
#include "StructuresForReading.h"
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include "methodsForAddingNewCharacter.h"
////////////////////////////////////////////////////////////////////////////////////
//INTERFACE


void newCommandExecution(Text* text,
                           char** commands,
                           int readingCommunicationChannel,
                           int writingCommunicationChannel);
void regenerateDescriptors(void);
bool classicRedirection(Text* text,currentConfigurationsForText* configurations);
bool conveyorCommandHandler(Text* text,
                            int readingCommunicationChannel,
                            int writingCommunicationChannel);
void regenerateDescriptors(void);
void endConveyorRedirection(void);
bool startConveyorRedirection(void);
int executeRedirection(char* fileName,bool clearFile);
savedStdDescriptors stdDescriptors;
bool backgroundCommandHandler(Text* text,currentConfigurationsForText* configurations);
void conveyorModeHandler(Text* text,currentConfigurationsForText* configurations);
bool secondRedirectionInConveyorMode(currentConfigurationsForText* configurations);
void outError(){
    printf("Error!");
}
////////////////////////////////////////////////////////////////////////////////////




////////////////////////////////////////////////////////////////////////////////////
ClassifiacationOfSymbols setDescriptorsForRediractions(Text* text,
                                                       char* fileName,
                                                       bool clearFile,
                                                       currentConfigurationsForText* configurations,
                                                       DirectionalOfConveyorMode directional){
    if(directional == into){
        if(text->conveyorParts->last->inputFile != NULL) return error;
        createFile(&text->conveyorParts->last->inputFile);
        text->conveyorParts->last->inputFile->fileName = fileName;
        text->conveyorParts->last->inputFile->clearFile = clearFile;
        text->conveyorParts->last->inputFile->position = text->conveyorParts->last->commands->amountOfCommands;
        return fileDescriptorsHaveChanged;
    }else if(outto){
        if(text->conveyorParts->last->outPutFile != NULL) return error;
        createFile(&text->conveyorParts->last->outPutFile);
        text->conveyorParts->last->outPutFile->fileName = fileName;
        text->conveyorParts->last->outPutFile->clearFile = clearFile;
        text->conveyorParts->last->outPutFile->position = text->conveyorParts->last->commands->amountOfCommands;
         return fileDescriptorsHaveChanged;
        }else return error;
}


void choiceCommandStatement(Text* text,currentConfigurationsForText* configurations){
    switch (text->statement) {
        case classic:classicCommandHandler(text,configurations);break;
        case backgroundMode:backgroundModeHandler(text,configurations);break;
        default:break;
    }
}
////////////////////////////////////////////////////////////////////////////////////






//CLASSIC HANDLER
//////////////////////////////////////////////////////////////////////////////////////////
int classicCommandHandler(Text* text,currentConfigurationsForText* configurations){
    if(text->conveyorParts->last->statement == conveyor){conveyorModeHandler(text, configurations);return true;}
    char** commands = getWholeCommand(text);
    int status;
    if (commands){
        ListEntryForConveyorPart* currentPosition = text->conveyorParts->currentPosition;
        FileName* inputFile = currentPosition->inputFile;
        FileName* outputFile = currentPosition->outPutFile;
        int inputFileDescriptor = channelIsNotPresent,outputFileDescriptor = channelIsNotPresent;
        if(inputFile)
            inputFileDescriptor = executeRedirection(inputFile->fileName, inputFile->clearFile);
        if(outputFile)
            outputFileDescriptor = executeRedirection(outputFile->fileName, outputFile->clearFile);
        newCommandExecution(text,commands, inputFileDescriptor, outputFileDescriptor);
        while (wait(&status)!=-1);
        char** carriage = commands;
        while(*carriage){
            free(*carriage++);
        }
        free(*carriage);
        return true;
    }
    return false;
}




//BACKGROUND MODE
//////////////////////////////////////////////////////////////////////////////////////////
void backgroundModeHandler(Text* text,currentConfigurationsForText* configurations){
        while(backgroundCommandHandler(text,configurations));
}

bool backgroundCommandHandler(Text* text,currentConfigurationsForText* configurations){
    
    if(text->conveyorParts->currentPosition == NULL) return false;
    if(text->conveyorParts->currentPosition->statement == conveyor){
        conveyorModeHandler(text,configurations);
    }else{
        char** commands = getWholeCommand(text);
        if (commands && *commands){
            ListEntryForConveyorPart* currentPosition = text->conveyorParts->currentPosition;
            FileName* inputFile = currentPosition->inputFile;
            FileName* outputFile = currentPosition->outPutFile;
            int inputFileDescriptor = channelIsNotPresent,outputFileDescriptor = channelIsNotPresent;
            if(inputFile)
             inputFileDescriptor = executeRedirection(inputFile->fileName, inputFile->clearFile);
            if(outputFile)
            outputFileDescriptor = executeRedirection(outputFile->fileName, outputFile->clearFile);
            newCommandExecution(text,commands, inputFileDescriptor, outputFileDescriptor);
            char** carriage = commands;
            while(*carriage){
                free(*carriage++);
            }
            free(*carriage);
        }
    }
    if (text->conveyorParts->currentPosition->amountOfCommands == 0) {
        text->conveyorParts->amountOfConveyorParts--;
        text->conveyorParts->currentPosition = text->conveyorParts->currentPosition->next;}
        
    return true;
}


//CONVEYOR MODE
//////////////////////////////////////////////////////////////////////////////////////////
void conveyorModeHandler(Text* text,currentConfigurationsForText* configurations){
     int status;
    int childPid = fork();
    if (childPid == 0) {
    int amountOfCommands = text->conveyorParts->currentPosition->commands->amountOfCommands;
    int communicationChannels[amountOfCommands+1][2];
    for (int currentChannel = 1; currentChannel < amountOfCommands; currentChannel++) pipe(communicationChannels[currentChannel]);
    
   
    const int firstChannel = 0,lastChannel = amountOfCommands;
    FileName* outPutFile = text->conveyorParts->currentPosition->outPutFile;
    FileName* inputFile = text->conveyorParts->currentPosition->inputFile;
    if(inputFile == NULL )communicationChannels[firstChannel][readingChannel] =  channelIsNotPresent;
    else communicationChannels[firstChannel][readingChannel] = open(inputFile->fileName,O_RDONLY);
    communicationChannels[firstChannel][writingChannel] = channelIsNotPresent;
    if(outPutFile == NULL)communicationChannels[lastChannel][writingChannel] = channelIsNotPresent;
    else {
        FILE* file = NULL;
        int fileDescriptor = open(outPutFile->fileName,O_RDWR|outPutFile->clearFile ? : O_APPEND);
        if(fileDescriptor < 0) {
            file = fopen(outPutFile->fileName, "w+");
            fileDescriptor = open(outPutFile->fileName, O_RDWR, outPutFile->clearFile ? : O_APPEND);
        }
        communicationChannels[lastChannel][writingChannel] = fileDescriptor;
    }
    communicationChannels[lastChannel][readingChannel] = channelIsNotPresent;
    int currentChannel = 1;
   
        int writingCommunicationChannel;
        int readingCommunicationChannel;
        do{
            unsigned previousChannel = currentChannel-1;
            readingCommunicationChannel = communicationChannels[previousChannel][readingChannel];
            writingCommunicationChannel = communicationChannels[currentChannel][writingChannel];
            currentChannel++;
        }while(conveyorCommandHandler(text,
                                      readingCommunicationChannel,
                                      writingCommunicationChannel));
    }else waitpid(childPid,&status, 0);
}

bool conveyorCommandHandler(Text* text,
                            int readingCommunicationChannel,
                            int writingCommunicationChannel){
    char** commands = getWholeCommand(text);
    if (commands){
        newCommandExecution(text, commands,
                              readingCommunicationChannel,
                              writingCommunicationChannel);
        return true;
    }
    return false;
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////






////////////////////////////////////////////////////////////////////////////////////

void newCommandExecution(Text* text,
                           char** commands,
                           int readingCommunicationChannel,
                           int writingCommunicationChannel){
    if(fork() == 0){
        int status;
        char* name = commands[0];
        if(readingCommunicationChannel != channelIsNotPresent){
            if(dup2(readingCommunicationChannel,STDIN_FILENO)<0) {printf("Error changing stdin");return;}
            close(readingCommunicationChannel);
        }
        if(writingCommunicationChannel != channelIsNotPresent){
            stdDescriptors.stdOutDescriptor =  dup(STDOUT_FILENO);
            if(dup2(writingCommunicationChannel,STDOUT_FILENO)<0) {printf("Error changing stdout");return;}
            close(writingCommunicationChannel);
            
        }
        if (fork() == 0){
            execvp(name, commands);
            printf("Command not found!\n");
            exit(1);
            }
        while (wait(&status)!= -1);
        if (text->statement == classic)
            while(wait(&status)!=-1){};
        char** carriage = commands;
        while(*carriage){
            free(*carriage++);
        }
        free(*carriage);
        if(writingCommunicationChannel != channelIsNotPresent) close(STDOUT_FILENO);
        exit(0);
    }
    close(readingCommunicationChannel);
    close(writingCommunicationChannel);
}
////////////////////////////////////////////////////////////////////////////////////





//REDIRECTION

int executeRedirection(char* fileName,bool clearFile){
    FILE* file = NULL;
    if (fileName == NULL) return channelIsNotPresent;
    int fileDescriptor = open(fileName, O_APPEND | O_RDWR);
    if(clearFile ||fileDescriptor < 0) {
        file = fopen(fileName,"wr");
        fileDescriptor = open(fileName, O_RDWR);
    }
    fclose(file);
    return fileDescriptor;
}



void regenerateDescriptors(){
    if(dup2(stdDescriptors.stdOutDescriptor,STDOUT_FILENO)<0){
        printf("ERROR!!!");
    }
    close(stdDescriptors.stdOutDescriptor);
}
/////////////////////////////////////////////////////////////////////////////////////////////////////////


void startString(){
        printf("MacBook-Pro-vlad:~ vlad$$$$$$$:");
}
