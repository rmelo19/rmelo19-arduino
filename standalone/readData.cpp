#include <reflexHand.h>

using namespace reflex_hand;

int main()
{
  std::string network_interface;
  network_interface = "eth1";

  ReflexHand rightHand(network_interface);
  
  while(1)
  {
    rightHand.listen(0.001);
    rightHand.printInfo(DYNAMIXEL_ANGLES);
    rightHand.printInfo(ENCODERS_POSITION);
    rightHand.printInfo(TAKKTILE_PRESSURES);
  }
  return 0;
}
