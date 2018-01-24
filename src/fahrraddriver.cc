/**
   \file fahrraddriver.cc
   \brief controller driver for bicycle

   \author Giso Grimm
   \date 2015, 2017

   \section license License (GPL)

   This program is free software; you can redistribute it and/or
   modify it under the terms of the GNU General Public License as
   published by the Free Software Foundation; version 2 of the
   License.

   This program is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
   General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA
   02110-1301, USA.

*/

#define OSC_ADDR "239.255.1.7"
#define OSC_PORT "6978"

#include "osc_helper.h"
#include <SerialStream.h>

class drv_t : public SerialPort, public TASCAR::osc_server_t {
public:
  drv_t(const std::string& ttydev);
  ~drv_t() throw();
  void run();
private:
  int32_t rot;
  int32_t vel;
};

drv_t::drv_t(const std::string& ttydev)
  : SerialPort(ttydev),
    TASCAR::osc_server_t(OSC_ADDR,OSC_PORT),
    rot(0),
    vel(0)
{
  Open(BAUD_9600,CHAR_SIZE_8,PARITY_NONE,STOP_BITS_1,FLOW_CONTROL_NONE);
  set_prefix("/cycledrv");
  add_int("/rot",&rot);
  add_int("/vel",&vel);
  activate();
}

drv_t::~drv_t() throw()
{
  Close();
  deactivate();
}

void drv_t::run()
{
  sleep(3);
  while(true){
    char ctmp[1024];
    sprintf(ctmp,"%d %d\n",rot,std::max(std::min(vel,255),-255));
    Write(std::string(ctmp));
    usleep(100000);
  }
}


int main(int argc, char** argv)
{
  std::string ttydev("/dev/ttyUSB0");
  if( argc > 1 )
    ttydev = argv[1];
  drv_t S(ttydev);
  S.run();
}

/*
 * Local Variables:
 * mode: c++
 * c-basic-offset: 2
 * indent-tabs-mode: nil
 * compile-command: "make -C .."
 * End:
 */
