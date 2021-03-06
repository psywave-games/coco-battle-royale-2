#include "header.h"

static signed char new_axis_x = 0;
static signed char new_axis_y = 0;
static bool new_attacking = false;

void InputStep(void)
{
    // movement
    new_axis_y = ((IsKeyDown(KEY_S) || IsKeyDown(KEY_DOWN)) - (IsKeyDown(KEY_W) || IsKeyDown(KEY_UP)));
    new_axis_x = ((IsKeyDown(KEY_D) || IsKeyDown(KEY_RIGHT)) - (IsKeyDown(KEY_A) || IsKeyDown(KEY_LEFT)));

    // attacking
    new_attacking = IsKeyDown(KEY_SPACE);
}

bool InputAttack(void)
{
    return (bool) new_attacking;
}

int InputAxisX(void)
{
    return (int) new_axis_x;
}
    
int InputAxisY(void)
{
    return (int) new_axis_y;
}