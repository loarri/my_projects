/*
 * Title: PWM_reader_with_Interrupts
 * Author: L. Arrigoni
 * Date: July 2022
 * 
 * -------------------------------------------------------
 * It shows how to righlty undestand 2 PWM signals
 * from an RC receiver using interrupt capability. 
 * Tested with STM32F401RE board
 * https://www.st.com/en/microcontrollers-microprocessors/stm32f401re.html
 * -------------------------------------------------------
 * 
 * THIS SOFTWARE IS PROVIDED "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, 
 * INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY 
 * AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL 
 * THE OWNER BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, 
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, 
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; 
 * OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, 
 * WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR 
 * OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF 
 * ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/


/* pins */
#define channel_1 D6 // pin connected to RC receiver
#define channel_2 D7 // pin connected to RC receiver

/* variables */
volatile long StartTimeChannel_1 = 0;
volatile long CurrentTimeChannel_1 = 0;
volatile long PulsesChannel_1 = 0;
int PulseWidthChannel_1 = 0;
volatile long StartTimeChannel_2 = 0;
volatile long CurrentTimeChannel_2 = 0;
volatile long PulsesChannel_2 = 0;
int PulseWidthChannel_2 = 0;

/* routines */
void PulseTimerChannel_1(void);
void PulseTimerChannel_2(void);

void setup() {
  //set up the serial monitor, pin mode, and external interrupt.
  Serial.begin(9600);
  pinMode(channel_1, INPUT_PULLUP);
  pinMode(channel_2, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(channel_1),PulseTimerChannel_1,CHANGE);
  attachInterrupt(digitalPinToInterrupt(channel_2),PulseTimerChannel_2,CHANGE);
  
  // start program
  Serial.println("Start program ...");
  delay(1000);
}

void loop() {
  // pwm signal from RC has a duration of 2000 microseconds
  if (PulsesChannel_1 < 2000){
    PulseWidthChannel_1 = PulsesChannel_1;
  } 
  if (PulsesChannel_2 < 2000){
    PulseWidthChannel_2 = PulsesChannel_2;
  } 

  // Print values
  Serial.print("Channel 1= ");
  Serial.print(PulseWidthChannel_1);
  Serial.print(" Channel 2= ");
  Serial.println(PulseWidthChannel_2); 
} // end of main loop


/* routines */
void PulseTimerChannel_1(void){
  //measure the time between interrupts
  CurrentTimeChannel_1 = micros();
  if (CurrentTimeChannel_1 > StartTimeChannel_1){
    PulsesChannel_1 = CurrentTimeChannel_1 - StartTimeChannel_1;
    StartTimeChannel_1 = CurrentTimeChannel_1;
  }
}

void PulseTimerChannel_2(void){
  //measure the time between interrupts
  CurrentTimeChannel_2 = micros();
  if (CurrentTimeChannel_2 > StartTimeChannel_2){
    PulsesChannel_2 = CurrentTimeChannel_2 - StartTimeChannel_2;
    StartTimeChannel_2 = CurrentTimeChannel_2;
  }
}
