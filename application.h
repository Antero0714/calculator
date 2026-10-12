#pragma once

const int STATUS_OK = 0;
const int STATUS_INPUT_ERROR = 4;
const int STATUS_HELP = 5;

struct Task
{
    int first_number;
    int second_number;
    char operation;
    int result;
    int status;
};

void applicationRun(int argc, char *argv[]);
void makeTask(int argc, char *argv[], Task &Task);
void makeCalculate(Task &Task);
void printResult(const Task &Task);
