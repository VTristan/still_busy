#include <windows.h>
#include <stdio.h>
#include <stdbool.h>

void checkNumberArguments(int argc, char *argv[])
{
    if (argc != 3)
    {
        printf("ERROR: expected 2 command-line-arguments : [.\\filename, arg1 (name of the window), arg2 (debug mode 0/1)]\n");
        exit(1);
    }
    else
    {
        printf("command-line-arguments :\n-name : \"%s\"\n-debug mode : %s\n", argv[1], argv[2]);
    }
}

void checkWindowIsEnable(HWND hwnd, char *WIN_NAME)
{
    char title[256];
    GetWindowText(hwnd, title, sizeof(title)); // get hwnd window by title
    if (strstr(title, WIN_NAME) != NULL)
    {
        printf("\"%s\" window found!\n", title);
        // SetWindowTextA(hwnd, "busy_app");                   // wait 5 seconds before to launch the program
    }
    else
    {
        printf("ERROR: \"%s\" window not found!\n", title);
        HWND foreground = GetForegroundWindow(); // get the hwnd window in foreground
        GetWindowText(foreground, title, sizeof(title));
        printf("\"%s\" instead!\n", title);
        exit(1);
    }
}

int transformArgToInt(char *argv)
{
    int value = atoi(argv);
    if (value < 0 || value > 1)
    {
        printf("ERROR: with the argument argv[2] : \"%s\"\n", argv);
        printf("expected 0 or 1");

        exit(1);
    }
    return value;
}

int main(int argc, char *argv[])
{

    Sleep(5000); // wait 5 seconds before to launch the program
    checkNumberArguments(argc, argv);

    char *const WIN_NAME = argv[1];
    HWND window = FindWindow(NULL, WIN_NAME); // find hwnd window
    checkWindowIsEnable(window, WIN_NAME);

    boolean isDebug = false;
    if (transformArgToInt(argv[2]) == 1)
    {
        isDebug = true; // debug mode is enable if the second parameter is 1
        printf("debug mode enabled\n");
    }

    LASTINPUTINFO info; // init keyboard input
    info.cbSize = sizeof(LASTINPUTINFO);
    INPUT input[1] = {};
    input[0].type = INPUT_KEYBOARD;
    input[0].ki.wVk = 0x00;                   // according to documentation : this value is not associated with any key
    input[0].ki.dwFlags = KEYEVENTF_SCANCODE; // according to documentation : If specified wVk is ignored

    int seconds = 0;
    printf("running...\n");
    while (true)
    {
        GetLastInputInfo(&info);                         // get last activity on the window in milliseconds
        seconds = (GetTickCount() - info.dwTime) / 1000; // allocate the last activity in seconds
        
        if (isDebug)
        {
            printf("%d s.\n", seconds);
        }

        if (seconds >= 50) // refresh every 50 seconds of inactivity
        {
            ShowWindow(window, SW_MAXIMIZE);                   // put the window in the foreground
            SendInput(ARRAYSIZE(input), input, sizeof(INPUT)); // send a message to keep the window active
            if (isDebug)
            {
                printf("refresh\n");
            }
            seconds = 0; // reset the counter
        }

        Sleep(1000); // wait 1 second
    }
    return 0;
}