#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <cstring>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <ifaddrs.h>
#include <netdb.h>
#include <stdio.h>

//#include <conio.h>
// #include <netinet/in.h>
// #include <vector>
// #include <boost/function.hpp>
#include <string>

#include <errno.h>

#ifdef __cplusplus__
  #include <cstdlib>
#else
  #include <stdlib.h>
#endif


bool listen(const double);
void rx(const uint8_t *msg, const uint16_t msg_len);
int setupNetwork(const std::string &interface);

class ReflexHandState
{
public:
  static const int NUM_FINGERS = 3;
  static const int NUM_TACTILE = NUM_FINGERS * 9 + 11;
  uint32_t systime_us_;
  uint16_t tactile_pressures_[NUM_TACTILE];
  uint16_t tactile_temperatures_[NUM_TACTILE];
  uint16_t encoders_[NUM_FINGERS];
  uint8_t  dynamixel_error_states_[4];
  uint16_t dynamixel_angles_[4];
  uint16_t dynamixel_speeds_[4];
  uint16_t dynamixel_loads_[4];
  uint8_t  dynamixel_voltages_[4];
  uint8_t  dynamixel_temperatures_[4];
  ReflexHandState();
  ~ReflexHandState();
};
ReflexHandState::ReflexHandState()
{
}
ReflexHandState::~ReflexHandState()
{
}

typedef struct
{
  uint8_t  header[4];
  uint32_t systime;
  uint16_t tactile_pressures[ReflexHandState::NUM_TACTILE];
  uint16_t tactile_temperatures[ReflexHandState::NUM_TACTILE];
  uint16_t encoders[ReflexHandState::NUM_FINGERS];
  uint8_t  dynamixel_error_states[4];
  uint16_t dynamixel_angles[4];
  uint16_t dynamixel_speeds[4];
  uint16_t dynamixel_loads[4];
  uint8_t  dynamixel_voltages[4];
  uint8_t  dynamixel_temperatures[4];
} __attribute__((packed)) mcu_state_format_1_t;

const static int PORT_BASE = 11333;
int tx_sock_ = socket(AF_INET, SOCK_DGRAM, 0);
int rx_sock_ = socket(AF_INET, SOCK_DGRAM, 0);
sockaddr_in mcast_addr_;
ReflexHandState rx_state_;

int main()
{
  std::string network_interface;
  network_interface = "eth1";
  printf("Starting network interface %s.\n", network_interface.c_str());
  setupNetwork(network_interface);
  uint8_t messages[202] = {1};
  printf("%f\n", (double)sizeof(messages));
  while(1)
  {
    listen(0.001);
    // sleep(1);
    // printf("Info:\n");
    // printf("    dynamixel_angles: ");
    // for (int i = 0; i < 3; i++)
    // {
    //   printf("%d, ", rx_state_.dynamixel_angles_[i]);
    // }
    // printf("%d\n", rx_state_.dynamixel_angles_[3]);
    // printf("    encoders: ");
    // for (int i = 0; i < 2; i++)
    // {
    //   printf("%d, ", rx_state_.encoders_[i]);
    // }
    // printf("%d\n", rx_state_.encoders_[3]);

    // printf("\n");
  }
  return 0;
}


bool listen(const double max_seconds)
{
  static uint8_t rxbuf[2000] = {0};
  fd_set rdset;
  FD_ZERO(&rdset);
  FD_SET(rx_sock_, &rdset);
  timeval timeout;
  timeout.tv_sec = (time_t)(max_seconds);
  timeout.tv_usec = (suseconds_t)((max_seconds - timeout.tv_sec) * 1e6);
  int rv = select(rx_sock_ + 1, &rdset, NULL, NULL, &timeout);
  //printf("File: %d\n", rv); // check or uncheck for debugging
  //clrscr();
  if (rv > 0 && FD_ISSET(rx_sock_, &rdset))
  {
    int nbytes = recvfrom(rx_sock_, rxbuf, sizeof(rxbuf), 0, NULL, NULL);
    // printf("Received: %d bytes: ", nbytes); // check or uncheck for debugging
    rx(rxbuf, nbytes);
    //if (system("CLS")) system("clear");
    printf("Info:\n");
    printf("    dynamixel_angles: ");
    for (int i = 0; i < 3; i++)
    {
      printf("%d, ", rx_state_.dynamixel_angles_[i]);
    }
    printf("%d\n", rx_state_.dynamixel_angles_[3]);
    
    printf("    encoders: ");
    for (int i = 0; i < 2; i++)
    {
      printf("%d, ", rx_state_.encoders_[i]);
    }
    printf("%d\n", rx_state_.encoders_[2]);

    printf("    takktile pressures:\n");
    printf("        f1)");
    for (int i = 0; i < 9 - 1; i++)
    {
    	printf("%d, ", rx_state_.tactile_pressures_[i]);
    }
    printf("%d\n", rx_state_.tactile_pressures_[8]);
    printf("        f2)");
    for (int i = 9; i < 18 - 1; i++)
    {
    	printf("%d, ", rx_state_.tactile_pressures_[i]);
    }
    printf("%d\n", rx_state_.tactile_pressures_[17]);
    printf("        f3)");
    for (int i = 18; i < 27 - 1; i++)
    {
    	printf("%d, ", rx_state_.tactile_pressures_[i]);
    }
    printf("%d\n", rx_state_.tactile_pressures_[26]);
    printf("        palm)");
	for (int i = 27; i < 38 - 1; i++)
    {
    	printf("%d, ", rx_state_.tactile_pressures_[i]);
    }
    printf("%d\n", rx_state_.tactile_pressures_[37]);

    // for (int i = 0; i < nbytes; ++i)
    // {
    //   printf("%x ", rxbuf[i]);
    //   // printf("%c", (char) rxbuf[i]);
    // }
    //printf("\n\n\n\n\n\n\n\n\n\n\n\n");
    usleep(100);

    
  }
  return true;
}

void rx(const uint8_t *msg, const uint16_t msg_len)
{
  // first, check the packet format "magic byte" and the length
  if (msg[0] != 1)
  {
    printf("unexpected magic byte received on UDP multicast port: 0x%02x.\n", msg[0]);
    return;
  }
  if (msg_len != sizeof(mcu_state_format_1_t))  // The leftover palm data adds 44 bytes
  {
    printf("expected packet length %d, but saw %d instead.\n", (int)sizeof(mcu_state_format_1_t), msg_len - 44);
    return;
  }

  mcu_state_format_1_t *rx_state_msg = (mcu_state_format_1_t *)msg;
  rx_state_.systime_us_ = rx_state_msg->systime;

  for (int i = 0; i < ReflexHandState::NUM_FINGERS; i++)
    rx_state_.encoders_[i] = rx_state_msg->encoders[i];
  for (int i = 0; i < ReflexHandState::NUM_TACTILE; i++)
  {
    rx_state_.tactile_pressures_[i]    = rx_state_msg->tactile_pressures[i];
    rx_state_.tactile_temperatures_[i] = rx_state_msg->tactile_temperatures[i];
  }
  for (int i = 0; i < 4; i++)
  {
    rx_state_.dynamixel_error_states_[i] = 
      rx_state_msg->dynamixel_error_states[i];
    rx_state_.dynamixel_angles_[i]   = rx_state_msg->dynamixel_angles[i];
    rx_state_.dynamixel_speeds_[i]   = rx_state_msg->dynamixel_speeds[i];
    rx_state_.dynamixel_loads_[i]    = rx_state_msg->dynamixel_loads[i];
    rx_state_.dynamixel_voltages_[i] = rx_state_msg->dynamixel_voltages[i];
    rx_state_.dynamixel_temperatures_[i] = rx_state_msg->dynamixel_temperatures[i];
  }
}

int setupNetwork(const std::string &interface)
{
  const char *mcast_addr_str = "224.0.0.124"; // parameterize someday !
  tx_sock_ = socket(AF_INET6, SOCK_DGRAM, 0);
  rx_sock_ = socket(AF_INET6, SOCK_DGRAM, 0);

  if(tx_sock_ < 0 || rx_sock_ < 0)
  {
    printf("Couldn't create socket.\n");
    return -1;
  }
  else
  {
    printf("Created socket...\n");
  }
  memset(&mcast_addr_, 0, sizeof(mcast_addr_));
  mcast_addr_.sin_family = AF_INET;
  mcast_addr_.sin_addr.s_addr = inet_addr(mcast_addr_str);

  ifaddrs *ifaddr;
  if (getifaddrs(&ifaddr) == -1)
  {
    printf("Couldn't get ipv6 address of interface %s\n", interface.c_str());
    return -1;
  }
  else
  {
    printf("Found ipv6 address of interface %s.\n", interface.c_str());
  }
  std::string tx_iface_addr;
  bool found_interface = false;
  for (ifaddrs *ifa = ifaddr; ifa; ifa = ifa->ifa_next)
  {
    printf("Interface: name: %s address family: %d\n", ifa->ifa_name, ifa->ifa_addr->sa_family);
    if (!ifa->ifa_addr)
      continue;
    int family = ifa->ifa_addr->sa_family;
    if (family != AF_INET6)
      continue;
    char host[NI_MAXHOST];
    int rt = getnameinfo(ifa->ifa_addr, sizeof(struct sockaddr_in), host, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);
    printf("%s\n", gai_strerror(rt));
    // if (rt)
    //   continue;
    printf("    found address %s on interface %s\n", host, ifa->ifa_name);
    if (std::string(ifa->ifa_name) == interface)
    {
      printf("using %s as the tx interface for IPv4 UDP multicast\n", host);
      tx_iface_addr = host;
      found_interface = true;
      break;
    }
  }
  freeifaddrs(ifaddr);
  int err = errno;
  if (!found_interface)
  {
    printf("Unable to find IPv4 address of interface %s %s. Perhaps it needs to be set to a static address?\n", strerror(err), interface.c_str());
    return -1;
  }

  in_addr local_addr;
  local_addr.s_addr = inet_addr(tx_iface_addr.c_str());
  int result = 0, loopback = 0;
  result = setsockopt(tx_sock_, IPPROTO_IP, IP_MULTICAST_IF, (char *)&local_addr, sizeof(local_addr));

    err = errno;
  if(result < 0)
  {
      printf("couldn't set local interface for udp tx sock, fails with errno: %s\n", strerror(err));
  }
  result = setsockopt(tx_sock_, IPPROTO_IP, IP_MULTICAST_LOOP, &loopback, sizeof(loopback));
  err = errno;
  if(result < 0)
  {
      printf("couldn't turn off outgoing multicast loopback, fails with errno: %s\n", strerror(err));
  }

  /////////////////////////////////////////////////////////////////////
  // set up the rx side of things
  int reuseaddr = 1;
  result = setsockopt(rx_sock_, SOL_SOCKET, SO_REUSEADDR, &reuseaddr, sizeof(reuseaddr));
  err = errno;
  if(result < 0)
  {
    printf("couldn't set SO_REUSEADDR on UDP RX socket, fails with errno: %s\n", strerror(err));
  } 
                 
  sockaddr_in rx_bind_addr;
  memset(&rx_bind_addr, 0, sizeof(rx_bind_addr));
  rx_bind_addr.sin_family = AF_INET;
  rx_bind_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  rx_bind_addr.sin_port = htons(PORT_BASE);
  result = bind(rx_sock_, (sockaddr *)&rx_bind_addr, sizeof(rx_bind_addr));
  err = errno;
  if(result < 0)
  {
    printf("couldn't bind rx socket to port %d, fails with errno: %s\n", PORT_BASE, strerror(err));
  } 
                 
  ip_mreq mreq;
  mreq.imr_multiaddr.s_addr = inet_addr(mcast_addr_str);
  mreq.imr_interface.s_addr = inet_addr(tx_iface_addr.c_str());
  result = setsockopt(rx_sock_, IPPROTO_IP, IP_ADD_MEMBERSHIP, &mreq, sizeof(mreq));
  err = errno;
  if(result < 0)
  {
    printf("couldn't add to multicast group, fails with errno: %s\n", strerror(err));
  }                
  printf("connected!\n\n");
}
