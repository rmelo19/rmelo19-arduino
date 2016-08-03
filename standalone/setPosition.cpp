#include <reflexHand.h>

using namespace reflex_hand;

int main()
{
  std::string network_interface;
  network_interface = "eth1";

  ReflexHand rightHand(network_interface);
  
  uint16_t targets[NUM_SERVOS] = {0};

  while(1)
  {
    sleep(1);
    rightHand.listen(0.001);
    for (int i = 0; i < NUM_SERVOS; i++)
    {
      targets[i] = rightHand.rx_state_.dynamixel_angles_[i] + 100;
    }
    rightHand.setServoTargets(targets);

    sleep(1);
    for (int i = 0; i < NUM_SERVOS; i++)
    {
      targets[i] = rightHand.rx_state_.dynamixel_angles_[i] - 100;
    }
    rightHand.setServoTargets(targets);
  }
  return 0;
}
