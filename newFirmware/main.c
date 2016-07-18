#include "config.h"

#include "async_poll.h"
int main()
{
  init();
  
  printf("Starting...\n");
  ledsPattern(OFF, OFF, OFF, ON);
  uint8_t result;
  udelay(10000);
  while(1)
  {
    errorService();
    // udelay(100000);
    result = asyncUpdate();
      
    if (result)
    {
      printf("All done, send via ethernet...\n");
      ethernetService();
    }
    // result = asyncUpdate();
    // if (result == 1)
    // {
        // printf("Result: %d\n", result);
        // udelay(10000);
    // }
  }

  return 0;
}

