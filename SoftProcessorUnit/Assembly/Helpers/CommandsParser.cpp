//
//  CommandsParser.cpp
//  SPUAsm
//
//  Created by Александр Дремов on 12.10.2020.
//

#include <cstring>
#include <cstdio>
#include <cctype>
#include "CommandsParser.hpp"
#include "AssemblyHelpers.hpp"
#include "Syntax.hpp"
#include "CommandsDTypes.hpp"
#include "SPUDtypes.hpp"


char *getSourceFileData(FILE *inputFile, size_t *length) {
    fseek(inputFile, 0, SEEK_END);
    *length = (size_t) ftell(inputFile);
    fseek(inputFile, 0, SEEK_SET);
    
    char *buffer = (char *) calloc(*length + 2, sizeof(char));
    if (buffer) {
        fread(buffer, 1, *length, inputFile);
        buffer[*length] = '\0';
    } else {
        return nullptr;
    }
    
    return buffer;
}

void removeDoubleWhitespaces(char *code, size_t *length) {
    for (size_t i = 0; i + 1 < *length;) {
        if (code[i] == ' ' && (code[i + 1] == ' ' || code[i - 1] == ' ')) {
            for (size_t j = i; j < *length; j++) {
                code[j] = code[j + 1];
            }
            (*length)--;
        } else {
            i++;
        }
    }
}

void preprocessSource(char *code, size_t *length) {
    removeDoubleWhitespaces(code, length);
    
    char *commentPos = strchr(code, ';');
    while (commentPos != nullptr) {
        char *commentPosNext = strchr(commentPos, '\n');
        if (commentPosNext == nullptr) {
            commentPosNext = code + *length - 1;
        }
        size_t emptyLength = 0;
        char *mover = commentPosNext;
        char *commentPosTmp = commentPos;
        while (mover != code + *length) {
            emptyLength++;
            *(commentPosTmp++) = *(mover++);
        }
        *(commentPosTmp) = '\0';
        *length = strlen(code);
        commentPos = strchr(commentPos + 1, ';');
    }
    
    for (size_t i = 0; i + 1 < *length;) {
        if (code[i] == '\n' && (code[i + 1] == '\n' || code[(i > 0) ? i - 1 : i] == '\n')) {
            for (size_t j = i; j < *length; j++) {
                code[j] = code[j + 1];
            }
            (*length)--;
        } else {
            i++;
        }
    }
    removeWhitespacesInTheEnd(code, length);
    removeWhitespacesInTheBeginning(code, length);
}

const SyntaxEntity *fetchCommand(const SyntaxMapping *mapping, char *codeBlock) {
    char *command = (char *) calloc(strlen(codeBlock) + 1, sizeof(char));
    strcpy(command, codeBlock);
    char *firstWhitespace = strchr(command, ' ');
    if (firstWhitespace != nullptr) {
        *firstWhitespace = '\0';
    } else {
        firstWhitespace = strchr(command, '\n');
        if (firstWhitespace != nullptr) {
            *firstWhitespace = '\0';
        }
    }
    
    const SyntaxEntity *foundCommand = getSyntaxEntityByName(mapping, (const char *) command);
    
    free(command);
    return foundCommand;
}

const char **getArgList(char *codeBlock, int *argc) {
    return (const char **) getArgList(codeBlock, argc, nullptr);
}

const char **getArgList(char *codeBlock, int *argc, int *argLens) {
    *argc = 1;
    char **args = (char **) calloc(SPU_CMD_MAXARGS, sizeof(char *));
    char *firstWhitespace = strchr(codeBlock, ' ');
    
    size_t maxLen = strlen(codeBlock);
    
    args[0] = (char *) calloc(maxLen, sizeof(char));
    sscanf(codeBlock, "%s", args[0]);
    
    if (firstWhitespace == nullptr)
        return (const char **) args;
    
    int argumentsAvailable = 1;
    
    firstWhitespace = strchr(codeBlock, ' ');
    while (firstWhitespace != nullptr) {
        if (!codeBlockEmpty(firstWhitespace)) {
            args[argumentsAvailable] = (char *) calloc(maxLen, sizeof(char));
            sscanf(firstWhitespace + 1, "%s", args[argumentsAvailable]);
            argumentsAvailable++;
        }
        firstWhitespace = strchr(firstWhitespace + 1, ' ');
    }
    
    *argc = argumentsAvailable;
    
    if (argLens != nullptr) {
        for (int i = 0; i < argumentsAvailable; i++) {
            argLens[i] = (int) strlen(args[i]);
        }
    }
    
    return (const char **) args;
}

void freeArgList(char **argv, int argc) {
    for (int i = 0; i < argc; i++) {
        free(argv[i]);
    }
    free(argv);
}


int isValidArgumentsNumber(const SyntaxEntity *mapping, int argc) {
    const char *formatPtr = mapping->format;
    
    int maxPossible = 0;
    int argumentsTotal = argc;
    int argumentsAvailable = argc;
    while (*formatPtr != '\0') {
        if (*formatPtr == '*' && argumentsAvailable <= 0)
            return 0;
        argumentsAvailable--;
        formatPtr++;
        maxPossible++;
    }
    
    if (argumentsTotal > maxPossible)
        return 0;
    
    return 1;
}

CommandParseResult
parseCommand(AssemblyParams *compileParams, const SyntaxMapping *mapping, BinaryFile *binary, char *codeBlock) {
    char *newlinePos = strchr(codeBlock, '\n');
    if (newlinePos != nullptr) {
        *newlinePos = '\0';
    }
    
    char *commPos = strchr(codeBlock, ';');
    if (commPos != nullptr) {
        *commPos = '\0';
    }
    
    const SyntaxEntity *foundEntity = fetchCommand(mapping, codeBlock);
    
    if (foundEntity == nullptr) {
        LabelParse resLabel = setLabelPos(compileParams, codeBlock, (unsigned int) binary->currentSize);
        if (resLabel == SPU_LABEL_NOTFOUND) {
            printf("%s: error: assembly: unknown instruction '%s' found\n", compileParams->inputFileName,
                   codeBlock);
            
            fprintf(compileParams->lstFile, "error: assembly: unknown instruction '%s' found\n", codeBlock);
            return SPU_UNKNOWN_COMMAND;
        } else {
            switch (resLabel) {
                case SPU_LABEL_DUBLICATE:
                case SPU_LABEL_OK:
                case SPU_LABEL_NOTFOUND: {
                    break;
                }
                case SPU_LABEL_INVALID: {
                    printf("%s: error: assembly: invalid label name '%s' found\n", compileParams->inputFileName,
                           codeBlock);
                    fprintf(compileParams->lstFile, "error: assembly: invalid label name '%s' found\n", codeBlock);
                    return SPU_PARSE_LABEL_INVALID;
                }
            }
            if (compileParams->lstFile != nullptr) {
                fprintf(compileParams->lstFile, "%-5s -> %5s | ", "x", "x");
                fprintf(compileParams->lstFile, "%-3d | ", 0);
                fprintf(compileParams->lstFile, "%-25s | ", codeBlock);
                fprintf(compileParams->lstFile, "\n");
            }
        }
    } else {
        
        int argc = 0;
        const char **argv = getArgList(codeBlock, &argc);
        
        int validArguments = isValidArgumentsNumber(foundEntity, argc - 1);
        
        if (validArguments == 0) {
            if (compileParams->verbose) {
                printf("%s: error: assembly: wrong instruction '%s' found. "
                       "Arguments number is not valid. "
                       "Valid format: '%s'\n", compileParams->inputFileName, codeBlock, foundEntity->format);
            }
            fprintf(compileParams->lstFile, "error: assembly: wrong instruction '%s' found. "
                    "Arguments number is not valid. "
                    "Valid format: '%s'\n", codeBlock, foundEntity->format);
            freeArgList((char **) argv, argc);
            return SPU_CMD_WRONG_ARGUMENTS;
        }
        
        CommandToBytesResult parseRes = foundEntity->cProcessor(foundEntity, compileParams, binary, argc, argv);
        
        switch (parseRes) {
            case SPU_CTB_ERROR: {
                printf("%s: error: assembly: general syntax error\n", compileParams->inputFileName);
                fprintf(compileParams->lstFile, "error: assembly: general syntax error\n");
                break;
            }
            case SPU_CTB_UNKNOWN_REGISTER: {
                printf("%s: error: assembly: unknown register\n", compileParams->inputFileName);
                fprintf(compileParams->lstFile, "error: assembly: unknown register\n");
                break;
            }
            case SPU_CTB_INVALID_NUMBER: {
                printf("%s error: assembly: invalid number of arguments\n", compileParams->inputFileName);
                fprintf(compileParams->lstFile, "error: assembly: invalid number of arguments\n");
                break;
            }
            case SPU_CTB_OK: {
                break;
            }
            case SPU_CTB_INVALID_ARGSTRUCTURE: {
                printf("%s error: assembly: invalid arguments structure\n", compileParams->inputFileName);
                fprintf(compileParams->lstFile, "error: assembly: invalid number of arguments\n");
                break;
            }
            case SPU_CTB_NONASSIGNABLE: {
                printf("%s error: assembly: invalid argument is not assignable\n", compileParams->inputFileName);
                fprintf(compileParams->lstFile, "error: assembly: invalid argument is not assignable\n");
                break;
            }
        }
        
        if (parseRes != SPU_CTB_OK) {
            return SPU_FINALPARSE_ERROR;
        }
        freeArgList((char **) argv, argc);
    }
    
    if (newlinePos != nullptr) {
        *newlinePos = '\n';
    }
    if (commPos != nullptr) {
        *commPos = ';';
    }
    
    return SPU_PARSE_OK;
}

int codeBlockEmpty(char *codeBlock) {
    if (*codeBlock == ';' || *(codeBlock + 1) == ';')
        return 1;
    while (*codeBlock != '\0' && *codeBlock != '\n') {
        if (*codeBlock != ' ' && *codeBlock != '\n' && *codeBlock != '\t' && *codeBlock != '\r')
            return 0;
        codeBlock++;
    }
    return 1;
}

CommandParseResult
parseCode(AssemblyParams *compileParams, const SyntaxMapping *mapping, BinaryFile *binary, char *code, size_t length) {
    char *lastBlockPos = code;
    
    size_t instrUct = 0;
    while (lastBlockPos != nullptr && lastBlockPos > (char *)(1)) {
        if (!codeBlockEmpty(lastBlockPos)) {
            CommandParseResult res = parseCommand(compileParams, mapping, binary, lastBlockPos);
            instrUct++;
            if (res != SPU_PARSE_OK) {
                if (compileParams->verbose) {
                    printf("%s: error: assembly: failed to parse instruction no. %zu '%s'\n",
                           compileParams->inputFileName, instrUct, lastBlockPos);
                }
                fprintf(compileParams->lstFile, "error: assembly: failed to parse instruction no. %zu '%s'\n", instrUct,
                        lastBlockPos);
                return res;
            }
        }
        
        lastBlockPos = ((char *) memchr(lastBlockPos, '\n', (size_t) length - (size_t) (lastBlockPos - code))) + 1;
    }
    
    
    return SPU_PARSE_OK;
}

int registerNoFromName(char *name) {
    char *whPos = strchr(name, ' ');
    if (whPos != nullptr)
        *whPos = '\0';
    int result = -1;
    if (strcmp("rax", name) == 0) {
        result = 0;
    } else if (strcmp("rbx", name) == 0) {
        result = 1;
    } else if (strcmp("rcx", name) == 0) {
        result = 2;
    } else if (strcmp("rdx", name) == 0) {
        result = 3;
    } else if (strcmp("rex", name) == 0) {
        result = 4;
    } else if (strcmp("rfx", name) == 0) {
        result = 5;
    } else if (strcmp("rgx", name) == 0) {
        result = 6;
    } else if (strcmp("rhx", name) == 0) {
        result = 7;
    }
    if (whPos != nullptr)
        *whPos = ' ';
    return result;
}

const char *registerNameFromNo(int no) {
    switch (no) {
        case 0:
            return "rax";
        case 1:
            return "rbx";
        case 2:
            return "rcx";
        case 3:
            return "rdx";
        case 4:
            return "rex";
        case 5:
            return "rfx";
        case 6:
            return "rgx";
        case 7:
            return "rhx";
        default:
            return "<UNKNOWN REGISTER>";
    }
}


void codeEstimations(BinaryFile *binary, char *code) {
    size_t stackInitSize = 0;
    char *p = strstr(code, "push");
    while (p != nullptr) {
        stackInitSize++;
        p = strstr(p + 1, "push");
    }
    binary->stackSize = stackInitSize;
}

void removeWhitespacesInTheEnd(char *code, size_t *length) {
    char *newLinePos = strchr(code, '\n');
    while (newLinePos != nullptr) {
        char *whPos = newLinePos;
        if (whPos != code) {
            whPos -= 1;
            char *mover = whPos + 1;
            if (*whPos == ' ') {
                while (mover <= code + *length) {
                    *(mover - 1) = *(mover);
                    mover++;
                }
                
                *length -= 1;
            }
        }
        newLinePos = strchr(newLinePos + 1, '\n');
    }
}

void removeWhitespacesInTheBeginning(char *code, size_t *length){
    char *newLinePos = code;
    while (newLinePos >= (char*)2) {
        char* startPos = newLinePos;
        char* moverPos = newLinePos;
        while (*moverPos == ' ' && *moverPos != '\0' && *moverPos != '\n') {
            moverPos++;
        }
        if (startPos != moverPos){
            while (moverPos <= code + *length) {
                *(startPos++) = *(moverPos++);
            }
            
            *(startPos) = '\0';
        }
        
        newLinePos = strchr(newLinePos + 1, '\n') + 1;
    }
}


LabelParse parseLabel(AssemblyParams *compileParams, char *code) {
    char *colonFound = strchr(code, ':');
    
    if (colonFound == nullptr)
        return SPU_LABEL_NOTFOUND;
    *colonFound = '\0';
    
    char *checker = code;
    while (checker < colonFound) {
        if (!(isalpha(*checker) || isdigit(*checker) || *checker == '_'))
            return SPU_LABEL_INVALID;
        checker++;
    }
    
    try {
        compileParams->labelsStore->newLabel(code);
    } catch(const char* e) {
        *colonFound = ':';
        return SPU_LABEL_DUBLICATE;
    }
    *colonFound = ':';
    return SPU_LABEL_OK;
}

LabelParse setLabelPos(AssemblyParams *compileParams, char *code, unsigned int pos) {
    char *colonFound = strchr(code, ':');
    
    if (colonFound == nullptr)
        return SPU_LABEL_NOTFOUND;
    *colonFound = '\0';
    
    char *checker = code;
    while (checker < colonFound) {
        if (!(isalpha(*checker) || isdigit(*checker) || *checker == '_'))
            return SPU_LABEL_INVALID;
        checker++;
    }
    
    try {
        compileParams->labelsStore->setLabelToPosition(code, pos);
    } catch(const char* e) {
        *colonFound = ':';
        return SPU_LABEL_DUBLICATE;
    }
    *colonFound = ':';
    return SPU_LABEL_OK;
}

LabelParse evaluateLabels(AssemblyParams *compileParams, BinaryFile *binary) {
    LabelParse completeTable = labelsTableComplete(compileParams, 0);
    if (completeTable != SPU_LABEL_OK) {
        return completeTable;
    }
    JMPLabel *current = compileParams->labelsStore->first;
    
    while (current != nullptr) {
        int offset = (int) (current->positionTo - current->positionFrom + 1);
        memcpy(binary->code + current->positionFrom, &offset, sizeof(offset));
        current = current->next;
    }
    
    return SPU_LABEL_OK;
}


LabelParse labelsTableComplete(AssemblyParams *compileParams, int quiet) {
    JMPLabel *current = compileParams->labelsStore->first;
    
    while (current != nullptr) {
        if (current->positionTo == -1) {
            if (quiet != 1)
                printf("%s: error: assembly: no label specified for '%s' jump\n", compileParams->inputFileName,
                       current->name);
            if (compileParams->lstFile != nullptr)
                fprintf(compileParams->lstFile, "%s: error: assembly: no label specified for '%s' jump\n",
                        compileParams->inputFileName, current->name);
            return SPU_LABEL_NOTFOUND;
        }
        
        if (current->positionFrom == -1) {
            if (quiet != 1)
                printf("%s: warning: assembly: label '%s' has no jump instructions\n", compileParams->inputFileName,
                       current->name);
            if (compileParams->lstFile != nullptr)
                fprintf(compileParams->lstFile, "%s: warning: assembly: label '%s' has no jump instructions\n",
                        compileParams->inputFileName, current->name);
//            return SPU_LABEL_INVALID;
        }
        
        if (current->used > 1) {
            if (quiet != 1)
                printf("%s: error: assembly: label '%s' has several definitions\n", compileParams->inputFileName,
                       current->name);
            if (compileParams->lstFile != nullptr)
                fprintf(compileParams->lstFile, "%s: error: assembly: label '%s' has several definitions\n",
                        compileParams->inputFileName, current->name);
            return SPU_LABEL_DUBLICATE;
        }
        
        current = current->next;
    }
    
    return SPU_LABEL_OK;
}
