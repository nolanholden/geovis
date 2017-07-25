# SD Card Write-time Woes

## What is the issue?
Shown below is the behavior of μSD cards to periodically perform internal memory management, causing a slowing of write times. This would become an issue if this behavior locks away (on the same thread) from mission-critical systems such as commanding control surfaces. **SEE CONCLUSION BELOW.**


## Example (from field teseting):
The test data below was taken with the following configuration:
+ Adapter:  `onboard Teensy 3.6 μSD port`
+ Disk:     `SanDisk Ultra 32GB MicroSDHC I (class 10)`
+ Library:  `Arduino “SD” library`

###### Let `ms` represent milliseconds (seconds ⋅ 10<sup>-3</sup>).

```
| System Time [ms] | Time Difference [ms] (backward)
| ---------------- | -------------------- | 
| 965932           | 16                   |
| 965955           | 23                   |
| 965972           | 17                   | 
| 965995           | 23                   | 
| 966017           | 22                   | 
| 966035           | 18                   | 
| 966056           | 21                   | 
| 966073           | 17                   | 
| 966095           | 22                   | 
| 966112           | 17                   | 
| 966132           | 20                   | 
| 966151           | 19                   | # Slow writes begin...
| 966454           | 303                  | # .
| 966702           | 248                  | # .
| 967074           | 372                  | # .
| 967326           | 252                  | # .
| 967702           | 376                  | # .
| 967937           | 235                  | # .
| 968309           | 372                  | # .
| 968563           | 254                  | # .
| 968916           | 353                  | # .
| 969164           | 248                  | # .
| 969826           | 662                  | # .
| 970076           | 250                  | # .
| 970396           | 320                  | # Slow writes discontinue.
| 970405           | 9                    | --> [duration of slow writes: 4254ms]
| 970416           | 11                   | --> [mean write-time: 327ms]
| 970426           | 10                   | 
| 970435           | 9                    | 
| 970445           | 10                   | 
| 970454           | 9                    | 
| 970465           | 11                   | 
| 970474           | 9                    |
-------------------------------------------
```
Above, we see a series of slow writes occuring for roughly 4 seconds, with a mean single write-time of 327 milliseconds.

## Conclusion:
_It is imperative that, unless file writing can be performed in the context of a separate thread (which is not directly supported on embedded ARM processors such as the Teensy 3.6's), a separate microcontroller/computer handles any on-board logging._
