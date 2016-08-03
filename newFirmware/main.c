#include "config.h"

int main()
{
  init();
  printf("Starting...\n");
  while(1)
  {
    errorService();

    if (asyncUpdate())
    {
      printInfo(HAND_STATE_INFO);
      // printf("All done, send via ethernet...\n");
      ethernetService();
    }    
  }
  return 0;
}
