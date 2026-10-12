#include "application.h"
#include <calc_math/math.h>
#include <errno.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

bool parseInt(const char *text, int &result)
{
    char *end = nullptr;
    errno = 0;

    long number = strtol(text, &end, 10);

    if (end == text || *end != '\0')
    {
        return false;
    }

    if (errno == ERANGE || number < INT_MIN || number > INT_MAX)
    {
        return false;
    }

    result = static_cast<int>(number);
    return true;
}

void applicationRun(int argc, char *argv[])
{
    Task task{};
    makeTask(argc, argv, task);
    makeCalculate(task);
    printResult(task);
}

void makeTask(int argc, char *argv[], Task &task)
{
    if (task.status != STATUS_OK)
    {
        return;
    }

    task.status = STATUS_INPUT_ERROR;

    bool first_seen = false;
    bool second_seen = false;
    bool operation_seen = false;

    opterr = 0;

    int option;

    while ((option = getopt(argc, argv, "a:b:o:h")) != -1)
    {
        if (option == 'a')
        {
            if (first_seen || !parseInt(optarg, task.first_number))
            {
                return;
            }

            first_seen = true;
        }

        else if (option == 'b')
        {
            if (second_seen || !parseInt(optarg, task.second_number))
            {
                return;
            }
            second_seen = true;
        }

        else if (option == 'o')
        {
            if (operation_seen || optarg[0] == '\0' || optarg[1] != '\0')
            {
                printf("Only one character\n");
                return;
            }

            char operation = optarg[0];

            if (operation != '+' && operation != '-' && operation != '*' && operation != '/' &&
                operation != '^' && operation != '!')
            {
                return;
            }

            task.operation = operation;
            operation_seen = true;
        }
        else if (option == 'h')
        {
            task.status = STATUS_HELP;
            return;
        }
        else
        {
            return;
        }
    }

    if (!first_seen || !second_seen || !operation_seen || optind < argc)
    {
        return;
    }

    task.status = STATUS_OK;
}

void makeCalculate(Task &task)
{
    if (task.status != STATUS_OK)
    {
        return;
    }

    if (task.operation == '+')
    {
        task.status = calc_math::add(task.first_number, task.second_number, task.result);
    }
    else if (task.operation == '-')
    {
        task.status = calc_math::substract(task.first_number, task.second_number, task.result);
    }
    else if (task.operation == '*')
    {
        task.status = calc_math::multiply(task.first_number, task.second_number, task.result);
    }
    else if (task.operation == '/')
    {
        task.status = calc_math::divide(task.first_number, task.second_number, task.result);
    }
    else if (task.operation == '^')
    {
        task.status = calc_math::power(task.first_number, task.second_number, task.result);
    }
    else if (task.operation == '!')
    {
        task.status = calc_math::factorial(task.first_number, task.result);
    }
    else
    {
        task.status = STATUS_INPUT_ERROR;
    }
}

void printResult(const Task &task)
{
    if (task.status == STATUS_HELP)
    {
        printf("Help:\n"
               "  calculator -a <num1> -b <num2> -o <op>\n"
               "  calculator -h  (show this help)\n"
               "\n"
               "Operations:\n"
               "  + add, - subtract, * multiply, / divide, ^ power, ! factorial\n"
               "\n"
               "Rules:\n"
               "  - For factorial (!), only -a is used. -b is not used.\n"
               "  - Division uses whole numbers (example: 5 / 2 = 2).\n"
               "  - Factorial and power need numbers >= 0.\n"
               "  - Any number to the power of 0 is 1 (including 0^0).\n"
               "\n"
               "Example:\n"
               "  calculator -a 12 -b 5 -o '+'\n");

        return;
    }

    if (task.status == STATUS_INPUT_ERROR)
    {
        fprintf(stderr, "Invalid argument. Use -h.\n");
        return;
    }

    printf("first=%d, second=%d, operation=%c\n", task.first_number, task.second_number,
           task.operation);

    if (task.status == calc_math::OVERFLOW)
    {
        fprintf(stderr, "Overflow\n");
        return;
    }

    if (task.status == calc_math::DIVISION_BY_ZERO)
    {
        fprintf(stderr, "Division by zero\n");
        return;
    }

    if (task.status == calc_math::NEGATIVE_ARGUMENT)
    {
        fprintf(stderr, "Nonnegative for exponent/factorial\n");
    }
    else
    {
        printf("%d\n", task.result);
    }
};
