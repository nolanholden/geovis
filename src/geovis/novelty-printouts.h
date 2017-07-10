#ifndef RCR_GEOVIS_NOVELTYPRINTOUTS_H_
#define RCR_GEOVIS_NOVELTYPRINTOUTS_H_

#if defined(ARDUINO) && ARDUINO >= 100
	#include "arduino.h"
#else
	#include "WProgram.h"
#endif

namespace rcr {
namespace geovis {

inline void print_menu() {
  Serial.println();
  //      ___          ___          ___        ___                   ___     
  //     /\  \        /\  \        /\  \      /\__\        ___      /\  \    
  //    /::\  \      /::\  \      /::\  \    /:/  /       /\  \    /::\  \   
  //   /:/\:\  \    /:/\:\  \    /:/\:\  \  /:/  /        \:\  \  /:/\ \  \  
  //  /:/  \:\  \  /::\~\:\  \  /:/  \:\  \/:/__/  ___    /::\__\_\:\~\ \  \ 
  // /:/__/_\:\__\/:/\:\ \:\__\/:/__/ \:\__\:|  | /\__\__/:/\/__/\ \:\ \ \__\
  // \:\  /\ \/__/\:\~\:\ \/__/\:\  \ /:/  /:|  |/:/  /\/:/  /  \:\ \:\ \/__/
  //  \:\ \:\__\   \:\ \:\__\   \:\  /:/  /|:|__/:/  /\::/__/    \:\ \:\__\  
  //   \:\/:/  /    \:\ \/__/    \:\/:/  /  \::::/__/  \:\__\     \:\/:/  /  
  //    \::/  /      \:\__\       \::/  /    ~~~~       \/__/      \::/  /   
  //     \/__/        \/__/        \/__/                            \/__/    
  Serial.println("       ___          ___          ___        ___                   ___     ");
  Serial.println("      /\\  \\        /\\  \\        /\\  \\      /\\__\\        ___      /\\  \\    ");
  Serial.println("     /::\\  \\      /::\\  \\      /::\\  \\    /:/  /       /\\  \\    /::\\  \\   ");
  Serial.println("    /:/\\:\\  \\    /:/\\:\\  \\    /:/\\:\\  \\  /:/  /        \\:\\  \\  /:/\\ \\  \\  ");
  Serial.println("   /:/  \\:\\  \\  /::\\~\\:\\  \\  /:/  \\:\\  \\/:/__/  ___    /::\\__\\_\\:\\~\\ \\  \\ ");
  Serial.println("  /:/__/_\\:\\__\\/:/\\:\\ \\:\\__\\/:/__/ \\:\\__\\:|  | /\\__\\__/:/\\/__/\\ \\:\\ \\ \\__\\");
  Serial.println("  \\:\\  /\\ \\/__/\\:\\~\\:\\ \\/__/\\:\\  \\ /:/  /:|  |/:/  /\\/:/  /  \\:\\ \\:\\ \\/__/");
  Serial.println("   \\:\\ \\:\\__\\   \\:\\ \\:\\__\\   \\:\\  /:/  /|:|__/:/  /\\::/__/    \\:\\ \\:\\__\\  ");
  Serial.println("    \\:\\/:/  /    \\:\\ \\/__/    \\:\\/:/  /  \\::::/__/  \\:\\__\\     \\:\\/:/  /  ");
  Serial.println("     \\::/  /      \\:\\__\\       \\::/  /    ~~~~       \\/__/      \\::/  /   ");
  Serial.println("      \\/__/        \\/__/        \\/__/                            \\/__/    ");
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
  delay(512);
  Serial.println(".");
  delay(512);
}

} // namespace geovis
} // namespace rcr

#endif // RCR_GEOVIS_NOVELTYPRINTOUTS_H_
