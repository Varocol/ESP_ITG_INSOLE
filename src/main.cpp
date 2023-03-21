#include "Hardware.h"
#include "Task.h"
void setup()
{
  if (Hardware_Init())
  {
    Task_Init();
  }
}

void loop()
{
}
