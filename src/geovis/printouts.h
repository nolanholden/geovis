#ifndef RCR_GEOVIS_PRINTOUTS_H_
#define RCR_GEOVIS_PRINTOUTS_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

namespace rcr {
namespace geovis {
namespace printouts {

inline void print_menu() {
  Serial.println();
  /*
        ___          ___          ___        ___                   ___     
       /\  \        /\  \        /\  \      /\__\        ___      /\  \    
      /::\  \      /::\  \      /::\  \    /:/  /       /\  \    /::\  \   
     /:/\:\  \    /:/\:\  \    /:/\:\  \  /:/  /        \:\  \  /:/\ \  \  
    /:/  \:\  \  /::\~\:\  \  /:/  \:\  \/:/__/  ___    /::\__\_\:\~\ \  \ 
   /:/__/_\:\__\/:/\:\ \:\__\/:/__/ \:\__\:|  | /\__\__/:/\/__/\ \:\ \ \__\
   \:\  /\ \/__/\:\~\:\ \/__/\:\  \ /:/  /:|  |/:/  /\/:/  /  \:\ \:\ \/__/
    \:\ \:\__\   \:\ \:\__\   \:\  /:/  /|:|__/:/  /\::/__/    \:\ \:\__\  
     \:\/:/  /    \:\ \/__/    \:\/:/  /  \::::/__/  \:\__\     \:\/:/  /  
      \::/  /      \:\__\       \::/  /    ~~~~       \/__/      \::/  /   
       \/__/        \/__/        \/__/                            \/__/    
   -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-
   _________________________________________________________________________
   Commands:
    'c' - to calibrate Inertial Measurement Unit
    'f' - to fly (begin flight-path interperetation.)
  */
  Serial.println(".......___          ___          ___        ___                   ___     ");
  Serial.println("....../\\  \\        /\\  \\        /\\  \\      /\\__\\        ___      /\\  \\    ");
  Serial.println("...../::\\  \\      /::\\  \\      /::\\  \\    /:/  /       /\\  \\    /::\\  \\   ");
  Serial.println("..../:/\\:\\  \\    /:/\\:\\  \\    /:/\\:\\  \\  /:/  /        \\:\\  \\  /:/\\ \\  \\  ");
  Serial.println(".../:/  \\:\\  \\  /::\\~\\:\\  \\  /:/  \\:\\  \\/:/__/  ___    /::\\__\\_\\:\\~\\ \\  \\ ");
  Serial.println("../:/__/_\\:\\__\\/:/\\:\\ \\:\\__\\/:/__/ \\:\\__\\:|  | /\\__\\__/:/\\/__/\\ \\:\\ \\ \\__\\");
  Serial.println("..\\:\\  /\\ \\/__/\\:\\~\\:\\ \\/__/\\:\\  \\ /:/  /:|  |/:/  /\\/:/  /  \\:\\ \\:\\ \\/__/");
  Serial.println("...\\:\\ \\:\\__\\   \\:\\ \\:\\__\\   \\:\\  /:/  /|:|__/:/  /\\::/__/    \\:\\ \\:\\__\\  ");
  Serial.println("....\\:\\/:/  /    \\:\\ \\/__/    \\:\\/:/  /  \\::::/__/  \\:\\__\\     \\:\\/:/  /  ");
  Serial.println(".....\\::/  /      \\:\\__\\       \\::/  /    ~~~~       \\/__/      \\::/  /   ");
  Serial.println("......\\/__/        \\/__/        \\/__/                            \\/__/    ");
  Serial.println(" -_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-");
  Serial.println(" _________________________________________________________________________");
  Serial.println(" Commands:");
  Serial.println("  'c' - to calibrate Inertial Measurement Unit.");
  Serial.println("  'r' - to reboot.");
  Serial.println("  'f' - to fly. (begin flight-path interperetation)");
  Serial.print  (" > ");
 }

// Print message with a delayed, trailing ellipses.
template <typename T>
inline void print_with_ellipses(T message) {
  Serial.print(message);
  delay(512);
  Serial.print(".");
  delay(512);
  Serial.print(".");
  delay(512);
  Serial.println(".");
}


} // namespace printouts
} // namespace geovis
} // namespace rcr

#endif // RCR_GEOVIS_PRINTOUTS_H_
