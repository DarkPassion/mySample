#include <ctime>
#include <iostream>
#include <string>
#include "sigslot.h"


using namespace std;

class Sender  {
public:
  sigslot::signal2<std::string, std::time_t> SignalDanger;

  sigslot::signal0<> SignalCallv;

  sigslot::signal1<string> SignalCalls;

  void Panic(){
    SignalDanger("Help!", std::time(0));
  }

  void TCallv() {
    SignalCallv();
  }

  void TCalls() {
    SignalCalls("helloworld");
  }
};

class Receiver : public sigslot::has_slots<> {
public:
  Receiver(Sender& sender){
    sender.SignalDanger.connect(this, &Receiver::OnDanger);
    sender.SignalCallv.connect(this, &Receiver::OnCall);
    sender.SignalCalls.connect(this, &Receiver::onCall2);
  }

  void OnDanger(std::string message, std::time_t time){
    if(message == "Help!")
    {
      std::cout << "Call the police" << std::endl;
    }
  }

  void OnCall() {

    cout << "on call !"<< endl;
  }

  void onCall2(string s) {
    cout << "on call 2!" << s.c_str() <<endl;
  }

};

int main(int argc, char** argv)
{
  Sender sender;
  Receiver receiver(sender);
  sender.Panic();
  sender.TCallv();
  sender.TCalls();
  return 0;
}


