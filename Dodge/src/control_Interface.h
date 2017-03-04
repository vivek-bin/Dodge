#ifndef CONTROL_INTERFACE_CLASS
#define CONTROL_INTERFACE_CLASS


class control_Interface{
    static bool ArrowKeys[5];
    static int MouseX;
    static int MouseY;

    static void arrowKeyUp(int key,int x,int y);

    static void arrowKeyDown(int key,int x,int y);

    static void otherKeysUp(unsigned char key,int x,int y);

    static void otherKeysDown(unsigned char key,int x,int y);

    static void setMouse(int x,int y);

public:

    static bool jump();

    static int horizontalCtrl();

    static int verticalCtrl();

    static int getMouseX();

    static int getMouseY();

    static void inputSetup();
};

#endif  //CONTROL_INTERFACE_CLASS
