/*
 * Title: PWM_reader_with_pulseIn
 * Author: L. Arrigoni
 * Date: July 2022
 *   
 * -------------------------------------------------------
 * It shows how to righlty undestand 2 PWM signals
 * from an RC receiver using pulseIn command. 
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
#define  channel_1  D6    // pin connected to RC receiver
#define  channel_2  D7    // pin connected to RC receiver

/* variables */
double  channel_1Val = 0;    // channel value 
double  channel_2Val = 0;    // channel value

void setup() {
  //set up the serial monitor and pin mode.
  Serial.begin(9600);
  pinMode(channel_1, INPUT);
  pinMode(channel_2, INPUT);

  //start program
  Serial.println("Start program ...");
  delay(1000);
}

void loop() {
  // reads 5 time to have a "good" value
  for (int j = 0;j < 6; j++)
  {
    channel_1Val = channel_1Val + pulseIn(channel_1, HIGH);  // reading value from receiver
    channel_2Val = channel_2Val + pulseIn(channel_2, HIGH);  // reading value from receiver
    if (j == 5)
    {
      channel_1Val = channel_1Val / 5;
      channel_2Val = channel_2Val / 5;
      break; // I make an average of the values because they are slightly fluctuating
    }
  }

  // Print values
  Serial.println("  ");
  Serial.print("Channel 1= ");
  Serial.print(int(channel_1Val));
  Serial.print(" Channel 2= ");
  Serial.println(int(channel_2Val));
} // end of main loop
