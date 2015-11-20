#include <ctime>
#include <iostream>
#include <string>
#include "sigslot.h"

class Sender  {
public:
  sigslot::signal2<std::string, std::time_t> SignalDanger;

  void Panic(){
    SignalDanger("Help!", std::time(0));
  }
};

class Receiver : public sigslot::has_slots<> {
public:
  Receiver(Sender& sender){
    sender.SignalDanger.connect(this, &Receiver::OnDanger);
  }

  void OnDanger(std::string message, std::time_t time){
    if(message == "Help!")
    {
      std::cout << "Call the police" << std::endl;
    }
  }
};

int main(int argc, char** argv)
{
  Sender sender;
  Receiver receiver(sender);
  sender.Panic();
  return 0;
}